<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
		<title><% nvram_get("router_name"); %> - Access Restrictions</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">
		
document.title = "<% nvram_get("router_name"); %>" + filter.titl;

<% filter_init(); %>
var summary_win = null;
var ipmac_win = null;

function dayall(F){
	if(F.day_all.checked == false) {
		I = 1;
	} else {
		I = 0;
	}
	day_enable_disable(F,I);
}

function day_enable_disable(F,I) {
	if(I == 1) {
		choose_enable(F.week0);
		choose_enable(F.week1);
		choose_enable(F.week2);
		choose_enable(F.week3);
		choose_enable(F.week4);
		choose_enable(F.week5);
		choose_enable(F.week6);
	} else if(I == 0) {
		choose_disable(F.week0);
		choose_disable(F.week1);
		choose_disable(F.week2);
		choose_disable(F.week3);
		choose_disable(F.week4);
		choose_disable(F.week5);
		choose_disable(F.week6);
	}
}

function timeall(F,I) {
	time_enable_disable(F,I);
}

function time_enable_disable(F,I){
	if(I == 1) {
		choose_enable(F.start_hour);
		choose_enable(F.start_min);
		choose_enable(F.start_time);
		choose_enable(F.end_hour);
		choose_enable(F.end_min);
		choose_enable(F.end_time);
	} else if(I == 0) {
		choose_disable(F.start_hour);
		choose_disable(F.start_min);
		choose_disable(F.start_time);
		choose_disable(F.end_hour);
		choose_disable(F.end_min);
		choose_disable(F.end_time);
	}
}


function to_submit(F) {
	if(valid(F) == true) {
		F.submit_type.value = "save";
		F.submit_button.value = "Filters";
		F.save_button.value = sbutton.saving;
		
		F.action.value = "Apply";
		apply(F);

	}
}

function to_save(F) {
	if(valid(F) == true) {
		F.submit_button.value = "Filters";
		F.change_action.value = "gozila_cgi";
		F.submit_type.value = "save";
		F.action.value = "Apply";
		F.submit();
	}
}

function to_delete(F) {
	if(confirm(filter.mess1)) {
		F.submit_button.value = "Filters";
		F.change_action.value = "gozila_cgi";
		F.submit_type.value = "delete";
		F.action.value = "Apply";
		F.submit();
	}
}

function valid(F) {
	if(
		F.day_all.checked == false &&
		F.week0.checked == false &&
		F.week1.checked == false &&
		F.week2.checked == false &&
		F.week3.checked == false &&
		F.week4.checked == false &&
		F.week5.checked == false &&
		F.week6.checked == false
	){
//		alert("You must at least select a day.");
		alert(filter.mess2);
		return false;
	}
	if(F.time_all[1].checked == true){
		start = (parseInt(F.start_time.value, 10)*12 + parseInt(F.start_hour.value, 10)) * 60 + parseInt(F.start_min.value, 10);
		end = (parseInt(F.end_time.value, 10)*12 + parseInt(F.end_hour.value, 10)) * 60 + parseInt(F.end_min.value, 10);
		if(end <= start){
//			alert("The end time must be bigger than start time.");
			alert(filter.mess3);
			return false;
		}
	}
	if(F.f_status1[1].checked == true) {		// Disable
		F.f_status.value = "0";
	} else {									// Enable
		if(F.f_status2[1].checked == true) {	// Allow
			F.f_status.value = "2";
		} else {								// deny
			F.f_status.value = "1";
		}
	}
	if (F._filter_p2p)
	if (F._filter_p2p.checked == false){
	    F.filter_p2p.value = 0;
	}else{
	    F.filter_p2p.value = 1;
	}

	return true;
}

function SelFilter(num,F) {
	F.submit_button.value = "Filters";
	F.change_action.value = "gozila_cgi";
	F.f_id.value=F.f_id.options[num].value;
	F.submit();
}

