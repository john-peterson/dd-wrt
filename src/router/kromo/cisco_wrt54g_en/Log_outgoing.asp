<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<% charset(); %>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<title><% nvram_get("router_name"); %> - Outgoing Log Table</title>
		<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->
		<script type="text/javascript">

document.title = "<% nvram_get("router_name"); %>" + log_out.titl;

		</script>
	</head>
	<body>
		<div class="popup">
			<form>
				<h2><% tran("log_out.h2"); %></h2>
				<table class="table">
					<tr>
						<th><% tran("log_out.th_lanip"); %></th>
						<th><% tran("log_out.th_wanip"); %></th>
						<th><% tran("share.proto"); %></th>
						<th><% tran("log_out.th_port"); %></th>
						<th><% tran("share.rule"); %></th>
					</tr>
					<% dumplog("outgoing"); %>
				</table><br />
				<div class="submitFooter">
					<script type="text/javascript">document.write("<input type=\"button\" name=\"button\" value=\"" + sbutton.refres + "\" onclick=\"window.location.reload()\" />")</script>
					<script type="text/javascript">document.write("<input type=\"reset\" value=\"" + sbutton.clos + "\" onclick=\"self.close()\" />")</script>
				</div>
			</form>
		</div>
	</body>
</html>