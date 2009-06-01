/*
 * $Id: heap.h,v 1.6 2003/01/23 00:36:47 robertc Exp $
 *
 * AUTHOR: John Dilley, Hewlett Packard
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *  
 */

/****************************************************************************
 * Copyright (C) 1999 by Hewlett Packard
 *
 * Heap data structure.  Used to store objects for cache replacement.  The
 * heap is implemented as a contiguous array in memory.  Heap sort and heap
 * update are done in-place.  The heap is ordered with the smallest value at
 * the top of the heap (as in the smallest object key value).  Child nodes
 * are larger than their parent.
 ****************************************************************************/

#ifndef	SQUID_HEAP_H
#define	SQUID_HEAP_H

/*
 * Function for generating heap keys.  The first argument will typically be
 * a dws_md_p passed in as a void *.  Should find a way to get type safety
 * without having heap know all about metadata objects...  The second arg is
 * the current aging factor for the heap.
 */
typedef unsigned long heap_mutex_t;
typedef void * heap_t;
typedef double heap_key;
typedef heap_key heap_key_func(heap_t, heap_key);


/*
 * Heap node.  Has a key value generated by a key_func, id (array index) so
 * it can be quickly found in its heap, and a pointer to a data object that
 * key_func can generate a key from.
 */
typedef struct _heap_node {
    heap_key key;
    unsigned long id;
    heap_t data;
} heap_node;


/*
 * Heap object.  Holds an array of heap_node objects along with a heap size
 * (array length), the index of the last heap element, and a key generation
 * function.  Also stores aging factor for this heap.
 */
typedef struct _heap {
    heap_mutex_t lock;
    unsigned long size;
    unsigned long last;
    heap_key_func *gen_key;	/* key generator for heap */
    heap_key age;		/* aging factor for heap */
    heap_node **nodes;
} heap;

/****************************************************************************
 * Public functions
 ****************************************************************************/

/* 
 * Create and initialize a new heap.
 */
SQUIDCEXTERN heap *new_heap(int init_size, heap_key_func gen_key);

/* 
 * Delete a heap and clean up its memory.  Does not delete what the heap
 * nodes are pointing to!
 */
SQUIDCEXTERN void delete_heap(heap *);

/*
 * Insert a new node into a heap, returning a pointer to it.  The heap_node
 * object returned is used to update or delete a heap object.  Nothing else
 * should be done with this data structure (especially modifying it!)  The
 * heap does not assume ownership of the data passed to it.
 */
SQUIDCEXTERN heap_node *heap_insert(heap *hp, heap_t dat);

/*
 * Delete a node out of a heap.  Returns the heap data from the deleted
 * node.  The caller is responsible for freeing this data.
 */
SQUIDCEXTERN heap_t heap_delete(heap *, heap_node * elm);

/*
 * The semantics of this routine is the same as the followings:
 *        heap_delete(hp, elm);
 *        heap_insert(hp, dat);
 * Returns the old data object from elm (the one being replaced).  The
 * caller must free this as necessary.
 */
SQUIDCEXTERN heap_t heap_update(heap *, heap_node * elm, heap_t dat);

/* 
 * Generate a heap key for a given data object.  Alternative macro form:
 */
#ifdef	MACRO_DEBUG
SQUIDCEXTERN heap_key heap_gen_key(heap * hp, heap_t dat);
#else
#define	heap_gen_key(hp,md)	((hp)->gen_key((md),(hp)->age))
#endif /* MACRO_DEBUG */


/* 
 * Extract the minimum (root) element and maintain the heap property.
 * Returns the data pointed to by the root node, which the caller must
 * free as necessary.
 */
SQUIDCEXTERN heap_t heap_extractmin(heap *);

/* 
 * Extract the last leaf node (does not change the heap property).
 * Returns the data that had been in the heap which the caller must free if
 * necessary.  Note that the last node is guaranteed to be less than its
 * parent, but may not be less than any of the other (leaf or parent) notes
 * in the tree.  This operation is fast but imprecise.
 */
SQUIDCEXTERN heap_t heap_extractlast(heap * hp);

/* 
 * Get the root key, the nth key, the root (smallest) element, or the nth
 * element.  None of these operations modify the heap.
 */
SQUIDCEXTERN heap_key heap_peepminkey(heap *);
SQUIDCEXTERN heap_key heap_peepkey(heap *, int n);

SQUIDCEXTERN heap_t heap_peepmin(heap *);
SQUIDCEXTERN heap_t heap_peep(heap *, int n);

/* 
 * Is the heap empty?  How many nodes (data objects) are in it? 
 */
#ifdef	MACRO_DEBUG
SQUIDCEXTERN int heap_empty(heap *);
SQUIDCEXTERN int heap_nodes(heap *);
#else /* MACRO_DEBUG */
#define	heap_nodes(heap)	((heap)->last)
#define	heap_empty(heap)	(((heap)->last <= 0) ? 1 : 0)
#endif /* MACRO_DEBUG */

/* 
 * Print the heap or a node in the heap.
 */
SQUIDCEXTERN void heap_print(heap *);
SQUIDCEXTERN void heap_printnode(char *msg, heap_node * elm);

SQUIDCEXTERN int verify_heap_property(heap *);

#endif /* SQUID_HEAP_H */
