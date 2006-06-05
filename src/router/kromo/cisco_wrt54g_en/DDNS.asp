<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %> - DDNS</title>
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + ddns.titl;

function ddns_check(F,T) {
	if(F.ddns_enable.value == 0) {
		return true;
	} else if(F.ddns_enable.value == 1) {
		username = eval("F.ddns_username");
		passwd = eval("F.ddns_passwd");
		hostname = eval("F.ddns_hostname");
		dyndnstype = eval("F.ddns_dyndnstype");
		wildcard = eval("F.ddns_wildcard");
	} else if(F.ddns_enable.value == 2) {
		username = eval("F.ddns_username_"+F.ddns_enable.value);
		passwd = eval("F.ddns_passwd_"+F.ddns_enable.value);
		hostname = eval("F.ddns_hostname_"+F.ddns_enable.value);
	} else if(F.ddns_enable.value == 3) {
		username = eval("F.ddns_username_"+F.ddns_enable.value);
		passwd = eval("F.ddns_passwd_"+F.ddns_enable.value);
		hostname = eval("F.ddns_hostname_"+F.ddns_enable.value);
	}
	if(username.value == "") {
//	alert("You must input a username.");
		alert(errmsg.err0);
		username.focus();
		return false;
	}
	if(passwd.value == "") {
//	alert("You must input a password.");
		alert(errmsg.err6);
		passwd.focus();
		return false;
	}
	if(hostname.value == "") {
//	alert("You must input a hostname.");
    alert(errmsg.err7);
		hostname.focus();
		return false;
	}

	return true;
}

function to_save(F) {
	if(!ddns_check(F,"update")) return;
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "DDNS";
	F.submit_type.value = "save";
	F.action.value = "Apply";
	F.submit();
}

function to_submit(F) {
	if(!ddns_check(F,"save")) return;
	F.submit_button.value = "DDNS";
//	F.save_button.value = "Saved";
	F.save_button.value = sbutton.saving;
	
	F.action.value = "Apply";
	apply(F);
}

