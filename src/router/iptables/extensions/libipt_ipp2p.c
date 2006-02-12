
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include <iptables.h>

#include "ipt_ipp2p.h"

static void
help(void)
{
    printf(
    "IPP2P v%s options:\n"
    " --ipp2p	Grab all known p2p packets\n"
    " --edk		[TCP&UDP]	All known eDonkey/eMule/Overnet packets\n"
    " --dc		[TCP] 		All known Direct Connect packets\n"
    " --kazaa	[TCP&UDP] 	All known KaZaA packets\n"
    " --gnu		[TCP&UDP]	All known Gnutella packets\n"
    " --bit		[TCP&UDP]	All known BitTorrent packets\n"
    " --apple	[TCP] 		All known AppleJuice packets\n"
    " --winmx	[TCP] 		All known WinMX\n"
    " --soul		[TCP] 		All known SoulSeek\n"
    " --ares		[TCP] 		All known Ares\n\n"
    " DEBUG SUPPPORT, use only if you know why\n"
    " --debug		Generate kernel debug output, THIS WILL SLOW DOWN THE FILTER\n"
    "\nNote that the follwing options will have the same meaning:\n"
    " '--ipp2p' is equal to '--edk --dc --kazaa --gnu --bit --apple --winmx --soul --ares'\n"
    "\nIPP2P was intended for TCP only. Due to increasing usage of UDP we needed to change this.\n"
    "You can now use -p udp to search UDP packets only or without -p switch to search UDP and TCP packets.\n"
    "\nSee README included with this package for more details or visit http://www.ipp2p.org\n"
    "\nExamples:\n"
    " iptables -A FORWARD -m ipp2p --ipp2p -j MARK --set-mark 0x01\n"
    " iptables -A FORWARD -p udp -m ipp2p --kazaa --bit -j DROP\n"
    " iptables -A FORWARD -p tcp -m ipp2p --edk --soul -j DROP\n\n"
    , IPP2P_VERSION);
}

static struct option opts[] = {
        { "ipp2p", 0, 0, '1' },
        { "edk", 0, 0, '2' },	
	{ "dc", 0, 0, '7' },
	{ "gnu", 0, 0, '9' },
	{ "kazaa", 0, 0, 'a' },
	{ "bit", 0, 0, 'b' },
	{ "apple", 0, 0, 'c' },	
	{ "soul", 0, 0, 'd' },	
	{ "winmx", 0, 0, 'e' },	
	{ "ares", 0, 0, 'f' },
	{ "debug", 0, 0, 'g' },
        {0}
};

	

static void
init(struct ipt_entry_match *m, unsigned int *nfcache)
{
    struct ipt_p2p_info *info = (struct ipt_p2p_info *)m->data;

    *nfcache |= NFC_UNKNOWN;

    /*init the module with default values*/
    info->cmd = 0;
    info->debug = 0;

}
	

static int
parse(int c, char **argv, int invert, unsigned int *flags,
	const struct ipt_entry *entry,
	unsigned int *nfcache,
	struct ipt_entry_match **match)
{
    struct ipt_p2p_info *info = (struct ipt_p2p_info *)(*match)->data;
    
    switch (c) {
	case '1':		/*cmd: ipp2p*/
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified once!");
/*	    if ((*flags & SHORT_HAND_DATA) == SHORT_HAND_DATA)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p-data' may only be "
				"specified alone!");*/
	    if ((*flags) != 0)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
	    *flags += SHORT_HAND_IPP2P;
	    info->cmd = *flags;
	    break;
	    
	case '2':		/*cmd: edk*/
	    if ((*flags & IPP2P_EDK) == IPP2P_EDK)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--edk' may only be "
				"specified once");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
/*	    if ((*flags & SHORT_HAND_DATA) == SHORT_HAND_DATA)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p-data' may only be "
				"specified alone!");*/
            if ((*flags & IPP2P_DATA_EDK) == IPP2P_DATA_EDK)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: use `--edk' OR `--edk-data' but not both of them!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
	    *flags += IPP2P_EDK;
	    info->cmd = *flags;	    
	    break;


