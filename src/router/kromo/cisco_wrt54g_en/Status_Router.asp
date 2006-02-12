<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html
  PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
   <head>
      <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
   
      <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
      <title><% nvram_get("router_name"); %> - Status</title>
      <link type="text/css" rel="stylesheet" href="style.css" /><script type="text/JavaScript" src="common.js">{}</script><script language="JavaScript">
function DHCPAction(F,I) {
	F.submit_type.value = I;
	F.submit_button.value = "Status_Router";
	F.change_action.value = "gozila_cgi";
	F.submit();
}
function Connect(F,I) {
	F.submit_type.value = I;
	F.submit_button.value = "Status_Router";
	F.change_action.value = "gozila_cgi";
	F.submit();
}
function init() {
	<% show_status("onload");%>
}
function ShowAlert(M) {
	var str = "";
	var mode = "";
	var wan_ip = "<% nvram_status_get("wan_ipaddr"); %>";
	var wan_proto = "<% nvram_safe_get("wan_proto"); %>";

	if(document.status.wan_proto.value == "pppoe")
		mode = "PPPoE";
	else if(document.status.wan_proto.value == "heartbeat")
		mode = "HBS";
	else
		mode = "PPTP";

	if(M == "AUTH_FAIL" || M == "PAP_AUTH_FAIL" || M == "CHAP_AUTH_FAIL")
		str = mode + " authentication fail";
	else if(M == "IP_FAIL" || (M == "TIMEOUT" && wan_ip == "0.0.0.0"))
		str = "Can not get a IP address from " + mode + " server";
	else if(M == "NEG_FAIL")
		str = mode + " negotication fail";
	else if(M == "LCP_FAIL")
		str = mode + " LCP negotication fail";
	else if(M == "TCP_FAIL" || (M == "TIMEOUT" && wan_ip != "0.0.0.0" && wan_proto == "heartbeat"))
		str = "Can not build a TCP connection to " + mode + " server";
	else
		str = "Can not connect to " + mode + " server";

	alert(str);

	Refresh();
}
var value=0;
function Refresh() {
	var refresh_time = "<% show_status("refresh_time"); %>";
	if(refresh_time == "")	refresh_time = 60000;
	if (value>=1) {
		window.location.replace("Status_Router.asp");
	}
	value++;
	timerID=setTimeout("Refresh()",refresh_time);
}
function ViewDHCP() {
	dhcp_win = self.open('DHCPTable.asp','inLogTable','alwaysRaised,resizable,scrollbars,width=720,height=600');
	dhcp_win.focus();
}
</script></head>
   <body class="gui" onload="init()"> <% showad(); %>
      <div id="wrapper">
         <div id="content">
            <div id="header">
               <div id="logo">
                  <h1><% show_control(); %></h1>
               </div>
               <div id="menu">
                  <div id="menuMain">
                     <ul id="menuMainList">
                        <li><a href="index.asp">Setup</a></li>
                        <li><a href="Wireless_Basic.asp">Wireless</a></li>
			<% nvram_invmatch("sipgate","1","<!--"); %>
			<li><a href="Sipath.asp">SIPatH</a></li>
                        <% nvram_invmatch("sipgate","1","-->"); %>
                        <li><a href="Firewall.asp">Security</a></li>
                        <li><a href='<% support_elsematch("PARENTAL_CONTROL_SUPPORT", "1", "Parental_Control.asp", "Filters.asp"); %>'>Access Restrictions</a></li>
                        <li><a href="Forward.asp">Applications&nbsp;&amp;&nbsp;Gaming</a></li>
                        <li><a href="Management.asp">Administration</a></li>
                        <li class="current"><span>Status</span><div id="menuSub">
                              <ul id="menuSubList">
                                 <li><span>Router</span></li>
                                 <li><a href="Status_Lan.asp">LAN</a></li>
                                 <li><a href="Status_Wireless.asp">Wireless</a></li>
				 <% show_sputnik(); %>
                              </ul>
                           </div>
                        </li>
                     </ul>
                  </div>
               </div>
            </div>
            <div id="main">
               <div id="contents">
                  <form name="status" action="apply.cgi" method="<% get_http_method(); %>">
		  <input type="hidden" name="submit_button" />
		  <input type="hidden" name="submit_type" />
		  <input type="hidden" name="change_action" />
		  <input type="hidden" name="action" />
		  <input type="hidden" name="wan_proto" value='<% nvram_get("wan_proto"); %>' />
		  <h2>Router Information</h2>
                     <div>
                        <div class="setting">
                           <div class="label">Firmware Version</div><% get_firmware_version(); %>
                        </div>
                        <div class="setting">
                           <div class="label">Current Time</div><% localtime(); %>
                        </div>
                        <div class="setting">
                           <div class="label">MAC Address</div><% nvram_get("wan_hwaddr"); %>
                        </div>
                        <div class="setting">
                           <div class="label">Router Name</div><% nvram_get("router_name"); %>
                        </div>
                        <div class="setting">
                           <div class="label">Host Name</div><% nvram_get("wan_hostname"); %>
                        </div>
                        <div class="setting">
                           <div class="label">Domain Name</div><% nvram_get("wan_domain"); %>
                        </div>
                        <div class="setting">
                           <div class="label">Time</div><% get_uptime(); %>
                        </div>
                     </div><br /><h2>Internet</h2>
                     <fieldset>
                        <legend>Configuration Type</legend>
                        <div class="setting">
                           <div class="label">Login Type</div>
			   <% nvram_match("wan_proto","dhcp","Automatic Configuration - DHCP"); %>
			   <% nvram_match("wan_proto","static","Static"); %>
			   <% nvram_match("wan_proto","pppoe","PPPoE"); %>
			   <% nvram_match("wan_proto","pptp","PPTP"); %>
			   <% nvram_match("wan_proto","heartbeat","HeartBeatSignal"); %>
			   <% nvram_match("wan_proto","disabled","Disabled"); %>
                        </div>
			<% show_status_setting(); %>
			<% nvram_match("wan_proto", "dhcp", "<input onclick=DHCPAction(this.form,'release') type=button value='DHCP Release'/><input onclick=DHCPAction(this.form,'renew') type=button value='DHCP Renew'/>"); %>
                     </fieldset><br /><div class="submitFooter"><input type="button" value="Refresh" onclick="window.location.replace('Status_Router.asp')" /></div>
                  </form>
               </div>
            </div>
            <div id="statusInfo">
               <div class="info">Firmware: <% get_firmware_version(); %></div>
               <div class="info">Time: <% get_uptime(); %></div>
               <div class="info">WAN IP: <% nvram_status_get("wan_ipaddr"); %></div>
            </div>
            <div id="helpContainer">
               <div id="help">
                  <div id="logo">
                     <h2>Help</h2>
                  </div>
                  <dl>
                     <dt class="term">Firmware Version: </dt>
                     <dd class="definition">This is the Router's current firmware.</dd>
                     <dt class="term">Current Time: </dt>
                     <dd class="definition">This shows the time, as you set on the Setup Tab.</dd>
                     <dt class="term">MAC Address: </dt>
                     <dd class="definition">This is the Router's MAC Address, as seen by your ISP.</dd>
                     <dt class="term">Router Name: </dt>
                     <dd class="definition">This is the specific name for the Router, which you set on the Setup Tab.</dd>
                     <dt class="term">Configuration Type: </dt>
                     <dd class="definition">This shows the information required by your ISP for connection to the Internet. This information was entered on the Setup
                        Tab. You can <em>Connect</em> or <em>Disconnect</em> your connection here by clicking on that button.
                     </dd>
                  </dl><br /><a target="_blank" href="help/HStatus.asp">More...</a></div>
            </div>
         </div>
      </div>
   </body>
</html>
