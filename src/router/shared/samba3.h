#ifndef _samba3_h_
#define _samba3_h_
#include <jansson.h>

struct samba3_shareuser {
	char username[64];
	struct samba3_shareuser *next;
};

struct samba3_share {
	char mp[32];
	char label[64];
	char access_perms[4];
	int public;
	struct samba3_shareuser *users;
	struct samba3_share *next;
};

struct samba3_user {
	char username[64];
	char password[64];
	struct samba3_user *next;
}; 

struct samba3_user *getsamba3users(void);
struct samba3_share *getsamba3shares(void);
struct samba3_share *getsamba3share(char *mp, char *label, char *access_perms, int public, struct samba3_shareuser *users);
struct samba3_shareuser *getsamba3shareuser(const char *username);
struct samba3_shareuser *getsamba3shareusers( json_t *users );
struct samba3_user *getsamba3user(char *username, char *password);

#endif
