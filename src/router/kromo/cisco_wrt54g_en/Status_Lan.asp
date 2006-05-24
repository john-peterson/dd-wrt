<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<% charset(); %>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<title><% nvram_get("router_name"); %> - LAN Status</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + status_lan.titl;

function deleteLease(val) {
	document.forms[0].d_0.value = val;
	document.forms[0].submit();
}

function setDHCPTable() {
	var val = arguments;
	var table = document.getElementById("dhcp_leases_table");
	cleanTable(table);
	if(!val.length) {
		var cell = table.insertRow(-1).insertCell(-1);
		cell.colSpan = 5;
		cell.align = "center";
		cell.innerHTML = "- " + share.none + " -";
		return;
	}
	for(var i = 0; i < val.length; i = i + 5) {
	
		var row = table.insertRow(-1);
		row.style.height = "15px";
		row.insertCell(-1).innerHTML = val[i];
		row.insertCell(-1).innerHTML = val[i + 1];
		
		var mac = val[i + 2];
		var cellmac = row.insertCell(-1);
		cellmac.title = share.oui;
		cellmac.style.cursor = "pointer";
		eval("addEvent(cellmac, 'click', function() { getOUIFromMAC('" + mac + "') })");
		cellmac.innerHTML = mac;

		row.insertCell(-1).innerHTML = val[i + 3];
		var cell = row.insertCell(-1);
		cell.className = "bin";
		cell.title = errmsg.err58;
		eval("addEvent(cell, 'click', function() { deleteLease(" + val[i + 4] + ") })");
	}
}

var update;

addEvent(window, "load", function() {
	setElementContent("dhcp_end_ip", "<% prefix_ip_get("lan_ipaddr",1); %>" + (parseInt("<% nvram_get("dhcp_start"); %>") + parseInt("<% nvram_get("dhcp_num"); %>") - 1));
	setDHCPTable(<% dumpleases(0); %>);
	setElementVisible("dhcp_1", "<% nvram_get("lan_proto"); %>" == "dhcp");
	setElementVisible("dhcp_2", "<% nvram_get("lan_proto"); %>" == "dhcp");

	update = new StatusUpdate("Status_Lan.live.asp", <% nvram_get("refresh_time"); %>);
	update.onUpdate("lan_proto", function(u) {
		setElementVisible("dhcp_1", u.lan_proto == "dhcp");
		setElementVisible("dhcp_2", u.lan_proto == "dhcp");
	});
	update.onUpdate("dhcp_start", function(u) {
		setElementContent("dhcp_start_ip", u.lan_ip_prefix + u.dhcp_start);
		setElementContent("dhcp_end_ip", u.lan_ip_prefix + (parseInt(u.dhcp_start) + parseInt(u.dhcp_num) - 1));
	});
	update.onUpdate("dhcp_leases", function(u) {
		eval('setDHCPTable(' + u.dhcp_leases + ')');
	});
	update.start();
});

