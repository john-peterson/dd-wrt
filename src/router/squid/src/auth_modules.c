/* automatically generated by ./auth_modules.sh basic
 * do not edit
 */
#include "squid.h"

extern AUTHSSETUP authSchemeSetup_basic;
void authSchemeSetup(void)
{
	authSchemeAdd("basic", authSchemeSetup_basic);
}
