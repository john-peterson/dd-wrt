/*
 * Dynamic data buffer
 * Copyright (c) 2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef WPABUF_H
#define WPABUF_H

#define WPABUF_DO_NOT_FREE_EXT_DATA 0x00000001

/*
 * Internal data structure for wpabuf. Please do not touch this directly from
 * elsewhere. This is only defined in header file to allow inline functions
 * from this file to access data.
 */
struct wpabuf {
	size_t size; /* total size of the allocated buffer */
	size_t used; /* length of data in the buffer */
	size_t refcount; /* number of users referencing this buffer */
	unsigned int flags; /* WPABUF_* */
	u8 *ext_data; /* pointer to external data; NULL if data follows
		       * struct wpabuf */
	/* optionally followed by the allocated buffer */
};


void wpabuf_overflow(const struct wpabuf *buf, size_t len);
int wpabuf_resize(struct wpabuf **buf, size_t add_len);
struct wpabuf * wpabuf_alloc(size_t len);
struct wpabuf * wpabuf_alloc_ext_data(u8 *data, size_t len);
struct wpabuf * wpabuf_alloc_ext_data_no_free(const u8 *data, size_t len);
struct wpabuf * wpabuf_alloc_copy(const void *data, size_t len);
struct wpabuf * wpabuf_dup(const struct wpabuf *src);
void wpabuf_free(struct wpabuf *buf);


/**
 * wpabuf_size - Get the currently allocated size of a wpabuf buffer
 * @buf: wpabuf buffer
 * Returns: Currently allocated size of the buffer
 */
static inline size_t wpabuf_size(const struct wpabuf *buf)
{
	return buf->size;
}

/**
 * wpabuf_len - Get the current length of a wpabuf buffer data
 * @buf: wpabuf buffer
 * Returns: Currently used length of the buffer
 */
static inline size_t wpabuf_len(const struct wpabuf *buf)
{
	return buf->used;
}

/**
 * wpabuf_head - Get pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static inline const void * wpabuf_head(const struct wpabuf *buf)
{
	if (buf->ext_data)
		return buf->ext_data;
	return buf + 1;
}

static inline const u8 * wpabuf_head_u8(const struct wpabuf *buf)
{
	return wpabuf_head(buf);
}

/**
 * wpabuf_mhead - Get modifiable pointer to the head of the buffer data
 * @buf: wpabuf buffer
 * Returns: Pointer to the head of the buffer data
 */
static inline void * wpabuf_mhead(struct wpabuf *buf)
{
	if (buf->ext_data)
		return buf->ext_data;
	return buf + 1;
}

static inline u8 * wpabuf_mhead_u8(struct wpabuf *buf)
{
	return wpabuf_mhead(buf);
}

static inline struct wpabuf * wpabuf_ref(struct wpabuf *buf)
{
	buf->refcount++;
	return buf;
}

static inline void * wpabuf_put(struct wpabuf *buf, size_t len)
{
	void *tmp = wpabuf_mhead_u8(buf) + wpabuf_len(buf);
	buf->used += len;
	if (buf->used > buf->size)
		wpabuf_overflow(buf, len);
	return tmp;
}

static inline void wpabuf_put_u8(struct wpabuf *buf, u8 data)
{
	u8 *pos = wpabuf_put(buf, 1);
	*pos = data;
}

static inline void wpabuf_put_be16(struct wpabuf *buf, u16 data)
{
	u8 *pos = wpabuf_put(buf, 2);
	WPA_PUT_BE16(pos, data);
}

static inline void wpabuf_put_be24(struct wpabuf *buf, u32 data)
{
	u8 *pos = wpabuf_put(buf, 3);
	WPA_PUT_BE24(pos, data);
}

static inline void wpabuf_put_be32(struct wpabuf *buf, u32 data)
{
	u8 *pos = wpabuf_put(buf, 4);
	WPA_PUT_BE32(pos, data);
}

static inline void wpabuf_put_data(struct wpabuf *buf, const void *data,
				   size_t len)
{
	if (data)
		os_memcpy(wpabuf_put(buf, len), data, len);
}

static inline void wpabuf_put_buf(struct wpabuf *dst,
				  const struct wpabuf *src)
{
	wpabuf_put_data(dst, wpabuf_head(src), wpabuf_len(src));
}

#endif /* WPABUF_H */
