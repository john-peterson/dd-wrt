<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<% charset(); %>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<title><% nvram_get("router_name"); %> - Advanced Wireless Settings</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]> <link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /> <![endif]-->
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + wl_adv.titl;

function to_submit(F) {
	F.submit_button.value = "Wireless_Advanced";
	F.save_button.value = sbutton.saving;
	F.action.value = "Apply";
	apply(F);
}

function setWMM(val) {
	setElementsActive("wl_wme_no_ack", "_wl_wme_sta_vo5", val == "on");
}

addEvent(window, "load", function() {
	setWMM("<% nvram_get("wl_wme"); %>");
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
                				<li class="current"><span><script type="text/javascript">Capture(bmenu.wireless)</script></span>
                  				<div id="menuSub">
                     			  <ul id="menuSubList">
                      			  	<li><a href="Wireless_Basic.asp"><script type="text/javascript">Capture(bmenu.wirelessBasic)</script></a></li>
                      			  	<li><a href="Wireless_radauth.asp"><script type="text/javascript">Capture(bmenu.wirelessRadius)</script></a></li>
                      			  	<li><a href="WL_WPATable.asp"><script type="text/javascript">Capture(bmenu.wirelessSecurity)</script></a></li>
                      				<li><a href="Wireless_MAC.asp"><script type="text/javascript">Capture(bmenu.wirelessMac)</script></a></li>
                      				<li><span><script type="text/javascript">Capture(bmenu.wirelessAdvanced)</script></span></li>
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
						<form action="apply.cgi" method="<% get_http_method(); %>" name="wireless" id="wireless">
							<input type="hidden" name="action" value="Apply" />
							<input type="hidden" name="submit_button" value="Wireless_Advanced" />
							<input type="hidden" name="commit" value="1" />
							<h2><script type="text/javascript">Capture(wl_adv.h2)</script></h2>
							
							<fieldset>
								<legend><script type="text/javascript">Capture(wl_adv.legend)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label)</script></div>
									<input class="spaceradio" type="radio" name="wl_auth" value="0" <% nvram_checked("wl_auth", "0"); %> ><script type="text/javascript">Capture(share.auto)</script></input>&nbsp;
									<input class="spaceradio" type="radio" name="wl_auth" value="1" <% nvram_checked("wl_auth", "1"); %> ><script type="text/javascript">Capture(share.share_key)</script></input>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.auto + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label2)</script></div>
									<select name="wl_rateset">
										<option value="12" <% nvram_selected("wl_rateset", "12"); %> />1-2 Mbps</option>
										<script type="text/javascript">document.write("<option value=\"default\" <% nvram_selected("wl_rateset", "default", "js"); %>>" + share.deflt + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"all\" <% nvram_selected("wl_rateset", "all", "js"); %>>" + share.all + "</option>");</script>
									</select>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.deflt + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label3)</script></div>
									<select name="wl_rate">
										<option value="1000000" <% nvram_selected("wl_rate", "1000000"); %>>1 Mbps</option>
										<option value="2000000" <% nvram_selected("wl_rate", "2000000"); %>>2 Mbps</option>
										<option value="5500000" <% nvram_selected("wl_rate", "5500000"); %>>5.5 Mbps</option>
										<option value="6000000" <% nvram_selected("wl_rate", "6000000"); %>>6 Mbps</option>
										<option value="9000000" <% nvram_selected("wl_rate", "9000000"); %>>9 Mbps</option>
										<option value="11000000" <% nvram_selected("wl_rate", "11000000"); %>>11 Mbps</option>
										<option value="12000000" <% nvram_selected("wl_rate", "12000000"); %>>12 Mbps</option>
										<option value="18000000" <% nvram_selected("wl_rate", "18000000"); %>>18 Mbps</option>
										<option value="24000000" <% nvram_selected("wl_rate", "24000000"); %>>24 Mbps</option>
										<option value="36000000" <% nvram_selected("wl_rate", "36000000"); %>>36 Mbps</option>
										<option value="48000000" <% nvram_selected("wl_rate", "48000000"); %>>48 Mbps</option>
										<option value="54000000" <% nvram_selected("wl_rate", "54000000"); %>>54 Mbps</option>
										<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selected("wl_rate", "0", "js"); %>>" + share.auto + "</option>");</script>
									</select>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.auto + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label4)</script></div>
									<input class="spaceradio" type="radio" name="wl_gmode_protection" value="auto" <% nvram_checked("wl_gmode_protection", "auto"); %>><script type="text/javascript">Capture(share.auto)</script></input>&nbsp;
									<input class="spaceradio" type="radio" name="wl_gmode_protection" value="off" <% nvram_checked("wl_gmode_protection", "off"); %>><script type="text/javascript">Capture(share.disable)</script></input>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.disable + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label5)</script></div>
									<input class="spaceradio" type="radio" name="wl_frameburst" value="on" <% nvram_checked("wl_frameburst", "on"); %>><script type="text/javascript">Capture(share.enable)</script></input>&nbsp;
									<input class="spaceradio" type="radio" name="wl_frameburst" value="off" <% nvram_checked("wl_frameburst", "off"); %>><script type="text/javascript">Capture(share.disable)</script></input>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.disable + ")")</script></span>
								</div><br />
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label6)</script></div>
									<input class="num" name="wl_bcn" size="6" maxlength="5" onblur="valid_range(this,1,65535,wl_adv.label6)" value="<% nvram_selget("wl_bcn"); %>" />&nbsp;
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": 100ms, " + share.range + ": 1 - 65535)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label7)</script></div>
									<input class="num" name="wl_dtim" size="6" maxlength="3" onblur="valid_range(this,1,255,wl_adv.label7)" value="<% nvram_selget("wl_dtim"); %>" />&nbsp;
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": ")</script><% get_wl_value("default_dtim"); %><script type="text/javascript">document.write(", " + share.range + ": 1 - 255)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label8)</script></div>
									<input class="num" name="wl_frag" size="6" maxlength="4" onblur="valid_range(this,256,2346,wl_adv.label8)" value="<% nvram_selget("wl_frag"); %>" />&nbsp;
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": 2346, " + share.range + ": 256 - 2346)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label9)</script></div>
									<input class="num" name="wl_rts" size="6" maxlength="4" onblur="valid_range(this,0,2347,wl_adv.label9)" value="<% nvram_selget("wl_rts"); %>" />&nbsp;
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": 2347, " + share.range + ": 0 - 2347)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label10)</script></div>
									<input class="num" name="wl_maxassoc" size="6" maxlength="4" onblur="valid_range(this,1,256,wl_adv.label10)" value="<% nvram_selget("wl_maxassoc"); %>" />&nbsp;
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": 128, " + share.range + ": 1 - 256)")</script></span>
							 	</div><br />
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label11)</script></div>
									<input class="spaceradio" type="radio" name="wl_ap_isolate" value="1" <% nvram_checked("wl_ap_isolate", "1"); %>><script type="text/javascript">Capture(share.enable)</script></input>&nbsp;
									<input class="spaceradio" type="radio" name="wl_ap_isolate" value="0" <% nvram_checked("wl_ap_isolate", "0"); %>><script type="text/javascript">Capture(share.disable)</script></input>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.disable + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label12)</script></div>
									<select name="txant">
										<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selected("txant", "0", "js"); %>>" + share.right + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"1\" <% nvram_selected("txant", "1", "js"); %>>" + share.left + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"3\" <% nvram_selected("txant", "3", "js"); %>>" + share.auto + "</option>");</script>
									</select>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.auto + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label13)</script></div>
									<select name="wl_antdiv">
										<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selected("wl_antdiv", "0", "js"); %>>" + share.right + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"1\" <% nvram_selected("wl_antdiv", "1", "js"); %>>" + share.left + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"3\" <% nvram_selected("wl_antdiv", "3", "js"); %>>" + share.auto + "</option>");</script>
									</select>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.auto + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label14)</script></div>
									<select name="wl_plcphdr">
										<script type="text/javascript">document.write("<option value=\"long\" <% nvram_selected("wl_plcphdr", "long", "js"); %>>" + wl_adv.lng + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"short\" <% nvram_selected("wl_plcphdr", "short", "js"); %>>" + wl_adv.shrt + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"auto\" <% nvram_selected("wl_plcphdr", "auto", "js"); %>>" + share.auto + "</option>");</script>
									</select>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + wl_adv.lng + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label15)</script></div>
									<input class="num" name="txpwr" size="6" maxlength="3" onblur="valid_range(this,0,251,wl_adv.label15)" value='<% nvram_selget("txpwr"); %>' />
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": 28, " + share.range + ": 0 - 251mW)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.reference)</script></div>
									<input class="num" name="noise_reference" size="6" maxlength="3" value='<% nvram_selget("noise_reference"); %>' />
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": -98, " + share.range + ": 0 - -100db)")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label16)</script></div>
									<select name="wl_afterburner">
										<script type="text/javascript">document.write("<option value=\"off\" <% nvram_selected("wl_afterburner", "off", "js"); %>>" + share.disable + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"on\" <% nvram_selected("wl_afterburner", "on", "js"); %>>" + share.enable + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"auto\" <% nvram_selected("wl_afterburner", "auto", "js"); %>>" + share.auto + "</option>");</script>
									</select>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label17)</script></div>
									<input class="spaceradio" type="radio" name="web_wl_filter" value="0" <% nvram_checked("web_wl_filter", "0"); %> /><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" name="web_wl_filter" value="1" <% nvram_checked("web_wl_filter", "1"); %> /><script type="text/javascript">Capture(share.disable)</script>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.enable + ")")</script></span>
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(wl_adv.legend2)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label18)</script></div>
									<input class="spaceradio" type="radio" name="wl_wme" value="on" <% nvram_checked("wl_wme", "on"); %>  onclick="setWMM(this.value)" /><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" name="wl_wme" value="off" <% nvram_checked("wl_wme", "off"); %>  onclick="setWMM(this.value)" /><script type="text/javascript">Capture(share.disable)</script>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.disable + ")")</script></span>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(wl_adv.label19)</script></div>
									<input class="spaceradio" type="radio" name="wl_wme_no_ack" value="on" <% nvram_checked("wl_wme_no_ack", "on"); %> /><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" name="wl_wme_no_ack" value="off" <% nvram_checked("wl_wme_no_ack", "off"); %> /><script type="text/javascript">Capture(share.disable)</script>
									<span class="default"><script type="text/javascript">document.write("(" + share.deflt + ": " + share.disable + ")")</script></span>
								</div>
								<table class="table center" cellspacing="5">
									<tr>
										<th colspan="7"><script type="text/javascript">Capture(wl_adv.table1)</script></td>
									</tr>
									<tr>
										<td>&nbsp;</td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col1)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col2)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col3)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col4)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col5)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col6)</script></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row1)</script><input type="hidden" name="wl_wme_ap_bk" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_bk0" value="<% nvram_list("wl_wme_ap_bk", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_bk1" value="<% nvram_list("wl_wme_ap_bk", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_bk2" value="<% nvram_list("wl_wme_ap_bk", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_bk3" value="<% nvram_list("wl_wme_ap_bk", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_bk4" value="<% nvram_list("wl_wme_ap_bk", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_ap_bk5" value="<% nvram_list("wl_wme_ap_bk", 5); %>" /><input type="checkbox" name="_wl_wme_ap_bk5" <% wme_match_op("wl_wme_ap_bk", "on", "checked='checked'"); %> onchange="this.form.wl_wme_ap_bk5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row2)</script><input type="hidden" name="wl_wme_ap_be" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_be0" value="<% nvram_list("wl_wme_ap_be", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_be1" value="<% nvram_list("wl_wme_ap_be", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_be2" value="<% nvram_list("wl_wme_ap_be", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_be3" value="<% nvram_list("wl_wme_ap_be", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_be4" value="<% nvram_list("wl_wme_ap_be", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_ap_be5" value="<% nvram_list("wl_wme_ap_be", 5); %>" /><input type="checkbox" name="_wl_wme_ap_be5" <% wme_match_op("wl_wme_ap_be", "on", "checked='checked'"); %> onchange="this.form.wl_wme_ap_be5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row3)</script><input type="hidden" name="wl_wme_ap_vi" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vi0" value="<% nvram_list("wl_wme_ap_vi", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vi1" value="<% nvram_list("wl_wme_ap_vi", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vi2" value="<% nvram_list("wl_wme_ap_vi", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vi3" value="<% nvram_list("wl_wme_ap_vi", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vi4" value="<% nvram_list("wl_wme_ap_vi", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_ap_vi5" value="<% nvram_list("wl_wme_ap_vi", 5); %>" /><input type="checkbox" name="_wl_wme_ap_vi5" <% wme_match_op("wl_wme_ap_vi", "on", "checked='checked'"); %> onchange="this.form.wl_wme_ap_vi5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row4)</script><input type="hidden" name="wl_wme_ap_vo" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vo0" value="<% nvram_list("wl_wme_ap_vo", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vo1" value="<% nvram_list("wl_wme_ap_vo", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vo2" value="<% nvram_list("wl_wme_ap_vo", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vo3" value="<% nvram_list("wl_wme_ap_vo", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_ap_vo4" value="<% nvram_list("wl_wme_ap_vo", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_ap_vo5" value="<% nvram_list("wl_wme_ap_vo", 5); %>" /><input type="checkbox" name="_wl_wme_ap_vo5" <% wme_match_op("wl_wme_ap_vo", "on", "checked='checked'"); %> onchange="this.form.wl_wme_ap_vo5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
								</table>
								<table cellspacing="5">
									<tr>
										<th colspan="7"><script type="text/javascript">Capture(wl_adv.table2)</script></td>
									</tr>
									<tr>
										<td>&nbsp;</td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col1)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col2)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col3)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col4)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col5)</script></td>
										<td align="center"><script type="text/javascript">Capture(wl_adv.col6)</script></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row1)</script><input type="hidden" name="wl_wme_sta_bk" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_bk0" value="<% nvram_list("wl_wme_sta_bk", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_bk1" value="<% nvram_list("wl_wme_sta_bk", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_bk2" value="<% nvram_list("wl_wme_sta_bk", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_bk3" value="<% nvram_list("wl_wme_sta_bk", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_bk4" value="<% nvram_list("wl_wme_sta_bk", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_sta_bk5" value="<% nvram_list("wl_wme_sta_bk", 5); %>" /><input type="checkbox" name="_wl_wme_sta_bk5" <% wme_match_op("wl_wme_sta_bk", "on", "checked='checked'"); %> onchange="this.form.wl_wme_sta_bk5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row2)</script><input type="hidden" name="wl_wme_sta_be" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_be0" value="<% nvram_list("wl_wme_sta_be", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_be1" value="<% nvram_list("wl_wme_sta_be", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_be2" value="<% nvram_list("wl_wme_sta_be", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_be3" value="<% nvram_list("wl_wme_sta_be", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_be4" value="<% nvram_list("wl_wme_sta_be", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_sta_be5" value="<% nvram_list("wl_wme_sta_be", 5); %>" /><input type="checkbox" name="_wl_wme_sta_be5" <% wme_match_op("wl_wme_sta_be", "on", "checked='checked'"); %> onchange="this.form.wl_wme_sta_be5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row3)</script><input type="hidden" name="wl_wme_sta_vi" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vi0" value="<% nvram_list("wl_wme_sta_vi", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vi1" value="<% nvram_list("wl_wme_sta_vi", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vi2" value="<% nvram_list("wl_wme_sta_vi", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vi3" value="<% nvram_list("wl_wme_sta_vi", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vi4" value="<% nvram_list("wl_wme_sta_vi", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_sta_vi5" value="<% nvram_list("wl_wme_sta_vi", 5); %>" /><input type="checkbox" name="_wl_wme_sta_vi5" <% wme_match_op("wl_wme_sta_vi", "on", "checked='checked'"); %> onchange="this.form.wl_wme_sta_vi5.value = (this.checked ? 'on' : 'off');" /></td>
									</tr>
									<tr>
										<td><script type="text/javascript">Capture(wl_adv.row4)</script><input type="hidden" name="wl_wme_sta_vo" value="5" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vo0" value="<% nvram_list("wl_wme_sta_vo", 0); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col1)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vo1" value="<% nvram_list("wl_wme_sta_vo", 1); %>" size="5" maxlength="6" onblur="valid_range(this,0,32767,wl_adv.col2)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vo2" value="<% nvram_list("wl_wme_sta_vo", 2); %>" size="5" maxlength="6" onblur="valid_range(this,1,15,wl_adv.col3)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vo3" value="<% nvram_list("wl_wme_sta_vo", 3); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col4)" /></td>
										<td align="center"><input class="num" name="wl_wme_sta_vo4" value="<% nvram_list("wl_wme_sta_vo", 4); %>" size="5" maxlength="6" onblur="valid_range(this,0,65504,wl_adv.col5)" /></td>
										<td align="center"><input type="hidden" name="wl_wme_sta_vo5" value="<% nvram_list("wl_wme_sta_vo", 5); %>" /><input type="checkbox" name="_wl_wme_sta_vo5" <% wme_match_op("wl_wme_sta_vo", "on", "checked='checked'"); %> onchange="this.form.wl_wme_sta_vo5.value = (this.checked ? 'on' : 'off');" /></td>
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
							<dt class="term"><script type="text/javascript">Capture(wl_adv.label)</script>:</dt>
							<dd class="definition"><script type="text/javascript">Capture(hwl_adv.right2)</script></dd>
						</dl><br />
						<a href="javascript:openHelpWindow('HWirelessAdvanced.asp');"><script type="text/javascript">Capture(share.more)</script></a>
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