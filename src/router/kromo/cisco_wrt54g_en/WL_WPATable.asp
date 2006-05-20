<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
		<title><% nvram_get("router_name"); %> - WPA</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">
		
document.title = "<% nvram_get("router_name"); %>" + wpa.titl;
		
function SelMode(varname,num,F)	{
	F.submit_button.value = "WL_WPATable";
	F.change_action.value = "gozila_cgi";
	F.submit_type.value = "security";
	F.security_varname.value = varname;
	F.submit();
}
function to_submit(F) {
	F.submit_button.value = "WL_WPATable";
	F.submit_type.value = "save";
//	F.save_button.value = "Saved";
	F.save_button.value = sbutton.saving;
	
	F.change_action.value = "gozila_cgi";
	apply(F);
}
function valid_value(F) {
	if(F.security_mode.value == "disabled")
		return true;

	if(!valid_wpa_psk(F) || !valid_wep(F) || !valid_radius(F)) {
		return false;
	} else  {
		return true;
	}
}
function valid_radius(F) {
	if(F.security_mode.value == "radius" || F.security_mode.value == "wpa" || F.security_mode.value == "wpa2" || F.security_mode.value == "wpa wpa2"){
		if(F.wl_radius_key.value == "") {
//			alert("Please enter a Shared Key!");
			alert(errmsg.err38);
			F.wl_radius_key.focus();
			return false;
		}
	}

	return true;
}
function valid_wpa_psk(F) {
	if(F.security_mode.value == "psk" || F.security_mode.value == "psk2" || F.security_mode.value == "psk psk2"){
		if(F.wl_wpa_psk.value.length == 64){
			if(!isxdigit(F.wl_wpa_psk, F.wl_wpa_psk.value)) {
				return false;
			}
		} else if(F.wl_wpa_psk.value.length >=8 && F.wl_wpa_psk.value.length <= 63 ){
			if(!isascii(F.wl_wpa_psk,F.wl_wpa_psk.value)) {
				return false;
			}
		} else{
//			alert("Invalid Key, must be between 8 and 63 ASCII characters");
			alert(errmsg.err39);
			return false;
		}
	}

	return true;
}

function valid_wep(F) {
	if(F.security_mode.value == "psk" || F.security_mode.value == "wpa" || F.security_mode.value == "psk2" || F.security_mode.value == "wpa2" || F.security_mode.value == "psk psk2" || F.security_mode.value == "wpa wpa2")
		return true;

	if (ValidateKey(F.wl_key1, F.wl_wep_bit.options[F.wl_wep_bit.selectedIndex].value,1) == false)
		return false;

  	if (ValidateKey(F.wl_key2, F.wl_wep_bit.options[F.wl_wep_bit.selectedIndex].value,2) == false)
		return false;

	if (ValidateKey(F.wl_key3, F.wl_wep_bit.options[F.wl_wep_bit.selectedIndex].value,3) == false)
		return false;

	if (ValidateKey(F.wl_key4, F.wl_wep_bit.options[F.wl_wep_bit.selectedIndex].value,4) == false)
		return false;

	for (var i=1; i <= 4; i++) {
		if(F.wl_key[i-1].checked){
			aaa = eval("F.wl_key"+i).value;
			//if(aaa == "" && F.security_mode.value == "wep"){
			if(aaa == ""){
//				alert('You have to enter a key for Key '+i);
				alert(errmsg.err40 + i);
				return false;
			}
			break;
		}
	}

    return true;
}
function ValidateKey(key, bit, index) {
	if(bit == 64) {
		switch(key.value.length){
			case 0:
				break;
			case 10:
				if(!isxdigit(key,key.value)) {
					return false;
				}
				break;
			default:
//				alert("Invalid Length in key " + key.value);
				alert(errmsg.err41 + key.value);
				return false;
		}
	} else {
		switch(key.value.length){
			case 0:
				break;
			case 26:
				if(!isxdigit(key,key.value)) {
					return false;
				}
				break;
			default:
//				alert("Invalid Length in key " + key.value);
				alert(errmsg.err41 + key.value);
				return false;
		}
	}

	return true;
}
function keyMode(F) {
F.submit_button.value = "WL_WPATable";
F.change_action.value = "gozila_cgi";
F.submit_type.value = "keysize";
F.submit();
}
function generateKey(F,PREFIX) {

	F.submit_button.value = "WL_WPATable";
	F.change_action.value = "gozila_cgi";
	F.security_varname.value = PREFIX;
	if(F.wl_wep_bit.value == 64) {
		F.submit_type.value = "key_64";
	} else {
		F.submit_type.value = "key_128";
	}

	F.submit();
}
function generateKey64(F,PREFIX) {

	F.submit_button.value = "WL_WPATable";
	F.change_action.value = "gozila_cgi";
	F.security_varname.value = PREFIX;
	F.submit_type.value = "key_64";
	F.submit();
}
function generateKey128(F,PREFIX) {

	F.submit_button.value = "WL_WPATable";
	F.change_action.value = "gozila_cgi";
	F.security_varname.value = PREFIX;
	F.submit_type.value = "key_128";

	F.submit();
}
function init(){
	var F = document.forms[0];
	if(F.security_mode)
	if(F.security_mode.value == "wep" || F.security_mode.value == "radius") {
		//keyMode(<% nvram_gozila_get("wl_wep_bit"); %>);
		keyMode(document.wpa.wl_wep_bit.value, F);
	}

}
		</script>
	</head>

	<body class="gui" onload="init()"> <% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<div id="menu">
						<div id="menuMain">
							<ul id="menuMainList">
								<li><a href="index.asp"><script type="text/javascript">Capture(bmenu.setup)</script></a></li>
								<li class="current"><span><script type="text/javascript">Capture(bmenu.wireless)</script></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><a href="Wireless_Basic.asp"><script type="text/javascript">Capture(bmenu.wirelessBasic)</script></a></li>
											<li><a href="Wireless_radauth.asp"><script type="text/javascript">Capture(bmenu.wirelessRadius)</script></a></li>
											<li><span><script type="text/javascript">Capture(bmenu.wirelessSecurity)</script></span></li>
											<li><a href="Wireless_MAC.asp"><script type="text/javascript">Capture(bmenu.wirelessMac)</script></a></li>
											<li><a href="Wireless_Advanced.asp"><script type="text/javascript">Capture(bmenu.wirelessAdvanced)</script></a></li>
											<li><a href="Wireless_WDS.asp"><script type="text/javascript">Capture(bmenu.wirelessWds)</script></a></li>
										</ul>
									</div>
								</li>
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
						<form name="wpa" action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="action" />
							<input type="hidden" name="security_varname" />
							<input type="hidden" name="security_mode_last" />
							<input type="hidden" name="wl_wep_last" />
							<input type="hidden" name="filter_mac_value" />
							<h2><script type="text/javascript">Capture(wpa.h2)</script></h2>
							<% show_security(); %><br />
							<div class="submitFooter">
								<script type="text/javascript">document.write("<input type=\"button\" name=\"save_button\" value=\"" + sbutton.save + "\" onclick=\"to_submit(this.form)\" />")</script>
								<script type="text/javascript">document.write("<input type=\"reset\" value=\"" + sbutton.cancel + "\" />");</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div id="logo"><h2><script type="text/javascript">Capture(share.help)</script></h2></div>
						<dl>
							<dt class="term"><script type="text/javascript">Capture(hwpa.right1)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hwpa.right2)</script></dd>
						</dl><br />
						<a href="javascript:openHelpWindow('HWPA.asp')"><script type="text/javascript">Capture(share.more)</script></a>
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
