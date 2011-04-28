#ifndef _WIN32_REPLACE_H
#define _WIN32_REPLACE_H

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

/* Map BSD Socket errorcodes to the WSA errorcodes (if possible) */ 

#define EAFNOSUPPORT	WSAEAFNOSUPPORT
#define ECONNREFUSED    WSAECONNREFUSED 
#define EINPROGRESS	WSAEINPROGRESS
#define EMSGSIZE	WSAEMSGSIZE 
#define ENOBUFS         WSAENOBUFS
#define ENOTSOCK	WSAENOTSOCK
#define ENETUNREACH	WSAENETUNREACH
#define ENOPROTOOPT	WSAENOPROTOOPT
#define ENOTCONN	WSAENOTCONN 
#define ENOTSUP		134 

/* We undefine the following constants due to conflicts with the w32api headers
 * and the Windows Platform SDK/DDK.
 */

#undef interface

#undef ERROR_INVALID_PARAMETER
#undef ERROR_INSUFFICIENT_BUFFER
#undef ERROR_INVALID_DATATYPE

#undef FILE_GENERIC_READ
#undef FILE_GENERIC_WRITE
#undef FILE_GENERIC_EXECUTE
#undef FILE_ATTRIBUTE_READONLY
#undef FILE_ATTRIBUTE_HIDDEN
#undef FILE_ATTRIBUTE_SYSTEM
#undef FILE_ATTRIBUTE_DIRECTORY
#undef FILE_ATTRIBUTE_ARCHIVE
#undef FILE_ATTRIBUTE_DEVICE
#undef FILE_ATTRIBUTE_NORMAL
#undef FILE_ATTRIBUTE_TEMPORARY
#undef FILE_ATTRIBUTE_REPARSE_POINT
#undef FILE_ATTRIBUTE_COMPRESSED
#undef FILE_ATTRIBUTE_OFFLINE
#undef FILE_ATTRIBUTE_ENCRYPTED
#undef FILE_FLAG_WRITE_THROUGH
#undef FILE_FLAG_NO_BUFFERING
#undef FILE_FLAG_RANDOM_ACCESS
#undef FILE_FLAG_SEQUENTIAL_SCAN
#undef FILE_FLAG_DELETE_ON_CLOSE
#undef FILE_FLAG_BACKUP_SEMANTICS
#undef FILE_FLAG_POSIX_SEMANTICS
#undef FILE_TYPE_DISK
#undef FILE_TYPE_UNKNOWN
#undef FILE_CASE_SENSITIVE_SEARCH
#undef FILE_CASE_PRESERVED_NAMES
#undef FILE_UNICODE_ON_DISK
#undef FILE_PERSISTENT_ACLS
#undef FILE_FILE_COMPRESSION
#undef FILE_VOLUME_QUOTAS
#undef FILE_VOLUME_IS_COMPRESSED
#undef FILE_NOTIFY_CHANGE_FILE_NAME
#undef FILE_NOTIFY_CHANGE_DIR_NAME
#undef FILE_NOTIFY_CHANGE_ATTRIBUTES
#undef FILE_NOTIFY_CHANGE_SIZE
#undef FILE_NOTIFY_CHANGE_LAST_WRITE
#undef FILE_NOTIFY_CHANGE_LAST_ACCESS
#undef FILE_NOTIFY_CHANGE_CREATION
#undef FILE_NOTIFY_CHANGE_EA
#undef FILE_NOTIFY_CHANGE_SECURITY
#undef FILE_NOTIFY_CHANGE_STREAM_NAME
#undef FILE_NOTIFY_CHANGE_STREAM_SIZE
#undef FILE_NOTIFY_CHANGE_STREAM_WRITE
#undef FILE_NOTIFY_CHANGE_NAME

#undef PRINTER_ATTRIBUTE_QUEUED
#undef PRINTER_ATTRIBUTE_DIRECT
#undef PRINTER_ATTRIBUTE_DEFAULT
#undef PRINTER_ATTRIBUTE_SHARED
#undef PRINTER_ATTRIBUTE_NETWORK
#undef PRINTER_ATTRIBUTE_HIDDEN
#undef PRINTER_ATTRIBUTE_LOCAL
#undef PRINTER_ATTRIBUTE_ENABLE_DEVQ
#undef PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS
#undef PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST
#undef PRINTER_ATTRIBUTE_WORK_OFFLINE
#undef PRINTER_ATTRIBUTE_ENABLE_BIDI
#undef PRINTER_ATTRIBUTE_RAW_ONLY
#undef PRINTER_ATTRIBUTE_PUBLISHED
#undef PRINTER_ENUM_DEFAULT
#undef PRINTER_ENUM_LOCAL
#undef PRINTER_ENUM_CONNECTIONS
#undef PRINTER_ENUM_FAVORITE
#undef PRINTER_ENUM_NAME
#undef PRINTER_ENUM_REMOTE
#undef PRINTER_ENUM_SHARED
#undef PRINTER_ENUM_NETWORK
#undef PRINTER_ENUM_EXPAND
#undef PRINTER_ENUM_CONTAINER
#undef PRINTER_ENUM_ICON1
#undef PRINTER_ENUM_ICON2
#undef PRINTER_ENUM_ICON3
#undef PRINTER_ENUM_ICON4
#undef PRINTER_ENUM_ICON5
#undef PRINTER_ENUM_ICON6
#undef PRINTER_ENUM_ICON7
#undef PRINTER_ENUM_ICON8
#undef PRINTER_STATUS_PAUSED
#undef PRINTER_STATUS_ERROR
#undef PRINTER_STATUS_PENDING_DELETION
#undef PRINTER_STATUS_PAPER_JAM
#undef PRINTER_STATUS_PAPER_OUT
#undef PRINTER_STATUS_MANUAL_FEED
#undef PRINTER_STATUS_PAPER_PROBLEM
#undef PRINTER_STATUS_OFFLINE
#undef PRINTER_STATUS_IO_ACTIVE
#undef PRINTER_STATUS_BUSY
#undef PRINTER_STATUS_PRINTING
#undef PRINTER_STATUS_OUTPUT_BIN_FULL
#undef PRINTER_STATUS_NOT_AVAILABLE
#undef PRINTER_STATUS_WAITING
#undef PRINTER_STATUS_PROCESSING
#undef PRINTER_STATUS_INITIALIZING
#undef PRINTER_STATUS_WARMING_UP
#undef PRINTER_STATUS_TONER_LOW
#undef PRINTER_STATUS_NO_TONER
#undef PRINTER_STATUS_PAGE_PUNT
#undef PRINTER_STATUS_USER_INTERVENTION
#undef PRINTER_STATUS_OUT_OF_MEMORY
#undef PRINTER_STATUS_DOOR_OPEN
#undef PRINTER_STATUS_SERVER_UNKNOWN
#undef PRINTER_STATUS_POWER_SAVE

#undef DWORD
#undef HKEY_CLASSES_ROOT
#undef HKEY_CURRENT_USER
#undef HKEY_LOCAL_MACHINE
#undef HKEY_USERS
#undef HKEY_PERFORMANCE_DATA
#undef HKEY_CURRENT_CONFIG
#undef HKEY_DYN_DATA
#undef REG_DWORD
#undef REG_QWORD

#undef SERVICE_STATE_ALL

#undef SE_GROUP_MANDATORY
#undef SE_GROUP_ENABLED_BY_DEFAULT
#undef SE_GROUP_ENABLED

#endif /* _WIN32_REPLACE_H */