function init() {
	day_enable_disable(document.filters, "<% filter_tod_get("day_all_init"); %>");
	time_enable_disable(document.filters, "<% filter_tod_get("time_all_init"); %>");
	setBlockedServicesValue();
	Status(document.filters, "<% filter_policy_get("f_status","onload_status"); %>");
	choose_disable(document.filters.port0_start);
	choose_disable(document.filters.port0_end);
	choose_disable(document.filters.port1_start);
	choose_disable(document.filters.port1_end);
	choose_disable(document.filters.port2_start);
	choose_disable(document.filters.port2_end);
	choose_disable(document.filters.port3_start);
	choose_disable(document.filters.port3_end);
}

function service(id, name, port_start, port_end, protocol) {
	this.id = id;
	this.name = name;
	this.start = port_start;
	this.end = port_end;
	this.protocol = protocol;
	this.deleted = false;
	this.modified = false;
}

var sorton = function(x,y){
	if(x.name <  y.name) return -1;
	else if (x.name == y.name) return 0;
	else return 1;
}

services=new Array();
services_length=0;
/* Init. services data structure */
<% filter_port_services_get("all_list", "0"); %>
services.sort(sorton);

servport_name0 = "<% filter_port_services_get("service", "0"); %>";
servport_name1 = "<% filter_port_services_get("service", "1"); %>";
servport_name2 = "<% filter_port_services_get("service", "2"); %>";
servport_name3 = "<% filter_port_services_get("service", "3"); %>";
p2p_value = "<% filter_port_services_get("p2p", "0"); %>";
function search_service_index(name) {
	for(var i=0; i<services_length ; i++){
		if(name == services[i].name){
			return i;
		}
	}

	return -1;
}

function write_service_options(name) {
	var index = search_service_index(name);
	for(var i=0 ; i<services_length ; i++){
		document.write("<option value=\""+services[i].name+"\"");
		if(i==index) {
			document.write(" selected=\"selected\"");
		}
		document.write(">"+services[i].name+"</option>");
	}
}

function setBlockedServicesValue() {
	var index;

	if (p2p_value)
	if (p2p_value == "1")
	    document.filters._filter_p2p.checked = true;
	else
	    document.filters._filter_p2p.checked = false;

	/* for service port 0 */
	index = search_service_index(servport_name0);
	if(index!=-1){
		document.filters.port0_start.value = services[index].start;
		document.filters.port0_end.value = services[index].end;
		document.filters.blocked_service0.selectedIndex = index+1; /* first will be none */
	}

	/* for service port 1 */
	index = search_service_index(servport_name1);
	if(index!=-1){
		document.filters.port1_start.value = services[index].start;
		document.filters.port1_end.value = services[index].end;
		document.filters.blocked_service1.selectedIndex = index+1; /* first will be none */
	}

	/* for service port 2 */
	index = search_service_index(servport_name2);
	if(index!=-1){
		document.filters.port2_start.value = services[index].start;
		document.filters.port2_end.value = services[index].end;
		document.filters.blocked_service2.selectedIndex = index+1; /* first will be none */
	}

	/* for service port 3 */
	index = search_service_index(servport_name3);
	if(index!=-1){
		document.filters.port3_start.value = services[index].start;
		document.filters.port3_end.value = services[index].end;
		document.filters.blocked_service3.selectedIndex = index+1; /* first will be none */
	}
}

function onChange_blockedServices(index, start, end) {
	index--
	if(index == -1) {
		start.value = '';
		end.value = '';
	} else {
		start.value = services[index].start;
		end.value = services[index].end;
	}
}

