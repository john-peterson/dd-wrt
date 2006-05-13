<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
		<title><% nvram_get("router_name"); %> - Quality of Service</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">
		
document.title = "<% nvram_get("router_name"); %>" + qos.titl;
		
function svcs_grey(sw_disabled,F) {
	F.add_svc.disabled = sw_disabled;
	for (i=0; i<F.svqos_nosvcs.value; i++) {
		eval("F.svqos_svcdel" + i).disabled = sw_disabled;
		eval("F.svqos_svcprio" + i).disabled = sw_disabled;
	}
}

function ips_grey(sw_disabled,F) {
	F.svqos_ipaddr0.disabled = sw_disabled;
	F.svqos_ipaddr1.disabled = sw_disabled;
	F.svqos_ipaddr2.disabled = sw_disabled;
	F.svqos_ipaddr3.disabled = sw_disabled;
	F.svqos_netmask.disabled = sw_disabled;
	F.add_ipsprio_button.disabled = sw_disabled;
	for (i=0; i<F.svqos_noips.value; i++){
		eval("F.svqos_ipdel" + i).disabled = sw_disabled;
		eval("F.svqos_ipprio" + i).disabled = sw_disabled;
	}
}

function macs_grey(sw_disabled,F) {
	F.svqos_hwaddr0.disabled = sw_disabled;
	F.svqos_hwaddr1.disabled = sw_disabled;
	F.svqos_hwaddr2.disabled = sw_disabled;
	F.svqos_hwaddr3.disabled = sw_disabled;
	F.svqos_hwaddr4.disabled = sw_disabled;
	F.svqos_hwaddr5.disabled = sw_disabled;
	F.add_macprio_button.disabled = sw_disabled;
	for (i=0; i<F.svqos_nomacs.value; i++){
		eval("F.svqos_macdel" + i).disabled = sw_disabled;
		eval("F.svqos_macprio" + i).disabled = sw_disabled;
	}
}

function port_grey(sw_disabled,F) {
	F.svqos_port1prio.disabled = sw_disabled;
	F.svqos_port2prio.disabled = sw_disabled;
	F.svqos_port3prio.disabled = sw_disabled;
	F.svqos_port4prio.disabled = sw_disabled;

	F.svqos_port1bw.disabled = sw_disabled;
	F.svqos_port2bw.disabled = sw_disabled;
	F.svqos_port3bw.disabled = sw_disabled;
	F.svqos_port4bw.disabled = sw_disabled;

}

function qos_grey(num,F) {
	var sw_disabled = (num == F.wshaper_enable[1].value) ? true : false;

	F._enable_game.disabled = sw_disabled;
	F.wshaper_uplink.disabled = sw_disabled;
	F.wshaper_downlink.disabled = sw_disabled;
	F.wshaper_dev.disabled = sw_disabled;
	F.qos_type.disabled = sw_disabled;
	F.add_svc_button.disabled = sw_disabled;
	F.edit_svc_button.disabled = sw_disabled;
	port_grey(sw_disabled, F);
	macs_grey(sw_disabled, F);
	ips_grey(sw_disabled, F);
	svcs_grey(sw_disabled, F);
}

function svc_add_submit(F) {
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "QoS";
	F.submit_type.value = "add_svc";
 	F.action.value = "Apply";
	F.submit();
}

function ip_add_submit(F) {
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "QoS";
	F.submit_type.value = "add_ip";
 	F.action.value = "Apply";
	F.submit();
}

function mac_add_submit(F) {
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "QoS";
	F.submit_type.value = "add_mac";
 	F.action.value = "Apply";
	F.submit();
}

function service(id, name, port_start, port_end) {
	this.id = id;
	this.name = name;
	this.start = port_start;
	this.end = port_end;
}

var sorton = function(x,y) {
	if(x.name <  y.name) return -1;
	else if (x.name == y.name) return 0;
	else return 1;
}

services=new Array();
services_length=0;
/* Init. services data structure */
<% filter_port_services_get("all_list", "0"); %>
services.sort(sorton);

