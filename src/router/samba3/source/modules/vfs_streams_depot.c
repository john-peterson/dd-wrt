/*
 * Store streams in a separate subdirectory
 *
 * Copyright (C) Volker Lendecke, 2007
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "includes.h"

#undef DBGC_CLASS
#define DBGC_CLASS DBGC_VFS

/*
 * Excerpt from a mail from tridge:
 *
 * Volker, what I'm thinking of is this:
 * /mount-point/.streams/XX/YY/aaaa.bbbb/namedstream1
 * /mount-point/.streams/XX/YY/aaaa.bbbb/namedstream2
 *
 * where XX/YY is a 2 level hash based on the fsid/inode. "aaaa.bbbb"
 * is the fsid/inode. "namedstreamX" is a file named after the stream
 * name.
 */

static uint32_t hash_fn(DATA_BLOB key)
{
	uint32_t value;	/* Used to compute the hash value.  */
	uint32_t i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	for (value = 0x238F13AF * key.length, i=0; i < key.length; i++)
		value = (value + (key.data[i] << (i*5 % 24)));

	return (1103515243 * value + 12345);
}

/*
 * With the hashing scheme based on the inode we need to protect against
 * streams showing up on files with re-used inodes. This can happen if we
 * create a stream directory from within Samba, and a local process or NFS
 * client deletes the file without deleting the streams directory. When the
 * inode is re-used and the stream directory is still around, the streams in
 * there would be show up as belonging to the new file.
 *
 * There are several workarounds for this, probably the easiest one is on
 * systems which have a true birthtime stat element: When the file has a later
 * birthtime than the streams directory, then we have to recreate the
 * directory.
 *
 * The other workaround is to somehow mark the file as generated by Samba with
 * something that a NFS client would not do. The closest one is a special
 * xattr value being set. On systems which do not support xattrs, it might be
 * an option to put in a special ACL entry for a non-existing group.
 */

#define SAMBA_XATTR_MARKER "user.SAMBA_STREAMS"

static bool file_is_valid(vfs_handle_struct *handle, const char *path,
			  bool check_valid)
{
	char buf;

	if (!check_valid) {
		return true;
	}

	DEBUG(10, ("file_is_valid (%s) called\n", path));

	if (SMB_VFS_GETXATTR(handle->conn, path, SAMBA_XATTR_MARKER,
				  &buf, sizeof(buf)) != sizeof(buf)) {
		DEBUG(10, ("GETXATTR failed: %s\n", strerror(errno)));
		return false;
	}

	if (buf != '1') {
		DEBUG(10, ("got wrong buffer content: '%c'\n", buf));
		return false;
	}

	return true;
}

static bool mark_file_valid(vfs_handle_struct *handle, const char *path,
			    bool check_valid)
{
	char buf = '1';
	int ret;

	if (!check_valid) {
		return true;
	}

	DEBUG(10, ("marking file %s as valid\n", path));

	ret = SMB_VFS_SETXATTR(handle->conn, path, SAMBA_XATTR_MARKER,
				    &buf, sizeof(buf), 0);

	if (ret == -1) {
		DEBUG(10, ("SETXATTR failed: %s\n", strerror(errno)));
		return false;
	}

	return true;
}

