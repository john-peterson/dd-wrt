#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <bcmnvram.h>

typedef struct linkedlist {
	char *name;
	int value;
	int port;
	struct linkedlist *prev;
	struct linkedlist *next;
};

void addEntry(struct linkedlist *list, char *name, char *port, int value)
{
	struct linkedlist *first = list;
	int p = atoi(port);
	if (list == NULL)
		return;
	while (1) {
		if (!strcmp(first->name, name) && first->port == p) {
			first->value += value;
			return;
		} else {
			if (first->next == NULL) {
				struct linkedlist *next =
				    malloc(sizeof(struct linkedlist));
				next->name = strdup(name);
				next->value = value;
				next->port = p;
				first->next = next;
				next->prev = first;
				return;
			}
			first = first->next;
		}
	}
}

void freeList(struct linkedlist *list)
{
	struct linkedlist *first = list->next;
	if (list == NULL)
		return;
	while (1) {
		if (first->next == NULL)
			return;
		struct linkedlist *next = first->next;
		free(first->name);
		free(first);
		first = next;
	}
}

void nextline(FILE * fp)
{
	while (1) {
		int c = getc(fp);
		if (c == 0xa)
			return;
		if (c == EOF)
			return;
	}
}

void getword(FILE * in, char *val)
{
	int c = 0;
	while (1) {
		int v = getc(in);
		if (v == EOF)
			return;
		if (v == 0xa)
			break;
		if (v == 0x20 && c == 0)
			continue;
		if (v == 0x20)
			break;
		val[c++] = v;
	}
	val[c++] = 0;
}

void send_email(struct linkedlist *list, char *source, int value)
{
	static char email_line[4096];
	char *server = nvram_safe_get("warn_server");
	char *from = nvram_safe_get("warn_from");
	char *fromfull = nvram_safe_get("warn_fromfull");
	char *to = nvram_safe_get("warn_to");
	char *domain = nvram_safe_get("warn_domain");

	char *user = nvram_safe_get("warn_user");
	char *pass = nvram_safe_get("warn_pass");
	static char subject[4096];
	sprintf(subject, "user %s reached connection limit\n", source);
	char mess[256];
	int c = sprintf(mess, "ip %s has %d open connections\n", source, value);
	while (1) {
		if (!strcmp(list->name, source)) {
			c += sprintf(&mess[c],
				     "%d open connections on port %d\n",
				     list->value,list->port);
		}
		list = list->next;
		if (list == NULL)
			break;
	}

	if (strlen(user) > 0)
		sprintf(email_line,
			"sendmail -S %s -f %s -F \"%s\" -s \"%s\" -u \"%s\" -p \"%s\"  \"%s\" -m \"%s\" -d \"%s\"",
			server, from, fromfull, subject, user, pass, to,
			mess, domain);
	else
		sprintf(email_line,
			"sendmail -S %s -f %s -F \"%s\" -s \"%s\" \"%s\" -m \"%s\" -d \"%s\"",
			server, from, fromfull, subject, to, mess, domain);

	system(email_line);

}

int main(int argc, char *argv[])
{
	if (!nvram_match("warn_enabled", "1"))
		return 0;
	int nf = 0;
	struct linkedlist list;
	struct linkedlist total;
	list.name = "entry";
	list.value = 0;
	list.next = NULL;
	list.prev = NULL;

	total.name = "entry";
	total.value = 0;
	total.next = NULL;
	total.prev = NULL;
	FILE *fp = fopen("/proc/net/ip_conntrack", "rb");
	if (fp == NULL) {
		fp = fopen("/proc/net/nf_conntrack", "rb");
		nf = 1;
	}
	while (1) {
		unsigned char proto[64];
		unsigned char dummy[64];
		unsigned char dummy2[64];
		unsigned char p2[64];
		unsigned char p3[64];
		unsigned char state[64];
		unsigned char src[64];
		unsigned char dst[64];
		unsigned char sport[64];
		unsigned char dport[64];
		if (nf) {
			getword(fp, dummy);
			getword(fp, dummy2);
			getword(fp, proto);
			getword(fp, p2);
			getword(fp, p3);
			getword(fp, state);
			getword(fp, src);
			getword(fp, dst);
			getword(fp, sport);
			getword(fp, dport);
		} else {
			getword(fp, proto);
			getword(fp, p2);
			getword(fp, p3);
			getword(fp, state);
			getword(fp, src);
			getword(fp, dst);
			getword(fp, sport);
			getword(fp, dport);
		}
		if (feof(fp))
			break;
		if (!strcmp(proto, "tcp")) {
			addEntry(&list, &src[4], &dport[6], 1);
			addEntry(&total, &src[4], "0", 1);
		} else {
			addEntry(&list, &state[4], &src[6], 1);
			addEntry(&total, &state[4], "0", 1);
		}
		nextline(fp);
	}
	fclose(fp);
	struct linkedlist *entry = &total;
	int limit = atoi(nvram_default_get("warn_connlimit", "500"));
	while (1) {
		if (entry->value > limit) {
			send_email(&list, entry->name, entry->value);
		}
		entry = entry->next;
		if (entry == NULL)
			break;
	}
	freeList(&list);
	freeList(&total);
	return 0;
}
