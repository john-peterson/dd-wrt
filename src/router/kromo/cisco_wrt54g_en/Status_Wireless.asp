<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %> - Wireless Status</title>
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + status_wireless.titl;

function setWirelessTable() {
	var table = document.getElementById("wireless_table");
	var val = arguments;
	cleanTable(table);
	if(!val.length) {
		var cell = table.insertRow(-1).insertCell(-1);
		cell.colSpan = 5;
		cell.align = "center";
		cell.innerHTML = "- None - ";
		return;
	}
	for(var i = 0; i < val.length; i = i + 4) {
		var row = table.insertRow(-1);
		
		var mac = val[i];
		var cellmac = row.insertCell(-1);
		cellmac.title = share.oui;
		cellmac.style.cursor = "pointer";
		eval("addEvent(cellmac, 'click', function() { getOUIFromMAC('" + mac + "') })");
		cellmac.innerHTML = mac;

		row.insertCell(-1).innerHTML = val[i + 1];
		row.insertCell(-1).innerHTML = val[i + 2];
		row.insertCell(-1).innerHTML = val[i + 3];
		setMeterBar(row.insertCell(-1), (val[i + 1] == "0" ? 0 : parseInt(val[i + 1]) * 1.24 + 116), "");
	}
}

function setWDSTable() {
	var table = document.getElementById("wds_table");
	var val = arguments;
	cleanTable(table);
	if(!val.length) {
		setElementVisible("wds", false);
		return;
	}
	for(var i = 0; i < val.length; i = i + 5) {
		var row = table.insertRow(-1);
		
		var mac = val[i];
		var cellmac = row.insertCell(-1);
		cellmac.title = share.oui;
		cellmac.style.cursor = "pointer";
		eval("addEvent(cellmac, 'click', function() { getOUIFromMAC('" + mac + "') })");
		cellmac.innerHTML = mac;
		
		row.insertCell(-1).innerHTML = val[i + 1];
		row.insertCell(-1).innerHTML = val[i + 2];
		row.insertCell(-1).innerHTML = val[i + 3];
		row.insertCell(-1).innerHTML = val[i + 4];
		setMeterBar(row.insertCell(-1), (val[i + 2] == "0" ? 0 : parseInt(val[i + 2]) * 1.24 + 116), "");
	}
	setElementVisible("wds", true);
}

function setPacketInfo(val) {
	var packet = val.replace(/[A-Za-z=]/g, "").split(";");
	setMeterBar("packet_rx", 
		(parseInt(packet[1]) == 0 ? 100 : parseInt(packet[0]) / (parseInt(packet[0]) + parseInt(packet[1])) * 100),
		packet[0] + " OK, " + (packet[1] > 0 ? packet[1] + " " + share.errs : share.none2 + " " + share.err)
	);
	setMeterBar("packet_tx",
		(parseInt(packet[3]) == 0 ? 100 : parseInt(packet[2]) / (parseInt(packet[2]) + parseInt(packet[3])) * 100),
		packet[2] + " OK, " + (packet[3] > 0 ? packet[3] + " " + share.errs : share.none2 + " " + share.err)
	);
}

var update;

