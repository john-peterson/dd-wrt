<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">document.write("<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=" + lang_charset.set + "\" />")</script>
		<title><% nvram_get("router_name"); %> - Setup</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + idx.titl;

var wan_proto = "<% nvram_get("wan_proto"); %>";
var dhcp_win = null;

function pptpUseDHCP(F, value)
{
    if(value == "0")
    {
		choose_enable(F.wan_ipaddr_0);
		choose_enable(F.wan_ipaddr_1);
		choose_enable(F.wan_ipaddr_2);
		choose_enable(F.wan_ipaddr_3);
		choose_enable(F.wan_netmask_0);
		choose_enable(F.wan_netmask_1);
		choose_enable(F.wan_netmask_2);
		choose_enable(F.wan_netmask_3);
	} else {
		choose_disable(F.wan_ipaddr_0);
		choose_disable(F.wan_ipaddr_1);
		choose_disable(F.wan_ipaddr_2);
		choose_disable(F.wan_ipaddr_3);
		choose_disable(F.wan_netmask_0);
		choose_disable(F.wan_netmask_1);
		choose_disable(F.wan_netmask_2);
		choose_disable(F.wan_netmask_3);
	}
}

function valid_mtu(I)
{
	var start = null;
	var end = null;
	if(wan_proto == "pppoe")
	{
		start = 576;
		end = 1492;
	} else if (wan_proto == "pptp" || wan_proto == "l2tp")
	{
		start = 1200;
        end = 1400;
	} else {
		start = 576;
		end = 1500;
	}

	valid_range(I,start,end,"MTU");
}

function SelMTU(num,F)
{
	mtu_enable_disable(F,num);
}

function mtu_enable_disable(F,I)
{
	if ( I == "0" )
	{
		choose_disable(F.wan_mtu);
	} else {
		choose_enable(F.wan_mtu);
	}
}

function SelWAN(num,F)
{
	F.submit_button.value = "index";
	F.submit_type.value = "wan_proto";
	F.change_action.value = "gozila_cgi";
	F.wan_proto.value=F.wan_proto.options[num].value;
	F.submit();
}

function SelPPP(num,F)
{
	F.submit_button.value = "index";
	F.change_action.value = "gozila_cgi";
	F.mpppoe_enable.value = F.mpppoe_enable.options[num].value;
	F.submit();
}

function SelDHCPFWD(num,F)
{
	F.submit_button.value = "index";
	F.submit_type.value = "dhcpfwd";
	F.change_action.value = "gozila_cgi";
	F.dhcpfwd_enable.value = F.dhcpfwd_enable.options[num].value;
	F.submit();
}

function to_submit(F) 
{
	if(valid_value(F))
	{
		if(F._daylight_time.checked == false)
		{
			F.daylight_time.value = 0;
		} else {
			F.daylight_time.value = 1;
		}
		
		if(F._dhcp_dnsmasq)
		{
			if(F._dhcp_dnsmasq.checked == false)
			{
				F.dhcp_dnsmasq.value = 0;
			} else {
				F.dhcp_dnsmasq.value = 1;
			}
		}

		if(F._dns_dnsmasq)
		{
			if(F._dns_dnsmasq.checked == false)
			{
				F.dns_dnsmasq.value = 0;
			} else {
				F.dns_dnsmasq.value = 1;
			}
		}

		if(F._auth_dnsmasq)
		{
			if(F._auth_dnsmasq.checked == false)
			{
				F.auth_dnsmasq.value = 0;
			} else {
				F.auth_dnsmasq.value = 1;
			}
		}
		
		if(F._fullswitch)
		{
			if(F._fullswitch.checked == false)
			{
				F.fullswitch.value = 0;
			} else {
				F.fullswitch.value = 1;
			}
		}
		
		F.submit_button.value = "index";
//		F.save_button.value = "Saved";
		F.save_button.value = sbutton.saving;

		F.action.value = "Apply";
		apply(F);
	}
}

function valid_value(F)
{
	if (F.now_proto.value == "pptp" || F.now_proto.value == "static")
	{
		pptp_dhcp = "";

		// Sveasoft: allow 0.0.0.0 for pptp. We'll let DHCP take care of it.
		if (F.now_proto.value == "pptp" &&
		    F.wan_ipaddr_0.value == "0" && 
		    F.wan_ipaddr_1.value == "0" && 
		    F.wan_ipaddr_2.value == "0" && 
		    F.wan_ipaddr_3.value == "0")
		    	pptp_dhcp = "skip";
		
		if (!F.pptp_use_dhcp || F.pptp_use_dhcp.value == "0")
		{
			if(pptp_dhcp != "skip" && !valid_ip(F,"F.wan_ipaddr","IP",ZERO_NO|MASK_NO))
				return false;

			if(pptp_dhcp != "skip" && !valid_mask(F,"F.wan_netmask",ZERO_NO|BCST_NO))
				return false;
		}

		if(F.now_proto.value == "static")
		{
			if(!valid_ip(F,"F.wan_gateway","Gateway",ZERO_NO|MASK_NO))
				return false;
		}

		if(pptp_dhcp != "skip" && F.now_proto.value == "pptp")
		{
			if (F.pptp_use_dhcp.value == "0")
			{
				if(!valid_ip(F,"F.pptp_server_ip","Gateway",ZERO_NO|MASK_NO))
					return false;
				
				if(!valid_ip_gw(F,"F.wan_ipaddr","F.wan_netmask","F.pptp_server_ip"))
					return false;
			}
		}
	}
	
	if(F.now_proto.value == "pppoe" || F.now_proto.value == "pptp" || F.now_proto.value == "l2tp")
	{
		if(F.ppp_username.value == "")
		{
//			alert("You must input a username.");
			alert(errmsg.err0);
			F.ppp_username.focus();
			return false;
		}
	}
	<% nvram_selmatch("dhcpfwd_enable", "1", "//"); %>if(!valid_dhcp_server(F))
	<% nvram_selmatch("dhcpfwd_enable", "1", "//"); %>return false;

	if(F.router_name)
	if(F.router_name.value == "")
	{
//		alert("You must input a Router Name.");
		alert(errmsg.err1);
		F.router_name.focus();
		return false;
	}
	if(document.setup)
	if(document.setup.now_proto)
	if(document.setup.now_proto.value == "pptp")
		pptpUseDHCP(document.setup, '<% nvram_get("pptp_use_dhcp"); %>')

	return true;
}