function to_submit(F) {
	if (F._enable_game.checked == false){
	    F.enable_game.value = 0;
	}else{
	    F.enable_game.value = 1;
	}

	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "QoS";
	F.submit_type.value = "save";
//	F.save_button.value = "Saved";
	F.save_button.value = sbutton.saving;

 	F.action.value = "Apply";
	apply(F);
}

function init() {
	qos_grey(<% nvram_get("wshaper_enable"); %>,document.QoS);
}
		</script>
	</head>

	<body class="gui" onload="init()">
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
								<li class="current"><span><script type="text/javascript">Capture(bmenu.applications)</script></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><a href="Forward.asp"><script type="text/javascript">Capture(bmenu.applicationsprforwarding)</script></a></li>
											<li><a href="ForwardSpec.asp"><script type="text/javascript">Capture(bmenu.applicationspforwarding)</script></a></li>
											<li><a href="Triggering.asp"><script type="text/javascript">Capture(bmenu.applicationsptriggering)</script></a></li>
											<% nvram_match("dist_type", "micro", "<!--"); %>
											<li><a href="UPnP.asp"><script type="text/javascript">Capture(bmenu.applicationsUpnp)</script></a></li>
											<% nvram_match("dist_type", "micro", "-->"); %>
											<li><a href="DMZ.asp"><script type="text/javascript">Capture(bmenu.applicationsDMZ)</script></a></li>
											<li><span><script type="text/javascript">Capture(bmenu.applicationsQoS)</script></span></li>
										</ul>
									</div>
								</li>
								<li><a href="Management.asp"><script type="text/javascript">Capture(bmenu.admin)</script></a></li>
								<li><a href="Status_Router.asp"><script type="text/javascript">Capture(bmenu.statu)</script></a></li>
							</ul>
						</div>
					</div>
				</div>
            	<div id="main">
					<div id="contents">
						<form name="QoS" action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="enable_game" value="1" />
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="action" value="Apply" />
							<input type="hidden" name="commit" value="1" />
							<h2><script type="text/javascript">Capture(qos.h2)</script></h2>
							
							<fieldset>
  								<legend><script type="text/javascript">Capture(qos.legend)</script></legend>
  								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(qos.srv)</script></div>
									<input class="spaceradio" type="radio" value="1" name="wshaper_enable" onclick="qos_grey(this.value,this.form)" <% nvram_checked("wshaper_enable","1"); %> /><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" value="0" name="wshaper_enable" onclick="qos_grey(this.value,this.form)" <% nvram_checked("wshaper_enable","0"); %> /><script type="text/javascript">Capture(share.disable)</script>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.port)</script></div>
									<select name="wshaper_dev">
										<option value="WAN" <% nvram_selmatch("wshaper_dev", "WAN", "selected"); %>>WAN</option>
										<option value="LAN" <% nvram_selmatch("wshaper_dev", "LAN", "selected"); %>>LAN &amp; WLAN</option>
									</select>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(qos.type)</script></div>
									<select name="qos_type">
										<option value="0" <% nvram_selmatch("qos_type", "0", "selected"); %>>HTB</option>
										<option value="1" <% nvram_selmatch("qos_type", "1", "selected"); %>>HFSC</option>
									</select>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(qos.uplink)</script></div>
									<input type="text" size="5" class="num" name="wshaper_uplink" value="<% nvram_get("wshaper_uplink"); %>" />
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(qos.dnlink)</script></div>
									<input type="text" size="5" class="num" name="wshaper_downlink" value="<% nvram_get("wshaper_downlink"); %>" />
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(qos.gaming)</script></div>
									<input type="checkbox" name="_enable_game" value="1" <% nvram_checked("enable_game", "1"); %> />
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(qos.legend2)</script></legend>
								<table class="table">
									<tr>
										<th><script type="text/javascript">Capture(share.del)</script></th>
										<th><script type="text/javascript">Capture(share.srv)</script></th>
										<th><script type="text/javascript">Capture(share.priority)</script></th>
									</tr>
									<% get_qossvcs(); %>
									<tr>
										<td>&nbsp;</td>
										<td colspan="2">
											<script type="text/javascript">document.write("<input type=\"button\" name=\"add_svc_button\" value=\"" + sbutton.add + "\" onclick=\"svc_add_submit(this.form)\" />")</script>&nbsp;&nbsp;&nbsp;
											<select name="add_svc">
												<script type="text/javascript">
													var i=0;
													for(i=0;i<services_length;i++)
													document.write("<option value="+services[i].name+">"+services[i].name+ " [ "+
													services[i].start+" ~ "+
													services[i].end + " ]" + "</option>");
												</script>
											</select>
										</td>
									</tr>
									<tr>
										<td>&nbsp;</td>
										<td colspan="2">
											<script type="text/javascript">document.write("<input type=\"button\" name=\"edit_svc_button\" value=\"" + sbutton.edit_srv + "\" onclick=\"openWindow('Port_Services.asp', 630, 420)\" />")</script>&nbsp;&nbsp;&nbsp;
										</td>
									</tr>
								</table>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(qos.legend3)</script></legend>
								<table class="table">
									<% get_qosips(); %>
									<tr>
										<td>&nbsp;</td>
										<td colspan="2">
											<script type="text/javascript">document.write("<input type=\"button\" name=\"add_ipsprio_button\" value=\"" + sbutton.add + "\" onclick=\"ip_add_submit(this.form)\" />")</script>&nbsp;&nbsp;&nbsp;
											<input size="3" maxlength="3" name="svqos_ipaddr0" value="0" onblur="valid_range(this,0,255,'IP')" class="num" />.<input size="3" maxlength="3" name="svqos_ipaddr1" value="0" onblur="valid_range(this,0,255,'IP')" class="num" />.<input size="3" maxlength="3" name="svqos_ipaddr2" value="0" onblur="valid_range(this,0,255,'IP')" class="num" />.<input size="3" maxlength="3" name="svqos_ipaddr3" value="0" onblur="valid_range(this,0,255,'IP')" class="num" />&nbsp;/&nbsp;
											<input size="3" maxlength="3" name="svqos_netmask" value="0" onblur="valid_range(this,0,32,share.subnet)" class="num" />
										</td>
									</tr>
								</table>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(qos.legend4)</script></legend>
								<table class="table">
									<% get_qosmacs(); %>
									<tr>
										<td>&nbsp;</td>
										<td colspan="2">
											<script type="text/javascript">document.write("<input type=\"button\" name=\"add_macprio_button\" value=\"" + sbutton.add + "\" onclick=\"mac_add_submit(this.form)\" />")</script>&nbsp;&nbsp;&nbsp;
											<input name="svqos_hwaddr0" value="00" size="2" maxlength="2" onblur="valid_mac(this,0)" class="num" />:<input name="svqos_hwaddr1" value="00" size="2" maxlength="2" onblur="valid_mac(this,1)" class="num" />:<input name="svqos_hwaddr2" value="00" size="2" maxlength="2" onblur="valid_mac(this,1)" class="num"/>:<input name="svqos_hwaddr3" value="00" size="2" maxlength="2" onblur="valid_mac(this,1)" class="num" />:<input name="svqos_hwaddr4" value="00" size="2" maxlength="2" onblur="valid_mac(this,1)" class="num" />:<input name="svqos_hwaddr5" value="00" size="2" maxlength="2" onblur="valid_mac(this,1)" class="num" />
										</td>
									</tr>
								</table>
							</fieldset><br />
							<% show_default_level(); %>
							
							<fieldset>
								<legend><script type="text/javascript">Capture(qos.legend5)</script></legend>
								<table>
									<tr>
										<th>&nbsp;</th>
										<th><script type="text/javascript">Capture(share.priority)</script></th>
										<th><script type="text/javascript">Capture(qos.maxrate_o)</script></th>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(share.port)</script> 1</td>
										<td>
											<select name="svqos_port1prio">
												<script type="text/javascript">document.write("<option value=\"100\" <% nvram_selmatch("svqos_port1prio", "100", "selected"); %> >" + qos.prio_x + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"10\" <% nvram_selmatch("svqos_port1prio", "10", "selected"); %> >" + qos.prio_p + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"20\" <% nvram_selmatch("svqos_port1prio", "20", "selected"); %> >" + qos.prio_e + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"30\" <% nvram_selmatch("svqos_port1prio", "30", "selected"); %> >" + qos.prio_s + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"40\" <% nvram_selmatch("svqos_port1prio", "40", "selected"); %> >" + qos.prio_b + "</option>");</script>
											</select>
										</td>
										<td>
											<select name="svqos_port1bw">
												<script type="text/javascript">document.write("<option value=\"0\" <% nvram_match("svqos_port1bw", "0", "selected"); %> >" + share.disable + "</option>");</script>
												<option value="256k" <% nvram_match("svqos_port1bw", "256k", "selected"); %>>256k</option>
												<option value="512k" <% nvram_match("svqos_port1bw", "512k", "selected"); %>>512k</option>
												<option value="1m" <% nvram_match("svqos_port1bw", "1m", "selected"); %>>1M</option>
												<option value="2m" <% nvram_match("svqos_port1bw", "2m", "selected"); %>>2M</option>
												<option value="5m" <% nvram_match("svqos_port1bw", "5m", "selected"); %>>5M</option>
												<option value="10m" <% nvram_match("svqos_port1bw", "10m", "selected"); %>>10M</option>
												<option value="20m" <% nvram_match("svqos_port1bw", "20m", "selected"); %>>20M</option>
												<option value="50m" <% nvram_match("svqos_port1bw", "50m", "selected"); %>>50M</option>
												<option value="full" <% nvram_match("svqos_port1bw", "full", "selected"); %>>100M</option>
											</select>
										</td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(share.port)</script> 2</td>
										<td>
											<select name="svqos_port2prio">
												<script type="text/javascript">document.write("<option value=\"100\" <% nvram_selmatch("svqos_port2prio", "100", "selected"); %> >" + qos.prio_x + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"10\" <% nvram_selmatch("svqos_port2prio", "10", "selected"); %> >" + qos.prio_p + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"20\" <% nvram_selmatch("svqos_port2prio", "20", "selected"); %> >" + qos.prio_e + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"30\" <% nvram_selmatch("svqos_port2prio", "30", "selected"); %> >" + qos.prio_s + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"40\" <% nvram_selmatch("svqos_port2prio", "40", "selected"); %> >" + qos.prio_b + "</option>");</script>
											</select>
										</td>
										<td>
											<select name="svqos_port2bw">
												<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selmatch("svqos_port2bw", "0", "selected"); %> >" + share.disable + "</option>");</script>
												<option value="256k" <% nvram_selmatch("svqos_port2bw", "256k", "selected"); %>>256k</option>
												<option value="512k" <% nvram_selmatch("svqos_port2bw", "512k", "selected"); %>>512k</option>
												<option value="1m" <% nvram_selmatch("svqos_port2bw", "1m", "selected"); %>>1M</option>
												<option value="2m" <% nvram_selmatch("svqos_port2bw", "2m", "selected"); %>>2M</option>
												<option value="5m" <% nvram_selmatch("svqos_port2bw", "5m", "selected"); %>>5M</option>
												<option value="10m" <% nvram_selmatch("svqos_port2bw", "10m", "selected"); %>>10M</option>
												<option value="20m" <% nvram_selmatch("svqos_port2bw", "20m", "selected"); %>>20M</option>
												<option value="50m" <% nvram_selmatch("svqos_port2bw", "50m", "selected"); %>>50M</option>
												<option value="full" <% nvram_selmatch("svqos_port2bw", "full", "selected"); %>>100M</option>
											</select>
										</td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(share.port)</script> 3</td>
										<td>
											<select name="svqos_port3prio">
												<script type="text/javascript">document.write("<option value=\"100\" <% nvram_selmatch("svqos_port3prio", "100", "selected"); %> >" + qos.prio_x + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"10\" <% nvram_selmatch("svqos_port3prio", "10", "selected"); %> >" + qos.prio_p + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"20\" <% nvram_selmatch("svqos_port3prio", "20", "selected"); %> >" + qos.prio_e + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"30\" <% nvram_selmatch("svqos_port3prio", "30", "selected"); %> >" + qos.prio_s + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"40\" <% nvram_selmatch("svqos_port3prio", "40", "selected"); %> >" + qos.prio_b + "</option>");</script>
										</select>
										</td>
										<td>
											<select name="svqos_port3bw">
												<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selmatch("svqos_port3bw", "0", "selected"); %> >" + share.disable + "</option>");</script>
												<option value="256k" <% nvram_selmatch("svqos_port3bw", "256k", "selected"); %>>256k</option>
												<option value="512k" <% nvram_selmatch("svqos_port3bw", "512k", "selected"); %>>512k</option>
												<option value="1m" <% nvram_selmatch("svqos_port3bw", "1m", "selected"); %>>1M</option>
												<option value="2m" <% nvram_selmatch("svqos_port3bw", "2m", "selected"); %>>2M</option>
												<option value="5m" <% nvram_selmatch("svqos_port3bw", "5m", "selected"); %>>5M</option>
												<option value="10m" <% nvram_selmatch("svqos_port3bw", "10m", "selected"); %>>10M</option>
												<option value="20m" <% nvram_selmatch("svqos_port3bw", "20m", "selected"); %>>20M</option>
												<option value="50m" <% nvram_selmatch("svqos_port3bw", "50m", "selected"); %>>50M</option>
												<option value="full" <% nvram_selmatch("svqos_port3bw", "full", "selected"); %>>100M</option>
											</select>
										</td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(share.port)</script> 4</td>
										<td>
											<select name="svqos_port4prio">
												<script type="text/javascript">document.write("<option value=\"100\" <% nvram_selmatch("svqos_port4prio", "100", "selected"); %> >" + qos.prio_x + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"10\" <% nvram_selmatch("svqos_port4prio", "10", "selected"); %> >" + qos.prio_p + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"20\" <% nvram_selmatch("svqos_port4prio", "20", "selected"); %> >" + qos.prio_e + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"30\" <% nvram_selmatch("svqos_port4prio", "30", "selected"); %> >" + qos.prio_s + "</option>");</script>
												<script type="text/javascript">document.write("<option value=\"40\" <% nvram_selmatch("svqos_port4prio", "40", "selected"); %> >" + qos.prio_b + "</option>");</script>
											</select>
										</td>
										<td>
											<select name="svqos_port4bw">
												<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selmatch("svqos_port4bw", "0", "selected"); %> >" + share.disable + "</option>");</script>
												<option value="256k" <% nvram_selmatch("svqos_port4bw", "256k", "selected"); %>>256k</option>
												<option value="512k" <% nvram_selmatch("svqos_port4bw", "512k", "selected"); %>>512k</option>
												<option value="1m" <% nvram_selmatch("svqos_port4bw", "1m", "selected"); %>>1M</option>
												<option value="2m" <% nvram_selmatch("svqos_port4bw", "2m", "selected"); %>>2M</option>
												<option value="5m" <% nvram_selmatch("svqos_port4bw", "5m", "selected"); %>>5M</option>
												<option value="10m" <% nvram_selmatch("svqos_port4bw", "10m", "selected"); %>>10M</option>
												<option value="20m" <% nvram_selmatch("svqos_port4bw", "20m", "selected"); %>>20M</option>
												<option value="50m" <% nvram_selmatch("svqos_port4bw", "50m", "selected"); %>>50M</option>
												<option value="full" <% nvram_selmatch("svqos_port4bw", "full", "selected"); %>>100M</option>
											</select>
										</td>
									</tr>
								</table>
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
							<dt class="term"><script type="text/javascript">Capture(hqos.right1)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right2)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hqos.right3)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right4)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hqos.right5)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right6)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hqos.right7)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right8)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hqos.right9)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right10)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hqos.right11)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hqos.right12)</script></dd>
						</dl>
						<br/>
						<a href="javascript:openHelpWindow('HQos.asp');"><script type="text/javascript">Capture(share.more)</script></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
					<div class="info">Firmware: <script>document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");</script></div>
					<div class="info"><script type="text/javascript">Capture(share.time)</script>: <% get_uptime(); %></div>
					<div class="info">WAN <% nvram_match("wl_mode","wet","disabled <!--"); %><% nvram_match("wan_proto","disabled","disabled <!--"); %>IP: <% nvram_status_get("wan_ipaddr"); %><% nvram_match("wan_proto","disabled","-->"); %><% nvram_match("wl_mode","wet","-->"); %></div>
				</div>
			</div>
		</div>
	</body>
</html>