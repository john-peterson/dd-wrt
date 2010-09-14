#include <zebra.h>
#include "command.h"
#include "vtysh.h"

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_cmd_vtysh, 
       "show ipv6 mbgp", 
       "Show running system information\n"
       "IP information\n"
       "MBGP information\n")

DEFSH (VTYSH_BGPD, show_ip_community_list_arg_cmd_vtysh, 
       "show ip community-list (<1-500>|WORD)", 
       "Show running system information\n"
       "IP information\n"
       "List community-list\n"
       "Community-list number\n"
       "Community-list name\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_cmd_vtysh, 
       "show ipv6 ospf6 interface", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       )

DEFSH (VTYSH_ISISD, domain_passwd_cmd_vtysh, 
       "domain-password WORD", 
       "Set the authentication password for a routing domain\n"
       "Routing domain password\n")

DEFSH (VTYSH_BGPD, no_neighbor_distribute_list_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "distribute-list (<1-199>|<1300-2699>|WORD) (in|out)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Filter updates to/from this neighbor\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n"
       "Filter incoming updates\n"
       "Filter outgoing updates\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_param1_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535>", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_RIPD, no_rip_offset_list_ifname_cmd_vtysh, 
       "no offset-list WORD (in|out) <0-16> IFNAME", 
       "Negate a command or set its defaults\n"
       "Modify RIP metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n"
       "Interface to match\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_as4_cmd_vtysh, 
       "no debug bgp as4", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 actions\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, no_neighbor_maximum_prefix_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_prefix_longer_cmd_vtysh, 
       "show ipv6 mbgp X:X::X:X/M longer-prefixes", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_param1_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_regexp_cmd_vtysh, 
       "show ipv6 mbgp regexp .LINE", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the MBGP AS paths\n")

DEFSH (VTYSH_RIPD, accept_lifetime_day_month_day_month_cmd_vtysh, 
       "accept-lifetime HH:MM:SS <1-31> MONTH <1993-2035> HH:MM:SS <1-31> MONTH <1993-2035>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Day of th month to expire\n"
       "Month of the year to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, no_set_community_val_cmd_vtysh, 
       "no set community .AA:NN", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP community attribute\n"
       "Community number in aa:nn format or local-AS|no-advertise|no-export|internet or additive\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community_list_exact_cmd_vtysh, 
       "show ipv6 bgp community-list WORD exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged9_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med next-hop as-path", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "Nexthop attribute\n"
       "As-path attribute\n")

DEFSH (VTYSH_OSPFD, ospf_max_metric_router_lsa_shutdown_cmd_vtysh, 
       "max-metric router-lsa on-shutdown <5-86400>", 
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Advertise stub-router prior to full shutdown of OSPF\n"
       "Time (seconds) to wait till full shutdown\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_zebra_sub_cmd_vtysh, 
       "no debug ospf zebra (interface|redistribute)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Zebra information\n"
       "Zebra interface\n"
       "Zebra redistribute\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_string_cmd_vtysh, 
       "no ip rip authentication string", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication string\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_soft_cmd_vtysh, 
       "clear ip bgp view WORD * soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_OSPFD, no_capability_opaque_cmd_vtysh, 
       "no capability opaque", 
       "Negate a command or set its defaults\n"
       "Enable specific OSPF feature\n"
       "Opaque LSA\n")

DEFSH (VTYSH_BGPD, show_bgp_community3_exact_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_RIPD, send_lifetime_day_month_month_day_cmd_vtysh, 
       "send-lifetime HH:MM:SS <1-31> MONTH <1993-2035> HH:MM:SS MONTH <1-31> <1993-2035>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Month of the year to expire\n"
       "Day of th month to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_routes_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, no_ipv6_bgp_network_route_map_cmd_vtysh, 
       "no network X:X::X:X/M route-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_distance_cmd_vtysh, 
       "no ip route A.B.C.D/M (A.B.C.D|INTERFACE|null0) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n"
       "Distance value for this route\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_int_cmd_vtysh, 
       "show ip ospf neighbor IFNAME", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, no_match_origin_val_cmd_vtysh, 
       "no match origin (egp|igp|incomplete)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "BGP origin code\n"
       "remote EGP\n"
       "local IGP\n"
       "unknown heritage\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_advertised_route_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_OSPF6D|VTYSH_RIPD|VTYSH_RIPNGD, no_router_zebra_cmd_vtysh, 
       "no router zebra", 
       "Negate a command or set its defaults\n"
       "Disable a routing process\n"
       "Stop connection to zebra daemon\n")

DEFSH (VTYSH_BGPD, no_bgp_graceful_restart_cmd_vtysh, 
       "no bgp graceful-restart", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Graceful restart capability parameters\n")

DEFSH (VTYSH_BGPD, no_router_bgp_cmd_vtysh, 
       "no router bgp " "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n"
       "BGP information\n"
       "AS number\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_cmd_vtysh, 
       "show ip route", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_neighbor_advertised_route_cmd_vtysh, 
       "show ip bgp view WORD neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_OSPF6D, no_ospf6_interface_area_cmd_vtysh, 
       "no interface IFNAME area A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Disable routing on an IPv6 interface\n"
       "Interface name(e.g. ep0)\n"
       "Specify the OSPF6 area ID\n"
       "OSPF6 area ID in IPv4 address notation\n"
       )

DEFSH (VTYSH_BGPD, clear_bgp_all_cmd_vtysh, 
       "clear bgp *", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n")

DEFSH (VTYSH_ZEBRA, show_ip_protocol_cmd_vtysh, 
       "show ip protocol", 
        "Show running system information\n"
        "IP information\n"
       "IP protocol filtering status\n")

DEFSH (VTYSH_BGPD, no_neighbor_default_originate_rmap_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "default-originate route-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Originate default route to this neighbor\n"
       "Route-map to specify criteria to originate default\n"
       "route-map name\n")

DEFSH (VTYSH_OSPFD, ospf_transmit_delay_cmd_vtysh, 
       "ospf transmit-delay <1-65535>", 
       "OSPF interface commands\n"
       "Link state transmit delay\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_summary_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_RIPNGD, no_ripng_default_metric_cmd_vtysh, 
       "no default-metric", 
       "Negate a command or set its defaults\n"
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_self_originated_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) self-originated "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display Self-originated LSAs\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_summary_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_neighbor_detail_cmd_vtysh, 
       "no debug ospf6 neighbor (state|event)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Neighbor\n"
       "Debug OSPFv3 Neighbor State Change\n"
       "Debug OSPFv3 Neighbor Event\n"
      )

DEFSH (VTYSH_OSPFD, ospf_area_range_substitute_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M substitute A.B.C.D/M", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "Announce area range as another prefix\n"
       "Network prefix to be announced instead of range\n")

DEFSH (VTYSH_OSPFD, no_mpls_te_cmd_vtysh, 
       "no mpls-te", 
       "Negate a command or set its defaults\n"
       "Configure MPLS-TE parameters\n"
       "Disable the MPLS-TE functionality\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbor_advertised_route_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_OSPF6D, no_ipv6_ospf6_ifmtu_cmd_vtysh, 
       "no ipv6 ospf6 ifmtu", 
       "Negate a command or set its defaults\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface MTU\n"
       )

DEFSH (VTYSH_BGPD, no_neighbor_timers_connect_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "timers connect <0-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "BGP per neighbor timers\n"
       "BGP connect timer\n"
       "Connect timer\n")

DEFSH (VTYSH_ALL, no_access_list_extended_any_mask_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip any A.B.C.D A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_intra_inter_cmd_vtysh, 
       "distance ospf intra-area <1-255> inter-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_debug_misc_cmd_vtysh, 
       "ip irdp debug misc", 
       "IP information\n"
       "ICMP Router discovery debug Averts. and Solicits (short)\n")

DEFSH (VTYSH_ISISD, show_database_detail_cmd_vtysh, 
       "show isis database detail", 
       "Show running system information\n"
       "IS-IS information\n"
       "IS-IS link state database\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_backdoor_ttl_cmd_vtysh, 
       "no network A.B.C.D mask A.B.C.D  backdoor pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_onmatch_goto_cmd_vtysh, 
       "on-match goto <1-65535>", 
       "Exit policy on matches\n"
       "Goto Clause number\n"
       "Number\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_soft_in_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_in_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_summary_cmd_vtysh, 
       "show ip bgp summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_l1_cmd_vtysh, 
       "no lsp-gen-interval level-1", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 1 only\n")

DEFSH (VTYSH_OSPFD, ospf_area_filter_list_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) filter-list prefix WORD (in|out)", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Filter networks between OSPF areas\n"
       "Filter prefixes between OSPF areas\n"
       "Name of an IP prefix-list\n"
       "Filter networks sent to this area\n"
       "Filter networks sent from this area\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_ipv4_rsclient_summary_cmd_vtysh, 
      "show ip bgp view WORD ipv4 (unicast|multicast) rsclient summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_soft_in_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_log_adjacency_changes_detail_cmd_vtysh, 
       "no log-adjacency-changes detail", 
       "Negate a command or set its defaults\n"
       "Log changes in adjacency state\n"
       "Log all state changes\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_prefix_cmd_vtysh, 
       "show ip bgp view WORD A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_type_detail_cmd_vtysh, 
       "show ipv6 ospf6 route (intra-area|inter-area|external-1|external-2) detail", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Dispaly Intra-Area routes\n"
       "Dispaly Inter-Area routes\n"
       "Dispaly Type-1 External routes\n"
       "Dispaly Type-2 External routes\n"
       "Detailed information\n"
       )

DEFSH (VTYSH_OSPFD, no_ospf_network_area_cmd_vtysh, 
       "no network A.B.C.D/M area (A.B.C.D|<0-4294967295>)", 
       "Negate a command or set its defaults\n"
       "Enable routing on an IP network\n"
       "OSPF network prefix\n"
       "Set the OSPF area ID\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n")

DEFSH (VTYSH_BGPD, bgp_deterministic_med_cmd_vtysh, 
       "bgp deterministic-med", 
       "BGP specific commands\n"
       "Pick the best-MED path among paths advertised from the neighboring AS\n")

DEFSH (VTYSH_BGPD, undebug_bgp_as4_cmd_vtysh, 
       "undebug bgp as4", 
       "Disable debugging functions (see also 'debug')\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 actions\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_out_cmd_vtysh, 
       "clear bgp peer-group WORD out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_max_metric_router_lsa_admin_cmd_vtysh, 
       "no max-metric router-lsa administrative", 
       "Negate a command or set its defaults\n"
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Administratively applied,  for an indefinite period\n")

DEFSH (VTYSH_OSPFD, ospf_default_metric_cmd_vtysh, 
       "default-metric <0-16777214>", 
       "Set metric of redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_intra_external_inter_cmd_vtysh, 
       "distance ospf intra-area <1-255> external <1-255> inter-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "External routes\n"
       "Distance for external routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n")

DEFSH (VTYSH_BGPD, aggregate_address_summary_only_cmd_vtysh, 
       "aggregate-address A.B.C.D/M summary-only", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_BGPD, no_set_pathlimit_ttl_val_cmd_vtysh, 
       "no set pathlimit ttl <1-255>", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "BGP AS-Pathlimit attribute\n"
       "Set AS-Path Hop-count TTL\n")

DEFSH (VTYSH_ISISD, isis_priority_l2_cmd_vtysh, 
       "isis priority <0-127> level-2", 
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n"
       "Specify priority for level-2 routing\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_linkstate_id_cmd_vtysh, 
       "show ipv6 ospf6 database linkstate-id A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_vpnv4_soft_out_cmd_vtysh, 
       "clear ip bgp * vpnv4 unicast soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, no_set_origin_val_cmd_vtysh, 
       "no set origin (egp|igp|incomplete)", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP origin code\n"
       "remote EGP\n"
       "local IGP\n"
       "unknown heritage\n")

DEFSH (VTYSH_RIPNGD, no_ripng_timers_val_cmd_vtysh, 
       "no timers basic <0-65535> <0-65535> <0-65535>", 
       "Negate a command or set its defaults\n"
       "RIPng timers setup\n"
       "Basic timer\n"
       "Routing table update timer value in second. Default is 30.\n"
       "Routing information timeout timer. Default is 180.\n"
       "Garbage collection timer. Default is 120.\n")

DEFSH (VTYSH_BGPD, bgp_damp_set3_cmd_vtysh, 
       "bgp dampening", 
       "BGP Specific commands\n"
       "Enable route-flap dampening\n")

DEFSH (VTYSH_RIPD, no_rip_offset_list_cmd_vtysh, 
       "no offset-list WORD (in|out) <0-16>", 
       "Negate a command or set its defaults\n"
       "Modify RIP metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_self_originated_linkstate_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) self-originated "
       "linkstate-id A.B.C.D (detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display Self-originated LSAs\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, ipv6_bgp_network_route_map_cmd_vtysh, 
       "network X:X::X:X/M route-map WORD", 
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_ISISD, no_debug_isis_upd_cmd_vtysh, 
       "no debug isis update-packets", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS Update related packets\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_type_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
         " metric-type (1|2) route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_flags_distance_cmd_vtysh, 
       "no ip route A.B.C.D/M (A.B.C.D|INTERFACE) (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_RIPNGD, debug_ripng_zebra_cmd_vtysh, 
       "debug ripng zebra", 
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng and zebra communication\n")

DEFSH (VTYSH_OSPFD, no_ospf_refresh_timer_val_cmd_vtysh, 
       "no refresh timer <10-1800>", 
       "Adjust refresh parameters\n"
       "Unset refresh timer\n"
       "Timer value in seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_remote_as_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "remote-as " "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Specify a BGP neighbor\n"
       "AS number\n")

DEFSH (VTYSH_BGPD, set_vpnv4_nexthop_cmd_vtysh, 
       "set vpnv4 next-hop A.B.C.D", 
       "Set values in destination routing protocol\n"
       "VPNv4 information\n"
       "VPNv4 next-hop address\n"
       "IP address of next hop\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_prefix_cmd_vtysh, 
       "no ipv6 prefix-list WORD (deny|permit) (X:X::X:X/M|any)", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Any prefix match.  Same as \"::0/0 le 128\"\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database * A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_RIPNGD, ipv6_ripng_split_horizon_poisoned_reverse_cmd_vtysh, 
       "ipv6 ripng split-horizon poisoned-reverse", 
       "IPv6 information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n"
       "With poisoned-reverse\n")

DEFSH (VTYSH_OSPFD, debug_ospf_zebra_cmd_vtysh, 
       "debug ospf zebra", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Zebra information\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_prefix_first_match_cmd_vtysh, 
       "show ip prefix-list WORD A.B.C.D/M first-match", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "First matched prefix\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbors_peer_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_ifname_flags_pref_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M X:X::X:X INTERFACE (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_OSPF6D, ospf6_routemap_no_set_forwarding_cmd_vtysh, 
       "no set forwarding-address X:X::X:X", 
       "Negate a command or set its defaults\n"
       "Set value\n"
       "Forwarding Address\n"
       "IPv6 Address\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_neighbors_cmd_vtysh, 
       "show ip bgp vpnv4 all neighbors", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_BGPD, neighbor_send_community_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "send-community", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Send Community attribute to this neighbor\n")

DEFSH (VTYSH_BGPD, no_set_pathlimit_ttl_cmd_vtysh, 
       "no set pathlimit ttl", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP AS-Pathlimit attribute\n"
       "Set AS-Path Hop-count TTL\n")

DEFSH (VTYSH_BGPD, dump_bgp_updates_cmd_vtysh, 
       "dump bgp updates PATH", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump BGP updates only\n"
       "Output filename\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_list_exact_cmd_vtysh, 
       "show ip bgp community-list (<1-500>|WORD) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp A.B.C.D in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig inbound update\n"
       "Push out the existing ORF prefix-list\n")

DEFSH (VTYSH_RIPD, accept_lifetime_infinite_day_month_cmd_vtysh, 
       "accept-lifetime HH:MM:SS <1-31> MONTH <1993-2035> infinite", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Never expires")

DEFSH (VTYSH_ISISD, no_debug_isis_adj_cmd_vtysh, 
       "no debug isis adj-packets", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS Adjacency related packets\n")

DEFSH (VTYSH_OSPFD, mpls_te_cmd_vtysh, 
       "mpls-te", 
       "Configure MPLS-TE parameters\n"
       "Enable the MPLS-TE functionality\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_soft_out_cmd_vtysh, 
       "clear bgp ipv6 * soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_message_digest_key_cmd_vtysh, 
       "no ip ospf message-digest-key <1-255>", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_packet_cmd_vtysh, 
       "debug zebra packet", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_med2_cmd_vtysh, 
       "no bgp bestpath med confed missing-as-worst", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Compare MED among confederation paths\n"
       "Treat missing MED as the least preferred one\n")

DEFSH (VTYSH_BGPD, no_bgp_multiple_instance_cmd_vtysh, 
       "no bgp multiple-instance", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "BGP multiple instance\n")

DEFSH (VTYSH_RIPD, no_rip_distance_cmd_vtysh, 
       "no distance <1-255>", 
       "Negate a command or set its defaults\n"
       "Administrative distance\n"
       "Distance value\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_soft_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_route_map_all_cmd_vtysh, 
       "no route-map WORD", 
       "Negate a command or set its defaults\n"
       "Create route-map or enter route-map command mode\n"
       "Route map tag\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_brouter_router_cmd_vtysh, 
       "debug ospf6 border-routers router-id A.B.C.D", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
       "Debug specific border router\n"
       "Specify border-router's router-id\n"
      )

DEFSH (VTYSH_BGPD, bgp_distance_source_access_list_cmd_vtysh, 
       "distance <1-255> A.B.C.D/M WORD", 
       "Define an administrative distance\n"
       "Administrative distance\n"
       "IP source prefix\n"
       "Access list name\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community3_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_self_originated_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) self-originated", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display Self-originated LSAs\n"
      )

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_spf_process_cmd_vtysh, 
       "no debug ospf6 spf process", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Quit Debugging SPF Calculation\n"
       "Quit Debugging Detailed SPF Process\n"
      )

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_sequence_number_cmd_vtysh, 
       "no ip prefix-list sequence-number", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Include/exclude sequence numbers in NVGEN\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_cmd_vtysh, 
       "clear bgp peer-group WORD", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n")

DEFSH (VTYSH_BGPD, no_neighbor_nexthop_local_unchanged_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "nexthop-local unchanged", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure treatment of outgoing link-local-nexthop attribute\n"
       "Leave link-local nexthop unchanged for this peer\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_rsclient_cmd_vtysh, 
       "show ip bgp rsclient (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n")

DEFSH (VTYSH_BGPD, show_bgp_view_rsclient_prefix_cmd_vtysh, 
       "show bgp view WORD rsclient (A.B.C.D|X:X::X:X) X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_ISISD, no_metric_style_cmd_vtysh, 
       "no metric-style", 
       "Negate a command or set its defaults\n"
       "Use old-style (ISO 10589) or new-style packet formats\n")

DEFSH (VTYSH_BGPD, show_bgp_statistics_vpnv4_cmd_vtysh, 
       "show bgp (ipv4) (vpnv4) statistics", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "BGP RIB advertisement statistics\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_metric_type_routemap_cmd_vtysh, 
       "default-information originate metric <0-16777214> metric-type (1|2) route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, show_bgp_statistics_view_cmd_vtysh, 
       "show bgp view WORD (ipv4|ipv6) (unicast|multicast) statistics", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "Address family\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "BGP RIB advertisement statistics\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_cmd_vtysh, 
       "show ip ospf database", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_threshold_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> <1-100>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Threshold value (%) at which to generate a warning msg\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbor_received_prefix_filter_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_seq_le_cmd_vtysh, 
       "ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M le <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_metric_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
         " metric <0-16777214> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_flags_cmd_vtysh, 
       "ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp view WORD * ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_route_cmd_vtysh, 
       "show bgp view WORD ipv6 X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_ISISD, no_debug_isis_err_cmd_vtysh, 
       "no debug isis protocol-errors", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS LSP protocol errors\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_arg_cmd_vtysh, 
       "no isis hello-interval (<1-65535>|minimal)", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 second,  interval depends on multiplier\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community_all_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_soft_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " soft", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n")

DEFSH (VTYSH_ALL, access_list_extended_host_any_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D any", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "Any destination host\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_out_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_flags_cmd_vtysh, 
       "no ip route A.B.C.D/M (A.B.C.D|INTERFACE) (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_ISISD, lsp_gen_interval_cmd_vtysh, 
       "lsp-gen-interval <1-120>", 
       "Minimum interval between regenerating same LSP\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_prefix_longer_cmd_vtysh, 
       "show ipv6 bgp X:X::X:X/M longer-prefixes", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_BGPD, no_bgp_distance_source_access_list_cmd_vtysh, 
       "no distance <1-255> A.B.C.D/M WORD", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "Administrative distance\n"
       "IP source prefix\n"
       "Access list name\n")

DEFSH (VTYSH_OSPFD, ospf_opaque_capable_cmd_vtysh, 
       "ospf opaque-lsa", 
       "OSPF specific commands\n"
       "Enable the Opaque-LSA capability (rfc2370)\n")

DEFSH (VTYSH_BGPD, neighbor_update_source_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "update-source " "(A.B.C.D|X:X::X:X|WORD)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Source of routing updates\n"
       "IPv4 address\n" "IPv6 address\n" "Interface name (requires zebra to be running)\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_sequence_number_cmd_vtysh, 
       "no ipv6 prefix-list sequence-number", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Include/exclude sequence numbers in NVGEN\n")

DEFSH (VTYSH_ISISD, debug_isis_err_cmd_vtysh, 
       "debug isis protocol-errors", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS LSP protocol errors\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv6_metric_rmap_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf6|ripng|static) metric <0-4294967295> route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, ipv6_aggregate_address_cmd_vtysh, 
       "aggregate-address X:X::X:X/M", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_message_cmd_vtysh, 
       "no debug ospf6 message (unknown|hello|dbdesc|lsreq|lsupdate|lsack|all)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 message\n"
       "Debug Unknown message\n"
       "Debug Hello message\n"
       "Debug Database Description message\n"
       "Debug Link State Request message\n"
       "Debug Link State Update message\n"
       "Debug Link State Acknowledgement message\n"
       "Debug All message\n"
       )

DEFSH (VTYSH_OSPFD, no_ospf_auto_cost_reference_bandwidth_cmd_vtysh, 
       "no auto-cost reference-bandwidth", 
       "Negate a command or set its defaults\n"
       "Calculate OSPF interface cost according to bandwidth\n"
       "Use reference bandwidth method to assign OSPF cost\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_max_metric_router_lsa_startup_cmd_vtysh, 
       "no max-metric router-lsa on-startup", 
       "Negate a command or set its defaults\n"
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Automatically advertise stub Router-LSA on startup of OSPF\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_vpnv4_soft_cmd_vtysh, 
       "clear ip bgp * vpnv4 unicast soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, no_neighbor_ebgp_multihop_ttl_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "ebgp-multihop <1-255>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Allow EBGP neighbors not on directly connected networks\n"
       "maximum hop count\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_routemap_cmd_vtysh, 
       "default-information originate route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_route_cmd_vtysh, 
       "debug ospf6 route (table|intra-area|inter-area)", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug route table calculation\n"
       "Debug detail\n"
       "Debug intra-area route calculation\n"
       "Debug inter-area route calculation\n"
       )

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_id_cmd_vtysh, 
       "show ipv6 ospf6 database * A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_RIPD, no_distribute_list_prefix_cmd_vtysh, 
       "no distribute-list prefix WORD (in|out) WORD", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, no_neighbor_interface_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "interface WORD", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Interface\n"
       "Interface name\n")

DEFSH (VTYSH_RIPD, no_rip_distance_source_access_list_cmd_vtysh, 
       "no distance <1-255> A.B.C.D/M WORD", 
       "Negate a command or set its defaults\n"
       "Administrative distance\n"
       "Distance value\n"
       "IP source prefix\n"
       "Access list name\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_cidr_only_cmd_vtysh, 
       "show ip bgp flap-statistics cidr-only", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Display only routes with non-natural netmasks\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_summary_cmd_vtysh, 
       "show ip bgp vpnv4 all summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_address_cmd_vtysh, 
       "no ipv6 address X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "Interface IPv6 config commands\n"
       "Set the IP address of an interface\n"
       "IPv6 address (e.g. 3ffe:506::1/48)\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_soft_cmd_vtysh, 
       "clear bgp external soft", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, no_set_tag_cmd_vtysh, 
       "no set tag", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Tag value for routing protocol\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_args_authkey_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) (message-digest|null) "
       "(authentication-key|) AUTH_KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n" "Use null authentication\n" "Use message-digest authentication\n"
       "Authentication password (key)\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, no_neighbor_shutdown_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "shutdown", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Administratively shut down this neighbor\n")

DEFSH (VTYSH_ALL, vty_no_restricted_mode_cmd_vtysh, 
       "no anonymous restricted", 
       "Negate a command or set its defaults\n"
       "Enable password checking\n")

DEFSH (VTYSH_BGPD, show_bgp_prefix_list_cmd_vtysh, 
       "show bgp prefix-list WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes conforming to the prefix-list\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_summary_cmd_vtysh, 
       "show ipv6 prefix-list summary", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Summary of prefix lists\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_flooding_cmd_vtysh, 
       "debug ospf6 flooding", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 flooding function\n"
      )

DEFSH (VTYSH_ISISD, no_debug_isis_csum_cmd_vtysh, 
       "no debug isis checksum-errors", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS LSP checksum errors\n")

DEFSH (VTYSH_BGPD, set_community_none_cmd_vtysh, 
       "set community none", 
       "Set values in destination routing protocol\n"
       "BGP community attribute\n"
       "No community attribute\n")

DEFSH (VTYSH_RIPD, distribute_list_prefix_all_cmd_vtysh, 
       "distribute-list prefix WORD (in|out)", 
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged6_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path med next-hop", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Med attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_RIPD, ip_rip_receive_version_cmd_vtysh, 
       "ip rip receive version (1|2)", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement reception\n"
       "Version control\n"
       "RIP version 1\n"
       "RIP version 2\n")

DEFSH (VTYSH_ISISD, show_isis_neighbors_cmd_vtysh, 
       "show isis neighbors", 
       "Show running system information\n"
       "IS-IS network information\n"
       "IS-IS neighbor adjacencies\n")

DEFSH (VTYSH_ISISD, show_hostname_cmd_vtysh, 
       "show isis hostname", 
       "Show running system information\n"
       "IS-IS information\n"
       "IS-IS Dynamic hostname mapping\n")

DEFSH (VTYSH_ISISD, ip_router_isis_cmd_vtysh, 
       "ip router isis WORD", 
       "Interface Internet Protocol config commands\n"
       "IP router interface commands\n"
       "IS-IS Routing for IP\n"
       "Routing process tag\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_as_set_cmd_vtysh, 
       "no aggregate-address A.B.C.D/M as-set", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_ZEBRA, no_linkdetect_cmd_vtysh, 
       "no link-detect", 
       "Negate a command or set its defaults\n"
       "Disable link detection on interface\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_out_cmd_vtysh, 
       "clear bgp ipv6 * out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D A.B.C.D "
       "(dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPFD, show_debugging_ospf_cmd_vtysh, 
       "show debugging ospf", 
       "Show running system information\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n")

DEFSH (VTYSH_ALL, no_access_list_all_cmd_vtysh, 
       "no access-list (<1-99>|<100-199>|<1300-1999>|<2000-2699>|WORD)", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP extended access list\n"
       "IP standard access list (expanded range)\n"
       "IP extended access list (expanded range)\n"
       "IP zebra access-list name\n")

DEFSH (VTYSH_BGPD, bgp_config_type_cmd_vtysh, 
       "bgp config-type (cisco|zebra)", 
       "BGP information\n"
       "Configuration type\n"
       "cisco\n"
       "zebra\n")

DEFSH (VTYSH_RIPNGD, if_ipv6_rmap_cmd_vtysh, 
       "route-map RMAP_NAME (in|out) IFNAME", 
       "Route map set\n"
       "Route map name\n"
       "Route map set for input filtering\n"
       "Route map set for output filtering\n"
       "Route map interface name\n")

DEFSH (VTYSH_OSPFD, ospf_area_nssa_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) nssa", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_int_detail_cmd_vtysh, 
       "show ip ospf neighbor IFNAME detail", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "Interface name\n"
       "detail of all neighbors")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged10_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med as-path next-hop", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "As-path attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_border_routers_cmd_vtysh, 
       "show ipv6 ospf6 border-routers", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display routing table for ABR and ASBR\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_network_backdoor_cmd_vtysh, 
       "no network A.B.C.D/M backdoor", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_ifname_prefix_cmd_vtysh, 
       "show ipv6 ospf6 interface IFNAME prefix", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Interface name(e.g. ep0)\n"
       "Display connected prefixes to advertise\n"
       )

DEFSH (VTYSH_ISISD, debug_isis_rtevents_cmd_vtysh, 
       "debug isis route-events", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS Route related events\n")

DEFSH (VTYSH_RIPNGD, no_debug_ripng_events_cmd_vtysh, 
       "no debug ripng events", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng events\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_simulate_spf_tree_root_cmd_vtysh, 
       "show ipv6 ospf6 simulate spf-tree A.B.C.D area A.B.C.D", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Shortest Path First caculation\n"
       "Show SPF tree\n"
       "Specify root's router-id to calculate another router's SPF tree\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_filter_list_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) filter-list prefix WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Filter networks between OSPF areas\n"
       "Filter prefixes between OSPF areas\n"
       "Name of an IP prefix-list\n"
       "Filter networks sent to this area\n"
       "Filter networks sent from this area\n")

DEFSH (VTYSH_BGPD, aggregate_address_mask_summary_only_cmd_vtysh, 
       "aggregate-address A.B.C.D A.B.C.D summary-only", 
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_all_cmd_vtysh, 
       "show ip bgp community", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged8_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop as-path med", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "As-path attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_OSPFD, ospf_neighbor_poll_interval_priority_cmd_vtysh, 
       "neighbor A.B.C.D poll-interval <1-65535> priority <0-255>", 
       "Specify neighbor router\n"
       "Neighbor address\n"
       "OSPF dead-router polling interval\n"
       "Seconds\n"
       "OSPF priority of non-broadcast neighbor\n"
       "Priority\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_adv_router_linkstate_id_cmd_vtysh, 
       "show ipv6 ospf6 database adv-router A.B.C.D linkstate-id A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
      )

DEFSH (VTYSH_RIPNGD, ripng_default_metric_cmd_vtysh, 
       "default-metric <1-16>", 
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_cmd_vtysh, 
       "no aggregate-address A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_BGPD, no_neighbor_allowas_in_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "allowas-in", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "allow local ASN appears in aspath attribute\n")

DEFSH (VTYSH_BGPD, aggregate_address_mask_as_set_cmd_vtysh, 
       "aggregate-address A.B.C.D A.B.C.D as-set", 
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_ISISD, spf_interval_cmd_vtysh, 
       "spf-interval <1-120>", 
       "Minimum interval between SPF calculations\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_vpnv4_soft_in_cmd_vtysh, 
       "clear ip bgp * vpnv4 unicast soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, rip_redistribute_type_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|ospf|isis|bgp)" " route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_brouter_cmd_vtysh, 
       "debug ospf6 border-routers", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
      )

DEFSH (VTYSH_ZEBRA, ip_route_mask_flags_distance2_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "Distance value for this route\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_metric_type_cmd_vtysh, 
       "default-information originate always metric <0-16777214> metric-type (1|2)", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_range_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) range A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_seq_cmd_vtysh, 
       "ip prefix-list WORD seq <1-4294967295> (deny|permit) (A.B.C.D/M|any)", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Any prefix match. Same as \"0.0.0.0/0 le 32\"\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_linkstate_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) linkstate-id A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPFD, no_ospf_network_cmd_vtysh, 
       "no ospf network", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Network type\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_prefix_cmd_vtysh, 
       "no ip prefix-list WORD (deny|permit) (A.B.C.D/M|any)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Any prefix match.  Same as \"0.0.0.0/0 le 32\"\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_distribute_list_all_cmd_vtysh, 
       "no distribute-list WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_BGPD, set_community_delete_cmd_vtysh, 
       "set comm-list (<1-99>|<100-500>|WORD) delete", 
       "Set values in destination routing protocol\n"
       "set BGP community list (for deletion)\n"
       "Community-list number (standard)\n"
       "Communitly-list number (expanded)\n"
       "Community-list name\n"
       "Delete matching communities\n")

DEFSH (VTYSH_OSPF6D, no_router_ospf6_cmd_vtysh, 
       "no router ospf6", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_stub_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) stub", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as stub\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_soft_out_cmd_vtysh, 
       "clear ip bgp external soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_neighbor_cmd_vtysh, 
       "show ipv6 ospf6 neighbor", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Neighbor list\n"
      )

DEFSH (VTYSH_RIPD, rip_passive_interface_cmd_vtysh, 
       "passive-interface (IFNAME|default)", 
       "Suppress routing updates on an interface\n"
       "Interface name\n"
       "default for all interfaces\n")

DEFSH (VTYSH_BGPD, no_neighbor_capability_orf_prefix_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "capability orf prefix-list (both|send|receive)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Advertise capability to the peer\n"
       "Advertise ORF capability to the peer\n"
       "Advertise prefixlist ORF capability to this neighbor\n"
       "Capability to SEND and RECEIVE the ORF to/from this neighbor\n"
       "Capability to RECEIVE the ORF from this neighbor\n"
       "Capability to SEND the ORF to this neighbor\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_ifname_cmd_vtysh, 
       "show ipv6 ospf6 interface IFNAME", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Interface name(e.g. ep0)\n"
       )

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_intra_inter_external_cmd_vtysh, 
       "distance ospf intra-area <1-255> inter-area <1-255> external <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "External routes\n"
       "Distance for external routes\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_out_cmd_vtysh, 
       "clear bgp * out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_export_list_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) export-list NAME", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Unset the filter for networks announced to other areas\n"
       "Name of the access-list\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_instance_peer_rsclient_cmd_vtysh, 
       "clear bgp ipv6 view WORD (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP view\n"
       "view name\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_ISISD, no_debug_isis_events_cmd_vtysh, 
       "no debug isis events", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS Events\n")

DEFSH (VTYSH_RIPNGD, ipv6_distribute_list_prefix_all_cmd_vtysh, 
       "distribute-list prefix WORD (in|out)", 
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_seq_le_ge_cmd_vtysh, 
       "no ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M le <0-128> ge <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_OSPFD, ospf_area_shortcut_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) shortcut (default|enable|disable)", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure the area's shortcutting mode\n"
       "Set default shortcutting behavior\n"
       "Enable shortcutting through the area\n"
       "Disable shortcutting through the area\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_l1_arg_cmd_vtysh, 
       "no lsp-lifetime level-1 <380-65535>", 
       "Negate a command or set its defaults\n"
       "Maximum LSP lifetime for Level 1 only\n"
       "LSP lifetime for Level 1 only in seconds\n")

DEFSH (VTYSH_OSPFD, ospf_area_import_list_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) import-list NAME", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Set the filter for networks from other areas announced to the specified one\n"
       "Name of the access-list\n")

DEFSH (VTYSH_OSPFD, debug_ospf_lsa_sub_cmd_vtysh, 
       "debug ospf lsa (generate|flooding|install|refresh)", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Link State Advertisement\n"
       "LSA Generation\n"
       "LSA Flooding\n"
       "LSA Install/Delete\n"
       "LSA Refresh\n")

DEFSH (VTYSH_RIPD, send_lifetime_day_month_day_month_cmd_vtysh, 
       "send-lifetime HH:MM:SS <1-31> MONTH <1993-2035> HH:MM:SS <1-31> MONTH <1993-2035>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Day of th month to expire\n"
       "Month of the year to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, no_ipv6_aggregate_address_cmd_vtysh, 
       "no aggregate-address X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_RIPNGD, ripng_offset_list_ifname_cmd_vtysh, 
       "offset-list WORD (in|out) <0-16> IFNAME", 
       "Modify RIPng metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n"
       "Interface to match\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_seq_ge_cmd_vtysh, 
       "no ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M ge <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_route_cmd_vtysh, 
       "show ip bgp vpnv4 all A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_all_soft_in_cmd_vtysh, 
       "clear bgp view WORD * soft in", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_debug_messages_cmd_vtysh, 
       "ip irdp debug messages", 
       "IP information\n"
       "ICMP Router discovery debug Averts. and Solicits (short)\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, aggregate_address_mask_as_set_summary_cmd_vtysh, 
       "aggregate-address A.B.C.D A.B.C.D as-set summary-only", 
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Generate AS set path information\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_OSPFD, ip_ospf_hello_interval_cmd_vtysh, 
       "ip ospf hello-interval <1-65535>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between HELLO packets\n"
       "Seconds\n")

DEFSH (VTYSH_ISISD, no_debug_isis_spfstats_cmd_vtysh, 
       "no debug isis spf-statistics", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS SPF Timing and Statistic Data\n")

DEFSH (VTYSH_BGPD, no_set_local_pref_cmd_vtysh, 
       "no set local-preference", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP local preference path attribute\n")

DEFSH (VTYSH_BGPD, no_bgp_client_to_client_reflection_cmd_vtysh, 
       "no bgp client-to-client reflection", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure client to client route reflection\n"
       "reflection of routes allowed\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, match_tag_cmd_vtysh, 
       "match tag <0-65535>", 
       "Match values from routing table\n"
       "Match tag of route\n"
       "Metric value\n")

DEFSH (VTYSH_BGPD, show_bgp_community4_exact_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, neighbor_remote_as_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "remote-as " "<1-4294967295>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a BGP neighbor\n"
       "AS number\n")

DEFSH (VTYSH_OSPFD, debug_ospf_ism_cmd_vtysh, 
       "debug ospf ism", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Interface State Machine\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_cmd_vtysh, 
       "no lsp-lifetime", 
       "Negate a command or set its defaults\n"
       "LSP lifetime in seconds\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_out_cmd_vtysh, 
       "clear ip bgp peer-group WORD out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_address_cmd_vtysh, 
       "no match ip address", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n")

DEFSH (VTYSH_RIPNGD, ripng_redistribute_type_metric_cmd_vtysh, 
       "redistribute (kernel|connected|static|ospf6|bgp) metric <0-16>", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n")

DEFSH (VTYSH_ISISD, no_spf_interval_cmd_vtysh, 
       "no spf-interval", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_dead_interval_cmd_vtysh, 
       "no ip ospf dead-interval", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_all_cmd_vtysh, 
       "show ip ospf neighbor all", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "include down status neighbor\n")

DEFSH (VTYSH_RIPD, no_rip_redistribute_type_metric_routemap_cmd_vtysh, 
       "no redistribute " "(kernel|connected|static|ospf|isis|bgp)"
       " metric <0-16> route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_seq_le_cmd_vtysh, 
       "no ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M le <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, neighbor_filter_list_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "filter-list WORD (in|out)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Establish BGP filters\n"
       "AS path access-list name\n"
       "Filter incoming routes\n"
       "Filter outgoing routes\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_attr_info_cmd_vtysh, 
       "show ip bgp attribute-info", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "List all bgp attribute information\n")

DEFSH (VTYSH_BGPD, neighbor_strict_capability_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "strict-capability-match", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Strict capability negotiation match\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_type_metric_routemap_cmd_vtysh, 
       "default-information originate always metric-type (1|2) metric <0-16777214> route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_abr_cmd_vtysh, 
       "debug ospf6 abr", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 ABR function\n"
      )

DEFSH (VTYSH_OSPFD, no_ospf_distance_ospf_cmd_vtysh, 
       "no distance ospf", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "OSPF Distance\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbor_flap_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) flap-statistics", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display flap statistics of the routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_prefix_cmd_vtysh, 
       "show ip bgp A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community2_exact_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_type_routemap_cmd_vtysh, 
       "default-information originate always metric-type (1|2) route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_range_cost_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) range A.B.C.D/M cost <0-16777215>", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "User specified metric for this range\n"
       "Advertised metric for this range\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_dampening_prefix_cmd_vtysh, 
       "clear ip bgp dampening A.B.C.D/M", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear route flap dampening information\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_name_standard_all_cmd_vtysh, 
       "no ip extcommunity-list standard WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify standard extcommunity-list\n"
       "Extended Community list name\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_prefix_longer_cmd_vtysh, 
       "show ipv6 prefix-list WORD X:X::X:X/M longer", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Lookup longer prefix\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_prefix_list_cmd_vtysh, 
       "show ip bgp prefix-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes conforming to the prefix-list\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_summary_cmd_vtysh, 
       "show bgp view WORD summary", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_name_cmd_vtysh, 
       "show ip prefix-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_BGPD, no_neighbor_password_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "password", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Set a password\n")

DEFSH (VTYSH_BGPD, debug_bgp_fsm_cmd_vtysh, 
       "debug bgp fsm", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP Finite State Machine\n")

DEFSH (VTYSH_RIPD, send_lifetime_month_day_day_month_cmd_vtysh, 
       "send-lifetime HH:MM:SS MONTH <1-31> <1993-2035> HH:MM:SS <1-31> MONTH <1993-2035>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Day of th month to expire\n"
       "Month of the year to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_all_rsclient_cmd_vtysh, 
       "clear bgp view WORD * rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_mask_as_set_summary_cmd_vtysh, 
       "no aggregate-address A.B.C.D A.B.C.D as-set summary-only", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Generate AS set path information\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_external_intra_inter_cmd_vtysh, 
       "distance ospf external <1-255> intra-area <1-255> inter-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "External routes\n"
       "Distance for external routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n")

DEFSH (VTYSH_RIPD, accept_lifetime_day_month_month_day_cmd_vtysh, 
       "accept-lifetime HH:MM:SS <1-31> MONTH <1993-2035> HH:MM:SS MONTH <1-31> <1993-2035>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Month of the year to expire\n"
       "Day of th month to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_neighbors_cmd_vtysh, 
       "show bgp view WORD neighbors", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_BGPD, no_neighbor_advertise_interval_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "advertisement-interval", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Minimum interval between sending BGP routing updates\n")

DEFSH (VTYSH_BGPD, neighbor_soft_reconfiguration_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "soft-reconfiguration inbound", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Per neighbor soft reconfiguration\n"
       "Allow inbound soft reconfiguration for this neighbor\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_neighbor_detail_cmd_vtysh, 
       "debug ospf6 neighbor (state|event)", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Neighbor\n"
       "Debug OSPFv3 Neighbor State Change\n"
       "Debug OSPFv3 Neighbor Event\n"
      )

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_cmd_vtysh, 
       "default-information originate always", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_damp_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) dampened-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the dampened routes received from neighbor\n")

DEFSH (VTYSH_BGPD, no_bgp_scan_time_cmd_vtysh, 
       "no bgp scan-time", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure background scanner interval\n")

DEFSH (VTYSH_BGPD, undebug_bgp_normal_cmd_vtysh, 
       "undebug bgp", 
       "Disable debugging functions (see also 'debug')\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, no_neighbor_description_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "description", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Neighbor specific description\n")

DEFSH (VTYSH_OSPFD, ospf_auto_cost_reference_bandwidth_cmd_vtysh, 
       "auto-cost reference-bandwidth <1-4294967>", 
       "Calculate OSPF interface cost according to bandwidth\n"
       "Use reference bandwidth method to assign OSPF cost\n"
       "The reference bandwidth in terms of Mbits per second\n")

DEFSH (VTYSH_BGPD, aggregate_address_mask_cmd_vtysh, 
       "aggregate-address A.B.C.D A.B.C.D", 
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n")

DEFSH (VTYSH_BGPD, ip_community_list_expanded_cmd_vtysh, 
       "ip community-list <100-500> (deny|permit) .LINE", 
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (expanded)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_soft_out_cmd_vtysh, 
       "clear bgp external soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, ipv6_bgp_network_ttl_cmd_vtysh, 
       "network X:X::X:X/M pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_ISISD, show_isis_neighbors_detail_cmd_vtysh, 
       "show isis neighbors detail", 
       "Show running system information\n"
       "IS-IS network information\n"
       "IS-IS neighbor adjacencies\n"
       "show detailed information\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_l2_arg_cmd_vtysh, 
       "no lsp-lifetime level-2 <380-65535>", 
       "Negate a command or set its defaults\n"
       "Maximum LSP lifetime for Level 2 only\n"
       "LSP lifetime for Level 2 only in seconds\n")

DEFSH (VTYSH_BGPD, no_match_pathlimit_as_cmd_vtysh, 
       "no match pathlimit as", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "BGP AS-Pathlimit attribute\n"
       "Match Pathlimit AS number\n")

DEFSH (VTYSH_ZEBRA, config_table_cmd_vtysh, 
       "table TABLENO", 
       "Configure target kernel routing table\n"
       "TABLE integer\n")

DEFSH (VTYSH_RIPNGD, ripng_aggregate_address_cmd_vtysh, 
       "aggregate-address X:X::X:X/M", 
       "Set aggregate RIPng route announcement\n"
       "Aggregate network\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_rsclient_cmd_vtysh, 
       "clear bgp ipv6 * rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_route_map_cmd_vtysh, 
       "show ip bgp flap-statistics route-map WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Display routes matching the route-map\n"
       "A route-map to match on\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_cmd_vtysh, 
       "show ipv6 ospf6 route", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       )

DEFSH (VTYSH_BGPD|VTYSH_BGPD, no_neighbor_maximum_prefix_warning_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> warning-only", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Only give warning message when limit is exceeded\n")

DEFSH (VTYSH_OSPF6D, redistribute_ospf6_cmd_vtysh, 
       "redistribute ospf6", 
       "Redistribute control\n"
       "OSPF6 route\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, match_ip_next_hop_prefix_list_cmd_vtysh, 
       "match ip next-hop prefix-list WORD", 
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_ifname_flags_cmd_vtysh, 
       "ipv6 route X:X::X:X/M X:X::X:X INTERFACE (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_ISISD, show_isis_topology_l1_cmd_vtysh, 
       "show isis topology level-1", 
       "Show running system information\n"
       "IS-IS information\n"
       "IS-IS paths to Intermediate Systems\n"
       "Paths to all level-1 routers in the area\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_detail_name_cmd_vtysh, 
       "show ipv6 prefix-list detail WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Detail of prefix lists\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_in_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) in", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_l1_arg_cmd_vtysh, 
       "no lsp-gen-interval level-1 <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 1 only\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged4_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med (as-path|next-hop)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "As-path attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_BGPD, neighbor_default_originate_rmap_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "default-originate route-map WORD", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Originate default route to this neighbor\n"
       "Route-map to specify criteria to originate default\n"
       "route-map name\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_in_cmd_vtysh, 
       "clear ip bgp * in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, match_ip_address_prefix_list_cmd_vtysh, 
       "match ip address prefix-list WORD", 
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_summary_cmd_vtysh, 
       "show ip prefix-list summary", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Summary of prefix lists\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_md5_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(message-digest-key|) <1-255>", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Message digest authentication password (key)\n" "dummy string \n" "Key ID\n" "Use MD5 algorithm\n" "The OSPF password (key)")

DEFSH (VTYSH_OSPFD, no_ospf_area_range_substitute_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) range A.B.C.D/M substitute A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "Announce area range as another prefix\n"
       "Network prefix to be announced instead of range\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_prefix_longer_cmd_vtysh, 
       "show ip bgp A.B.C.D/M longer-prefixes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_ALL, show_ip_access_list_cmd_vtysh, 
       "show ip access-list", 
       "Show running system information\n"
       "IP information\n"
       "List IP access lists\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD|VTYSH_RIPNGD, no_match_metric_cmd_vtysh, 
       "no match metric", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match metric of route\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged2_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path (next-hop|med)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD, set_ip_nexthop_cmd_vtysh, 
       "set ip next-hop A.B.C.D", 
       "Set values in destination routing protocol\n"
       "IP information\n"
       "Next hop address\n"
       "IP address of next hop\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_continue_cmd_vtysh, 
       "continue", 
       "Continue on a different entry within the route-map\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_med3_cmd_vtysh, 
       "bgp bestpath med missing-as-worst confed", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Treat missing MED as the least preferred one\n"
       "Compare MED among confederation paths\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_prefix_match_cmd_vtysh, 
       "show ipv6 ospf6 interface prefix X:X::X:X/M (match|detail)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Display connected prefixes to advertise\n"
       "Display the route\n"
       "Display the route matches the prefix\n"
       "Dispaly details of the prefixes\n"
       )

DEFSH (VTYSH_OSPFD, no_ospf_neighbor_cmd_vtysh, 
       "no neighbor A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor IP address\n")

DEFSH (VTYSH_BGPD, dump_bgp_updates_interval_cmd_vtysh, 
       "dump bgp updates PATH INTERVAL", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump BGP updates only\n"
       "Output filename\n"
       "Interval of output\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_dampening_address_mask_cmd_vtysh, 
       "clear ip bgp dampening A.B.C.D A.B.C.D", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear route flap dampening information\n"
       "Network to clear damping information\n"
       "Network mask\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_adv_router_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) adv-router A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv4_rmap_metric_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf|rip|static) route-map WORD metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, no_ipv6_bgp_network_ttl_cmd_vtysh, 
       "no network X:X::X:X/M pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_ISISD, isis_hello_interval_cmd_vtysh, 
       "isis hello-interval (<1-65535>|minimal)", 
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 seconds,  interval depends on multiplier\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_expanded_all_cmd_vtysh, 
       "no ip extcommunity-list <100-500>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (expanded)\n")

DEFSH (VTYSH_ZEBRA, linkdetect_cmd_vtysh, 
       "link-detect", 
       "Enable link detection on interface\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community2_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_neighbor_advertised_routes_cmd_vtysh, 
       "show ip bgp vpnv4 all neighbors A.B.C.D advertised-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_ISISD, debug_isis_adj_cmd_vtysh, 
       "debug isis adj-packets", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS Adjacency related packets\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_next_hop_val_cmd_vtysh, 
       "no match ip next-hop (<1-199>|<1300-2699>|WORD)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n")

DEFSH (VTYSH_ISISD, area_passwd_cmd_vtysh, 
       "area-password WORD", 
       "Configure the authentication password for an area\n"
       "Area password\n")

DEFSH (VTYSH_BGPD, no_bgp_default_local_preference_cmd_vtysh, 
       "no bgp default local-preference", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure BGP defaults\n"
       "local preference (higher=more preferred)\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPFD, debug_ospf_zebra_sub_cmd_vtysh, 
       "debug ospf zebra (interface|redistribute)", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Zebra information\n"
       "Zebra interface\n"
       "Zebra redistribute\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_neighbor_cmd_vtysh, 
       "debug ospf6 neighbor", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Neighbor\n"
      )

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_cmd_vtysh, 
       "no ipv6 prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv6_metric_rmap_cmd_vtysh, 
       "redistribute (connected|kernel|ospf6|ripng|static) metric <0-4294967295> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_tags_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn tags", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Display BGP tags for prefixes\n")

DEFSH (VTYSH_BGPD, no_neighbor_maximum_prefix_restart_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> restart <1-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Restart bgp connection after limit is exceeded\n"
       "Restart interval in minutes")

DEFSH (VTYSH_RIPD, send_lifetime_month_day_month_day_cmd_vtysh, 
       "send-lifetime HH:MM:SS MONTH <1-31> <1993-2035> HH:MM:SS MONTH <1-31> <1993-2035>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Month of the year to expire\n"
       "Day of th month to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbors_cmd_vtysh, 
       "show bgp neighbors", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_range_advertise_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) range A.B.C.D/M (advertise|not-advertise)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "Advertise this range (default)\n"
       "DoNotAdvertise this range\n")

DEFSH (VTYSH_BGPD, no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> <1-100> warning-only", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Threshold value (%) at which to generate a warning msg\n"
       "Only give warning message when limit is exceeded\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_natural_backdoor_cmd_vtysh, 
       "no network A.B.C.D backdoor", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_OSPF6D, ospf6_routemap_set_forwarding_cmd_vtysh, 
       "set forwarding-address X:X::X:X", 
       "Set value\n"
       "Forwarding Address\n"
       "IPv6 Address\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community_list_cmd_vtysh, 
       "show ipv6 bgp community-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list name\n")

DEFSH (VTYSH_ALL, access_list_extended_any_any_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip any any", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "Any destination host\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_zebra_cmd_vtysh, 
       "debug ospf6 zebra", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug connection between zebra\n"
      )

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, no_neighbor_maximum_prefix_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_in_prefix_filter_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_description_arg_cmd_vtysh, 
       "no ipv6 prefix-list WORD description .LINE", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n"
       "Up to 80 characters describing this prefix-list\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_linkstate_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database linkstate-id A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_network_mask_backdoor_cmd_vtysh, 
       "no network A.B.C.D mask A.B.C.D backdoor", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_lsa_hex_cmd_vtysh, 
       "no debug ospf6 lsa XXXX/0xXXXX", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug Link State Advertisements (LSAs)\n"
       "Specify LS type as Hexadecimal\n"
      )

DEFSH (VTYSH_BGPD, no_neighbor_send_community_type_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "send-community (both|extended|standard)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Send Community attribute to this neighbor\n"
       "Send Standard and Extended Community attributes\n"
       "Send Extended Community attributes\n"
       "Send Standard Community attributes\n")

DEFSH (VTYSH_RIPNGD, ripng_default_information_originate_cmd_vtysh, 
       "default-information originate", 
       "Default route information\n"
       "Distribute default route\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_name_expanded_cmd_vtysh, 
       "no ip extcommunity-list expanded WORD (deny|permit) .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify expanded extcommunity-list\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_distribute_list_prefix_all_cmd_vtysh, 
       "no distribute-list prefix WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_aspath_ignore_cmd_vtysh, 
       "no bgp bestpath as-path ignore", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "AS-path attribute\n"
       "Ignore as-path length in selecting a route\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_prefix_cmd_vtysh, 
       "show ip route A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_OSPFD, mpls_te_on_cmd_vtysh, 
       "mpls-te on", 
       "Configure MPLS-TE parameters\n"
       "Enable the MPLS-TE functionality\n")

DEFSH (VTYSH_BGPD, undebug_bgp_update_cmd_vtysh, 
       "undebug bgp updates", 
       "Disable debugging functions (see also 'debug')\n"
       "BGP information\n"
       "BGP updates\n")

DEFSH (VTYSH_BGPD, no_set_vpnv4_nexthop_val_cmd_vtysh, 
       "no set vpnv4 next-hop A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "VPNv4 information\n"
       "VPNv4 next-hop address\n"
       "IP address of next hop\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_authkey_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) "
       "(authentication-key|) AUTH_KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n"
       "Authentication password (key)\n" "The OSPF password (key)")

DEFSH (VTYSH_OSPFD, no_ospf_rfc1583_flag_cmd_vtysh, 
       "no ospf rfc1583compatibility", 
       "Negate a command or set its defaults\n"
       "OSPF specific commands\n"
       "Disable the RFC1583Compatibility flag\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_asbr_cmd_vtysh, 
       "debug ospf6 asbr", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 ASBR function\n"
      )

DEFSH (VTYSH_BGPD, show_ip_community_list_cmd_vtysh, 
       "show ip community-list", 
       "Show running system information\n"
       "IP information\n"
       "List community-list\n")

DEFSH (VTYSH_BGPD, set_weight_cmd_vtysh, 
       "set weight <0-4294967295>", 
       "Set values in destination routing protocol\n"
       "BGP weight for routing table\n"
       "Weight value\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_ripng_split_horizon_poisoned_reverse_cmd_vtysh, 
       "no ipv6 ripng split-horizon poisoned-reverse", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n"
       "With poisoned-reverse\n")

DEFSH (VTYSH_OSPFD, ip_ospf_transmit_delay_addr_cmd_vtysh, 
       "ip ospf transmit-delay <1-65535> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Link state transmit delay\n"
       "Seconds\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv4_rmap_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf|rip|static) route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ISISD, no_isis_hello_cmd_vtysh, 
       "no isis hello padding", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Add padding to IS-IS hello packets\n"
       "Pad hello packets\n"
       "<cr>\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_type_cmd_vtysh, 
       "default-information originate always metric-type (1|2)", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_OSPFD, no_ospf_passive_interface_addr_cmd_vtysh, 
       "no passive-interface IFNAME A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Allow routing updates on an interface\n"
       "Interface's name\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_in_prefix_filter_cmd_vtysh, 
       "clear bgp ipv6 external in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, no_match_tag_val_cmd_vtysh, 
       "no match tag <0-65535>", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match tag of route\n"
       "Metric value\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_name_standard_all_cmd_vtysh, 
       "no ip community-list standard WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Add a standard community-list entry\n"
       "Community list name\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_packet_send_recv_cmd_vtysh, 
       "no debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all) (send|recv|detail)", 
       "Negate a command or set its defaults\n"
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n"
       "Packet sent\n"
       "Packet received\n"
       "Detail Information\n")

DEFSH (VTYSH_OSPFD, no_ospf_refresh_timer_cmd_vtysh, 
       "no refresh timer", 
       "Adjust refresh parameters\n"
       "Unset refresh timer\n")

DEFSH (VTYSH_BGPD, undebug_bgp_filter_cmd_vtysh, 
       "undebug bgp filters", 
       "Disable debugging functions (see also 'debug')\n"
       "BGP information\n"
       "BGP filters\n")

DEFSH (VTYSH_RIPD, accept_lifetime_duration_month_day_cmd_vtysh, 
       "accept-lifetime HH:MM:SS MONTH <1-31> <1993-2035> duration <1-2147483646>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Duration of the key\n"
       "Duration seconds\n")

DEFSH (VTYSH_OSPFD, no_ospf_router_id_cmd_vtysh, 
       "no ospf router-id", 
       "Negate a command or set its defaults\n"
       "OSPF specific commands\n"
       "router-id for the OSPF process\n")

DEFSH (VTYSH_ALL, no_access_list_exact_cmd_vtysh, 
       "no access-list WORD (deny|permit) A.B.C.D/M exact-match", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n"
       "Exact match of the prefixes\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_kernel_cmd_vtysh, 
       "debug zebra kernel", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_cmd_vtysh, 
       "show ip ospf", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n")

DEFSH (VTYSH_BGPD, neighbor_unsuppress_map_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "unsuppress-map WORD", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Route-map to selectively unsuppress suppressed routes\n"
       "Name of route map\n")

DEFSH (VTYSH_ZEBRA, no_set_src_cmd_vtysh, 
       "no set src", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Source address for route\n")

DEFSH (VTYSH_ISISD, debug_isis_lupd_cmd_vtysh, 
       "debug isis local-updates", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS local update packets\n")

DEFSH (VTYSH_OSPFD, ospf_retransmit_interval_cmd_vtysh, 
       "ospf retransmit-interval <3-65535>", 
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n"
       "Seconds\n")

DEFSH (VTYSH_RIPD, no_rip_default_information_originate_cmd_vtysh, 
       "no default-information originate", 
       "Negate a command or set its defaults\n"
       "Control distribution of default route\n"
       "Distribute a default route\n")

DEFSH (VTYSH_BGPD, no_dump_bgp_all_cmd_vtysh, 
       "no dump bgp all [PATH] [INTERVAL]", 
       "Negate a command or set its defaults\n"
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump all BGP packets\n")

DEFSH (VTYSH_BGPD, neighbor_advertise_interval_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "advertisement-interval <0-600>", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Minimum interval between sending BGP routing updates\n"
       "time in seconds\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_interface_cmd_vtysh, 
       "show ip ospf interface [INTERFACE]", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Interface information\n"
       "Interface name\n")

DEFSH (VTYSH_ALL, access_list_cmd_vtysh, 
       "access-list WORD (deny|permit) A.B.C.D/M", 
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n")

DEFSH (VTYSH_RIPD, rip_distance_cmd_vtysh, 
       "distance <1-255>", 
       "Administrative distance\n"
       "Distance value\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp * in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_detail_cmd_vtysh, 
       "show ipv6 prefix-list detail", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Detail of prefix lists\n")

DEFSH (VTYSH_BGPD, neighbor_dont_capability_negotiate_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "dont-capability-negotiate", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Do not perform capability negotiation\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_cmd_vtysh, 
       "no ip prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, no_match_tag_cmd_vtysh, 
       "no match tag", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match tag of route\n")

DEFSH (VTYSH_BGPD, no_bgp_default_ipv4_unicast_cmd_vtysh, 
       "no bgp default ipv4-unicast", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure BGP defaults\n"
       "Activate ipv4-unicast for a peer by default\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_route_cmd_vtysh, 
       "show bgp ipv6 X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, match_peer_local_cmd_vtysh, 
        "match peer local", 
        "Match values from routing table\n"
        "Match peer address\n"
        "Static or Redistributed routes\n")

DEFSH (VTYSH_RIPD, accept_lifetime_month_day_month_day_cmd_vtysh, 
       "accept-lifetime HH:MM:SS MONTH <1-31> <1993-2035> HH:MM:SS MONTH <1-31> <1993-2035>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Month of the year to expire\n"
       "Day of th month to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_expanded_cmd_vtysh, 
       "no ip extcommunity-list <100-500> (deny|permit) .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (expanded)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_seq_ge_cmd_vtysh, 
       "ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M ge <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community_list_exact_cmd_vtysh, 
       "show ipv6 mbgp community-list WORD exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the community-list\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_OSPFD, no_ospf_default_information_originate_cmd_vtysh, 
       "no default-information originate", 
       "Negate a command or set its defaults\n"
       "Control distribution of default information\n"
       "Distribute a default route\n")

DEFSH (VTYSH_BGPD, bgp_log_neighbor_changes_cmd_vtysh, 
       "bgp log-neighbor-changes", 
       "BGP specific commands\n"
       "Log neighbor up/down and reset reason\n")

DEFSH (VTYSH_BGPD, match_ipv6_address_cmd_vtysh, 
       "match ipv6 address WORD", 
       "Match values from routing table\n"
       "IPv6 information\n"
       "Match IPv6 address of route\n"
       "IPv6 access-list name\n")

DEFSH (VTYSH_BGPD, no_neighbor_activate_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "activate", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Enable the Address Family for this Neighbor\n")

DEFSH (VTYSH_ALL, access_list_standard_nomask_cmd_vtysh, 
       "access-list (<1-99>|<1300-1999>) (deny|permit) A.B.C.D", 
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Address to match\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_border_routers_cmd_vtysh, 
       "show ip ospf border-routers", 
       "Show running system information\n"
       "IP information\n"
       "show all the ABR's and ASBR's\n"
       "for this area\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_in_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, no_rip_timers_cmd_vtysh, 
       "no timers basic", 
       "Negate a command or set its defaults\n"
       "Adjust routing timers\n"
       "Basic routing protocol update timers\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_brouter_area_cmd_vtysh, 
       "debug ospf6 border-routers area-id A.B.C.D", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
       "Debug border routers in specific Area\n"
       "Specify Area-ID\n"
      )

DEFSH (VTYSH_BGPD, no_set_community_cmd_vtysh, 
       "no set community", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP community attribute\n")

DEFSH (VTYSH_OSPFD, ospf_distance_cmd_vtysh, 
       "distance <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n")

DEFSH (VTYSH_BGPD, no_ipv6_bgp_network_cmd_vtysh, 
       "no network X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, no_neighbor_passive_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "passive", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Don't send open messages to this neighbor\n")

DEFSH (VTYSH_ISISD, no_isis_priority_l1_arg_cmd_vtysh, 
       "no isis priority <0-127> level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n"
       "Specify priority for level-1 routing\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_vpnv4_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D vpnv4 unicast out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ALL, access_list_remark_cmd_vtysh, 
       "access-list (<1-99>|<100-199>|<1300-1999>|<2000-2699>|WORD) remark .LINE", 
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP extended access list\n"
       "IP standard access list (expanded range)\n"
       "IP extended access list (expanded range)\n"
       "IP zebra access-list\n"
       "Access list entry comment\n"
       "Comment up to 100 characters\n")

DEFSH (VTYSH_ISISD, isis_hello_interval_l1_cmd_vtysh, 
       "isis hello-interval (<1-65535>|minimal) level-1", 
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 second,  interval depends on multiplier\n"
       "Specify hello-interval for level-1 IIHs\n")

DEFSH (VTYSH_RIPNGD, show_ipv6_ripng_cmd_vtysh, 
       "show ipv6 ripng", 
       "Show running system information\n"
       "IPv6 information\n"
       "Show RIPng routes\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_soft_out_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_prefix_cmd_vtysh, 
       "show bgp view WORD ipv6 X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, no_match_peer_local_cmd_vtysh, 
       "no match peer local", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match peer address\n"
       "Static or Redistributed routes\n")

DEFSH (VTYSH_OSPFD, no_set_metric_type_cmd_vtysh, 
       "no set metric-type", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Type of metric for destination routing protocol\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv4_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf|rip|static)", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n")

DEFSH (VTYSH_BGPD, no_match_ipv6_next_hop_cmd_vtysh, 
       "no match ipv6 next-hop X:X::X:X", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IPv6 information\n"
       "Match IPv6 next-hop address of route\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_ZEBRA, ip_route_flags2_cmd_vtysh, 
       "ip route A.B.C.D/M (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_OSPFD, ip_ospf_mtu_ignore_addr_cmd_vtysh, 
       "ip ospf mtu-ignore A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Disable mtu mismatch detection\n"
       "Address of interface")

DEFSH (VTYSH_OSPF6D, debug_ospf6_spf_time_cmd_vtysh, 
       "debug ospf6 spf time", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug SPF Calculation\n"
       "Measure time taken by SPF Calculation\n"
      )

DEFSH (VTYSH_BGPD|VTYSH_RIPD, no_set_ip_nexthop_val_cmd_vtysh, 
       "no set ip next-hop A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IP information\n"
       "Next hop address\n"
       "IP address of next hop\n")

DEFSH (VTYSH_RIPNGD, ipv6_distribute_list_prefix_cmd_vtysh, 
       "distribute-list prefix WORD (in|out) WORD", 
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_l1_arg_cmd_vtysh, 
       "no isis hello-multiplier <3-1000> level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n"
       "Specify hello multiplier for level-1 IIHs\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_standard_cmd_vtysh, 
       "no ip community-list <1-99> (deny|permit) .AA:NN", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Community number in aa:nn format or internet|local-AS|no-advertise|no-export\n")

DEFSH (VTYSH_RIPD, send_lifetime_duration_month_day_cmd_vtysh, 
       "send-lifetime HH:MM:SS MONTH <1-31> <1993-2035> duration <1-2147483646>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Duration of the key\n"
       "Duration seconds\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_rsclient_summary_cmd_vtysh, 
       "show bgp view WORD rsclient summary", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community4_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_call_cmd_vtysh, 
       "call WORD", 
       "Jump to another Route-Map after match+set\n"
       "Target route-map name\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_prefix_longer_cmd_vtysh, 
       "show ip prefix-list WORD A.B.C.D/M longer", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Lookup longer prefix\n")

DEFSH (VTYSH_OSPFD, no_router_ospf_id_cmd_vtysh, 
       "no router-id", 
       "Negate a command or set its defaults\n"
       "router-id for the OSPF process\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_cmd_vtysh, 
       "network A.B.C.D mask A.B.C.D", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n")

DEFSH (VTYSH_BGPD, no_match_ip_route_source_prefix_list_val_cmd_vtysh, 
       "no match ip route-source prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_cost_cmd_vtysh, 
       "no ip ospf cost", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_detail_name_cmd_vtysh, 
       "show ip prefix-list detail WORD", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Detail of prefix lists\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_RIPD, ip_rip_split_horizon_cmd_vtysh, 
       "ip rip split-horizon", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_flags_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_l2_arg_cmd_vtysh, 
       "no isis hello-multiplier <3-1000> level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n"
       "Specify hello multiplier for level-2 IIHs\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_detail_all_cmd_vtysh, 
       "show ip ospf neighbor detail all", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "detail of all neighbors\n"
       "include down status neighbor\n")

DEFSH (VTYSH_ISISD, no_isis_metric_cmd_vtysh, 
       "no isis metric", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set default metric for circuit\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_spf_database_cmd_vtysh, 
       "no debug ospf6 spf database", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug SPF Calculation\n"
       "Quit Logging number of LSAs at SPF Calculation time\n"
      )

DEFSH (VTYSH_OSPFD, debug_ospf_packet_send_recv_detail_cmd_vtysh, 
       "debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all) (send|recv) (detail|)", 
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n"
       "Packet sent\n"
       "Packet received\n"
       "Detail Information\n")

DEFSH (VTYSH_BGPD, bgp_network_backdoor_cmd_vtysh, 
       "network A.B.C.D/M backdoor", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv6_metric_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf6|ripng|static) metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_cmd_vtysh, 
       "clear ip bgp view WORD *", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n")

DEFSH (VTYSH_BGPD, no_match_ecommunity_val_cmd_vtysh, 
       "no match extcommunity (<1-99>|<100-500>|WORD)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP/VPN extended community list\n"
       "Extended community-list number (standard)\n"
       "Extended community-list number (expanded)\n"
       "Extended community-list name\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPNGD, no_set_ipv6_nexthop_local_cmd_vtysh, 
       "no set ipv6 next-hop local", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 local address\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_route_cmd_vtysh, 
       "show ip ospf route", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "OSPF routing table\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_prefix_longer_cmd_vtysh, 
       "show ip bgp flap-statistics A.B.C.D/M longer-prefixes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_BGPD, no_set_weight_cmd_vtysh, 
       "no set weight", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP weight for routing table\n")

DEFSH (VTYSH_BGPD, neighbor_distribute_list_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "distribute-list (<1-199>|<1300-2699>|WORD) (in|out)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Filter updates to/from this neighbor\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n"
       "Filter incoming updates\n"
       "Filter outgoing updates\n")

DEFSH (VTYSH_BGPD, no_match_community_cmd_vtysh, 
       "no match community", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP community list\n")

DEFSH (VTYSH_RIPNGD, ripng_redistribute_type_routemap_cmd_vtysh, 
       "redistribute (kernel|connected|static|ospf6|bgp) route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, no_set_aggregator_as_cmd_vtysh, 
       "no set aggregator as", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP aggregator attribute\n"
       "AS number of aggregator\n")

DEFSH (VTYSH_BGPD, bgp_damp_set2_cmd_vtysh, 
       "bgp dampening <1-45>", 
       "BGP Specific commands\n"
       "Enable route-flap dampening\n"
       "Half-life time for the penalty\n")

DEFSH (VTYSH_BGPD, show_bgp_rsclient_prefix_cmd_vtysh, 
       "show bgp rsclient (A.B.C.D|X:X::X:X) X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_BGPD, ipv6_mbgp_neighbor_received_routes_cmd_vtysh, 
       "show ipv6 mbgp neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_statistics_cmd_vtysh, 
       "show ip bgp flap-statistics", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_vpnv4_soft_cmd_vtysh, 
       "clear ip bgp A.B.C.D vpnv4 unicast soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community3_exact_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPFD, ospf_area_export_list_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) export-list NAME", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Set the filter for networks announced to other areas\n"
       "Name of the access-list\n")

DEFSH (VTYSH_RIPNGD, debug_ripng_packet_cmd_vtysh, 
       "debug ripng packet", 
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng packet\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_routes_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_OSPFD, ospf_area_authentication_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) authentication", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Enable authentication\n")

DEFSH (VTYSH_RIPNGD, no_ripng_timers_cmd_vtysh, 
       "no timers basic", 
       "Negate a command or set its defaults\n"
       "RIPng timers setup\n"
       "Basic timer\n")

DEFSH (VTYSH_RIPNGD, ripng_route_cmd_vtysh, 
       "route IPV6ADDR", 
       "Static route setup\n"
       "Set static RIPng route announcement\n")

DEFSH (VTYSH_ISISD, no_spf_interval_arg_cmd_vtysh, 
       "no spf-interval <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_OSPFD, ospf_passive_interface_addr_cmd_vtysh, 
       "passive-interface IFNAME A.B.C.D", 
       "Suppress routing updates on an interface\n"
       "Interface's name\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_external_inter_intra_cmd_vtysh, 
       "distance ospf external <1-255> inter-area <1-255> intra-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "External routes\n"
       "Distance for external routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n")

DEFSH (VTYSH_OSPFD, ip_ospf_hello_interval_addr_cmd_vtysh, 
       "ip ospf hello-interval <1-65535> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between HELLO packets\n"
       "Seconds\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_filter_list_cmd_vtysh, 
       "show bgp ipv6 filter-list WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_BGPD, debug_bgp_update_cmd_vtysh, 
       "debug bgp updates", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP updates\n")

DEFSH (VTYSH_BGPD, show_bgp_cmd_vtysh, 
       "show bgp", 
       "Show running system information\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, show_bgp_filter_list_cmd_vtysh, 
       "show bgp filter-list WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_adv_router_linkstate_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) "
       "adv-router A.B.C.D linkstate-id A.B.C.D "
       "(dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, clear_bgp_peer_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X)", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n")

DEFSH (VTYSH_ZEBRA, show_interface_desc_cmd_vtysh, 
       "show interface description", 
       "Show running system information\n"
       "Interface status and configuration\n"
       "Interface description\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbor_received_routes_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_flags_pref_cmd_vtysh, 
       "ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_seq_ge_cmd_vtysh, 
       "ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M ge <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_soft_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_redistribute_cmd_vtysh, 
       "show ipv6 ospf6 redistribute", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "redistributing External information\n"
       )

DEFSH (VTYSH_ZEBRA, ip_irdp_multicast_cmd_vtysh, 
       "ip irdp multicast", 
       "IP information\n"
       "ICMP Router discovery on this interface using multicast\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_address_prefix_list_val_cmd_vtysh, 
       "no match ip address prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_RIPD, ip_rip_authentication_string_cmd_vtysh, 
       "ip rip authentication string LINE", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication string\n"
       "Authentication string\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_soft_cmd_vtysh, 
       "clear bgp peer-group WORD soft", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, bgp_timers_cmd_vtysh, 
       "timers bgp <0-65535> <0-65535>", 
       "Adjust routing timers\n"
       "BGP timers\n"
       "Keepalive interval\n"
       "Holdtime\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_RIPD|VTYSH_RIPNGD, set_metric_cmd_vtysh, 
       "set metric <0-4294967295>", 
       "Set value\n"
       "Metric value for destination routing protocol\n"
       "Metric value\n")

DEFSH (VTYSH_OSPFD, debug_ospf_nsm_sub_cmd_vtysh, 
       "debug ospf nsm (status|events|timers)", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Neighbor State Machine\n"
       "NSM Status Information\n"
       "NSM Event Information\n"
       "NSM Timer Information\n")

DEFSH (VTYSH_RIPNGD, no_ripng_default_information_originate_cmd_vtysh, 
       "no default-information originate", 
       "Negate a command or set its defaults\n"
       "Default route information\n"
       "Distribute default route\n")

DEFSH (VTYSH_BGPD, no_ip_as_path_all_cmd_vtysh, 
       "no ip as-path access-list WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "BGP autonomous system path filter\n"
       "Specify an access list name\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_RIPD, rip_redistribute_type_metric_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|ospf|isis|bgp)" " metric <0-16> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, match_origin_cmd_vtysh, 
       "match origin (egp|igp|incomplete)", 
       "Match values from routing table\n"
       "BGP origin code\n"
       "remote EGP\n"
       "local IGP\n"
       "unknown heritage\n")

DEFSH (VTYSH_ALL, no_access_list_extended_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D A.B.C.D A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_md5_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) "
       "(message-digest-key|) <1-255> md5 KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n"
       "Message digest authentication password (key)\n" "dummy string \n" "Key ID\n" "Use MD5 algorithm\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, no_neighbor_timers_connect_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "timers connect", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "BGP per neighbor timers\n"
       "BGP connect timer\n")

DEFSH (VTYSH_BGPD, set_aspath_prepend_cmd_vtysh, 
       "set as-path prepend ." "<1-4294967295>", 
       "Set values in destination routing protocol\n"
       "Transform BGP AS_PATH attribute\n"
       "Prepend to the as-path\n"
       "AS number\n")

DEFSH (VTYSH_OSPFD, ospf_area_nssa_translate_no_summary_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) nssa (translate-candidate|translate-never|translate-always) no-summary", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n"
       "Configure NSSA-ABR for translate election (default)\n"
       "Configure NSSA-ABR to never translate\n"
       "Configure NSSA-ABR to always translate\n"
       "Do not inject inter-area routes into nssa\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_in_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_received_routes_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_regexp_cmd_vtysh, 
       "show ip bgp regexp .LINE", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_BGPD, no_neighbor_weight_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "weight", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Set default weight for routes from this neighbor\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_spf_database_cmd_vtysh, 
       "debug ospf6 spf database", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug SPF Calculation\n"
       "Log number of LSAs at SPF Calculation time\n"
      )

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv4_rmap_cmd_vtysh, 
       "redistribute (connected|kernel|ospf|rip|static) route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_in_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, dump_bgp_routes_interval_cmd_vtysh, 
       "dump bgp routes-mrt PATH INTERVAL", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump whole BGP routing table\n"
       "Output filename\n"
       "Interval of output\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_pref_cmd_vtysh, 
       "ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_RIPD, accept_lifetime_duration_day_month_cmd_vtysh, 
       "accept-lifetime HH:MM:SS <1-31> MONTH <1993-2035> duration <1-2147483646>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Duration of the key\n"
       "Duration seconds\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_dampened_paths_cmd_vtysh, 
       "show ip bgp dampened-paths", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display paths suppressed due to dampening\n")

DEFSH (VTYSH_OSPFD, ip_ospf_mtu_ignore_cmd_vtysh, 
      "ip ospf mtu-ignore", 
      "IP Information\n"
      "OSPF interface commands\n"
      "Disable mtu mismatch detection\n")

DEFSH (VTYSH_RIPD, no_ip_rip_send_version_num_cmd_vtysh, 
       "no ip rip send version (1|2)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement transmission\n"
       "Version control\n"
       "Version 1\n"
       "Version 2\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_in_cmd_vtysh, 
       "clear ip bgp peer-group WORD in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, distribute_list_all_cmd_vtysh, 
       "distribute-list WORD (in|out)", 
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_BGPD, show_bgp_view_cmd_vtysh, 
       "show bgp view WORD", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n")

DEFSH (VTYSH_OSPFD, ip_ospf_dead_interval_minimal_addr_cmd_vtysh, 
       "ip ospf dead-interval minimal hello-multiplier <1-10> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Minimal 1s dead-interval with fast sub-second hellos\n"
       "Hello multiplier factor\n"
       "Number of Hellos to send each second\n"
       "Address of interface\n")

DEFSH (VTYSH_BGPD, no_neighbor_nexthop_self_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "next-hop-self", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Disable the next hop calculation for this neighbor\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_authentication_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) authentication", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Enable authentication\n")

DEFSH (VTYSH_BGPD, debug_bgp_as4_segment_cmd_vtysh, 
       "debug bgp as4 segment", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 aspath segment handling\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_cmd_vtysh, 
       "show ipv6 ospf6 database", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
      )

DEFSH (VTYSH_ZEBRA, show_ipv6_route_cmd_vtysh, 
       "show ipv6 route", 
       "Show running system information\n"
       "IP information\n"
       "IPv6 routing table\n")

DEFSH (VTYSH_OSPFD, no_ospf_redistribute_source_cmd_vtysh, 
       "no redistribute " "(kernel|connected|static|rip|isis|bgp)", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_OSPFD, ospf_network_cmd_vtysh, 
       "ospf network (broadcast|non-broadcast|point-to-multipoint|point-to-point)", 
       "OSPF interface commands\n"
       "Network type\n"
       "Specify OSPF broadcast multi-access network\n"
       "Specify OSPF NBMA network\n"
       "Specify OSPF point-to-multipoint network\n"
       "Specify OSPF point-to-point network\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_out_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_tags_cmd_vtysh, 
       "show ip bgp vpnv4 all tags", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Display BGP tags for prefixes\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_fsm_cmd_vtysh, 
       "no debug bgp fsm", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "Finite State Machine\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_prefix_cmd_vtysh, 
       "show ip bgp flap-statistics A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_BGPD, match_ip_route_source_cmd_vtysh, 
       "match ip route-source (<1-199>|<1300-2699>|WORD)", 
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP standard access-list name\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_natural_ttl_cmd_vtysh, 
       "network A.B.C.D pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_in_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, no_rip_redistribute_rip_cmd_vtysh, 
       "no redistribute rip", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Routing Information Protocol (RIP)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_route_cmd_vtysh, 
       "show ip bgp view WORD A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_ZEBRA, no_debug_zebra_events_cmd_vtysh, 
       "no debug zebra events", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra events\n")

DEFSH (VTYSH_OSPFD, no_ospf_passive_interface_cmd_vtysh, 
       "no passive-interface IFNAME", 
       "Negate a command or set its defaults\n"
       "Allow routing updates on an interface\n"
       "Interface's name\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_seq_le_cmd_vtysh, 
       "ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M le <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, no_set_ecommunity_rt_val_cmd_vtysh, 
       "no set extcommunity rt .ASN:nn_or_IP-address:nn", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Route Target extended community\n"
       "VPN extended community\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbors_cmd_vtysh, 
       "show ip bgp neighbors", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_OSPFD, no_ospf_distance_cmd_vtysh, 
       "no distance <1-255>", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "OSPF Administrative distance\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_natural_cmd_vtysh, 
       "network A.B.C.D", 
       "Specify a network to announce via BGP\n"
       "Network number\n")

DEFSH (VTYSH_BGPD, debug_bgp_events_cmd_vtysh, 
       "debug bgp events", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP events\n")

DEFSH (VTYSH_BGPD, set_pathlimit_ttl_cmd_vtysh, 
       "set pathlimit ttl <1-255>", 
       "Set values in destination routing protocol\n"
       "BGP AS-Pathlimit attribute\n"
       "Set AS-Path Hop-count TTL\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_rib_q_cmd_vtysh, 
       "debug zebra rib queue", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug RIB events\n"
       "Debug RIB queueing\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_soft_in_cmd_vtysh, 
       "clear bgp * soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_self_originated_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D self-originated "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display Self-originated LSAs\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_ge_le_cmd_vtysh, 
       "ip prefix-list WORD (deny|permit) A.B.C.D/M ge <0-32> le <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, bgp_network_backdoor_ttl_cmd_vtysh, 
       "network A.B.C.D/M backdoor pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_in_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_rib_cmd_vtysh, 
       "debug zebra rib", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug RIB events\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_paths_cmd_vtysh, 
       "show ip bgp paths", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Path information\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPFD, no_router_ospf_cmd_vtysh, 
       "no router ospf", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n"
       "Start OSPF configuration\n")

DEFSH (VTYSH_OSPFD, ip_ospf_transmit_delay_cmd_vtysh, 
       "ip ospf transmit-delay <1-65535>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Link state transmit delay\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, no_bgp_scan_time_val_cmd_vtysh, 
       "no bgp scan-time <5-60>", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure background scanner interval\n"
       "Scanner interval (seconds)\n")

DEFSH (VTYSH_ISISD, isis_hello_multiplier_l1_cmd_vtysh, 
       "isis hello-multiplier <3-1000> level-1", 
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n"
       "Specify hello multiplier for level-1 IIHs\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_all_cmd_vtysh, 
       "no debug all bgp", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Enable all debugging\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_address_cmd_vtysh, 
       "show ip bgp flap-statistics A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_OSPFD|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_ZEBRA, no_match_interface_val_cmd_vtysh, 
       "no match interface WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match first hop interface of route\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, no_bgp_cluster_id_arg_cmd_vtysh, 
       "no bgp cluster-id A.B.C.D", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Configure Route-Reflector Cluster-id\n"
       "Route-Reflector Cluster-id in IP address format\n")

DEFSH (VTYSH_ZEBRA, router_id_cmd_vtysh, 
       "router-id A.B.C.D", 
       "Manually set the router-id\n"
       "IP address to use for router-id\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_spf_time_cmd_vtysh, 
       "no debug ospf6 spf time", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Quit Debugging SPF Calculation\n"
       "Quit Measuring time taken by SPF Calculation\n"
      )

DEFSH (VTYSH_ALL, no_access_list_extended_any_host_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip any host A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_RIPD, no_rip_version_val_cmd_vtysh, 
       "no version <1-2>", 
       "Negate a command or set its defaults\n"
       "Set routing protocol version\n"
       "version\n")

DEFSH (VTYSH_BGPD, undebug_bgp_zebra_cmd_vtysh, 
       "undebug bgp zebra", 
       "Disable debugging functions (see also 'debug')\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP Zebra messages\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbors_peer_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_seq_ge_le_cmd_vtysh, 
       "ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M ge <0-128> le <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_ISISD, is_type_cmd_vtysh, 
       "is-type (level-1|level-1-2|level-2-only)", 
       "IS Level for this routing process (OSI only)\n"
       "Act as a station router only\n"
       "Act as both a station router and an area router\n"
       "Act as an area router only\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_inter_cmd_vtysh, 
       "distance ospf inter-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_as4_segment_cmd_vtysh, 
       "no debug bgp as4 segment", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 aspath segment handling\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, set_community_cmd_vtysh, 
       "set community .AA:NN", 
       "Set values in destination routing protocol\n"
       "BGP community attribute\n"
       "Community number in aa:nn format or local-AS|no-advertise|no-export|internet or additive\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_name_cmd_vtysh, 
       "show ipv6 prefix-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_natural_cmd_vtysh, 
       "no network A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n")

DEFSH (VTYSH_RIPD, no_rip_passive_interface_cmd_vtysh, 
       "no passive-interface (IFNAME|default)", 
       "Negate a command or set its defaults\n"
       "Suppress routing updates on an interface\n"
       "Interface name\n"
       "default for all interfaces\n")

DEFSH (VTYSH_RIPD, no_key_chain_cmd_vtysh, 
       "no key chain WORD", 
       "Negate a command or set its defaults\n"
       "Authentication key management\n"
       "Key-chain management\n"
       "Key-chain name\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_RIPNGD, no_set_metric_cmd_vtysh, 
       "no set metric", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Metric value for destination routing protocol\n")

DEFSH (VTYSH_BGPD, no_match_ip_route_source_cmd_vtysh, 
       "no match ip route-source", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_l2_arg_cmd_vtysh, 
       "no lsp-gen-interval level-2 <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 2 only\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community3_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_in_prefix_filter_cmd_vtysh, 
       "clear bgp external in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_received_prefix_filter_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_RIPD, if_rmap_cmd_vtysh, 
       "route-map RMAP_NAME (in|out) IFNAME", 
       "Route map set\n"
       "Route map name\n"
       "Route map set for input filtering\n"
       "Route map set for output filtering\n"
       "Route map interface name\n")

DEFSH (VTYSH_RIPD, no_debug_rip_events_cmd_vtysh, 
       "no debug rip events", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP events\n")

DEFSH (VTYSH_ISISD, show_clns_neighbors_detail_cmd_vtysh, 
       "show clns neighbors detail", 
       "Show running system information\n"
       "clns network information\n"
       "CLNS neighbor adjacencies\n"
       "show detailed information\n")

DEFSH (VTYSH_BGPD, undebug_bgp_events_cmd_vtysh, 
       "undebug bgp events", 
       "Disable debugging functions (see also 'debug')\n"
       "BGP information\n"
       "BGP events\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp view WORD * ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_in_cmd_vtysh, 
       "clear bgp external in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_out_cmd_vtysh, 
       "clear bgp external out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ALL, no_access_list_cmd_vtysh, 
       "no access-list WORD (deny|permit) A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_id_self_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" ") A.B.C.D (self-originate|)", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "Link State ID (as an IP address)\n"
       "Self-originated link states\n"
       "\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_cmd_vtysh, 
       "show ip bgp", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, no_neighbor_remove_private_as_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "remove-private-AS", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Remove private AS number from outbound updates\n")

DEFSH (VTYSH_ZEBRA, ip_forwarding_cmd_vtysh, 
       "ip forwarding", 
       "IP information\n"
       "Turn on IP forwarding")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_self_originated_linkstate_id_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) self-originated "
       "linkstate-id A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display Self-originated LSAs\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_neighbor_routes_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn neighbors A.B.C.D routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, neighbor_local_as_no_prepend_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "local-as " "<1-4294967295>" " no-prepend", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a local-as number\n"
       "AS number used as local AS\n"
       "Do not prepend local-as to updates from ebgp peers\n")

DEFSH (VTYSH_ISISD, no_spf_interval_l2_arg_cmd_vtysh, 
       "no spf-interval level-2 <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n"
       "Set interval for level 2 only\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_BGPD, bgp_router_id_cmd_vtysh, 
       "bgp router-id A.B.C.D", 
       "BGP information\n"
       "Override configured router identifier\n"
       "Manually configured router identifier\n")

DEFSH (VTYSH_BGPD, aggregate_address_summary_as_set_cmd_vtysh, 
       "aggregate-address A.B.C.D/M summary-only as-set", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_BGPD, neighbor_port_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "port <0-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Neighbor's BGP port\n"
       "TCP port number\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_type_metric_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
         " metric-type (1|2) metric <0-16777214>", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_external_cmd_vtysh, 
       "distance ospf external <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "External routes\n"
       "Distance for external routes\n")

DEFSH (VTYSH_OSPFD, ospf_log_adjacency_changes_cmd_vtysh, 
       "log-adjacency-changes", 
       "Log changes in adjacency state\n")

DEFSH (VTYSH_OSPFD, mpls_te_link_rsc_clsclr_cmd_vtysh, 
       "mpls-te link rsc-clsclr BITPATTERN", 
       "MPLS-TE specific commands\n"
       "Configure MPLS-TE link parameters\n"
       "Administrative group membership\n"
       "32-bit Hexadecimal value (ex. 0xa1)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_list_cmd_vtysh, 
       "show ip bgp community-list (<1-500>|WORD)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_metric_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)" " metric <0-16777214>", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_soft_cmd_vtysh, 
       "clear bgp ipv6 * soft", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_rsclient_cmd_vtysh, 
       "clear ip bgp (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_prefix_detail_cmd_vtysh, 
       "show ipv6 ospf6 interface prefix (X:X::X:X|X:X::X:X/M|detail)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Display connected prefixes to advertise\n"
       "Display the route bestmatches the address\n"
       "Display the route\n"
       "Dispaly details of the prefixes\n"
       )

DEFSH (VTYSH_OSPFD, no_ospf_log_adjacency_changes_cmd_vtysh, 
       "no log-adjacency-changes", 
       "Negate a command or set its defaults\n"
       "Log changes in adjacency state\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_sequence_number_cmd_vtysh, 
       "ip prefix-list sequence-number", 
       "IP information\n"
       "Build a prefix list\n"
       "Include/exclude sequence numbers in NVGEN\n")

DEFSH (VTYSH_OSPFD, ospf_neighbor_priority_cmd_vtysh, 
       "neighbor A.B.C.D priority <0-255>", 
       "Specify neighbor router\n"
       "Neighbor IP address\n"
       "Neighbor Priority\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> <1-100> restart <1-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Threshold value (%) at which to generate a warning msg\n"
       "Restart bgp connection after limit is exceeded\n"
       "Restart interval in minutes")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_warning_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> warning-only", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Only give warning message when limit is exceeded\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_next_hop_prefix_list_val_cmd_vtysh, 
       "no match ip next-hop prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_BGPD, undebug_bgp_all_cmd_vtysh, 
       "undebug all bgp", 
       "Disable debugging functions (see also 'debug')\n"
       "Enable all debugging\n"
       "BGP information\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_self_originated_detail_cmd_vtysh, 
       "show ipv6 ospf6 database self-originated "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Self-originated LSAs\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPFD, no_ospf_default_metric_val_cmd_vtysh, 
       "no default-metric <0-16777214>", 
       "Negate a command or set its defaults\n"
       "Set metric of redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_routemap_cmd_vtysh, 
       "default-information originate always route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ZEBRA, no_ip_irdp_shutdown_cmd_vtysh, 
       "no ip irdp shutdown", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "ICMP Router discovery no shutdown on this interface\n")

DEFSH (VTYSH_RIPD, ip_rip_authentication_key_chain_cmd_vtysh, 
       "ip rip authentication key-chain LINE", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication key-chain\n"
       "name of key-chain\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_le_cmd_vtysh, 
       "no ipv6 prefix-list WORD (deny|permit) X:X::X:X/M le <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_RIPD, rip_version_cmd_vtysh, 
       "version <1-2>", 
       "Set routing protocol version\n"
       "version\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_standard2_cmd_vtysh, 
       "ip extcommunity-list <1-99> (deny|permit)", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n")

DEFSH (VTYSH_OSPFD, no_ospf_cost_u32_cmd_vtysh, 
       "no ospf cost <1-65535>", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost")

DEFSH (VTYSH_RIPD, no_distribute_list_prefix_all_cmd_vtysh, 
       "no distribute-list prefix WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_ISISD, no_dynamic_hostname_cmd_vtysh, 
       "no hostname dynamic", 
       "Negate a command or set its defaults\n"
       "Dynamic hostname for IS-IS\n"
       "Dynamic hostname\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_ipv4_soft_cmd_vtysh, 
       "clear ip bgp view WORD * ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, ipv6_bgp_neighbor_received_routes_cmd_vtysh, 
       "show ipv6 bgp neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_ALL, no_access_list_any_cmd_vtysh, 
       "no access-list WORD (deny|permit) any", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n")

DEFSH (VTYSH_BGPD, no_set_community_delete_val_cmd_vtysh, 
       "no set comm-list (<1-99>|<100-500>|WORD) delete", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "set BGP community list (for deletion)\n"
       "Community-list number (standard)\n"
       "Communitly-list number (expanded)\n"
       "Community-list name\n"
       "Delete matching communities\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_vpnv4_soft_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " vpnv4 unicast soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, neighbor_remove_private_as_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "remove-private-AS", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Remove private AS number from outbound updates\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_mode_type_authlen_cmd_vtysh, 
       "no ip rip authentication mode (md5|text) auth-length (rfc|old-ripd)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication mode\n"
       "Keyed message digest\n"
       "Clear text authentication\n"
       "MD5 authentication data length\n"
       "RFC compatible\n"
       "Old ripd compatible\n")

DEFSH (VTYSH_RIPD, no_ip_rip_split_horizon_poisoned_reverse_cmd_vtysh, 
       "no ip rip split-horizon poisoned-reverse", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n"
       "With poisoned-reverse\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbor_prefix_counts_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X) prefix-counts", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display detailed prefix count information\n")

DEFSH (VTYSH_ZEBRA, ipv6_address_cmd_vtysh, 
       "ipv6 address X:X::X:X/M", 
       "Interface IPv6 config commands\n"
       "Set the IP address of an interface\n"
       "IPv6 address (e.g. 3ffe:506::1/48)\n")

DEFSH (VTYSH_BGPD, neighbor_nexthop_local_unchanged_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "nexthop-local unchanged", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure treatment of outgoing link-local nexthop attribute\n"
       "Leave link-local nexthop unchanged for this peer\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_expanded_cmd_vtysh, 
       "no ip community-list <100-500> (deny|permit) .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (expanded)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|)", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_standard_cmd_vtysh, 
       "no ip extcommunity-list <1-99> (deny|permit) .AA:NN", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Extended community attribute in 'rt aa:nn_or_IPaddr:nn' OR 'soo aa:nn_or_IPaddr:nn' format\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_type_cmd_vtysh, 
       "show ipv6 ospf6 route (intra-area|inter-area|external-1|external-2)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Dispaly Intra-Area routes\n"
       "Dispaly Inter-Area routes\n"
       "Dispaly Type-1 External routes\n"
       "Dispaly Type-2 External routes\n"
       )

DEFSH (VTYSH_OSPFD, ospf_passive_interface_cmd_vtysh, 
       "passive-interface IFNAME", 
       "Suppress routing updates on an interface\n"
       "Interface's name\n")

DEFSH (VTYSH_ISISD, spf_interval_l2_cmd_vtysh, 
       "spf-interval level-2 <1-120>", 
       "Minimum interval between SPF calculations\n"
       "Set interval for level 2 only\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_seq_le_ge_cmd_vtysh, 
       "ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M le <0-32> ge <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_compare_router_id_cmd_vtysh, 
       "bgp bestpath compare-routerid", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "Compare router-id for identical EBGP paths\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD|VTYSH_RIPNGD, match_metric_cmd_vtysh, 
       "match metric <0-4294967295>", 
       "Match values from routing table\n"
       "Match metric of route\n"
       "Metric value\n")

DEFSH (VTYSH_BGPD, show_bgp_statistics_view_vpnv4_cmd_vtysh, 
       "show bgp view WORD (ipv4) (vpnv4) statistics", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "Address family\n"
       "Address Family modifier\n"
       "BGP RIB advertisement statistics\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_args_md5_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) (message-digest|null) "
       "(message-digest-key|) <1-255> md5 KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n" "Use null authentication\n" "Use message-digest authentication\n"
       "Message digest authentication password (key)\n" "dummy string \n" "Key ID\n" "Use MD5 algorithm\n" "The OSPF password (key)")

DEFSH (VTYSH_ISISD, no_spf_interval_l2_cmd_vtysh, 
       "no spf-interval level-2", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n"
       "Set interval for level 2 only\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ALL, access_list_extended_mask_any_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D any", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "Any destination host\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_in_prefix_filter_cmd_vtysh, 
       "clear bgp * in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_ge_cmd_vtysh, 
       "no ip prefix-list WORD (deny|permit) A.B.C.D/M ge <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_ZEBRA, show_ipv6_route_addr_cmd_vtysh, 
       "show ipv6 route X:X::X:X", 
       "Show running system information\n"
       "IP information\n"
       "IPv6 routing table\n"
       "IPv6 Address\n")

DEFSH (VTYSH_ISISD, no_router_isis_cmd_vtysh, 
       "no router isis WORD", 
       "no\n" "Enable a routing process\n" "ISO IS-IS\n" "ISO Routing area tag")

DEFSH (VTYSH_BGPD, ipv6_mbgp_neighbor_advertised_route_cmd_vtysh, 
       "show ipv6 mbgp neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community_all_cmd_vtysh, 
       "show bgp ipv6 community", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_soft_in_cmd_vtysh, 
       "clear ip bgp view WORD * soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_backdoor_cmd_vtysh, 
       "network A.B.C.D mask A.B.C.D backdoor", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_med3_cmd_vtysh, 
       "no bgp bestpath med missing-as-worst confed", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Treat missing MED as the least preferred one\n"
       "Compare MED among confederation paths\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_description_cmd_vtysh, 
       "no ipv6 prefix-list WORD description", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_out_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_route_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged8_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop as-path med", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "As-path attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD, no_set_origin_cmd_vtysh, 
       "no set origin", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP origin code\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_name_expanded_all_cmd_vtysh, 
       "no ip extcommunity-list expanded WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify expanded extcommunity-list\n"
       "Extended Community list name\n")

DEFSH (VTYSH_ZEBRA, ip_route_distance_cmd_vtysh, 
       "ip route A.B.C.D/M (A.B.C.D|INTERFACE|null0) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n"
       "Distance value for this route\n")

DEFSH (VTYSH_RIPNGD, no_ripng_offset_list_ifname_cmd_vtysh, 
       "no offset-list WORD (in|out) <0-16> IFNAME", 
       "Negate a command or set its defaults\n"
       "Modify RIPng metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n"
       "Interface to match\n")

DEFSH (VTYSH_BGPD, show_bgp_summary_cmd_vtysh, 
       "show bgp summary", 
       "Show running system information\n"
       "BGP information\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community_list_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community-list (<1-500>|WORD)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n")

DEFSH (VTYSH_BGPD, no_neighbor_description_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "description .LINE", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Neighbor specific description\n"
       "Up to 80 characters describing this neighbor\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_ipv6_summary_cmd_vtysh, 
       "show bgp view WORD ipv6 summary", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_hellointerval_cmd_vtysh, 
       "ipv6 ospf6 hello-interval <1-65535>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interval time of Hello packets\n"
       "<1-65535> Seconds\n"
       )

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, match_ip_address_cmd_vtysh, 
       "match ip address (<1-199>|<1300-2699>|WORD)", 
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_summary_as_set_cmd_vtysh, 
       "no aggregate-address A.B.C.D/M summary-only as-set", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_cost_u32_inet4_cmd_vtysh, 
       "no ip ospf cost <1-65535> A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost\n"
       "Address of interface")

DEFSH (VTYSH_ZEBRA, no_ip_route_mask_flags2_cmd_vtysh, 
       "no ip route A.B.C.D A.B.C.D (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_RIPD, no_rip_network_cmd_vtysh, 
       "no network (A.B.C.D/M|WORD)", 
       "Negate a command or set its defaults\n"
       "Enable routing on an IP network\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Interface name\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_prefix_cmd_vtysh, 
       "show ipv6 prefix-list WORD X:X::X:X/M", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_BGPD, set_aggregator_as_cmd_vtysh, 
       "set aggregator as " "<1-4294967295>" " A.B.C.D", 
       "Set values in destination routing protocol\n"
       "BGP aggregator attribute\n"
       "AS number of aggregator\n"
       "AS number\n"
       "IP address of aggregator\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_prefix_list_cmd_vtysh, 
       "show ipv6 bgp prefix-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the prefix-list\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_ge_cmd_vtysh, 
       "ipv6 prefix-list WORD (deny|permit) X:X::X:X/M ge <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_ZEBRA, ip_route_mask_flags_distance_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE) (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Distance value for this route\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_ISISD, lsp_gen_interval_l1_cmd_vtysh, 
       "lsp-gen-interval level-1 <1-120>", 
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 1 only\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_inter_intra_external_cmd_vtysh, 
       "distance ospf inter-area <1-255> intra-area <1-255> external <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "External routes\n"
       "Distance for external routes\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_soft_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_key_chain2_cmd_vtysh, 
       "no ip rip authentication key-chain LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication key-chain\n"
       "name of key-chain\n")

DEFSH (VTYSH_OSPFD, no_ospf_max_metric_router_lsa_shutdown_cmd_vtysh, 
       "no max-metric router-lsa on-shutdown", 
       "Negate a command or set its defaults\n"
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Advertise stub-router prior to full shutdown of OSPF\n")

DEFSH (VTYSH_OSPFD, no_ospf_authentication_key_cmd_vtysh, 
       "no ospf authentication-key", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Authentication password (key)\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_mask_flags_distance2_cmd_vtysh, 
       "no ip route A.B.C.D A.B.C.D (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_prefix_cmd_vtysh, 
       "show bgp ipv6 X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_RIPNGD, no_ripng_redistribute_type_metric_cmd_vtysh, 
       "no redistribute (kernel|connected|static|ospf6|bgp) metric <0-16>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n")

DEFSH (VTYSH_OSPFD, ospf_area_range_not_advertise_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M not-advertise", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "DoNotAdvertise this range\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged7_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop med as-path", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "Med attribute\n"
       "As-path attribute\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged3_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop (as-path|med)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "As-path attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_med2_cmd_vtysh, 
       "bgp bestpath med confed missing-as-worst", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Compare MED among confederation paths\n"
       "Treat missing MED as the least preferred one\n")

DEFSH (VTYSH_ALL, no_access_list_standard_nomask_cmd_vtysh, 
       "no access-list (<1-99>|<1300-1999>) (deny|permit) A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Address to match\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community4_exact_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_ISISD, no_ip_router_isis_cmd_vtysh, 
       "no ip router isis WORD", 
       "Negate a command or set its defaults\n"
       "Interface Internet Protocol config commands\n"
       "IP router interface commands\n"
       "IS-IS Routing for IP\n"
       "Routing process tag\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_metric_type_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
       " metric <0-16777214> metric-type (1|2)", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_BGPD, no_bgp_enforce_first_as_cmd_vtysh, 
       "no bgp enforce-first-as", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Enforce the first AS for EBGP routes\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_ALL, access_list_standard_host_cmd_vtysh, 
       "access-list (<1-99>|<1300-1999>) (deny|permit) host A.B.C.D", 
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "A single host address\n"
       "Address to match\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_mask_summary_only_cmd_vtysh, 
       "no aggregate-address A.B.C.D A.B.C.D summary-only", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_detail_cmd_vtysh, 
       "show ip prefix-list detail", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Detail of prefix lists\n")

DEFSH (VTYSH_RIPNGD, debug_ripng_packet_detail_cmd_vtysh, 
       "debug ripng packet (recv|send) detail", 
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n"
       "Debug option set detaied information\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_priority_cmd_vtysh, 
       "no ip ospf priority", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Router priority\n")

DEFSH (VTYSH_BGPD, no_neighbor_unsuppress_map_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "unsuppress-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Route-map to selectively unsuppress suppressed routes\n"
       "Name of route map\n")

DEFSH (VTYSH_BGPD, undebug_bgp_as4_segment_cmd_vtysh, 
       "undebug bgp as4 segment", 
       "Disable debugging functions (see also 'debug')\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 aspath segment handling\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_spf_process_cmd_vtysh, 
       "debug ospf6 spf process", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug SPF Calculation\n"
       "Debug Detailed SPF Process\n"
      )

DEFSH (VTYSH_BGPD, old_no_ipv6_aggregate_address_cmd_vtysh, 
       "no ipv6 bgp aggregate-address X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "BGP information\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_seq_le_cmd_vtysh, 
       "no ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M le <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_received_routes_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_BGPD, no_neighbor_route_map_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "route-map WORD (in|out|import|export)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Apply route map to neighbor\n"
       "Name of route map\n"
       "Apply map to incoming routes\n"
       "Apply map to outbound routes\n"
       "Apply map to routes going into a Route-Server client's table\n"
       "Apply map to routes coming from a Route-Server client")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_advertise_prefix_list_cmd_vtysh, 
       "ipv6 ospf6 advertise prefix-list WORD", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Advertising options\n"
       "Filter prefix using prefix-list\n"
       "Prefix list name\n"
       )

DEFSH (VTYSH_ISISD, isis_hello_cmd_vtysh, 
       "isis hello padding", 
       "IS-IS commands\n"
       "Add padding to IS-IS hello packets\n"
       "Pad hello packets\n"
       "<cr>\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp peer-group WORD in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_zebra_cmd_vtysh, 
       "no debug bgp zebra", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP Zebra messages\n")

DEFSH (VTYSH_BGPD, show_bgp_prefix_longer_cmd_vtysh, 
       "show bgp X:X::X:X/M longer-prefixes", 
       "Show running system information\n"
       "BGP information\n"
       "IPv6 prefix <network>/<length>\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_ISISD, area_passwd_snpauth_cmd_vtysh, 
       "area-password WORD authenticate snp (send-only|validate)", 
       "Configure the authentication password for an area\n"
       "Area password\n"
       "Authentication\n"
       "SNP PDUs\n"
       "Send but do not check PDUs on receiving\n"
       "Send and check PDUs on receiving\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_soft_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbors_peer_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_OSPFD, ospf_abr_type_cmd_vtysh, 
       "ospf abr-type (cisco|ibm|shortcut|standard)", 
       "OSPF specific commands\n"
       "Set OSPF ABR type\n"
       "Alternative ABR,  cisco implementation\n"
       "Alternative ABR,  IBM implementation\n"
       "Shortcut ABR\n"
       "Standard behavior (RFC2328)\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_natural_backdoor_ttl_cmd_vtysh, 
       "network A.B.C.D backdoor pathlimit (1-255>", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_soft_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) soft", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n")

DEFSH (VTYSH_RIPD, rip_redistribute_type_metric_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|ospf|isis|bgp)" " metric <0-16>", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n")

DEFSH (VTYSH_ZEBRA, no_shutdown_if_cmd_vtysh, 
       "no shutdown", 
       "Negate a command or set its defaults\n"
       "Shutdown the selected interface\n")

DEFSH (VTYSH_ISISD, no_isis_priority_cmd_vtysh, 
       "no isis priority", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n")

DEFSH (VTYSH_BGPD, set_originator_id_cmd_vtysh, 
       "set originator-id A.B.C.D", 
       "Set values in destination routing protocol\n"
       "BGP originator ID attribute\n"
       "IP address of originator\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_description_cmd_vtysh, 
       "ipv6 prefix-list WORD description .LINE", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n"
       "Up to 80 characters describing this prefix-list\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_metric_type_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
         " metric <0-16777214> metric-type (1|2) route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_adv_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database adv-router A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, no_match_peer_val_cmd_vtysh, 
       "no match peer (A.B.C.D|X:X::X:X)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match peer address\n"
       "IPv6 address of peer\n"
       "IP address of peer\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_filter_cmd_vtysh, 
       "no debug bgp filters", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP filters\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_neighbor_routes_cmd_vtysh, 
       "show ip bgp vpnv4 all neighbors A.B.C.D routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_flooding_cmd_vtysh, 
       "no debug ospf6 flooding", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 flooding function\n"
      )

DEFSH (VTYSH_OSPFD, no_ospf_passive_interface_default_cmd_vtysh, 
       "no passive-interface default", 
       "Negate a command or set its defaults\n"
       "Allow routing updates on an interface\n"
       "Allow routing updates on interfaces by default\n")

DEFSH (VTYSH_ISISD, no_debug_isis_lupd_cmd_vtysh, 
       "no debug isis local-updates", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS local update packets\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_nsm_cmd_vtysh, 
       "no debug ospf nsm", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Neighbor State Machine")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_type_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)" " metric-type (1|2)", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_continue_index_cmd_vtysh, 
      "continue <1-65536>", 
      "Exit policy on matches\n"
      "Goto Clause number\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_forwarding_cmd_vtysh, 
       "no ipv6 forwarding", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Turn off IPv6 forwarding")

DEFSH (VTYSH_RIPD, no_debug_rip_packet_cmd_vtysh, 
       "no debug rip packet", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP packet\n")

DEFSH (VTYSH_BGPD, neighbor_password_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "password LINE", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Set a password\n"
       "The password\n")

DEFSH (VTYSH_ISISD, isis_circuit_type_cmd_vtysh, 
       "isis circuit-type (level-1|level-1-2|level-2-only)", 
       "IS-IS commands\n"
       "Configure circuit type for interface\n"
       "Level-1 only adjacencies are formed\n"
       "Level-1-2 adjacencies are formed\n"
       "Level-2 only adjacencies are formed\n")

DEFSH (VTYSH_OSPFD, ospf_neighbor_poll_interval_cmd_vtysh, 
       "neighbor A.B.C.D poll-interval <1-65535>", 
       "Specify neighbor router\n"
       "Neighbor IP address\n"
       "Dead Neighbor Polling interval\n"
       "Seconds\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_le_ge_cmd_vtysh, 
       "ip prefix-list WORD (deny|permit) A.B.C.D/M le <0-32> ge <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_ISISD, debug_isis_csum_cmd_vtysh, 
       "debug isis checksum-errors", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS LSP checksum errors\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_cmd_vtysh, 
       "ip prefix-list WORD (deny|permit) (A.B.C.D/M|any)", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Any prefix match. Same as \"0.0.0.0/0 le 32\"\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_info_cmd_vtysh, 
       "show ip bgp community-info", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "List all bgp community information\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged9_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med next-hop as-path", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "Nexthop attribute\n"
       "As-path attribute\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_packet_direct_cmd_vtysh, 
       "debug zebra packet (recv|send)", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n")

DEFSH (VTYSH_ALL, no_access_list_remark_cmd_vtysh, 
       "no access-list (<1-99>|<100-199>|<1300-1999>|<2000-2699>|WORD) remark", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP extended access list\n"
       "IP standard access list (expanded range)\n"
       "IP extended access list (expanded range)\n"
       "IP zebra access-list\n"
       "Access list entry comment\n")

DEFSH (VTYSH_BGPD, no_set_ecommunity_soo_val_cmd_vtysh, 
       "no set extcommunity soo .ASN:nn_or_IP-address:nn", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Site-of-Origin extended community\n"
       "VPN extended community\n")

DEFSH (VTYSH_BGPD, no_set_ipv6_nexthop_global_cmd_vtysh, 
       "no set ipv6 next-hop global", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 global address\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, no_bgp_always_compare_med_cmd_vtysh, 
       "no bgp always-compare-med", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Allow comparing MED from different neighbors\n")

DEFSH (VTYSH_ZEBRA, show_ipv6_route_prefix_cmd_vtysh, 
       "show ipv6 route X:X::X:X/M", 
       "Show running system information\n"
       "IP information\n"
       "IPv6 routing table\n"
       "IPv6 prefix\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_neighbors_peer_cmd_vtysh, 
       "show ip bgp view WORD neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_soft_out_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_cost_u32_cmd_vtysh, 
       "no ip ospf cost <1-65535>", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp external in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_vpnv4_soft_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D vpnv4 unicast soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_soft_in_cmd_vtysh, 
       "clear bgp ipv6 * soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_brouter_router_cmd_vtysh, 
       "no debug ospf6 border-routers router-id", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
       "Debug specific border router\n"
      )

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, no_set_tag_val_cmd_vtysh, 
       "no set tag <0-65535>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Tag value for routing protocol\n"
       "Tag value\n")

DEFSH (VTYSH_OSPF6D, ospf6_redistribute_routemap_cmd_vtysh, 
       "redistribute (static|kernel|connected|ripng|bgp) route-map WORD", 
       "Redistribute\n"
       "Static routes\n"
       "Kernel route\n"
       "Connected route\n"
       "RIPng route\n"
       "BGP route\n"
       "Route map reference\n"
       "Route map name\n"
      )

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_rmap_description_cmd_vtysh, 
       "no description", 
       "Negate a command or set its defaults\n"
       "Route-map comment\n")

DEFSH (VTYSH_BGPD, set_ecommunity_rt_cmd_vtysh, 
       "set extcommunity rt .ASN:nn_or_IP-address:nn", 
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Route Target extended community\n"
       "VPN extended community\n")

DEFSH (VTYSH_BGPD, neighbor_local_as_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "local-as " "<1-4294967295>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a local-as number\n"
       "AS number used as local AS\n")

DEFSH (VTYSH_ALL, show_ipv6_access_list_name_cmd_vtysh, 
       "show ipv6 access-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "List IPv6 access lists\n"
       "IPv6 zebra access-list\n")

DEFSH (VTYSH_ALL, no_access_list_standard_host_cmd_vtysh, 
       "no access-list (<1-99>|<1300-1999>) (deny|permit) host A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "A single host address\n"
       "Address to match\n")

DEFSH (VTYSH_OSPF6D, no_area_import_list_cmd_vtysh, 
       "no area A.B.C.D import-list NAME", 
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Unset the filter for networks announced to other areas\n"
       "NAme of the access-list\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_cidr_only_cmd_vtysh, 
       "show ip bgp cidr-only", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display only routes with non-natural netmasks\n")

DEFSH (VTYSH_RIPNGD, ripng_timers_cmd_vtysh, 
       "timers basic <0-65535> <0-65535> <0-65535>", 
       "RIPng timers setup\n"
       "Basic timer\n"
       "Routing table update timer value in second. Default is 30.\n"
       "Routing information timeout timer. Default is 180.\n"
       "Garbage collection timer. Default is 120.\n")

DEFSH (VTYSH_RIPNGD, ripng_offset_list_cmd_vtysh, 
       "offset-list WORD (in|out) <0-16>", 
       "Modify RIPng metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_expanded_cmd_vtysh, 
       "ip extcommunity-list <100-500> (deny|permit) .LINE", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (expanded)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_ALL, exec_timeout_sec_cmd_vtysh, 
       "exec-timeout <0-35791> <0-2147483>", 
       "Set the EXEC timeout\n"
       "Timeout in minutes\n"
       "Timeout in seconds\n")

DEFSH (VTYSH_RIPD, no_router_rip_cmd_vtysh, 
       "no router rip", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n"
       "Routing Information Protocol (RIP)\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_le_cmd_vtysh, 
       "no ip prefix-list WORD (deny|permit) A.B.C.D/M le <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_ALL, vty_access_class_cmd_vtysh, 
       "access-class WORD", 
       "Filter connections based on an IP access list\n"
       "IP access list\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_route_cmd_vtysh, 
       "show ip bgp A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_seq_ge_le_cmd_vtysh, 
       "ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M ge <0-32> le <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_neighbor_advertised_routes_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn neighbors A.B.C.D advertised-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_OSPFD, no_ospf_opaque_capable_cmd_vtysh, 
       "no ospf opaque-lsa", 
       "Negate a command or set its defaults\n"
       "OSPF specific commands\n"
       "Disable the Opaque-LSA capability (rfc2370)\n")

DEFSH (VTYSH_OSPFD, ospf_priority_cmd_vtysh, 
       "ospf priority <0-255>", 
       "OSPF interface commands\n"
       "Router priority\n"
       "Priority\n")

DEFSH (VTYSH_OSPFD, ospf_hello_interval_cmd_vtysh, 
       "ospf hello-interval <1-65535>", 
       "OSPF interface commands\n"
       "Time between HELLO packets\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_cmd_vtysh, 
       "no network A.B.C.D mask A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_summary_only_cmd_vtysh, 
       "no aggregate-address A.B.C.D/M summary-only", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_prefix_first_match_cmd_vtysh, 
       "show ipv6 prefix-list WORD X:X::X:X/M first-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "First matched prefix\n")

DEFSH (VTYSH_BGPD, neighbor_capability_orf_prefix_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "capability orf prefix-list (both|send|receive)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Advertise capability to the peer\n"
       "Advertise ORF capability to the peer\n"
       "Advertise prefixlist ORF capability to this neighbor\n"
       "Capability to SEND and RECEIVE the ORF to/from this neighbor\n"
       "Capability to RECEIVE the ORF from this neighbor\n"
       "Capability to SEND the ORF to this neighbor\n")

DEFSH (VTYSH_RIPD, no_ip_rip_split_horizon_cmd_vtysh, 
       "no ip rip split-horizon", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n")

DEFSH (VTYSH_RIPD, rip_redistribute_type_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|ospf|isis|bgp)", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_soft_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_advertised_route_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_soft_out_cmd_vtysh, 
       "clear ip bgp view WORD * soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, no_neighbor_set_peer_group_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "peer-group WORD", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Member of the peer-group\n"
       "peer-group name\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_route_map_cmd_vtysh, 
       "network A.B.C.D mask A.B.C.D route-map WORD", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_OSPFD, ospf_timers_throttle_spf_cmd_vtysh, 
       "timers throttle spf <0-600000> <0-600000> <0-600000>", 
       "Adjust routing timers\n"
       "Throttling adaptive timer\n"
       "OSPF SPF timers\n"
       "Delay (msec) from first change received till SPF calculation\n"
       "Initial hold time (msec) between consecutive SPF calculations\n"
       "Maximum hold time (msec)\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_ripng_split_horizon_cmd_vtysh, 
       "no ipv6 ripng split-horizon", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n")

DEFSH (VTYSH_BGPD, aggregate_address_as_set_cmd_vtysh, 
       "aggregate-address A.B.C.D/M as-set", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_BGPD, no_neighbor_peer_group_remote_as_cmd_vtysh, 
       "no neighbor WORD remote-as " "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor tag\n"
       "Specify a BGP neighbor\n"
       "AS number\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_seq_ge_cmd_vtysh, 
       "no ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M ge <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_RIPD, rip_network_cmd_vtysh, 
       "network (A.B.C.D/M|WORD)", 
       "Enable routing on an IP network\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Interface name\n")

DEFSH (VTYSH_ZEBRA, ip_route_mask_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE|null0)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_regexp_cmd_vtysh, 
       "show bgp ipv6 regexp .LINE", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_RIPD, debug_rip_zebra_cmd_vtysh, 
       "debug rip zebra", 
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP and ZEBRA communication\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD|VTYSH_RIPNGD, no_match_metric_val_cmd_vtysh, 
       "no match metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match metric of route\n"
       "Metric value\n")

DEFSH (VTYSH_BGPD, no_match_ipv6_address_cmd_vtysh, 
       "no match ipv6 address WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IPv6 information\n"
       "Match IPv6 address of route\n"
       "IPv6 access-list name\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_cmd_vtysh, 
       "show ip ospf neighbor", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n")

DEFSH (VTYSH_ALL, no_exec_timeout_cmd_vtysh, 
       "no exec-timeout", 
       "Negate a command or set its defaults\n"
       "Set the EXEC timeout\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community_list_cmd_vtysh, 
       "show ipv6 mbgp community-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the community-list\n"
       "community-list name\n")

DEFSH (VTYSH_RIPNGD, no_ripng_default_metric_val_cmd_vtysh, 
       "no default-metric <1-16>", 
       "Negate a command or set its defaults\n"
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_RIPNGD, no_debug_ripng_zebra_cmd_vtysh, 
       "no debug ripng zebra", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng and zebra communication\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_any_cmd_vtysh, 
       "no ipv6 access-list WORD (deny|permit) any", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any prefixi to match\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_ge_cmd_vtysh, 
       "no ipv6 prefix-list WORD (deny|permit) X:X::X:X/M ge <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_ZEBRA, no_ip_forwarding_cmd_vtysh, 
       "no ip forwarding", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Turn off IP forwarding")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged4_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med (as-path|next-hop)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "As-path attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_BGPD, no_set_aspath_prepend_val_cmd_vtysh, 
       "no set as-path prepend ." "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Transform BGP AS_PATH attribute\n"
       "Prepend to the as-path\n"
       "AS number\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_id_adv_router_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" ") A.B.C.D adv-router A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "Link State ID (as an IP address)\n"
       "Advertising Router link states\n"
       "Advertising Router (as an IP address)\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_le_cmd_vtysh, 
       "ip prefix-list WORD (deny|permit) A.B.C.D/M le <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_OSPFD|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_ZEBRA, match_interface_cmd_vtysh, 
       "match interface WORD", 
       "Match values from routing table\n"
       "match first hop interface of route\n"
       "Interface name\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_authentication_cmd_vtysh, 
       "no ip ospf authentication", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n")

DEFSH (VTYSH_OSPFD, ospf_cost_u32_inet4_cmd_vtysh, 
       "ospf cost <1-65535> A.B.C.D", 
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, no_bgp_default_local_preference_val_cmd_vtysh, 
       "no bgp default local-preference <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Configure BGP defaults\n"
       "local preference (higher=more preferred)\n"
       "Configure default local preference value\n")

DEFSH (VTYSH_OSPFD, ospf_rfc1583_flag_cmd_vtysh, 
       "ospf rfc1583compatibility", 
       "OSPF specific commands\n"
       "Enable the RFC1583Compatibility flag\n")

DEFSH (VTYSH_ZEBRA, set_src_cmd_vtysh, 
       "set src A.B.C.D", 
       "Set values in destination routing protocol\n"
       "src address for route\n"
       "src address\n")

DEFSH (VTYSH_OSPF6D, area_filter_list_cmd_vtysh, 
       "area A.B.C.D filter-list prefix WORD (in|out)", 
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Filter networks between OSPFv6 areas\n"
       "Filter prefixes between OSPFv6 areas\n"
       "Name of an IPv6 prefix-list\n"
       "Filter networks sent to this area\n"
       "Filter networks sent from this area\n")

DEFSH (VTYSH_RIPD, ip_rip_send_version_1_cmd_vtysh, 
       "ip rip send version 1 2", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement transmission\n"
       "Version control\n"
       "RIP version 1\n"
       "RIP version 2\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community3_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_vpnv4_in_cmd_vtysh, 
       "clear ip bgp * vpnv4 unicast in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, bgp_network_import_check_cmd_vtysh, 
       "bgp network import-check", 
       "BGP specific commands\n"
       "BGP network command\n"
       "Check BGP network route exists in IGP\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_soft_out_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, ip_ospf_priority_cmd_vtysh, 
       "ip ospf priority <0-255>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Router priority\n"
       "Priority\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_param4_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535>", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_OSPFD, debug_ospf_lsa_cmd_vtysh, 
       "debug ospf lsa", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Link State Advertisement\n")

DEFSH (VTYSH_ALL, service_advanced_vty_cmd_vtysh, 
       "service advanced-vty", 
       "Set up miscellaneous service\n"
       "Enable advanced mode vty interface\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_prefix_longer_cmd_vtysh, 
       "show ip route A.B.C.D/M longer-prefixes", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Show route matching the specified Network/Mask pair only\n")

DEFSH (VTYSH_BGPD, bgp_always_compare_med_cmd_vtysh, 
       "bgp always-compare-med", 
       "BGP specific commands\n"
       "Allow comparing MED from different neighbors\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_rsclient_summary_cmd_vtysh, 
      "show ip bgp ipv4 (unicast|multicast) rsclient summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_cmd_vtysh, 
       "no isis csnp-interval", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n")

DEFSH (VTYSH_ALL, no_vty_access_class_cmd_vtysh, 
       "no access-class [WORD]", 
       "Negate a command or set its defaults\n"
       "Filter connections based on an IP access list\n"
       "IP access list\n")

DEFSH (VTYSH_OSPFD, ospf_distribute_list_out_cmd_vtysh, 
       "distribute-list WORD out " "(kernel|connected|static|rip|isis|bgp)", 
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter outgoing routing updates\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community3_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_RIPNGD, show_ipv6_ripng_status_cmd_vtysh, 
       "show ipv6 ripng status", 
       "Show running system information\n"
       "IPv6 information\n"
       "Show RIPng routes\n"
       "IPv6 routing protocol process parameters and statistics\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, match_ip_next_hop_cmd_vtysh, 
       "match ip next-hop (<1-199>|<1300-2699>|WORD)", 
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n")

DEFSH (VTYSH_OSPFD, ospf_refresh_timer_cmd_vtysh, 
       "refresh timer <10-1800>", 
       "Adjust refresh parameters\n"
       "Set refresh timer\n"
       "Timer value in seconds\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_prefix_cmd_vtysh, 
       "show ip prefix-list WORD A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, clear_ipv6_prefix_list_name_cmd_vtysh, 
       "clear ipv6 prefix-list WORD", 
       "Reset functions\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_holdtime_cmd_vtysh, 
       "ip irdp holdtime <0-9000>", 
       "IP information\n"
       "ICMP Router discovery on this interface\n"
       "Set holdtime value\n"
       "Holdtime value in seconds. Default is 1800 seconds\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_soft_cmd_vtysh, 
       "clear bgp * soft", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_mask_cmd_vtysh, 
       "no aggregate-address A.B.C.D A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n")

DEFSH (VTYSH_RIPD, key_string_cmd_vtysh, 
       "key-string LINE", 
       "Set key string\n"
       "The key\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_cost_inet4_cmd_vtysh, 
       "no ip ospf cost A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_route_cmd_vtysh, 
       "show ipv6 bgp X:X::X:X", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authtype_args_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) (message-digest|null)", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n" "Use null authentication\n" "Use message-digest authentication\n")

DEFSH (VTYSH_BGPD, show_bgp_community2_exact_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_address_prefix_list_cmd_vtysh, 
       "no match ip address prefix-list", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n"
       "Match entries of prefix-lists\n")

DEFSH (VTYSH_ALL, access_list_any_cmd_vtysh, 
       "access-list WORD (deny|permit) any", 
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n")

DEFSH (VTYSH_BGPD, match_pathlimit_as_cmd_vtysh, 
       "match pathlimit as <1-65535>", 
       "Match values from routing table\n"
       "BGP AS-Pathlimit attribute\n"
       "Match Pathlimit AS number\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_lsa_hex_cmd_vtysh, 
       "debug ospf6 lsa XXXX/0xXXXX", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug Link State Advertisements (LSAs)\n"
       "Specify LS type as Hexadecimal\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_graceful_restart_stalepath_time_cmd_vtysh, 
       "no bgp graceful-restart stalepath-time", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Graceful restart capability parameters\n"
       "Set the max time to hold onto restarting peer's stale paths\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_adv_router_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" ") adv-router A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "Advertising Router link states\n"
       "Advertising Router (as an IP address)\n")

DEFSH (VTYSH_OSPFD, ospf_cost_u32_cmd_vtysh, 
       "ospf cost <1-65535>", 
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost")

DEFSH (VTYSH_ZEBRA, ipv6_route_cmd_vtysh, 
       "ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE)", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_authentication_addr_cmd_vtysh, 
       "no ip ospf authentication A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n"
       "Address of interface")

DEFSH (VTYSH_ALL, no_access_list_extended_host_mask_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D A.B.C.D A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, clear_ipv6_prefix_list_name_prefix_cmd_vtysh, 
       "clear ipv6 prefix-list WORD X:X::X:X/M", 
       "Reset functions\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community4_exact_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_l2_cmd_vtysh, 
       "no lsp-gen-interval level-2", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 2 only\n")

DEFSH (VTYSH_ISISD, debug_isis_spfevents_cmd_vtysh, 
       "debug isis spf-events", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS Shortest Path First Events\n")

DEFSH (VTYSH_BGPD, no_set_aspath_prepend_cmd_vtysh, 
       "no set as-path prepend", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Transform BGP AS_PATH attribute\n"
       "Prepend to the as-path\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_summary_cmd_vtysh, 
       "show bgp ipv6 summary", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_name_standard2_cmd_vtysh, 
       "ip extcommunity-list standard WORD (deny|permit)", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify standard extcommunity-list\n"
       "Extended Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n")

DEFSH (VTYSH_ISISD, no_domain_passwd_cmd_vtysh, 
       "no domain-password WORD", 
       "Negate a command or set its defaults\n"
       "Set the authentication password for a routing domain\n")

DEFSH (VTYSH_RIPD, rip_route_cmd_vtysh, 
       "route A.B.C.D/M", 
       "RIP static route configuration\n"
       "IP prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, ip_community_list_standard_cmd_vtysh, 
       "ip community-list <1-99> (deny|permit) .AA:NN", 
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Community number in aa:nn format or internet|local-AS|no-advertise|no-export\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_soft_out_cmd_vtysh, 
       "clear bgp * soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_shortcut_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) shortcut (enable|disable)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Deconfigure the area's shortcutting mode\n"
       "Deconfigure enabled shortcutting through the area\n"
       "Deconfigure disabled shortcutting through the area\n")

DEFSH (VTYSH_RIPNGD, ripng_redistribute_ripng_cmd_vtysh, 
       "redistribute ripng", 
       "Redistribute information from another routing protocol\n"
       "RIPng route\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbor_received_prefix_filter_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_BGPD, no_neighbor_advertise_interval_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "advertisement-interval <0-600>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Minimum interval between sending BGP routing updates\n"
       "time in seconds\n")

DEFSH (VTYSH_BGPD, no_match_aspath_cmd_vtysh, 
       "no match as-path", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP AS path list\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_soft_in_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) soft in", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ZEBRA, ip_route_mask_flags2_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_OSPFD, ospf_max_metric_router_lsa_startup_cmd_vtysh, 
       "max-metric router-lsa on-startup <5-86400>", 
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Automatically advertise stub Router-LSA on startup of OSPF\n"
       "Time (seconds) to advertise self as stub-router\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_self_originated_cmd_vtysh, 
       "show ipv6 ospf6 database self-originated", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Self-originated LSAs\n"
      )

DEFSH (VTYSH_BGPD, show_ipv6_bgp_summary_cmd_vtysh, 
       "show ipv6 bgp summary", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_rmap_continue_cmd_vtysh, 
       "no continue", 
       "Negate a command or set its defaults\n"
       "Continue on a different entry within the route-map\n")

DEFSH (VTYSH_BGPD, bgp_damp_unset2_cmd_vtysh, 
       "no bgp dampening <1-45> <1-20000> <1-20000> <1-255>", 
       "Negate a command or set its defaults\n"
       "BGP Specific commands\n"
       "Enable route-flap dampening\n"
       "Half-life time for the penalty\n"
       "Value to start reusing a route\n"
       "Value to start suppressing a route\n"
       "Maximum duration to suppress a stable route\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_vpnv4_soft_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " vpnv4 unicast soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ISISD, show_isis_topology_cmd_vtysh, 
       "show isis topology", 
       "Show running system information\n"
       "IS-IS information\n"
       "IS-IS paths to Intermediate Systems\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_rsclient_prefix_cmd_vtysh, 
       "show ip bgp view WORD rsclient (A.B.C.D|X:X::X:X) A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_expanded_all_cmd_vtysh, 
       "no ip community-list <100-500>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (expanded)\n")

DEFSH (VTYSH_BGPD, no_neighbor_prefix_list_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "prefix-list WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Filter updates to/from this neighbor\n"
       "Name of a prefix list\n"
       "Filter incoming updates\n"
       "Filter outgoing updates\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_ifname_prefix_match_cmd_vtysh, 
       "show ipv6 ospf6 interface IFNAME prefix X:X::X:X/M (match|detail)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Interface name(e.g. ep0)\n"
       "Display connected prefixes to advertise\n"
       "Display the route\n"
       "Display the route matches the prefix\n"
       "Dispaly details of the prefixes\n"
       )

DEFSH (VTYSH_BGPD, show_bgp_view_prefix_cmd_vtysh, 
       "show bgp view WORD X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, bgp_network_route_map_cmd_vtysh, 
       "network A.B.C.D/M route-map WORD", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_backdoor_ttl_cmd_vtysh, 
       "network A.B.C.D mask A.B.C.D backdoor pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_damp_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) dampened-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the dampened routes received from neighbor\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_cmd_vtysh, 
       "show bgp view WORD ipv6", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_transmit_delay_addr_cmd_vtysh, 
       "no ip ospf transmit-delay A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Link state transmit delay\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged1_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged (as-path|next-hop|med)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_show_name_cmd_vtysh, 
       "show route-map [WORD]", 
       "Show running system information\n"
       "route-map information\n"
       "route-map name\n")

DEFSH (VTYSH_BGPD, bgp_network_cmd_vtysh, 
       "network A.B.C.D/M", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_in_prefix_filter_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig inbound update\n"
       "Push out the existing ORF prefix-list\n")

DEFSH (VTYSH_BGPD, no_bgp_network_ttl_cmd_vtysh, 
       "no network A.B.C.D/M pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_scan_cmd_vtysh, 
       "show ip bgp scan", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP scan status\n")

DEFSH (VTYSH_BGPD, no_match_ip_route_source_val_cmd_vtysh, 
       "no match ip route-source (<1-199>|<1300-2699>|WORD)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP standard access-list name\n")

DEFSH (VTYSH_RIPD, rip_offset_list_ifname_cmd_vtysh, 
       "offset-list WORD (in|out) <0-16> IFNAME", 
       "Modify RIP metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n"
       "Interface to match\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_message_cmd_vtysh, 
       "debug ospf6 message (unknown|hello|dbdesc|lsreq|lsupdate|lsack|all)", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 message\n"
       "Debug Unknown message\n"
       "Debug Hello message\n"
       "Debug Database Description message\n"
       "Debug Link State Request message\n"
       "Debug Link State Update message\n"
       "Debug Link State Acknowledgement message\n"
       "Debug All message\n"
       )

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_rmap_onmatch_next_cmd_vtysh, 
       "no on-match next", 
       "Negate a command or set its defaults\n"
       "Exit policy on matches\n"
       "Next clause\n")

DEFSH (VTYSH_ZEBRA, ip_route_flags_distance2_cmd_vtysh, 
       "ip route A.B.C.D/M (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_BGPD, bgp_default_local_preference_cmd_vtysh, 
       "bgp default local-preference <0-4294967295>", 
       "BGP specific commands\n"
       "Configure BGP defaults\n"
       "local preference (higher=more preferred)\n"
       "Configure default local preference value\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_natural_backdoor_cmd_vtysh, 
       "network A.B.C.D backdoor", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Specify a BGP backdoor route\n")

DEFSH (VTYSH_BGPD, show_bgp_community_all_cmd_vtysh, 
       "show bgp community", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_OSPFD, ip_ospf_dead_interval_minimal_cmd_vtysh, 
       "ip ospf dead-interval minimal hello-multiplier <1-10>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Minimal 1s dead-interval with fast sub-second hellos\n"
       "Hello multiplier factor\n"
       "Number of Hellos to send each second\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_id_router_cmd_vtysh, 
       "show ipv6 ospf6 database * A.B.C.D A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPFD, mpls_te_link_max_rsv_bw_cmd_vtysh, 
       "mpls-te link max-rsv-bw BANDWIDTH", 
       "MPLS-TE specific commands\n"
       "Configure MPLS-TE link parameters\n"
       "Maximum bandwidth that may be reserved\n"
       "Bytes/second (IEEE floating point format)\n")

DEFSH (VTYSH_ALL, vty_login_cmd_vtysh, 
       "login", 
       "Enable password checking\n")

DEFSH (VTYSH_BGPD, undebug_bgp_fsm_cmd_vtysh, 
       "undebug bgp fsm", 
       "Disable debugging functions (see also 'debug')\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "Finite State Machine\n")

DEFSH (VTYSH_BGPD, neighbor_prefix_list_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "prefix-list WORD (in|out)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Filter updates to/from this neighbor\n"
       "Name of a prefix list\n"
       "Filter incoming updates\n"
       "Filter outgoing updates\n")

DEFSH (VTYSH_ZEBRA, show_zebra_client_cmd_vtysh, 
       "show zebra client", 
       "Show running system information\n"
       "Zebra information"
       "Client information")

DEFSH (VTYSH_RIPD, rip_distance_source_access_list_cmd_vtysh, 
       "distance <1-255> A.B.C.D/M WORD", 
       "Administrative distance\n"
       "Distance value\n"
       "IP source prefix\n"
       "Access list name\n")

DEFSH (VTYSH_BGPD, no_match_peer_cmd_vtysh, 
       "no match peer", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match peer address\n")

DEFSH (VTYSH_RIPNGD, ripng_passive_interface_cmd_vtysh, 
       "passive-interface IFNAME", 
       "Suppress routing updates on an interface\n"
       "Interface name\n")

DEFSH (VTYSH_ISISD, spf_interval_l1_cmd_vtysh, 
       "spf-interval level-1 <1-120>", 
       "Minimum interval between SPF calculations\n"
       "Set interval for level 1 only\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_BGPD, dump_bgp_all_cmd_vtysh, 
       "dump bgp all PATH", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump all BGP packets\n"
       "Output filename\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_received_routes_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_ZEBRA, no_debug_zebra_packet_cmd_vtysh, 
       "no debug zebra packet", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n")

DEFSH (VTYSH_BGPD, no_bgp_network_route_map_cmd_vtysh, 
       "no network A.B.C.D/M route-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbor_received_routes_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_ISISD, no_isis_priority_l2_arg_cmd_vtysh, 
       "no isis priority <0-127> level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n"
       "Specify priority for level-2 routing\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_soft_out_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) soft out", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_ifmtu_cmd_vtysh, 
       "ipv6 ospf6 ifmtu <1-65535>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface MTU\n"
       "OSPFv3 Interface MTU\n"
       )

DEFSH (VTYSH_ZEBRA, ip_address_cmd_vtysh, 
       "ip address A.B.C.D/M", 
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP address (e.g. 10.0.0.1/8)\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_pref_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_OSPFD, ospf_area_range_advertise_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M advertise", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "OSPF area range for route advertise (default)\n"
       "Area range prefix\n"
       "Advertise this range (default)\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_mtu_ignore_addr_cmd_vtysh, 
       "no ip ospf mtu-ignore A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Disable mtu mismatch detection\n"
       "Address of interface")

DEFSH (VTYSH_ZEBRA, ipv6_route_ifname_cmd_vtysh, 
       "ipv6 route X:X::X:X/M X:X::X:X INTERFACE", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n")

DEFSH (VTYSH_BGPD, show_bgp_regexp_cmd_vtysh, 
       "show bgp regexp .LINE", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_BGPD, show_debugging_bgp_cmd_vtysh, 
       "show debugging bgp", 
       "Show running system information\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, capability_opaque_cmd_vtysh, 
       "capability opaque", 
       "Enable specific OSPF feature\n"
       "Opaque LSA\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_flap_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) flap-statistics", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display flap statistics of the routes learned from neighbor\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_transmit_delay_cmd_vtysh, 
       "no ip ospf transmit-delay", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Link state transmit delay\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_out_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, show_bgp_view_rsclient_cmd_vtysh, 
       "show bgp view WORD rsclient (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_type_cmd_vtysh, 
       "default-information originate metric-type (1|2)", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_name_seq_cmd_vtysh, 
       "show ipv6 prefix-list WORD seq <1-4294967295>", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_soft_cmd_vtysh, 
       "clear ip bgp A.B.C.D soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_route_cmd_vtysh, 
       "show bgp X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged3_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop (as-path|med)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "As-path attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_intra_external_cmd_vtysh, 
       "distance ospf intra-area <1-255> external <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n"
       "External routes\n"
       "Distance for external routes\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, ip_ospf_dead_interval_addr_cmd_vtysh, 
       "ip ospf dead-interval <1-65535> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Seconds\n"
       "Address of interface\n")

DEFSH (VTYSH_RIPNGD, no_ripng_redistribute_type_routemap_cmd_vtysh, 
       "no redistribute (kernel|connected|static|ospf6|bgp) route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_type_routemap_cmd_vtysh, 
       "default-information originate metric-type (1|2) route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ALL, no_access_list_extended_host_any_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D any", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "Any destination host\n")

DEFSH (VTYSH_BGPD, neighbor_allowas_in_arg_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "allowas-in <1-10>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Accept as-path with my AS present in it\n"
       "Number of occurances of AS number\n")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_arg_cmd_vtysh, 
       "no lsp-gen-interval <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_BGPD, neighbor_ebgp_multihop_ttl_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "ebgp-multihop <1-255>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Allow EBGP neighbors not on directly connected networks\n"
       "maximum hop count\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_keepalive_cmd_vtysh, 
       "no debug bgp keepalives", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP keepalives\n")

DEFSH (VTYSH_BGPD, show_bgp_community2_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_priority_addr_cmd_vtysh, 
       "no ip ospf priority A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Router priority\n"
       "Address of interface")

DEFSH (VTYSH_RIPD, no_rip_default_metric_cmd_vtysh, 
       "no default-metric", 
       "Negate a command or set its defaults\n"
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_ZEBRA, no_bandwidth_if_val_cmd_vtysh, 
       "no bandwidth <1-10000000>", 
       "Negate a command or set its defaults\n"
       "Set bandwidth informational parameter\n"
       "Bandwidth in kilobits\n")

DEFSH (VTYSH_RIPD, send_lifetime_infinite_month_day_cmd_vtysh, 
       "send-lifetime HH:MM:SS MONTH <1-31> <1993-2035> infinite", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Never expires")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_ifname_prefix_detail_cmd_vtysh, 
       "show ipv6 ospf6 interface IFNAME prefix (X:X::X:X|X:X::X:X/M|detail)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Interface name(e.g. ep0)\n"
       "Display connected prefixes to advertise\n"
       "Display the route bestmatches the address\n"
       "Display the route\n"
       "Dispaly details of the prefixes\n"
       )

DEFSH (VTYSH_BGPD, no_vpnv4_network_cmd_vtysh, 
       "no network A.B.C.D/M rd ASN:nn_or_IP-address:nn tag WORD", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify Route Distinguisher\n"
       "VPN Route Distinguisher\n"
       "BGP tag\n"
       "tag value\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_filter_list_cmd_vtysh, 
       "show ip bgp filter-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_route_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_RIPD, send_lifetime_duration_day_month_cmd_vtysh, 
       "send-lifetime HH:MM:SS <1-31> MONTH <1993-2035> duration <1-2147483646>", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Duration of the key\n"
       "Duration seconds\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_ism_cmd_vtysh, 
       "no debug ospf ism", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Interface State Machine")

DEFSH (VTYSH_BGPD, undebug_bgp_keepalive_cmd_vtysh, 
       "undebug bgp keepalives", 
       "Disable debugging functions (see also 'debug')\n"
       "BGP information\n"
       "BGP keepalives\n")

DEFSH (VTYSH_OSPFD, no_ospf_neighbor_poll_interval_cmd_vtysh, 
       "no neighbor A.B.C.D poll-interval <1-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor IP address\n"
       "Dead Neighbor Polling interval\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged5_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path next-hop med", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_RIPD, rip_default_information_originate_cmd_vtysh, 
       "default-information originate", 
       "Control distribution of default route\n"
       "Distribute a default route\n")

DEFSH (VTYSH_RIPD, ip_rip_authentication_mode_authlen_cmd_vtysh, 
       "ip rip authentication mode (md5|text) auth-length (rfc|old-ripd)", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication mode\n"
       "Keyed message digest\n"
       "Clear text authentication\n"
       "MD5 authentication data length\n"
       "RFC compatible\n"
       "Old ripd compatible\n")

DEFSH (VTYSH_ZEBRA, no_ip_irdp_cmd_vtysh, 
       "no ip irdp", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Disable ICMP Router discovery on this interface\n")

DEFSH (VTYSH_BGPD, debug_bgp_as4_cmd_vtysh, 
       "debug bgp as4", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP AS4 actions\n")

DEFSH (VTYSH_RIPD, rip_default_metric_cmd_vtysh, 
       "default-metric <1-16>", 
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, bgp_confederation_identifier_cmd_vtysh, 
       "bgp confederation identifier " "<1-4294967295>", 
       "BGP specific commands\n"
       "AS confederation parameters\n"
       "AS number\n"
       "Set routing domain confederation AS\n")

DEFSH (VTYSH_ALL, exec_timeout_min_cmd_vtysh, 
       "exec-timeout <0-35791>", 
       "Set timeout value\n"
       "Timeout value in minutes\n")

DEFSH (VTYSH_RIPD, no_key_string_cmd_vtysh, 
       "no key-string [LINE]", 
       "Negate a command or set its defaults\n"
       "Unset key string\n"
       "The key\n")

DEFSH (VTYSH_ISISD, no_is_type_cmd_vtysh, 
       "no is-type (level-1|level-1-2|level-2-only)", 
       "Negate a command or set its defaults\n"
       "IS Level for this routing process (OSI only)\n"
       "Act as a station router only\n"
       "Act as both a station router and an area router\n"
       "Act as an area router only\n")

DEFSH (VTYSH_BGPD, debug_bgp_update_direct_cmd_vtysh, 
       "debug bgp updates (in|out)", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP updates\n"
       "Inbound updates\n"
       "Outbound updates\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_filter_list_cmd_vtysh, 
       "show ip bgp flap-statistics filter-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_summary_name_cmd_vtysh, 
       "show ipv6 prefix-list summary WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Summary of prefix lists\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_maxadvertinterval_cmd_vtysh, 
       "ip irdp maxadvertinterval <4-1800>", 
       "IP information\n"
       "ICMP Router discovery on this interface\n"
       "Set maximum time between advertisement\n"
       "Maximum advertisement interval in seconds\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_natural_route_map_cmd_vtysh, 
       "network A.B.C.D route-map WORD", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_ZEBRA, show_interface_cmd_vtysh, 
       "show interface [IFNAME]", 
       "Show running system information\n"
       "Interface status and configuration\n"
       "Inteface name\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_cmd_vtysh, 
       "show bgp ipv6", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n")

DEFSH (VTYSH_BGPD, match_community_exact_cmd_vtysh, 
       "match community (<1-99>|<100-500>|WORD) exact-match", 
       "Match values from routing table\n"
       "Match BGP community list\n"
       "Community-list number (standard)\n"
       "Community-list number (expanded)\n"
       "Community-list name\n"
       "Do exact matching of communities\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_vpnv4_soft_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " vpnv4 unicast soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_description_cmd_vtysh, 
       "ip prefix-list WORD description .LINE", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n"
       "Up to 80 characters describing this prefix-list\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_ifname_pref_cmd_vtysh, 
       "ipv6 route X:X::X:X/M X:X::X:X INTERFACE <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv6_metric_cmd_vtysh, 
       "redistribute (connected|kernel|ospf6|ripng|static) metric <0-4294967295>", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_rsclient_summary_cmd_vtysh, 
       "show ip bgp rsclient summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_RIPD, rip_neighbor_cmd_vtysh, 
       "neighbor A.B.C.D", 
       "Specify a neighbor router\n"
       "Neighbor address\n")

DEFSH (VTYSH_RIPD, no_rip_default_metric_val_cmd_vtysh, 
       "no default-metric <1-16>", 
       "Negate a command or set its defaults\n"
       "Set a metric of redistribute routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_cmd_vtysh, 
       "show ipv6 bgp", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n")

DEFSH (VTYSH_RIPNGD, ripng_redistribute_type_cmd_vtysh, 
       "redistribute (kernel|connected|static|ospf6|bgp)", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_BGPD, neighbor_route_reflector_client_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "route-reflector-client", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure a neighbor as Route Reflector client\n")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_received_prefix_filter_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_OSPFD, ospf_area_default_cost_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) default-cost <0-16777215>", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Set the summary-default cost of a NSSA or stub area\n"
       "Stub's advertised default summary cost\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_soft_cmd_vtysh, 
       "clear ip bgp * soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_l1_cmd_vtysh, 
       "no isis hello-multiplier level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Specify hello multiplier for level-1 IIHs\n")

DEFSH (VTYSH_ALL, access_list_extended_host_mask_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D A.B.C.D A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_BGPD, bgp_network_mask_ttl_cmd_vtysh, 
       "network A.B.C.D mask A.B.C.D pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_standard_cmd_vtysh, 
       "ip extcommunity-list <1-99> (deny|permit) .AA:NN", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Extended community attribute in 'rt aa:nn_or_IPaddr:nn' OR 'soo aa:nn_or_IPaddr:nn' format\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_rsclient_cmd_vtysh, 
       "clear bgp * rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, no_bgp_config_type_cmd_vtysh, 
       "no bgp config-type", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Display configuration type\n")

DEFSH (VTYSH_BGPD, no_neighbor_filter_list_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "filter-list WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Establish BGP filters\n"
       "AS path access-list name\n"
       "Filter incoming routes\n"
       "Filter outgoing routes\n")

DEFSH (VTYSH_RIPD, ip_rip_receive_version_1_cmd_vtysh, 
       "ip rip receive version 1 2", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement reception\n"
       "Version control\n"
       "RIP version 1\n"
       "RIP version 2\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_nssa_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) nssa", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_events_cmd_vtysh, 
       "no debug bgp events", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP events\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_broadcast_cmd_vtysh, 
       "ip irdp broadcast", 
       "IP information\n"
       "ICMP Router discovery on this interface using broadcast\n")

DEFSH (VTYSH_BGPD, no_neighbor_port_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "port <0-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Neighbor's BGP port\n"
       "TCP port number\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community3_exact_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_RIPD, no_key_cmd_vtysh, 
       "no key <0-2147483647>", 
       "Negate a command or set its defaults\n"
       "Delete a key\n"
       "Key identifier number\n")

DEFSH (VTYSH_BGPD, ipv6_bgp_network_cmd_vtysh, 
       "network X:X::X:X/M", 
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_OSPFD, show_mpls_te_router_cmd_vtysh, 
       "show mpls-te router", 
       "Show running system information\n"
       "MPLS-TE information\n"
       "Router information\n")

DEFSH (VTYSH_BGPD, no_match_ip_route_source_prefix_list_cmd_vtysh, 
       "no match ip route-source prefix-list", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n"
       "Match entries of prefix-lists\n")

DEFSH (VTYSH_BGPD, ipv6_bgp_neighbor_routes_cmd_vtysh, 
       "show ipv6 bgp neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, ip_community_list_name_standard2_cmd_vtysh, 
       "ip community-list standard WORD (deny|permit)", 
       "IP information\n"
       "Add a community list entry\n"
       "Add a standard community-list entry\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_name_expanded_cmd_vtysh, 
       "ip extcommunity-list expanded WORD (deny|permit) .LINE", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify expanded extcommunity-list\n"
       "Extended Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_cmd_vtysh, 
       "clear bgp " "<1-4294967295>", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_neighbor_prefix_counts_cmd_vtysh, 
       "show ip bgp vpnv4 all neighbors (A.B.C.D|X:X::X:X) prefix-counts", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display detailed prefix count information\n")

DEFSH (VTYSH_OSPF6D, no_redistribute_ospf6_cmd_vtysh, 
       "no redistribute ospf6", 
       "Negate a command or set its defaults\n"
       "Redistribute control\n"
       "OSPF6 route\n")

DEFSH (VTYSH_BGPD, show_bgp_statistics_cmd_vtysh, 
       "show bgp (ipv4|ipv6) (unicast|multicast) statistics", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "BGP RIB advertisement statistics\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPF6D, no_area_export_list_cmd_vtysh, 
       "no area A.B.C.D export-list NAME", 
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Unset the filter for networks announced to other areas\n"
       "Name of the access-list\n")

DEFSH (VTYSH_OSPFD, no_ospf_hello_interval_cmd_vtysh, 
       "no ospf hello-interval", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Time between HELLO packets\n")

DEFSH (VTYSH_ISISD, isis_hello_multiplier_cmd_vtysh, 
       "isis hello-multiplier <3-1000>", 
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_filter_list_cmd_vtysh, 
       "show ipv6 mbgp filter-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_RIPD, debug_rip_packet_direct_cmd_vtysh, 
       "debug rip packet (recv|send)", 
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP packet\n"
       "RIP receive packet\n"
       "RIP send packet\n")

DEFSH (VTYSH_BGPD, no_set_ecommunity_rt_cmd_vtysh, 
       "no set extcommunity rt", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Route Target extended community\n")

DEFSH (VTYSH_ISISD, no_debug_isis_spftrigg_cmd_vtysh, 
       "no debug isis spf-triggers", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS SPF triggering events\n")

DEFSH (VTYSH_OSPFD, ospf_router_id_cmd_vtysh, 
       "ospf router-id A.B.C.D", 
       "OSPF specific commands\n"
       "router-id for the OSPF process\n"
       "OSPF router-id in IP address format\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community_list_cmd_vtysh, 
       "show bgp ipv6 community-list (<1-500>|WORD)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_cmd_vtysh, 
       "clear bgp ipv6 *", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_in_prefix_filter_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig inbound update\n"
       "Push out the existing ORF prefix-list\n")

DEFSH (VTYSH_OSPFD, ospf_area_authentication_message_digest_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) authentication message-digest", 
       "OSPF area parameters\n"
       "Enable authentication\n"
       "Use message-digest authentication\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_name_expanded_all_cmd_vtysh, 
       "no ip community-list expanded WORD", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Add an expanded community-list entry\n"
       "Community list name\n")

DEFSH (VTYSH_OSPFD|VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_ZEBRA, no_match_interface_cmd_vtysh, 
       "no match interface", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match first hop interface of route\n")

DEFSH (VTYSH_ZEBRA, bandwidth_if_cmd_vtysh, 
       "bandwidth <1-10000000>", 
       "Set bandwidth informational parameter\n"
       "Bandwidth in kilobits\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_l2_cmd_vtysh, 
       "no isis csnp-interval level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "Specify interval for level-2 CSNPs\n")

DEFSH (VTYSH_OSPF6D, show_version_ospf6_cmd_vtysh, 
       "show version ospf6", 
       "Show running system information\n"
       "Displays ospf6d version\n"
      )

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_flap_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) flap-statistics", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display flap statistics of the routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, no_neighbor_route_server_client_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "route-server-client", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure a neighbor as Route Server client\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_packet_detail_cmd_vtysh, 
       "debug zebra packet (recv|send) detail", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n"
       "Debug option set detaied information\n")

DEFSH (VTYSH_RIPNGD, no_ripng_redistribute_ripng_cmd_vtysh, 
       "no redistribute ripng", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "RIPng route\n")

DEFSH (VTYSH_BGPD, no_bgp_confederation_identifier_cmd_vtysh, 
       "no bgp confederation identifier", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "AS confederation parameters\n"
       "AS number\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_inter_external_intra_cmd_vtysh, 
       "distance ospf inter-area <1-255> external <1-255> intra-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "External routes\n"
       "Distance for external routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n")

DEFSH (VTYSH_BGPD, no_set_ecommunity_soo_cmd_vtysh, 
       "no set extcommunity soo", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Site-of-Origin extended community\n")

DEFSH (VTYSH_ZEBRA, no_ip_address_label_cmd_vtysh, 
       "no ip address A.B.C.D/M label LINE", 
       "Negate a command or set its defaults\n"
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP address (e.g. 10.0.0.1/8)\n"
       "Label of this address\n"
       "Label\n")

DEFSH (VTYSH_OSPFD, ospf_max_metric_router_lsa_admin_cmd_vtysh, 
       "max-metric router-lsa administrative", 
       "OSPF maximum / infinite-distance metric\n"
       "Advertise own Router-LSA with infinite distance (stub router)\n"
       "Administratively applied,  for an indefinite period\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbors_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_OSPFD|VTYSH_OSPF6D, ospf6_routemap_no_set_metric_type_cmd_vtysh, 
       "no set metric-type (type-1|type-2)", 
       "Negate a command or set its defaults\n"
       "Set value\n"
       "Type of metric\n"
       "OSPF6 external type 1 metric\n"
       "OSPF6 external type 2 metric\n")

DEFSH (VTYSH_OSPFD, no_ospf_transmit_delay_cmd_vtysh, 
       "no ospf transmit-delay", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Link state transmit delay\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_regexp_cmd_vtysh, 
       "show ipv6 bgp regexp .LINE", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_out_cmd_vtysh, 
       "clear ip bgp * out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_filter_list_cmd_vtysh, 
       "show ipv6 bgp filter-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_ZEBRA, ipv6_route_ifname_flags_pref_cmd_vtysh, 
       "ipv6 route X:X::X:X/M X:X::X:X INTERFACE (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_ISISD, no_isis_passwd_cmd_vtysh, 
       "no isis password", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Configure the authentication password for interface\n")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_threshold_warning_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> <1-100> warning-only", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Threshold value (%) at which to generate a warning msg\n"
       "Only give warning message when limit is exceeded\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv4_cmd_vtysh, 
       "redistribute (connected|kernel|ospf|rip|static)", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n")

DEFSH (VTYSH_OSPFD, no_ospf_message_digest_key_cmd_vtysh, 
       "no ospf message-digest-key <1-255>", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n")

DEFSH (VTYSH_OSPFD, ospf_message_digest_key_cmd_vtysh, 
       "ospf message-digest-key <1-255> md5 KEY", 
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n"
       "Use MD5 algorithm\n"
       "The OSPF password (key)")

DEFSH (VTYSH_ZEBRA, no_ip_route_mask_flags_distance_cmd_vtysh, 
       "no ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE) (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_BGPD, neighbor_peer_group_cmd_vtysh, 
       "neighbor WORD peer-group", 
       "Specify neighbor router\n"
       "Neighbor tag\n"
       "Configure peer-group\n")

DEFSH (VTYSH_OSPFD, mpls_te_link_maxbw_cmd_vtysh, 
       "mpls-te link max-bw BANDWIDTH", 
       "MPLS-TE specific commands\n"
       "Configure MPLS-TE link parameters\n"
       "Maximum bandwidth that can be used\n"
       "Bytes/second (IEEE floating point format)\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out the existing ORF prefix-list\n")

DEFSH (VTYSH_ISISD, domain_passwd_snpauth_cmd_vtysh, 
       "domain-password WORD authenticate snp (send-only|validate)", 
       "Set the authentication password for a routing domain\n"
       "Routing domain password\n"
       "Authentication\n"
       "SNP PDUs\n"
       "Send but do not check PDUs on receiving\n"
       "Send and check PDUs on receiving\n")

DEFSH (VTYSH_BGPD, clear_bgp_external_cmd_vtysh, 
       "clear bgp external", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_cmd_vtysh, 
       "ip ospf authentication", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n")

DEFSH (VTYSH_OSPFD, no_ospf_abr_type_cmd_vtysh, 
       "no ospf abr-type (cisco|ibm|shortcut|standard)", 
       "Negate a command or set its defaults\n"
       "OSPF specific commands\n"
       "Set OSPF ABR type\n"
       "Alternative ABR,  cisco implementation\n"
       "Alternative ABR,  IBM implementation\n"
       "Shortcut ABR\n")

DEFSH (VTYSH_BGPD, no_bgp_router_id_cmd_vtysh, 
       "no bgp router-id", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Override configured router identifier\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD, set_metric_addsub_cmd_vtysh, 
       "set metric <+/-metric>", 
       "Set values in destination routing protocol\n"
       "Metric value for destination routing protocol\n"
       "Add or subtract metric\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_self_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" ") (self-originate|)", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "Self-originated link states\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" "|max-age|self-originate)", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "LSAs in MaxAge list\n"
       "Self-originated link states\n")

DEFSH (VTYSH_BGPD, show_ip_as_path_access_list_all_cmd_vtysh, 
       "show ip as-path-access-list", 
       "Show running system information\n"
       "IP information\n"
       "List AS path access lists\n")

DEFSH (VTYSH_ISISD, isis_hello_interval_l2_cmd_vtysh, 
       "isis hello-interval (<1-65535>|minimal) level-2", 
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 second,  interval depends on multiplier\n"
       "Specify hello-interval for level-2 IIHs\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_ge_le_cmd_vtysh, 
       "no ipv6 prefix-list WORD (deny|permit) X:X::X:X/M ge <0-128> le <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_soft_in_cmd_vtysh, 
       "clear ip bgp external soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ISISD, net_cmd_vtysh, 
       "net WORD", 
       "A Network Entity Title for this process (OSI only)\n"
       "XX.XXXX. ... .XXX.XX  Network entity title (NET)\n")

DEFSH (VTYSH_BGPD, match_ip_route_source_prefix_list_cmd_vtysh, 
       "match ip route-source prefix-list WORD", 
       "Match values from routing table\n"
       "IP information\n"
       "Match advertising source address of route\n"
       "Match entries of prefix-lists\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_ISISD, lsp_gen_interval_l2_cmd_vtysh, 
       "lsp-gen-interval level-2 <1-120>", 
       "Minimum interval between regenerating same LSP\n"
       "Set interval for level 2 only\n"
       "Minimum interval in seconds\n")

DEFSH (VTYSH_ZEBRA, ip_route_flags_cmd_vtysh, 
       "ip route A.B.C.D/M (A.B.C.D|INTERFACE) (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_adv_router_linkstate_id_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) "
       "adv-router A.B.C.D linkstate-id A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_transmitdelay_cmd_vtysh, 
       "ipv6 ospf6 transmit-delay <1-3600>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Transmit delay of this interface\n"
       "<1-65535> Seconds\n"
       )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n")

DEFSH (VTYSH_ISISD, no_isis_priority_l1_cmd_vtysh, 
       "no isis priority level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Specify priority for level-1 routing\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_neighbors_peer_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn neighbors A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_soft_out_cmd_vtysh, 
       "clear bgp peer-group WORD soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_neighbor_priority_cmd_vtysh, 
       "no neighbor A.B.C.D priority <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor IP address\n"
       "Neighbor Priority\n"
       "Priority\n")

DEFSH (VTYSH_BGPD, no_neighbor_ebgp_multihop_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "ebgp-multihop", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Allow EBGP neighbors not on directly connected networks\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community_exact_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp view WORD * ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ISISD, isis_metric_cmd_vtysh, 
       "isis metric <0-16777215>", 
       "IS-IS commands\n"
       "Set default metric for circuit\n"
       "Default metric value\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_l2_cmd_vtysh, 
       "no lsp-lifetime level-2", 
       "Negate a command or set its defaults\n"
       "LSP lifetime for Level 2 only in seconds\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community4_exact_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_retransmitinterval_cmd_vtysh, 
       "ipv6 ospf6 retransmit-interval <1-65535>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Time between retransmitting lost link state advertisements\n"
       "<1-65535> Seconds\n"
       )

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_soft_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_soft_cmd_vtysh, 
       "clear ip bgp external soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_all_cmd_vtysh, 
       "no ipv6 access-list WORD", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_longer_detail_cmd_vtysh, 
       "show ipv6 ospf6 route X:X::X:X/M longer detail", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Specify IPv6 prefix\n"
       "Display routes longer than the specified route\n"
       "Detailed information\n"
       )

DEFSH (VTYSH_OSPF6D, ospf6_router_id_cmd_vtysh, 
       "router-id A.B.C.D", 
       "Configure OSPF Router-ID\n"
       "specify by IPv4 address notation(e.g. 0.0.0.0)\n")

DEFSH (VTYSH_RIPD, show_debugging_rip_cmd_vtysh, 
       "show debugging rip", 
       "Show running system information\n"
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_authtype_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_soft_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_onmatch_next_cmd_vtysh, 
       "on-match next", 
       "Exit policy on matches\n"
       "Next clause\n")

DEFSH (VTYSH_RIPD, no_ip_rip_receive_version_num_cmd_vtysh, 
       "no ip rip receive version (1|2)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement reception\n"
       "Version control\n"
       "Version 1\n"
       "Version 2\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_cmd_vtysh, 
       "show ip bgp view WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n")

DEFSH (VTYSH_ISISD, no_debug_isis_snp_cmd_vtysh, 
       "no debug isis snp-packets", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS CSNP/PSNP packets\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_neighbors_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn neighbors", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_routes_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community4_exact_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, show_bgp_view_rsclient_route_cmd_vtysh, 
       "show bgp view WORD rsclient (A.B.C.D|X:X::X:X) X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_prefix_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_ISISD, no_isis_priority_arg_cmd_vtysh, 
       "no isis priority <0-127>", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n")

DEFSH (VTYSH_ALL, access_list_standard_any_cmd_vtysh, 
       "access-list (<1-99>|<1300-1999>) (deny|permit) any", 
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any source host\n")

DEFSH (VTYSH_BGPD, neighbor_override_capability_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "override-capability", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Override capability negotiation result\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_intra_cmd_vtysh, 
       "distance ospf intra-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n")

DEFSH (VTYSH_ZEBRA, show_ipv6_route_summary_cmd_vtysh, 
       "show ipv6 route summary", 
       "Show running system information\n"
       "IP information\n"
       "IPv6 routing table\n"
       "Summary of all IPv6 routes\n")

DEFSH (VTYSH_BGPD, no_set_ipv6_nexthop_global_val_cmd_vtysh, 
       "no set ipv6 next-hop global X:X::X:X", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 global address\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_ISISD, no_isis_priority_l2_cmd_vtysh, 
       "no isis priority level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Specify priority for level-2 routing\n")

DEFSH (VTYSH_BGPD, no_ip_as_path_cmd_vtysh, 
       "no ip as-path access-list WORD (deny|permit) .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "BGP autonomous system path filter\n"
       "Specify an access list name\n"
       "Regular expression access list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_linkstate_network_cmd_vtysh, 
       "show ipv6 ospf6 linkstate network A.B.C.D A.B.C.D", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display linkstate routing table\n"
       "Display Network Entry\n"
       "Specify Router ID as IPv4 address notation\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_prefix_list_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) prefix-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes conforming to the prefix-list\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_BGPD, neighbor_default_originate_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "default-originate", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Originate default route to this neighbor\n")

DEFSH (VTYSH_RIPD, no_rip_redistribute_type_cmd_vtysh, 
       "no redistribute " "(kernel|connected|static|ospf|isis|bgp)", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_OSPFD, ospf_network_area_cmd_vtysh, 
       "network A.B.C.D/M area (A.B.C.D|<0-4294967295>)", 
       "Enable routing on an IP network\n"
       "OSPF network prefix\n"
       "Set the OSPF area ID\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_l2_cmd_vtysh, 
       "no isis hello-multiplier level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Specify hello multiplier for level-2 IIHs\n")

DEFSH (VTYSH_OSPFD, ip_ospf_cost_u32_inet4_cmd_vtysh, 
       "ip ospf cost <1-65535> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_threshold_restart_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> <1-100> restart <1-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Threshold value (%) at which to generate a warning msg\n"
       "Restart bgp connection after limit is exceeded\n"
       "Restart interval in minutes")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community2_exact_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPFD, no_debug_ospf_lsa_sub_cmd_vtysh, 
       "no debug ospf lsa (generate|flooding|install|refresh)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Link State Advertisement\n"
       "LSA Generation\n"
       "LSA Flooding\n"
       "LSA Install/Delete\n"
       "LSA Refres\n")

DEFSH (VTYSH_RIPNGD, ripng_redistribute_type_metric_routemap_cmd_vtysh, 
       "redistribute (kernel|connected|static|ospf6|bgp) metric <0-16> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, bgp_fast_external_failover_cmd_vtysh, 
       "bgp fast-external-failover", 
       "BGP information\n"
       "Immediately reset session if a link to a directly connected external peer goes down\n")

DEFSH (VTYSH_OSPFD, ip_ospf_message_digest_key_cmd_vtysh, 
       "ip ospf message-digest-key <1-255> md5 KEY", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n"
       "Use MD5 algorithm\n"
       "The OSPF password (key)")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_prefix_list_cmd_vtysh, 
       "show ipv6 mbgp prefix-list WORD", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the prefix-list\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_out_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, ipv6_forwarding_cmd_vtysh, 
       "ipv6 forwarding", 
       "IPv6 information\n"
       "Turn on IPv6 forwarding")

DEFSH (VTYSH_BGPD, show_bgp_community_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_RIPD, no_rip_route_cmd_vtysh, 
       "no route A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "RIP static route configuration\n"
       "IP prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_out_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) out", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_RIPNGD, no_set_metric_val_cmd_vtysh, 
       "no set metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Metric value for destination routing protocol\n"
       "Metric value\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_le_ge_cmd_vtysh, 
       "no ipv6 prefix-list WORD (deny|permit) X:X::X:X/M le <0-128> ge <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_router_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) * A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Any Link state ID\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_detail_cmd_vtysh, 
       "show ipv6 ospf6 route (X:X::X:X|X:X::X:X/M|detail|summary)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Specify IPv6 address\n"
       "Specify IPv6 prefix\n"
       "Detailed information\n"
       "Summary of route table\n"
       )

DEFSH (VTYSH_OSPFD, ospf_area_vlink_authkey_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication-key|) AUTH_KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Authentication password (key)\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, neighbor_activate_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "activate", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Enable the Address Family for this Neighbor\n")

DEFSH (VTYSH_OSPFD, ospf_compatible_rfc1583_cmd_vtysh, 
       "compatible rfc1583", 
       "OSPF compatibility list\n"
       "compatible with RFC 1583\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_route_map_cmd_vtysh, 
       "no network A.B.C.D mask A.B.C.D route-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_BGPD, neighbor_description_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "description .LINE", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Neighbor specific description\n"
       "Up to 80 characters describing this neighbor\n")

DEFSH (VTYSH_RIPD, ip_rip_send_version_2_cmd_vtysh, 
       "ip rip send version 2 1", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement transmission\n"
       "Version control\n"
       "RIP version 2\n"
       "RIP version 1\n")

DEFSH (VTYSH_ISISD, dynamic_hostname_cmd_vtysh, 
       "hostname dynamic", 
       "Dynamic hostname for IS-IS\n"
       "Dynamic hostname\n")

DEFSH (VTYSH_BGPD, no_neighbor_enforce_multihop_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "enforce-multihop", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Enforce EBGP neighbors perform multihop\n")

DEFSH (VTYSH_RIPD, distribute_list_cmd_vtysh, 
       "distribute-list WORD (in|out) WORD", 
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_route_map_cmd_vtysh, 
       "show bgp ipv6 route-map WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the route-map\n"
       "A route-map to match on\n")

DEFSH (VTYSH_OSPFD, no_ospf_cost_u32_inet4_cmd_vtysh, 
       "no ospf cost <1-65535> A.B.C.D", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost\n"
       "Address of interface")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, clear_ip_prefix_list_cmd_vtysh, 
       "clear ip prefix-list", 
       "Reset functions\n"
       "IP information\n"
       "Build a prefix list\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_key_addr_cmd_vtysh, 
       "ip ospf authentication-key AUTH_KEY A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Authentication password (key)\n"
       "The OSPF password (key)\n"
       "Address of interface")

DEFSH (VTYSH_OSPFD, ip_ospf_dead_interval_cmd_vtysh, 
       "ip ospf dead-interval <1-65535>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_port_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "port", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Neighbor's BGP port\n")

DEFSH (VTYSH_BGPD, no_neighbor_route_reflector_client_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "route-reflector-client", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure a neighbor as Route Reflector client\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_message_sendrecv_cmd_vtysh, 
       "debug ospf6 message (unknown|hello|dbdesc|lsreq|lsupdate|lsack|all) (send|recv)", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 message\n"
       "Debug Unknown message\n"
       "Debug Hello message\n"
       "Debug Database Description message\n"
       "Debug Link State Request message\n"
       "Debug Link State Update message\n"
       "Debug Link State Acknowledgement message\n"
       "Debug All message\n"
       "Debug only sending message\n"
       "Debug only receiving message\n"
       )

DEFSH (VTYSH_BGPD, no_set_community_delete_cmd_vtysh, 
       "no set comm-list", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "set BGP community list (for deletion)\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community4_exact_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, no_bgp_fast_external_failover_cmd_vtysh, 
       "no bgp fast-external-failover", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Immediately reset session if a link to a directly connected external peer goes down\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_mode_cmd_vtysh, 
       "no ip rip authentication mode", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication mode\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_natural_backdoor_ttl_cmd_vtysh, 
       "no network A.B.C.D backdoor pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n")

DEFSH (VTYSH_BGPD, no_match_pathlimit_as_val_cmd_vtysh, 
       "no match pathlimit as <1-65535>", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "BGP AS-Pathlimit attribute\n"
       "Match Pathlimit ASN\n")

DEFSH (VTYSH_BGPD, show_bgp_prefix_cmd_vtysh, 
       "show bgp X:X::X:X/M", 
       "Show running system information\n"
       "BGP information\n"
       "IPv6 prefix <network>/<length>\n")

DEFSH (VTYSH_BGPD, neighbor_route_server_client_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "route-server-client", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Configure a neighbor as Route Server client\n")

DEFSH (VTYSH_BGPD, show_bgp_community3_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_id_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database * A.B.C.D A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPFD, ospf_neighbor_priority_poll_interval_cmd_vtysh, 
       "neighbor A.B.C.D priority <0-255> poll-interval <1-65535>", 
       "Specify neighbor router\n"
       "Neighbor IP address\n"
       "Neighbor Priority\n"
       "Priority\n"
       "Dead Neighbor Polling interval\n"
       "Seconds\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_message_sendrecv_cmd_vtysh, 
       "no debug ospf6 message "
       "(unknown|hello|dbdesc|lsreq|lsupdate|lsack|all) (send|recv)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 message\n"
       "Debug Unknown message\n"
       "Debug Hello message\n"
       "Debug Database Description message\n"
       "Debug Link State Request message\n"
       "Debug Link State Update message\n"
       "Debug Link State Acknowledgement message\n"
       "Debug All message\n"
       "Debug only sending message\n"
       "Debug only receiving message\n"
       )

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_match_detail_cmd_vtysh, 
       "show ipv6 ospf6 route X:X::X:X/M match detail", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Specify IPv6 prefix\n"
       "Display routes which match the specified route\n"
       "Detailed information\n"
       )

DEFSH (VTYSH_BGPD, show_bgp_instance_ipv6_neighbors_peer_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community_exact_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_ZEBRA, ip_irdp_debug_disable_cmd_vtysh, 
       "ip irdp debug disable", 
       "IP information\n"
       "ICMP Router discovery debug Averts. and Solicits (short)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_advertised_route_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_advertised_route_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_OSPFD, mpls_te_router_addr_cmd_vtysh, 
       "mpls-te router-address A.B.C.D", 
       "MPLS-TE specific commands\n"
       "Stable IP address of the advertising router\n"
       "MPLS-TE router address in IPv4 address format\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_cmd_vtysh, 
       "show ipv6 ospf6", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, show_ipv6_prefix_list_cmd_vtysh, 
       "show ipv6 prefix-list", 
       "Show running system information\n"
       "IPv6 information\n"
       "Build a prefix list\n")

DEFSH (VTYSH_ZEBRA, no_debug_zebra_kernel_cmd_vtysh, 
       "no debug zebra kernel", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra between kernel interface\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_in_prefix_filter_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbors_cmd_vtysh, 
       "show bgp ipv6 neighbors", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_damp_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) dampened-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the dampened routes received from neighbor\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_preference_cmd_vtysh, 
       "ip irdp preference <0-2147483647>", 
       "IP information\n"
       "ICMP Router discovery on this interface\n"
       "Set default preference level for this interface\n"
       "Preference level\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_external_intra_cmd_vtysh, 
       "distance ospf external <1-255> intra-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "External routes\n"
       "Distance for external routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n")

DEFSH (VTYSH_BGPD, no_set_vpnv4_nexthop_cmd_vtysh, 
       "no set vpnv4 next-hop", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "VPNv4 information\n"
       "VPNv4 next-hop address\n")

DEFSH (VTYSH_ISISD, lsp_lifetime_l2_cmd_vtysh, 
       "lsp-lifetime level-2 <380-65535>", 
       "Maximum LSP lifetime for Level 2 only\n"
       "LSP lifetime for Level 2 only in seconds\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_lsa_cmd_vtysh, 
       "no debug ospf lsa", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Link State Advertisement\n")

DEFSH (VTYSH_BGPD, no_match_community_exact_cmd_vtysh, 
       "no match community (<1-99>|<100-500>|WORD) exact-match", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP community list\n"
       "Community-list number (standard)\n"
       "Community-list number (expanded)\n"
       "Community-list name\n"
       "Do exact matching of communities\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_routes_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_rsclient_cmd_vtysh, 
       "clear ip bgp view WORD * rsclient", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, ip_community_list_standard2_cmd_vtysh, 
       "ip community-list <1-99> (deny|permit)", 
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (standard)\n"
       "Specify community to reject\n"
       "Specify community to accept\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_authkey_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication-key|)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Authentication password (key)\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_ipv4_summary_cmd_vtysh, 
       "show ip bgp view WORD ipv4 (unicast|multicast) summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_natural_route_map_cmd_vtysh, 
       "no network A.B.C.D route-map WORD", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Route-map to modify the attributes\n"
       "Name of the route map\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_l1_arg_cmd_vtysh, 
       "no isis csnp-interval <0-65535> level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n"
       "Specify interval for level-1 CSNPs\n")

DEFSH (VTYSH_OSPF6D, no_area_range_cmd_vtysh, 
       "no area A.B.C.D range X:X::X:X/M", 
       "OSPF area parameters\n"
       "Area ID (as an IPv4 notation)\n"
       "Configured address range\n"
       "Specify IPv6 prefix\n"
       )

DEFSH (VTYSH_BGPD, show_bgp_neighbor_damp_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) dampened-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the dampened routes received from neighbor\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv4_metric_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf|rip|static) metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community2_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_RIPD, no_distribute_list_cmd_vtysh, 
       "no distribute-list WORD (in|out) WORD", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community2_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_dead_interval_addr_cmd_vtysh, 
       "no ip ospf dead-interval A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, show_ip_bgp_community_exact_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_hello_interval_cmd_vtysh, 
       "no ip ospf hello-interval", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between HELLO packets\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community4_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_remark_cmd_vtysh, 
       "no ipv6 access-list WORD remark", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Access list entry comment\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_soft_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) soft", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, no_match_origin_cmd_vtysh, 
       "no match origin", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "BGP origin code\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_neighbors_peer_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_nssa_no_summary_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) nssa no-summary", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n"
       "Do not inject inter-area routes into nssa\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_soft_out_cmd_vtysh, 
       "clear ip bgp * soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_summary_cmd_vtysh, 
       "show ip route summary", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "Summary of all routes\n")

DEFSH (VTYSH_BGPD, show_bgp_neighbor_routes_cmd_vtysh, 
       "show bgp neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, aggregate_address_cmd_vtysh, 
       "aggregate-address A.B.C.D/M", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_OSPFD, ospf_area_range_cost_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M cost <0-16777215>", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "User specified metric for this range\n"
       "Advertised metric for this range\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPF6D, ospf6_routemap_no_match_address_prefixlist_cmd_vtysh, 
       "no match ipv6 address prefix-list WORD", 
       "Negate a command or set its defaults\n"
       "Match values\n"
       "IPv6 information\n"
       "Match address of route\n"
       "Match entries of prefix-lists\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_BGPD, old_ipv6_aggregate_address_summary_only_cmd_vtysh, 
       "ipv6 bgp aggregate-address X:X::X:X/M summary-only", 
       "IPv6 information\n"
       "BGP information\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_linkstate_cmd_vtysh, 
       "show ipv6 ospf6 linkstate", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display linkstate routing table\n"
      )

DEFSH (VTYSH_ALL, no_access_list_extended_mask_any_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D any", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "Any destination host\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_aspath_confed_cmd_vtysh, 
       "no bgp bestpath as-path confed", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "AS-path attribute\n"
       "Compare path lengths including confederation sets & sequences in selecting a route\n")

DEFSH (VTYSH_BGPD, no_bgp_network_cmd_vtysh, 
       "no network A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_RIPNGD, ipv6_distribute_list_all_cmd_vtysh, 
       "distribute-list WORD (in|out)", 
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_RIPNGD, no_ripng_network_cmd_vtysh, 
       "no network IF_OR_ADDR", 
       "Negate a command or set its defaults\n"
       "RIPng enable on specified interface or network.\n"
       "Interface or address")

DEFSH (VTYSH_BGPD, bgp_multiple_instance_cmd_vtysh, 
       "bgp multiple-instance", 
       "BGP information\n"
       "Enable bgp multiple instance\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_out_cmd_vtysh, 
       "clear ip bgp external out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_RIPD, no_rip_neighbor_cmd_vtysh, 
       "no neighbor A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Specify a neighbor router\n"
       "Neighbor address\n")

DEFSH (VTYSH_BGPD, no_bgp_log_neighbor_changes_cmd_vtysh, 
       "no bgp log-neighbor-changes", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Log neighbor up/down and reset reason\n")

DEFSH (VTYSH_OSPFD, no_ospf_cost_inet4_cmd_vtysh, 
       "no ospf cost A.B.C.D", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Address of interface")

DEFSH (VTYSH_RIPD, no_if_rmap_cmd_vtysh, 
       "no route-map ROUTEMAP_NAME (in|out) IFNAME", 
       "Negate a command or set its defaults\n"
       "Route map unset\n"
       "Route map name\n"
       "Route map for input filtering\n"
       "Route map for output filtering\n"
       "Route map interface name\n")

DEFSH (VTYSH_BGPD, no_set_aspath_exclude_val_cmd_vtysh, 
       "no set as-path exclude ." "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Transform BGP AS_PATH attribute\n"
       "Exclude from the as-path\n"
       "AS number\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_router_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_cmd_vtysh, 
       "show ip bgp vpnv4 all", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_all_soft_out_cmd_vtysh, 
       "clear bgp view WORD * soft out", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, debug_zebra_events_cmd_vtysh, 
       "debug zebra events", 
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug option set for zebra events\n")

DEFSH (VTYSH_ZEBRA, ip_route_mask_distance_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE|null0) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n"
       "Distance value for this route\n")

DEFSH (VTYSH_ZEBRA, show_ipv6_route_protocol_cmd_vtysh, 
       "show ipv6 route (bgp|connected|isis|kernel|ospf6|ripng|static)", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "Border Gateway Protocol (BGP)\n"
       "Connected\n"
       "ISO IS-IS (ISIS)\n"
       "Kernel\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n")

DEFSH (VTYSH_BGPD, bgp_client_to_client_reflection_cmd_vtysh, 
       "bgp client-to-client reflection", 
       "BGP specific commands\n"
       "Configure client to client route reflection\n"
       "reflection of routes allowed\n")

DEFSH (VTYSH_BGPD, bgp_network_ttl_cmd_vtysh, 
       "network A.B.C.D/M pathlimit <0-255>", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_neighbor_cmd_vtysh, 
       "no debug ospf6 neighbor", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Neighbor\n"
      )

DEFSH (VTYSH_ALL, access_list_exact_cmd_vtysh, 
       "access-list WORD (deny|permit) A.B.C.D/M exact-match", 
       "Add an access list entry\n"
       "IP zebra access-list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 10.0.0.0/8\n"
       "Exact match of the prefixes\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_standard_all_cmd_vtysh, 
       "no ip community-list <1-99>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Community list number (standard)\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_cmd_vtysh, 
       "no ipv6 access-list WORD (deny|permit) X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 3ffe:506::/32\n")

DEFSH (VTYSH_ISISD, csnp_interval_cmd_vtysh, 
       "isis csnp-interval <0-65535>", 
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n")

DEFSH (VTYSH_ZEBRA, ip_route_mask_flags_cmd_vtysh, 
       "ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE) (reject|blackhole)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_instance_all_rsclient_cmd_vtysh, 
       "clear bgp ipv6 view WORD * rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, set_atomic_aggregate_cmd_vtysh, 
       "set atomic-aggregate", 
       "Set values in destination routing protocol\n"
       "BGP atomic aggregate attribute\n" )

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_param2_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_soft_reconfiguration_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "soft-reconfiguration inbound", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Per neighbor soft reconfiguration\n"
       "Allow inbound soft reconfiguration for this neighbor\n")

DEFSH (VTYSH_ALL, show_ipv6_access_list_cmd_vtysh, 
       "show ipv6 access-list", 
       "Show running system information\n"
       "IPv6 information\n"
       "List IPv6 access lists\n")

DEFSH (VTYSH_BGPD, no_bgp_distance_cmd_vtysh, 
       "no distance bgp <1-255> <1-255> <1-255>", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "BGP distance\n"
       "Distance for routes external to the AS\n"
       "Distance for routes internal to the AS\n"
       "Distance for local routes\n")

DEFSH (VTYSH_BGPD, match_community_cmd_vtysh, 
       "match community (<1-99>|<100-500>|WORD)", 
       "Match values from routing table\n"
       "Match BGP community list\n"
       "Community-list number (standard)\n"
       "Community-list number (expanded)\n"
       "Community-list name\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_compare_router_id_cmd_vtysh, 
       "no bgp bestpath compare-routerid", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "Compare router-id for identical EBGP paths\n")

DEFSH (VTYSH_ZEBRA, no_bandwidth_if_cmd_vtysh, 
       "no bandwidth", 
       "Negate a command or set its defaults\n"
       "Set bandwidth informational parameter\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_zebra_cmd_vtysh, 
       "no debug ospf6 zebra", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug connection between zebra\n"
      )

DEFSH (VTYSH_BGPD, no_set_originator_id_cmd_vtysh, 
       "no set originator-id", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP originator ID attribute\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_prefix_cmd_vtysh, 
       "show ip bgp vpnv4 all A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_OSPFD, no_ospf_distribute_list_out_cmd_vtysh, 
       "no distribute-list WORD out " "(kernel|connected|static|rip|isis|bgp)", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter outgoing routing updates\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_rsclient_prefix_cmd_vtysh, 
       "show ip bgp rsclient (A.B.C.D|X:X::X:X) A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_ALL, access_list_extended_host_host_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D host A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_retransmit_interval_cmd_vtysh, 
       "no ip ospf retransmit-interval", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_cmd_vtysh, 
       "clear ip bgp (A.B.C.D|X:X::X:X)", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n")

DEFSH (VTYSH_RIPD, accept_lifetime_month_day_day_month_cmd_vtysh, 
       "accept-lifetime HH:MM:SS MONTH <1-31> <1993-2035> HH:MM:SS <1-31> MONTH <1993-2035>", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Time to expire\n"
       "Day of th month to expire\n"
       "Month of the year to expire\n"
       "Year to expire\n")

DEFSH (VTYSH_BGPD, no_bgp_distance2_cmd_vtysh, 
       "no distance bgp", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "BGP distance\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_all_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp view WORD * in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_OSPFD, debug_ospf_packet_all_cmd_vtysh, 
       "debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all)", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_id_self_originated_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) A.B.C.D self-originated", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display Self-originated LSAs\n"
      )

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community_all_cmd_vtysh, 
       "show ipv6 mbgp community", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_BGPD, aggregate_address_as_set_summary_cmd_vtysh, 
       "aggregate-address A.B.C.D/M as-set summary-only", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Generate AS set path information\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_supernets_cmd_vtysh, 
       "show ip route supernets-only", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "Show supernet entries only\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv6_cmd_vtysh, 
       "redistribute (connected|kernel|ospf6|ripng|static)", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_string2_cmd_vtysh, 
       "no ip rip authentication string LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication string\n"
       "Authentication string\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_ge_le_cmd_vtysh, 
       "no ip prefix-list WORD (deny|permit) A.B.C.D/M ge <0-32> le <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_soft_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, no_router_bgp_view_cmd_vtysh, 
       "no router bgp " "<1-4294967295>" " view WORD", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n"
       "BGP information\n"
       "AS number\n"
       "BGP view\n"
       "view name\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_linkstate_id_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) linkstate-id A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_ISISD, no_debug_isis_spfevents_cmd_vtysh, 
       "no debug isis spf-events", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS Shortest Path First Events\n")

DEFSH (VTYSH_BGPD, old_ipv6_bgp_network_cmd_vtysh, 
       "ipv6 bgp network X:X::X:X/M", 
       "IPv6 information\n"
       "BGP information\n"
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_distribute_list_prefix_cmd_vtysh, 
       "no distribute-list prefix WORD (in|out) WORD", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_type_metric_routemap_cmd_vtysh, 
       "default-information originate metric-type (1|2) metric <0-16777214> route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_prefix_cmd_vtysh, 
       "show ipv6 mbgp X:X::X:X/M", 
       "Show running system information\n"
       "IP information\n"
       "MBGP information\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_ISISD, isis_passwd_cmd_vtysh, 
       "isis password WORD", 
       "IS-IS commands\n"
       "Configure the authentication password for interface\n"
       "Password\n")

DEFSH (VTYSH_ISISD, lsp_lifetime_l1_cmd_vtysh, 
       "lsp-lifetime level-1 <380-65535>", 
       "Maximum LSP lifetime for Level 1 only\n"
       "LSP lifetime for Level 1 only in seconds\n")

DEFSH (VTYSH_OSPFD, ospf_area_nssa_translate_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) nssa (translate-candidate|translate-never|translate-always)", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n"
       "Configure NSSA-ABR for translate election (default)\n"
       "Configure NSSA-ABR to never translate\n"
       "Configure NSSA-ABR to always translate\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_name_expanded_cmd_vtysh, 
       "no ip community-list expanded WORD (deny|permit) .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Specify an expanded community-list\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_OSPFD, ospf_dead_interval_cmd_vtysh, 
       "ospf dead-interval <1-65535>", 
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n"
       "Seconds\n")

DEFSH (VTYSH_OSPFD, ospf_area_stub_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) stub", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as stub\n")

DEFSH (VTYSH_ZEBRA, multicast_cmd_vtysh, 
       "multicast", 
       "Set multicast flag to interface\n")

DEFSH (VTYSH_OSPF6D, no_ipv6_ospf6_advertise_prefix_list_cmd_vtysh, 
       "no ipv6 ospf6 advertise prefix-list", 
       "Negate a command or set its defaults\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Advertising options\n"
       "Filter prefix using prefix-list\n"
       )

DEFSH (VTYSH_ISISD, no_debug_isis_rtevents_cmd_vtysh, 
       "no debug isis route-events", 
       "Disable debugging functions (see also 'debug')\n"
       "IS-IS information\n"
       "IS-IS Route related events\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_zebra_sendrecv_cmd_vtysh, 
       "debug ospf6 zebra (send|recv)", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug connection between zebra\n"
       "Debug Sending zebra\n"
       "Debug Receiving zebra\n"
      )

DEFSH (VTYSH_OSPFD, ospf_area_range_advertise_cost_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M advertise cost <0-16777215>", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "Advertise this range (default)\n"
       "User specified metric for this range\n"
       "Advertised metric for this range\n")

DEFSH (VTYSH_BGPD, no_bgp_graceful_restart_stalepath_time_val_cmd_vtysh, 
       "no bgp graceful-restart stalepath-time <1-3600>", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Graceful restart capability parameters\n"
       "Set the max time to hold onto restarting peer's stale paths\n"
       "Delay value (seconds)\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_soft_in_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_longer_cmd_vtysh, 
       "show ipv6 ospf6 route X:X::X:X/M longer", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Specify IPv6 prefix\n"
       "Display routes longer than the specified route\n"
       )

DEFSH (VTYSH_ALL, no_vty_login_cmd_vtysh, 
       "no login", 
       "Negate a command or set its defaults\n"
       "Enable password checking\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_sequence_number_cmd_vtysh, 
       "ipv6 prefix-list sequence-number", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Include/exclude sequence numbers in NVGEN\n")

DEFSH (VTYSH_BGPD, bgp_default_ipv4_unicast_cmd_vtysh, 
       "bgp default ipv4-unicast", 
       "BGP specific commands\n"
       "Configure BGP defaults\n"
       "Activate ipv4-unicast for a peer by default\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_param3_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535>", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_seq_ge_le_cmd_vtysh, 
       "no ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M ge <0-128> le <0-128>", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community3_exact_cmd_vtysh, 
       "show bgp ipv6 community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_rd_prefix_cmd_vtysh, 
       "show ip bgp vpnv4 rd ASN:nn_or_IP-address:nn A.B.C.D/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information for a route distinguisher\n"
       "VPN Route Distinguisher\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_ALL, no_vty_ipv6_access_class_cmd_vtysh, 
       "no ipv6 access-class [WORD]", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Filter connections based on an IP access list\n"
       "IPv6 access list\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_vpnv4_soft_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D vpnv4 unicast soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_minadvertinterval_cmd_vtysh, 
       "ip irdp minadvertinterval <3-1800>", 
       "IP information\n"
       "ICMP Router discovery on this interface\n"
       "Set minimum time between advertisement\n"
       "Minimum advertisement interval in seconds\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged6_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path med next-hop", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Med attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_next_hop_cmd_vtysh, 
       "no match ip next-hop", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_in_cmd_vtysh, 
       "clear ip bgp external in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_instance_peer_rsclient_cmd_vtysh, 
       "clear ip bgp view WORD (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_cmd_vtysh, 
       "show ip prefix-list", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_cmd_vtysh, 
       "default-information originate", 
       "Control distribution of default information\n"
       "Distribute a default route\n")

DEFSH (VTYSH_ALL, ipv6_access_list_exact_cmd_vtysh, 
       "ipv6 access-list WORD (deny|permit) X:X::X:X/M exact-match", 
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 3ffe:506::/32\n"
       "Exact match of the prefixes\n")

DEFSH (VTYSH_ISISD, isis_priority_l1_cmd_vtysh, 
       "isis priority <0-127> level-1", 
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n"
       "Specify priority for level-1 routing\n")

DEFSH (VTYSH_BGPD, no_dump_bgp_updates_cmd_vtysh, 
       "no dump bgp updates [PATH] [INTERVAL]", 
       "Negate a command or set its defaults\n"
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump BGP updates only\n")

DEFSH (VTYSH_BGPD, no_neighbor_local_as_val2_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "local-as " "<1-4294967295>" " no-prepend", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a local-as number\n"
       "AS number used as local AS\n"
       "Do not prepend local-as to updates from ebgp peers\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_next_hop_prefix_list_cmd_vtysh, 
       "no match ip next-hop prefix-list", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match next-hop address of route\n"
       "Match entries of prefix-lists\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_md5_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(message-digest-key|) <1-255> md5 KEY", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Message digest authentication password (key)\n" "dummy string \n" "Key ID\n" "Use MD5 algorithm\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbor_advertised_route_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_ALL, access_list_extended_any_mask_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip any A.B.C.D A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_BGPD, no_set_local_pref_val_cmd_vtysh, 
       "no set local-preference <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP local preference path attribute\n"
       "Preference value\n")

DEFSH (VTYSH_BGPD, no_bgp_network_backdoor_ttl_cmd_vtysh, 
       "no network A.B.C.D/M backdoor pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify a BGP backdoor route\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)" " route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ALL, access_list_standard_cmd_vtysh, 
       "access-list (<1-99>|<1300-1999>) (deny|permit) A.B.C.D A.B.C.D", 
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Address to match\n"
       "Wildcard bits\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv6_rmap_metric_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf6|ripng|static) route-map WORD metric <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_ifname_pref_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M X:X::X:X INTERFACE <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_RIPNGD, ripng_network_cmd_vtysh, 
       "network IF_OR_ADDR", 
       "RIPng enable on specified interface or network.\n"
       "Interface or address")

DEFSH (VTYSH_BGPD, clear_bgp_peer_rsclient_cmd_vtysh, 
       "clear bgp (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_ALL, access_list_extended_mask_host_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D host A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_rsclient_cmd_vtysh, 
       "show ip bgp view WORD rsclient (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n")

DEFSH (VTYSH_ALL, show_ip_access_list_name_cmd_vtysh, 
       "show ip access-list (<1-99>|<100-199>|<1300-1999>|<2000-2699>|WORD)", 
       "Show running system information\n"
       "IP information\n"
       "List IP access lists\n"
       "IP standard access list\n"
       "IP extended access list\n"
       "IP standard access list (expanded range)\n"
       "IP extended access list (expanded range)\n"
       "IP zebra access-list\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged1_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged (as-path|next-hop|med)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD, vpnv4_network_cmd_vtysh, 
       "network A.B.C.D/M rd ASN:nn_or_IP-address:nn tag WORD", 
       "Specify a network to announce via BGP\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Specify Route Distinguisher\n"
       "VPN Route Distinguisher\n"
       "BGP tag\n"
       "tag value\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_ge_le_cmd_vtysh, 
       "ipv6 prefix-list WORD (deny|permit) X:X::X:X/M ge <0-128> le <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_RIPD, no_ip_rip_receive_version_cmd_vtysh, 
       "no ip rip receive version", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement reception\n"
       "Version control\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPF6D, ospf6_routemap_match_address_prefixlist_cmd_vtysh, 
       "match ipv6 address prefix-list WORD", 
       "Match values\n"
       "IPv6 information\n"
       "Match address of route\n"
       "Match entries of prefix-lists\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_l2_arg_cmd_vtysh, 
       "no isis hello-interval (<1-65535>|minimal) level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 second,  interval depends on multiplier\n"
       "Specify hello-interval for level-2 IIHs\n")

DEFSH (VTYSH_ISISD, isis_hello_multiplier_l2_cmd_vtysh, 
       "isis hello-multiplier <3-1000> level-2", 
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n"
       "Specify hello multiplier for level-2 IIHs\n")

DEFSH (VTYSH_ALL, no_access_list_extended_host_host_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip host A.B.C.D host A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "A single source host\n"
       "Source address\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_BGPD, dump_bgp_all_interval_cmd_vtysh, 
       "dump bgp all PATH INTERVAL", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump all BGP packets\n"
       "Output filename\n"
       "Interval of output\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_soft_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD soft", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_in_cmd_vtysh, 
       "clear bgp peer-group WORD in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_l1_cmd_vtysh, 
       "no lsp-lifetime level-1", 
       "Negate a command or set its defaults\n"
       "LSP lifetime for Level 1 only in seconds\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_args_addr_cmd_vtysh, 
       "ip ospf authentication (null|message-digest) A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n"
       "Use null authentication\n"
       "Use message-digest authentication\n"
       "Address of interface")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_type_metric_routemap_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)"
         " metric-type (1|2) metric <0-16777214> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "OSPF exterior metric type for redistributed routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Metric for redistributed routes\n"
       "OSPF default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_RIPNGD, no_ripng_offset_list_cmd_vtysh, 
       "no offset-list WORD (in|out) <0-16>", 
       "Negate a command or set its defaults\n"
       "Modify RIPng metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n")

DEFSH (VTYSH_ISISD, lsp_lifetime_cmd_vtysh, 
       "lsp-lifetime <380-65535>", 
       "Maximum LSP lifetime\n"
       "LSP lifetime in seconds\n")

DEFSH (VTYSH_BGPD, bgp_enforce_first_as_cmd_vtysh, 
       "bgp enforce-first-as", 
       "BGP information\n"
       "Enforce the first AS for EBGP routes\n")

DEFSH (VTYSH_ZEBRA, no_debug_zebra_rib_cmd_vtysh, 
       "no debug zebra rib", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra RIB\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp peer-group WORD ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_zebra_sendrecv_cmd_vtysh, 
       "no debug ospf6 zebra (send|recv)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug connection between zebra\n"
       "Debug Sending zebra\n"
       "Debug Receiving zebra\n"
      )

DEFSH (VTYSH_BGPD, show_bgp_instance_ipv6_rsclient_summary_cmd_vtysh, 
       "show bgp view WORD ipv6 rsclient summary", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_route_map_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) route-map WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the route-map\n"
       "A route-map to match on\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_remark_arg_cmd_vtysh, 
       "no ipv6 access-list WORD remark .LINE", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Access list entry comment\n"
       "Comment up to 100 characters\n")

DEFSH (VTYSH_BGPD, show_bgp_community_list_exact_cmd_vtysh, 
       "show bgp community-list (<1-500>|WORD) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_stub_no_summary_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) stub no-summary", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as stub\n"
       "Do not inject inter-area routes into area\n")

DEFSH (VTYSH_BGPD, no_neighbor_peer_group_cmd_vtysh, 
       "no neighbor WORD peer-group", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor tag\n"
       "Configure peer-group\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_l2_cmd_vtysh, 
       "no isis hello-interval level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Specify hello-interval for level-2 IIHs\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_address_preference_cmd_vtysh, 
       "ip irdp address A.B.C.D preference <0-2147483647>", 
       "IP information\n"
       "Alter ICMP Router discovery preference this interface\n"
       "Specify IRDP non-default preference to advertise\n"
       "Set IRDP address for advertise\n"
       "Preference level\n")

DEFSH (VTYSH_RIPD, rip_timers_cmd_vtysh, 
       "timers basic <5-2147483647> <5-2147483647> <5-2147483647>", 
       "Adjust routing timers\n"
       "Basic routing protocol update timers\n"
       "Routing table update timer value in second. Default is 30.\n"
       "Routing information timeout timer. Default is 180.\n"
       "Garbage collection timer. Default is 120.\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_metric_routemap_cmd_vtysh, 
       "default-information originate always metric <0-16777214> route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, no_bgp_timers_arg_cmd_vtysh, 
       "no timers bgp <0-65535> <0-65535>", 
       "Negate a command or set its defaults\n"
       "Adjust routing timers\n"
       "BGP timers\n"
       "Keepalive interval\n"
       "Holdtime\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_natural_ttl_cmd_vtysh, 
       "no network A.B.C.D pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_linkstate_detail_cmd_vtysh, 
       "show ipv6 ospf6 linkstate detail", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display linkstate routing table\n"
      )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_soft_in_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, no_bgp_network_import_check_cmd_vtysh, 
       "no bgp network import-check", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "BGP network command\n"
       "Check BGP network route exists in IGP\n")

DEFSH (VTYSH_OSPF6D, ospf6_interface_area_cmd_vtysh, 
       "interface IFNAME area A.B.C.D", 
       "Enable routing on an IPv6 interface\n"
       "Interface name(e.g. ep0)\n"
       "Specify the OSPF6 area ID\n"
       "OSPF6 area ID in IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPFD, show_ip_ospf_database_type_id_cmd_vtysh, 
       "show ip ospf database (" "asbr-summary|external|network|router|summary" "|nssa-external" "|opaque-link|opaque-area|opaque-as" ") A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Database summary\n"
       "ASBR summary link states\n" "External link states\n" "Network link states\n" "Router link states\n" "Network summary link states\n" "NSSA external link state\n" "Link local Opaque-LSA\n" "Link area Opaque-LSA\n" "Link AS Opaque-LSA\n"
       "Link State ID (as an IP address)\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_authtype_authkey_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) "
       "(authentication-key|)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n"
       "Authentication password (key)\n" "The OSPF password (key)")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_rmap_onmatch_goto_cmd_vtysh, 
       "no on-match goto", 
       "Negate a command or set its defaults\n"
       "Exit policy on matches\n"
       "Goto Clause number\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_area_spf_tree_cmd_vtysh, 
       "show ipv6 ospf6 area A.B.C.D spf tree", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Area information\n"
       "Area ID (as an IPv4 notation)\n"
       "Shortest Path First caculation\n"
       "Show SPF tree\n")

DEFSH (VTYSH_BGPD, address_family_ipv6_safi_cmd_vtysh, 
       "address-family ipv6 (unicast|multicast)", 
       "Enter Address Family command mode\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_rsclient_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community4_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_prefix_cmd_vtysh, 
       "show ipv6 bgp X:X::X:X/M", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv6_rmap_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf6|ripng|static) route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_rsclient_cmd_vtysh, 
       "clear ip bgp * rsclient", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_route_match_cmd_vtysh, 
       "show ipv6 ospf6 route X:X::X:X/M match", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Routing Table\n"
       "Specify IPv6 prefix\n"
       "Display routes which match the specified route\n"
       )

DEFSH (VTYSH_RIPD, debug_rip_packet_detail_cmd_vtysh, 
       "debug rip packet (recv|send) detail", 
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP packet\n"
       "RIP receive packet\n"
       "RIP send packet\n"
       "Detailed information display\n")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_flap_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) flap-statistics", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display flap statistics of the routes learned from neighbor\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_priority_cmd_vtysh, 
       "ipv6 ospf6 priority <0-255>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Router priority\n"
       "Priority value\n"
       )

DEFSH (VTYSH_RIPNGD, no_ripng_aggregate_address_cmd_vtysh, 
       "no aggregate-address X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "Delete aggregate RIPng route announcement\n"
       "Aggregate network")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_seq_le_ge_cmd_vtysh, 
       "no ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M le <0-32> ge <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_asbr_cmd_vtysh, 
       "no debug ospf6 asbr", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 ASBR function\n"
      )

DEFSH (VTYSH_RIPD, no_ip_rip_send_version_cmd_vtysh, 
       "no ip rip send version", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement transmission\n"
       "Version control\n")

DEFSH (VTYSH_BGPD, set_ip_nexthop_peer_cmd_vtysh, 
       "set ip next-hop peer-address", 
       "Set values in destination routing protocol\n"
       "IP information\n"
       "Next hop address\n"
       "Use peer address (for BGP only)\n")

DEFSH (VTYSH_BGPD, ip_as_path_cmd_vtysh, 
       "ip as-path access-list WORD (deny|permit) .LINE", 
       "IP information\n"
       "BGP autonomous system path filter\n"
       "Specify an access list name\n"
       "Regular expression access list name\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_BGPD, show_bgp_views_cmd_vtysh, 
       "show bgp views", 
       "Show running system information\n"
       "BGP information\n"
       "Show the defined BGP views\n")

DEFSH (VTYSH_ISISD, show_debugging_cmd_vtysh, 
       "show debugging", 
       "Show running system information\n"
       "State of each debugging option\n")

DEFSH (VTYSH_BGPD, no_bgp_bestpath_med_cmd_vtysh, 
       "no bgp bestpath med (confed|missing-as-worst)", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Compare MED among confederation paths\n"
       "Treat missing MED as the least preferred one\n")

DEFSH (VTYSH_BGPD, neighbor_timers_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "timers <0-65535> <0-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP per neighbor timers\n"
       "Keepalive interval\n"
       "Holdtime\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_abr_cmd_vtysh, 
       "no debug ospf6 abr", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 ABR function\n"
      )

DEFSH (VTYSH_ZEBRA, show_ipv6_forwarding_cmd_vtysh, 
       "show ipv6 forwarding", 
       "Show running system information\n"
       "IPv6 information\n"
       "Forwarding status\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_summary_name_cmd_vtysh, 
       "show ip prefix-list summary WORD", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Summary of prefix lists\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_ALL, no_terminal_monitor_cmd_vtysh, 
       "no terminal monitor", 
       "Negate a command or set its defaults\n"
       "Set terminal line parameters\n"
       "Copy debug output to the current terminal line\n")

DEFSH (VTYSH_RIPNGD, ipv6_ripng_split_horizon_cmd_vtysh, 
       "ipv6 ripng split-horizon", 
       "IPv6 information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_peer_rsclient_cmd_vtysh, 
       "clear bgp view WORD (A.B.C.D|X:X::X:X) rsclient", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "BGP neighbor IP address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig for rsclient RIB\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_in_prefix_filter_cmd_vtysh, 
       "clear bgp ipv6 * in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, debug_bgp_normal_cmd_vtysh, 
       "debug bgp", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_cmd_vtysh, 
       "no isis hello-interval", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n")

DEFSH (VTYSH_BGPD, no_bgp_network_mask_ttl_cmd_vtysh, 
       "no network A.B.C.D mask A.B.C.D pathlimit <0-255>", 
       "Negate a command or set its defaults\n"
       "Specify a network to announce via BGP\n"
       "Network number\n"
       "Network mask\n"
       "Network mask\n"
       "AS-Path hopcount limit attribute\n"
       "AS-Pathlimit TTL,  in number of AS-Path hops\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_out_cmd_vtysh, 
       "clear bgp ipv6 external WORD out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ZEBRA, no_multicast_cmd_vtysh, 
       "no multicast", 
       "Negate a command or set its defaults\n"
       "Unset multicast flag to interface\n")

DEFSH (VTYSH_RIPNGD, no_debug_ripng_packet_cmd_vtysh, 
       "no debug ripng packet", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng packet\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_neighbor_received_routes_cmd_vtysh, 
       "show ip bgp view WORD neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_BGPD, debug_bgp_zebra_cmd_vtysh, 
       "debug bgp zebra", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP Zebra messages\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_border_routers_detail_cmd_vtysh, 
       "show ipv6 ospf6 border-routers (A.B.C.D|detail)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display routing table for ABR and ASBR\n"
       "Specify Router-ID\n"
       "Display Detail\n"
      )

DEFSH (VTYSH_RIPD, ip_rip_send_version_cmd_vtysh, 
       "ip rip send version (1|2)", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement transmission\n"
       "Version control\n"
       "RIP version 1\n"
       "RIP version 2\n")

DEFSH (VTYSH_RIPD, no_rip_distance_source_cmd_vtysh, 
       "no distance <1-255> A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Administrative distance\n"
       "Distance value\n"
       "IP source prefix\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_retransmit_interval_addr_cmd_vtysh, 
       "no ip ospf retransmit-interval A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, no_neighbor_weight_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "weight <0-65535>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Set default weight for routes from this neighbor\n"
       "default weight\n")

DEFSH (VTYSH_BGPD, no_bgp_distance_source_cmd_vtysh, 
       "no distance <1-255> A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Define an administrative distance\n"
       "Administrative distance\n"
       "IP source prefix\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_paths_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) paths", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Path information\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community2_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPFD, router_ospf_id_cmd_vtysh, 
       "router-id A.B.C.D", 
       "router-id for the OSPF process\n"
       "OSPF router-id in IP address format\n")

DEFSH (VTYSH_BGPD, show_bgp_memory_cmd_vtysh, 
       "show bgp memory", 
       "Show running system information\n"
       "BGP information\n"
       "Global BGP memory statistics\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_param4_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_BGPD, neighbor_set_peer_group_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "peer-group WORD", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Member of the peer-group\n"
       "peer-group name\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_cost_cmd_vtysh, 
       "ipv6 ospf6 cost <1-65535>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface cost\n"
       "Outgoing metric of this interface\n"
       )

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_metric_type_cmd_vtysh, 
       "default-information originate metric <0-16777214> metric-type (1|2)", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_flags_pref_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M (X:X::X:X|INTERFACE) (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this prefix\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_type_metric_cmd_vtysh, 
       "default-information originate metric-type (1|2) metric <0-16777214>", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "OSPF default metric\n"
       "OSPF metric\n")

DEFSH (VTYSH_ALL, no_service_advanced_vty_cmd_vtysh, 
       "no service advanced-vty", 
       "Negate a command or set its defaults\n"
       "Set up miscellaneous service\n"
       "Enable advanced mode vty interface\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_out_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_default_cost_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) default-cost <0-16777215>", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Set the summary-default cost of a NSSA or stub area\n"
       "Stub's advertised default summary cost\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_regexp_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) regexp .LINE", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_BGPD, match_ecommunity_cmd_vtysh, 
       "match extcommunity (<1-99>|<100-500>|WORD)", 
       "Match values from routing table\n"
       "Match BGP/VPN extended community list\n"
       "Extended community-list number (standard)\n"
       "Extended community-list number (expanded)\n"
       "Extended community-list name\n")

DEFSH (VTYSH_BGPD, no_ip_community_list_name_standard_cmd_vtysh, 
       "no ip community-list standard WORD (deny|permit) .AA:NN", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a community list entry\n"
       "Specify a standard community-list\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Community number in aa:nn format or internet|local-AS|no-advertise|no-export\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_summary_cmd_vtysh, 
       "show ip bgp view WORD summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_ifname_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M X:X::X:X INTERFACE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n")

DEFSH (VTYSH_OSPFD, ospf_authentication_key_cmd_vtysh, 
       "ospf authentication-key AUTH_KEY", 
       "OSPF interface commands\n"
       "Authentication password (key)\n"
       "The OSPF password (key)")

DEFSH (VTYSH_OSPFD, no_debug_ospf_packet_all_cmd_vtysh, 
       "no debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_soft_out_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, ospf_passive_interface_default_cmd_vtysh, 
       "passive-interface default", 
       "Suppress routing updates on an interface\n"
       "Suppress routing updates on interfaces by default\n")

DEFSH (VTYSH_ISISD, csnp_interval_l1_cmd_vtysh, 
       "isis csnp-interval <0-65535> level-1", 
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n"
       "Specify interval for level-1 CSNPs\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_route_map_cmd_vtysh, 
       "no route-map WORD (deny|permit) <1-65535>", 
       "Negate a command or set its defaults\n"
       "Create route-map or enter route-map command mode\n"
       "Route map tag\n"
       "Route map denies set operations\n"
       "Route map permits set operations\n"
       "Sequence to insert to/delete from existing route-map entry\n")

DEFSH (VTYSH_BGPD, neighbor_interface_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "interface WORD", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Interface\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, neighbor_send_community_type_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "send-community (both|extended|standard)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Send Community attribute to this neighbor\n"
       "Send Standard and Extended Community attributes\n"
       "Send Extended Community attributes\n"
       "Send Standard Community attributes\n")

DEFSH (VTYSH_ZEBRA, no_ip_address_cmd_vtysh, 
       "no ip address A.B.C.D/M", 
       "Negate a command or set its defaults\n"
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP Address (e.g. 10.0.0.1/8)")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_flap_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) flap-statistics", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display flap statistics of the routes learned from neighbor\n")

DEFSH (VTYSH_OSPFD, ip_ospf_network_cmd_vtysh, 
       "ip ospf network (broadcast|non-broadcast|point-to-multipoint|point-to-point)", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Network type\n"
       "Specify OSPF broadcast multi-access network\n"
       "Specify OSPF NBMA network\n"
       "Specify OSPF point-to-multipoint network\n"
       "Specify OSPF point-to-point network\n")

DEFSH (VTYSH_BGPD, no_neighbor_send_community_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "send-community", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Send Community attribute to this neighbor\n")

DEFSH (VTYSH_BGPD, no_neighbor_strict_capability_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X) " "strict-capability-match", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "Strict capability negotiation match\n")

DEFSH (VTYSH_BGPD, no_neighbor_update_source_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "update-source", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Source of routing updates\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_l1_cmd_vtysh, 
       "no isis csnp-interval level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "Specify interval for level-1 CSNPs\n")

DEFSH (VTYSH_OSPF6D, no_area_filter_list_cmd_vtysh, 
       "no area A.B.C.D filter-list prefix WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Filter networks between OSPFv6 areas\n"
       "Filter prefixes between OSPFv6 areas\n"
       "Name of an IPv6 prefix-list\n"
       "Filter networks sent to this area\n"
       "Filter networks sent from this area\n")

DEFSH (VTYSH_ZEBRA, show_debugging_zebra_cmd_vtysh, 
       "show debugging zebra", 
       "Show running system information\n"
       "Zebra configuration\n"
       "Debugging information\n")

DEFSH (VTYSH_BGPD, ip_community_list_name_standard_cmd_vtysh, 
       "ip community-list standard WORD (deny|permit) .AA:NN", 
       "IP information\n"
       "Add a community list entry\n"
       "Add a standard community-list entry\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Community number in aa:nn format or internet|local-AS|no-advertise|no-export\n")

DEFSH (VTYSH_BGPD, show_bgp_view_ipv6_neighbor_received_prefix_filter_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_out_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " out", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPF6D, show_zebra_cmd_vtysh, 
       "show zebra", 
       "Show running system information\n"
       "Zebra information\n")

DEFSH (VTYSH_ISISD, no_lsp_gen_interval_cmd_vtysh, 
       "no lsp-gen-interval", 
       "Negate a command or set its defaults\n"
       "Minimum interval between regenerating same LSP\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_as_set_summary_cmd_vtysh, 
       "no aggregate-address A.B.C.D/M as-set summary-only", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Generate AS set path information\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_ALL, vty_restricted_mode_cmd_vtysh, 
       "anonymous restricted", 
       "Restrict view commands available in anonymous,  unauthenticated vty\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_summary_cmd_vtysh, 
       "show ipv6 mbgp summary", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Summary of BGP neighbor status\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n")

DEFSH (VTYSH_OSPF6D, no_ospf6_redistribute_cmd_vtysh, 
       "no redistribute (static|kernel|connected|ripng|bgp)", 
       "Negate a command or set its defaults\n"
       "Redistribute\n"
       "Static route\n"
       "Kernel route\n"
       "Connected route\n"
       "RIPng route\n"
       "BGP route\n"
      )

DEFSH (VTYSH_BGPD, show_bgp_ipv6_community_list_exact_cmd_vtysh, 
       "show bgp ipv6 community-list (<1-500>|WORD) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_OSPF6D, debug_ospf6_interface_cmd_vtysh, 
       "debug ospf6 interface", 
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Interface\n"
      )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_in_cmd_vtysh, 
       "clear bgp ipv6 external WORD in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_neighbor_routes_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X)", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_range_advertise_cost_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) range A.B.C.D/M advertise cost <0-16777215>", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n"
       "Advertise this range (default)\n"
       "User specified metric for this range\n"
       "Advertised metric for this range\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community4_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_ZEBRA, no_router_id_cmd_vtysh, 
       "no router-id", 
       "Negate a command or set its defaults\n"
       "Remove the manually configured router-id\n")

DEFSH (VTYSH_BGPD|VTYSH_OSPFD|VTYSH_RIPD|VTYSH_ZEBRA, no_match_ip_address_val_cmd_vtysh, 
       "no match ip address (<1-199>|<1300-2699>|WORD)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "IP information\n"
       "Match address of route\n"
       "IP access-list number\n"
       "IP access-list number (expanded range)\n"
       "IP Access-list name\n")

DEFSH (VTYSH_BGPD, no_neighbor_attr_unchanged10_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged med as-path next-hop", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Med attribute\n"
       "As-path attribute\n"
       "Nexthop attribute\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_vpnv4_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " vpnv4 unicast out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_soft_cmd_vtysh, 
       "clear bgp ipv6 external soft", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_in_prefix_filter_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_update_cmd_vtysh, 
       "no debug bgp updates", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP updates\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_ipv6_prefix_list_seq_cmd_vtysh, 
       "no ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) (X:X::X:X/M|any)", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Any prefix match.  Same as \"::0/0 le 128\"\n")

DEFSH (VTYSH_OSPFD|VTYSH_OSPF6D, ospf6_routemap_set_metric_type_cmd_vtysh, 
       "set metric-type (type-1|type-2)", 
       "Set value\n"
       "Type of metric\n"
       "OSPF6 external type 1 metric\n"
       "OSPF6 external type 2 metric\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database * * A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Any Link state ID\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_confederation_identifier_arg_cmd_vtysh, 
       "no bgp confederation identifier " "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "AS confederation parameters\n"
       "AS number\n"
       "Set routing domain confederation AS\n")

DEFSH (VTYSH_ZEBRA, no_ip_protocol_cmd_vtysh, 
       "no ip protocol PROTO", 
       "Negate a command or set its defaults\n"
       "Remove route map from PROTO\n"
       "Protocol name\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_deadinterval_cmd_vtysh, 
       "ipv6 ospf6 dead-interval <1-65535>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interval time after which a neighbor is declared down\n"
       "<1-65535> Seconds\n"
       )

DEFSH (VTYSH_RIPD, send_lifetime_infinite_day_month_cmd_vtysh, 
       "send-lifetime HH:MM:SS <1-31> MONTH <1993-2035> infinite", 
       "Set send lifetime of the key\n"
       "Time to start\n"
       "Day of th month to start\n"
       "Month of the year to start\n"
       "Year to start\n"
       "Never expires")

DEFSH (VTYSH_ALL, access_list_extended_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D A.B.C.D A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "Destination address\n"
       "Destination Wildcard bits\n")

DEFSH (VTYSH_ISISD, debug_isis_upd_cmd_vtysh, 
       "debug isis update-packets", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS Update related packets\n")

DEFSH (VTYSH_OSPFD, ip_ospf_retransmit_interval_addr_cmd_vtysh, 
       "ip ospf retransmit-interval <3-65535> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n"
       "Seconds\n"
       "Address of interface")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_detail_cmd_vtysh, 
       "show ip ospf neighbor detail", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "detail of all neighbors\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, show_ip_prefix_list_name_seq_cmd_vtysh, 
       "show ip prefix-list WORD seq <1-4294967295>", 
       "Show running system information\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_description_arg_cmd_vtysh, 
       "no ip prefix-list WORD description .LINE", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n"
       "Up to 80 characters describing this prefix-list\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_authtype_md5_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(authentication|) "
       "(message-digest-key|)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Enable authentication on this virtual link\n" "dummy string \n"
       "Message digest authentication password (key)\n" "dummy string \n" "Key ID\n" "Use MD5 algorithm\n" "The OSPF password (key)")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv6_rmap_cmd_vtysh, 
       "redistribute (connected|kernel|ospf6|ripng|static) route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_RIPD, no_distribute_list_all_cmd_vtysh, 
       "no distribute-list WORD (in|out)", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n")

DEFSH (VTYSH_RIPD, rip_redistribute_rip_cmd_vtysh, 
       "redistribute rip", 
       "Redistribute information from another routing protocol\n"
       "Routing Information Protocol (RIP)\n")

DEFSH (VTYSH_BGPD, show_ip_extcommunity_list_cmd_vtysh, 
       "show ip extcommunity-list", 
       "Show running system information\n"
       "IP information\n"
       "List extended-community list\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_soft_in_cmd_vtysh, 
       "clear bgp ipv6 external soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, no_ospf_cost_cmd_vtysh, 
       "no ospf cost", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Interface cost\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_filter_list_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) filter-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes conforming to the filter-list\n"
       "Regular expression access list name\n")

DEFSH (VTYSH_BGPD, ipv6_bgp_neighbor_advertised_route_cmd_vtysh, 
       "show ipv6 bgp neighbors (A.B.C.D|X:X::X:X) advertised-routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the routes advertised to a BGP neighbor\n")

DEFSH (VTYSH_BGPD, bgp_graceful_restart_stalepath_time_cmd_vtysh, 
       "bgp graceful-restart stalepath-time <1-3600>", 
       "BGP specific commands\n"
       "Graceful restart capability parameters\n"
       "Set the max time to hold onto restarting peer's stale paths\n"
       "Delay value (seconds)\n")

DEFSH (VTYSH_OSPFD, debug_ospf_nssa_cmd_vtysh, 
       "debug ospf nssa", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF nssa information\n")

DEFSH (VTYSH_BGPD, set_aspath_exclude_cmd_vtysh, 
       "set as-path exclude ." "<1-4294967295>", 
       "Set values in destination routing protocol\n"
       "Transform BGP AS-path attribute\n"
       "Exclude from the as-path\n"
       "AS number\n")

DEFSH (VTYSH_ISISD, debug_isis_events_cmd_vtysh, 
       "debug isis events", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS Events\n")

DEFSH (VTYSH_BGPD, neighbor_passive_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "passive", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Don't send open messages to this neighbor\n")

DEFSH (VTYSH_BGPD, neighbor_timers_connect_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X) " "timers connect <0-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nIPv6 address\n"
       "BGP per neighbor timers\n"
       "BGP connect timer\n"
       "Connect timer\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_key_chain_cmd_vtysh, 
       "no ip rip authentication key-chain", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication key-chain\n")

DEFSH (VTYSH_OSPFD, debug_ospf_event_cmd_vtysh, 
       "debug ospf event", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF event information\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_soft_out_cmd_vtysh, 
       "clear ip bgp peer-group WORD soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, old_ipv6_aggregate_address_cmd_vtysh, 
       "ipv6 bgp aggregate-address X:X::X:X/M", 
       "IPv6 information\n"
       "BGP information\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community_all_cmd_vtysh, 
       "show ipv6 bgp community", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, ospf_log_adjacency_changes_detail_cmd_vtysh, 
       "log-adjacency-changes detail", 
       "Log changes in adjacency state\n"
       "Log all state changes\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_soft_cmd_vtysh, 
       "clear ip bgp peer-group WORD soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n")

DEFSH (VTYSH_ALL, access_list_extended_any_host_cmd_vtysh, 
       "access-list (<100-199>|<2000-2699>) (deny|permit) ip any host A.B.C.D", 
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_hello_interval_addr_cmd_vtysh, 
       "no ip ospf hello-interval A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between HELLO packets\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_ipv4_soft_cmd_vtysh, 
       "clear ip bgp A.B.C.D ipv4 (unicast|multicast) soft", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Address Family Modifier\n"
       "Soft reconfig\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_description_cmd_vtysh, 
       "no ip prefix-list WORD description", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Prefix-list specific description\n")

DEFSH (VTYSH_ISISD, isis_priority_cmd_vtysh, 
       "isis priority <0-127>", 
       "IS-IS commands\n"
       "Set priority for Designated Router election\n"
       "Priority value\n")

DEFSH (VTYSH_BGPD, old_no_ipv6_bgp_network_cmd_vtysh, 
       "no ipv6 bgp network X:X::X:X/M", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "BGP information\n"
       "Specify a network to announce via BGP\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n")

DEFSH (VTYSH_OSPF6D, area_range_advertise_cmd_vtysh, 
       "area A.B.C.D range X:X::X:X/M (advertise|not-advertise)", 
       "OSPF area parameters\n"
       "Area ID (as an IPv4 notation)\n"
       "Configured address range\n"
       "Specify IPv6 prefix\n"
       )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_cmd_vtysh, 
       "clear bgp ipv6 external", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n")

DEFSH (VTYSH_RIPD, no_rip_version_cmd_vtysh, 
       "no version", 
       "Negate a command or set its defaults\n"
       "Set routing protocol version\n")

DEFSH (VTYSH_BGPD, match_ipv6_next_hop_cmd_vtysh, 
       "match ipv6 next-hop X:X::X:X", 
       "Match values from routing table\n"
       "IPv6 information\n"
       "Match IPv6 next-hop address of route\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_RIPNGD, no_ripng_redistribute_type_cmd_vtysh, 
       "no redistribute (kernel|connected|static|ospf6|bgp)", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_BGPD, bgp_confederation_peers_cmd_vtysh, 
       "bgp confederation peers ." "<1-4294967295>", 
       "BGP specific commands\n"
       "AS confederation parameters\n"
       "Peer ASs in BGP confederation\n"
       "AS number\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_detail_cmd_vtysh, 
       "show ipv6 ospf6 database (detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community3_exact_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, clear_ip_prefix_list_name_prefix_cmd_vtysh, 
       "clear ip prefix-list WORD A.B.C.D/M", 
       "Reset functions\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, clear_ipv6_prefix_list_cmd_vtysh, 
       "clear ipv6 prefix-list", 
       "Reset functions\n"
       "IPv6 information\n"
       "Build a prefix list\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbors_peer_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_packet_send_recv_detail_cmd_vtysh, 
       "no debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all) (send|recv) (detail|)", 
       "Negate a command or set its defaults\n"
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n"
       "Packet sent\n"
       "Packet received\n"
       "Detail Information\n")

DEFSH (VTYSH_ISISD, no_spf_interval_l1_cmd_vtysh, 
       "no spf-interval level-1", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n"
       "Set interval for level 1 only\n")

DEFSH (VTYSH_RIPD, ip_rip_split_horizon_poisoned_reverse_cmd_vtysh, 
       "ip rip split-horizon poisoned-reverse", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Perform split horizon\n"
       "With poisoned-reverse\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_arg_cmd_vtysh, 
       "no isis csnp-interval <0-65535>", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n")

DEFSH (VTYSH_ISISD, show_clns_neighbors_cmd_vtysh, 
       "show clns neighbors", 
       "Show running system information\n"
       "clns network information\n"
       "CLNS neighbor adjacencies\n")

DEFSH (VTYSH_OSPFD, debug_ospf_ism_sub_cmd_vtysh, 
       "debug ospf ism (status|events|timers)", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Interface State Machine\n"
       "ISM Status Information\n"
       "ISM Event Information\n"
       "ISM TImer Information\n")

DEFSH (VTYSH_OSPFD, ospf_area_range_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) range A.B.C.D/M", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Summarize routes matching address/mask (border routers only)\n"
       "Area range prefix\n")

DEFSH (VTYSH_ALL, no_ipv6_access_list_exact_cmd_vtysh, 
       "no ipv6 access-list WORD (deny|permit) X:X::X:X/M exact-match", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 3ffe:506::/32\n"
       "Exact match of the prefixes\n")

DEFSH (VTYSH_BGPD, bgp_distance_cmd_vtysh, 
       "distance bgp <1-255> <1-255> <1-255>", 
       "Define an administrative distance\n"
       "BGP distance\n"
       "Distance for routes external to the AS\n"
       "Distance for routes internal to the AS\n"
       "Distance for local routes\n")

DEFSH (VTYSH_BGPD, neighbor_shutdown_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "shutdown", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Administratively shut down this neighbor\n")

DEFSH (VTYSH_RIPNGD, no_ipv6_distribute_list_cmd_vtysh, 
       "no distribute-list WORD (in|out) WORD", 
       "Negate a command or set its defaults\n"
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_OSPFD, ip_ospf_retransmit_interval_cmd_vtysh, 
       "ip ospf retransmit-interval <3-65535>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n"
       "Seconds\n")

DEFSH (VTYSH_OSPFD, ospf_area_nssa_no_summary_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) nssa no-summary", 
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as nssa\n"
       "Do not inject inter-area routes into nssa\n")

DEFSH (VTYSH_ISISD, show_database_cmd_vtysh, 
       "show isis database", 
       "Show running system information\n" "IS-IS information\n" "IS-IS link state database\n")

DEFSH (VTYSH_OSPFD, mpls_te_link_unrsv_bw_cmd_vtysh, 
       "mpls-te link unrsv-bw <0-7> BANDWIDTH", 
       "MPLS-TE specific commands\n"
       "Configure MPLS-TE link parameters\n"
       "Unreserved bandwidth at each priority level\n"
       "Priority\n"
       "Bytes/second (IEEE floating point format)\n")

DEFSH (VTYSH_ZEBRA, no_ipv6_route_ifname_flags_cmd_vtysh, 
       "no ipv6 route X:X::X:X/M X:X::X:X INTERFACE (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IPv6 destination prefix (e.g. 3ffe:506::/32)\n"
       "IPv6 gateway address\n"
       "IPv6 gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_ZEBRA, ip_route_flags_distance_cmd_vtysh, 
       "ip route A.B.C.D/M (A.B.C.D|INTERFACE) (reject|blackhole) <1-255>", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_BGPD, no_debug_bgp_normal_cmd_vtysh, 
       "no debug bgp", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n")

DEFSH (VTYSH_BGPD, no_neighbor_override_capability_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "override-capability", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Override capability negotiation result\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_key_cmd_vtysh, 
       "ip ospf authentication-key AUTH_KEY", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Authentication password (key)\n"
       "The OSPF password (key)")

DEFSH (VTYSH_OSPFD, no_ospf_dead_interval_cmd_vtysh, 
       "no ospf dead-interval", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Interval after which a neighbor is declared dead\n")

DEFSH (VTYSH_RIPD, rip_offset_list_cmd_vtysh, 
       "offset-list WORD (in|out) <0-16>", 
       "Modify RIP metric\n"
       "Access-list name\n"
       "For incoming updates\n"
       "For outgoing updates\n"
       "Metric value\n")

DEFSH (VTYSH_ISISD, csnp_interval_l2_cmd_vtysh, 
       "isis csnp-interval <0-65535> level-2", 
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n"
       "Specify interval for level-2 CSNPs\n")

DEFSH (VTYSH_BGPD, debug_bgp_filter_cmd_vtysh, 
       "debug bgp filters", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP filters\n")

DEFSH (VTYSH_BGPD, neighbor_enforce_multihop_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "enforce-multihop", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Enforce EBGP neighbors perform multihop\n")

DEFSH (VTYSH_BGPD, no_neighbor_dont_capability_negotiate_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "dont-capability-negotiate", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Do not perform capability negotiation\n")

DEFSH (VTYSH_ISISD, no_area_passwd_cmd_vtysh, 
       "no area-password", 
       "Negate a command or set its defaults\n"
       "Configure the authentication password for an area\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community_exact_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_rsclient_summary_cmd_vtysh, 
      "show bgp ipv6 rsclient summary", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_BGPD, no_bgp_router_id_val_cmd_vtysh, 
       "no bgp router-id A.B.C.D", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Override configured router identifier\n"
       "Manually configured router identifier\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_soft_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_prefix_list_cmd_vtysh, 
       "show ip bgp flap-statistics prefix-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Display routes conforming to the prefix-list\n"
       "IP prefix-list name\n")

DEFSH (VTYSH_ISISD, metric_style_cmd_vtysh, 
       "metric-style (narrow|transition|wide)", 
       "Use old-style (ISO 10589) or new-style packet formats\n"
       "Use old style of TLVs with narrow metric\n"
       "Send and accept both styles of TLVs during transition\n"
       "Use new style of TLVs to carry wider metric\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community2_exact_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, show_bgp_view_neighbor_damp_cmd_vtysh, 
       "show bgp view WORD neighbors (A.B.C.D|X:X::X:X) dampened-routes", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the dampened routes received from neighbor\n")

DEFSH (VTYSH_ZEBRA, shutdown_if_cmd_vtysh, 
       "shutdown", 
       "Shutdown the selected interface\n")

DEFSH (VTYSH_BGPD, no_set_atomic_aggregate_cmd_vtysh, 
       "no set atomic-aggregate", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP atomic aggregate attribute\n" )

DEFSH (VTYSH_BGPD, no_set_aspath_exclude_cmd_vtysh, 
       "no set as-path exclude", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "Transform BGP AS_PATH attribute\n"
       "Exclude from the as-path\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_prefix_counts_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) prefix-counts", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display detailed prefix count information\n")

DEFSH (VTYSH_RIPD, show_ip_rip_cmd_vtysh, 
       "show ip rip", 
       "Show running system information\n"
       "IP information\n"
       "Show RIP routes\n")

DEFSH (VTYSH_BGPD, no_neighbor_default_originate_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "default-originate", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Originate default route to this neighbor\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_metric_type_routemap_cmd_vtysh, 
       "default-information originate always metric <0-16777214> metric-type (1|2) route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_dampening_address_cmd_vtysh, 
       "clear ip bgp dampening A.B.C.D", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear route flap dampening information\n"
       "Network to clear damping information\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_cmd_vtysh, 
       "no isis hello-multiplier", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_adv_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) adv-router A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_BGPD, show_bgp_community_list_cmd_vtysh, 
       "show bgp community-list (<1-500>|WORD)", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_as_soft_cmd_vtysh, 
       "clear bgp ipv6 " "<1-4294967295>" " soft", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n")

DEFSH (VTYSH_OSPFD, ospf_area_vlink_param2_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535> "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) <1-65535>", 
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPD, no_set_ip_nexthop_cmd_vtysh, 
       "no set ip next-hop", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IP information\n"
       "Next hop address\n")

DEFSH (VTYSH_RIPD, rip_distance_source_cmd_vtysh, 
       "distance <1-255> A.B.C.D/M", 
       "Administrative distance\n"
       "Distance value\n"
       "IP source prefix\n")

DEFSH (VTYSH_BGPD, neighbor_maximum_prefix_restart_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "maximum-prefix <1-4294967295> restart <1-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Maximum number of prefix accept from this peer\n"
       "maximum no. of prefix limit\n"
       "Restart bgp connection after limit is exceeded\n"
       "Restart interval in minutes")

DEFSH (VTYSH_ZEBRA, ip_route_cmd_vtysh, 
       "ip route A.B.C.D/M (A.B.C.D|INTERFACE|null0)", 
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n")

DEFSH (VTYSH_ALL, vty_ipv6_access_class_cmd_vtysh, 
       "ipv6 access-class WORD", 
       "IPv6 information\n"
       "Filter connections based on an IP access list\n"
       "IPv6 access list\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_adv_router_cmd_vtysh, 
       "show ipv6 ospf6 database adv-router A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_route_cmd_vtysh, 
       "no debug ospf6 route (table|intra-area|inter-area)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug route table calculation\n"
       "Debug intra-area route calculation\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_rsclient_route_cmd_vtysh, 
       "show ip bgp rsclient (A.B.C.D|X:X::X:X) A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_cmd_vtysh, 
       "clear ip bgp external", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n")

DEFSH (VTYSH_ALL, no_access_list_extended_any_any_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip any any", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Any source host\n"
       "Any destination host\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv6_rmap_metric_cmd_vtysh, 
       "redistribute (connected|kernel|ospf6|ripng|static) route-map WORD metric <0-4294967295>", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_seq_cmd_vtysh, 
       "ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) (X:X::X:X/M|any)", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Any prefix match.  Same as \"::0/0 le 128\"\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_route_map_cmd_vtysh, 
       "show ip bgp route-map WORD", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the route-map\n"
       "A route-map to match on\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_ism_sub_cmd_vtysh, 
       "no debug ospf ism (status|events|timers)", 
       "Negate a command or set its defaults\n"
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF Interface State Machine\n"
       "ISM Status Information\n"
       "ISM Event Information\n"
       "ISM Timer Information\n")

DEFSH (VTYSH_RIPD, ip_rip_authentication_mode_cmd_vtysh, 
       "ip rip authentication mode (md5|text)", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication mode\n"
       "Keyed message digest\n"
       "Clear text authentication\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_le_cmd_vtysh, 
       "ipv6 prefix-list WORD (deny|permit) X:X::X:X/M le <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_ISISD, no_csnp_interval_l2_arg_cmd_vtysh, 
       "no isis csnp-interval <0-65535> level-2", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set CSNP interval in seconds\n"
       "CSNP interval value\n"
       "Specify interval for level-2 CSNPs\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_debug_packet_cmd_vtysh, 
       "ip irdp debug packet", 
       "IP information\n"
       "ICMP Router discovery debug Averts. and Solicits (short)\n")

DEFSH (VTYSH_ZEBRA, ip_irdp_shutdown_cmd_vtysh, 
       "ip irdp shutdown", 
       "IP information\n"
       "ICMP Router discovery shutdown on this interface\n")

DEFSH (VTYSH_OSPFD, debug_ospf_nsm_cmd_vtysh, 
       "debug ospf nsm", 
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Neighbor State Machine\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_dampening_cmd_vtysh, 
       "clear ip bgp dampening", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear route flap dampening information\n")

DEFSH (VTYSH_BGPD, no_match_community_val_cmd_vtysh, 
       "no match community (<1-99>|<100-500>|WORD)", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP community list\n"
       "Community-list number (standard)\n"
       "Community-list number (expanded)\n"
       "Community-list name\n")

DEFSH (VTYSH_OSPFD, show_ip_ospf_neighbor_id_cmd_vtysh, 
       "show ip ospf neighbor A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "OSPF information\n"
       "Neighbor list\n"
       "Neighbor ID\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_seq_ge_le_cmd_vtysh, 
       "no ip prefix-list WORD seq <1-4294967295> (deny|permit) A.B.C.D/M ge <0-32> le <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n")

DEFSH (VTYSH_OSPFD, mpls_te_link_metric_cmd_vtysh, 
       "mpls-te link metric <0-4294967295>", 
       "MPLS-TE specific commands\n"
       "Configure MPLS-TE link parameters\n"
       "Link metric for MPLS-TE purpose\n"
       "Metric\n")

DEFSH (VTYSH_OSPF6D, area_range_cmd_vtysh, 
       "area A.B.C.D range X:X::X:X/M", 
       "OSPF area parameters\n"
       "Area ID (as an IPv4 notation)\n"
       "Configured address range\n"
       "Specify IPv6 prefix\n"
       )

DEFSH (VTYSH_BGPD, clear_bgp_external_soft_in_cmd_vtysh, 
       "clear bgp external soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, no_neighbor_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) ", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_external_inter_cmd_vtysh, 
       "distance ospf external <1-255> inter-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "External routes\n"
       "Distance for external routes\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n")

DEFSH (VTYSH_OSPF6D, area_export_list_cmd_vtysh, 
       "area A.B.C.D export-list NAME", 
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Set the filter for networks announced to other areas\n"
       "Name of the acess-list\n")

DEFSH (VTYSH_OSPFD, no_ospf_compatible_rfc1583_cmd_vtysh, 
       "no compatible rfc1583", 
       "Negate a command or set its defaults\n"
       "OSPF compatibility list\n"
       "compatible with RFC 1583\n")

DEFSH (VTYSH_BGPD, no_match_ecommunity_cmd_vtysh, 
       "no match extcommunity", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP/VPN extended community list\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPNGD, set_ipv6_nexthop_local_cmd_vtysh, 
       "set ipv6 next-hop local X:X::X:X", 
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 local address\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_OSPF6D, area_import_list_cmd_vtysh, 
       "area A.B.C.D import-list NAME", 
       "OSPFv6 area parameters\n"
       "OSPFv6 area ID in IP address format\n"
       "Set the filter for networks from other areas announced to the specified one\n"
       "Name of the acess-list\n")

DEFSH (VTYSH_BGPD, debug_bgp_keepalive_cmd_vtysh, 
       "debug bgp keepalives", 
       "Debugging functions (see also 'undebug')\n"
       "BGP information\n"
       "BGP keepalives\n")

DEFSH (VTYSH_ISISD, debug_isis_snp_cmd_vtysh, 
       "debug isis snp-packets", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS CSNP/PSNP packets\n")

DEFSH (VTYSH_BGPD, dump_bgp_routes_cmd_vtysh, 
       "dump bgp routes-mrt PATH", 
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump whole BGP routing table\n"
       "Output filename\n")

DEFSH (VTYSH_BGPD, no_match_aspath_val_cmd_vtysh, 
       "no match as-path WORD", 
       "Negate a command or set its defaults\n"
       "Match values from routing table\n"
       "Match BGP AS path list\n"
       "AS path access-list name\n")

DEFSH (VTYSH_BGPD, no_synchronization_cmd_vtysh, 
       "no synchronization", 
       "Negate a command or set its defaults\n"
       "Perform IGP synchronization\n")

DEFSH (VTYSH_BGPD, neighbor_capability_dynamic_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "capability dynamic", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Advertise capability to the peer\n"
       "Advertise dynamic capability to this neighbor\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_soft_in_cmd_vtysh, 
       "clear ip bgp peer-group WORD soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ZEBRA, ip_protocol_cmd_vtysh, 
       "ip protocol PROTO route-map ROUTE-MAP", 
       "Negate a command or set its defaults\n"
       "Apply route map to PROTO\n"
       "Protocol name\n"
       "Route map name\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_all_soft_cmd_vtysh, 
       "clear bgp view WORD * soft", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n"
       "Soft reconfig\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_group_cmd_vtysh, 
       "clear ip bgp peer-group WORD", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_mask_summary_as_set_cmd_vtysh, 
       "no aggregate-address A.B.C.D A.B.C.D summary-only as-set", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Filter more specific routes from updates\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_RIPD, no_rip_redistribute_type_routemap_cmd_vtysh, 
       "no redistribute " "(kernel|connected|static|ospf|isis|bgp)" " route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_metric_cmd_vtysh, 
       "default-information originate always metric <0-16777214>", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "OSPF metric type for default routes\n")

DEFSH (VTYSH_BGPD, no_set_originator_id_val_cmd_vtysh, 
       "no set originator-id A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP originator ID attribute\n"
       "IP address of originator\n")

DEFSH (VTYSH_BGPD, show_bgp_rsclient_route_cmd_vtysh, 
       "show bgp rsclient (A.B.C.D|X:X::X:X) X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_cmd_vtysh, 
       "ipv6 prefix-list WORD (deny|permit) (X:X::X:X/M|any)", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Any prefix match.  Same as \"::0/0 le 128\"\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, ip_prefix_list_ge_cmd_vtysh, 
       "ip prefix-list WORD (deny|permit) A.B.C.D/M ge <0-32>", 
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_RIPD, no_debug_rip_zebra_cmd_vtysh, 
       "no debug rip zebra", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP and ZEBRA communication\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_cidr_only_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) cidr-only", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display only routes with non-natural netmasks\n")

DEFSH (VTYSH_ZEBRA, no_ip_irdp_address_preference_cmd_vtysh, 
       "no ip irdp address A.B.C.D preference <0-2147483647>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Alter ICMP Router discovery preference this interface\n"
       "Removes IRDP non-default preference\n"
       "Select IRDP address\n"
       "Old preference level\n")

DEFSH (VTYSH_BGPD, set_local_pref_cmd_vtysh, 
       "set local-preference <0-4294967295>", 
       "Set values in destination routing protocol\n"
       "BGP local preference path attribute\n"
       "Preference value\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_med_cmd_vtysh, 
       "bgp bestpath med (confed|missing-as-worst)", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "MED attribute\n"
       "Compare MED among confederation paths\n"
       "Treat missing MED as the least preferred one\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_addr_cmd_vtysh, 
       "ip ospf authentication A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n"
       "Address of interface")

DEFSH (VTYSH_RIPNGD, no_debug_ripng_packet_direct_cmd_vtysh, 
       "no debug ripng packet (recv|send)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n")

DEFSH (VTYSH_BGPD, no_neighbor_local_as_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "local-as", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a local-as number\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged7_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged next-hop med as-path", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "Nexthop attribute\n"
       "Med attribute\n"
       "As-path attribute\n")

DEFSH (VTYSH_ISISD, debug_isis_spfstats_cmd_vtysh, 
       "debug isis spf-statistics ", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS SPF Timing and Statistic Data\n")

DEFSH (VTYSH_BGPD, show_bgp_view_route_cmd_vtysh, 
       "show bgp view WORD X:X::X:X", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_RIPD, debug_rip_packet_cmd_vtysh, 
       "debug rip packet", 
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP packet\n")

DEFSH (VTYSH_ALL, no_access_list_extended_mask_host_cmd_vtysh, 
       "no access-list (<100-199>|<2000-2699>) (deny|permit) ip A.B.C.D A.B.C.D host A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP extended access list\n"
       "IP extended access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any Internet Protocol\n"
       "Source address\n"
       "Source wildcard bits\n"
       "A single destination host\n"
       "Destination address\n")

DEFSH (VTYSH_RIPNGD, no_if_ipv6_rmap_cmd_vtysh, 
       "no route-map ROUTEMAP_NAME (in|out) IFNAME", 
       "Negate a command or set its defaults\n"
       "Route map unset\n"
       "Route map name\n"
       "Route map for input filtering\n"
       "Route map for output filtering\n"
       "Route map interface name\n")

DEFSH (VTYSH_OSPFD, ip_ospf_authentication_args_cmd_vtysh, 
       "ip ospf authentication (null|message-digest)", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Enable authentication on this interface\n"
       "Use null authentication\n"
       "Use message-digest authentication\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_route_cmd_vtysh, 
       "show ipv6 mbgp X:X::X:X", 
       "Show running system information\n"
       "IP information\n"
       "MBGP information\n"
       "Network in the MBGP routing table to display\n")

DEFSH (VTYSH_RIPNGD, no_router_ripng_cmd_vtysh, 
       "no router ripng", 
       "Negate a command or set its defaults\n"
       "Enable a routing process\n"
       "Make RIPng instance command\n")

DEFSH (VTYSH_ISISD, debug_isis_spftrigg_cmd_vtysh, 
       "debug isis spf-triggers", 
       "Debugging functions (see also 'undebug')\n"
       "IS-IS information\n"
       "IS-IS SPF triggering events\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_vpnv4_in_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " vpnv4 unicast in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ALL, ipv6_access_list_cmd_vtysh, 
       "ipv6 access-list WORD (deny|permit) X:X::X:X/M", 
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Prefix to match. e.g. 3ffe:506::/32\n")

DEFSH (VTYSH_ISISD, no_isis_circuit_type_cmd_vtysh, 
       "no isis circuit-type (level-1|level-1-2|level-2-only)", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Configure circuit type for interface\n"
       "Level-1 only adjacencies are formed\n"
       "Level-1-2 adjacencies are formed\n"
       "Level-2 only adjacencies are formed\n")

DEFSH (VTYSH_BGPD, show_bgp_community4_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_neighbor_detail_cmd_vtysh, 
       "show ipv6 ospf6 neighbor (detail|drchoice)", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Neighbor list\n"
       "Display details\n"
       "Display DR choices\n"
      )

DEFSH (VTYSH_RIPNGD, debug_ripng_packet_direct_cmd_vtysh, 
       "debug ripng packet (recv|send)", 
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng packet\n"
       "Debug option set for receive packet\n"
       "Debug option set for send packet\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_nssa_cmd_vtysh, 
       "no debug ospf nssa", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF nssa information\n")

DEFSH (VTYSH_BGPD, show_bgp_rsclient_summary_cmd_vtysh, 
       "show bgp rsclient summary", 
       "Show running system information\n"
       "BGP information\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_RIPD, no_rip_timers_val_cmd_vtysh, 
       "no timers basic <0-65535> <0-65535> <0-65535>", 
       "Negate a command or set its defaults\n"
       "Adjust routing timers\n"
       "Basic routing protocol update timers\n"
       "Routing table update timer value in second. Default is 30.\n"
       "Routing information timeout timer. Default is 180.\n"
       "Garbage collection timer. Default is 120.\n")

DEFSH (VTYSH_ISISD, show_isis_topology_l2_cmd_vtysh, 
       "show isis topology level-2", 
       "Show running system information\n"
       "IS-IS information\n"
       "IS-IS paths to Intermediate Systems\n"
       "Paths to all level-2 routers in the domain\n")

DEFSH (VTYSH_BGPD, neighbor_allowas_in_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "allowas-in", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Accept as-path with my AS present in it\n")

DEFSH (VTYSH_ISISD, no_ipv6_router_isis_cmd_vtysh, 
       "no ipv6 router isis WORD", 
       "Negate a command or set its defaults\n"
       "IPv6 interface subcommands\n"
       "IPv6 Router interface commands\n"
       "IS-IS Routing for IPv6\n"
       "Routing process tag\n")

DEFSH (VTYSH_BGPD, no_auto_summary_cmd_vtysh, 
       "no auto-summary", 
       "Negate a command or set its defaults\n"
       "Enable automatic network number summarization\n")

DEFSH (VTYSH_BGPD, no_neighbor_capability_dynamic_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "capability dynamic", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Advertise capability to the peer\n"
       "Advertise dynamic capability to this neighbor\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_l1_arg_cmd_vtysh, 
       "no isis hello-interval (<1-65535>|minimal) level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Hello interval value\n"
       "Holdtime 1 second,  interval depends on multiplier\n"
       "Specify hello-interval for level-1 IIHs\n")

DEFSH (VTYSH_OSPFD, ospf_neighbor_cmd_vtysh, 
       "neighbor A.B.C.D", 
       "Specify neighbor router\n"
       "Neighbor IP address\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_network_cmd_vtysh, 
       "no ip ospf network", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Network type\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_aspath_ignore_cmd_vtysh, 
       "bgp bestpath as-path ignore", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "AS-path attribute\n"
       "Ignore as-path length in selecting a route\n")

DEFSH (VTYSH_BGPD, show_ip_extcommunity_list_arg_cmd_vtysh, 
       "show ip extcommunity-list (<1-500>|WORD)", 
       "Show running system information\n"
       "IP information\n"
       "List extended-community list\n"
       "Extcommunity-list number\n"
       "Extcommunity-list name\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_vlink_param3_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) virtual-link A.B.C.D "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval) "
       "(hello-interval|retransmit-interval|transmit-delay|dead-interval)", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n" "OSPF area ID in IP address format\n" "OSPF area ID as a decimal value\n" "Configure a virtual link\n" "Router ID of the remote ABR\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n"
       "Time between HELLO packets\n" "Time between retransmitting lost link state advertisements\n" "Link state transmit delay\n" "Interval after which a neighbor is declared dead\n" "Seconds\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_vpnv4_all_neighbors_peer_cmd_vtysh, 
       "show ip bgp vpnv4 all neighbors A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display VPNv4 NLRI specific information\n"
       "Display information about all VPNv4 NLRIs\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n")

DEFSH (VTYSH_BGPD, ip_community_list_name_expanded_cmd_vtysh, 
       "ip community-list expanded WORD (deny|permit) .LINE", 
       "IP information\n"
       "Add a community list entry\n"
       "Add an expanded community-list entry\n"
       "Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "An ordered list as a regular-expression\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_vpnv4_out_cmd_vtysh, 
       "clear ip bgp * vpnv4 unicast out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_out_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_RIPD, ip_rip_receive_version_2_cmd_vtysh, 
       "ip rip receive version 2 1", 
       "IP information\n"
       "Routing Information Protocol\n"
       "Advertisement reception\n"
       "Version control\n"
       "RIP version 2\n"
       "RIP version 1\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_view_rsclient_route_cmd_vtysh, 
       "show ip bgp view WORD rsclient (A.B.C.D|X:X::X:X) A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "BGP view name\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n"
       "Network in the BGP routing table to display\n")

DEFSH (VTYSH_ALL, no_access_list_remark_arg_cmd_vtysh, 
       "no access-list (<1-99>|<100-199>|<1300-1999>|<2000-2699>|WORD) remark .LINE", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP extended access list\n"
       "IP standard access list (expanded range)\n"
       "IP extended access list (expanded range)\n"
       "IP zebra access-list\n"
       "Access list entry comment\n"
       "Comment up to 100 characters\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_ISISD, no_lsp_lifetime_arg_cmd_vtysh, 
       "no lsp-lifetime <380-65535>", 
       "Negate a command or set its defaults\n"
       "Maximum LSP lifetime\n"
       "LSP lifetime in seconds\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_prefix_list_cmd_vtysh, 
       "show bgp ipv6 prefix-list WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Display routes conforming to the prefix-list\n"
       "IPv6 prefix-list name\n")

DEFSH (VTYSH_ISISD, no_spf_interval_l1_arg_cmd_vtysh, 
       "no spf-interval level-1 <1-120>", 
       "Negate a command or set its defaults\n"
       "Minimum interval between SPF calculations\n"
       "Set interval for level 1 only\n"
       "Minimum interval between consecutive SPFs in seconds\n")

DEFSH (VTYSH_RIPD, no_debug_rip_packet_direct_cmd_vtysh, 
       "no debug rip packet (recv|send)", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP packet\n"
       "RIP option set for receive packet\n"
       "RIP option set for send packet\n")

DEFSH (VTYSH_BGPD, no_neighbor_disable_connected_check_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "disable-connected-check", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "one-hop away EBGP peer using loopback address\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD, set_tag_cmd_vtysh, 
       "set tag <0-65535>", 
       "Set values in destination routing protocol\n"
       "Tag value for routing protocol\n"
       "Tag value\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_le_ge_cmd_vtysh, 
       "no ip prefix-list WORD (deny|permit) A.B.C.D/M le <0-32> ge <0-32>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_OSPFD, no_ospf_default_metric_cmd_vtysh, 
       "no default-metric", 
       "Negate a command or set its defaults\n"
       "Set metric of redistributed routes\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_community2_exact_cmd_vtysh, 
       "show ip bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, no_set_community_none_cmd_vtysh, 
       "no set community none", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP community attribute\n"
       "No community attribute\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_flap_regexp_cmd_vtysh, 
       "show ip bgp flap-statistics regexp .LINE", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Display flap statistics of routes\n"
       "Display routes matching the AS path regular expression\n"
       "A regular-expression to match the BGP AS paths\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_brouter_area_cmd_vtysh, 
       "no debug ospf6 border-routers area-id", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
       "Debug border routers in specific Area\n"
      )

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv4_rmap_metric_cmd_vtysh, 
       "redistribute (connected|kernel|ospf|rip|static) route-map WORD metric <0-4294967295>", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Route map reference\n"
       "Pointer to route-map entries\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, bgp_bestpath_aspath_confed_cmd_vtysh, 
       "bgp bestpath as-path confed", 
       "BGP specific commands\n"
       "Change the default bestpath selection\n"
       "AS-path attribute\n"
       "Compare path lengths including confederation sets & sequences in selecting a route\n")

DEFSH (VTYSH_BGPD, no_set_aggregator_as_val_cmd_vtysh, 
       "no set aggregator as " "<1-4294967295>" " A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP aggregator attribute\n"
       "AS number of aggregator\n"
       "AS number\n"
       "IP address of aggregator\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_interface_prefix_cmd_vtysh, 
       "show ipv6 ospf6 interface prefix", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Interface infomation\n"
       "Display connected prefixes to advertise\n"
       )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_out_cmd_vtysh, 
       "clear bgp ipv6 (A.B.C.D|X:X::X:X) out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "BGP neighbor address to clear\n"
       "BGP IPv6 neighbor to clear\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_inter_external_cmd_vtysh, 
       "distance ospf inter-area <1-255> external <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "External routes\n"
       "Distance for external routes\n")

DEFSH (VTYSH_OSPFD, no_ospf_priority_cmd_vtysh, 
       "no ospf priority", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Router priority\n")

DEFSH (VTYSH_BGPD, set_origin_cmd_vtysh, 
       "set origin (egp|igp|incomplete)", 
       "Set values in destination routing protocol\n"
       "BGP origin code\n"
       "remote EGP\n"
       "local IGP\n"
       "unknown heritage\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_nsm_sub_cmd_vtysh, 
       "no debug ospf nsm (status|events|timers)", 
       "Negate a command or set its defaults\n"
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF Interface State Machine\n"
       "NSM Status Information\n"
       "NSM Event Information\n"
       "NSM Timer Information\n")

DEFSH (VTYSH_BGPD, neighbor_disable_connected_check_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "disable-connected-check", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "one-hop away EBGP peer using loopback address\n")

DEFSH (VTYSH_OSPFD, ospf_redistribute_source_cmd_vtysh, 
       "redistribute " "(kernel|connected|static|rip|isis|bgp)", 
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Routing Information Protocol (RIP)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n")

DEFSH (VTYSH_RIPD, debug_rip_events_cmd_vtysh, 
       "debug rip events", 
       "Debugging functions (see also 'undebug')\n"
       "RIP information\n"
       "RIP events\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_mask_cmd_vtysh, 
       "no ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE|null0)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_cmd_vtysh, 
       "clear ip bgp *", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n")

DEFSH (VTYSH_OSPFD, show_mpls_te_link_cmd_vtysh, 
       "show mpls-te interface [INTERFACE]", 
       "Show running system information\n"
       "MPLS-TE information\n"
       "Interface information\n"
       "Interface name\n")

DEFSH (VTYSH_OSPFD, ip_ospf_priority_addr_cmd_vtysh, 
       "ip ospf priority <0-255> A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Router priority\n"
       "Priority\n"
       "Address of interface")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_always_type_metric_cmd_vtysh, 
       "default-information originate always metric-type (1|2) metric <0-16777214>", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "Always advertise default route\n"
       "OSPF metric type for default routes\n"
       "Set OSPF External Type 1 metrics\n"
       "Set OSPF External Type 2 metrics\n"
       "OSPF default metric\n"
       "OSPF metric\n")

DEFSH (VTYSH_BGPD, show_ip_as_path_access_list_cmd_vtysh, 
       "show ip as-path-access-list WORD", 
       "Show running system information\n"
       "IP information\n"
       "List AS path access lists\n"
       "AS path access list name\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_standard_all_cmd_vtysh, 
       "no ip extcommunity-list <1-99>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Extended Community list number (standard)\n")

DEFSH (VTYSH_ISISD, no_isis_hello_interval_l1_cmd_vtysh, 
       "no isis hello-interval level-1", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set Hello interval\n"
       "Specify hello-interval for level-1 IIHs\n")

DEFSH (VTYSH_BGPD, neighbor_route_map_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "route-map WORD (in|out|import|export)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Apply route map to neighbor\n"
       "Name of route map\n"
       "Apply map to incoming routes\n"
       "Apply map to outbound routes\n"
       "Apply map to routes going into a Route-Server client's table\n"
       "Apply map to routes coming from a Route-Server client")

DEFSH (VTYSH_RIPNGD, debug_ripng_events_cmd_vtysh, 
       "debug ripng events", 
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n"
       "Debug option set for ripng events\n")

DEFSH (VTYSH_ISISD, no_net_cmd_vtysh, 
       "no net WORD", 
       "Negate a command or set its defaults\n"
       "A Network Entity Title for this process (OSI only)\n"
       "XX.XXXX. ... .XXX.XX  Network entity title (NET)\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, clear_ip_prefix_list_name_cmd_vtysh, 
       "clear ip prefix-list WORD", 
       "Reset functions\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_zebra_cmd_vtysh, 
       "no debug ospf zebra", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF Zebra information\n")

DEFSH (VTYSH_OSPFD, ospf_distance_ospf_inter_intra_cmd_vtysh, 
       "distance ospf inter-area <1-255> intra-area <1-255>", 
       "Define an administrative distance\n"
       "OSPF Administrative distance\n"
       "Inter-area routes\n"
       "Distance for inter-area routes\n"
       "Intra-area routes\n"
       "Distance for intra-area routes\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_flags2_cmd_vtysh, 
       "no ip route A.B.C.D/M (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_external_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp external ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all external peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community_exact_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_router_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) * A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Any Link state ID\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_adv_router_linkstate_id_detail_cmd_vtysh, 
       "show ipv6 ospf6 database adv-router A.B.C.D linkstate-id A.B.C.D "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Search by Advertising Router\n"
       "Specify Advertising Router as IPv4 address notation\n"
       "Search by Link state ID\n"
       "Specify Link state ID as IPv4 address notation\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_ZEBRA, ip_address_label_cmd_vtysh, 
       "ip address A.B.C.D/M label LINE", 
       "Interface Internet Protocol config commands\n"
       "Set the IP address of an interface\n"
       "IP address (e.g. 10.0.0.1/8)\n"
       "Label of this address\n"
       "Label\n")

DEFSH (VTYSH_BGPD, bgp_damp_unset_cmd_vtysh, 
       "no bgp dampening", 
       "Negate a command or set its defaults\n"
       "BGP Specific commands\n"
       "Enable route-flap dampening\n")

DEFSH (VTYSH_ALL, no_access_list_standard_cmd_vtysh, 
       "no access-list (<1-99>|<1300-1999>) (deny|permit) A.B.C.D A.B.C.D", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Address to match\n"
       "Wildcard bits\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_flags_distance2_cmd_vtysh, 
       "no ip route A.B.C.D/M (reject|blackhole) <1-255>", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n"
       "Distance value for this route\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_received_prefix_filter_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) received prefix-filter", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display information received from a BGP neighbor\n"
       "Display the prefixlist filter\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_interface_cmd_vtysh, 
       "no debug ospf6 interface", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug OSPFv3 Interface\n"
      )

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_peer_group_in_cmd_vtysh, 
       "clear bgp ipv6 peer-group WORD in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_all_in_cmd_vtysh, 
       "clear bgp * in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, show_ipv6_mbgp_community3_cmd_vtysh, 
       "show ipv6 mbgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, bgp_damp_set_cmd_vtysh, 
       "bgp dampening <1-45> <1-20000> <1-20000> <1-255>", 
       "BGP Specific commands\n"
       "Enable route-flap dampening\n"
       "Half-life time for the penalty\n"
       "Value to start reusing a route\n"
       "Value to start suppressing a route\n"
       "Maximum duration to suppress a stable route\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_instance_cmd_vtysh, 
       "ipv6 ospf6 instance-id <0-255>", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Instance ID for this interface\n"
       "Instance ID value\n"
       )

DEFSH (VTYSH_BGPD, no_dump_bgp_routes_cmd_vtysh, 
       "no dump bgp routes-mrt [PATH] [INTERVAL]", 
       "Negate a command or set its defaults\n"
       "Dump packet\n"
       "BGP packet dump\n"
       "Dump whole BGP routing table\n")

DEFSH (VTYSH_RIPNGD, no_ripng_route_cmd_vtysh, 
       "no route IPV6ADDR", 
       "Negate a command or set its defaults\n"
       "Static route setup\n"
       "Delete static RIPng route announcement\n")

DEFSH (VTYSH_BGPD, no_aggregate_address_mask_as_set_cmd_vtysh, 
       "no aggregate-address A.B.C.D A.B.C.D as-set", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_OSPF6D, no_debug_ospf6_brouter_cmd_vtysh, 
       "no debug ospf6 border-routers", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Debug border router\n"
      )

DEFSH (VTYSH_BGPD, no_bgp_confederation_peers_cmd_vtysh, 
       "no bgp confederation peers ." "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "AS confederation parameters\n"
       "Peer ASs in BGP confederation\n"
       "AS number\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community4_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_BGPD, no_set_weight_val_cmd_vtysh, 
       "no set weight <0-4294967295>", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "BGP weight for routing table\n"
       "Weight value\n")

DEFSH (VTYSH_BGPD, bgp_graceful_restart_cmd_vtysh, 
       "bgp graceful-restart", 
       "BGP specific commands\n"
       "Graceful restart capability parameters\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community_list_exact_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community-list (<1-500>|WORD) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the community-list\n"
       "community-list number\n"
       "community-list name\n"
       "Exact match of the communities\n")

DEFSH (VTYSH_BGPD, bgp_scan_time_cmd_vtysh, 
       "bgp scan-time <5-60>", 
       "BGP specific commands\n"
       "Configure background scanner interval\n"
       "Scanner interval (seconds)\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_neighbor_prefix_counts_cmd_vtysh, 
       "show ip bgp neighbors (A.B.C.D|X:X::X:X) prefix-counts", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display detailed prefix count information\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community3_exact_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_BGPD, clear_ip_bgp_peer_vpnv4_in_cmd_vtysh, 
       "clear ip bgp A.B.C.D vpnv4 unicast in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "BGP neighbor address to clear\n"
       "Address family\n"
       "Address Family Modifier\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_authentication_key_addr_cmd_vtysh, 
       "no ip ospf authentication-key A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Authentication password (key)\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, ipv6_aggregate_address_summary_only_cmd_vtysh, 
       "aggregate-address X:X::X:X/M summary-only", 
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_BGPD, ipv6_mbgp_neighbor_routes_cmd_vtysh, 
       "show ipv6 mbgp neighbors (A.B.C.D|X:X::X:X) routes", 
       "Show running system information\n"
       "IPv6 information\n"
       "MBGP information\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display routes learned from neighbor\n")

DEFSH (VTYSH_RIPD, accept_lifetime_infinite_month_day_cmd_vtysh, 
       "accept-lifetime HH:MM:SS MONTH <1-31> <1993-2035> infinite", 
       "Set accept lifetime of the key\n"
       "Time to start\n"
       "Month of the year to start\n"
       "Day of th month to start\n"
       "Year to start\n"
       "Never expires")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_seq_le_ge_cmd_vtysh, 
       "ipv6 prefix-list WORD seq <1-4294967295> (deny|permit) X:X::X:X/M le <0-128> ge <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_external_soft_out_cmd_vtysh, 
       "clear bgp ipv6 external soft out", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all external peers\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_as_ipv4_in_prefix_filter_cmd_vtysh, 
       "clear ip bgp " "<1-4294967295>" " ipv4 (unicast|multicast) in prefix-filter", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, show_bgp_instance_ipv6_neighbors_cmd_vtysh, 
       "show bgp view WORD ipv6 neighbors", 
       "Show running system information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged5_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path next-hop med", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_neighbors_cmd_vtysh, 
       "show ip bgp view WORD neighbors", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Detailed information on TCP and BGP neighbor connections\n")

DEFSH (VTYSH_OSPF6D, ipv6_ospf6_passive_cmd_vtysh, 
       "ipv6 ospf6 passive", 
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "passive interface,  No adjacency will be formed on this interface\n"
       )

DEFSH (VTYSH_OSPF6D, no_ipv6_ospf6_passive_cmd_vtysh, 
       "no ipv6 ospf6 passive", 
       "Negate a command or set its defaults\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "passive interface: No Adjacency will be formed on this I/F\n"
       )

DEFSH (VTYSH_OSPFD, no_ip_ospf_authentication_key_cmd_vtysh, 
       "no ip ospf authentication-key", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Authentication password (key)\n")

DEFSH (VTYSH_RIPNGD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, ipv6_prefix_list_le_ge_cmd_vtysh, 
       "ipv6 prefix-list WORD (deny|permit) X:X::X:X/M le <0-128> ge <0-128>", 
       "IPv6 information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IPv6 prefix <network>/<length>,  e.g.,  3ffe::/16\n"
       "Maximum prefix length to be matched\n"
       "Maximum prefix length\n"
       "Minimum prefix length to be matched\n"
       "Minimum prefix length\n")

DEFSH (VTYSH_OSPF6D, ospf6_redistribute_cmd_vtysh, 
       "redistribute (static|kernel|connected|ripng|bgp)", 
       "Redistribute\n"
       "Static route\n"
       "Kernel route\n"
       "Connected route\n"
       "RIPng route\n"
       "BGP route\n"
      )

DEFSH (VTYSH_BGPD, bgp_cluster_id_cmd_vtysh, 
       "bgp cluster-id A.B.C.D", 
       "BGP information\n"
       "Configure Route-Reflector Cluster-id\n"
       "Route-Reflector Cluster-id in IP address format\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_ipv4_soft_out_cmd_vtysh, 
       "clear ip bgp * ipv4 (unicast|multicast) soft out", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Soft reconfig\n"
       "Soft reconfig outbound update\n")

DEFSH (VTYSH_ALL, no_access_list_standard_any_cmd_vtysh, 
       "no access-list (<1-99>|<1300-1999>) (deny|permit) any", 
       "Negate a command or set its defaults\n"
       "Add an access list entry\n"
       "IP standard access list\n"
       "IP standard access list (expanded range)\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any source host\n")

DEFSH (VTYSH_BGPD, set_ecommunity_soo_cmd_vtysh, 
       "set extcommunity soo .ASN:nn_or_IP-address:nn", 
       "Set values in destination routing protocol\n"
       "BGP extended community attribute\n"
       "Site-of-Origin extended community\n"
       "VPN extended community\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv4_metric_cmd_vtysh, 
       "redistribute (connected|kernel|ospf|rip|static) metric <0-4294967295>", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n")

DEFSH (VTYSH_BGPD, clear_bgp_instance_all_cmd_vtysh, 
       "clear bgp view WORD *", 
       "Reset functions\n"
       "BGP information\n"
       "BGP view\n"
       "view name\n"
       "Clear all peers\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_prefix_longer_cmd_vtysh, 
       "show bgp ipv6 X:X::X:X/M longer-prefixes", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "IPv6 prefix <network>/<length>\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_BGPD, clear_bgp_as_soft_in_cmd_vtysh, 
       "clear bgp " "<1-4294967295>" " soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear peers with the AS number\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ALL, ipv6_access_list_any_cmd_vtysh, 
       "ipv6 access-list WORD (deny|permit) any", 
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "Any prefixi to match\n")

DEFSH (VTYSH_OSPFD, no_ospf_retransmit_interval_cmd_vtysh, 
       "no ospf retransmit-interval", 
       "Negate a command or set its defaults\n"
       "OSPF interface commands\n"
       "Time between retransmitting lost link state advertisements\n")

DEFSH (VTYSH_ZEBRA, no_ip_route_mask_flags_cmd_vtysh, 
       "no ip route A.B.C.D A.B.C.D (A.B.C.D|INTERFACE) (reject|blackhole)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix\n"
       "IP destination prefix mask\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Emit an ICMP unreachable when matched\n"
       "Silently discard pkts when matched\n")

DEFSH (VTYSH_BGPD, no_neighbor_timers_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "timers", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP per neighbor timers\n")

DEFSH (VTYSH_BGPD, bgp_cluster_id32_cmd_vtysh, 
       "bgp cluster-id <1-4294967295>", 
       "BGP information\n"
       "Configure Route-Reflector Cluster-id\n"
       "Route-Reflector Cluster-id as 32 bit quantity\n")

DEFSH (VTYSH_BGPD, old_no_ipv6_aggregate_address_summary_only_cmd_vtysh, 
       "no ipv6 bgp aggregate-address X:X::X:X/M summary-only", 
       "Negate a command or set its defaults\n"
       "IPv6 information\n"
       "BGP information\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_OSPFD, no_ospf_timers_throttle_spf_cmd_vtysh, 
       "no timers throttle spf", 
       "Negate a command or set its defaults\n"
       "Adjust routing timers\n"
       "Throttling adaptive timer\n"
       "OSPF SPF timers\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, no_rmap_call_cmd_vtysh, 
       "no call", 
       "Negate a command or set its defaults\n"
       "Jump to another Route-Map after match+set\n")

DEFSH (VTYSH_BGPD, set_ipv6_nexthop_global_cmd_vtysh, 
       "set ipv6 next-hop global X:X::X:X", 
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 global address\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_BGPD|VTYSH_RIPNGD, no_set_ipv6_nexthop_local_val_cmd_vtysh, 
       "no set ipv6 next-hop local X:X::X:X", 
       "Negate a command or set its defaults\n"
       "Set values in destination routing protocol\n"
       "IPv6 information\n"
       "IPv6 next-hop address\n"
       "IPv6 local address\n"
       "IPv6 address of next hop\n")

DEFSH (VTYSH_OSPFD, no_ip_ospf_message_digest_key_addr_cmd_vtysh, 
       "no ip ospf message-digest-key <1-255> A.B.C.D", 
       "Negate a command or set its defaults\n"
       "IP Information\n"
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n"
       "Address of interface")

DEFSH (VTYSH_BGPD, aggregate_address_mask_summary_as_set_cmd_vtysh, 
       "aggregate-address A.B.C.D A.B.C.D summary-only as-set", 
       "Configure BGP aggregate entries\n"
       "Aggregate address\n"
       "Aggregate mask\n"
       "Filter more specific routes from updates\n"
       "Generate AS set path information\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv6_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf6|ripng|static)", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPFv3)\n"
       "Routing Information Protocol (RIPng)\n"
       "Static routes\n")

DEFSH (VTYSH_OSPFD, ip_ospf_cost_u32_cmd_vtysh, 
       "ip ospf cost <1-65535>", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Interface cost\n"
       "Cost")

DEFSH (VTYSH_ZEBRA, no_ip_route_cmd_vtysh, 
       "no ip route A.B.C.D/M (A.B.C.D|INTERFACE|null0)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Establish static routes\n"
       "IP destination prefix (e.g. 10.0.0.0/8)\n"
       "IP gateway address\n"
       "IP gateway interface name\n"
       "Null interface\n")

DEFSH (VTYSH_BGPD, neighbor_attr_unchanged2_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "attribute-unchanged as-path (next-hop|med)", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "BGP attribute is propagated unchanged to this neighbor\n"
       "As-path attribute\n"
       "Nexthop attribute\n"
       "Med attribute\n")

DEFSH (VTYSH_OSPFD, ospf_area_stub_no_summary_cmd_vtysh, 
       "area (A.B.C.D|<0-4294967295>) stub no-summary", 
       "OSPF stub parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Configure OSPF area as stub\n"
       "Do not inject inter-area routes into stub\n")

DEFSH (VTYSH_BGPD, no_ipv6_aggregate_address_summary_only_cmd_vtysh, 
       "no aggregate-address X:X::X:X/M summary-only", 
       "Negate a command or set its defaults\n"
       "Configure BGP aggregate entries\n"
       "Aggregate prefix\n"
       "Filter more specific routes from updates\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_prefix_longer_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) A.B.C.D/M longer-prefixes", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Display route and more specific routes\n")

DEFSH (VTYSH_BGPD, no_bgp_deterministic_med_cmd_vtysh, 
       "no bgp deterministic-med", 
       "Negate a command or set its defaults\n"
       "BGP specific commands\n"
       "Pick the best-MED path among paths advertised from the neighboring AS\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_ipv4_community2_cmd_vtysh, 
       "show ip bgp ipv4 (unicast|multicast) community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export)", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "Address family\n"
       "Address Family modifier\n"
       "Address Family modifier\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n")

DEFSH (VTYSH_RIPD|VTYSH_RIPNGD|VTYSH_OSPFD|VTYSH_OSPF6D|VTYSH_BGPD|VTYSH_ZEBRA, rmap_description_cmd_vtysh, 
       "description .LINE", 
       "Route-map comment\n"
       "Comment describing this route-map rule\n")

DEFSH (VTYSH_BGPD, show_ipv6_bgp_community2_exact_cmd_vtysh, 
       "show ipv6 bgp community (AA:NN|local-AS|no-advertise|no-export) (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "IPv6 information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_OSPFD, debug_ospf_packet_send_recv_cmd_vtysh, 
       "debug ospf packet (hello|dd|ls-request|ls-update|ls-ack|all) (send|recv|detail)", 
       "Debugging functions\n"
       "OSPF information\n"
       "OSPF packets\n"
       "OSPF Hello\n"
       "OSPF Database Description\n"
       "OSPF Link State Request\n"
       "OSPF Link State Update\n"
       "OSPF Link State Acknowledgment\n"
       "OSPF all packets\n"
       "Packet sent\n"
       "Packet received\n"
       "Detail information\n")

DEFSH (VTYSH_ALL, ipv6_access_list_remark_cmd_vtysh, 
       "ipv6 access-list WORD remark .LINE", 
       "IPv6 information\n"
       "Add an access list entry\n"
       "IPv6 zebra access-list\n"
       "Access list entry comment\n"
       "Comment up to 100 characters\n")

DEFSH (VTYSH_BGPD, no_bgp_redistribute_ipv4_metric_rmap_cmd_vtysh, 
       "no redistribute (connected|kernel|ospf|rip|static) metric <0-4294967295> route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ZEBRA, show_table_cmd_vtysh, 
       "show table", 
       "Show running system information\n"
       "default routing table to use for all clients\n")

DEFSH (VTYSH_ISISD, no_isis_metric_arg_cmd_vtysh, 
       "no isis metric <0-16777215>", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set default metric for circuit\n"
       "Default metric value\n")

DEFSH (VTYSH_OSPFD, no_debug_ospf_event_cmd_vtysh, 
       "no debug ospf event", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "OSPF information\n"
       "OSPF event information\n")

DEFSH (VTYSH_BGPD, show_bgp_community_exact_cmd_vtysh, 
       "show bgp community (AA:NN|local-AS|no-advertise|no-export) exact-match", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the communities\n"
       "community number\n"
       "Do not send outside local AS (well-known community)\n"
       "Do not advertise to any peer (well-known community)\n"
       "Do not export to next AS (well-known community)\n"
       "Exact match of the communities")

DEFSH (VTYSH_ZEBRA, show_ip_forwarding_cmd_vtysh, 
       "show ip forwarding", 
       "Show running system information\n"
       "IP information\n"
       "IP forwarding status\n")

DEFSH (VTYSH_RIPNGD, no_ripng_redistribute_type_metric_routemap_cmd_vtysh, 
       "no redistribute (kernel|connected|static|ospf6|bgp) metric <0-16> route-map WORD", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes\n"
       "Connected\n"
       "Static routes\n"
       "Open Shortest Path First (OSPFv3)\n"
       "Border Gateway Protocol (BGP)\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_protocol_cmd_vtysh, 
       "show ip route (bgp|connected|isis|kernel|ospf|rip|static)", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "Border Gateway Protocol (BGP)\n"
       "Connected\n"
       "ISO IS-IS (ISIS)\n"
       "Kernel\n"
       "Open Shortest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n")

DEFSH (VTYSH_BGPD, show_bgp_rsclient_cmd_vtysh, 
       "show bgp rsclient (A.B.C.D|X:X::X:X)", 
       "Show running system information\n"
       "BGP information\n"
       "Information about Route Server Client\n"
       "Neighbor address\nIPv6 address\n")

DEFSH (VTYSH_BGPD, ip_extcommunity_list_name_standard_cmd_vtysh, 
       "ip extcommunity-list standard WORD (deny|permit) .AA:NN", 
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify standard extcommunity-list\n"
       "Extended Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Extended community attribute in 'rt aa:nn_or_IPaddr:nn' OR 'soo aa:nn_or_IPaddr:nn' format\n")

DEFSH (VTYSH_BGPD, no_bgp_cluster_id_cmd_vtysh, 
       "no bgp cluster-id", 
       "Negate a command or set its defaults\n"
       "BGP information\n"
       "Configure Route-Reflector Cluster-id\n")

DEFSH (VTYSH_RIPNGD, show_debugging_ripng_cmd_vtysh, 
       "show debugging ripng", 
       "Show running system information\n"
       "Debugging functions (see also 'undebug')\n"
       "RIPng configuration\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_metric_cmd_vtysh, 
       "default-information originate metric <0-16777214>", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF default metric\n"
       "OSPF metric\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_spf_tree_cmd_vtysh, 
       "show ipv6 ospf6 spf tree", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Shortest Path First caculation\n"
       "Show SPF tree\n")

DEFSH (VTYSH_RIPD, no_ip_rip_authentication_mode_type_cmd_vtysh, 
       "no ip rip authentication mode (md5|text)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Routing Information Protocol\n"
       "Authentication control\n"
       "Authentication mode\n"
       "Keyed message digest\n"
       "Clear text authentication\n")

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_soft_in_cmd_vtysh, 
       "clear bgp peer-group WORD soft in", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, neighbor_ebgp_multihop_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "ebgp-multihop", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Allow EBGP neighbors not on directly connected networks\n")

DEFSH (VTYSH_BGPD, match_aspath_cmd_vtysh, 
       "match as-path WORD", 
       "Match values from routing table\n"
       "Match BGP AS path list\n"
       "AS path access-list name\n")

DEFSH (VTYSH_OSPFD, ospf_default_information_originate_metric_routemap_cmd_vtysh, 
       "default-information originate metric <0-16777214> route-map WORD", 
       "Control distribution of default information\n"
       "Distribute a default route\n"
       "OSPF default metric\n"
       "OSPF metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, clear_ip_bgp_all_soft_in_cmd_vtysh, 
       "clear ip bgp * soft in", 
       "Reset functions\n"
       "IP information\n"
       "BGP information\n"
       "Clear all peers\n"
       "Soft reconfig\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_BGPD, clear_bgp_ipv6_all_in_cmd_vtysh, 
       "clear bgp ipv6 * in", 
       "Reset functions\n"
       "BGP information\n"
       "Address family\n"
       "Clear all peers\n"
       "Soft reconfig inbound update\n")

DEFSH (VTYSH_ISISD, ipv6_router_isis_cmd_vtysh, 
       "ipv6 router isis WORD", 
       "IPv6 interface subcommands\n"
       "IPv6 Router interface commands\n"
       "IS-IS Routing for IPv6\n"
       "Routing process tag\n")

DEFSH (VTYSH_RIPNGD, ipv6_distribute_list_cmd_vtysh, 
       "distribute-list WORD (in|out) WORD", 
       "Filter networks in routing updates\n"
       "Access-list name\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_ZEBRA, show_ip_route_addr_cmd_vtysh, 
       "show ip route A.B.C.D", 
       "Show running system information\n"
       "IP information\n"
       "IP routing table\n"
       "Network in the IP routing table to display\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_type_detail_cmd_vtysh, 
       "show ipv6 ospf6 database "
       "(router|network|inter-prefix|inter-router|as-external|"
       "group-membership|type-7|link|intra-prefix) "
       "(detail|dump|internal)", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Display Router LSAs\n"
       "Display Network LSAs\n"
       "Display Inter-Area-Prefix LSAs\n"
       "Display Inter-Area-Router LSAs\n"
       "Display As-External LSAs\n"
       "Display Group-Membership LSAs\n"
       "Display Type-7 LSAs\n"
       "Display Link LSAs\n"
       "Display Intra-Area-Prefix LSAs\n"
       "Display details of LSAs\n"
       "Dump LSAs\n"
       "Display LSA's internal information\n"
      )

DEFSH (VTYSH_OSPFD, no_ip_ospf_mtu_ignore_cmd_vtysh, 
      "no ip ospf mtu-ignore", 
      "IP Information\n"
      "OSPF interface commands\n"
      "Disable mtu mismatch detection\n")

DEFSH (VTYSH_OSPFD, no_ospf_area_import_list_cmd_vtysh, 
       "no area (A.B.C.D|<0-4294967295>) import-list NAME", 
       "Negate a command or set its defaults\n"
       "OSPF area parameters\n"
       "OSPF area ID in IP address format\n"
       "OSPF area ID as a decimal value\n"
       "Unset the filter for networks announced to other areas\n"
       "Name of the access-list\n")

DEFSH (VTYSH_BGPD, match_peer_cmd_vtysh, 
       "match peer (A.B.C.D|X:X::X:X)", 
       "Match values from routing table\n"
       "Match peer address\n"
       "IPv6 address of peer\n"
       "IP address of peer\n")

DEFSH (VTYSH_RIPD|VTYSH_OSPFD|VTYSH_BGPD|VTYSH_ZEBRA, no_ip_prefix_list_seq_cmd_vtysh, 
       "no ip prefix-list WORD seq <1-4294967295> (deny|permit) (A.B.C.D/M|any)", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Build a prefix list\n"
       "Name of a prefix list\n"
       "sequence number of an entry\n"
       "Sequence number\n"
       "Specify packets to reject\n"
       "Specify packets to forward\n"
       "IP prefix <network>/<length>,  e.g.,  35.0.0.0/8\n"
       "Any prefix match.  Same as \"0.0.0.0/0 le 32\"\n")

DEFSH (VTYSH_BGPD, show_ip_bgp_instance_rsclient_summary_cmd_vtysh, 
       "show ip bgp view WORD rsclient summary", 
       "Show running system information\n"
       "IP information\n"
       "BGP information\n"
       "BGP view\n"
       "View name\n"
       "Information about Route Server Clients\n"
       "Summary of all Route Server Clients\n")

DEFSH (VTYSH_RIPD, no_rip_redistribute_type_metric_cmd_vtysh, 
       "no redistribute " "(kernel|connected|static|ospf|isis|bgp)" " metric <0-16>", 
       "Negate a command or set its defaults\n"
       "Redistribute information from another routing protocol\n"
       "Kernel routes (not installed via the zebra RIB)\n" "Connected routes (directly attached subnet or host)\n" "Statically configured routes\n" "Open Shortest Path First (OSPFv2)\n" "Intermediate System to Intermediate System (IS-IS)\n" "Border Gateway Protocol (BGP)\n"
       "Metric\n"
       "Metric value\n")

DEFSH (VTYSH_ZEBRA, show_ipv6_route_prefix_longer_cmd_vtysh, 
       "show ipv6 route X:X::X:X/M longer-prefixes", 
       "Show running system information\n"
       "IP information\n"
       "IPv6 routing table\n"
       "IPv6 prefix\n"
       "Show route matching the specified Network/Mask pair only\n")

DEFSH (VTYSH_BGPD, neighbor_weight_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "weight <0-65535>", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Set default weight for routes from this neighbor\n"
       "default weight\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_linkstate_router_cmd_vtysh, 
       "show ipv6 ospf6 linkstate router A.B.C.D", 
       "Show running system information\n"
       "IPv6 Information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display linkstate routing table\n"
       "Display Router Entry\n"
       "Specify Router ID as IPv4 address notation\n"
      )

DEFSH (VTYSH_BGPD, clear_bgp_peer_group_in_prefix_filter_cmd_vtysh, 
       "clear bgp peer-group WORD in prefix-filter", 
       "Reset functions\n"
       "BGP information\n"
       "Clear all members of peer-group\n"
       "BGP peer-group name\n"
       "Soft reconfig inbound update\n"
       "Push out prefix-list ORF and do inbound soft reconfig\n")

DEFSH (VTYSH_BGPD, bgp_distance_source_cmd_vtysh, 
       "distance <1-255> A.B.C.D/M", 
       "Define an administrative distance\n"
       "Administrative distance\n"
       "IP source prefix\n")

DEFSH (VTYSH_ISISD, no_isis_hello_multiplier_arg_cmd_vtysh, 
       "no isis hello-multiplier <3-1000>", 
       "Negate a command or set its defaults\n"
       "IS-IS commands\n"
       "Set multiplier for Hello holding time\n"
       "Hello multiplier value\n")

DEFSH (VTYSH_ZEBRA, no_debug_zebra_rib_q_cmd_vtysh, 
       "no debug zebra rib queue", 
       "Negate a command or set its defaults\n"
       "Debugging functions (see also 'undebug')\n"
       "Zebra configuration\n"
       "Debug zebra RIB\n"
       "Debug RIB queueing\n")

DEFSH (VTYSH_BGPD, no_neighbor_local_as_val_cmd_vtysh, 
       "no neighbor (A.B.C.D|X:X::X:X|WORD) " "local-as " "<1-4294967295>", 
       "Negate a command or set its defaults\n"
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Specify a local-as number\n"
       "AS number used as local AS\n")

DEFSH (VTYSH_BGPD, no_ip_extcommunity_list_name_standard_cmd_vtysh, 
       "no ip extcommunity-list standard WORD (deny|permit) .AA:NN", 
       "Negate a command or set its defaults\n"
       "IP information\n"
       "Add a extended community list entry\n"
       "Specify standard extcommunity-list\n"
       "Extended Community list name\n"
       "Specify community to reject\n"
       "Specify community to accept\n"
       "Extended community attribute in 'rt aa:nn_or_IPaddr:nn' OR 'soo aa:nn_or_IPaddr:nn' format\n")

DEFSH (VTYSH_RIPD, show_ip_rip_status_cmd_vtysh, 
       "show ip rip status", 
       "Show running system information\n"
       "IP information\n"
       "Show RIP routes\n"
       "IP routing protocol process parameters and statistics\n")

DEFSH (VTYSH_OSPFD, ip_ospf_message_digest_key_addr_cmd_vtysh, 
       "ip ospf message-digest-key <1-255> md5 KEY A.B.C.D", 
       "IP Information\n"
       "OSPF interface commands\n"
       "Message digest authentication password (key)\n"
       "Key ID\n"
       "Use MD5 algorithm\n"
       "The OSPF password (key)"
       "Address of interface")

DEFSH (VTYSH_RIPNGD, no_ripng_passive_interface_cmd_vtysh, 
       "no passive-interface IFNAME", 
       "Negate a command or set its defaults\n"
       "Suppress routing updates on an interface\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, no_bgp_timers_cmd_vtysh, 
       "no timers bgp", 
       "Negate a command or set its defaults\n"
       "Adjust routing timers\n"
       "BGP timers\n")

DEFSH (VTYSH_RIPD, distribute_list_prefix_cmd_vtysh, 
       "distribute-list prefix WORD (in|out) WORD", 
       "Filter networks in routing updates\n"
       "Filter prefixes in routing updates\n"
       "Name of an IP prefix-list\n"
       "Filter incoming routing updates\n"
       "Filter outgoing routing updates\n"
       "Interface name\n")

DEFSH (VTYSH_BGPD, show_bgp_ipv6_neighbor_received_routes_cmd_vtysh, 
       "show bgp ipv6 neighbors (A.B.C.D|X:X::X:X) received-routes", 
       "Show running system information\n"
       "BGP information\n"
       "Address family\n"
       "Detailed information on TCP and BGP neighbor connections\n"
       "Neighbor to display information about\n"
       "Neighbor to display information about\n"
       "Display the received routes from neighbor\n")

DEFSH (VTYSH_BGPD, bgp_redistribute_ipv4_metric_rmap_cmd_vtysh, 
       "redistribute (connected|kernel|ospf|rip|static) metric <0-4294967295> route-map WORD", 
       "Redistribute information from another routing protocol\n"
       "Connected\n"
       "Kernel routes\n"
       "Open Shurtest Path First (OSPF)\n"
       "Routing Information Protocol (RIP)\n"
       "Static routes\n"
       "Metric for redistributed routes\n"
       "Default metric\n"
       "Route map reference\n"
       "Pointer to route-map entries\n")

DEFSH (VTYSH_BGPD, neighbor_nexthop_self_cmd_vtysh, 
       "neighbor (A.B.C.D|X:X::X:X|WORD) " "next-hop-self", 
       "Specify neighbor router\n"
       "Neighbor address\nNeighbor IPv6 address\nNeighbor tag\n"
       "Disable the next hop calculation for this neighbor\n")

DEFSH (VTYSH_BGPD, show_bgp_route_map_cmd_vtysh, 
       "show bgp route-map WORD", 
       "Show running system information\n"
       "BGP information\n"
       "Display routes matching the route-map\n"
       "A route-map to match on\n")

DEFSH (VTYSH_OSPF6D, show_ipv6_ospf6_database_router_cmd_vtysh, 
       "show ipv6 ospf6 database * * A.B.C.D", 
       "Show running system information\n"
       "IPv6 information\n"
       "Open Shortest Path First (OSPF) for IPv6\n"
       "Display Link state database\n"
       "Any Link state Type\n"
       "Any Link state ID\n"
       "Specify Advertising Router as IPv4 address notation\n"
      )

void
vtysh_init_cmd ()
{
  install_element (CONFIG_NODE, &no_debug_zebra_rib_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_string_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_irdp_cmd_vtysh);
  install_element (BGP_NODE, &bgp_damp_unset2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_damp_unset_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_seq_ge_le_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_router_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_sequence_number_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ripng_split_horizon_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &ipv6_aggregate_address_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_neighbors_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_spftrigg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_clns_neighbors_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_standard_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_router_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_all_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_max_metric_router_lsa_startup_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ripng_status_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community_all_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_flags_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_ipv6_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_out_cmd_vtysh);
  install_element (RMAP_NODE, &match_peer_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_spfevents_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_int_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_route_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_local_as_no_prepend_cmd_vtysh);
  install_element (ZEBRA_NODE, &ripng_redistribute_ripng_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authkey_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_neighbor_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community3_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_md5_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_passive_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_sequence_number_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_err_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ripng_events_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_csum_cmd_vtysh);
  install_element (CONFIG_NODE, &no_route_map_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_tag_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_out_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_memory_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_strict_capability_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_default_metric_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_ipv6_neighbors_peer_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_in_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_paths_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_prefix_list_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_graceful_restart_stalepath_time_val_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_rsclient_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_substitute_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_override_capability_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_in_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_route_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_instance_ipv4_summary_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_interface_cmd_vtysh);
  install_element (BGP_NODE, &bgp_cluster_id32_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_id_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_regexp_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ripng_packet_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_shortcut_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_priority_addr_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_flags_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (RIP_NODE, &no_distribute_list_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_csum_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_timers_throttle_spf_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_id_self_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_vpnv4_soft_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_gen_interval_l1_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_any_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community3_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_filter_list_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_aggregate_address_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_zebra_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &bgp_cluster_id_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_type_metric_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_ifname_flags_pref_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &no_key_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_ttl_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_distance_source_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_passwd_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_dont_capability_negotiate_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ripng_split_horizon_poisoned_reverse_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_import_check_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_cmd_vtysh);
  install_element (OSPF_NODE, &capability_opaque_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_forwarding_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_origin_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_prefix_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_natural_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_address_label_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_next_hop_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_rsclient_route_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community2_exact_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_shutdown_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_memory_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_bgp_redistribute_ipv6_rmap_metric_cmd_vtysh);
  install_element (RMAP_NODE, &match_tag_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_filter_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_received_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_rip_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community_list_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_update_direct_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_name_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_upd_cmd_vtysh);
  install_element (VTY_NODE, &vty_restricted_mode_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_routes_interval_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_rip_zebra_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_hellointerval_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_summary_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community_cmd_vtysh);
  install_element (OSPF_NODE, &no_router_ospf_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_community_list_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_host_mask_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_packet_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_aspath_confed_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_split_horizon_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_ge_le_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_id_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_host_any_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_mask_as_set_cmd_vtysh);
  install_element (BGP_NODE, &bgp_deterministic_med_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_id_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_ipv6_summary_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_capability_dynamic_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_l2_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_in_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbors_peer_cmd_vtysh);
  install_element (RMAP_NODE, &no_rmap_onmatch_goto_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_param4_cmd_vtysh);
  install_element (BGP_NODE, &old_no_ipv6_aggregate_address_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_name_standard_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbors_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_upd_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_brouter_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_nsm_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community2_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_damp_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_route_source_prefix_list_val_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_metric_val_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_route_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_le_ge_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_updates_cmd_vtysh);
  install_element (RMAP_NODE, &match_interface_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_route_map_cmd_vtysh);
  install_element (RIPNG_NODE, &no_if_ipv6_rmap_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_hello_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_router_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_prefix_match_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_regexp_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_ebgp_multihop_ttl_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_month_day_day_month_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_as4_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_l1_arg_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ecommunity_soo_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community_all_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_l1_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_backdoor_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_multicast_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_zebra_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_redistribute_type_metric_routemap_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (OSPF6_NODE, &no_ospf6_interface_area_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_out_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community2_exact_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_rip_events_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_metric_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_database_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_natural_backdoor_ttl_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_lsa_hex_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_summary_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_authtype_md5_cmd_vtysh);
  install_element (CONFIG_NODE, &no_dump_bgp_all_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_snp_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_l2_arg_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_passive_interface_default_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_adv_router_linkstate_id_detail_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_match_detail_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_split_horizon_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_pathlimit_ttl_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_adj_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_rip_packet_direct_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_weight_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_neighbor_priority_cmd_vtysh);
  install_element (OSPF_NODE, &no_capability_opaque_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_border_routers_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_rsclient_route_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &key_string_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_description_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_ism_sub_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_flooding_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_summary_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community3_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_as4_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_passive_interface_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_flags_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_in_cmd_vtysh);
  install_element (INTERFACE_NODE, &csnp_interval_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_standard_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_normal_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community3_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (RMAP_NODE, &set_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_route_map_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_flap_cmd_vtysh);
  install_element (RMAP_NODE, &set_community_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_linkstate_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_standard_all_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_intra_inter_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_rsclient_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_seq_le_ge_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_distance2_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_received_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &linkdetect_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_rsclient_summary_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_offset_list_ifname_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_summary_as_set_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_l2_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_in_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_seq_ge_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_export_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_cost_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community_exact_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_l1_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_distance_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &bgp_redistribute_ipv6_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_ipv6_neighbors_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_rsclient_route_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (RMAP_NODE, &set_aspath_exclude_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_retransmit_interval_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_mask_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_neighbors_cmd_vtysh);
  install_element (RMAP_NODE, &match_aspath_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_asbr_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_vpnv4_soft_in_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_rtevents_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_lsa_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_weight_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_soft_in_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community3_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_rib_q_cmd_vtysh);
  install_element (BGP_NODE, &bgp_fast_external_failover_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_flooding_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_summary_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_route_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ripng_events_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbors_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_prefix_list_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_rd_summary_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_soft_out_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_ripng_split_horizon_poisoned_reverse_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_zebra_sub_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_natural_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_ifname_flags_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_mask_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_summary_name_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_adv_router_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_default_cost_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ripng_packet_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_param2_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_infinite_day_month_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community_exact_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (BGP_NODE, &old_no_ipv6_aggregate_address_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_filter_list_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_timers_arg_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_transmit_delay_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ip_nexthop_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_soft_in_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_vpnv4_soft_out_cmd_vtysh);
  install_element (OSPF6_NODE, &ospf6_redistribute_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_damp_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_vpnv4_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_filter_list_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ipv6_distribute_list_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_confederation_identifier_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_zebra_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_ipv4_soft_in_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_snp_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_bgp_neighbor_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_neighbor_detail_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbors_peer_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_out_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_ifname_prefix_match_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_events_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_name_standard_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_seq_ge_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_backdoor_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_ebgp_multihop_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_nssa_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community2_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_ipv6_neighbors_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_forwarding_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_metric_routemap_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_ism_sub_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community4_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_le_ge_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_vpnv4_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_packet_send_recv_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_mode_type_authlen_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aspath_prepend_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community_list_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_adj_cmd_vtysh);
  install_element (RMAP_NODE, &set_src_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_cost_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_update_source_cmd_vtysh);
  install_element (BGP_NODE, &old_no_ipv6_bgp_network_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_auto_cost_reference_bandwidth_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community_list_exact_cmd_vtysh);
  install_element (RMAP_NODE, &match_peer_local_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_route_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_mask_host_cmd_vtysh);
  install_element (KEYCHAIN_NODE, &no_key_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_network_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_received_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_spf_process_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_bgp_neighbor_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_cost_u32_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (INTERFACE_NODE, &mpls_te_link_metric_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_default_originate_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_default_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community3_exact_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_stub_no_summary_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_onmatch_next_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_port_val_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_redistribute_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_interface_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_instance_peer_rsclient_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aspath_prepend_val_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_ebgp_multihop_ttl_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_simulate_spf_tree_root_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_address_label_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_advertised_route_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ripng_events_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_type_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_ospf_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_passive_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_regexp_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_in_prefix_filter_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbors_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_rip_packet_detail_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_activate_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_timers_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_regexp_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_ipv4_rsclient_summary_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_events_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_id_router_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbor_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_irdp_address_preference_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_distance_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_mtu_ignore_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_damp_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_compare_router_id_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_log_adjacency_changes_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_split_horizon_poisoned_reverse_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_rsclient_summary_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_neighbor_priority_poll_interval_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_authentication_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_arg_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_args_authkey_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_peer_val_cmd_vtysh);
  install_element (ISIS_NODE, &no_metric_style_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_prefix_longer_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_description_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_received_routes_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (VIEW_NODE, &show_isis_neighbors_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_flap_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_med2_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_range_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_routemap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbors_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_onmatch_goto_cmd_vtysh);
  install_element (VTY_NODE, &no_vty_access_class_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_l1_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &bgp_log_neighbor_changes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_adv_router_linkstate_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_message_sendrecv_cmd_vtysh);
  install_element (RIPNG_NODE, &ipv6_distribute_list_prefix_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_redistribute_type_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_snp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_address_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_default_originate_rmap_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_prefix_list_name_cmd_vtysh);
  install_element (ENABLE_NODE, &rmap_show_name_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_longer_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_prefix_list_name_prefix_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbors_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_metric_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community3_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_packet_send_recv_detail_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_opaque_capable_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_auto_cost_reference_bandwidth_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_vpnv4_in_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community_all_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_lupd_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (RIP_NODE, &rip_redistribute_type_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_range_cost_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community4_cmd_vtysh);
  install_element (RMAP_NODE, &match_pathlimit_as_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_month_day_day_month_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_brouter_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_dampened_paths_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_nssa_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_in_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &router_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_rsclient_summary_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_zebra_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_flap_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_access_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_update_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbors_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_filter_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_in_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_external_inter_intra_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_le_ge_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_next_hop_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_route_source_prefix_list_cmd_vtysh);
  install_element (OSPF6_NODE, &no_area_export_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community_all_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_inter_intra_external_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_rsclient_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_in_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_default_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_zebra_sub_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_address_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_name_seq_cmd_vtysh);
  install_element (CONFIG_NODE, &bgp_config_type_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_tag_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_ospf6_passive_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community_all_cmd_vtysh);
  install_element (ISIS_NODE, &spf_interval_l2_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_debug_packet_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_mask_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_debug_messages_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_spf_time_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_soft_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_abr_cmd_vtysh);
  install_element (INTERFACE_NODE, &csnp_interval_l2_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_vpnv4_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_regexp_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_adv_router_linkstate_id_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_as_path_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_hostname_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_id_adv_router_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_remark_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_spfevents_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_priority_l1_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_backdoor_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_rib_q_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_ge_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_le_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_l1_arg_cmd_vtysh);
  install_element (BGP_NODE, &bgp_distance_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_summary_as_set_cmd_vtysh);
  install_element (OSPF6_NODE, &no_area_filter_list_cmd_vtysh);
  install_element (ISIS_NODE, &spf_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_compatible_rfc1583_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_med3_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_router_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_filter_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community2_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_as_set_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_priority_l2_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_name_standard2_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (ISIS_NODE, &net_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_type_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ripng_packet_direct_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_enforce_multihop_cmd_vtysh);
  install_element (BGP_NODE, &bgp_graceful_restart_stalepath_time_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_router_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_mask_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_redistribute_type_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_flap_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_summary_only_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_sequence_number_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_interface_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_flap_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_events_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_ifname_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_maxadvertinterval_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community3_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_in_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_keepalive_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_cost_u32_inet4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_route_map_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_seq_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_prefix_longer_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_distance_source_access_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_intra_external_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_as4_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_adv_router_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_dead_interval_addr_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_bgp_redistribute_ipv6_rmap_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_access_list_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community3_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_spf_tree_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_router_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_all_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_neighbor_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbors_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_lupd_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aggregator_as_val_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_description_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_ifname_prefix_detail_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_isis_topology_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_scan_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_flags_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_soft_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_forwarding_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_default_originate_rmap_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_host_any_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_address_cmd_vtysh);
  install_element (INTERFACE_NODE, &multicast_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_address_preference_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aspath_exclude_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_access_list_name_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_community_delete_val_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_key_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community_list_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_neighbor_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_filter_list_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_refresh_timer_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_peer_rsclient_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ripng_packet_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_views_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_natural_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_as_set_summary_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_timers_connect_val_cmd_vtysh);
  install_element (BGP_NODE, &bgp_default_ipv4_unicast_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_scan_time_val_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_mtu_ignore_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_local_as_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_in_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_in_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ripng_packet_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_damp_set3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_area_spf_tree_cmd_vtysh);
  install_element (VTY_NODE, &vty_no_restricted_mode_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_rib_q_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_soft_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_as_path_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_nexthop_local_unchanged_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_nsm_sub_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_router_isis_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_route_map_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_remark_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_redistribute_type_routemap_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_router_isis_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_nssa_no_summary_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_external_intra_inter_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_flags2_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_seq_le_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_message_digest_key_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ipv6_prefix_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_mode_type_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_abr_type_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (RIP_NODE, &rip_default_information_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_seq_le_ge_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_seq_ge_le_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_direct_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_external_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_log_adjacency_changes_detail_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_cost_inet4_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &ipv6_bgp_network_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_shutdown_cmd_vtysh);
  install_element (BGP_NODE, &bgp_redistribute_ipv4_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_damp_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ipv6_nexthop_global_val_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_activate_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_distribute_list_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_adv_router_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_rip_packet_detail_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_nssa_translate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_neighbor_received_routes_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_param4_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_rsclient_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_flags_distance_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_passive_interface_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_passive_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_tags_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_advertise_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_linkstate_detail_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_passive_interface_addr_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_neighbor_advertised_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_message_digest_key_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_type_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community4_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_in_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_routes_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_shortcut_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_ipv4_soft_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_bgp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_hostname_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_type_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (VTY_NODE, &exec_timeout_min_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_adv_router_linkstate_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_router_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_arg_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community2_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_hello_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_zebra_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_inter_external_intra_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_statistics_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_self_originated_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_message_sendrecv_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_prefix_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_retransmit_interval_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_ttl_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_rfc1583_flag_cmd_vtysh);
  install_element (ENABLE_NODE, &show_isis_topology_l2_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_name_standard_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_all_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community3_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_timers_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_events_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_distance_ospf_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_duration_day_month_cmd_vtysh);
  install_element (RIP_NODE, &rip_network_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community2_exact_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_filter_list_cmd_vtysh);
  install_element (RMAP_NODE, &set_vpnv4_nexthop_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_lupd_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_name_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_router_isis_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ipv6_distribute_list_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_paths_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_stub_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_month_day_month_day_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_summary_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_mbgp_neighbor_advertised_route_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_mbgp_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ip_nexthop_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_filter_cmd_vtysh);
  install_element (RIP_NODE, &rip_timers_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_authentication_key_chain_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_mask_as_set_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_hello_interval_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ipv6_address_cmd_vtysh);
  install_element (BGP_NODE, &no_auto_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_linkstate_id_cmd_vtysh);
  install_element (INTERFACE_NODE, &mpls_te_link_rsc_clsclr_cmd_vtysh);
  install_element (OSPF6_NODE, &no_ospf6_redistribute_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_update_direct_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_zebra_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_default_originate_rmap_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_rib_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_community_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_ospf_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_as4_segment_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_rsclient_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_interval_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_redistribute_type_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_linkstate_id_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_summary_name_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_paths_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_address_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_interface_cmd_vtysh);
  install_element (BGP_NODE, &bgp_redistribute_ipv4_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_self_originated_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_soft_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_l1_arg_cmd_vtysh);
  install_element (RMAP_NODE, &match_ipv6_address_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_rip_status_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_normal_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_damp_set_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_message_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_timers_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_default_originate_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_metric_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_community_none_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community4_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_lifetime_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_views_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_disable_connected_check_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_timers_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_external_inter_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_type_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_timers_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_spfstats_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_packet_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_confederation_peers_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_split_horizon_poisoned_reverse_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_neighbors_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_mask_as_set_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_nsm_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_neighbor_advertised_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_rsclient_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_spf_process_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_packet_direct_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_cmd_vtysh);
  install_element (BGP_NODE, &bgp_damp_set3_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_pathlimit_as_val_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_rsclient_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_vpnv4_nexthop_val_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_address_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_rip_events_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_in_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_ge_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_brouter_router_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_version_val_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_timers_connect_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_type_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_mask_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_event_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_default_originate_rmap_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_remote_as_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_rtevents_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_as_set_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_nsm_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_packet_send_recv_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_interval_l2_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_summary_name_cmd_vtysh);
  install_element (ENABLE_NODE, &show_table_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_ipv6_neighbors_cmd_vtysh);
  install_element (RIP_NODE, &rip_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_default_cost_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_spf_database_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_event_cmd_vtysh);
  install_element (ISIS_NODE, &domain_passwd_snpauth_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_tags_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_border_routers_detail_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_route_map_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (ZEBRA_NODE, &no_ripng_redistribute_ripng_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (INTERFACE_NODE, &bandwidth_if_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_summary_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (BGP_NODE, &bgp_redistribute_ipv4_metric_rmap_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_as_set_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &shutdown_if_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_rib_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_route_source_val_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_prefix_longer_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_peer_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_description_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_detail_name_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_minadvertinterval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_border_routers_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_name_seq_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_in_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_mbgp_neighbor_routes_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_lsa_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_filter_list_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_offset_list_ifname_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_aspath_val_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_disable_connected_check_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_lsa_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_detail_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_neighbor_prefix_counts_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_zebra_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_in_prefix_filter_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_attr_info_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_ipv4_summary_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_vpnv4_out_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_ism_sub_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_transmit_delay_addr_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_ifname_pref_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_spftrigg_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_rsclient_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ripng_zebra_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_receive_version_1_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged4_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community2_exact_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_transmit_delay_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_zebra_kernel_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_out_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_instance_all_rsclient_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_router_detail_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_send_version_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_router_id_val_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_packet_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community4_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_prefix_counts_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (RIP_NODE, &rip_default_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_router_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_authentication_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_nsm_sub_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_soft_in_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_default_local_preference_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_neighbor_cmd_vtysh);
  install_element (OSPF6_NODE, &no_area_import_list_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_detail_name_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_import_check_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_forwarding_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_ospf6_ifmtu_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_remark_arg_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_spftrigg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_prefix_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_activate_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_rsclient_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_prefix_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_instance_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_soft_in_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community4_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_prefix_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_ifname_prefix_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ecommunity_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbor_prefix_counts_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community4_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_retransmit_interval_addr_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_tag_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_statistics_view_vpnv4_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_dampening_address_mask_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_neighbor_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_spf_database_cmd_vtysh);
  install_element (ENABLE_NODE, &show_database_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_prefix_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_self_originated_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_update_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_adj_cmd_vtysh);
  install_element (BGP_NODE, &bgp_distance_source_access_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_detail_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (RIP_NODE, &rip_redistribute_type_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_detail_name_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_prefix_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_param3_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_neighbor_priority_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_soft_in_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_type_metric_routemap_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_hello_interval_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_message_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_mbgp_neighbor_received_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community2_exact_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_send_version_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_filter_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_rip_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_max_metric_router_lsa_shutdown_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_graceful_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_in_prefix_filter_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_address_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &ipv6_aggregate_address_summary_only_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_ttl_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_holdtime_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_l2_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_rsclient_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_ism_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_zebra_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_param1_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community_list_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_address_prefix_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_description_arg_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_rsclient_route_cmd_vtysh);
  install_element (ZEBRA_NODE, &redistribute_ospf6_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_nssa_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (RMAP_NODE, &set_ipv6_nexthop_local_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_default_metric_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_le_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_ipv6_bgp_network_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_type_metric_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_origin_val_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_aggregate_address_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (VIEW_NODE, &show_mpls_te_link_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_mask_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_views_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_prefix_match_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community_list_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_routes_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_neighbor_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_packet_send_recv_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_soft_in_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_damp_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_community_list_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community3_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_soft_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_default_local_preference_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_redistribute_ipv4_metric_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ripng_packet_direct_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_ipv6_neighbors_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_soft_in_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_any_mask_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_rip_zebra_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_route_map_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_spfevents_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_out_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_out_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_circuit_type_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_statistics_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_scan_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_prefix_list_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_authentication_key_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_log_neighbor_changes_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community3_cmd_vtysh);
  install_element (VIEW_NODE, &show_debugging_zebra_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_ospf6_advertise_prefix_list_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_transmit_delay_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_arg_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ipv6_ripng_split_horizon_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_vpnv4_soft_in_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_ipv6_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_originator_id_val_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_inter_intra_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_activate_cmd_vtysh);
  install_element (ISIS_NODE, &dynamic_hostname_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_l2_arg_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_dead_interval_minimal_addr_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_address_prefix_list_val_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_view_rsclient_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_authentication_message_digest_cmd_vtysh);
  install_element (BGP_NODE, &bgp_redistribute_ipv4_rmap_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_isis_topology_l1_cmd_vtysh);
  install_element (INTERFACE_NODE, &csnp_interval_l1_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_vpnv4_out_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_ttl_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_as_set_summary_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_receive_version_num_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_in_prefix_filter_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_metric_routemap_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (RIP_NODE, &rip_redistribute_type_metric_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_message_sendrecv_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_isis_neighbors_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbors_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_rip_zebra_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (ISIS_NODE, &no_is_type_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_transmitdelay_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_l1_cmd_vtysh);
  install_element (BGP_NODE, &bgp_redistribute_ipv4_rmap_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_retransmit_interval_cmd_vtysh);
  install_element (RMAP_NODE, &set_weight_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_fsm_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_interface_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_packet_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_prefix_longer_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ripng_packet_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_authentication_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_lsa_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_vpnv4_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_in_prefix_filter_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_type_metric_routemap_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_rip_packet_direct_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_detail_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community_list_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (INTERFACE_NODE, &mpls_te_link_unrsv_bw_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_normal_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_redistribute_ipv4_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community2_exact_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_origin_val_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_standard_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (RMAP_NODE, &set_community_none_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_cmd_vtysh);
  install_element (RIP_NODE, &rip_distance_source_access_list_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_advertise_interval_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_flags_distance_cmd_vtysh);
  install_element (BGP_NODE, &bgp_damp_set_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_standard_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_rsclient_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_flooding_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_forwarding_cmd_vtysh);
  install_element (CONFIG_NODE, &no_route_map_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_rsclient_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_out_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_rd_neighbors_peer_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_ttl_cmd_vtysh);
  install_element (RMAP_NODE, &match_ipv6_next_hop_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_default_originate_rmap_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_cluster_id_arg_cmd_vtysh);
  install_element (VTY_NODE, &no_vty_ipv6_access_class_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_zebra_kernel_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_supernets_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_no_match_address_prefixlist_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_in_prefix_filter_cmd_vtysh);
  install_element (ISIS_NODE, &no_area_passwd_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_ospf6_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_soft_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community_list_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_router_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_all_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ecommunity_rt_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community4_exact_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_args_md5_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_as_path_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ipv6_distribute_list_prefix_all_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_zebra_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_detail_name_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_timers_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_rsclient_route_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_memory_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_metric_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_rsclient_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_upd_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community2_cmd_vtysh);
  install_element (BGP_NODE, &bgp_scan_time_cmd_vtysh);
  install_element (BGP_NODE, &bgp_client_to_client_reflection_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_cidr_only_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_lsa_hex_cmd_vtysh);
  install_element (RMAP_NODE, &set_aggregator_as_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_spf_database_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_mbgp_neighbor_routes_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_ttl_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_ge_le_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_neighbors_peer_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (RMAP_NODE, &match_metric_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_day_month_day_month_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_bgp_config_type_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_passive_interface_addr_cmd_vtysh);
  install_element (BGP_NODE, &old_ipv6_aggregate_address_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_forwarding_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_prefix_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_bgp_redistribute_ipv6_metric_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_router_detail_cmd_vtysh);
  install_element (RMAP_NODE, &set_metric_addsub_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_distance_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_name_seq_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_int_detail_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_l2_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community4_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community_list_exact_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_all_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_redistribute_ipv4_rmap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_neighbor_received_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_packet_send_recv_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_message_digest_key_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_advertised_route_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_linkstate_id_detail_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_deadinterval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_summary_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_neighbors_peer_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_local_as_val2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbors_cmd_vtysh);
  install_element (RIP_NODE, &rip_passive_interface_cmd_vtysh);
  install_element (RIP_NODE, &rip_offset_list_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_rip_events_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_as4_segment_cmd_vtysh);
  install_element (OSPF_NODE, &router_ospf_id_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_cluster_id_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_duration_month_day_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_deterministic_med_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_port_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_in_prefix_filter_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_debug_disable_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_events_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_border_routers_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_detail_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &ipv6_bgp_network_ttl_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_network_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_cost_u32_inet4_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_le_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_set_forwarding_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_irdp_shutdown_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_any_host_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_self_originated_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_soft_in_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_mask_any_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_redistribute_type_metric_routemap_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_set_metric_type_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_natural_backdoor_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community_list_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_adv_router_detail_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_local_pref_val_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_protocol_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_interface_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_network_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_le_ge_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community3_exact_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_fast_external_failover_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_damp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community3_exact_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_access_list_name_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_as4_segment_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_as4_segment_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_view_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_rtevents_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_standard2_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_detail_all_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community4_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_zebra_sendrecv_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_keepalive_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_rsclient_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_database_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_compatible_rfc1583_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_prefix_first_match_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_advertised_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_metric_type_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_protocol_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_rsclient_route_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_password_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_route_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_strict_capability_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community4_exact_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_multiplier_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_zebra_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_route_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ecommunity_rt_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_flags_distance2_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_no_set_metric_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_rsclient_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbors_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_prefix_longer_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_any_host_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_gen_interval_l2_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_redistribute_ipv4_metric_rmap_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_summary_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_rip_packet_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_spf_time_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_prefix_cmd_vtysh);
  install_element (BGP_NODE, &bgp_damp_set2_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_route_server_client_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_soft_out_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_rip_status_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_soft_in_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_refresh_timer_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community4_exact_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community3_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_area_spf_tree_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_flags_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_seq_ge_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_import_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_intra_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_natural_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_distance_source_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_route_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_table_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_update_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_l1_arg_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_shutdown_if_cmd_vtysh);
  install_element (BGP_NODE, &address_family_ipv6_safi_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_fsm_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_passwd_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_any_mask_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_normal_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_prefix_longer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_adv_router_detail_cmd_vtysh);
  install_element (OSPF6_NODE, &no_area_range_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_update_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_default_metric_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_router_id_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_origin_cmd_vtysh);
  install_element (VTY_NODE, &no_exec_timeout_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_protocol_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &set_atomic_aggregate_cmd_vtysh);
  install_element (BGP_NODE, &bgp_default_local_preference_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_id_cmd_vtysh);
  install_element (RIPNG_NODE, &ipv6_distribute_list_all_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_prefix_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_gen_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_self_originated_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_out_cmd_vtysh);
  install_element (ENABLE_NODE, &no_terminal_monitor_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_soft_in_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_community_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_events_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_authentication_mode_authlen_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_asbr_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_default_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_in_prefix_filter_cmd_vtysh);
  install_element (ISIS_NODE, &no_dynamic_hostname_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_cmd_vtysh);
  install_element (BGP_NODE, &no_synchronization_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_neighbor_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_damp_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_rsclient_summary_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_name_standard_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_soft_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_distance_cmd_vtysh);
  install_element (ISIS_NODE, &no_domain_passwd_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_out_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_authtype_authkey_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_brouter_router_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_not_advertise_cmd_vtysh);
  install_element (VIEW_NODE, &show_isis_topology_l2_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_fsm_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_next_hop_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_abr_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_default_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_all_cmd_vtysh);
  install_element (BGP_NODE, &bgp_always_compare_med_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community4_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_circuit_type_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_description_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_vpnv4_in_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_password_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_all_interval_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_nexthop_local_unchanged_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_dead_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_protocol_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_metric_type_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_soft_out_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_key_addr_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_graceful_restart_stalepath_time_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_expanded_all_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_l2_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_prefix_list_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_rsclient_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_rsclient_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_shutdown_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_redistribute_type_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_message_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_arg_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_rd_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_cmd_vtysh);
  install_element (BGP_NODE, &bgp_timers_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_neighbors_peer_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_key_chain2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_neighbor_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_events_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_aspath_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_nssa_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_in_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_ripng_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_dead_interval_minimal_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_bgp_redistribute_ipv6_metric_rmap_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_ifname_prefix_match_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_gen_interval_l2_arg_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_interface_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_view_rsclient_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_description_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_soft_in_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_hello_interval_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_rip_packet_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_zebra_sendrecv_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_bgp_redistribute_ipv6_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_send_version_2_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_lupd_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_standard_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (VTY_NODE, &vty_access_class_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_flags2_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_community_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_neighbors_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_prefix_longer_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_redistribute_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_any_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_exact_cmd_vtysh);
  install_element (RIP_NODE, &distribute_list_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_brouter_router_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_event_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_priority_addr_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_upd_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_seq_le_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_neighbors_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_in_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_lsa_sub_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_damp_unset2_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_natural_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_prefix_first_match_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_description_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_flags_pref_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_adv_router_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_redistribute_type_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_cmd_vtysh);
  install_element (ENABLE_NODE, &show_version_ospf6_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_protocol_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_send_version_num_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_le_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_priority_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_priority_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_ripng_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_prefix_first_match_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_prefix_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_ipv6_bgp_network_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_self_originated_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_isis_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_routes_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_authkey_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_atomic_aggregate_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_dead_interval_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &ipv6_bgp_network_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_adv_router_linkstate_id_detail_cmd_vtysh);
  install_element (RMAP_NODE, &no_rmap_onmatch_next_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_summary_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_soft_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_prefix_list_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_match_address_prefixlist_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_redistribute_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_ism_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_max_metric_router_lsa_startup_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_duration_day_month_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_mask_flags_distance2_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community4_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_range_advertise_cost_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_next_hop_prefix_list_val_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community2_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_broadcast_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_refresh_timer_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_ipv6_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_keepalive_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_ifname_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_rip_packet_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ipv6_next_hop_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_l1_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_network_area_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_isis_neighbors_detail_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_range_advertise_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_all_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_neighbor_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_lsa_hex_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_summary_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_l1_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_neighbor_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_prefix_list_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_cmd_vtysh);
  install_element (RMAP_NODE, &set_tag_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_flap_cmd_vtysh);
  install_element (ISIS_NODE, &metric_style_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_log_adjacency_changes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_adv_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_clns_neighbors_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_all_soft_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_extcommunity_list_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_interface_val_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_import_list_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_stub_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_stub_no_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_tags_cmd_vtysh);
  install_element (ISIS_NODE, &area_passwd_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (OSPF6_NODE, &ospf6_interface_area_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_update_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community4_exact_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_nssa_translate_no_summary_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_dampening_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_prefix_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_zebra_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_redistribute_type_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_prefix_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community2_cmd_vtysh);
  install_element (RIP_NODE, &rip_version_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_default_originate_rmap_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_cost_u32_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (BGP_NODE, &bgp_enforce_first_as_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_abr_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_send_version_1_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_interface_desc_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_route_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_rsclient_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_routes_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_linkstate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_neighbor_routes_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_cost_u32_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_transmit_delay_addr_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_peer_group_remote_as_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_soft_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_id_adv_router_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_community_val_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_protocol_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_max_metric_router_lsa_admin_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_unsuppress_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_router_detail_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_receive_version_cmd_vtysh);
  install_element (RMAP_NODE, &set_aspath_prepend_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_weight_val_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_l2_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_packet_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_distance_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_kernel_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_expanded_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_message_digest_key_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_csum_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_all_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_linkstate_id_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_retransmit_interval_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_rsclient_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_multiplier_l1_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_message_digest_key_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_nsm_sub_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_spf_process_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_route_reflector_client_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_default_originate_rmap_cmd_vtysh);
  install_element (ISIS_NODE, &spf_interval_l1_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_any_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_prefix_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_flags_distance2_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_out_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_soft_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_nssa_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_as_path_access_list_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community3_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_continue_index_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_route_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_instance_ipv4_rsclient_summary_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged8_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_ge_le_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_id_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_send_community_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_self_originated_linkstate_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_match_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_peer_rsclient_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_transmit_delay_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (ZEBRA_NODE, &no_rip_redistribute_rip_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_vpnv4_network_cmd_vtysh);
  install_element (CONFIG_NODE, &config_table_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_spf_process_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_adv_router_linkstate_id_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_brouter_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community4_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_description_cmd_vtysh);
  install_element (ENABLE_NODE, &show_mpls_te_link_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_summary_only_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_spfstats_cmd_vtysh);
  install_element (OSPF6_NODE, &area_export_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_cidr_only_cmd_vtysh);
  install_element (RIP_NODE, &rip_distance_source_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_interface_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community4_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_vpnv4_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_mask_flags_distance_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_name_standard2_cmd_vtysh);
  install_element (VIEW_NODE, &show_mpls_te_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_regexp_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_always_compare_med_cmd_vtysh);
  install_element (ENABLE_NODE, &show_interface_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_border_routers_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_metric_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_name_seq_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_self_originated_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community3_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_default_originate_rmap_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_host_host_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ipv6_nexthop_local_val_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_ipv6_rsclient_summary_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_redistribute_source_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community3_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aggregator_as_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_linkstate_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_prefix_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_intra_inter_external_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_out_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_neighbor_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_soft_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (BGP_NODE, &old_ipv6_aggregate_address_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_spf_time_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_view_route_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_summary_as_set_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_packet_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ripng_packet_detail_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_lifetime_l1_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_name_expanded_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_self_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_detail_all_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_natural_backdoor_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_soft_cmd_vtysh);
  install_element (RMAP_NODE, &match_community_exact_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_rsclient_prefix_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_regexp_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ipv6_nexthop_local_cmd_vtysh);
  install_element (ISIS_NODE, &area_passwd_snpauth_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community3_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_default_originate_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_month_day_month_day_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_default_information_originate_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_prefix_longer_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_rsclient_route_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_linkstate_router_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_ism_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_neighbor_poll_interval_cmd_vtysh);
  install_element (ENABLE_NODE, &show_debugging_bgp_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_cidr_only_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_summary_cmd_vtysh);
  install_element (RIP_NODE, &no_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_rsclient_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_description_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_summary_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_rfc1583_flag_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_infinite_day_month_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ipv6_prefix_list_name_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_prefix_first_match_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_metric_type_cmd_vtysh);
  install_element (BGP_NODE, &bgp_distance_source_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_lsa_sub_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_cmd_vtysh);
  install_element (OSPF_NODE, &mpls_te_router_addr_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_next_hop_prefix_list_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_local_as_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_instance_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ripng_zebra_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_seq_ge_le_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_compare_router_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_regexp_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_rsclient_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_allowas_in_cmd_vtysh);
  install_element (VTY_NODE, &exec_timeout_sec_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_host_host_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_range_advertise_cost_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_ism_sub_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_zebra_sub_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_rsclient_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_retransmit_interval_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_any_any_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_neighbor_flap_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_prefix_detail_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_neighbor_routes_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_out_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_expanded_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_abr_type_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_summary_only_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_name_expanded_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_adv_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_name_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_cmd_vtysh);
  install_element (RMAP_NODE, &set_ecommunity_soo_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_multiplier_l2_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_damp_set2_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ecommunity_soo_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_prefix_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_redistribute_type_routemap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_rsclient_route_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_fsm_cmd_vtysh);
  install_element (CONFIG_NODE, &bgp_multiple_instance_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_seq_le_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_match_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_linkstate_id_detail_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_offset_list_ifname_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_standard_host_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_default_metric_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_paths_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_nssa_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_cost_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_ipv6_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_adv_router_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_default_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_filter_list_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_seq_le_ge_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_neighbor_received_routes_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_set_peer_group_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_self_originated_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_prefix_longer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_addr_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_mask_flags2_cmd_vtysh);
  install_element (CONFIG_NODE, &dump_bgp_updates_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community4_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_confederation_identifier_arg_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged4_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community3_exact_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_scan_time_cmd_vtysh);
  install_element (OSPF6_NODE, &area_filter_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_string2_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_redistribute_ipv4_rmap_metric_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbor_received_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_arg_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_key_chain_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_tags_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_args_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_linkstate_id_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_any_any_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_soft_in_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_default_metric_val_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_mask_flags2_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_name_expanded_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_detail_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_send_community_type_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_route_map_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbors_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community4_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_soft_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_day_month_month_day_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged10_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_nssa_no_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_cidr_only_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_linkstate_network_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_address_cmd_vtysh);
  install_element (VTY_NODE, &vty_login_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_summary_cmd_vtysh);
  install_element (VTY_NODE, &vty_ipv6_access_class_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_med3_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_flap_route_map_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_simulate_spf_tree_root_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_route_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_soft_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_ipv6_summary_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_day_month_day_month_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_all_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_normal_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_border_routers_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_aspath_exclude_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_out_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_rd_route_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_packet_send_recv_detail_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_pathlimit_ttl_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_extcommunity_list_arg_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_metric_type_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_err_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_asbr_cmd_vtysh);
  install_element (VIEW_NODE, &ipv6_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_message_digest_key_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_neighbor_detail_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_remove_private_as_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_warning_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community_all_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged9_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_self_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_cost_u32_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_ipv6_bgp_network_ttl_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_receive_version_2_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_export_list_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_param3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_rip_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_mask_as_set_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_aggregate_address_as_set_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_zebra_kernel_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_duration_month_day_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_route_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_spftrigg_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_standard2_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community3_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_as4_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_expanded_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_med2_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_passive_interface_default_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_regexp_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_default_information_originate_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_authentication_key_addr_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_seq_le_ge_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_neighbors_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_longer_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_brouter_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_address_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_default_information_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_as_soft_in_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_pref_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_packet_all_cmd_vtysh);
  install_element (ISIS_NODE, &domain_passwd_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_default_originate_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_neighbors_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_prefix_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_rtevents_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community4_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_scan_cmd_vtysh);
  install_element (INTERFACE_NODE, &mpls_te_link_max_rsv_bw_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_regexp_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_keepalive_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_neighbor_damp_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_rip_packet_direct_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_bgp_as4_segment_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_filter_list_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_enforce_multihop_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_routemap_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_extended_mask_host_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_received_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_name_expanded_all_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_med_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_authkey_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_rip_zebra_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community3_exact_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_external_intra_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_priority_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_self_originated_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_statistics_cmd_vtysh);
  install_element (RMAP_NODE, &set_ipv6_nexthop_global_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_in_prefix_filter_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_received_routes_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_default_originate_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_routes_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_extcommunity_list_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_longer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_ifname_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_local_as_val_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_zebra_sub_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_soft_out_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_flags_pref_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_as_set_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_ipv4_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_soft_in_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ripng_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (BGP_NODE, &bgp_damp_unset_cmd_vtysh);
  install_element (CONFIG_NODE, &no_dump_bgp_updates_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_standard_host_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_events_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community4_exact_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_aspath_confed_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_community_list_expanded_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_dead_interval_addr_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_access_list_remark_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_soft_in_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_ipv4_out_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_route_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_lsa_sub_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ipv6_prefix_list_name_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_events_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ripng_status_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_route_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_lsa_hex_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_as_path_access_list_all_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_host_mask_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_summary_only_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ecommunity_val_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_soft_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (KEYCHAIN_NODE, &no_key_chain_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_route_mask_flags_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_retransmitinterval_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_self_originated_linkstate_id_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_info_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_event_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_snp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_all_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_debug_misc_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_filter_list_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_linkstate_id_detail_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_soft_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_authentication_mode_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_port_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_group_soft_in_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_network_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_seq_le_cmd_vtysh);
  install_element (BGP_NODE, &bgp_confederation_peers_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community3_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_rsclient_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_isis_adj_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ripng_zebra_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_cost_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_id_router_detail_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_metric_type_routemap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_ipv6_aggregate_address_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_view_prefix_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged3_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_cmd_vtysh);
  install_element (RIP_NODE, &no_distribute_list_prefix_all_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_enforce_first_as_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_prefix_list_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_int_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_distance_cmd_vtysh);
  install_element (BGP_NODE, &old_ipv6_bgp_network_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_backdoor_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_standard_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_med_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_default_originate_rmap_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_ipv6_rsclient_summary_cmd_vtysh);
  install_element (ZEBRA_NODE, &no_redistribute_ospf6_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_standard_all_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_keepalive_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_damp_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_dampening_address_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_as4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_filter_list_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_bestpath_aspath_ignore_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_external_in_prefix_filter_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community2_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_authentication_string_cmd_vtysh);
  install_element (RMAP_NODE, &set_ip_nexthop_peer_cmd_vtysh);
  install_element (VIEW_NODE, &show_debugging_ripng_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_neighbor_advertised_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_id_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_natural_backdoor_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_seq_ge_le_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_attr_unchanged2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community_exact_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_infinite_month_day_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_forwarding_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_originator_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_flap_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged1_cmd_vtysh);
  install_element (VIEW_NODE, &show_version_ospf6_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_inter_external_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_statistics_view_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_remark_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &aggregate_address_mask_as_set_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_router_isis_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_ipv6_nexthop_global_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_abr_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_passive_interface_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_adv_router_linkstate_id_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_standard_nomask_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_passive_interface_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_bgp_neighbor_advertised_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_multiplier_l1_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_events_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_metric_val_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_zebra_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_community4_exact_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_interface_ifname_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_attr_info_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_neighbors_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbor_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_spf_time_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_prefix_list_cmd_vtysh);
  install_element (RIPNG_NODE, &if_ipv6_rmap_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (RIP_NODE, &rip_offset_list_ifname_cmd_vtysh);
  install_element (OSPF_NODE, &mpls_te_on_cmd_vtysh);
  install_element (RIPNG_NODE, &ipv6_distribute_list_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_mask_as_set_cmd_vtysh);
  install_element (CONFIG_NODE, &no_router_bgp_view_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_default_ipv4_unicast_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_rip_events_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_route_map_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_dont_capability_negotiate_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_flags_distance_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_neighbor_advertised_route_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_community4_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_clns_neighbors_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community_list_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_rsclient_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &send_lifetime_infinite_month_day_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_advertise_interval_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_linkdetect_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &no_key_string_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_ipv4_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_zebra_sendrecv_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_prefix_longer_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (RMAP_NODE, &set_origin_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_network_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_inter_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ripng_zebra_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_timers_connect_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_priority_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_nsm_sub_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged10_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_name_standard_all_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_rsclient_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_priority_l1_cmd_vtysh);
  install_element (INTERFACE_NODE, &ospf_priority_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_bandwidth_if_val_cmd_vtysh);
  install_element (RIPNG_NODE, &ripng_offset_list_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &aggregate_address_as_set_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_authentication_key_cmd_vtysh);
  install_element (CONFIG_NODE, &no_bgp_multiple_instance_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_peer_group_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_damp_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_log_adjacency_changes_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_zebra_cmd_vtysh);
  install_element (RMAP_NODE, &ospf6_routemap_no_set_forwarding_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_interface_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_route_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_timers_throttle_spf_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_multicast_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_linkstate_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_prefix_longer_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_mask_route_map_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_community2_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_view_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_adv_router_linkstate_id_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_self_originated_linkstate_id_detail_cmd_vtysh);
  install_element (ISIS_NODE, &lsp_lifetime_l2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_route_map_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_soft_cmd_vtysh);
  install_element (RIP_NODE, &distribute_list_all_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_seq_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_next_hop_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_out_cmd_vtysh);
  install_element (RIP_NODE, &distribute_list_prefix_all_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_zebra_sendrecv_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_routemap_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_authentication_key_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_soft_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_hello_interval_addr_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_metric_type_routemap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community4_exact_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_summary_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_rsclient_summary_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_forwarding_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_default_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_isis_neighbors_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (CONFIG_NODE, &no_dump_bgp_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_access_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_ge_le_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_ism_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_summary_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_instance_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_ifmtu_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_nexthop_self_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_mtu_ignore_addr_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_remove_private_as_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_community4_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_param1_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged6_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_metric_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_group_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_prefix_list_sequence_number_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_asbr_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_ge_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_brouter_router_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_ifname_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community_cmd_vtysh);
  install_element (BGP_NODE, &aggregate_address_mask_as_set_summary_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_cost_inet4_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_default_originate_rmap_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_all_soft_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_rsclient_summary_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged5_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_regexp_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_prefix_list_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_summary_name_cmd_vtysh);
  install_element (INTERFACE_NODE, &mpls_te_link_maxbw_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_self_originated_detail_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_authentication_args_addr_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_rip_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_natural_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_in_prefix_filter_cmd_vtysh);
  install_element (OSPF6_NODE, &area_import_list_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_client_to_client_reflection_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ripng_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_dead_interval_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbor_received_routes_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_rsclient_summary_cmd_vtysh);
  install_element (BGP_NODE, &bgp_confederation_identifier_cmd_vtysh);
  install_element (RIPNG_NODE, &ipv6_distribute_list_prefix_all_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_weight_val_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_ipv6_neighbors_peer_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_pathlimit_as_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_spfstats_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_authentication_addr_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_rip_receive_version_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_all_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &access_list_standard_any_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_route_match_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_rib_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_priority_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community3_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_soft_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_natural_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_neighbor_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_instance_all_soft_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_in_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_zebra_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_neighbor_flap_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_spfstats_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_brouter_area_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_neighbors_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_zebra_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_peer_group_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_packet_all_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_seq_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_prefix_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_attr_unchanged1_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_backdoor_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_in_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_route_map_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_spfevents_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_adv_router_detail_cmd_vtysh);
  install_element (BGP_NODE, &bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_set_peer_group_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_as_set_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_mask_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_packet_all_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_default_information_originate_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_timers_val_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_all_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_rsclient_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_type_id_self_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_neighbors_peer_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_max_metric_router_lsa_admin_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_all_rsclient_cmd_vtysh);
  install_element (VIEW_NODE, &show_database_detail_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_l1_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_brouter_area_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_prefix_list_cmd_vtysh);
  install_element (RIP_NODE, &rip_neighbor_cmd_vtysh);
  install_element (RMAP_NODE, &no_rmap_call_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_id_self_originated_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_rd_neighbor_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_isis_csum_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_neighbor_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_regexp_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_id_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_soft_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_ge_cmd_vtysh);
  install_element (RMAP_NODE, &set_community_delete_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_rsclient_summary_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &bgp_redistribute_ipv6_rmap_metric_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_args_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_address_prefix_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbor_advertised_route_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_instance_neighbors_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_ifname_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_pref_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_database_cmd_vtysh);
  install_element (RIP_NODE, &if_rmap_cmd_vtysh);
  install_element (ENABLE_NODE, &show_zebra_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ripng_events_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_address_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_neighbor_detail_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_route_source_prefix_list_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_attr_unchanged7_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_ifname_pref_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_community4_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_soft_in_cmd_vtysh);
  install_element (RMAP_NODE, &match_community_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_detail_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_router_detail_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ripng_packet_direct_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_peer_out_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &bgp_redistribute_ipv6_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_external_cmd_vtysh);
  install_element (RMAP_NODE, &no_rmap_continue_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_aggregate_address_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_ifname_flags_pref_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_name_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_spf_tree_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_route_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_in_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_out_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_weight_cmd_vtysh);
  install_element (RIP_NODE, &rip_redistribute_type_metric_routemap_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_isis_err_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_ipv6_neighbor_flap_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (RIP_NODE, &distribute_list_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_attr_unchanged3_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_prefix_longer_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_route_map_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_route_reflector_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf_packet_send_recv_detail_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (INTERFACE_NODE, &isis_hello_interval_l1_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_l2_arg_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_nexthop_self_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_route_flags_distance2_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_maximum_prefix_threshold_restart_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_summary_only_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_cidr_only_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_l2_arg_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &bgp_network_mask_ttl_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_neighbors_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_ipv6_aggregate_address_summary_only_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_in_prefix_filter_cmd_vtysh);
  install_element (CONFIG_NODE, &ipv6_access_list_any_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_bgp_neighbors_peer_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &vpnv4_network_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_network_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_maximum_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ripng_packet_direct_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_offset_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_self_originated_detail_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_metric_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_standard_any_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_neighbor_poll_interval_cmd_vtysh);
  install_element (BGP_NODE, &bgp_bestpath_aspath_ignore_cmd_vtysh);
  install_element (VTY_NODE, &no_vty_login_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_route_addr_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_advertise_interval_val_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_neighbor_advertised_routes_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_prefix_detail_cmd_vtysh);
  install_element (OSPF6_NODE, &ospf6_router_id_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_always_type_routemap_cmd_vtysh);
  install_element (BGP_NODE, &bgp_router_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_route_server_client_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_extcommunity_list_name_expanded_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_activate_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_extcommunity_list_name_standard_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_route_map_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_ip_route_source_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_as_path_access_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_group_ipv4_in_prefix_filter_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_capability_orf_prefix_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_route_map_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_exact_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_interface_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_interface_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_prefix_counts_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_community_info_cmd_vtysh);
  install_element (ZEBRA_NODE, &rip_redistribute_rip_cmd_vtysh);
  install_element (RMAP_NODE, &match_ecommunity_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_statistics_vpnv4_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_id_router_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_summary_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_community_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_isis_topology_l1_cmd_vtysh);
  install_element (RMAP_NODE, &match_ip_route_source_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_ipv4_out_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_cost_u32_inet4_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_route_ifname_flags_cmd_vtysh);
  install_element (RMAP_NODE, &match_origin_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_isis_err_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf_packet_send_recv_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_type_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_distance_source_access_list_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_backdoor_ttl_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_passive_interface_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_attr_unchanged7_cmd_vtysh);
  install_element (ISIS_NODE, &no_lsp_lifetime_l2_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_int_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_view_prefix_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_mbgp_community4_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_soft_in_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_community_delete_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_self_originated_linkstate_id_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community_list_exact_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_version_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_tag_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_type_detail_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_protocol_cmd_vtysh);
  install_element (CONFIG_NODE, &service_advanced_vty_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_md5_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_access_list_cmd_vtysh);
  install_element (OSPF6_NODE, &area_range_advertise_cmd_vtysh);
  install_element (RMAP_NODE, &set_ecommunity_rt_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_update_source_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_src_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_ospf6_message_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_threshold_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_prefix_longer_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ipv6_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_message_sendrecv_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_hello_interval_l1_arg_cmd_vtysh);
  install_element (RIP_NODE, &no_if_rmap_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_default_originate_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community2_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_zebra_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_dead_interval_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_zebra_rib_q_cmd_vtysh);
  install_element (CONFIG_NODE, &no_service_advanced_vty_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_brouter_area_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_bandwidth_if_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_as_vpnv4_soft_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_vpnv4_all_neighbor_advertised_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_flooding_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_local_pref_cmd_vtysh);
  install_element (ISIS_NODE, &no_spf_interval_l2_arg_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_database_type_linkstate_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community2_exact_cmd_vtysh);
  install_element (RIP_NODE, &no_distribute_list_all_cmd_vtysh);
  install_element (CONFIG_NODE, &ip_prefix_list_seq_ge_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_peer_local_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_vpnv4_all_neighbors_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community2_exact_cmd_vtysh);
  install_element (ISIS_NODE, &no_net_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf6_neighbor_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_mask_summary_only_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ospf_network_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_community_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_instance_all_rsclient_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_rip_authentication_mode_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_bgp_network_mask_route_map_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &no_neighbor_distribute_list_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_allowas_in_arg_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf_nsm_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_irdp_preference_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_max_metric_router_lsa_shutdown_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_attr_unchanged9_cmd_vtysh);
  install_element (RIP_NODE, &no_rip_offset_list_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_range_substitute_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &bgp_redistribute_ipv6_rmap_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_name_expanded_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_attr_unchanged6_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_opaque_capable_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbor_received_routes_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_community_list_arg_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_allowas_in_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_as_in_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_prefix_list_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distance_ospf_intra_external_inter_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged8_cmd_vtysh);
  install_element (INTERFACE_NODE, &ip_ospf_mtu_ignore_addr_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_community3_cmd_vtysh);
  install_element (RIPNG_NODE, &no_ripng_redistribute_type_metric_routemap_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_timers_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_peer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_route_addr_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &no_neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_prefix_list_summary_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community4_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_bgp_ipv6_all_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_ospf_lsa_sub_cmd_vtysh);
  install_element (ENABLE_NODE, &show_clns_neighbors_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_prefix_list_seq_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_override_capability_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_restart_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_neighbor_routes_cmd_vtysh);
  install_element (ENABLE_NODE, &show_mpls_te_router_cmd_vtysh);
  install_element (ENABLE_NODE, &undebug_bgp_events_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_as_set_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_send_community_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_external_ipv4_in_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_network_area_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_distribute_list_out_cmd_vtysh);
  install_element (ENABLE_NODE, &ipv6_mbgp_neighbor_received_routes_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &no_key_chain_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_rsclient_prefix_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_ebgp_multihop_cmd_vtysh);
  install_element (KEYCHAIN_KEY_NODE, &accept_lifetime_day_month_month_day_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_redistribute_source_type_routemap_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community4_exact_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_neighbor_unsuppress_map_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &no_bgp_network_mask_route_map_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_isis_metric_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_protocol_cmd_vtysh);
  install_element (RMAP_NODE, &no_set_vpnv4_nexthop_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_route_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_zebra_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_linkstate_network_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_dampened_paths_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_ospf6_database_id_router_cmd_vtysh);
  install_element (OSPF_NODE, &mpls_te_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_ipv4_community2_exact_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_community_list_exact_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_default_information_originate_type_metric_routemap_cmd_vtysh);
  install_element (CONFIG_NODE, &no_debug_ospf6_route_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_ip_ospf_cost_u32_inet4_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_bgp_community_exact_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_capability_dynamic_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ip_community_list_expanded_all_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_regexp_cmd_vtysh);
  install_element (BGP_NODE, &bgp_graceful_restart_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_extended_mask_any_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_md5_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_authtype_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_all_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_ospf_neighbor_id_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_prefix_list_prefix_longer_cmd_vtysh);
  install_element (ENABLE_NODE, &show_isis_topology_cmd_vtysh);
  install_element (OSPF_NODE, &no_ospf_area_vlink_authtype_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_rip_packet_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_neighbors_peer_cmd_vtysh);
  install_element (ISIS_NODE, &is_type_cmd_vtysh);
  install_element (BGP_NODE, &no_neighbor_filter_list_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_peer_vpnv4_out_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_bgp_regexp_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_continue_cmd_vtysh);
  install_element (BGP_NODE, &no_bgp_network_mask_natural_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_route_cmd_vtysh);
  install_element (OSPF6_NODE, &area_range_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_route_cmd_vtysh);
  install_element (RMAP_NODE, &set_pathlimit_ttl_cmd_vtysh);
  install_element (OSPF6_NODE, &ospf6_redistribute_routemap_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_ipv6_neighbor_advertised_route_cmd_vtysh);
  install_element (RIP_NODE, &rip_distance_cmd_vtysh);
  install_element (BGP_VPNV4_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_summary_cmd_vtysh);
  install_element (CONFIG_NODE, &no_access_list_standard_nomask_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_soft_out_cmd_vtysh);
  install_element (ENABLE_NODE, &no_debug_bgp_fsm_cmd_vtysh);
  install_element (RESTRICTED_NODE, &show_ip_bgp_ipv4_community4_exact_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &no_neighbor_attr_unchanged2_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_ipv6_community_all_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_all_ipv4_soft_in_cmd_vtysh);
  install_element (INTERFACE_NODE, &ipv6_ospf6_advertise_prefix_list_cmd_vtysh);
  install_element (RMAP_NODE, &no_match_metric_cmd_vtysh);
  install_element (CONFIG_NODE, &no_key_chain_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_flap_route_map_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_remote_as_cmd_vtysh);
  install_element (BGP_IPV6M_NODE, &neighbor_attr_unchanged5_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_neighbors_peer_cmd_vtysh);
  install_element (CONFIG_NODE, &no_ipv6_access_list_remark_arg_cmd_vtysh);
  install_element (ENABLE_NODE, &show_bgp_instance_ipv6_rsclient_summary_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &neighbor_soft_reconfiguration_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &neighbor_maximum_prefix_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_brouter_area_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_bgp_events_cmd_vtysh);
  install_element (VIEW_NODE, &show_bgp_view_ipv6_neighbor_routes_cmd_vtysh);
  install_element (BGP_NODE, &neighbor_distribute_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_route_supernets_cmd_vtysh);
  install_element (RMAP_NODE, &set_ip_nexthop_cmd_vtysh);
  install_element (ENABLE_NODE, &clear_ip_bgp_dampening_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_bgp_events_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_longer_detail_cmd_vtysh);
  install_element (RMAP_NODE, &set_originator_id_cmd_vtysh);
  install_element (RMAP_NODE, &set_local_pref_cmd_vtysh);
  install_element (BGP_NODE, &no_aggregate_address_mask_summary_as_set_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_ospf6_route_type_cmd_vtysh);
  install_element (ENABLE_NODE, &debug_rip_packet_direct_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_as_path_access_list_all_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_neighbor_poll_interval_priority_cmd_vtysh);
  install_element (OSPF_NODE, &no_mpls_te_cmd_vtysh);
  install_element (BGP_IPV6_NODE, &bgp_redistribute_ipv6_metric_rmap_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_bgp_ipv4_cidr_only_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_instance_ipv4_rsclient_summary_cmd_vtysh);
  install_element (RMAP_NODE, &no_rmap_description_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_bgp_vpnv4_rd_neighbors_cmd_vtysh);
  install_element (INTERFACE_NODE, &no_csnp_interval_l2_cmd_vtysh);
  install_element (CONFIG_NODE, &debug_ospf6_spf_database_cmd_vtysh);
  install_element (VIEW_NODE, &show_ip_ospf_database_type_id_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ip_extcommunity_list_cmd_vtysh);
  install_element (ENABLE_NODE, &show_ipv6_mbgp_prefix_cmd_vtysh);
  install_element (BGP_IPV4M_NODE, &no_neighbor_maximum_prefix_threshold_warning_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &bgp_network_mask_ttl_cmd_vtysh);
  install_element (VIEW_NODE, &show_ipv6_route_prefix_cmd_vtysh);
  install_element (BGP_IPV4_NODE, &neighbor_send_community_cmd_vtysh);
  install_element (OSPF_NODE, &ospf_area_vlink_param2_cmd_vtysh);
  install_element (RMAP_NODE, &rmap_call_cmd_vtysh);
}
