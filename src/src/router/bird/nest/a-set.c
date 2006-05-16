/*
 *	BIRD -- Set/Community-list Operations
 *
 *	(c) 2000 Martin Mares <mj@ucw.cz>
 *	(c) 2000 Pavel Machek <pavel@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#include "nest/bird.h"
#include "nest/route.h"
#include "nest/attrs.h"
#include "lib/resource.h"
#include "lib/string.h"

void
int_set_format(struct adata *set, byte *buf, unsigned int size)
{
  u32 *z = (u32 *) set->data;
  int l = set->length / 4;
  int sp = 1;
  byte *end = buf + size - 16;

  while (l--)
    {
      if (!sp)
	*buf++ = ' ';
      if (buf > end)
	{
	  strcpy(buf, "...");
	  return;
	}
      buf += bsprintf(buf, "(%d,%d)", *z >> 16, *z & 0xffff);
      z++;
      sp = 0;
    }
  *buf = 0;
}

struct adata *
int_set_add(struct linpool *pool, struct adata *list, u32 val)
{
  struct adata *res = lp_alloc(pool, list->length + sizeof(struct adata) + 4);
  res->length = list->length+4;
  * (u32 *) res->data = val;
  memcpy((char *) res->data + 4, list->data, list->length);
  return res;
}

int
int_set_contains(struct adata *list, u32 val)
{
  u32 *l = (u32 *) list->data;
  unsigned int i;
  for (i=0; i<list->length/4; i++)
    if (*l++ == val)
      return 1;
  return 0;
}

struct adata *
int_set_del(struct linpool *pool, struct adata *list, u32 val)
{
  struct adata *res;
  u32 *l, *k;
  unsigned int i;

  if (!int_set_contains(list, val))
    return list;

  res = lp_alloc(pool, list->length + sizeof(struct adata) - 4);
  res->length = list->length-4;

  l = (u32 *) list->data;
  k = (u32 *) res->data;
  for (i=0; i<list->length/4; i++)
    if (l[i] != val)
      *k++ = l[i];

  return res;
}