function Status(F,I) {
	var start = '';
	var end = '';
	var total = F.elements.length;
	for(i=0 ; i < total ; i++){
		if(F.elements[i].name == "blocked_service0")
			start = i;
		if(F.elements[i].name == "url7")
			end = i;
	}
	if(start == '' || end == '')
		return true;

	if(I == "deny" ) {
		for(i = start; i<=end ;i++) {
			choose_disable(F.elements[i]);
		}
	} else {
		for(i = start; i<=end ;i++) {
			choose_enable(F.elements[i]);
		}
		choose_disable(document.filters.port0_start);
		choose_disable(document.filters.port0_end);
		choose_disable(document.filters.port1_start);
		choose_disable(document.filters.port1_end);
		choose_disable(document.filters.port2_start);
		choose_disable(document.filters.port2_end);
		choose_disable(document.filters.port3_start);
		choose_disable(document.filters.port3_end);
	}
}
		</script>
	</head>

	<body class="gui" onload="init()">
		<% showad(); %>
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo">
						<h1><% show_control(); %></h1>
					</div>
					<div id="menu">
						<div id="menuMain">
							<ul id="menuMainList">
								<li><a href="index.asp"><script type="text/javascript">Capture(bmenu.setup)</script></a></li>
								<li><a href="Wireless_Basic.asp"><script type="text/javascript">Capture(bmenu.wireless)</script></a></li>
								<% nvram_invmatch("sipgate","1","<!--"); %>
								<li><a href="Sipath.asp"><script type="text/javascript">Capture(bmenu.sipath)</script></a></li>
								<% nvram_invmatch("sipgate","1","-->"); %>
								<li><a href="Firewall.asp"><script type="text/javascript">Capture(bmenu.security)</script></a></li>
								<li class="current"><span><script type="text/javascript">Capture(bmenu.accrestriction)</script></span>
									<div id="menuSub">
										<ul id="menuSubList">
											<li><span><script type="text/javascript">Capture(bmenu.webaccess)</script></span></li>
										</ul>
									</div>
								</li>
								<li><a href="Forward.asp"><script type="text/javascript">Capture(bmenu.applications)</script></a></li>
								<li><a href="Management.asp"><script type="text/javascript">Capture(bmenu.admin)</script></a></li>
								<li><a href="Status_Router.asp"><script type="text/javascript">Capture(bmenu.statu)</script></a></li>
							</ul>
						</div>
					</div>
				</div>
				<div id="main">
					<div id="contents">
						<form name="filters" action="apply.cgi" method="<% get_http_method(); %>" >
							<input type="hidden" name="submit_button" />
							<input type="hidden" name="action" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="blocked_service" />
							<input type="hidden" name="filter_web" />
							<input type="hidden" name="filter_policy" />
							<input type="hidden" name="filter_p2p" />
							<input type="hidden" name="f_status" />
							<h2><script type="text/javascript">Capture(filter.h2)</script></h2>
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.legend)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(filter.pol)</script></div>
									<select name="f_id" onchange="SelFilter(this.form.f_id.selectedIndex,this.form)"><% filter_policy_select(); %></select>
									<script type="text/javascript">document.write("<input type=\"button\" value=\"" + sbutton.del + "\" onclick=\"to_delete(this.form)\" />")</script>
									<script type="text/javascript">document.write("<input type=\"button\" value=\"" + sbutton.summary + "\" onclick=\"openWindow('FilterSummary.asp', 700, 480)\" />")</script>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.statu)</script></div>
									<input class="spaceradio" type="radio" value="enable" name="f_status1" <% filter_policy_get("f_status","enable"); %>/><script type="text/javascript">Capture(share.enable)</script>&nbsp;
									<input class="spaceradio" type="radio" value="disable" name="f_status1" <% filter_policy_get("f_status","disable"); %>/><script type="text/javascript">Capture(share.disable)</script>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(filter.polname)</script></div>
									<input maxlength="30" size="22" name="f_name" value="<% filter_policy_get("f_name",""); %>"/>
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(filter.pcs)</script></div>
									<script type="text/javascript">document.write("<input type=\"button\" value=\"" + sbutton.filterIP + "\" onclick=\"openWindow('FilterIPMAC.asp', 590, 700)\" />")</script>
								</div>
								<div class="setting">
									<div class="label">
										<input class="spaceradio" type="radio" name="f_status2" value="deny" onclick="Status(this.form,'deny')" <% filter_policy_get("f_status","deny"); %> /><script type="text/javascript">Capture(share.deny)</script>
									</div>
									<script type="text/javascript">Capture(filter.polallow)</script>
								</div>
								<div class="setting">
									<div class="label">
										<input class="spaceradio" type="radio" name="f_status2" value="allow" onclick="Status(this.form,'allow')" <% filter_policy_get("f_status","allow"); %> /><script type="text/javascript">Capture(share.allow)</script>
									</div>
								</div>
								<br />
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.legend2)</script></legend>
								<div class="setting">
									<table>
										<tr>
											<td align="center"><script type="text/javascript">Capture(share.everyday)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.sun_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.mon_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.tue_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.wed_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.thu_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.fri_s)</script></td>
											<td align="center"><script type="text/javascript">Capture(share.sat_s)</script></td>
										</tr>
										<tr>
											<td align="center"><input type="checkbox" value="1" name="day_all" onClick="dayall(this.form)" <% filter_tod_get("day_all"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week0" <% filter_tod_get("week0"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week1" <% filter_tod_get("week1"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week2" <% filter_tod_get("week2"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week3" <% filter_tod_get("week3"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week4" <% filter_tod_get("week4"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week5" <% filter_tod_get("week5"); %> /></td>
											<td align="center"><input type="checkbox" value="1" name="week6" <% filter_tod_get("week6"); %> /></td>
										</tr>
									</table>
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.time)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(filter.h24)</script></div>
									<input class="spaceradio" type="radio" value="1" name="time_all" onclick="timeall(this.form,'0')" <% filter_tod_get("time_all_en"); %> />
								</div>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(share.from)</script></div>
									<input type="hidden" name="allday" />
									<input class="spaceradio" type="radio" value="0" name="time_all" onclick="timeall(this.form,'1')" <% filter_tod_get("time_all_dis"); %> />
									<select name="start_hour"><% filter_tod_get("start_hour_12"); %></select>:<select name="start_min"><% filter_tod_get("start_min_5"); %></select>
									<select name="start_time">
										<option value="0" <% filter_tod_get("start_time_am"); %>>AM</option>
										<option value="1" <% filter_tod_get("start_time_pm"); %>>PM</option>
									</select>&nbsp;<script type="text/javascript">Capture(share.to)</script>&nbsp;<select name="end_hour"><% filter_tod_get("end_hour_12"); %></select>:<select name="end_min"><% filter_tod_get("end_min_5"); %></select>
									<select name="end_time">
										<option value="0" <% filter_tod_get("end_time_am"); %>>AM</option>
										<option value="1" <% filter_tod_get("end_time_pm"); %>>PM</option>
									</select>
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.legend3)</script></legend>
								<div class="setting">
									<div class="label"><script type="text/javascript">Capture(filter.catchall)</script></div>
  									<input class="spaceradio" type="checkbox" name="_filter_p2p" value="1" <% nvram_checked("filter_p2p", "1"); %> />
								</div>
								<div class="setting">
									<select size="1" name="blocked_service0" onchange="onChange_blockedServices(blocked_service0.selectedIndex, port0_start, port0_end)">
										<option value="None" selected="selected">None</option>
										<script type="text/javascript">write_service_options(servport_name0);</script>
									</select>
									<input maxLength="5" size="5" name="port0_start" class="num" readonly="readonly" /> ~ <input maxLength="5" size="5" name="port0_end" class="num" readonly="readonly" />
								</div>
								<div class="setting">
									<select size="1" name="blocked_service1" onchange="onChange_blockedServices(blocked_service1.selectedIndex, port1_start, port1_end)">
										<option value="None" selected="selected">None</option>
										<script type="text/javascript">write_service_options(servport_name1);</script>
									</select>
									<input maxLength="5" size="5" name="port1_start" class="num" readonly="readonly" /> ~ <input maxLength="5" size="5" name="port1_end" class="num" readonly="readonly" />
								</div>
								<div class="setting">
									<select size="1" name="blocked_service2" onchange="onChange_blockedServices(blocked_service2.selectedIndex, port2_start, port2_end)">
										<option value="None" selected="selected">None</option>
										<script type="text/javascript">write_service_options(servport_name2);</script>
									</select>
										<input maxLength="5" size="5" name="port2_start" class="num" readonly="readonly" /> ~ <input maxLength="5" size="5" name="port2_end" class="num" readonly="readonly" />
								</div>
								<div class="setting">
									<select size="1" name="blocked_service3" onchange="onChange_blockedServices(blocked_service3.selectedIndex, port3_start, port3_end)">
										<option value="None" selected="selected">None</option>
										<script type="text/javascript">write_service_options(servport_name3);</script>
									</select>
									<input maxLength="5" size="5" name="port3_start" class="num" readonly="readonly" /> ~ <input maxLength="5" size="5" name="port3_end" class="num" readonly="readonly" />
								</div>
								<div class="setting">
									<script type="text/javascript">document.write("<input type=\"button\" value=\"" + sbutton.filterSer + "\" onclick=\"openWindow('Port_Services.asp', 630, 420)\" />")</script>
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.legend4)</script></legend>
								<div class="setting center">
									<input class="num" size="30" maxlength="79" name="host0" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","0"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="30" maxlength="79" name="host1" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","1"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="30" maxlength="79" name="host2" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","2"); %>" />
								</div>
								<div class="setting center">
									<input class="num" size="30" maxlength="79" name="host3" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","3"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="30" maxlength="79" name="host4" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","4"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="30" maxlength="79" name="host5" onblur="valid_name(this,'URL')" value="<% filter_web_get("host","5"); %>" />
								</div>
							</fieldset><br />
							
							<fieldset>
								<legend><script type="text/javascript">Capture(filter.legend5)</script></legend>
								<div class="setting center">
									<input class="num" size="18" maxlength="79" name="url0" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","0"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url1" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","1"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url2" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","2"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url3" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","3"); %>" />
								</div>
								<div class="setting center">
									<input class="num" size="18" maxlength="79" name="url4" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","4"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url5" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","5"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url6" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","6"); %>" />&nbsp;&nbsp;&nbsp;
									<input class="num" size="18" maxlength="79" name="url7" onblur="valid_name(this,'Keyword')" value="<% filter_web_get("url","7"); %>" />
								</div>
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
							<dt class="term"><script type="text/javascript">Capture(hfilter.right1)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right2)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right3)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right4)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right5)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right6)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right7)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right8)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right9)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right10)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right11)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right12)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right13)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right14)</script></dd>
							<dt class="term"><script type="text/javascript">Capture(hfilter.right15)</script></dt>
							<dd class="definition"><script type="text/javascript">Capture(hfilter.right16)</script></dd>
						</dl>
						<br />
						<a href="javascript:openHelpWindow('HFilters.asp');"><script type="text/javascript">Capture(share.more)</script></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
					<div class="info">Firmware: <script>document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");</script></div>
					<div class="info"><script type="text/javascript">Capture(share.time)</script>: <% get_uptime(); %></div>
					<div class="info">WAN <% nvram_match("wl_mode","wet","disabled <!--"); %><% nvram_match("wan_proto","disabled","disabled <!--"); %>IP: <% nvram_status_get("wan_ipaddr"); %><% nvram_match("wan_proto","disabled","-->"); %><% nvram_match("wl_mode","wet","-->"); %></div>
				</div>
			</div>
		</div>
	</body>
</html>
