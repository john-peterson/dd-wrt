<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
	<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1"/>
	<link type="text/css" rel="stylesheet" href="style.css"/>
	<script type="text/JavaScript" src="common.js">{}</script>
	<script language="JavaScript">

var submit_button = '<% get_web_page_name(); %>';

function to_submit()
{
	if(submit_button == "")
		history.go(-1);
	else if(submit_button == 'WL_WEPTable.asp')
		self.close();
	else if(submit_button == 'Register_ok.asp'){
		document.location.href =  "HotSpot_Admin.asp";
	}
	else
		document.location.href =  submit_button;
}
		</script>
	</head>
	
	<body>
		<div class="message">
			<div>
				<form>Settings saved.<br/>
					<input type="button" name="action" value="Continue" onclick="to_submit()"/>
				</form>
			</div>
		</div>
   </body>
</html>