function SelDDNS(num,F) {
	F.submit_button.value = "DDNS";
	F.change_action.value = "gozila_cgi";
	F.ddns_enable.value=F.ddns_enable.options[num].value;
	F.submit();
}

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
								<li class="current"><span><% tran("bmenu.setup"); %></span>
									<div id="menuSub">
										<ul id="menuSubList">
	  										<li><a href="index.asp"><% tran("bmenu.setupbasic"); %></a></li>
	  										<li><span><% tran("bmenu.setupddns"); %></span></li>
	  										<li><a href="WanMAC.asp"><% tran("bmenu.setupmacclone"); %></a></li>
	  										<li><a href="Routing.asp"><% tran("bmenu.setuprouting"); %></a></li>
	  										<li><a href="Vlan.asp"><% tran("bmenu.setupvlan"); %></a></li>
  										</ul>
  									</div>
  								</li>
  								<li><a href="Wireless_Basic.asp"><% tran("bmenu.wireless"); %></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><% tran("bmenu.sipath"); %></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><% tran("bmenu.security"); %></a></li>
								<li><a href="Filters.asp"><% tran("bmenu.accrestriction"); %></a></li>
								<li><a href="Forward.asp"><% tran("bmenu.applications"); %></a></li>
								<li><a href="Management.asp"><% tran("bmenu.admin"); %></a></li>
								<li><a href="Status_Router.asp"><% tran("bmenu.statu"); %></a></li>
							</ul>
						</div>
					</div>
				</div>
				<div id="main">
					<div id="contents">
						<form name="ddns" action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="action" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<h2><% tran("ddns.h2"); %></h2>
							
							<fieldset>
								<legend><% tran("ddns.legend"); %></legend>
								<div class="setting">
									<div class="label"><% tran("ddns.srv"); %></div>
									<select name="ddns_enable" onchange="SelDDNS(this.form.ddns_enable.selectedIndex,this.form)">
										<script type="text/javascript">document.write("<option value=\"0\" <% nvram_selmatch("ddns_enable", "0", "selected"); %> >" + share.disable + "</option>");</script>
										<option value="1" <% nvram_selmatch("ddns_enable", "1", "selected"); %> >DynDNS.org</option>
										<option value="2" <% nvram_selmatch("ddns_enable", "2", "selected"); %> >TZO.com</option>
										<option value="3" <% nvram_selmatch("ddns_enable", "3", "selected"); %> >ZoneEdit.com</option>
									</select>
								</div>
								<% nvram_selmatch("ddns_enable","0","<!--"); %>
								<% nvram_selmatch("ddns_enable","2","<!--"); %>
								<% nvram_selmatch("ddns_enable","3","<!--"); %>
								<div class="setting">
									<div class="label"><% tran("share.usrname"); %></div>
									<input name="ddns_username" size="30" maxlength="32" onblur="valid_name(this,share.usrname)" value="<% nvram_get("ddns_username"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.passwd"); %></div>
									<input type="password" name="ddns_passwd" size="30" maxlength="32" onblur="valid_name(this,share.passwd)" value="<% nvram_invmatch("ddns_passwd","","d6nw5v1x2pc7st9m"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.hostname"); %></div>
									<input name="ddns_hostname" size="42" maxlength="48" onblur="valid_name(this,share.hostname)" value="<% nvram_get("ddns_hostname"); %>" />
								</div>
								<% nvram_selmatch("ddns_enable","2","-->"); %>
								<% nvram_selmatch("ddns_enable","3","-->"); %>
								<% nvram_selmatch("ddns_enable","1","<!--"); %>
								<% nvram_selmatch("ddns_enable","3","<!--"); %>
								<div class="setting">
									<div class="label"><% tran("ddns.emailaddr"); %></div>
									<input name="ddns_username_2" size="30" maxlength="32" onblur="valid_name(this,ddns.emailaddr)" value="<% nvram_get("ddns_username_2"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.passwd"); %></div>
									<input type="password" name="ddns_passwd_2" size="30" maxlength="32" onblur="valid_name(this,share.passwd)" value="<% nvram_invmatch("ddns_passwd_2","","d6nw5v1x2pc7st9m"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.domainname"); %></div>
									<input name="ddns_hostname_2" size="42" maxlength="48" onblur="valid_name(this,share.domainname)" value="<% nvram_get("ddns_hostname_2"); %>" />
								</div>
								<% nvram_selmatch("ddns_enable","1","-->"); %>
								<% nvram_selmatch("ddns_enable","3","-->"); %>
								<% nvram_selmatch("ddns_enable","1","<!--"); %>
								<% nvram_selmatch("ddns_enable","2","<!--"); %>
								<div class="setting">
									<div class="label"><% tran("share.usrname"); %></div>
									<input name="ddns_username_3" size="30" maxlength="32" onblur="valid_name(this,share.usrname)" value="<% nvram_get("ddns_username_3"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.passwd"); %></div>
									<input type="password" name="ddns_passwd_3" size="30" maxlength="32" onblur="valid_name(this,share.passwd)" value="<% nvram_invmatch("ddns_passwd_3","","d6nw5v1x2pc7st9m"); %>" />
								</div>
								<div class="setting">
									<div class="label"><% tran("share.hostname"); %></div>
									<input name="ddns_hostname_3" size="42" maxlength="48" onblur="valid_name(this,share.hostname)" value="<% nvram_get("ddns_hostname_3"); %>" />
								</div>
								<% nvram_selmatch("ddns_enable","1","-->"); %>
								<% nvram_selmatch("ddns_enable","2","-->"); %>
								<div class="setting">
									<div class="label"><% tran("ddns.typ"); %></div>
									<select name="ddns_dyndnstype">
										<script type="text/javascript">document.write("<option value=\"1\" <% nvram_selmatch("ddns_dyndnstype", "1", "selected"); %> >" + ddns.dynamic + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"2\" <% nvram_selmatch("ddns_dyndnstype", "2", "selected"); %> >" + share.sttic + "</option>");</script>
										<script type="text/javascript">document.write("<option value=\"3\" <% nvram_selmatch("ddns_dyndnstype", "3", "selected"); %> >" + ddns.custom + "</option>");</script>
									</select>
								</div>
								<% nvram_selmatch("ddns_enable","2","<!--"); %>
								<% nvram_selmatch("ddns_enable","3","<!--"); %>
								<div class="setting">
									<div class="label"><% tran("ddns.wildcard"); %></div>
									<input type="checkbox" value="1" name="ddns_wildcard" <% nvram_checked("ddns_wildcard", "1"); %> />
								</div>
								<% nvram_selmatch("ddns_enable","2","-->"); %>
								<% nvram_selmatch("ddns_enable","3","-->"); %>
							</fieldset><br />
							<fieldset>
								<legend><% tran("ddns.statu"); %></legend>
								<div class="setting">
									<div class="label"><% tran("share.statu"); %></div>
									<script type="text/javascript">Capture(<% show_ddns_status(); %>)</script>&nbsp;
								</div>
								<div class="setting">
									<div class="label"><% tran("share.interipaddr"); %></div>
									<% show_ddns_ip(); %>&nbsp;
								</div>
								<% nvram_selmatch("ddns_enable","0","-->"); %>
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
							<h2><% tran("share.help"); %></h2>
						</div>
						<dl>
							<dt class="term"><% tran("ddns.srv"); %>:</dt>
							<dd class="definition"><% tran("hddns.right2"); %></dd>
						</dl><br />
						<a href="javascript:openHelpWindow<% nvram_selmatch("dist_type","micro","Ext"); %>('HAlive.asp');">('HDDNS.asp');"><% tran("share.more"); %></a>
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