addEvent(window, "unload", function() {
	update.stop();
});
		</script>
	 </head>

	 <body class="gui">
	 	<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<div id="menu">
						<div id="menuMain">
							<ul id="menuMainList">
								<li><a href="index.asp"><script type="text/javascript">Capture(bmenu.setup)</script></a></li>
								<li><a href="Wireless_Basic.asp"><script type="text/javascript">Capture(bmenu.wireless)</script></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><script type="text/javascript">Capture(bmenu.sipath)</script></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><script type="text/javascript">Capture(bmenu.security)</script></a></li>
								<li><a href="Filters.asp"><script type="text/javascript">Capture(bmenu.accrestriction)</script></a></li>
								<li><a href="Forward.asp"><script type="text/javascript">Capture(bmenu.applications)</script></a></li>
								<li><a href="Management.asp"><script type="text/javascript">Capture(bmenu.admin)</script></a></li>
								<li class="current"><span><script type="text/javascript">Capture(bmenu.statu)</script></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><a href="Status_Router.asp"><script type="text/javascript">Capture(bmenu.statuRouter)</script></a></li>
											<li><span><script type="text/javascript">Capture(bmenu.statuLAN)</script></span></li>
											<li><a href="Status_Wireless.asp"><script type="text/javascript">Capture(bmenu.statuWLAN)</script></a></li>
											<% show_sputnik(); %>
											<% nvram_invmatch("status_auth","1","<!--"); %>
											<li><a href="Info.htm"><script type="text/javascript">Capture(bmenu.statuSysInfo)</script></a></li>
											<% nvram_invmatch("status_auth","1","-->"); %>
										</ul>
									</div>
								</li>
							</ul>
						</div>
					</div>
				</div>
				<div id="main">
					<div id="contents">
						<form action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" value="DHCPTable" />
							<input type="hidden" name="change_action" value="gozila_cgi" />
							<input type="hidden" name="submit_type" value="delete" />
							<input type="hidden" name="d_0" />
							<input type="hidden" name="next_page" value="Status_Lan.asp" />
							<h2><script type="text/javascript">Capture(status_lan.h2)</script></h2>
							<fieldset>
								<legend><script type="text/javascript">Capture(status_lan.legend)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.mac)</script></div>
									<script>document.write("<span id=\"lan_mac\" style=\"cursor:pointer\" title=\"" + share.oui + "\" onclick=\"getOUIFromMAC('<% nvram_get("lan_hwaddr"); %>')\" >");</script><% nvram_get("lan_hwaddr"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.ip)</script></div>
									<span id="lan_ip"><% nvram_get("lan_ipaddr"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.subnet)</script></div>
									<span id="lan_netmask"><% nvram_get("lan_netmask"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.gateway)</script></div>
									<span id="lan_gateway"><% nvram_get("lan_gateway"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.localdns)</script></div>
									<span id="lan_dns"><% nvram_get("sv_localdns"); %></span>&nbsp;
								</div>
							</fieldset><br />
							<h2><script type="text/javascript">Capture(status_lan.h22)</script></h2>
							<fieldset>
								<legend><script type="text/javascript">Capture(status_lan.legend2)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(service.dhcp_legend2)</script></div>
									<span id="dhcp_enabled"><% nvram_match("lan_proto", "dhcp", "Enabled"); %><% nvram_match("lan_proto", "static", "Disabled"); %></span>&nbsp;
								</div>
								<span id="dhcp_1" style="display:none">
									<div class="setting">
										<div class="label"><script type="text/javascript">Capture(service.dhcp_srv)</script></div>
										<span id="dhcp_daemon"><% nvram_else_match("dhcp_dnsmasq", "1", "DNSMasq", "uDHCPd"); %></span>&nbsp;
									</div>
									<div class="setting">
										<div class="label"><script type="text/javascript">Capture(idx.dhcp_start)</script></div>
										<span id="dhcp_start_ip"><% prefix_ip_get("lan_ipaddr", "1"); %><% nvram_get("dhcp_start"); %></span>&nbsp;
									</div>
									<div class="setting">
										<div class="label"><script type="text/javascript">Capture(idx.dhcp_end)</script></div>
										<span id="dhcp_end_ip"></span>&nbsp;
									</div>
									<div class="setting">
										<div class="label"><script type="text/javascript">Capture(idx.dhcp_lease)</script></div>
										<span id="dhcp_lease_time"><% nvram_get("dhcp_lease"); %></span> <script type="text/javascript">Capture(share.minutes)</script>&nbsp;
									</div>
								</span>
							</fieldset><br />
							<span id="dhcp_2" style="display:none">
								<fieldset>
									<legend><script type="text/javascript">Capture(status_lan.legend3)</script></legend>
									<table class="table center" cellspacing="6" id="dhcp_leases_table">
										<tr>
											<th width="25%"><script type="text/javascript">Capture(share.hostname)</script></th>
											<th width="25%"><script type="text/javascript">Capture(share.ip)</script></th>
											<th width="25%"><script type="text/javascript">Capture(share.mac)</script></th>
											<th width="25%"><script type="text/javascript">Capture(share.expires)</script></th>
											<th>Delete</th>
										</tr>
									</table>
								</fieldset><br />
							</span>
							<div class="submitFooter">
								<script>document.write("<input type=\"button\" name=\"refresh_button\" value=\"" + <% nvram_else_match("refresh_time","0","sbutton.refres","sbutton.autorefresh"); %> + "\" onclick=\"window.location.reload()\">");</script>
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
							<dt class="term"><script type="text/javascript">Capture(share.mac)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hstatus_lan.right2)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.ip)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hstatus_lan.right4)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.subnet)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hstatus_lan.right6)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(idx.dhcp_srv)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hstatus_lan.right8)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(share.oui)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hstatus_lan.right10)</script></dd>
						</dl><br />
						<a href="javascript:openHelpWindow('HStatusLan.asp');"><script type="text/javascript">Capture(share.more)</script></a>
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