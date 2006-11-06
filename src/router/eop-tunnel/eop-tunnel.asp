<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %> - EOP Tunnel</title>
		<script type="text/javascript">
//<![CDATA[
document.title = "<% nvram_get("router_name"); %> EOP Tunnel";

function to_submit(F) {
	F.submit_button.value = "EOP";
//	F.save_button.value = "Saved";
	F.save_button.value = sbutton.saving;
	
	F.action.value = "Apply";
	apply(F);
}

	</head>

	<body class="gui">
		<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<% do_menu("index.asp","eop-tunnel.asp"); %>
				</div>
				<div id="main">
					<div id="contents">
						<form name="eop" action="apply.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="action" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />

<fieldset>
	<legend><% tran("service.eoip_legend"); %></legend>
		<div class="setting">
			<div class="label"><% tran("service.eoip_srv"); %></div>
			<input class="spaceradio" type="radio" value="1" name="oet1_en" <% nvram_selmatch("oet1_en","1","checked"); %> onclick="show_layer_ext(this, 'idoet', true)" /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" value="0" name="oet1_en" <% nvram_selmatch("oet1_en","0","checked"); %> onclick="show_layer_ext(this, 'idoet', false)" /><% tran("share.disable"); %>
		</div>
	<div id="idoet">
		<div class="setting">
			<div class="label"><% tran("service.eoip_remoteIP"); %></div>
			<input type="hidden" name="oet1_rem" value="0.0.0.0"/>
			<input size="3" maxlength="3" class="num" name="oet1_rem_0" onblur="valid_range(this,0,255,service.eoip_remoteIP)" value="<% get_single_ip("oet1_rem","0"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_rem_1" onblur="valid_range(this,0,255,service.eoip_tunnelID)" value="<% get_single_ip("oet1_rem","1"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_rem_2" onblur="valid_range(this,0,255,service.eoip_tunnelID)" value="<% get_single_ip("oet1_rem","2"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_rem_3" onblur="valid_range(this,1,254,service.eoip_tunnelID)" value="<% get_single_ip("oet1_rem","3"); %>" />
		</div>
		<div class="setting">
			<div class="label"><% tran("service.eoip_tunnelID"); %></div>
			<input size="4" maxlength="3" class="num" name="oet1_id" onblur="valid_range(this,0,999,service.eoip_tunnelID)" value="<% nvram_get("oet1_id"); %>" />
		</div>			
		<div class="setting">
			<div class="label"><% tran("service.eoip_comp"); %></div>
			<input class="spaceradio" type="radio" value="1" name="oet1_comp" <% nvram_selmatch("oet1_comp","1","checked"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" value="0" name="oet1_comp" <% nvram_selmatch("oet1_comp","0","checked"); %> /><% tran("share.disable"); %>
		</div>
		<div class="setting">
			<div class="label"><% tran("service.eoip_passtos"); %></div>
			<input class="spaceradio" type="radio" value="1" name="oet1_pt" <% nvram_selmatch("oet1_pt","1","checked"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" value="0" name="oet1_pt" <% nvram_selmatch("oet1_pt","0","checked"); %> /><% tran("share.disable"); %>
		</div>
		<div class="setting">
			<div class="label"><% tran("service.eoip_frag"); %></div>
			<input size="4" maxlength="4" class="num" name="oet1_fragment" onblur="valid_range(this,0,1500,service.eoip_frag)" value="<% nvram_get("oet1_fragment"); %>" />
		</div>			
		<div class="setting">
			<div class="label"><% tran("service.eoip_mssfix"); %></div>
			<input class="spaceradio" type="radio" value="1" name="oet1_mssfix" <% nvram_selmatch("oet1_mssfix","1","checked"); %> /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" value="0" name="oet1_mssfix" <% nvram_selmatch("oet1_mssfix","0","checked"); %> /><% tran("share.disable"); %>
		</div>
		<div class="setting">
			<div class="label"><% tran("service.eoip_shaper"); %></div>
			<input size="6" maxlength="6" class="num" name="oet1_shaper" onblur="valid_range(this,0,100000,service.eoip_shaper)" value="<% nvram_get("oet1_shaper"); %>" />
		</div>	
		<div class="setting">
			<div class="label"><% tran("service.eoip_bridging"); %></div>
			<input class="spaceradio" type="radio" value="1" name="oet1_bridged" <% nvram_selmatch("oet1_bridged","1","checked"); %> onclick="show_layer_ext(this, 'idbridged', false)" /><% tran("share.enable"); %>&nbsp;
			<input class="spaceradio" type="radio" value="0" name="oet1_bridged" <% nvram_selmatch("oet1_bridged","0","checked"); %> onclick="show_layer_ext(this, 'idbridged', true)" /><% tran("share.disable"); %>
		</div>
		<div id="idbridged">
			<div class="setting">
				<div class="label"><% tran("share.ip"); %></div>
				<input type="hidden" name="oet1_ip" value="0.0.0.0"/>
				<input size="3" maxlength="3" class="num" name="oet1_ip_0" onblur="valid_range(this,0,255,share.ip)" value="<% get_single_ip("oet1_ip","0"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_ip_1" onblur="valid_range(this,0,255,share.ip)" value="<% get_single_ip("oet1_ip","1"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_ip_2" onblur="valid_range(this,0,255,share.ip)" value="<% get_single_ip("oet1_ip","2"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_ip_3" onblur="valid_range(this,1,254,share.ip)" value="<% get_single_ip("oet1_ip","3"); %>" />
			</div>
			<div class="setting">
				<div class="label"><% tran("share.subnet"); %></div>
				<input type="hidden" name="oet1_netmask" value="0.0.0.0"/>
				<input size="3" maxlength="3" class="num" name="oet1_netmask_0" onblur="valid_range(this,0,255,share.subnet)" value="<% get_single_ip("oet1_netmask","0"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_netmask_1" onblur="valid_range(this,0,255,share.subnet)" value="<% get_single_ip("oet1_netmask","1"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_netmask_2" onblur="valid_range(this,0,255,share.subnet)" value="<% get_single_ip("oet1_netmask","2"); %>" />.<input size="3" maxlength="3" class="num" name="oet1_netmask_3" onblur="valid_range(this,0,254,share.subnet)" value="<% get_single_ip("oet1_netmask","3"); %>" />
			</div>
		</div>
	</div>
</fieldset><br/>
							<div class="submitFooter">
							 <script type="text/javascript">
							 //<![CDATA[
							 document.write("<input type=\"button\" name=\"save_button\" value=\"" + sbutton.save + "\" onclick=\"to_submit(this.form)\" />");
							 //]]>
							 </script>
							 <script type="text/javascript">
							 //<![CDATA[
							 document.write("<input type=\"reset\" value=\"" + sbutton.cancel + "\" />");
							 //]]>
							 </script>
							 <script type="text/javascript">
							 //<![CDATA[
							 document.write("<input type=\"button\" name=\"refresh_button\" value=\"" + <% nvram_else_match("refresh_time","0","sbutton.refres","sbutton.autorefresh"); %> + "\" onclick=\"window.location.reload()\">");
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
						</dl><br />
						<a href="javascript:openHelpWindow<% nvram_selmatch("dist_type","micro","Ext"); %>('HEOP.asp');"><% tran("share.more"); %></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<% do_statusinfo(); %>
			</div>
		</div>
	</body>
</html>