addEvent(window, "load", function() {
	setWirelessTable(<% active_wireless(0); %>);
	setWDSTable(<% active_wds(0); %>);
	setPacketInfo("<% wl_packet_get(); %>");

	update = new StatusUpdate("Status_Wireless.live.asp", <% nvram_get("refresh_time"); %>);
	update.onUpdate("active_wireless", function(u) {
		eval('setWirelessTable(' + u.active_wireless + ')');
	});
	update.onUpdate("active_wds", function(u) {
		eval('setWDSTable(' + u.active_wds + ')');
	});
	update.onUpdate("packet_info", function(u) {
		setPacketInfo(u.packet_info);
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
								<li><a href="index.asp"><% tran("bmenu.setup"); %></a></li>
								<li><a href="Wireless_Basic.asp"><% tran("bmenu.wireless"); %></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><% tran("bmenu.sipath"); %></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><% tran("bmenu.security"); %></a></li>
								<li><a href="Filters.asp"><% tran("bmenu.accrestriction"); %></a></li>
								<li><a href="Forward.asp"><% tran("bmenu.applications"); %></a></li>
								<li><a href="Management.asp"><% tran("bmenu.admin"); %></a></li>
								<li class="current"><span><% tran("bmenu.statu"); %></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><a href="Status_Router.asp"><% tran("bmenu.statuRouter"); %></a></li>
											<li><a href="Status_Lan.asp"><% tran("bmenu.statuLAN"); %></a></li>
											<li><span><% tran("bmenu.statuWLAN"); %></span></li>
											<% show_sputnik(); %>
											<% nvram_invmatch("status_auth","1","<!--"); %>
											<li><a href="Info.htm"><% tran("bmenu.statuSysInfo"); %></a></li>
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
						<form>
							<h2><% tran("status_wireless.h2"); %></h2>
							
							<fieldset>
								<legend><% tran("status_wireless.legend"); %></legend>
								<div class="setting">
									<div class="label"><% tran("share.mac"); %></div>
									<script>document.write("<span id=\"wl_mac\" style=\"cursor:pointer\" title=\"" + share.oui + "\" onclick=\"getOUIFromMAC('<% nvram_get("wl0_hwaddr"); %>')\" >");</script><% nvram_get("wl0_hwaddr"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.mode"); %></div>
									<% nvram_match("wl_mode", "wet", "<script type="text/javascript">Capture(wl_basic.clientBridge)</script>"); %><% nvram_match("wl_mode", "ap", "<script type="text/javascript">Capture(wl_basic.ap)</script>"); %><% nvram_match("wl_mode", "sta", "<script type="text/javascript">Capture(wl_basic.client)</script>"); %><% nvram_match("wl_mode", "infra", "<script type="text/javascript">Capture(wl_basic.adhoc)</script>"); %><% nvram_match("wl_mode", "apsta", "Repeater"); %>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("status_wireless.net"); %></div>
									<% nvram_match("wl_net_mode", "disabled","<script type="text/javascript">Capture(share.disabled)</script>"); %><% nvram_match("wl_net_mode", "mixed", "<script type="text/javascript">Capture(wl_basic.mixed)</script>"); %><% nvram_match("wl_net_mode", "g-only", "<script type="text/javascript">Capture(wl_basic.g)</script>"); %><% nvram_match("wl_net_mode", "b-only", "<script type="text/javascript">Capture(wl_basic.b)</script>"); %>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.ssid"); %></div>
									<span id="wl_ssid"><% nvram_get("wl_ssid"); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.channel"); %></div>
									<span id="wl_channel"><% get_curchannel(); %></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label">Xmit</div>
									<span id="wl_xmit"><% nvram_get("txpwr"); %> mW</span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.rates"); %></div>
									<span id="wl_rate"><% get_currate(); %> Mbps</span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.encrypt"); %></div>
									<% nvram_match("security_mode", "disabled", "<script type="text/javascript">Capture(share.disable)</script>"); %><% nvram_invmatch("security_mode", "disabled", "<script type="text/javascript">Capture(share.enable)</script>,&nbsp;"); %><% nvram_match("security_mode", "psk", "WPA Pre-shared Key"); %><% nvram_match("security_mode", "wpa", "WPA RADIUS"); %><% nvram_match("security_mode", "psk2", "WPA2 Pre-Shared Key Only"); %><% nvram_match("security_mode", "wpa2", "WPA2 RADIUS Only"); %><% nvram_match("security_mode", "psk psk2", "WPA2 Pre-Shared Key Mixed"); %><% nvram_match("security_mode", "wpa wpa2", "WPA2 RADIUS Mixed"); %><% nvram_match("security_mode", "radius", "RADIUS"); %><% nvram_match("security_mode", "wep", "WEP"); %>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("status_wireless.pptp"); %></div>
									<% nvram_else_match("pptpd_connected", "1", "<script type="text/javascript">Capture(share.connected)</script>", "<script type="text/javascript">Capture(share.disconnected)</script>"); %>&nbsp;
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><% tran("status_wireless.legend2"); %></legend>
								<div class="setting">
									<div class="label"><% tran("status_wireless.rx"); %></div>
									<span id="packet_rx"></span>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("status_wireless.tx"); %></div>
									<span id="packet_tx"></span>&nbsp;
								</div>
							</fieldset><br />
							
							<h2><% tran("status_wireless.h22"); %></h2>
							<fieldset>
								<legend><% nvram_match("wl_mode", "wet", "<script type="text/javascript">Capture(info.ap)</script>"); %><% nvram_match("wl_mode", "ap", "<script type="text/javascript">Capture(status_wireless.legend3)</script>"); %><% nvram_match("wl_mode", "sta", "<script type="text/javascript">Capture(info.ap)</script>"); %><% nvram_match("wl_mode", "infra", "<script type="text/javascript">Capture(info.ap)</script>"); %><% nvram_match("wl_mode", "apsta", "<script type="text/javascript">Capture(status_wireless.legend3)</script>"); %></legend>
								<table class="table center" cellspacing="6" id="wireless_table">
									<tr>
										<th width="54%"><% tran("share.mac"); %></th>
										<th width="8%"><% tran("share.signal"); %></th>
										<th width="8%"><% tran("share.noise"); %></th>
										<th width="8%">SNR</th>
										<th width="22%"><% tran("status_wireless.signal_qual"); %></th>
									</tr>
								</table>
							</fieldset><br />
							
							<span id="wds" style="display:none">
								<fieldset>
									<legend><% tran("status_wireless.wds"); %></legend>
									<table class="table center" cellspacing="6" id="wds_table">
										<tr>
											<th width="20%"><% tran("share.mac"); %></th>
											<th width="31%"><% tran("share.descr"); %></th>
											<th width="8%"><% tran("share.signal"); %></th>
											<th width="8%"><% tran("share.noise"); %></th>
											<th width="8%">SNR</th>
											<th width="22%"><% tran("status_wireless.signal_qual"); %></th>
										</tr>
									</table>
								</fieldset><br />
								
							</span>
							<div class="center">
								<script>document.write("<input type=\"button\" name=\"site_survey\" value=\"" + sbutton.survey + "\" onclick=\"<% nvram_else_match("wl_net_mode", "disabled", "alert(errmsg.err59)", "openWindow('Site_Survey.asp', 760, 700)"); %>\" />");</script>
							</div><br />
							<div class="submitFooter">
								<script>document.write("<input type=\"button\" name=\"refresh_button\" value=\"" + <% nvram_else_match("refresh_time","0","sbutton.refres","sbutton.autorefresh"); %> + "\" onclick=\"window.location.reload()\" />");</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div id="logo">
							<h2><% tran("share.help"); %></h2>
						</div>
						<dl>
							<dt class="term"><% tran("share.mac"); %>:</dt>
							<dd class="definition"><% tran("hstatus_wireless.right2"); %></dd>
							<dt class="term"><% tran("status_wireless.net"); %>:</dt>
							<dd class="definition"><% tran("hstatus_wireless.right4"); %></dd>
							<dt class="term"><% tran("share.oui"); %>:</dt>
							<dd class="definition"><% tran("hstatus_lan.right10"); %></dd>
						</dl><br />
						<a href="javascript:openHelpWindow('HStatusWireless.asp');"><% tran("share.more"); %></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
					<div class="info"><% tran("share.firmware"); %>: <script>document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");</script></div>
					<div class="info"><% tran("share.time"); %>: <% get_uptime(); %></div>
					<div class="info">WAN <% nvram_match("wl_mode","wet","disabled <!--"); %><% nvram_match("wan_proto","disabled","disabled <!--"); %>IP: <% nvram_status_get("wan_ipaddr"); %><% nvram_match("wan_proto","disabled","-->"); %><% nvram_match("wl_mode","wet","-->"); %></div>
				</div>
			</div>
		</div>
	</body>
</html>
