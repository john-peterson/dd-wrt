<% do_pagehead("nas.titl"); %>
		<script type="text/javascript">
		//<![CDATA[

function addSambaShare() {
	
	var position = 0;
	var shareIndex = null;
	var template = null;
	
	var share = addTableEntry('samba_shares');
	position = share.id.lastIndexOf('_') + 1;
	shareIndex = share.id.substr( position, share.id.length - position );

	var users = $('samba_users').childElements()[0].childElements();
	for( var i = 0; i < users.length; i++ ) {
		if( users[i].id ) {
			// check for template user row
			position = users[i].id.lastIndexOf('_') + 1;
			if( users[i].id.substr( 0, position ) == 'samba_users_row_' ) {
				addSambaUserShare( users[i].id.substr( position, users[i].id.length - position ), template, share );
			} else if(users[i].id == 'n_smbuser_template') {
				var cols = users[i].childElements();
				for( var j = 0; j < cols.length; j++ ) {
					if( cols[j].id) {
						if( cols[j].id == 'n_smbuser_shareaccess' ) {
							template = cols[j].childElements()[0];
						}
					}
				}
			}
		}
	}
}

function removeSambaShare( button ) {
	
	tableId = 'samba_shares';
	
	// remove the share
	removeTableEntry( tableId, button );
	
	// remove users share entries and reorder
	var position = button.name.lastIndexOf('_') + 1;
	var index = button.name.substr( position, button.name.length - position);
	
	removeSambaUserShare(index);
}

function setSambaShareAccessOptions( shareElement ) {

	var position = shareElement.name.lastIndexOf('_') + 1;	
	
	// get the select
	var shareSelect = document.getElementById('smbshare_mp_' + shareElement.name.substr( position, shareElement.name.length - position ) );
	if(!shareSelect) return;
	
	var label = 'smbshare_public_perms_' + shareSelect.name.substr(12,shareSelect.name.length - 12);
	var properties = null;
	
	// remove options
	var len = $(label).options.length;
	for(i = 0; i < len; i++) {
		$(label).removeChild($(label).childElements()[0]);
	}
	
	// read properties in case something is selected
	if( shareSelect.selectedIndex > 0 ) {
		properties = eval('(' + shareSelect.options[shareSelect.selectedIndex].getAttribute('rel') + ')' );
	}
	
	// add options from default
	var options = $('smbshare_public_perms').childElements();
	for(i = 0; i < options.length; i++) {
		var option = options[i].cloneNode(true);
		if(properties) {
			// check if value exists
			for(j = 0; j < properties.perms.length; j++) {
				if( properties.perms[j] == option.value) {
					$(label).appendChild(option);
				}
			}
		} else {
			$(label).appendChild(option);	
		}
	}
		
	if(shareSelect.selectedIndex > 0) {
		$(label).disabled = false;
	} else {
		$(label).disabled = true;
	}
}

function verifySambaSettings() {
	
	var error = false;
	
	// get the share entries
	var table = $('samba_shares');
	var section = table.childElements()[0];
	var rows = section.childElements();
	
	// walk through the rows
	for(i = 0; i < rows.length; i++) {
		for(j = 0; j < rows[i].childElements().length; j++) {
			//alert(rows[i].childElements(j));
		}
	}
	return false;
}

function checkSambaShares() {
	
	var shares = $('samba_shares').childElements()[0].childElements();
	var mp = '';
	var label = '';
	var sublabel = 'samba_shares_row_';
	var index = 0;
	var error = false;
		
	for( i = 0; i < shares.length; i++ ) {
		mp = '';
		label = '';
		
		if( shares[i].id.substr( 0, sublabel.length ) == sublabel && shares[i].id.substr(shares[i].id.length - 9, 9) != '_template') {
			index = shares[i].id.substr( sublabel.length, shares[i].id.length - sublabel.length );
			for(j = 0; j < shares[i].childElements().length; j++) {
				element = shares[i].childElements()[j].childElements()[0];
				error = false;
				if( element.name ) {
					if( element.name.substr(0, element.name.length - index.length - 1 ) == 'smbshare_mp' ) {
						mp = element.value.replace(/^\s+|\s+$/g,"");
						if(mp.length == 0) error = true;
					} else if( element.name.substr(0, element.name.length - index.length - 1 ) == 'smbshare_label' ) {
						label = element.value.replace(/^\s+|\s+$/g,"");
						if(label.length == 0) error = true;
					}
				}
				
				// error handling
				if(error) {
					element.className = "value_error";
				} else if( element.className == 'value_error') {
					element.className = '';
				}
			}
			
			// error handling
			if( mp.length == 0 || label.length == 0) {
				alert( 'Please select a mountpoint and enter a share label to proceed.' );
				return false;
			}
		}
	}
	
	return true;
}