function valid_dhcp_server(F)
{
	if(F.lan_proto == null)
		return true;
	if (F.lan_proto.selectedIndex == 0)
	{
		return true;
	}

	a1 = parseInt(F.dhcp_start.value,10);
	a2 = parseInt(F.dhcp_num.value,10);
	if (a1 + a2 > 255)
	{
//		alert("Out of range, please adjust start IP address or user's numbers.");
		alert(errmsg.err2);
		return false;
	}

	if (F.wan_dns0 != null)
	if (!valid_ip(F,"F.wan_dns0","DNS",MASK_NO))
	{
		return false;
	}
	
	if (F.wan_dns1 != null)
	if (!valid_ip(F,"F.wan_dns1","DNS",MASK_NO))
	{
		return false;
	}
	
	if (F.wan_dns2 != null)
	if (!valid_ip(F,"F.wan_dns2","DNS",MASK_NO))
	{
		return false;
	}
	
	if (F.wan_wins != null)
	if (!valid_ip(F,"F.wan_wins","WINS",MASK_NO))
	{
		return false;
	}

	return true;
}

function SelDHCP(T,F)
{
	dhcp_enable_disable(F,T);
}

function dhcp_enable_disable(F,T)
{
	var start = '';
	var end = '';
 	var total = F.elements.length;
	for(i=0 ; i < total ; i++)
	{
		if(F.elements[i].name == "dhcp_start")
			start = i;
		if(F.elements[i].name == "wan_wins_3")
			end = i;
	}
	if(start == '' || end == '')
		return true;

	if( T == "static" ) 
	{
		for(i = start; i<=end ;i++)
		{
			choose_disable(F.elements[i]);
		}
	} else {
		for(i = start; i<=end ;i++)
		{
			choose_enable(F.elements[i]);
		}
	}
}

function SelTime(num,f)
{
	aaa = f.time_zone.options[num].value.charAt(4);
	daylight_enable_disable(f,aaa);
}

function ppp_enable_disable(F,I)
{
	if( I == "0")
	{
		choose_disable(F.ppp_idletime);
		choose_enable(F.ppp_redialperiod);
	} else {
		choose_enable(F.ppp_idletime);
		choose_disable(F.ppp_redialperiod);
	}
}

function daylight_enable_disable(F,aaa)
{
	if(aaa == 0) {
		F._daylight_time.checked = false;
		choose_disable(F._daylight_time);
		F.daylight_time.value = 0;
	} else {
		choose_enable(F._daylight_time);
		F._daylight_time.checked = true;
		F.daylight_time.value = 1;
	}
}