	case '7':		/*cmd: dc*/
            if ((*flags & IPP2P_DC) == IPP2P_DC)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--dc' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
/*	    if ((*flags & SHORT_HAND_DATA) == SHORT_HAND_DATA)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p-data' may only be "
				"specified alone!");*/
            if ((*flags & IPP2P_DATA_DC) == IPP2P_DATA_DC)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: use `--dc' OR `--dc-data' but not both of them!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_DC;
	    info->cmd = *flags;
	    break;


	case '9':		/*cmd: gnu*/
            if ((*flags & IPP2P_GNU) == IPP2P_GNU)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--gnu' may only be "
                                "specified once!");
/*	    if ((*flags & SHORT_HAND_DATA) == SHORT_HAND_DATA)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p-data' may only be "
				"specified alone!");*/
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
            if ((*flags & IPP2P_DATA_GNU) == IPP2P_DATA_GNU)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: use `--gnu' OR `--gnu-data' but not both of them!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_GNU;
	    info->cmd = *flags;
	    break;

	case 'a':		/*cmd: kazaa*/
            if ((*flags & IPP2P_KAZAA) == IPP2P_KAZAA)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--kazaa' may only be "
                                "specified once!");
/*	    if ((*flags & SHORT_HAND_DATA) == SHORT_HAND_DATA)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p-data' may only be "
				"specified alone!");*/
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
            if ((*flags & IPP2P_DATA_KAZAA) == IPP2P_DATA_KAZAA)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: use `--kazaa' OR `--kazaa-data' but not both of them!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_KAZAA;
	    info->cmd = *flags;
	    break;																											

	case 'b':		/*cmd: bit*/
            if ((*flags & IPP2P_BIT) == IPP2P_BIT)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--bit' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_BIT;
	    info->cmd = *flags;
	    break;																											

	case 'c':		/*cmd: apple*/
            if ((*flags & IPP2P_APPLE) == IPP2P_APPLE)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--apple' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_APPLE;
	    info->cmd = *flags;
	    break;																											


	case 'd':		/*cmd: soul*/
            if ((*flags & IPP2P_SOUL) == IPP2P_SOUL)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--soul' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_SOUL;
	    info->cmd = *flags;
	    break;																											


	case 'e':		/*cmd: winmx*/
            if ((*flags & IPP2P_WINMX) == IPP2P_WINMX)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--winmx' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_WINMX;
	    info->cmd = *flags;
	    break;																											

	case 'f':		/*cmd: ares*/
            if ((*flags & IPP2P_ARES) == IPP2P_ARES)
            exit_error(PARAMETER_PROBLEM,
                                "ipp2p: `--ares' may only be "
                                "specified once!");
	    if ((*flags & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P)
		    exit_error(PARAMETER_PROBLEM,
				"ipp2p: `--ipp2p' may only be "
				"specified alone!");
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
            *flags += IPP2P_ARES;
	    info->cmd = *flags;
	    break;																											

	case 'g':		/*cmd: debug*/
	    if (invert) exit_error(PARAMETER_PROBLEM, "ipp2p: invert [!] is not allowed!");
	    info->debug = 1;
	    break;																											

	default:
//            exit_error(PARAMETER_PROBLEM,
//	    "\nipp2p-parameter problem: for ipp2p usage type: iptables -m ipp2p --help\n");
	    return 0;
    }
    return 1;
}


static void
final_check(unsigned int flags)
{
    if (!flags)
            exit_error(PARAMETER_PROBLEM,
	    "\nipp2p-parameter problem: for ipp2p usage type: iptables -m ipp2p --help\n");
}



