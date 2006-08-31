<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %> - Services</title>
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + service.titl;

function verify_unique_static_ip(F) {                                              
	static_leasenum = <% nvram_get("static_leasenum"); %>;
                                                                                
	//Check all static leases
	var static_leases=' ';                                                   
    for(i=0;i < static_leasenum;i++) {                                               
		var elem = F.elements["lease"+i+"_ip"];                                       
		if (static_leases.indexOf(" " + elem.value + " ") == -1) {
			static_leases += elem.value + " ";
		} else {
//			alert(elem.value + " is already defined as a static lease.");
			alert(elem.value + errmsg.err62);
			elem.focus();
			return false;
		}
    }
    return true;                                                            
} 

function lease_add_submit(F) {
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "Services";
	F.submit_type.value = "add_lease";
	checked(F);
	F.action.value = "Apply";
	F.submit();
}

function lease_remove_submit(F) {
	F.change_action.value = "gozila_cgi";
	F.submit_button.value = "Services";
	F.submit_type.value = "remove_lease";
	checked(F);
	F.action.value = "Apply";
	F.submit();
}

function to_reboot(F) {
	F.action.value = "Reboot";
	F.submit();
}

function to_submit(F) {
	if(!verify_unique_static_ip(F)) {
		return false;
	}
	
	F.submit_button.value = "Services";
	F.save_button.value = sbutton.saving;
	checked(F);
	F.action.value = "Apply";
	apply(F);
}

function checked(F) {
	if (F._openvpn_certtype) {
		if (F._openvpn_certtype.checked == true) {
			F.openvpn_certtype.value = 1;
		} else {
			F.openvpn_certtype.value = 0;
		}
	}
	
	if (F._dhcpd_usejffs) {
		if (F._dhcpd_usejffs.checked == true) {
			F.dhcpd_usejffs.value = 1;
		} else {
			F.dhcpd_usejffs.value = 0;
		}
	}

	if (F._dhcpd_usenvram) {
		if (F._dhcpd_usenvram.checked == true) {
			F.dhcpd_usenvram.value = 1;
		} else {
			F.dhcpd_usenvram.value = 0;
		}
	}
}

addEvent(window, "load", function() {

		show_layer_ext(document.setup.sshd_enable, 'idssh', <% nvram_else_match("sshd_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.dnsmasq_enable, 'iddnsmasq', <% nvram_else_match("dnsmasq_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.kaid_enable, 'idkaid', <% nvram_else_match("kaid_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.snmpd_enable, 'idsnmp', <% nvram_else_match("snmpd_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.rflow_enable, 'idrflow', <% nvram_else_match("rflow_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.macupd_enable, 'idMACupd', <% nvram_else_match("macupd_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.pptpd_enable, 'idpptp', <% nvram_else_match("pptpd_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.pptpd_client_enable, 'idpptpcli', <% nvram_else_match("pptpd_client_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.openvpn_enable, 'idvpn', <% nvram_else_match("openvpn_enable", "1", "1", "0"); %> == 1);
		show_layer_ext(document.setup.syslogd_enable, 'idsyslog', <% nvram_else_match("syslogd_enable", "1", "1", "0"); %> == 1);
});

		</script>
	</head>

	<body class="gui">
		<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
				<% do_menu("Management.asp","Services.asp"); %>
				</div>
				<div id="main">
					<div id="contents">
						<form name="setup" action="applyuser.cgi" method="<% get_http_method(); %>">
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="action" />
							<input type="hidden" name="reboot_button" />
							<input type="hidden" name="commit" value="1" />
							<input type="hidden" name="static_leases" value="13" />
							<input type="hidden" name="openvpn_certtype" />
							<input type="hidden" name="dhcpd_usejffs" />
							<input type="hidden" name="dhcpd_usenvram" />
							<h2><% tran("service.h2"); %></h2>
							<% show_modules(".webservices"); %>
							<div class="submitFooter">
								<script type="text/javascript">document.write("<input type=\"button\" name=\"save_button\" value=\"" + sbutton.save + "\" onclick=\"to_submit(this.form)\" />")</script>
								<script type="text/javascript">document.write("<input type=\"reset\" value=\"" + sbutton.cancel + "\" />")</script>
								<script type="text/javascript">document.write("<input type=\"button\" name=\"reboot_button\" value=\"" + sbutton.reboot + "\" onclick=\"to_reboot(this.form)\" />")</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<div><h2><% tran("share.help"); %></h2></div>
						<br/>
						<a href="javascript:openHelpWindow<% nvram_selmatch("dist_type","micro","Ext"); %>('HServices.asp');"><% tran("share.more"); %></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<% do_statusinfo(); %>
			</div>
		</div>
	</body>
</html>