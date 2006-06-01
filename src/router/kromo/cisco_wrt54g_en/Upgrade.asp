<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<% charset(); %>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<title><% nvram_get("router_name"); %> - Firmware Upgrade</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + upgrad.titl;

function process_aborted(F) {
	bar1.togglePause();
//	alert("Upgrade failed.");
	alert(fail.mess2);
	window.location.replace("Upgrade.asp")
	return false;
}

function upgrade(F,id) {
	if (F.file.value == "")	{
//		alert("Please select a file to upgrade.");
		alert(errmsg.err60);
		return false;
	}
	var len = F.file.value.length;
	var ext = new Array('.','b','i','n');
	var IMAGE = F.file.value.toLowerCase();
	for (i=0; i < 4; i++)	{
		if (ext[i] != IMAGE.charAt(len-4+i)){
//			alert("Incorrect image file.");
			alert(errmsg.err61);
			return false;
		}
	}

	choose_disable(F.Upgrade_b);
	bar1.togglePause();
	change_style(id,'textblink');
	F.Upgrade_b.value = sbutton.upgrading;
	
	F.submit_button.value = "Upgrade";
	apply(F);
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
								<li><a href="index.asp"><% tran("bmenu.setup"); %></a></li>
								<li><a href="Wireless_Basic.asp"><% tran("bmenu.wireless"); %></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><% tran("bmenu.sipath"); %></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><% tran("bmenu.security"); %></a></li>
								<li><a href="Filters.asp"><% tran("bmenu.accrestriction"); %></a></li>
								<li><a href="Forward.asp"><% tran("bmenu.applications"); %></a></li>
								<li class="current"><span><% tran("bmenu.admin"); %></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><a href="Management.asp"><% tran("bmenu.adminManagement"); %></a></li>
											<li><a href="Hotspot.asp"><% tran("bmenu.adminHotspot"); %></a></li>
											<li><a href="Services.asp"><% tran("bmenu.adminServices"); %></a></li>
											<li><a href="Alive.asp"><% tran("bmenu.adminAlive"); %></a></li>
											<li><a href="Log.asp"><% tran("bmenu.adminLog"); %></a></li>
											<li><a href="Diagnostics.asp"><% tran("bmenu.adminDiag"); %></a></li>
											<li><a href="Wol.asp"><% tran("bmenu.adminWol"); %></a></li>
											<li><a href="Factory_Defaults.asp"><% tran("bmenu.adminFactory"); %></a></li>
											<li><span><% tran("bmenu.adminUpgrade"); %></span></li>
											<li><a href="config.asp"><% tran("bmenu.adminBackup"); %></a></li>
										</ul>
									</div>
								</li>
								<li><a href="Status_Router.asp"><% tran("bmenu.statu"); %></a></li>
							</ul>
						</div>
					</div>
				</div>
				<div id="main">
					<div id="contents">
						<form name="firmware" method="post" action="upgrade.cgi" enctype="multipart/form-data">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="action" />
							<input type="hidden" name="change_action" />
							<h2><% tran("upgrad.h2"); %></h2>
							
							<fieldset>
								<legend><% tran("upgrad.legend"); %></legend>
								<div class="setting">
									<div class="label"><% tran("upgrad.info1"); %></div>
									<input class="spaceradio" type="radio" value="0" name="erase" checked="checked" /><% tran("upgrad.resetOff"); %>&nbsp;
									<input class="spaceradio" type="radio" value="1" name="erase" /><% tran("upgrad.resetOn"); %>
								</div>
								<div class="setting">
									<div class="label"><% tran("upgrad.file"); %></div>
									<input type="file" name="file" size="40"/>
								</div>
							</fieldset><br />
							
							<div class="warning">
								<p><div id="warning_text"><b><% tran("upgrad.warning"); %></b></div></p>
								<p><% tran("upgrad.mess1"); %></p>
								<div align="center"><script type="text/javascript">
									var bar1 = createBar(500,15,100,15,50,"process_aborted(this.form)");
									bar1.togglePause();
								</script></div><br />
							</div><br />
							<div class="submitFooter">
								<script type="text/javascript">document.write("<input type=\"button\" name=\"Upgrade_b\" value=\"" + sbutton.upgrade + "\" onclick=\"upgrade(this.form,'warning_text')\" />")</script>
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
							<dt class="term"><% tran("upgrad.legend"); %>: </dt>
							<dd class="definition"><% tran("hupgrad.right2"); %></dd>
						</dl>
						<br/>
						<a href="javascript:openHelpWindow('HUpgrade.asp');"><% tran("share.more"); %></a>
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