static void
print(const struct ipt_ip *ip,
        const struct ipt_entry_match *match,
	int numeric)
{
    struct ipt_p2p_info *info = (struct ipt_p2p_info *)match->data;
    
    printf("ipp2p v%s", IPP2P_VERSION);
    if ((info->cmd & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P) printf(" --ipp2p");
//    if ((info->cmd & SHORT_HAND_DATA) == SHORT_HAND_DATA) printf(" --ipp2p-data");
    if ((info->cmd & IPP2P_KAZAA) == IPP2P_KAZAA) printf(" --kazaa");
//    if ((info->cmd & IPP2P_DATA_KAZAA) == IPP2P_DATA_KAZAA) printf(" --kazaa-data");
//    if ((info->cmd & IPP2P_DATA_GNU) == IPP2P_DATA_GNU) printf(" --gnu-data");
    if ((info->cmd & IPP2P_GNU) == IPP2P_GNU) printf(" --gnu");
    if ((info->cmd & IPP2P_EDK) == IPP2P_EDK) printf(" --edk");
//    if ((info->cmd & IPP2P_DATA_EDK) == IPP2P_DATA_EDK) printf(" --edk-data");
//    if ((info->cmd & IPP2P_DATA_DC) == IPP2P_DATA_DC) printf(" --dc-data");
    if ((info->cmd & IPP2P_DC) == IPP2P_DC) printf(" --dc");
    if ((info->cmd & IPP2P_BIT) == IPP2P_BIT) printf(" --bit");
    if ((info->cmd & IPP2P_APPLE) == IPP2P_APPLE) printf(" --apple");
    if ((info->cmd & IPP2P_SOUL) == IPP2P_SOUL) printf(" --soul");
    if ((info->cmd & IPP2P_WINMX) == IPP2P_WINMX) printf(" --winmx");
    if ((info->cmd & IPP2P_ARES) == IPP2P_ARES) printf(" --ares");
    if (info->debug != 0) printf(" --debug");
    printf(" ");
}
			    						   


static void
save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
    struct ipt_p2p_info *info = (struct ipt_p2p_info *)match->data;
    
    if ((info->cmd & SHORT_HAND_IPP2P) == SHORT_HAND_IPP2P) printf("--ipp2p ");
//    if ((info->cmd & SHORT_HAND_DATA) == SHORT_HAND_DATA) printf("--ipp2p-data ");
    if ((info->cmd & IPP2P_KAZAA) == IPP2P_KAZAA) printf("--kazaa ");
//    if ((info->cmd & IPP2P_DATA_KAZAA) == IPP2P_DATA_KAZAA) printf("--kazaa-data ");
//    if ((info->cmd & IPP2P_DATA_GNU) == IPP2P_DATA_GNU) printf("--gnu-data ");
    if ((info->cmd & IPP2P_GNU) == IPP2P_GNU) printf("--gnu ");
    if ((info->cmd & IPP2P_EDK) == IPP2P_EDK) printf("--edk ");
//    if ((info->cmd & IPP2P_DATA_EDK) == IPP2P_DATA_EDK) printf("--edk-data ");
//    if ((info->cmd & IPP2P_DATA_DC) == IPP2P_DATA_DC) printf("--dc-data ");
    if ((info->cmd & IPP2P_DC) == IPP2P_DC) printf("--dc ");
    if ((info->cmd & IPP2P_BIT) == IPP2P_BIT) printf("--bit ");
    if ((info->cmd & IPP2P_APPLE) == IPP2P_APPLE) printf("--apple ");
    if ((info->cmd & IPP2P_SOUL) == IPP2P_SOUL) printf("--soul ");
    if ((info->cmd & IPP2P_WINMX) == IPP2P_WINMX) printf("--winmx ");
    if ((info->cmd & IPP2P_ARES) == IPP2P_ARES) printf("--ares ");
    if (info->debug != 0) printf("--debug ");
}

		


static 
struct iptables_match ipp2p= 
{ 
    .next           = NULL,
    .name           = "ipp2p",
    .version        = IPTABLES_VERSION,
    .size           = IPT_ALIGN(sizeof(struct ipt_p2p_info)),
    .userspacesize  = IPT_ALIGN(sizeof(struct ipt_p2p_info)),
    .help           = &help,
    .init           = &init,
    .parse          = &parse,
    .final_check    = &final_check,
    .print          = &print,
    .save           = &save,
    .extra_opts     = opts
};
					    


void _init(void)
{
    register_match(&ipp2p);
}