static char *stream_dir(vfs_handle_struct *handle, const char *base_path,
			const SMB_STRUCT_STAT *base_sbuf, bool create_it)
{
	uint32_t hash;
	char *result = NULL;
	SMB_STRUCT_STAT sbuf;
	uint8_t first, second;
	char *tmp;
	char *id_hex;
	struct file_id id;
	uint8 id_buf[16];
	bool check_valid;
	const char *rootdir;

	check_valid = lp_parm_bool(SNUM(handle->conn),
			"streams_depot", "check_valid", true);

	tmp = talloc_asprintf(talloc_tos(), "%s/.streams", handle->conn->connectpath);

	if (tmp == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	rootdir = lp_parm_const_string(
		SNUM(handle->conn), "streams_depot", "directory",
		tmp);

	if (base_sbuf == NULL) {
		if (SMB_VFS_NEXT_STAT(handle, base_path, &sbuf) == -1) {
			/*
			 * base file is not there
			 */
			goto fail;
		}
		base_sbuf = &sbuf;
	}

	id = SMB_VFS_FILE_ID_CREATE(handle->conn, base_sbuf->st_dev,
				    base_sbuf->st_ino);

	push_file_id_16((char *)id_buf, &id);

	hash = hash_fn(data_blob_const(id_buf, sizeof(id_buf)));

	first = hash & 0xff;
	second = (hash >> 8) & 0xff;

	id_hex = hex_encode(talloc_tos(), id_buf, sizeof(id_buf));

	if (id_hex == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	result = talloc_asprintf(talloc_tos(), "%s/%2.2X/%2.2X/%s", rootdir,
				 first, second, id_hex);

	TALLOC_FREE(id_hex);

	if (result == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	if (SMB_VFS_NEXT_STAT(handle, result, &sbuf) == 0) {
		char *newname;

		if (!S_ISDIR(sbuf.st_mode)) {
			errno = EINVAL;
			goto fail;
		}

		if (file_is_valid(handle, base_path, check_valid)) {
			return result;
		}

		/*
		 * Someone has recreated a file under an existing inode
		 * without deleting the streams directory. For now, just move
		 * it away.
		 */

	again:
		newname = talloc_asprintf(talloc_tos(), "lost-%lu", random());
		if (newname == NULL) {
			errno = ENOMEM;
			goto fail;
		}

		if (SMB_VFS_NEXT_RENAME(handle, result, newname) == -1) {
			if ((errno == EEXIST) || (errno == ENOTEMPTY)) {
				TALLOC_FREE(newname);
				goto again;
			}
			goto fail;
		}

		TALLOC_FREE(newname);
	}

	if (!create_it) {
		errno = ENOENT;
		goto fail;
	}

	if ((SMB_VFS_NEXT_MKDIR(handle, rootdir, 0755) != 0)
	    && (errno != EEXIST)) {
		goto fail;
	}

	tmp = talloc_asprintf(result, "%s/%2.2X", rootdir, first);
	if (tmp == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	if ((SMB_VFS_NEXT_MKDIR(handle, tmp, 0755) != 0)
	    && (errno != EEXIST)) {
		goto fail;
	}

	TALLOC_FREE(tmp);

	tmp = talloc_asprintf(result, "%s/%2.2X/%2.2X", rootdir, first,
			      second);
	if (tmp == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	if ((SMB_VFS_NEXT_MKDIR(handle, tmp, 0755) != 0)
	    && (errno != EEXIST)) {
		goto fail;
	}

	TALLOC_FREE(tmp);

	if ((SMB_VFS_NEXT_MKDIR(handle, result, 0755) != 0)
	    && (errno != EEXIST)) {
		goto fail;
	}

	if (!mark_file_valid(handle, base_path, check_valid)) {
		goto fail;
	}

	return result;

 fail:
	TALLOC_FREE(result);
	return NULL;
}

static char *stream_name(vfs_handle_struct *handle, const char *fname,
			 bool create_dir)
{
	char *base = NULL;
	char *sname = NULL;
	char *id_hex = NULL;
	char *dirname, *stream_fname;

	if (!NT_STATUS_IS_OK(split_ntfs_stream_name(talloc_tos(), fname,
						    &base, &sname))) {
		DEBUG(10, ("split_ntfs_stream_name failed\n"));
		errno = ENOMEM;
		goto fail;
	}

	/* if it's the ::$DATA stream just return the base file name */
	if (!sname) {
		return base;
	}

	dirname = stream_dir(handle, base, NULL, create_dir);

	if (dirname == NULL) {
		goto fail;
	}

	stream_fname = talloc_asprintf(talloc_tos(), "%s/:%s", dirname, sname);

	if (stream_fname == NULL) {
		errno = ENOMEM;
		goto fail;
	}

	DEBUG(10, ("stream filename = %s\n", stream_fname));

	TALLOC_FREE(base);
	TALLOC_FREE(sname);
	TALLOC_FREE(id_hex);

	return stream_fname;

 fail:
	DEBUG(5, ("stream_name failed: %s\n", strerror(errno)));
	TALLOC_FREE(base);
	TALLOC_FREE(sname);
	TALLOC_FREE(id_hex);
	return NULL;
}

static NTSTATUS walk_streams(vfs_handle_struct *handle,
			     const char *fname,
			     const SMB_STRUCT_STAT *sbuf,
			     char **pdirname,
			     bool (*fn)(const char *dirname,
					const char *dirent,
					void *private_data),
			     void *private_data)
{
	char *dirname;
	SMB_STRUCT_DIR *dirhandle = NULL;
	char *dirent;

	dirname = stream_dir(handle, fname, sbuf, false);

	if (dirname == NULL) {
		if (errno == ENOENT) {
			/*
			 * no stream around
			 */
			return NT_STATUS_OK;
		}
		return map_nt_error_from_unix(errno);
	}

	DEBUG(10, ("walk_streams: dirname=%s\n", dirname));

	dirhandle = SMB_VFS_NEXT_OPENDIR(handle, dirname, NULL, 0);

	if (dirhandle == NULL) {
		TALLOC_FREE(dirname);
		return map_nt_error_from_unix(errno);
	}

	while ((dirent = vfs_readdirname(handle->conn, dirhandle)) != NULL) {

		if (ISDOT(dirent) || ISDOTDOT(dirent)) {
			continue;
		}

		DEBUG(10, ("walk_streams: dirent=%s\n", dirent));

		if (!fn(dirname, dirent, private_data)) {
			break;
		}
	}

	SMB_VFS_NEXT_CLOSEDIR(handle, dirhandle);

	if (pdirname != NULL) {
		*pdirname = dirname;
	}
	else {
		TALLOC_FREE(dirname);
	}

	return NT_STATUS_OK;
}

static int streams_depot_stat(vfs_handle_struct *handle, const char *fname,
			      SMB_STRUCT_STAT *sbuf)
{
	char *stream_fname;
	int ret = -1;

	DEBUG(10, ("streams_depot_stat called for [%s]\n", fname));

	if (!is_ntfs_stream_name(fname)) {
		return SMB_VFS_NEXT_STAT(handle, fname, sbuf);
	}

	stream_fname = stream_name(handle, fname, false);
	if (stream_fname == NULL) {
		goto done;
	}

	ret = SMB_VFS_NEXT_STAT(handle, stream_fname, sbuf);

 done:
	TALLOC_FREE(stream_fname);
	return ret;
}

static int streams_depot_lstat(vfs_handle_struct *handle, const char *fname,
			       SMB_STRUCT_STAT *sbuf)
{
	char *stream_fname;
	int ret = -1;

	if (!is_ntfs_stream_name(fname)) {
		return SMB_VFS_NEXT_LSTAT(handle, fname, sbuf);
	}

	stream_fname = stream_name(handle, fname, false);
	if (stream_fname == NULL) {
		goto done;
	}

	ret = SMB_VFS_NEXT_LSTAT(handle, stream_fname, sbuf);

 done:
	TALLOC_FREE(stream_fname);
	return ret;
}

static int streams_depot_open(vfs_handle_struct *handle,  const char *fname,
			      files_struct *fsp, int flags, mode_t mode)
{
	TALLOC_CTX *frame;
	char *base = NULL;
	char *sname = NULL;
	SMB_STRUCT_STAT base_sbuf;
	char *stream_fname;
	int ret = -1;

	if (!is_ntfs_stream_name(fname)) {
		return SMB_VFS_NEXT_OPEN(handle, fname, fsp, flags, mode);
	}

	frame = talloc_stackframe();

	if (!NT_STATUS_IS_OK(split_ntfs_stream_name(talloc_tos(), fname,
						    &base, &sname))) {
		errno = ENOMEM;
		goto done;
	}

	if (!sname) {
		ret = SMB_VFS_NEXT_OPEN(handle, base, fsp, flags, mode);
		goto done;
	}

	ret = SMB_VFS_NEXT_STAT(handle, base, &base_sbuf);

	if (ret == -1) {
		goto done;
	}

	TALLOC_FREE(base);

	stream_fname = stream_name(handle, fname, true);
	if (stream_fname == NULL) {
		goto done;
	}

	ret = SMB_VFS_NEXT_OPEN(handle, stream_fname, fsp, flags, mode);

 done:
	TALLOC_FREE(frame);
	return ret;
}

static int streams_depot_unlink(vfs_handle_struct *handle,  const char *fname)
{
	int ret = -1;
	SMB_STRUCT_STAT sbuf;

	DEBUG(10, ("streams_depot_unlink called for %s\n", fname));

	if (is_ntfs_stream_name(fname)) {
		char *stream_fname;

		stream_fname = stream_name(handle, fname, false);
		if (stream_fname == NULL) {
			return -1;
		}

		ret = SMB_VFS_NEXT_UNLINK(handle, stream_fname);

		TALLOC_FREE(stream_fname);
		return ret;
	}

	/*
	 * We potentially need to delete the per-inode streams directory
	 */

	if (lp_posix_pathnames()) {
		ret = SMB_VFS_NEXT_LSTAT(handle, fname, &sbuf);
	} else {
		ret = SMB_VFS_NEXT_STAT(handle, fname, &sbuf);
	}

	if (ret == -1) {
		return -1;
	}

	if (sbuf.st_nlink == 1) {
		char *dirname = stream_dir(handle, fname, &sbuf, false);

		if (dirname != NULL) {
			SMB_VFS_NEXT_RMDIR(handle, dirname);
		}
		TALLOC_FREE(dirname);
	}

	return SMB_VFS_NEXT_UNLINK(handle, fname);
}

static int streams_depot_rename(vfs_handle_struct *handle,
				const char *oldname,
				const char *newname)
{
	TALLOC_CTX *frame = NULL;
	int ret = -1;
	bool old_is_stream;
	bool new_is_stream;
	char *obase = NULL;
	char *osname = NULL;
	char *nbase = NULL;
	char *nsname = NULL;
	char *ostream_fname = NULL;
	char *nstream_fname = NULL;
	char *newname_full = NULL;

	DEBUG(10, ("streams_depot_rename called for %s => %s\n",
		   oldname, newname));

	old_is_stream = is_ntfs_stream_name(oldname);
	new_is_stream = is_ntfs_stream_name(newname);

	if (!old_is_stream && !new_is_stream) {
		return SMB_VFS_NEXT_RENAME(handle, oldname, newname);
	}

	frame = talloc_stackframe();

	if (!NT_STATUS_IS_OK(split_ntfs_stream_name(talloc_tos(), oldname,
						    &obase, &osname))) {
		errno = ENOMEM;
		goto done;
	}

	if (!NT_STATUS_IS_OK(split_ntfs_stream_name(talloc_tos(), newname,
						    &nbase, &nsname))) {
		errno = ENOMEM;
		goto done;
	}

	/* for now don't allow renames from or to the default stream */
	if (!osname || !nsname) {
		errno = ENOSYS;
		goto done;
	}

	ostream_fname = stream_name(handle, oldname, false);
	if (ostream_fname == NULL) {
		return -1;
	}

	/*
	 * Handle passing in a stream name without the base file.  This is
	 * exercised by the NTRENAME streams rename path.
	 */
	if (StrCaseCmp(nbase, "./") == 0) {
		newname_full = talloc_asprintf(talloc_tos(), "%s:%s", obase,
					       nsname);
		if (newname_full == NULL) {
			errno = ENOMEM;
			goto done;
		}
	}

	nstream_fname = stream_name(handle,
				    newname_full ? newname_full : newname,
				    false);
	if (nstream_fname == NULL) {
		return -1;
	}

	ret = SMB_VFS_NEXT_RENAME(handle, ostream_fname, nstream_fname);

done:
	TALLOC_FREE(frame);
	return ret;
}

static bool add_one_stream(TALLOC_CTX *mem_ctx, unsigned int *num_streams,
			   struct stream_struct **streams,
			   const char *name, SMB_OFF_T size,
			   SMB_OFF_T alloc_size)
{
	struct stream_struct *tmp;

	tmp = TALLOC_REALLOC_ARRAY(mem_ctx, *streams, struct stream_struct,
				   (*num_streams)+1);
	if (tmp == NULL) {
		return false;
	}

	tmp[*num_streams].name = talloc_strdup(tmp, name);
	if (tmp[*num_streams].name == NULL) {
		return false;
	}

	tmp[*num_streams].size = size;
	tmp[*num_streams].alloc_size = alloc_size;

	*streams = tmp;
	*num_streams += 1;
	return true;
}

struct streaminfo_state {
	TALLOC_CTX *mem_ctx;
	vfs_handle_struct *handle;
	unsigned int num_streams;
	struct stream_struct *streams;
	NTSTATUS status;
};

static bool collect_one_stream(const char *dirname,
			       const char *dirent,
			       void *private_data)
{
	struct streaminfo_state *state =
		(struct streaminfo_state *)private_data;
	char *full_sname;
	SMB_STRUCT_STAT sbuf;

	if (asprintf(&full_sname, "%s/%s", dirname, dirent) == -1) {
		state->status = NT_STATUS_NO_MEMORY;
		return false;
	}
	if (SMB_VFS_NEXT_STAT(state->handle, full_sname, &sbuf) == -1) {
		DEBUG(10, ("Could not stat %s: %s\n", full_sname,
			   strerror(errno)));
		SAFE_FREE(full_sname);
		return true;
	}

	SAFE_FREE(full_sname);

	if (!add_one_stream(state->mem_ctx,
			    &state->num_streams, &state->streams,
			    dirent, sbuf.st_size,
			    get_allocation_size(
				    state->handle->conn, NULL, &sbuf))) {
		state->status = NT_STATUS_NO_MEMORY;
		return false;
	}

	return true;
}

static NTSTATUS streams_depot_streaminfo(vfs_handle_struct *handle,
					 struct files_struct *fsp,
					 const char *fname,
					 TALLOC_CTX *mem_ctx,
					 unsigned int *pnum_streams,
					 struct stream_struct **pstreams)
{
	SMB_STRUCT_STAT sbuf;
	int ret;
	NTSTATUS status;
	struct streaminfo_state state;

	if ((fsp != NULL) && (fsp->fh->fd != -1)) {
		if (is_ntfs_stream_name(fsp->fsp_name)) {
			return NT_STATUS_INVALID_PARAMETER;
		}
		ret = SMB_VFS_NEXT_FSTAT(handle, fsp, &sbuf);
	}
	else {
		if (is_ntfs_stream_name(fname)) {
			return NT_STATUS_INVALID_PARAMETER;
		}
		if (lp_posix_pathnames()) {
			ret = SMB_VFS_NEXT_LSTAT(handle, fname, &sbuf);
		} else {
			ret = SMB_VFS_NEXT_STAT(handle, fname, &sbuf);
		}
	}

	if (ret == -1) {
		return map_nt_error_from_unix(errno);
	}

	state.streams = NULL;
	state.num_streams = 0;

	if (!S_ISDIR(sbuf.st_mode)) {
		if (!add_one_stream(mem_ctx,
				    &state.num_streams, &state.streams,
				    "::$DATA", sbuf.st_size,
				    get_allocation_size(handle->conn, fsp,
							&sbuf))) {
			return NT_STATUS_NO_MEMORY;
		}
	}

	state.mem_ctx = mem_ctx;
	state.handle = handle;
	state.status = NT_STATUS_OK;

	status = walk_streams(handle, fname, &sbuf, NULL, collect_one_stream,
			      &state);

	if (!NT_STATUS_IS_OK(status)) {
		TALLOC_FREE(state.streams);
		return status;
	}

	if (!NT_STATUS_IS_OK(state.status)) {
		TALLOC_FREE(state.streams);
		return state.status;
	}

	*pnum_streams = state.num_streams;
	*pstreams = state.streams;
	return NT_STATUS_OK;
}

static uint32_t streams_depot_fs_capabilities(struct vfs_handle_struct *handle)
{
	return SMB_VFS_NEXT_FS_CAPABILITIES(handle) | FILE_NAMED_STREAMS;
}

/* VFS operations structure */

static vfs_op_tuple streams_depot_ops[] = {
	{SMB_VFS_OP(streams_depot_fs_capabilities), SMB_VFS_OP_FS_CAPABILITIES,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_open), SMB_VFS_OP_OPEN,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_stat), SMB_VFS_OP_STAT,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_lstat), SMB_VFS_OP_LSTAT,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_unlink), SMB_VFS_OP_UNLINK,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_rename), SMB_VFS_OP_RENAME,
	 SMB_VFS_LAYER_TRANSPARENT},
	{SMB_VFS_OP(streams_depot_streaminfo), SMB_VFS_OP_STREAMINFO,
	 SMB_VFS_LAYER_OPAQUE},
	{SMB_VFS_OP(NULL), SMB_VFS_OP_NOOP, SMB_VFS_LAYER_NOOP}
};

NTSTATUS vfs_streams_depot_init(void);
NTSTATUS vfs_streams_depot_init(void)
{
	return smb_register_vfs(SMB_VFS_INTERFACE_VERSION, "streams_depot",
				streams_depot_ops);
}