function init()
{
	mtu_enable_disable(document.setup,'<% nvram_get("mtu_enable"); %>');
	aaa = document.setup.time_zone.options[document.setup.time_zone.selectedIndex].value.charAt(4);
	if (aaa == 0)
	{
		document.setup._daylight_time.checked = false;
		choose_disable(document.setup._daylight_time);
		document.setup.daylight_time.value = 0;
	}

	if (document.setup.now_proto.value == "pppoe" ||
		document.setup.now_proto.value == "pptp" ||
		document.setup.now_proto.value == "l2tp" ||
		document.setup.now_proto.value == "heartbeat")
			ppp_enable_disable(document.setup,'<% nvram_get("ppp_demand"); %>');

	dhcp_enable_disable(document.setup,'<% nvram_get("lan_proto"); %>');
}
		</script>
	</head>

	<body class="gui" onload="init();">
	<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<div id="menu">
						<div id="menuMain">
							<ul id="menuMainList">
								<li class="current"><span><script type="text/javascript">Capture(bmenu.setup)</script></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><span><script type="text/javascript">Capture(bmenu.setupbasic)</script></span></li>
											<li><a href="DDNS.asp"><script type="text/javascript">Capture(bmenu.setupddns)</script></a></li>
											<li><a href="WanMAC.asp"><script type="text/javascript">Capture(bmenu.setupmacclone)</script></a></li>
	  										<li><a href="Routing.asp"><script type="text/javascript">Capture(bmenu.setuprouting)</script></a></li>
	  										<li><a href="Vlan.asp"><script type="text/javascript">Capture(bmenu.setupvlan)</script></a></li>
  										</ul>
  									</div>
  								</li>
  								<li><a href="Wireless_Basic.asp"><script type="text/javascript">Capture(bmenu.wireless)</script></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><script type="text/javascript">Capture(bmenu.sipath)</script></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><script type="text/javascript">Capture(bmenu.security)</script></a></li>
								<li><a href="Filters.asp"><script type="text/javascript">Capture(bmenu.accrestriction)</script></a></li>
								<li><a href="Forward.asp"><script type="text/javascript">Capture(bmenu.applications)</script></a></li>
								<li><a href="Management.asp"><script type="text/javascript">Capture(bmenu.admin)</script></a></li>
								<li><a href="Status_Router.asp"><script type="text/javascript">Capture(bmenu.statu)</script></a></li>
							</ul>
						</div>
					</div>
				</div>
				<div id="main">
					<div id="contents">
						<form name="setup" action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="action" />
							<input type="hidden" name="now_proto" value="<% nvram_gozila_get("wan_proto"); %>" />
							<input type="hidden" name="dhcp_dnsmasq" value="0" />
							<input type="hidden" name="fullswitch" value="0" />
							<input type="hidden" name="daylight_time" value="0" />
							<input type="hidden" name="lan_ipaddr" value="4" />
							<% nvram_match("wl_mode", "wet", "<!--"); %>
							<h2><% nvram_else_match("wl_mode", "ap", "<script type="text/javascript">Capture(idx.h2)</script>", "<script type="text/javascript">Capture(idx.h22)</script>"); %></h2>
							<fieldset>
								<legend><script type="text/javascript">Capture(idx.legend)</script></legend>
								<div class="setting">
							    	<div class="label"><script type="text/javascript">Capture(idx.conn_type)</script></div>
							    	<select name="wan_proto" onchange="SelWAN(this.form.wan_proto.selectedIndex,this.form)">
									<% show_connectiontype(); %>
									</select>
								</div>
								<% show_index_setting(); %>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(idx.stp)</script></div>
									<input class="spaceradio" type="radio" value="1" name="lan_stp" <% nvram_selmatch("lan_stp", "1", "checked"); %> /><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" value="0" name="lan_stp" <% nvram_selmatch("lan_stp", "0", "checked"); %> /><script type="text/javascript">Capture(share.disable)</script>
									<span class="default"><script type="text/javascript">Capture(idx.stp_mess)</script></span>
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(idx.optional)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.routername)</script></div>
									<input maxlength="39" name="router_name" size="20" onblur="valid_name(this,&#34;Router%20Name&#34;)" value="<% nvram_get("router_name"); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.hostname)</script></div>
									<input maxlength="39" name="wan_hostname" size="20" onblur="valid_name(this,&#34;Host%20Name&#34;)" value="<% nvram_get("wan_hostname"); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.domainname)</script></div>
									<input maxlength="79" name="wan_domain" size="20" onblur="valid_name(this,&#34;Domain%20name&#34;,SPACE_NO)" value="<% nvram_get("wan_domain"); %>" />
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(idx.mtu)</script></div>
									<select name="mtu_enable" onchange="SelMTU(this.form.mtu_enable.selectedIndex,this.form)">
										<option value="0" <% nvram_selmatch("mtu_enable", "0", "selected"); %>>Auto</option>
										<script type="text/javascript">document.write("<option value=\"1\" <% nvram_selmatch("mtu_enable", "1", "selected"); %> >" + share.manual + "</option>");</script>
									</select>&nbsp;
									<input class="num" maxlength="4" onblur="valid_mtu(this)" size="5" name="wan_mtu" value="<% nvram_get("wan_mtu"); %>" />
								</div>
							</fieldset><br />
							<% nvram_match("wl_mode", "wet", "-->"); %>
							
							<h2><script type="text/javascript">Capture(idx.h23)</script></h2>
							<fieldset>
								<legend><script type="text/javascript">Capture(idx.routerip)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(idx.lanip)</script></div>
									<input class="num" maxlength="3" size="3" onblur="valid_range(this,1,223,'IP')" name="lan_ipaddr_0" value="<% get_single_ip("lan_ipaddr","0"); %>"/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,0,255,'IP')" name="lan_ipaddr_1" value="<% get_single_ip("lan_ipaddr","1"); %>"/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,0,255,'IP')" name="lan_ipaddr_2" value="<% get_single_ip("lan_ipaddr","2"); %>"/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,1,254,'IP')" name="lan_ipaddr_3" value="<% get_single_ip("lan_ipaddr","3"); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.subnet)</script></div>
									<input type="hidden" name="lan_netmask" value="4" />
									<input class="num" maxlength="3" size="3" name="lan_netmask_0" onblur="valid_range(this,0,255,'Netmask')" value="<% get_single_ip("lan_netmask","0"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_netmask_1" onblur="valid_range(this,0,255,'Netmask')" value="<% get_single_ip("lan_netmask","1"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_netmask_2" onblur="valid_range(this,0,255,'Netmask')" value="<% get_single_ip("lan_netmask","2"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_netmask_3" onblur="valid_range(this,0,255,'Netmask')" value="<% get_single_ip("lan_netmask","3"); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.gateway)</script></div>
									<input type="hidden" name="lan_gateway" value="4" />
									<input class="num" maxlength="3" size="3" name="lan_gateway_0" onblur="valid_range(this,0,255,share.gateway)" value="<% get_single_ip("lan_gateway","0"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_gateway_1" onblur="valid_range(this,0,255,share.gateway)" value="<% get_single_ip("lan_gateway","1"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_gateway_2" onblur="valid_range(this,0,255,share.gateway)" value="<% get_single_ip("lan_gateway","2"); %>"/>.<input class="num" maxlength="3" size="3" name="lan_gateway_3" onblur="valid_range(this,0,254,share.gateway)" value="<% get_single_ip("lan_gateway","3"); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.localdns)</script></div>
									<input type="hidden" name="sv_localdns" value="4" />
									<input class="num" maxlength="3" size="3" name="sv_localdns_0" onblur="valid_range(this,0,255,share.localdns)" value="<% get_single_ip("sv_localdns","0"); %>"/>.<input class="num" maxlength="3" size="3" name="sv_localdns_1" onblur="valid_range(this,0,255,share.localdns)" value="<% get_single_ip("sv_localdns","1"); %>"/>.<input class="num" maxlength="3" size="3" name="sv_localdns_2" onblur="valid_range(this,0,255,share.localdns)" value="<% get_single_ip("sv_localdns","2"); %>"/>.<input class="num" maxlength="3" size="3" name="sv_localdns_3" onblur="valid_range(this,0,254,share.localdns)" value="<% get_single_ip("sv_localdns","3"); %>"/>
								</div>
							</fieldset><br />
							
							<% show_wan_to_switch(); %>
							<% show_dhcpd_settings(); %>
							
							<fieldset>
								<legend><script type="text/javascript">Capture(idx.legend3)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(idx.timeset)</script></div>
									<select name="time_zone" onchange="SelTime(this.form.time_zone.selectedIndex,this.form)">
										<option value="-12 1 1" <% nvram_selmatch("time_zone", "-12 1 1", "selected"); %>>UTC-12:00 / none</option>
										<option value="-12 1 2" <% nvram_selmatch("time_zone", "-12 1 2", "selected"); %>>UTC-12:00 / first Sun Apr - last Sun Oct</option>
										<option value="-12 1 3" <% nvram_selmatch("time_zone", "-12 1 3", "selected"); %>>UTC-12:00 / last Sun Mar - last Sun Oct</option>
										<option value="-12 1 4" <% nvram_selmatch("time_zone", "-12 1 4", "selected"); %>>UTC-12:00 / last Sun Oct - last Sun Mar</option>
										<option value="-11 1 1" <% nvram_selmatch("time_zone", "-11 1 1", "selected"); %>>UTC-11:00 / none</option>
										<option value="-11 1 2" <% nvram_selmatch("time_zone", "-11 1 2", "selected"); %>>UTC-11:00 / first Sun Apr - last Sun Oct</option>
										<option value="-11 1 3" <% nvram_selmatch("time_zone", "-11 1 3", "selected"); %>>UTC-11:00 / last Sun Mar - last Sun Oct</option>
										<option value="-11 1 4" <% nvram_selmatch("time_zone", "-11 1 3", "selected"); %>>UTC-11:00 / last Sun Oct - last Sun Mar</option>
										<option value="-10 1 1" <% nvram_selmatch("time_zone", "-10 1 1", "selected"); %>>UTC-10:00 / none</option>
										<option value="-10 1 2" <% nvram_selmatch("time_zone", "-10 1 2", "selected"); %>>UTC-10:00 / first Sun Apr - last Sun Oct</option>
										<option value="-10 1 3" <% nvram_selmatch("time_zone", "-10 1 3", "selected"); %>>UTC-10:00 / last Sun Mar - last Sun Oct</option>
										<option value="-10 1 4" <% nvram_selmatch("time_zone", "-10 1 4", "selected"); %>>UTC-10:00 / last Sun Oct - last Sun Mar</option>
										<option value="-09.5 1 1" <% nvram_selmatch("time_zone", "-09.5 1 1", "selected"); %>>UTC-09:30 / none</option>
										<option value="-09.5 1 2" <% nvram_selmatch("time_zone", "-09.5 1 2", "selected"); %>>UTC-09:30 / first Sun Apr - last Sun Oct</option>
										<option value="-09.5 1 3" <% nvram_selmatch("time_zone", "-09.5 1 3", "selected"); %>>UTC-09:30 / last Sun Mar - last Sun Oct</option>
										<option value="-09.5 1 4" <% nvram_selmatch("time_zone", "-09.5 1 4", "selected"); %>>UTC-09:30 / last Sun Oct - last Sun Mar</option>
										<option value="-09 1 1" <% nvram_selmatch("time_zone", "-09 1 1", "selected"); %>>UTC-09:00 / none</option>
										<option value="-09 1 2" <% nvram_selmatch("time_zone", "-09 1 2", "selected"); %>>UTC-09:00 / first Sun Apr - last Sun Oct</option>
										<option value="-09 1 3" <% nvram_selmatch("time_zone", "-09 1 3", "selected"); %>>UTC-09:00 / last Sun Mar - last Sun Oct</option>
										<option value="-09 1 4" <% nvram_selmatch("time_zone", "-09 1 4", "selected"); %>>UTC-09:00 / last Sun Oct - last Sun Mar</option>
										<option value="-08 1 1" <% nvram_selmatch("time_zone", "-08 1 1", "selected"); %>>UTC-08:00 / none</option>
										<option value="-08 1 2" <% nvram_selmatch("time_zone", "-08 1 2", "selected"); %>>UTC-08:00 / first Sun Apr - last Sun Oct</option>
										<option value="-08 1 3" <% nvram_selmatch("time_zone", "-08 1 3", "selected"); %>>UTC-08:00 / last Sun Mar - last Sun Oct</option>
										<option value="-08 1 4" <% nvram_selmatch("time_zone", "-08 1 4", "selected"); %>>UTC-08:00 / last Sun Oct - last Sun Mar</option>
										<option value="-07 1 1" <% nvram_selmatch("time_zone", "-07 1 1", "selected"); %>>UTC-07:00 / none</option>
										<option value="-07 1 2" <% nvram_selmatch("time_zone", "-07 1 2", "selected"); %>>UTC-07:00 / first Sun Apr - last Sun Oct</option>
										<option value="-07 1 3" <% nvram_selmatch("time_zone", "-07 1 3", "selected"); %>>UTC-07:00 / last Sun Mar - last Sun Oct</option>
										<option value="-07 1 4" <% nvram_selmatch("time_zone", "-07 1 4", "selected"); %>>UTC-07:00 / last Sun Oct - last Sun Mar</option>
										<option value="-06 1 1" <% nvram_selmatch("time_zone", "-06 1 1", "selected"); %>>UTC-06:00 / none</option>
										<option value="-06 1 2" <% nvram_selmatch("time_zone", "-06 1 2", "selected"); %>>UTC-06:00 / first Sun Apr - last Sun Oct</option>
										<option value="-06 1 3" <% nvram_selmatch("time_zone", "-06 1 3", "selected"); %>>UTC-06:00 / last Sun Mar - last Sun Oct</option>
										<option value="-06 1 4" <% nvram_selmatch("time_zone", "-06 1 4", "selected"); %>>UTC-06:00 / last Sun Oct - last Sun Mar</option>
										<option value="-05 1 1" <% nvram_selmatch("time_zone", "-05 1 1", "selected"); %>>UTC-05:00 / none</option>
										<option value="-05 1 2" <% nvram_selmatch("time_zone", "-05 1 2", "selected"); %>>UTC-05:00 / first Sun Apr - last Sun Oct</option>
										<option value="-05 1 3" <% nvram_selmatch("time_zone", "-05 1 3", "selected"); %>>UTC-05:00 / last Sun Mar - last Sun Oct</option>
										<option value="-05 1 4" <% nvram_selmatch("time_zone", "-05 1 4", "selected"); %>>UTC-05:00 / last Sun Oct - last Sun Mar</option>
										<option value="-04 1 1" <% nvram_selmatch("time_zone", "-04 1 1", "selected"); %>>UTC-04:00 / none</option>
										<option value="-04 1 2" <% nvram_selmatch("time_zone", "-04 1 2", "selected"); %>>UTC-04:00 / first Sun Apr - last Sun Oct</option>
										<option value="-04 1 3" <% nvram_selmatch("time_zone", "-04 1 3", "selected"); %>>UTC-04:00 / last Sun Mar - last Sun Oct</option>
										<option value="-04 1 4" <% nvram_selmatch("time_zone", "-04 1 4", "selected"); %>>UTC-04:00 / last Sun Oct - last Sun Mar</option>
										<option value="-03.5 1 1" <% nvram_selmatch("time_zone", "-03.5 1 1", "selected"); %>>UTC-03:30 / none</option>
										<option value="-03.5 1 2" <% nvram_selmatch("time_zone", "-03.5 1 2", "selected"); %>>UTC-03:30 / first Sun Apr - last Sun Oct</option>
										<option value="-03.5 1 3" <% nvram_selmatch("time_zone", "-03.5 1 3", "selected"); %>>UTC-03:30 / last Sun Mar - last Sun Oct</option>
										<option value="-03.5 1 4" <% nvram_selmatch("time_zone", "-03.5 1 4", "selected"); %>>UTC-03:30 / last Sun Oct - last Sun Mar</option>
										<option value="-03 1 1" <% nvram_selmatch("time_zone", "-03 1 1", "selected"); %>>UTC-03:00 / none</option>
										<option value="-03 1 2" <% nvram_selmatch("time_zone", "-03 1 2", "selected"); %>>UTC-03:00 / first Sun Apr - last Sun Oct</option>
										<option value="-03 1 3" <% nvram_selmatch("time_zone", "-03 1 3", "selected"); %>>UTC-03:00 / last Sun Mar - last Sun Oct</option>
										<option value="-03 1 4" <% nvram_selmatch("time_zone", "-03 1 4", "selected"); %>>UTC-03:00 / last Sun Oct - last Sun Mar</option>
										<option value="-02 1 1" <% nvram_selmatch("time_zone", "-02 1 1", "selected"); %>>UTC-02:00 / none</option>
										<option value="-02 1 2" <% nvram_selmatch("time_zone", "-02 1 2", "selected"); %>>UTC-02:00 / first Sun Apr - last Sun Oct</option>
										<option value="-02 1 3" <% nvram_selmatch("time_zone", "-02 1 3", "selected"); %>>UTC-02:00 / last Sun Mar - last Sun Oct</option>
										<option value="-02 1 4" <% nvram_selmatch("time_zone", "-02 1 4", "selected"); %>>UTC-02:00 / last Sun Oct - last Sun Mar</option>
										<option value="-01 1 1" <% nvram_selmatch("time_zone", "-01 1 1", "selected"); %>>UTC-01:00 / none</option>
										<option value="-01 1 2" <% nvram_selmatch("time_zone", "-01 1 2", "selected"); %>>UTC-01:00 / first Sun Apr - last Sun Oct</option>
										<option value="-01 1 3" <% nvram_selmatch("time_zone", "-01 1 3", "selected"); %>>UTC-01:00 / last Sun Mar - last Sun Oct</option>
										<option value="-01 1 4" <% nvram_selmatch("time_zone", "-01 1 4", "selected"); %>>UTC-01:00 / last Sun Oct - last Sun Mar</option>
										<option value="+00 1 1" <% nvram_selmatch("time_zone", "+00 1 1", "selected"); %>>UTC / none</option>
										<option value="+00 1 2" <% nvram_selmatch("time_zone", "+00 1 2", "selected"); %>>UTC / first Sun Apr - last Sun Oct</option>
										<option value="+00 1 3" <% nvram_selmatch("time_zone", "+00 1 3", "selected"); %>>UTC / last Sun Mar - last Sun Oct</option>
										<option value="+00 1 4" <% nvram_selmatch("time_zone", "+00 1 4", "selected"); %>>UTC / last Sun Oct - last Sun Mar</option>
										<option value="+01 1 1" <% nvram_selmatch("time_zone", "+01 1 1", "selected"); %>>UTC+01:00 / none</option>
										<option value="+01 1 2" <% nvram_selmatch("time_zone", "+01 1 2", "selected"); %>>UTC+01:00 / first Sun Apr - last Sun Oct</option>
										<option value="+01 1 3" <% nvram_selmatch("time_zone", "+01 1 3", "selected"); %>>UTC+01:00 / last Sun Mar - last Sun Oct</option>
										<option value="+01 1 4" <% nvram_selmatch("time_zone", "+01 1 4", "selected"); %>>UTC+01:00 / last Sun Oct - last Sun Mar</option>
										<option value="+02 1 1" <% nvram_selmatch("time_zone", "+02 1 1", "selected"); %>>UTC+02:00 / none</option>
										<option value="+02 1 2" <% nvram_selmatch("time_zone", "+02 1 2", "selected"); %>>UTC+02:00 / first Sun Apr - last Sun Oct</option>
										<option value="+02 1 3" <% nvram_selmatch("time_zone", "+02 1 3", "selected"); %>>UTC+02:00 / last Sun Mar - last Sun Oct</option>
										<option value="+02 1 4" <% nvram_selmatch("time_zone", "+02 1 4", "selected"); %>>UTC+02:00 / last Sun Oct - last Sun Mar</option>
										<option value="+03 1 1" <% nvram_selmatch("time_zone", "+03 1 1", "selected"); %>>UTC+03:00 / none</option>
										<option value="+03 1 2" <% nvram_selmatch("time_zone", "+03 1 2", "selected"); %>>UTC+03:00 / first Sun Apr - last Sun Oct</option>
										<option value="+03 1 3" <% nvram_selmatch("time_zone", "+03 1 3", "selected"); %>>UTC+03:00 / last Sun Mar - last Sun Oct</option>
										<option value="+03 1 4" <% nvram_selmatch("time_zone", "+03 1 4", "selected"); %>>UTC+03:00 / last Sun Oct - last Sun Mar</option>
										<option value="+03.5 1 1" <% nvram_selmatch("time_zone", "+03.5 1 1", "selected"); %>>UTC+03:30 / none</option>
										<option value="+03.5 1 2" <% nvram_selmatch("time_zone", "+03.5 1 2", "selected"); %>>UTC+03:30 / first Sun Apr - last Sun Oct</option>
										<option value="+03.5 1 3" <% nvram_selmatch("time_zone", "+03.5 1 3", "selected"); %>>UTC+03:30 / last Sun Mar - last Sun Oct</option>
										<option value="+03.5 1 4" <% nvram_selmatch("time_zone", "+03.5 1 3", "selected"); %>>UTC+03:30 / last Sun Oct - last Sun Mar</option>
										<option value="+04 1 1" <% nvram_selmatch("time_zone", "+04 1 1", "selected"); %>>UTC+04:00 / none</option>
										<option value="+04 1 2" <% nvram_selmatch("time_zone", "+04 1 2", "selected"); %>>UTC+04:00 / first Sun Apr - last Sun Oct</option>
										<option value="+04 1 3" <% nvram_selmatch("time_zone", "+04 1 3", "selected"); %>>UTC+04:00 / last Sun Mar - last Sun Oct</option>
										<option value="+04 1 4" <% nvram_selmatch("time_zone", "+04 1 4", "selected"); %>>UTC+04:00 / last Sun Oct - last Sun Mar</option>
										<option value="+05 1 1" <% nvram_selmatch("time_zone", "+05 1 1", "selected"); %>>UTC+05:00 / none</option>
										<option value="+05 1 2" <% nvram_selmatch("time_zone", "+05 1 2", "selected"); %>>UTC+05:00 / first Sun Apr - last Sun Oct</option>
										<option value="+05 1 3" <% nvram_selmatch("time_zone", "+05 1 3", "selected"); %>>UTC+05:00 / last Sun Mar - last Sun Oct</option>
										<option value="+05 1 4" <% nvram_selmatch("time_zone", "+05 1 4", "selected"); %>>UTC+05:00 / last Sun Oct - last Sun Mar</option>
										<option value="+05.5 1 1" <% nvram_selmatch("time_zone", "+05.5 1 1", "selected"); %>>UTC+05:30 / none</option>
										<option value="+05.5 1 2" <% nvram_selmatch("time_zone", "+05.5 1 2", "selected"); %>>UTC+05:30 / first Sun Apr - last Sun Oct</option>
										<option value="+05.5 1 3" <% nvram_selmatch("time_zone", "+05.5 1 3", "selected"); %>>UTC+05:30 / last Sun Mar - last Sun Oct</option>
										<option value="+05.5 1 4" <% nvram_selmatch("time_zone", "+05.5 1 4", "selected"); %>>UTC+05:30 / last Sun Oct - last Sun Mar</option>
										<option value="+05.75 1 1" <% nvram_selmatch("time_zone", "+05.75 1 1", "selected"); %>>UTC+05:45 / none</option>
										<option value="+05.75 1 2" <% nvram_selmatch("time_zone", "+05.75 1 2", "selected"); %>>UTC+05:45 / first Sun Apr - last Sun Oct</option>
										<option value="+05.75 1 3" <% nvram_selmatch("time_zone", "+05.75 1 3", "selected"); %>>UTC+05:45 / last Sun Mar - last Sun Oct</option>
										<option value="+05.75 1 4" <% nvram_selmatch("time_zone", "+05.75 1 4", "selected"); %>>UTC+05:45 / last Sun Oct - last Sun Mar</option>
										<option value="+06 1 1" <% nvram_selmatch("time_zone", "+06 1 1", "selected"); %>>UTC+06:00 / none</option>
										<option value="+06 1 2" <% nvram_selmatch("time_zone", "+06 1 2", "selected"); %>>UTC+06:00 / first Sun Apr - last Sun Oct</option>
										<option value="+06 1 3" <% nvram_selmatch("time_zone", "+06 1 3", "selected"); %>>UTC+06:00 / last Sun Mar - last Sun Oct</option>
										<option value="+06 1 4" <% nvram_selmatch("time_zone", "+06 1 4", "selected"); %>>UTC+06:00 / last Sun Oct - last Sun Mar</option>
										<option value="+06.5 1 1" <% nvram_selmatch("time_zone", "+06.5 1 1", "selected"); %>>UTC+06:30 / none</option>
										<option value="+06.5 1 2" <% nvram_selmatch("time_zone", "+06.5 1 2", "selected"); %>>UTC+06:30 / first Sun Apr - last Sun Oct</option>
										<option value="+06.5 1 3" <% nvram_selmatch("time_zone", "+06.5 1 3", "selected"); %>>UTC+06:30 / last Sun Mar - last Sun Oct</option>
										<option value="+06.5 1 4" <% nvram_selmatch("time_zone", "+06.5 1 4", "selected"); %>>UTC+06:30 / last Sun Oct - last Sun Mar</option>
										<option value="+07 1 1" <% nvram_selmatch("time_zone", "+07 1 1", "selected"); %>>UTC+07:00 / none</option>
										<option value="+07 1 2" <% nvram_selmatch("time_zone", "+07 1 2", "selected"); %>>UTC+07:00 / first Sun Apr - last Sun Oct</option>
										<option value="+07 1 3" <% nvram_selmatch("time_zone", "+07 1 3", "selected"); %>>UTC+07:00 / last Sun Mar - last Sun Oct</option>
										<option value="+07 1 4" <% nvram_selmatch("time_zone", "+07 1 4", "selected"); %>>UTC+07:00 / last Sun Oct - last Sun Mar</option>
										<option value="+08 1 1" <% nvram_selmatch("time_zone", "+08 1 1", "selected"); %>>UTC+08:00 / none</option>
										<option value="+08 1 2" <% nvram_selmatch("time_zone", "+08 1 2", "selected"); %>>UTC+08:00 / first Sun Apr - last Sun Oct</option>
										<option value="+08 1 3" <% nvram_selmatch("time_zone", "+08 1 3", "selected"); %>>UTC+08:00 / last Sun Mar - last Sun Oct</option>
										<option value="+08 1 4" <% nvram_selmatch("time_zone", "+08 1 4", "selected"); %>>UTC+08:00 / last Sun Oct - last Sun Mar</option>
										<option value="+09 1 1" <% nvram_selmatch("time_zone", "+09 1 1", "selected"); %>>UTC+09:00 / none</option>
										<option value="+09 1 2" <% nvram_selmatch("time_zone", "+09 1 2", "selected"); %>>UTC+09:00 / first Sun Apr - last Sun Oct</option>
										<option value="+09 1 3" <% nvram_selmatch("time_zone", "+09 1 3", "selected"); %>>UTC+09:00 / last Sun Mar - last Sun Oct</option>
										<option value="+09 1 4" <% nvram_selmatch("time_zone", "+09 1 4", "selected"); %>>UTC+09:00 / last Sun Oct - last Sun Mar</option>
										<option value="+09.5 1 1" <% nvram_selmatch("time_zone", "+09.5 1 1", "selected"); %>>UTC+09:30 / none</option>
										<option value="+09.5 1 2" <% nvram_selmatch("time_zone", "+09.5 1 2", "selected"); %>>UTC+09:30 / first Sun Apr - last Sun Oct</option>
										<option value="+09.5 1 3" <% nvram_selmatch("time_zone", "+09.5 1 3", "selected"); %>>UTC+09:30 / last Sun Mar - last Sun Oct</option>
										<option value="+09.5 1 4" <% nvram_selmatch("time_zone", "+09.5 1 4", "selected"); %>>UTC+09:30 / last Sun Oct - last Sun Mar</option>
										<option value="+10 1 1" <% nvram_selmatch("time_zone", "+10 1 1", "selected"); %>>UTC+10:00 / none</option>
										<option value="+10 1 2" <% nvram_selmatch("time_zone", "+10 1 2", "selected"); %>>UTC+10:00 / first Sun Apr - last Sun Oct</option>
										<option value="+10 1 3" <% nvram_selmatch("time_zone", "+10 1 3", "selected"); %>>UTC+10:00 / last Sun Mar - last Sun Oct</option>
										<option value="+10 1 4" <% nvram_selmatch("time_zone", "+10 1 4", "selected"); %>>UTC+10:00 / last Sun Oct - last Sun Mar</option>
										<option value="+10.5 1 1" <% nvram_selmatch("time_zone", "+10.5 1 1", "selected"); %>>UTC+10:30 / none</option>
										<option value="+10.5 1 2" <% nvram_selmatch("time_zone", "+10.5 1 2", "selected"); %>>UTC+10:30 / first Sun Apr - last Sun Oct</option>
										<option value="+10.5 1 3" <% nvram_selmatch("time_zone", "+10.5 1 3", "selected"); %>>UTC+10:30 / last Sun Mar - last Sun Oct</option>
										<option value="+10.5 1 4" <% nvram_selmatch("time_zone", "+10.5 1 4", "selected"); %>>UTC+10:30 / last Sun Oct - last Sun Mar</option>
										<option value="+11 1 1" <% nvram_selmatch("time_zone", "+11 1 1", "selected"); %>>UTC+11:00 / none</option>
										<option value="+11 1 2" <% nvram_selmatch("time_zone", "+11 1 2", "selected"); %>>UTC+11:00 / first Sun Apr - last Sun Oct</option>
										<option value="+11 1 3" <% nvram_selmatch("time_zone", "+11 1 3", "selected"); %>>UTC+11:00 / last Sun Mar - last Sun Oct</option>
										<option value="+11 1 4" <% nvram_selmatch("time_zone", "+11 1 4", "selected"); %>>UTC+11:00 / last Sun Oct - last Sun Mar</option>
										<option value="+11.5 1 1" <% nvram_selmatch("time_zone", "+11.5 1 1", "selected"); %>>UTC+11:30 / none</option>
										<option value="+11.5 1 2" <% nvram_selmatch("time_zone", "+11.5 1 2", "selected"); %>>UTC+11:30 / first Sun Apr - last Sun Oct</option>
										<option value="+11.5 1 3" <% nvram_selmatch("time_zone", "+11.5 1 3", "selected"); %>>UTC+11:30 / last Sun Mar - last Sun Oct</option>
										<option value="+11.5 1 4" <% nvram_selmatch("time_zone", "+11.5 1 4", "selected"); %>>UTC+11:30 / last Sun Oct - last Sun Mar</option>
										<option value="+12 1 1" <% nvram_selmatch("time_zone", "+12 1 1", "selected"); %>>UTC+12:00 / none</option>
										<option value="+12 1 2" <% nvram_selmatch("time_zone", "+12 1 2", "selected"); %>>UTC+12:00 / first Sun Apr - last Sun Oct</option>
										<option value="+12 1 3" <% nvram_selmatch("time_zone", "+12 1 3", "selected"); %>>UTC+12:00 / last Sun Mar - last Sun Oct</option>
										<option value="+12 1 4" <% nvram_selmatch("time_zone", "+12 1 4", "selected"); %>>UTC+12:00 / last Sun Oct - last Sun Mar</option>
										<option value="+12.75 1 1" <% nvram_selmatch("time_zone", "+12.75 1 1", "selected"); %>>UTC+12:45 / none</option>
										<option value="+12.75 1 2" <% nvram_selmatch("time_zone", "+12.75 1 2", "selected"); %>>UTC+12:45 / first Sun Apr - last Sun Oct</option>
										<option value="+12.75 1 3" <% nvram_selmatch("time_zone", "+12.75 1 3", "selected"); %>>UTC+12:45 / last Sun Mar - last Sun Oct</option>
										<option value="+12.75 1 4" <% nvram_selmatch("time_zone", "+12.75 1 4", "selected"); %>>UTC+12:45 / last Sun Oct - last Sun Mar</option>
										<option value="+13 1 1" <% nvram_selmatch("time_zone", "+13 1 1", "selected"); %>>UTC+13:00 / none</option>
										<option value="+13 1 2" <% nvram_selmatch("time_zone", "+13 1 2", "selected"); %>>UTC+13:00 / first Sun Apr - last Sun Oct</option>
										<option value="+13 1 3" <% nvram_selmatch("time_zone", "+13 1 3", "selected"); %>>UTC+13:00 / last Sun Mar - last Sun Oct</option>
										<option value="+13 1 4" <% nvram_selmatch("time_zone", "+13 1 4", "selected"); %>>UTC+13:00 / last Sun Oct - last Sun Mar</option>
										<option value="+14 1 1" <% nvram_selmatch("time_zone", "+14 1 1", "selected"); %>>UTC+14:00 / none</option>
										<option value="+14 1 2" <% nvram_selmatch("time_zone", "+14 1 2", "selected"); %>>UTC+14:00 / first Sun Apr - last Sun Oct</option>
										<option value="+14 1 3" <% nvram_selmatch("time_zone", "+14 1 3", "selected"); %>>UTC+14:00 / last Sun Mar - last Sun Oct</option>
										<option value="+14 1 4" <% nvram_selmatch("time_zone", "+14 1 4", "selected"); %>>UTC+14:00 / last Sun Oct - last Sun Mar</option>
									</select>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(idx.localtime)</script></div>
									<input class="spaceradio" type="checkbox" value="1" name="_daylight_time" <% nvram_checked("daylight_time", "1"); %> />
								</div>
							</fieldset><br />
							
							<div class="submitFooter">
								<script type="text/javascript">document.write("<input type=\"button\" name=\"save_button\" value=\"" + sbutton.save + "\" onclick=\"to_submit(this.form)\" />")</script>
								<script type="text/javascript">document.write("<input type=\"reset\" value=\"" + sbutton.cancel + "\" />")</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div id="logo">
							<h2><script type="text/javascript">Capture(share.help)</script></h2>
						</div>
						<dl>
							<dt class="term"><script type="text/javascript">Capture(idx.dhcp)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right2)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.hostname)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right4)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.domainname)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right6)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.lanip)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right8)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.subnet)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right10)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.dhcp_srv)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right12)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.dhcp_start)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right14)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.dhcp_maxusers)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right16)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.legend3)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hidx.right18)</script></dd>
						</dl>
						<br/>
						<a href="javascript:openHelpWindow('HSetup.asp');"><script type="text/javascript">Capture(share.more)</script></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
					<div class="info"><script type="text/javascript">Capture(share.firmware)</script>: <script>document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");</script></div>
					<div class="info"><script type="text/javascript">Capture(share.time)</script>: <% get_uptime(); %></div>
					<div class="info">WAN <% nvram_match("wl_mode","wet","disabled <!--"); %><% nvram_match("wan_proto","disabled","disabled <!--"); %>IP: <% nvram_status_get("wan_ipaddr"); %><% nvram_match("wan_proto","disabled","-->"); %><% nvram_match("wl_mode","wet","-->"); %></div>
				</div>
			</div>
		</div>
	</body>
</html>