/*
 *	BIRD Resource Manager
 *
 *	(c) 1998--2000 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#include <stdio.h>
#include <stdlib.h>

#include "nest/bird.h"
#include "lib/resource.h"
#include "lib/string.h"

/**
 * DOC: Resource pools
 *
 * Resource pools (&pool) are just containers holding a list of
 * other resources. Freeing a pool causes all the listed resources
 * to be freed as well. Each existing &resource is linked to some pool
 * except for a root pool which isn't linked anywhere, so all the
 * resources form a tree structure with internal nodes corresponding
 * to pools and leaves being the other resources.
 *
 * Example: Almost all modules of BIRD have their private pool which
 * is freed upon shutdown of the module.
 */

struct pool {
  resource r;
  list inside;
  char *name;
};

static void pool_dump(resource *);
static void pool_free(resource *);
static resource *pool_lookup(resource *, unsigned long);

static struct resclass pool_class = {
  "Pool",
  sizeof(pool),
  pool_free,
  pool_dump,
  pool_lookup
};

pool root_pool;

static int indent;

/**
 * rp_new - create a resource pool
 * @p: parent pool
 * @name: pool name (to be included in debugging dumps)
 *
 * rp_new() creates a new resource pool inside the specified
 * parent pool.
 */
pool *
rp_new(pool *p, char *name)
{
  pool *z = ralloc(p, &pool_class);
  z->name = name;
  init_list(&z->inside);
  return z;
}

static void
pool_free(resource *P)
{
  pool *p = (pool *) P;
  resource *r, *rr;

  r = HEAD(p->inside);
  while (rr = (resource *) r->n.next)
    {
      r->class->free(r);
      xfree(r);
      r = rr;
    }
}

static void
pool_dump(resource *P)
{
  pool *p = (pool *) P;
  resource *r;

  bdebug("%s\n", p->name);
  indent += 3;
  WALK_LIST(r, p->inside)
    rdump(r);
  indent -= 3;
}

static resource *
pool_lookup(resource *P, unsigned long a)
{
  pool *p = (pool *) P;
  resource *r, *q;

  WALK_LIST(r, p->inside)
    if (r->class->lookup && (q = r->class->lookup(r, a)))
      return q;
  return NULL;
}

/**
 * rmove - move a resource
 * @res: resource
 * @p: pool to move the resource to
 *
 * rmove() moves a resource from one pool to another.
 */

void rmove(void *res, pool *p)
{
  resource *r = res;

  if (r)
    {
      if (r->n.next)
        rem_node(&r->n);
      add_tail(&p->inside, &r->n);
    }
}

/**
 * rfree - free a resource
 * @res: resource
 *
 * rfree() frees the given resource and all information associated
 * with it. In case it's a resource pool, it also frees all the objects
 * living inside the pool.
 *
 * It works by calling a class-specific freeing function.
 */
void
rfree(void *res)
{
  resource *r = res;

  if (r)
    {
      if (r->n.next)
	rem_node(&r->n);
      r->class->free(r);
      xfree(r);
    }
}

/**
 * rdump - dump a resource
 * @res: resource
 *
 * This function prints out all available information about the given
 * resource to the debugging output.
 *
 * It works by calling a class-specific dump function.
 */
void
rdump(void *res)
{
  char x[16];
  resource *r = res;

  bsprintf(x, "%%%ds%%08x ", indent);
  bdebug(x, "", (int) r);
  if (r)
    {
      bdebug("%s ", r->class->name);
      r->class->dump(r);
    }
  else
    bdebug("NULL\n");
}

/**
 * ralloc - create a resource
 * @p: pool to create the resource in
 * @c: class of the new resource
 *
 * This function is called by the resource classes to create a new
 * resource of the specified class and link it to the given pool.
 * Size of the resource structure is taken from the @size field
 * of the &resclass.
 */
void *
ralloc(pool *p, struct resclass *c)
{
  resource *r = xmalloc(c->size);

  r->class = c;
  add_tail(&p->inside, &r->n);
  return r;
}

/**
 * rlookup - look up a memory location
 * @a: memory address
 *
 * This function examines all existing resources to see whether
 * the address @a is inside any resource. It's used for debugging
 * purposes only.
 *
 * It works by calling a class-specific lookup function for each
 * resource.
 */
void
rlookup(unsigned long a)
{
  resource *r;

  bdebug("Looking up %08lx\n", a);
  if (r = pool_lookup(&root_pool.r, a))
    rdump(r);
  else
    bdebug("Not found.\n");
}

/**
 * resource_init - initialize the resource manager
 *
 * This function is called during BIRD startup. It initializes
 * all data structures of the resource manager and creates the
 * root pool.
 */
void
resource_init(void)
{
  root_pool.r.class = &pool_class;
  root_pool.name = "Root";
  init_list(&root_pool.inside);
}

/**
 * DOC: Memory blocks
 *
 * Memory blocks are pieces of contiguous allocated memory.
 * They are a bit non-standard since they are represented not by a pointer
 * to &resource, but by a void pointer to the start of data of the
 * memory block. All memory block functions know how to locate the header
 * given the data pointer.
 *
 * Example: All "unique" data structures such as hash tables are allocated
 * as memory blocks.
 */

struct mblock {
  resource r;
  unsigned size;
  byte data[0];
};

static void mbl_free(resource *r UNUSED)
{
}

static void mbl_debug(resource *r)
{
  struct mblock *m = (struct mblock *) r;

  bdebug("(size=%d)\n", m->size);
}

static resource *
mbl_lookup(resource *r, unsigned long a)
{
  struct mblock *m = (struct mblock *) r;

  if ((unsigned long) m->data <= a && (unsigned long) m->data + m->size > a)
    return r;
  return NULL;
}

static struct resclass mb_class = {
  "Memory",
  0,
  mbl_free,
  mbl_debug,
  mbl_lookup
};

/**
 * mb_alloc - allocate a memory block
 * @p: pool
 * @size: size of the block
 *
 * mb_alloc() allocates memory of a given size and creates
 * a memory block resource representing this memory chunk
 * in the pool @p.
 *
 * Please note that mb_alloc() returns a pointer to the memory
 * chunk, not to the resource, hence you have to free it using
 * mb_free(), not rfree().
 */
void *
mb_alloc(pool *p, unsigned size)
{
  struct mblock *b = xmalloc(sizeof(struct mblock) + size);

  b->r.class = &mb_class;
  add_tail(&p->inside, &b->r.n);
  b->size = size;
  return b->data;
}

/**
 * mb_allocz - allocate and clear a memory block
 * @p: pool
 * @size: size of the block
 *
 * mb_allocz() allocates memory of a given size, initializes it to
 * zeroes and creates a memory block resource representing this memory
 * chunk in the pool @p.
 *
 * Please note that mb_alloc() returns a pointer to the memory
 * chunk, not to the resource, hence you have to free it using
 * mb_free(), not rfree().
 */
void *
mb_allocz(pool *p, unsigned size)
{
  void *x = mb_alloc(p, size);
  bzero(x, size);
  return x;
}

/**
 * mb_free - free a memory block
 * @m: memory block
 *
 * mb_free() frees all memory associated with the block @m.
 */
void
mb_free(void *m)
{
  struct mblock *b = SKIP_BACK(struct mblock, data, m);
  rfree(b);
}
