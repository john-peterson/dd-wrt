<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html
  PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
   <head>
      <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
   
      <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
      <title><% nvram_get("router_name"); %> - Wireless Active Client MAC List</title>
      <link type="text/css" rel="stylesheet" href="style.css" /><script type="text/JavaScript" src="common.js">{}</script><script language="JavaScript">
function MACAct(F)
{
	if(valid_value(F)){
		F.submit_button.value="WL_ActiveTable";
		F.change_action.value="gozila_cgi";
		F.submit_type.value="add_mac";
		F.submit();
	}
}
function valid_value(F)
{
	var num = F.elements.length;
	var count = 0;

	for(i=0;i<num;i++){
		if(F.elements[i].type == "checkbox"){
			if(F.elements[i].checked == true)
				count = count + 1;
		}
	}
	if(count > 128){
		alert("The total checks exceed 128 counts!");
		return false;
	}
	return true;
}
function init() {
	<% onload("WL_ActiveTable", "setTimeout('opener.window.location.reload();',500);"); %>
	window.focus();
}
</script></head>
   <body onload="init()">
      <form action="apply.cgi" method="<% get_http_method(); %>"><input type="hidden" name="submit_button" /><input type="hidden" name="submit_type" /><input type="hidden" name="change_action" /><table>
            <tbody>
               <tr>
                  <td colSpan="2">Wireless Client MAC List</td>
                  <td>&nbsp;</td>
                  <td><input onclick="window.location.reload()" type="button" value=" Refresh " /></td>
               </tr>
               <tr>
                  <td>Active PC</td>
                  <td colspan="3">&nbsp;</td>
               </tr>
               <tr>
                  <th>Client Host Name</th>
                  <th>IP Address</th>
                  <th>MAC Address</th>
                  <th>Enable MAC Filter</th>
               </tr><% wireless_active_table("online"); %>
               <tr>
                  <td>Inactive PC</td>
                  <td colspan="3">&nbsp;</td>
               </tr>
               <tr>
                  <th>Client Host Name</th>
                  <th>IP Address</th>
                  <th>MAC Address</th>
                  <th>Enable MAC Filter</th>
               </tr><% wireless_active_table("offline"); %>
               <tr>
                  <td colspan="4">&nbsp;</td>
               </tr>
               <tr>
                  <td colspan="2">&nbsp;</td>
                  <td colspan="2"><input type="button" name="action" value="Update Filter List" onClick="MACAct(this.form)" />&nbsp;<input onclick="self.close()" type="reset" value="Close" /></td>
               </tr>
            </tbody>
         </table>
      </form>
   </body>
</html>