function addSambaUser() {
	
	var col = null;
	var userIndex = null;
	var template = null;
	
	// get and check the shares
	var shares = $('samba_shares').childElements()[0].childElements();
	if(!shares || !checkSambaShares()) return;
	
	// check if adding was successful
	if( row = addTableEntry('samba_users') ) {
		
		cols = row.children;
		
		for(var i = 0; i < cols.length; i++) {
			col = cols[i];
			if(col.id == 'n_smbuser_shareaccess') {
				for(var j = 0; j < col.children.length; j++ ) {
					if( j == 0 ) {
						template = col.children[j];
					}
					col.removeChild(col.children[j]);
				}
			}
			if( col.id == 'n_smbuser_user' ) {
				for(var j = 0; j < col.children.length; j++ ) {
					if( col.children[j].type ) {
						if( col.children[j].type == 'text' ) {
							var position = col.children[j].name.lastIndexOf('_');
							if( position) {
								position++;
								userIndex = col.children[j].name.substr( position, col.children[j].name.length - position );
							}
						}
					}
				}
			}
		}
		
		if( template && userIndex ) {
			
			var sublabel = 'samba_shares_row_';
			
			for( i = 0; i < shares.length; i++ ) {
				if( shares[i].id ) {
					if( shares[i].id.substr( 0, sublabel.length ) == sublabel && shares[i].id.substr(shares[i].id.length - 9, 9) != '_template') {
						addSambaUserShare( userIndex, template, shares[i] );
					}
				}
			}
		}
	}
}

function addSambaUserShare( userIndex, template, share ) {
	
	var mp = null;
	var label = null;
	var element = null;
	
	// extrace the reqired share information
	for( var i = 0; i < share.childElements().length; i++ ) {	
		element = share.childElements()[i].childElements()[0];
		if( element.name ) {
			if( element.name.substr(0, element.name.length - i.toString().length - 1 ) == 'smbshare_mp' ) {
				mp = element.value.replace(/^\s+|\s+$/g,"");
			} else if( element.name.substr(0, element.name.length - i.toString().length - 1 ) == 'smbshare_label' ) {
				label = element.value.replace(/^\s+|\s+$/g,"");
			}
		}
	}
	
	// get the share index
	var prefix = 'samba_shares_row_';
	var shareIndex = share.id.substr( prefix.length, share.id.length - prefix.length );
		
	for( i = 0; i < template.childElements().length; i++ ) {
		element = template.childElements()[i];
		if( element.tagName == 'SPAN' ) {
			template.childElements()[i].innerHTML = label;
		} else if( element.tagName == 'INPUT' ) {
			template.childElements()[i].name = 'smbshare_' + shareIndex + '_user_' + userIndex;
		}
	}
		
	// add the template to the user entry
	var users = $('samba_users').childElements()[0].childElements();
	for( i = 0; i < users.length; i++ ) {
		if( users[i].id ) {
			if( users[i].id == 'samba_users_row_' +  userIndex ) {
				users[i].childElements()[2].appendChild( template.cloneNode(true) );
			}
		}
	}
}


function removeSambaUserShare( shareIndex ) {
	
	var position = 0;
	var next = 0;
	var shares = null;
	var share = null;
	var index = 0;
	
	var users = $('samba_users').childElements()[0].childElements();
	for( i = 0; i < users.length; i++ ) {
		if( users[i].id ) {
			// check for template user row
			position = users[i].id.lastIndexOf('_') + 1;
			if( users[i].id.substr( 0, position ) == 'samba_users_row_' ) {
				shares = users[i].childElements()[2].childElements();
				if(shares.length) {
					for( j = 0; j < shares.length; j++ ) {
						checkbox = shares[j].childElements()[0];
						position = checkbox.name.indexOf('_') + 1;
						next = checkbox.name.indexOf('_', position);
						if( position && next > position ) {
							if( shareIndex == checkbox.name.substr(position, next - position ) ) {
								users[i].childElements()[2].removeChild(shares[j]);
							} else if( parseInt( shareIndex ) < parseInt( checkbox.name.substr(position, next - position ) ) ) {
								index = parseInt( checkbox.name.substr(position, next - position ) ) - 1;
								checkbox.name = checkbox.name.substr(0 , position) + index + checkbox.name.substr( next, checkbox.name.length - next );
							}
						}
					}
				}
			}
		}
	}
}


