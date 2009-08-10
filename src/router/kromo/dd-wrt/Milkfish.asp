<!--##################################################################
# This program is free software; you can redistribute it and/or      #
# modify it under the terms of the GNU General Public License as     #
# published by the Free Software Foundation; either version 3 of the #
# License, or (at your option) any later version.                    #
#                                                                    #
# This program is distributed in the hope that it will be useful,    #
# but WITHOUT ANY WARRANTY; without even the implied warranty of     #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      #
# GNU General Public License for more details.                       #
#                                                                    #
# You should have received a copy of the GNU General Public License  #
# along with this program; if not, write to the                      #
# Free Software Foundation, Inc.,                                    #
# 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA             #
######################################################################
# This file is part of a Milkfish Embedded OpenSER SIP Router Setup  #
# and was derived from similar files in this folder by other authors #
#                                                                    #
# The Milkfish-dd Web Interface - Main Page                          #
#                                                                    #
# Built/Version:  20071229                                           #
# Co-Author/Contact: Michael Poehnl <budrus@sipwerk.com>             #
# Copyright (C) 2007 partly by sipwerk - All rights reserved.        #
#                                                                    #
# Please note that this software is under development and comes with #
# absolutely no warranty, to the extend permitted by applicable law. #
###################################################################-->
<% do_pagehead("bmenu.servicesMilkfish"); %>
		<script type="text/javascript">
		//<![CDATA[

function to_submit(F) {
	F.change_action.value = "";
	F.submit_type.value = "";
	F.save_button.value = sbutton.saving;
	apply(F);
}
function to_apply(F) {
	F.change_action.value = "";
	F.submit_type.value = "";
	F.save_button.value = sbutton.saving;
	applytake(F);
}
function to_reboot(F) {
        F.change_action.value = "";
        F.submit_type.value = "";
        F.action.value = "Reboot";
        apply(F);
}

var update;

addEvent(window, "load", function() {

		show_layer_ext(document.setup.milkfish_fromswitch, 'idfrom', <% nvram_else_match("milkfish_fromswitch", "on", "1", "0"); %> == 1);
		show_layer_ext(document.setup.milkfish_enabled, 'idmilkfish', <% nvram_else_match("milkfish_enabled", "1", "1", "0"); %> == 1);

		update = new StatusbarUpdate();
		update.start();
		
});

addEvent(window, "unload", function() {
	update.stop();

});		
		//]]>
		</script>
	</head>

	<body class="gui">
		<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<% do_menu("Services.asp","Milkfish.asp"); %>
				</div>
				<div id="main">
					<div id="contents">
						<form name="setup" action="applyuser.cgi" method="post">
							<input type="hidden" name="submit_button" value="Milkfish" />
							<input type="hidden" name="action" value="Apply" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="commit" value="1" />
							
	<h2><% tran("bmenu.servicesMilkfish"); %></h2>

	<fieldset>
		<legend><% tran("bmenu.servicesMilkfish"); %></legend>
			<div class="setting">
			<div class="label"><% tran("service.milkfish_mainswitch"); %></div>
				<input class="spaceradio" type="radio" name="milkfish_enabled" value="1" <% nvram_checked("milkfish_enabled", "1"); %> onclick="show_layer_ext(this, 'idmilkfish', true)" /><% tran("share.enable"); %>&nbsp;
				<input class="spaceradio" type="radio" name="milkfish_enabled" value="0" <% nvram_checked("milkfish_enabled", "0"); %> onclick="show_layer_ext(this, 'idmilkfish', false)" /><% tran("share.disable"); %>
			</div>
<div id="idmilkfish">
		<div class="setting">
			<div class="label"><% tran("service.milkfish_fromswitch"); %></div>
			<input class="spaceradio" type="radio" name="milkfish_fromswitch" value="on" <% nvram_checked("milkfish_fromswitch", "on"); %> onclick="show_layer_ext(this, 'idfrom', true)" /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" name="milkfish_fromswitch" value="off" <% nvram_checked("milkfish_fromswitch", "off"); %> onclick="show_layer_ext(this, 'idfrom', false)" /><% tran("share.disable"); %>
		</div>
<div id="idfrom">
		<div class="setting">
			<div class="label"><% tran("service.milkfish_fromdomain"); %></div>
			<input size="27" name="milkfish_fromdomain" value="<% nvram_get("milkfish_fromdomain"); %>" />
		</div>
</div>
		<div class="setting">
			<div class="label"><% tran("service.milkfish_username"); %></div>
			<input size="27" name="milkfish_username" value="<% nvram_get("milkfish_username"); %>" />
		</div>
		<div class="setting">
			<div class="label"><% tran("service.milkfish_password"); %></div>
			<input type="password" size="27" name="milkfish_password" value="<% nvram_get("milkfish_password"); %>" />
		</div>
