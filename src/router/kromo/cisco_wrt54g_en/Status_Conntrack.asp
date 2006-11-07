<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %> - Active IP Connections Table</title>
		<script type="text/javascript">
		//<![CDATA[

document.title = "<% nvram_get("router_name"); %>" + status_conn.titl;

		//]]>
		</script>
	</head>
	<body>
		<div class="popup">
			<form>
				<div id="bulle" class="bulle"></div>
				<h2><% tran("status_conn.h2"); %></h2>
				<div class="setting">
					<div class="label"><% tran("status_router.net_conntrack"); %></div>
					<% dumpip_conntrack(); %>
				</div></br>
				<table class="table">
						<th><% tran("filterSum.polnum"); %></th>
						<th><% tran("share.proto"); %></th>
						<th><% tran("share.timeout"); %></th>
						<th><% tran("share.src"); %></th>
						<th><% tran("share.dst"); %></th>
						<th><% tran("share.srv"); %></th>
						<th><% tran("share.state"); %></th>
					</tr>
					<% ip_conntrack_table(); %>
				</table><br />
				<div class="submitFooter">
					<script type="text/javascript">
					//<![CDATA[
					document.write("<input type=\"button\" name=\"button\" value=\"" + sbutton.refres + "\" onclick=\"window.location.reload()\" />");
					//]]>
					</script>
					<script type="text/javascript">
					//<![CDATA[
					document.write("<input type=\"reset\" value=\"" + sbutton.clos + "\" onclick=\"self.close()\" />");
					//]]>
					</script>
				</div>
			</form>
		</div>
	</body>
</html>