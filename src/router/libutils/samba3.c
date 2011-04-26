#include <samba3.h>


struct samba3_shareuser *getsamba3shareuser(const char *username) {
	
	struct samba3_shareuser *user = calloc(1, sizeof(struct samba3_shareuser));
	
	strcpy(user->username, username);

	return user;
};

struct samba3_shareuser *getsamba3shareusers( json_t *users ) {

	int count, entry_count;
	const char *value;
	struct samba3_shareuser *shareusers, *current;

	entry_count = json_array_size( users );	
	if(entry_count == 0) {
		return NULL;
	}
	for( count = 0; count < entry_count; count++ ) {
		value = json_string_value( json_array_get( users, count ) );
		if( count == 0 ) {
			current = getsamba3shareuser( value );
			shareusers = current;	
		} else {
			current->next = getsamba3shareuser( value );
			current = current->next;
		}	
	}

	return shareusers;
}

struct samba3_user *getsamba3user(char *username, char *password) {
	
	struct samba3_user *user = calloc(1, sizeof(struct samba3_user));
	
	strcpy(user->username, username);
	strcpy(user->password, password);

	return user;
}

struct samba3_user *getsamba3users(void) {
	
	struct samba3_user *list, *current;
	int count, entry_count;
	json_t *json;
    	json_error_t error;
	const char *key;
	json_t *iterator, *entry, *value;
	char username[64],password[64];

	// first create dummy entry
	list = getsamba3user("", "");
	current = list;
	
	//json = json_loads( "[{\"user\":\"peter\",\"pass\":\"test\"},{\"user\":\"chris\",\"pass\":\"test\"}]", &error );
	json = json_loads( nvram_default_get( "samba3_users", "[]"), &error);
	if( !json ) {
		fprintf( stderr, "[JASON] ERROR\n");
	} else {
		entry_count = json_array_size(json);
		for( count = 0; count < entry_count; count++ ) {
			entry = json_array_get( json, count );
			iterator = json_object_iter(entry);

			// reset
			username[0] = 0;
			password[0] = 0;
				
			while(iterator)
			{
				key = json_object_iter_key(iterator);
				value = json_object_iter_value(iterator);
				/* use key and value ... */
				if( !strcmp( key, "user" ) ) {
					strcpy( username, json_string_value( value ));
				} else if( !strcmp( key, "pass" ) ) {
					strcpy( password, json_string_value( value ));
				}
				iterator = json_object_iter_next(entry, iterator);
			}
			if( username[0] != 0 ) {
				current->next = getsamba3user(username, password);
				current = current->next;
			}
		}
	json_array_clear(json);		
	}
		
	return list;
}

struct samba3_share *getsamba3share(char *mp, char *label, char *access_perms, int public, struct samba3_shareuser *users) {
	
	struct samba3_share *share = calloc(1, sizeof(struct samba3_share));
	
	strcpy(share->mp, mp);
	strcpy(share->label, label);
	strcpy(share->access_perms, access_perms);
	share->public = public;
	if(users != NULL) {
		//fprintf(stderr, "[SHAREUSERS] add\n");
		share->users = users;
	}
	
	return share;
}

void refjson(void)
{
    json_dumps( NULL, JSON_COMPACT ) ;
}

struct samba3_share *getsamba3shares(void) {
	
	struct samba3_share *list, *current;
	int public, count, entry_count;
	struct samba3_shareuser *shareusers;
	json_t *json;
    	json_error_t error;
	const char *key;
	json_t *iterator, *entry, *value;
	char mp[64],label[32],access_perms[4];

	// first create dummy entry
	list = getsamba3share("", "", "", 0, NULL);
	current = list;
	
//	json = json_loads( "[{\"mp\":\"/jffs\",\"label\":\"testshare\",\"perms\":\"rw\",\"public\":0},{\"mp\":\"/mnt\",\"label\":\"othertest\",\"perms\":\"ro\",\"public\":1},{\"label\":\"blah\"}]", &error );
	json = json_loads( nvram_default_get( "samba3_shares", "[]"), &error);
	if( !json ) {
		fprintf( stderr, "[JASON] ERROR\n");
	} else {
		entry_count = json_array_size(json);
		for( count = 0; count < entry_count; count++ ) {
			entry = json_array_get( json, count );
			iterator = json_object_iter(entry);

			// reset
			mp[0] = 0;
			label[0] = 0;
			access_perms[0] = 0;
			public = 0;
			shareusers = NULL;
				
			while(iterator)
			{
				key = json_object_iter_key(iterator);
				value = json_object_iter_value(iterator);
				/* use key and value ... */
				if( !strcmp( key, "mp" ) ) {
					strcpy( mp, json_string_value( value ));
				} else if( !strcmp( key, "label" ) ) {
					strcpy( label, json_string_value( value ));
				} else if( !strcmp( key, "perms" ) ) {
					strcpy( access_perms, json_string_value( value ));
				} else if( !strcmp( key, "public") ) {
					public = json_integer_value( value );
				} else if( !strcmp( key, "users" ) ) {
					//fprintf( stderr, "[SAMABA SHARES] users\n" );
					shareusers = getsamba3shareusers( value );
				}
				iterator = json_object_iter_next(entry, iterator);
			}
			if( mp[0] != 0 && label != 0 && access_perms != 0 ) {
				current->next = getsamba3share(mp, label, access_perms, public, shareusers);
				current = current->next;
			}
		}
	json_array_clear(json);		
	}
	return list;
}
