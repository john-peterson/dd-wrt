<% do_pagehead("bmenu.servicesMilkfish"); %>
		<script type="text/javascript">
		//<![CDATA[

function to_submit(F) {
	F.save_button.value = sbutton.saving;
	apply(F);
}
		
		//]]>
		</script>
	</head>
	
	<body>
		<form name="setup" action="applyuser.cgi" method="<% get_http_method(); %>">
		<input type="hidden" name="submit_button" value="Milkfish" />
		<input type="hidden" name="action" value="Apply" />
		<input type="hidden" name="change_action" value="gozila_cgi" />
		<input type="hidden" name="submit_type" />
		<input type="hidden" name="commit" value="1" />
			<div id="main">
				<div id="contentsInfo">
					<h2>Milkfish Dynamic SIP - Advanced Settings</h2>
					Leave all settings empty when using the Milkfish HomeSIP Service<br />
					<br />
        				<div class="setting">
	                        	<div class="label">Dynamic SIP</div>
                                            <input class="spaceradio" type="radio" name="milkfish_dynsip" value="on" <% nvram_checked("milkfish_dynsip", "on"); %> onclick="show_layer_ext(this, 'idmilkfish_dynsip', true)" /><% tran("share.enable"); %>&nbsp;
                                            <input class="spaceradio" type="radio" name="milkfish_dynsip" value="off" <% nvram_checked("milkfish_dynsip", "off"); %> onclick="show_layer_ext(this, 'idmilkfish_dynsip', false)" /><% tran("share.disable"); %>
					</div>							   					
           				<div id="idmilkfish_dynsip">
		            		    <div class="setting">
				        	<div class="label">Dynamic SIP Domain</div>
						<input size="27" name="milkfish_dynsipdomain" value="<% nvram_get("milkfish_dynsipdomain"); %>" />
					    </div>
					    <div class="setting">
					        <div class="label">Dynamic SIP Update URL</div>
					        <input size="27" name="milkfish_dynsipurl" value="<% nvram_get("milkfish_dynsipurl"); %>" />
					    </div>
					    <div class="setting">
					        <div class="label">Dynamic SIP Username</div>
						<input size="27" name="milkfish_dsusername" value="<% nvram_get("milkfish_dsusername"); %>" />
					    </div>
					    <div class="setting">
						<div class="label">Dynamic SIP Password</div>
					        <input size="27" name="milkfish_dspassword" value="<% nvram_get("milkfish_dspassword"); %>" />
					    </div>
					</div>
					<br />
            <div class="setting">
                        <div class="label">Milkfish Audit</div>
                        <input class="spaceradio" type="radio" name="milkfish_audit" value="on" <% nvram_checked("milkfish_audit", "on"); %> /><% tran("share.enable"); %>&nbsp;
                        <input class="spaceradio" type="radio" name="milkfish_audit" value="off" <% nvram_checked("milkfish_audit", "off"); %> /><% tran("share.disable"); %>
                </div>				
				<br />
					<div class="submitFooter">
						<script type="text/javascript">
						//<![CDATA[
						submitFooterButton(1,1,0,0,0,1);
						//]]>
						</script>
					</div>
				</div>
			</div>
		</form>
	</body>
</html>