function updateSambaUserShare( element ) {
	
	var i = 0;
	var j = 0;
	var position = 0;
	var shares = null;
	var share_prefix = "smbshare_label_";
	
	// get the share index
	var index = element.name.substr( share_prefix.length, element.name.length - share_prefix.length );
	var user_prefix = 'smbshare_' + index + '_user';
	
	var users = $('samba_users').childElements()[0].childElements();
	for( i = 0; i < users.length; i++ ) {
		if( users[i].id ) {
			// check for template user row
			position = users[i].id.lastIndexOf('_') + 1;
			if( users[i].id.substr( 0, position ) == 'samba_users_row_' ) {
				shares = users[i].childElements()[2].childElements();
				if(shares.length) {
					for( j = 0; j < shares.length; j++ ) {
						if( shares[j].childElements()[0].name.substr(0, user_prefix.length) == user_prefix ) {
							shares[j].childElements()[1].innerHTML = element.value;
						}
					}
				}
			}
		}
	}
}

function checkSambaUsers() {
	
	var users = $('samba_users').childElements()[0].childElements();
	var username = null;
	var password = null;
	var sublabel = 'samba_users_row_';
	var index = 0;
	var error = false;
		
	for( i = 0; i < users.length; i++ ) {
		username = '';
		password = '';
		
		if( users[i].id.substr( 0, sublabel.length ) == sublabel && users[i].id.substr(users[i].id.length - 9, 9) != '_template') {
			index = users[i].id.substr( sublabel.length, users[i].id.length - sublabel.length );
			for(j = 0; j < users[i].childElements().length; j++) {
				element = users[i].childElements()[j].childElements()[0];
				error = false
				if( element.name ) {
					if( element.name.substr(0, element.name.length - index.length - 1 ) == 'smbuser_username' ) {
						username = element.value.replace(/^\s+|\s+$/g,"");
						if(username.length == 0) error = true;
					} else if( element.name.substr(0, element.name.length - index.length - 1 ) == 'smbuser_password' ) {
						password = element.value.replace(/^\s+|\s+$/g,"");
						if(password.length == 0) error = true;
					}
					
					// error handling
					if(error) {
						element.className = "value_error";
					} else if( element.className == 'value_error') {
						element.className = '';
					}
				}
			}
			
			// error handling
			if( username.length == 0 || password.length == 0) {
				alert( 'Please enter username and password for the samba users.' );
				return false;
			}
		}
	}
	
	return true;
}
function to_submit(F) {
	if($('samba_shares')) {
		if(!checkSambaShares() || !checkSambaUsers())
			return false;
	}	
	F.change_action.value="gozila_cgi";
	F.submit_type.value = "save";
	F.save_button.value = sbutton.saving;
	apply(F);
}
function to_apply(F) {
	if($('samba_shares')) {
		if(!checkSambaShares() || !checkSambaUsers())
			return false;	
	}
	F.change_action.value="gozila_cgi";
	F.submit_type.value = "save";
	F.save_button.value = sbutton.saving;
	applytake(F);
}

var update;

addEvent(window, "load", function() {
	show_layer_ext(document.setup.proftpd_enable, 'ftpen', <% nvram_else_match("proftpd_enable", "1", "1", "0"); %> == 1);
	show_layer_ext(document.setup.proftpd_anon, 'ftpanon', <% nvram_else_match("proftpd_anon", "1", "1", "0"); %> == 1);
	show_layer_ext(document.setup.samba3_enable, 'samba3config', <% nvram_else_match("samba3_enable", "1", "1", "0"); %> == 1);
	//show_layer_ext(document.setup.samba3_pub, 'samba3pub', <% nvram_else_match("samba3_pub", "1", "1", "0"); %> == 1);
	show_layer_ext(document.setup.samba3_advanced, 'samba3advanced', <% nvram_else_match("samba3_advanced", "1", "1", "0"); %> == 1);
	show_layer_ext(document.setup.samba3_advanced, 'samba3basic', <% nvram_else_match("samba3_advanced", "1", "1", "0"); %> != 1);

	update = new StatusbarUpdate();
	update.start();
	
});

addEvent(window, "unload", function() {
	update.stop();

});
		
		//]]>
		</script>

		<style type="text/css">
			.value_error {
				 border: solid 2px #f00;
			}
		</style>
	
	</head>

	<body class="gui">
		<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<% do_menu("Services.asp", "NAS.asp"); %>
				</div>
				<div id="main">
					<div id="contents">
						<form name="setup" action="apply.cgi" method="post">
							<input type="hidden" name="submit_button" value="NAS" />
							<input type="hidden" name="action" value="Apply" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="commit" value="1"/>
							
						<% show_modules(".webnas"); %>

							<div class="submitFooter">
								<script type="text/javascript">
								//<![CDATA[
								submitFooterButton(1,1);
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
							<dt class="term"><% tran("nas.proftpd_srv"); %></dt>
							<dd class="definition"><% tran("hnas.right2"); %></dd>
						</dl>
						<br />
						<a href="javascript:openHelpWindow<% ifdef("EXTHELP","Ext"); %>('HNAS.asp')"><% tran("share.more"); %></a>
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
