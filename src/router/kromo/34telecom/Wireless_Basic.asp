<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html
  PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
   <head>
      <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
   
      <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
      <title><% nvram_get("router_name"); %> - Wireless</title>
      <link type="text/css" rel="stylesheet" href="style.css" /><script type="text/JavaScript" src="common.js">{}</script><script language="JavaScript">
var EN_DIS = '<% nvram_else_match("wl_gmode","-1","0","1"); %>';
function SelWL(num,F) {
	if ( num == 0)
		I = '0';
	else
		I = '1';
	wl_enable_disable(F,I);
}
function wl_enable_disable(F,I) {
	EN_DIS = I;
	if( I == "0"){
		choose_disable(F.wl_ssid);
		choose_disable(F.wl_channel);
		<% nvram_match("wl_mode", "ap", "choose_disable(F.wl_closed[0]);"); %>
		<% nvram_match("wl_mode", "ap", "choose_disable(F.wl_closed[1]);"); %>
	} else {
		choose_enable(F.wl_ssid);
		choose_enable(F.wl_channel);
		<% nvram_match("wl_mode", "ap", "choose_enable(F.wl_closed[0]);"); %>
		<% nvram_match("wl_mode", "ap", "choose_enable(F.wl_closed[1]);"); %>
	}
}

function to_submit(F) {
	if(F.wl_ssid.value == ""){
		alert("You must input a SSID!");
		F.wl_ssid.focus();
		return false;
	}

	F.submit_button.value = "Wireless_Basic";
	F.action.value = "Apply";
	F.submit();
}
function init() {
	wl_enable_disable(document.wireless,'<% nvram_else_match("wl_gmode","-1","0","1"); %>');
}
</script></head>
   <body class="gui" onload="init()">
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
                        <li class="current"><span>Wireless</span><div id="menuSub">
                              <ul id="menuSubList">
                                 <li><span>Basic Settings</span></li>
                                 <li><a href="Wireless_radauth.asp">WRT-radauth</a></li>
                                 <li><a href="WL_WPATable.asp">Wireless Security</a></li>
                                 <li><a href="Wireless_MAC.asp">MAC Filter</a></li>
                                 <li><a href="Wireless_Advanced.asp">Advanced Settings</a></li>
                                 <li><a href="Wireless_WDS.asp">WDS</a></li>
                              </ul>
                           </div>
                        </li>
			<% nvram_invmatch("sipgate","1","<!--"); %>
			<li><a href="Sipath.asp">SIPatH</a></li>
                        <% nvram_invmatch("sipgate","1","-->"); %>
                        <li><a href="Firewall.asp">Security</a></li>
                        <li><a href='<% support_elsematch("PARENTAL_CONTROL_SUPPORT", "1", "Parental_Control.asp", "Filters.asp"); %>'>Access Restrictions</a></li>
                        <li><a href="Forward.asp">Applications&nbsp;&amp;&nbsp;Gaming</a></li>
                        <li><a href="Management.asp">Administration</a></li>
                        <li><a href="Status_Router.asp">Status</a></li>
                     </ul>
                  </div>
               </div>
            </div>
            <div id="main">
               <div id="contents">
                  <form name="wireless" action="apply.cgi" method="<% get_http_method(); %>"><input type="hidden" name="submit_button" value="Wireless_Basic" /><input type="hidden" name="commit" value="1" /><input type="hidden" name="action" value="Apply" /><h2>Wireless</h2>
                     <div>
                        <div class="setting"><div class="label">Wireless Mode</div><select name="wl_mode">
                              <option value="sta" <% nvram_match("wl_mode", "sta", "selected"); %>>Client</option>
			      <option value="wet" <% nvram_match("wl_mode", "wet", "selected"); %>>Client Bridged</option>
			      </select>
			</div>
                        <div class="setting">
                           <div class="label">Wireless Network Mode</div><select name="wl_net_mode" onChange=SelWL(this.form.wl_net_mode.selectedIndex,this.form)>
                              <option value="disabled" <% nvram_match("wl_net_mode", "disabled", "selected"); %>>Disabled</option>
			      <option value="mixed" <% nvram_match("wl_net_mode", "mixed", "selected"); %>>Mixed</option>
                              <option value="b-only" <% nvram_match("wl_net_mode", "b-only", "selected"); %>>B-Only</option>
                              <option value="g-only" <% nvram_match("wl_net_mode", "g-only", "selected"); %>>G-Only</option></select></div>
                        <div class="setting">
                           <div class="label">Wireless Network Name (SSID)</div><input name="wl_ssid" size="20" maxLength="32" onBlur="valid_name(this,'SSID')" value='<% nvram_get("wl_ssid"); %>' /></div>
			 <% nvram_invmatch("wl_mode", "ap", "<!--"); %>
                        <div class="setting">
                           <div class="label">Wireless Channel</div><select name="wl_channel" onFocus="check_action(this,0)"><script language="javascript">
	var max_channel = 14;
	var wl_channel = '<% nvram_get("wl_channel"); %>';
	var buf = "";
	var freq = new Array("Auto","2.412","2.417","2.422","2.427","2.432","2.437","2.442","2.447","2.452","2.457","2.462","2.467","2.472","2.484");
	for(i=0; i<=max_channel ; i++){
		if(i == wl_channel)	buf = "selected";
		else			buf = "";
		if(i==0)
		 document.write("<option value="+i+" "+buf+">Auto</option>");
		else
		 document.write("<option value="+i+" "+buf+">"+i+" - "+freq[i]+"GHz</option>");
	}
</script></select></div>
                        <div class="setting">
                           <div class="label">Wireless SSID Broadcast</div>
			   <input type="radio" value="0" name="wl_closed" <% nvram_match("wl_closed","0","checked"); %>>Enable</input>
			   <input type="radio" value="1" name="wl_closed" <% nvram_match("wl_closed","1","checked"); %>>Disable</input>
			</div>
			<% nvram_invmatch("wl_mode", "ap", "-->"); %>
                     </div><br /><div class="submitFooter"><input type="button" value="Save Settings" onClick="to_submit(this.form)" /><input type="reset" value="Cancel Changes" /></div>
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
                     <dt class="term">Wireless Network Mode: </dt>
                     <dd class="definition">If you wish to exclude Wireless-G clients, choose <em>B-Only</em> mode. If you would like to disable wireless access, choose <em>Disable</em>.
                     </dd>
                  </dl><br /><a target="_blank" href="help/HWireless.asp">More...</a></div>
            </div>
         </div>
      </div>
   </body>
</html>