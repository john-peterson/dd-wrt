<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
		<title><% nvram_get("router_name"); %> - Setup</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">
var EN_DIS1 = '<% nvram_get("mtu_enable"); %>';
var wan_proto = '<% nvram_get("wan_proto"); %>';
var dhcp_win = null;

function pptpUseDHCP(F, value)
{
    if(value == "0"){
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

function valid_mtu(I) {
	var start = null;
	var end = null;
	if(wan_proto == "pppoe"){
		start = 576;
		end = 1492;
	} else if(wan_proto == "pptp" || wan_proto == "l2tp") {
		start = 1200;
                end = 1400;
	} else{
		start = 576;
		end = 1500;
	}

	valid_range(I,start,end,"MTU");
}

function SelMTU(num,F) {
	mtu_enable_disable(F,num);
}

function mtu_enable_disable(F,I) {
	EN_DIS1 = I;
	if ( I == "0" ) {
		choose_disable(F.wan_mtu);
	} else {
		choose_enable(F.wan_mtu);
	}
}

function SelWAN(num,F) {
	F.submit_button.value = "index";
	F.submit_type.value = "wan_proto";
	F.change_action.value = "gozila_cgi";
	F.wan_proto.value=F.wan_proto.options[num].value;
	F.submit();
}

function SelPPP(num,F) {
	F.submit_button.value = "index";
	F.change_action.value = "gozila_cgi";
	F.mpppoe_enable.value = F.mpppoe_enable.options[num].value;
	F.submit();
}

function SelDHCPFWD(num,F) {
	F.submit_button.value = "index";
	F.submit_type.value = "dhcpfwd";
	F.change_action.value = "gozila_cgi";
	F.dhcpfwd_enable.value = F.dhcpfwd_enable.options[num].value;
	F.submit();
}

function to_submit(F) {
	if(valid_value(F)){
		if(F._daylight_time.checked == false) {
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
		F.action.value = "Apply";
		F.save_button.value = "Saved";
		F.save_button.disabled = true;
		F.submit();
	}
}

function valid_value(F) {
	if(F.now_proto.value == "pptp" || F.now_proto.value == "static"){

		pptp_dhcp = "";

		// Sveasoft: allow 0.0.0.0 for pptp. We'll let DHCP take care of it.
		if (F.now_proto.value == "pptp" &&
		    F.wan_ipaddr_0.value == "0" && F.wan_ipaddr_1.value == "0" && F.wan_ipaddr_2.value == "0" && F.wan_ipaddr_3.value == "0")
			pptp_dhcp = "skip";
		if (!F.pptp_use_dhcp || F.pptp_use_dhcp.value == "0")
		{
		if(pptp_dhcp != "skip" && !valid_ip(F,"F.wan_ipaddr","IP",ZERO_NO|MASK_NO))
			return false;

		if(pptp_dhcp != "skip" && !valid_mask(F,"F.wan_netmask",ZERO_NO|BCST_NO))
			return false;
		}

		if(F.now_proto.value == "static"){
			if(!valid_ip(F,"F.wan_gateway","Gateway",ZERO_NO|MASK_NO))
				return false;
		}

		if(pptp_dhcp != "skip" && F.now_proto.value == "pptp"){
			if (F.pptp_use_dhcp.value == "0")
			{
			if(!valid_ip(F,"F.pptp_server_ip","Gateway",ZERO_NO|MASK_NO))
				return false;
			if(!valid_ip_gw(F,"F.wan_ipaddr","F.wan_netmask","F.pptp_server_ip"))
				return false;
			}
		}

	}
	if(F.now_proto.value == "pppoe" || F.now_proto.value == "pptp" || F.now_proto.value == "l2tp"){
		if(F.ppp_username.value == ""){
			alert("You must input a username!");
			F.ppp_username.focus();
			return false;
		}
		//if(F.ppp_passwd.value == ""){
		//	alert("You must input a passwd!");
		//	F.ppp_passwd.focus();
		//	return false;
		//}
	}
	<% nvram_selmatch("dhcpfwd_enable", "1", "//"); %>if(!valid_dhcp_server(F))
	<% nvram_selmatch("dhcpfwd_enable", "1", "//"); %>return false;

	if(F.router_name)
	if(F.router_name.value == ""){
		alert("You must input a Router Name!");
		F.router_name.focus();
		return false;
	}
	if(document.setup)
	if(document.setup.now_proto)
	if(document.setup.now_proto.value == "pptp")
		pptpUseDHCP(document.setup, '<% nvram_get("pptp_use_dhcp"); %>')


	return true;
}

function valid_dhcp_server(F) {
	if(F.lan_proto == null)
		return true;
	if(F.lan_proto.selectedIndex == 0) {
		return true;
	}

	a1 = parseInt(F.dhcp_start.value,10);
	a2 = parseInt(F.dhcp_num.value,10);
	if(a1 + a2 > 255){
		alert("Out of range, please adjust start IP address or user's numbers.");
		return false;
	}

	if (F.wan_dns0 != null)
	if(!valid_ip(F,"F.wan_dns0","DNS",MASK_NO)) {
		return false;
	}
	if (F.wan_dns1 != null)
	if(!valid_ip(F,"F.wan_dns1","DNS",MASK_NO)) {
		return false;
	}
	if (F.wan_dns2 != null)
	if(!valid_ip(F,"F.wan_dns2","DNS",MASK_NO)) {
		return false;
	}
	if (F.wan_wins != null)
	if(!valid_ip(F,"F.wan_wins","WINS",MASK_NO)) {
		return false;
	}

	return true;
}

function SelDHCP(T,F) {
	dhcp_enable_disable(F,T);
}

function dhcp_enable_disable(F,T) {
	var start = '';
	var end = '';
 	var total = F.elements.length;
	for(i=0 ; i < total ; i++){
		if(F.elements[i].name == "dhcp_start")
			start = i;
		if(F.elements[i].name == "wan_wins_3")
			end = i;
	}
	if(start == '' || end == '')
		return true;

	if( T == "static" ) {
		EN_DIS = 0;
		for(i = start; i<=end ;i++) {
			choose_disable(F.elements[i]);
		}
	} else {
		EN_DIS = 1;
		for(i = start; i<=end ;i++) {
			choose_enable(F.elements[i]);
		}
	}
}

function SelTime(num,f) {
	aaa = f.time_zone.options[num].value.charAt(4);
	daylight_enable_disable(f,aaa);
}

function ppp_enable_disable(F,I) {
	if( I == "0") {
		choose_disable(F.ppp_idletime);
		choose_enable(F.ppp_redialperiod);
	} else{
		choose_enable(F.ppp_idletime);
		choose_disable(F.ppp_redialperiod);
	}
}

function daylight_enable_disable(F,aaa) {
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

function init() {
	mtu_enable_disable(document.setup,'<% nvram_get("mtu_enable"); %>');
	aaa = document.setup.time_zone.options[document.setup.time_zone.selectedIndex].value.charAt(4);
	if(aaa == 0) {
		document.setup._daylight_time.checked = false;
		choose_disable(document.setup._daylight_time);
		document.setup.daylight_time.value = 0;
	}

	if(document.setup.now_proto.value == "pppoe" || document.setup.now_proto.value == "pptp" || document.setup.now_proto.value == "l2tp" || document.setup.now_proto.value == "heartbeat")
		ppp_enable_disable(document.setup,'<% nvram_get("ppp_demand"); %>');

	dhcp_enable_disable(document.setup,'<% nvram_get("lan_proto"); %>');
}
		</script>
	</head>

	<body class="gui" onload="init();"> <% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<div id="menu">
						<div id="menuMain">
							<ul id="menuMainList">
								<li class="current"><span>Setup</span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><span>Basic Setup</span></li>
											<li><a href="DDNS.asp">DDNS</a></li>
											<li><a href="WanMAC.asp">MAC Address Clone</a></li>
											<li><a href="Routing.asp">Advanced Routing</a></li>
											<li><a href="Vlan.asp">VLANs</a></li>
										</ul>
									</div>
								</li>
								<li><a href="Wireless_Basic.asp">Wireless</a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp">SIPatH</a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp">Security</a></li>
								<li><a href="Filters.asp">Access Restrictions</a></li>
								<li><a href="Forward.asp">Applications&nbsp;&amp;&nbsp;Gaming</a></li>
								<li><a href="Management.asp">Administration</a></li>
								<li><a href="Status_Router.asp">Status</a></li>
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
							<input type="hidden" name="now_proto" value='<% nvram_gozila_get("wan_proto"); %>' />
							<input type="hidden" name="dhcp_dnsmasq" value="0" />
							<input type="hidden" name="fullswitch" value="0" />
							<input type="hidden" name="daylight_time" value="0" />
							<input type="hidden" name="lan_ipaddr" value="4" />
							<% nvram_selmatch("wl_mode", "wet", "<!--"); %>
							<h2><% nvram_else_match("wl_mode", "ap", "Internet", "Wireless"); %> Setup</h2>
							<fieldset>
								<legend>Internet Connection Type</legend>
								<div class="setting">
							    	<div class="label">Connection Type</div>
							    	<select name="wan_proto" onchange="SelWAN(this.form.wan_proto.selectedIndex,this.form)">
								<% show_connectiontype(); %>
								</select>
								</div>
								<% show_index_setting(); %>
								<div class="setting">
									<div class="label">STP</div>
									<input type="radio" value="1" name="lan_stp" <% nvram_match("lan_stp","1","checked"); %> />Enable&nbsp;
									<input type="radio" value="0" name="lan_stp" <% nvram_match("lan_stp","0","checked"); %> />Disable&nbsp;
									<span class="default">(disable for COMCAST ISP)</span>
								</div>
							</fieldset><br />
							<fieldset>
								<legend>Optional Settings (required by some ISPs)</legend>
								<div class="setting">
									<div class="label">Router Name</div>
									<input maxlength="39" name="router_name" size="20" onblur="valid_name(this,&#34;Router%20Name&#34;)" value='<% nvram_get("router_name"); %>'/>
								</div>
								<div class="setting">
									<div class="label">Host Name</div>
									<input maxlength="39" name="wan_hostname" size="20" onblur="valid_name(this,&#34;Host%20Name&#34;)" value='<% nvram_get("wan_hostname"); %>'/>
								</div>
								<div class="setting">
									<div class="label">Domain Name</div>
									<input maxlength="79" name="wan_domain" size="20" onblur="valid_name(this,&#34;Domain%20name&#34;,SPACE_NO)" value='<% nvram_get("wan_domain"); %>'/>
								</div>
								<div class="setting">
									<div class="label">MTU</div>
									<select name="mtu_enable" onchange="SelMTU(this.form.mtu_enable.selectedIndex,this.form)">
										<option value="0" <% nvram_match("mtu_enable", "0", "selected"); %>>Auto</option>
										<option value="1" <% nvram_match("mtu_enable", "1", "selected"); %>>Manual</option>
									</select>&nbsp;
									<input class="num" maxlength="4" onblur="valid_mtu(this)" size="5" name="wan_mtu" value='<% nvram_get("wan_mtu"); %>'/>
								</div>
							</fieldset><br />
							<% nvram_selmatch("wl_mode", "wet", "-->"); %>
							<h2>Network Setup</h2>
							<fieldset>
								<legend>Router IP</legend>
								<div class="setting">
									<div class="label">Local IP Address</div>
									<input class="num" maxlength="3" size="3" onblur="valid_range(this,1,223,'IP')" name="lan_ipaddr_0" value='<% get_single_ip("lan_ipaddr","0"); %>'/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,0,255,'IP')" name="lan_ipaddr_1" value='<% get_single_ip("lan_ipaddr","1"); %>'/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,0,255,'IP')" name="lan_ipaddr_2" value='<% get_single_ip("lan_ipaddr","2"); %>'/>.<input class="num" maxlength="3" size="3" onblur="valid_range(this,1,254,'IP')" name="lan_ipaddr_3" value='<% get_single_ip("lan_ipaddr","3"); %>'/>
								</div>
								<div class="setting">
									<div class="label">Subnet Mask</div>
									<input type="hidden" name="lan_netmask" value="4" />
									<input class="num" maxlength="3" size="3" name="lan_netmask_0" onblur="valid_range(this,0,255,'Netmask')" value='<% get_single_ip("lan_netmask","0"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_netmask_1" onblur="valid_range(this,0,255,'Netmask')" value='<% get_single_ip("lan_netmask","1"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_netmask_2" onblur="valid_range(this,0,255,'Netmask')" value='<% get_single_ip("lan_netmask","2"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_netmask_3" onblur="valid_range(this,0,255,'Netmask')" value='<% get_single_ip("lan_netmask","3"); %>'/>
								</div>
								<div class="setting">
									<div class="label">Gateway</div>
									<input type="hidden" name="lan_gateway" value="4" />
									<input class="num" maxlength="3" size="3" name="lan_gateway_0" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("lan_gateway","0"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_gateway_1" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("lan_gateway","1"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_gateway_2" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("lan_gateway","2"); %>'/>.<input class="num" maxlength="3" size="3" name="lan_gateway_3" onblur="valid_range(this,0,254,'IP')" value='<% get_single_ip("lan_gateway","3"); %>'/>
								</div>
								<div class="setting">
									<div class="label">Local DNS</div>
									<input type="hidden" name="sv_localdns" value="4" />
									<input class="num" maxlength="3" size="3" name="sv_localdns_0" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("sv_localdns","0"); %>'/>.<input class="num" maxlength="3" size="3" name="sv_localdns_1" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("sv_localdns","1"); %>'/>.<input class="num" maxlength="3" size="3" name="sv_localdns_2" onblur="valid_range(this,0,255,'IP')" value='<% get_single_ip("sv_localdns","2"); %>'/>.<input class="num" maxlength="3" size="3" name="sv_localdns_3" onblur="valid_range(this,0,254,'IP')" value='<% get_single_ip("sv_localdns","3"); %>'/>
								</div>
							</fieldset><br />
							<% nvram_match("wl_mode", "ap", "<!--"); %>
							<fieldset>
								<legend>WAN Port</legend>
								<div class="setting">
									<div class="label">Assign WAN Port to Switch</div>
									<input type="checkbox" name="_fullswitch" value="1" <% nvram_checked("fullswitch", "1"); %> />
								</div>
							</fieldset><br />
							<% nvram_match("wl_mode", "ap", "-->"); %>
							<% show_dhcpd_settings(); %>
							<fieldset>
								<legend>Time Settings</legend>
								<div class="setting">
									<div class="label">Time Zone / Summer Time (DST)</div>
									<select name="time_zone" onchange="SelTime(this.form.time_zone.selectedIndex,this.form)">
										<option value="-12 1 1" <% nvram_match("time_zone", "-12 1 1", "selected"); %>>UTC-12:00 / none</option>
										<option value="-12 1 2" <% nvram_match("time_zone", "-12 1 2", "selected"); %>>UTC-12:00 / first Sun Apr - last Sun Oct</option>
										<option value="-12 1 3" <% nvram_match("time_zone", "-12 1 3", "selected"); %>>UTC-12:00 / last Sun Mar - last Sun Oct</option>
										<option value="-12 1 4" <% nvram_match("time_zone", "-12 1 4", "selected"); %>>UTC-12:00 / last Sun Oct - last Sun Mar</option>
										<option value="-11 1 1" <% nvram_match("time_zone", "-11 1 1", "selected"); %>>UTC-11:00 / none</option>
										<option value="-11 1 2" <% nvram_match("time_zone", "-11 1 2", "selected"); %>>UTC-11:00 / first Sun Apr - last Sun Oct</option>
										<option value="-11 1 3" <% nvram_match("time_zone", "-11 1 3", "selected"); %>>UTC-11:00 / last Sun Mar - last Sun Oct</option>
										<option value="-11 1 4" <% nvram_match("time_zone", "-11 1 3", "selected"); %>>UTC-11:00 / last Sun Oct - last Sun Mar</option>
										<option value="-10 1 1" <% nvram_match("time_zone", "-10 1 1", "selected"); %>>UTC-10:00 / none</option>
										<option value="-10 1 2" <% nvram_match("time_zone", "-10 1 2", "selected"); %>>UTC-10:00 / first Sun Apr - last Sun Oct</option>
										<option value="-10 1 3" <% nvram_match("time_zone", "-10 1 3", "selected"); %>>UTC-10:00 / last Sun Mar - last Sun Oct</option>
										<option value="-10 1 4" <% nvram_match("time_zone", "-10 1 4", "selected"); %>>UTC-10:00 / last Sun Oct - last Sun Mar</option>
										<option value="-09.5 1 1" <% nvram_match("time_zone", "-09.5 1 1", "selected"); %>>UTC-09:30 / none</option>
										<option value="-09.5 1 2" <% nvram_match("time_zone", "-09.5 1 2", "selected"); %>>UTC-09:30 / first Sun Apr - last Sun Oct</option>
										<option value="-09.5 1 3" <% nvram_match("time_zone", "-09.5 1 3", "selected"); %>>UTC-09:30 / last Sun Mar - last Sun Oct</option>
										<option value="-09.5 1 4" <% nvram_match("time_zone", "-09.5 1 4", "selected"); %>>UTC-09:30 / last Sun Oct - last Sun Mar</option>
										<option value="-09 1 1" <% nvram_match("time_zone", "-09 1 1", "selected"); %>>UTC-09:00 / none</option>
										<option value="-09 1 2" <% nvram_match("time_zone", "-09 1 2", "selected"); %>>UTC-09:00 / first Sun Apr - last Sun Oct</option>
										<option value="-09 1 3" <% nvram_match("time_zone", "-09 1 3", "selected"); %>>UTC-09:00 / last Sun Mar - last Sun Oct</option>
										<option value="-09 1 4" <% nvram_match("time_zone", "-09 1 4", "selected"); %>>UTC-09:00 / last Sun Oct - last Sun Mar</option>
										<option value="-08 1 1" <% nvram_match("time_zone", "-08 1 1", "selected"); %>>UTC-08:00 / none</option>
										<option value="-08 1 2" <% nvram_match("time_zone", "-08 1 2", "selected"); %>>UTC-08:00 / first Sun Apr - last Sun Oct</option>
										<option value="-08 1 3" <% nvram_match("time_zone", "-08 1 3", "selected"); %>>UTC-08:00 / last Sun Mar - last Sun Oct</option>
										<option value="-08 1 4" <% nvram_match("time_zone", "-08 1 4", "selected"); %>>UTC-08:00 / last Sun Oct - last Sun Mar</option>
										<option value="-07 1 1" <% nvram_match("time_zone", "-07 1 1", "selected"); %>>UTC-07:00 / none</option>
										<option value="-07 1 2" <% nvram_match("time_zone", "-07 1 2", "selected"); %>>UTC-07:00 / first Sun Apr - last Sun Oct</option>
										<option value="-07 1 3" <% nvram_match("time_zone", "-07 1 3", "selected"); %>>UTC-07:00 / last Sun Mar - last Sun Oct</option>
										<option value="-07 1 4" <% nvram_match("time_zone", "-07 1 4", "selected"); %>>UTC-07:00 / last Sun Oct - last Sun Mar</option>
										<option value="-06 1 1" <% nvram_match("time_zone", "-06 1 1", "selected"); %>>UTC-06:00 / none</option>
										<option value="-06 1 2" <% nvram_match("time_zone", "-06 1 2", "selected"); %>>UTC-06:00 / first Sun Apr - last Sun Oct</option>
										<option value="-06 1 3" <% nvram_match("time_zone", "-06 1 3", "selected"); %>>UTC-06:00 / last Sun Mar - last Sun Oct</option>
										<option value="-06 1 4" <% nvram_match("time_zone", "-06 1 4", "selected"); %>>UTC-06:00 / last Sun Oct - last Sun Mar</option>
										<option value="-05 1 1" <% nvram_match("time_zone", "-05 1 1", "selected"); %>>UTC-05:00 / none</option>
										<option value="-05 1 2" <% nvram_match("time_zone", "-05 1 2", "selected"); %>>UTC-05:00 / first Sun Apr - last Sun Oct</option>
										<option value="-05 1 3" <% nvram_match("time_zone", "-05 1 3", "selected"); %>>UTC-05:00 / last Sun Mar - last Sun Oct</option>
										<option value="-05 1 4" <% nvram_match("time_zone", "-05 1 4", "selected"); %>>UTC-05:00 / last Sun Oct - last Sun Mar</option>
										<option value="-04 1 1" <% nvram_match("time_zone", "-04 1 1", "selected"); %>>UTC-04:00 / none</option>
										<option value="-04 1 2" <% nvram_match("time_zone", "-04 1 2", "selected"); %>>UTC-04:00 / first Sun Apr - last Sun Oct</option>
										<option value="-04 1 3" <% nvram_match("time_zone", "-04 1 3", "selected"); %>>UTC-04:00 / last Sun Mar - last Sun Oct</option>
										<option value="-04 1 4" <% nvram_match("time_zone", "-04 1 4", "selected"); %>>UTC-04:00 / last Sun Oct - last Sun Mar</option>
										<option value="-03.5 1 1" <% nvram_match("time_zone", "-03.5 1 1", "selected"); %>>UTC-03:30 / none</option>
										<option value="-03.5 1 2" <% nvram_match("time_zone", "-03.5 1 2", "selected"); %>>UTC-03:30 / first Sun Apr - last Sun Oct</option>
										<option value="-03.5 1 3" <% nvram_match("time_zone", "-03.5 1 3", "selected"); %>>UTC-03:30 / last Sun Mar - last Sun Oct</option>
										<option value="-03.5 1 4" <% nvram_match("time_zone", "-03.5 1 4", "selected"); %>>UTC-03:30 / last Sun Oct - last Sun Mar</option>
										<option value="-03 1 1" <% nvram_match("time_zone", "-03 1 1", "selected"); %>>UTC-03:00 / none</option>
										<option value="-03 1 2" <% nvram_match("time_zone", "-03 1 2", "selected"); %>>UTC-03:00 / first Sun Apr - last Sun Oct</option>
										<option value="-03 1 3" <% nvram_match("time_zone", "-03 1 3", "selected"); %>>UTC-03:00 / last Sun Mar - last Sun Oct</option>
										<option value="-03 1 4" <% nvram_match("time_zone", "-03 1 4", "selected"); %>>UTC-03:00 / last Sun Oct - last Sun Mar</option>
										<option value="-02 1 1" <% nvram_match("time_zone", "-02 1 1", "selected"); %>>UTC-02:00 / none</option>
										<option value="-02 1 2" <% nvram_match("time_zone", "-02 1 2", "selected"); %>>UTC-02:00 / first Sun Apr - last Sun Oct</option>
										<option value="-02 1 3" <% nvram_match("time_zone", "-02 1 3", "selected"); %>>UTC-02:00 / last Sun Mar - last Sun Oct</option>
										<option value="-02 1 4" <% nvram_match("time_zone", "-02 1 4", "selected"); %>>UTC-02:00 / last Sun Oct - last Sun Mar</option>
										<option value="-01 1 1" <% nvram_match("time_zone", "-01 1 1", "selected"); %>>UTC-01:00 / none</option>
										<option value="-01 1 2" <% nvram_match("time_zone", "-01 1 2", "selected"); %>>UTC-01:00 / first Sun Apr - last Sun Oct</option>
										<option value="-01 1 3" <% nvram_match("time_zone", "-01 1 3", "selected"); %>>UTC-01:00 / last Sun Mar - last Sun Oct</option>
										<option value="-01 1 4" <% nvram_match("time_zone", "-01 1 4", "selected"); %>>UTC-01:00 / last Sun Oct - last Sun Mar</option>
										<option value="+00 1 1" <% nvram_match("time_zone", "+00 1 1", "selected"); %>>UTC / none</option>
										<option value="+00 1 2" <% nvram_match("time_zone", "+00 1 2", "selected"); %>>UTC / first Sun Apr - last Sun Oct</option>
										<option value="+00 1 3" <% nvram_match("time_zone", "+00 1 3", "selected"); %>>UTC / last Sun Mar - last Sun Oct</option>
										<option value="+00 1 4" <% nvram_match("time_zone", "+00 1 4", "selected"); %>>UTC / last Sun Oct - last Sun Mar</option>
										<option value="+01 1 1" <% nvram_match("time_zone", "+01 1 1", "selected"); %>>UTC+01:00 / none</option>
										<option value="+01 1 2" <% nvram_match("time_zone", "+01 1 2", "selected"); %>>UTC+01:00 / first Sun Apr - last Sun Oct</option>
										<option value="+01 1 3" <% nvram_match("time_zone", "+01 1 3", "selected"); %>>UTC+01:00 / last Sun Mar - last Sun Oct</option>
										<option value="+01 1 4" <% nvram_match("time_zone", "+01 1 4", "selected"); %>>UTC+01:00 / last Sun Oct - last Sun Mar</option>
										<option value="+02 1 1" <% nvram_match("time_zone", "+02 1 1", "selected"); %>>UTC+02:00 / none</option>
										<option value="+02 1 2" <% nvram_match("time_zone", "+02 1 2", "selected"); %>>UTC+02:00 / first Sun Apr - last Sun Oct</option>
										<option value="+02 1 3" <% nvram_match("time_zone", "+02 1 3", "selected"); %>>UTC+02:00 / last Sun Mar - last Sun Oct</option>
										<option value="+02 1 4" <% nvram_match("time_zone", "+02 1 4", "selected"); %>>UTC+02:00 / last Sun Oct - last Sun Mar</option>
										<option value="+03 1 1" <% nvram_match("time_zone", "+03 1 1", "selected"); %>>UTC+03:00 / none</option>
										<option value="+03 1 2" <% nvram_match("time_zone", "+03 1 2", "selected"); %>>UTC+03:00 / first Sun Apr - last Sun Oct</option>
										<option value="+03 1 3" <% nvram_match("time_zone", "+03 1 3", "selected"); %>>UTC+03:00 / last Sun Mar - last Sun Oct</option>
										<option value="+03 1 4" <% nvram_match("time_zone", "+03 1 4", "selected"); %>>UTC+03:00 / last Sun Oct - last Sun Mar</option>
										<option value="+03.5 1 1" <% nvram_match("time_zone", "+03.5 1 1", "selected"); %>>UTC+03:30 / none</option>
										<option value="+03.5 1 2" <% nvram_match("time_zone", "+03.5 1 2", "selected"); %>>UTC+03:30 / first Sun Apr - last Sun Oct</option>
										<option value="+03.5 1 3" <% nvram_match("time_zone", "+03.5 1 3", "selected"); %>>UTC+03:30 / last Sun Mar - last Sun Oct</option>
										<option value="+03.5 1 4" <% nvram_match("time_zone", "+03.5 1 3", "selected"); %>>UTC+03:30 / last Sun Oct - last Sun Mar</option>
										<option value="+04 1 1" <% nvram_match("time_zone", "+04 1 1", "selected"); %>>UTC+04:00 / none</option>
										<option value="+04 1 2" <% nvram_match("time_zone", "+04 1 2", "selected"); %>>UTC+04:00 / first Sun Apr - last Sun Oct</option>
										<option value="+04 1 3" <% nvram_match("time_zone", "+04 1 3", "selected"); %>>UTC+04:00 / last Sun Mar - last Sun Oct</option>
										<option value="+04 1 4" <% nvram_match("time_zone", "+04 1 4", "selected"); %>>UTC+04:00 / last Sun Oct - last Sun Mar</option>
										<option value="+05 1 1" <% nvram_match("time_zone", "+05 1 1", "selected"); %>>UTC+05:00 / none</option>
										<option value="+05 1 2" <% nvram_match("time_zone", "+05 1 2", "selected"); %>>UTC+05:00 / first Sun Apr - last Sun Oct</option>
										<option value="+05 1 3" <% nvram_match("time_zone", "+05 1 3", "selected"); %>>UTC+05:00 / last Sun Mar - last Sun Oct</option>
										<option value="+05 1 4" <% nvram_match("time_zone", "+05 1 4", "selected"); %>>UTC+05:00 / last Sun Oct - last Sun Mar</option>
										<option value="+05.5 1 1" <% nvram_match("time_zone", "+05.5 1 1", "selected"); %>>UTC+05:30 / none</option>
										<option value="+05.5 1 2" <% nvram_match("time_zone", "+05.5 1 2", "selected"); %>>UTC+05:30 / first Sun Apr - last Sun Oct</option>
										<option value="+05.5 1 3" <% nvram_match("time_zone", "+05.5 1 3", "selected"); %>>UTC+05:30 / last Sun Mar - last Sun Oct</option>
										<option value="+05.5 1 4" <% nvram_match("time_zone", "+05.5 1 4", "selected"); %>>UTC+05:30 / last Sun Oct - last Sun Mar</option>
										<option value="+05.75 1 1" <% nvram_match("time_zone", "+05.75 1 1", "selected"); %>>UTC+05:45 / none</option>
										<option value="+05.75 1 2" <% nvram_match("time_zone", "+05.75 1 2", "selected"); %>>UTC+05:45 / first Sun Apr - last Sun Oct</option>
										<option value="+05.75 1 3" <% nvram_match("time_zone", "+05.75 1 3", "selected"); %>>UTC+05:45 / last Sun Mar - last Sun Oct</option>
										<option value="+05.75 1 4" <% nvram_match("time_zone", "+05.75 1 4", "selected"); %>>UTC+05:45 / last Sun Oct - last Sun Mar</option>
										<option value="+06 1 1" <% nvram_match("time_zone", "+06 1 1", "selected"); %>>UTC+06:00 / none</option>
										<option value="+06 1 2" <% nvram_match("time_zone", "+06 1 2", "selected"); %>>UTC+06:00 / first Sun Apr - last Sun Oct</option>
										<option value="+06 1 3" <% nvram_match("time_zone", "+06 1 3", "selected"); %>>UTC+06:00 / last Sun Mar - last Sun Oct</option>
										<option value="+06 1 4" <% nvram_match("time_zone", "+06 1 4", "selected"); %>>UTC+06:00 / last Sun Oct - last Sun Mar</option>
										<option value="+06.5 1 1" <% nvram_match("time_zone", "+06.5 1 1", "selected"); %>>UTC+06:30 / none</option>
										<option value="+06.5 1 2" <% nvram_match("time_zone", "+06.5 1 2", "selected"); %>>UTC+06:30 / first Sun Apr - last Sun Oct</option>
										<option value="+06.5 1 3" <% nvram_match("time_zone", "+06.5 1 3", "selected"); %>>UTC+06:30 / last Sun Mar - last Sun Oct</option>
										<option value="+06.5 1 4" <% nvram_match("time_zone", "+06.5 1 4", "selected"); %>>UTC+06:30 / last Sun Oct - last Sun Mar</option>
										<option value="+07 1 1" <% nvram_match("time_zone", "+07 1 1", "selected"); %>>UTC+07:00 / none</option>
										<option value="+07 1 2" <% nvram_match("time_zone", "+07 1 2", "selected"); %>>UTC+07:00 / first Sun Apr - last Sun Oct</option>
										<option value="+07 1 3" <% nvram_match("time_zone", "+07 1 3", "selected"); %>>UTC+07:00 / last Sun Mar - last Sun Oct</option>
										<option value="+07 1 4" <% nvram_match("time_zone", "+07 1 4", "selected"); %>>UTC+07:00 / last Sun Oct - last Sun Mar</option>
										<option value="+08 1 1" <% nvram_match("time_zone", "+08 1 1", "selected"); %>>UTC+08:00 / none</option>
										<option value="+08 1 2" <% nvram_match("time_zone", "+08 1 2", "selected"); %>>UTC+08:00 / first Sun Apr - last Sun Oct</option>
										<option value="+08 1 3" <% nvram_match("time_zone", "+08 1 3", "selected"); %>>UTC+08:00 / last Sun Mar - last Sun Oct</option>
										<option value="+08 1 4" <% nvram_match("time_zone", "+08 1 4", "selected"); %>>UTC+08:00 / last Sun Oct - last Sun Mar</option>
										<option value="+09 1 1" <% nvram_match("time_zone", "+09 1 1", "selected"); %>>UTC+09:00 / none</option>
										<option value="+09 1 2" <% nvram_match("time_zone", "+09 1 2", "selected"); %>>UTC+09:00 / first Sun Apr - last Sun Oct</option>
										<option value="+09 1 3" <% nvram_match("time_zone", "+09 1 3", "selected"); %>>UTC+09:00 / last Sun Mar - last Sun Oct</option>
										<option value="+09 1 4" <% nvram_match("time_zone", "+09 1 4", "selected"); %>>UTC+09:00 / last Sun Oct - last Sun Mar</option>
										<option value="+09.5 1 1" <% nvram_match("time_zone", "+09.5 1 1", "selected"); %>>UTC+09:30 / none</option>
										<option value="+09.5 1 2" <% nvram_match("time_zone", "+09.5 1 2", "selected"); %>>UTC+09:30 / first Sun Apr - last Sun Oct</option>
										<option value="+09.5 1 3" <% nvram_match("time_zone", "+09.5 1 3", "selected"); %>>UTC+09:30 / last Sun Mar - last Sun Oct</option>
										<option value="+09.5 1 4" <% nvram_match("time_zone", "+09.5 1 4", "selected"); %>>UTC+09:30 / last Sun Oct - last Sun Mar</option>
										<option value="+10 1 1" <% nvram_match("time_zone", "+10 1 1", "selected"); %>>UTC+10:00 / none</option>
										<option value="+10 1 2" <% nvram_match("time_zone", "+10 1 2", "selected"); %>>UTC+10:00 / first Sun Apr - last Sun Oct</option>
										<option value="+10 1 3" <% nvram_match("time_zone", "+10 1 3", "selected"); %>>UTC+10:00 / last Sun Mar - last Sun Oct</option>
										<option value="+10 1 4" <% nvram_match("time_zone", "+10 1 4", "selected"); %>>UTC+10:00 / last Sun Oct - last Sun Mar</option>
										<option value="+10.5 1 1" <% nvram_match("time_zone", "+10.5 1 1", "selected"); %>>UTC+10:30 / none</option>
										<option value="+10.5 1 2" <% nvram_match("time_zone", "+10.5 1 2", "selected"); %>>UTC+10:30 / first Sun Apr - last Sun Oct</option>
										<option value="+10.5 1 3" <% nvram_match("time_zone", "+10.5 1 3", "selected"); %>>UTC+10:30 / last Sun Mar - last Sun Oct</option>
										<option value="+10.5 1 4" <% nvram_match("time_zone", "+10.5 1 4", "selected"); %>>UTC+10:30 / last Sun Oct - last Sun Mar</option>
										<option value="+11 1 1" <% nvram_match("time_zone", "+11 1 1", "selected"); %>>UTC+11:00 / none</option>
										<option value="+11 1 2" <% nvram_match("time_zone", "+11 1 2", "selected"); %>>UTC+11:00 / first Sun Apr - last Sun Oct</option>
										<option value="+11 1 3" <% nvram_match("time_zone", "+11 1 3", "selected"); %>>UTC+11:00 / last Sun Mar - last Sun Oct</option>
										<option value="+11 1 4" <% nvram_match("time_zone", "+11 1 4", "selected"); %>>UTC+11:00 / last Sun Oct - last Sun Mar</option>
										<option value="+11.5 1 1" <% nvram_match("time_zone", "+11.5 1 1", "selected"); %>>UTC+11:30 / none</option>
										<option value="+11.5 1 2" <% nvram_match("time_zone", "+11.5 1 2", "selected"); %>>UTC+11:30 / first Sun Apr - last Sun Oct</option>
										<option value="+11.5 1 3" <% nvram_match("time_zone", "+11.5 1 3", "selected"); %>>UTC+11:30 / last Sun Mar - last Sun Oct</option>
										<option value="+11.5 1 4" <% nvram_match("time_zone", "+11.5 1 4", "selected"); %>>UTC+11:30 / last Sun Oct - last Sun Mar</option>
										<option value="+12 1 1" <% nvram_match("time_zone", "+12 1 1", "selected"); %>>UTC+12:00 / none</option>
										<option value="+12 1 2" <% nvram_match("time_zone", "+12 1 2", "selected"); %>>UTC+12:00 / first Sun Apr - last Sun Oct</option>
										<option value="+12 1 3" <% nvram_match("time_zone", "+12 1 3", "selected"); %>>UTC+12:00 / last Sun Mar - last Sun Oct</option>
										<option value="+12 1 4" <% nvram_match("time_zone", "+12 1 4", "selected"); %>>UTC+12:00 / last Sun Oct - last Sun Mar</option>
										<option value="+12.75 1 1" <% nvram_match("time_zone", "+12.75 1 1", "selected"); %>>UTC+12:45 / none</option>
										<option value="+12.75 1 2" <% nvram_match("time_zone", "+12.75 1 2", "selected"); %>>UTC+12:45 / first Sun Apr - last Sun Oct</option>
										<option value="+12.75 1 3" <% nvram_match("time_zone", "+12.75 1 3", "selected"); %>>UTC+12:45 / last Sun Mar - last Sun Oct</option>
										<option value="+12.75 1 4" <% nvram_match("time_zone", "+12.75 1 4", "selected"); %>>UTC+12:45 / last Sun Oct - last Sun Mar</option>
										<option value="+13 1 1" <% nvram_match("time_zone", "+13 1 1", "selected"); %>>UTC+13:00 / none</option>
										<option value="+13 1 2" <% nvram_match("time_zone", "+13 1 2", "selected"); %>>UTC+13:00 / first Sun Apr - last Sun Oct</option>
										<option value="+13 1 3" <% nvram_match("time_zone", "+13 1 3", "selected"); %>>UTC+13:00 / last Sun Mar - last Sun Oct</option>
										<option value="+13 1 4" <% nvram_match("time_zone", "+13 1 4", "selected"); %>>UTC+13:00 / last Sun Oct - last Sun Mar</option>
										<option value="+14 1 1" <% nvram_match("time_zone", "+14 1 1", "selected"); %>>UTC+14:00 / none</option>
										<option value="+14 1 2" <% nvram_match("time_zone", "+14 1 2", "selected"); %>>UTC+14:00 / first Sun Apr - last Sun Oct</option>
										<option value="+14 1 3" <% nvram_match("time_zone", "+14 1 3", "selected"); %>>UTC+14:00 / last Sun Mar - last Sun Oct</option>
										<option value="+14 1 4" <% nvram_match("time_zone", "+14 1 4", "selected"); %>>UTC+14:00 / last Sun Oct - last Sun Mar</option>
									</select>
								</div>
								<div class="setting">
									<div class="label">Use local time</div>
									<input type="checkbox" value="1" name="_daylight_time" <% nvram_checked("daylight_time", "1"); %> />
								</div>
							</fieldset><br />
							<div class="submitFooter">
								<input type="button" name="save_button" value="Save Settings" onclick="to_submit(this.form)"/>
								<input type="reset" value="Cancel Changes"/>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div id="logo"><h2>Help</h2></div>
						<dl>
							<dt class="term">Automatic Configuration - DHCP: </dt>
							<dd class="definition">This setting is most commonly used by Cable operators.</dd>
							<dt class="term">Host Name: </dt>
							<dd class="definition">Enter the host name provided by your ISP.</dd>
							<dt class="term">Domain Name: </dt>
							<dd class="definition">Enter the domain name provided by your ISP.</dd>
							<dt class="term">Local IP Address: </dt>
							<dd class="definition">This is the address of the router.</dd>
							<dt class="term">Subnet Mask: </dt>
							<dd class="definition">This is the subnet mask of the router.</dd>
							<dt class="term">DHCP Server: </dt>
							<dd class="definition">Allows the router to manage your IP addresses.</dd>
							<dt class="term">Starting IP Address: </dt>
							<dd class="definition">The address you would like to start with.</dd>
							<dt class="term">Maximum number of DHCP Users: </dt>
							<dd class="definition">You may limit the number of addresses your router hands out.</dd>
							<dt class="term">Time Setting: </dt>
							<dd class="definition">Choose the time zone you are in and Summer Time (DST) period. The router can use local time or UTC time.</dd>
						</dl>
						<br/>
						<a href="javascript:openHelpWindow('HSetup.asp')">More...</a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
					<div class="info">Firmware: <script>document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");</script></div>
					<div class="info">Time: <% get_uptime(); %></div>
					<div class="info">WAN <% nvram_match("wl_mode","wet","disabled <!--"); %><% nvram_match("wan_proto","disabled","disabled <!--"); %>IP: <% nvram_status_get("wan_ipaddr"); %><% nvram_match("wan_proto","disabled","-->"); %><% nvram_match("wl_mode","wet","-->"); %></div>
				</div>
			</div>
		</div>
	</body>
</html>