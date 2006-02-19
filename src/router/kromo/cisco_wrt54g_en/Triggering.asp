<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html
  PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1" />
    <title><% nvram_get("router_name"); %> - Port Triggering</title>
    <link type="text/css" rel="stylesheet" href="style.css" />
    <script type="text/JavaScript" src="common.js">{}</script>
    <script language="JavaScript">

function trigger_add_submit(F) {
  F.change_action.value = "gozila_cgi";
  F.submit_button.value = "Triggering";
  F.submit_type.value = "add_trigger";
  F.action.value = "Apply";
  F.submit();
}

function trigger_remove_submit(F) {
  F.change_action.value = "gozila_cgi";
  F.submit_button.value = "Triggering";
  F.submit_type.value = "remove_trigger";
  F.action.value = "Apply";
  F.submit();
}

function to_submit(F)
{
  F.submit_button.value = "Triggering";
  F.action.value = "Apply";
  F.save_button.value = "Saved";
  F.save_button.disabled = true;
        F.submit();
}
    </script>
  </head>
  
  <body class="gui"> <% showad(); %>
    <div id="wrapper">
      <div id="content">
        <div id="header">
          <div id="logo"><h1><% show_control(); %></h1></div>
          <div id="menu">
            <div id="menuMain">
              <ul id="menuMainList">
                <li><a href="index.asp">Setup</a></li>
                <li><a href="Wireless_Basic.asp">Wireless</a></li>
                <% nvram_invmatch("sipgate","1","<!--"); %>
                <li><a href="Sipath.asp">SIPatH</a></li>
                <% nvram_invmatch("sipgate","1","-->"); %>
                <li><a href="Firewall.asp">Security</a></li>
                <li><a href="<% support_elsematch("PARENTAL_CONTROL_SUPPORT", "1", "Parental_Control.asp", "Filters.asp"); %>">Access Restrictions</a></li>
                <li class="current"><a href="Forward.asp">Applications&nbsp;&amp;&nbsp;Gaming</a>
                  <div id="menuSub">
                    <ul id="menuSubList">
                      <li><a href="Forward.asp">Port Range Forward</a></li>
                      <li><a href="ForwardSpec.asp">Port Forwarding</a></li>
                      <li><span>Port Triggering</span></li>
                      <li><a href="UPnP.asp">UPnP Forward</a></li>
                      <li><a href="DMZ.asp">DMZ</a></li>
                      <li><a href="QoS.asp">QoS</a></li>
                    </ul>
                  </div>
                </li>
                <li><a href="Management.asp">Administration</a></li>
                <li><a href="Status_Router.asp">Status</a></li>
              </ul>
            </div>
          </div>
        </div>
        <div id="main">
          <div id="contents">
            <form name="trigger" action="apply.cgi" method="<% get_http_method(); %>">
              <input type="hidden" name="action"/>
              <input type="hidden" name="change_action"/>
              <input type="hidden" name="submit_button"/>
              <input type="hidden" name="submit_type"/>
              <input type="hidden" name="port_trigger" value="10"/>
              <h2>Port Triggering</h2>
              <table class="table center">
                <tr>
                  <th>&nbsp;</th>
                  <th colspan="2">Triggered Range</th>
                  <th colspan="2">Forwarded Range</th>
                  <th>&nbsp;</th>
                </tr>
                <tr>
                  <th>Application</th>
                  <th>Start Port</th>
                  <th>End Port</th>
                  <th>Start Port</th>
                  <th>End Port</th>
                  <th>Enable</th>
                </tr>
                <% show_triggering(); %>
                <tr>
                  <td>&nbsp;</td>
                  <td colspan="2">
                    <input type="button" value="Add" onclick="trigger_add_submit(this.form)"/>
                    <input type="button" value="Remove" onclick="trigger_remove_submit(this.form)"/>
                  </td>
                </tr>
              </table>
              <div class="submitFooter">
                <input type="button" name="save_button" value="Save Settings" onclick="to_submit(this.form)"/>
                <input type="reset" value="Cancel Changes"/>
              </div>
            </form>
          </div>
        </div>
        <div id="statusInfo">
          <div class="info">Firmware: <% get_firmware_version(); %></div>
          <div class="info">Time: <% get_uptime(); %></div>
          <div class="info">WAN IP: <% nvram_status_get("wan_ipaddr"); %></div>
        </div>
        <div id="helpContainer">
          <div id="help">
            <div id="logo"><h2>Help</h2></div>
            <dl>
              <dt class="term">Application: </dt>
              <dd class="definition">Enter the application name of the trigger.</dd>
              <dt class="term">Triggered Range: </dt>
              <dd class="definition">For each application, list the triggered port number range. Check with the Internet application documentation for the port number(s) needed.</dd>
              <dt class="term">Start Port: </dt>
              <dd class="definition">Enter the starting port number of the Triggered Range.</dd>
              <dt class="term">End Port: </dt>
              <dd class="definition">Enter the ending port number of the Triggered Range.</dd>
              <dt class="term">Forwarded Range: </dt>
              <dd class="definition">For each application, list the forwarded port number range. Check with the Internet application documentation for the port number(s) needed.</dd>
              <dt class="term">Start Port: </dt>
              <dd class="definition">Enter the starting port number of the Forwarded Range.</dd>
              <dt class="term">End Port: </dt>
              <dd class="definition">Enter the ending port number of the Forwarded Range.</dd>
            </dl>
            <br />
            <a target="_blank" href="help/HTrigger.asp">More...</a>
          </div>
        </div>
      </div>
    </div>
  </body>
</html>