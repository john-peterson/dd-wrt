<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
	<head>
		<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=iso-8859-1">
		<link type="text/css" rel="stylesheet" href="help.css">
		<title>Help - Services</title>
	</head>
	<body>
		<div id="header">
			<div class="logo"> </div>
			<div class="navig"><a href="index.asp">Index</a> | <a href="javascript:self.close();">Close</a></div>
		</div>
		<div id="content">
			<h2>Services</h2>
			<dl>
				<dt>DHCP Server</dt>
				<dd>DHCPd assigns IP addresses to your local devices. While the main configuration is on the setup page you can program some nifty special functions here.
					<ul class="wide">
						<li>DHCPd &ndash; Disabling here will disable DHCPd on this router irrespective of the settings on the Setup screen.</li>
						<li>Used domain &ndash; You can select here which domain the DHCP clients should get as their local domain. This can be the WAN domain set on the Setup screen or the LAN domain which can be set here.</li>
						<li>LAN Domain &ndash; You can define here your local LAN domain which is used as local domain for DNSmasq and DHCP service if chosen above.</li>
						<li>Static allocations &ndash; If you want to assign certain hosts a specific address then you can define them here. This is also the way to add hosts with a fixed address to the router's local DNS service (DNSmasq).</li>
					</ul><br/>
					There are some extra options you can set by entering them in <em>Additional DHCPD Options</em>.
				</dd>
				<dt>XBOX Kaid</dt>
				<dd>Kai is a means of connecting platform games over the internet. Enable the service and then add XBox MAC addresses separeted with ";".<br/><br/>
					<div class="note">
						<h4>Note</h4>
						<div>Xbox must be connected directly via one of the Ethernet ports of the router.</div>
					</div>
				</dd>
				<dt>PPTP</dt>
				<dd>A VPN technology by Microsoft and remote access vendors. It is implemented in Windows XP. Configuring this allows you to access you LAN at home remotely.
					<ul class="wide">
						<li>Server IP &ndash; The IP address of your router</li>
						<li>Client IP &ndash; A range of IP addresses for remotely connected machines. This range should not overlap with the DHCP range (Example: 192.168.0.210-220).</li>
						<li>CHAP-Secrets &ndash; A list of usernames and passwords for the VPN login, one user per line (Example: joe * joespassword *). For more details look up the pppd man page.</li>
					</ul>
				</dd>
				<dt>PPTP Client</dt>
				<dd>A VPN Client that enable you to connect to VPN servers by Microsoft and remote access vendors. Configuring this allows the router to VPN into a remote network.
					<ul class="wide">
						<li>Server IP or DNS Name &ndash; The IP address or DNS Name of the VPN server that you would like to connect to (Example: www.MyServer.com). </li>
						<li>Remote Subnet &ndash; Remote Subnet of the network you are connecting to (Example: 192.168.2.0). </li>
						<li>Remote Subnet Mask &ndash; Remote Subnet Mask of the network you are connecting to (Example: 255.255.255.0). </li>
						<li>MPPE Encryption  &ndash; The type of security to use for the connection. If you are connecting to another DD-WRT router you need (Example: mppe required). But if you are connecting to a Windows VPN server you need (Example: mppe required,no40,no56,stateless) or (Example: mppe required,no40,no56,stateful) </li>
						<li>MTU &ndash; Default Maximum Transmission Unit (Default: 1450) </li>
						<li>MRU &ndash; Default Maximum Receiving Unit (Default: 1450) </li>
						<li>User Name &ndash; Enter the UserName that you will use to connect to the VPN server. If you are connecting to another Linux base PPTP server you just need to enter the UserName (Example: root). But if you are connecting to a Windows VPN server you need to enter the servername and username (Example: DOMAIN\\UserName). </li>
						<li>Password &ndash; Enter the password of the for the username </li>
					</ul>
				</dd>

				<dt>RFlow / MACupd</dt>
				<dd>RFlow Collector is a traffic monitoring and management tool that allows to watch a complete network of DD-WRT routers.<br/>
					<ul class="wide">
						<li>RFlow Default port is 2055</li>
						<li>MACupd Default port is 2056</li>
						<li>Interval = 10 seems messages will be sent to server each 10 seconds</li>
						<li>Interface : choose which interface to monitor</li>
					</ul><br/>
					<div class="note">
						<h4>Note</h4>
						<div>For each RFlow and MACupd server IP : enter the IP address of the listening server (win32 PC with RFlow Collector).</div>
					</div>
				 </dd>
				<dt>Secure Shell</dt>
				<dd>Enabling SSHd allows you to access the Linux OS of your router with an SSH client (Putty works well on Windows, for example).
					<ul class="wide">
						<li>Password login &ndash; allow login with the router password (username is <tt>root</tt>)</li>
						<li>SSHd Port &ndash; the port number for SSHd (default is 22)</li>
						<li>Authorized Keys &ndash; here you paste your public keys to enable key-based login (more secure than a simple password)</li>
					</ul>
				</dd>
				<dt>System Log</dt>
				<dd>Enable Syslogd to capture and forward all messages to another system. Enter the IP-address of the server for the syslog messages.</dd>
				<dt>Telnet</dt>
				<dd>Enable a telnet server to connect to the router with telnet. The username is <tt>root</tt> and the password is the router password.<br/><br/>
					<div class="note">
						<h4>Note</h4>
						<div>If you using the router in an untrusted environment (for example as a public hotspot), it is strongly recommended to use SSHd and deactivate telnet.</div>
					</div>
				</dd>
				<dd>Check all values and click <em>Save Settings</em> to save your settings. Click <em>Cancel Changes</em> to cancel your unsaved changes. Click <em>Reboot router</em> to reboot your router immediately.</dd>
			</dl>
		</div>
		<div class="also">
			<h4>See also</h4>
			<ul>
				<li><a href="HSetup.asp">Setup</a></li>
				<li><a href="HManagement.asp">Management</a></li>
			</ul>
		</div>
	</body>
</html>
