/*
 * Stubs for some cli functions used by the test routines.
 * $Revision: 92103 $
 */
void ast_cli(int fd, const char *fmt, ...);
void ast_cli(int fd, const char *fmt, ...)
{
}

struct ast_cli_entry;

int ast_cli_register_multiple(struct ast_cli_entry *e, int len);
int ast_cli_register_multiple(struct ast_cli_entry *e, int len)
{
	return 0;
}