<!--		<div class="setting">
			<div class="label"><% tran("service.milkfish_audit"); %></div>
			<input class="spaceradio" type="radio" name="milkfish_audit" value="on" <% nvram_checked("milkfish_audit", "on"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" name="milkfish_audit" value="off" <% nvram_checked("milkfish_audit", "off"); %> /><% tran("share.disable"); %>
		</div>-->
		<div class="setting">
			<div class="label"><% tran("service.milkfish_siptrace"); %></div>
			<input class="spaceradio" type="radio" name="milkfish_siptrace" value="on" <% nvram_checked("milkfish_siptrace", "on"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" name="milkfish_siptrace" value="off" <% nvram_checked("milkfish_siptrace", "off"); %> /><% tran("share.disable"); %>
		</div>
		<div class="setting">
			<div class="label"><% tran("service.milkfish_dynsip"); %></div>
			<input class="spaceradio" type="radio" name="milkfish_dynsip" value="on" <% nvram_checked("milkfish_dynsip", "on"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" name="milkfish_dynsip" value="off" <% nvram_checked("milkfish_dynsip", "off"); %> /><% tran("share.disable"); %>
		</div>
	<br />

	<fieldset>
		<legend><% tran("service.milkfish_status"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"SIP Status\" onclick=\"openWindow('Milkfish_status.asp', 820, 730);\" />");
				//]]>
				</script>
	</fieldset>
	
<br />

	<fieldset>
		<legend><% tran("service.milkfish_phonebook"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"SIP Phonebook\" onclick=\"openWindow('Milkfish_phonebook.asp', 820, 730);\" />");
				//]]>
				</script>
	</fieldset>
	
<br />



	<fieldset>
		<legend><% tran("service.milkfish_database"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"Local Subscribers\" onclick=\"openWindow('Milkfish_database.asp', 820, 730);\" />");
				//]]>
                                //<![CDATA[
                                document.write("<input class=\"button\" type=\"button\" value=\"Local Aliases\" onclick=\"openWindow('Milkfish_aliases.asp', 820, 730);\" />");
                                //]]>
				</script>
	</fieldset>
	
<br />



	<fieldset>
		<legend><% tran("service.milkfish_messaging"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"SIP Messaging\" onclick=\"openWindow('Milkfish_messaging.asp', 820, 730);\" />");
				//]]>
				</script>


	</fieldset>
	
<br />



	<fieldset>
		<legend><% tran("service.milkfish_siptrace"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"SIP Trace\" onclick=\"openWindow('Milkfish_siptrace.asp', 820, 730);\" />");
				//]]>
				</script>


	</fieldset>
	
<br />

	<fieldset>
		<legend><% tran("service.milkfish_dynsip"); %></legend>
				<script type="text/javascript">
				//<![CDATA[
				document.write("<input class=\"button\" type=\"button\" value=\"Advanced DynSIP Settings\" onclick=\"openWindow('Milkfish_dynsip.asp', 820, 730);\" />");
				//]]>
				</script>
	</fieldset>

<br />
	<fieldset>
            <div class="center"><br />
		<b>Problems, Questions, Suggestions? - Find answers in the <a href="http://forum.milkfish.org" target="_blank">Milkfish-dd User Forum</a></b><br />
		<br />
	    </div>
	</fieldset>
<!--	<fieldset>

	    <div class="center">powered by<br><img src="images/sipwerk.png" alt="sipwerk logo" /></div><br />
	    
	</fieldset>
-->
<br />	
</div>
</fieldset>
<br/>
							<div class="submitFooter">
								<script type="text/javascript">
								//<![CDATA[
								submitFooterButton(1,1,1);
								//]]>
								</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div><h2><% tran("share.help"); %></h2></div>
                                               <dl>
                                                        <dt class="term"><% tran("service.milkfish_mainswitch"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right2"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_fromswitch"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right4"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_fromdomain"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right6"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_username"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right8"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_password"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right10"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_siptrace"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right12"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_dynsip"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right18"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_subscribers"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right14"); %></dd>
                                                        <dt class="term"><% tran("service.milkfish_aliases"); %>:</dt>
                                                        <dd class="definition"><% tran("service.hmilkfish_right16"); %></dd>
                                                </dl>
						<br/>
						<a href="javascript:openHelpWindow<% ifdef("EXTHELP","Ext"); %>('HMilkfish.asp');"><% tran("share.more"); %></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
				<div class="info"><% tran("share.firmware"); %>: 
					<script type="text/javascript">
					//<![CDATA[
					document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");
					//]]>
					</script>
				</div>
				<div class="info"><% tran("share.time"); %>:  <span id="uptime"><% get_uptime(); %></span></div>
				<div class="info">WAN<span id="ipinfo"><% show_wanipinfo(); %></span></div>
				</div>
			</div>
		</div>
	</body>
</html>
