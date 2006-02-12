#include <stdio.h>
#include "../include/ebtables_u.h"

#define NAT_VALID_HOOKS ((1 << NF_BR_PRE_ROUTING) | (1 << NF_BR_LOCAL_OUT) | \
   (1 << NF_BR_POST_ROUTING))

static void print_help(char **hn)
{
	int i;

	printf("Supported chains for the nat table:\n");
	for (i = 0; i < NF_BR_NUMHOOKS; i++)
		if (NAT_VALID_HOOKS & (1 << i))
			printf("%s ", hn[i]);
	printf("\n");
}

static struct
ebt_u_table table =
{
	.name		= "nat",
	.help		= print_help,
};

static void _init(void) __attribute__ ((constructor));
static void _init(void)
{
	register_table(&table);
}
