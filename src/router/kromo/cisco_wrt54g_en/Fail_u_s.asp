<% do_pagehead(); %>
		<title><% nvram_get("router_name"); %></title>
		
<!-- Botho 22/04 : css_include() and js_include() correct in a temporary way the loss of style and javascript includes -->

		<style type="text/css">
<% css_include(); %>
		</style>
		<script type="text/javascript">
<% js_include(); %>
		
function to_submit()
{
	document.location.href = "Upgrade.asp";
}

		</script>
	</head>

   <body>
      <div class="message">
         <div>
            <form>
            	<% tran("fail.mess2"); %><br />
				<script type="text/javascript">document.write("<input type=\"button\" name=\"action\" value=\"" + sbutton.continu + "\" onclick=\"to_submit()\" />")</script>
			</form>
         </div>
      </div>
   </body>
</html>