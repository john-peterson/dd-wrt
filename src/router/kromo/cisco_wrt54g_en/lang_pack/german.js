
// ************************************************************ COMMON SHARE LABEL ***************************************************************//
var lang_charset = new Object();
lang_charset.set="iso-8859-1";

var share = new Object();
share.firmware="Firmware";
share.time="Zeit";
share.interipaddr="Internet IP-Addresse";
share.more="Mehr...";
share.help="Hilfe";
share.enable="Einschalten";
share.enabled="Eingeschaltet";
share.disable="Abschalten";
share.disabled="Ausgeschaltet";
share.usrname="Nutzername";
share.passwd="Passwort";
share.hostname="Hostname";
share.domainname="Domainname";
share.statu="Status";
share.start="Start";
share.end="Ende";
share.proto="Protokol";
share.ip="IP-Adresse";
share.mac="MAC-Adresse";
share.none="Nichts";
share.both="Beides";
share.del="L�schen";
share.remove="Entfernen";
share.descr="Beschreibung";
share.from="Von";
share.to="Nach";
share.about="�ber";
share.everyday="Jeden Tag";
share.sun="Sonntag";
share.sun_s="Son";
share.sun_s1="S";
share.mon="Montag";
share.mon_s="Mon";
share.mon_s1="M";
share.tue="Dienstag";
share.tue_s="Die";
share.tue_s1="D";
share.wed="Mittwoch";
share.wed_s="Mit";
share.wed_s1="W";
share.thu="Donnerstag";
share.thu_s="Don";
share.thu_s1="D";
share.fri="Freitag";
share.fri_s="Fre";
share.fri_s1="F";
share.sat="Samstag";
share.sat_s="Sam";
share.sat_s1="S";
share.expires="L�uft ab";
share.yes="Ja";
share.no="Nein";
share.allow="Erlauben";
share.deny="Verbieten";
share.range="Bereich";
share.use="Nutze";
share.mins="Min.";
share.secs="Sek.";
share.routername="Router Name";
share.manual="Manuell";
share.port="Port";
share.ssid="SSID";
share.channel="Kanal";
share.rssi="Signal";
share.signal="Signal";
share.noise="Rauschen";
share.beacon="beacon";
share.openn="Offen";
share.dtim="dtim";
share.rates="Rate";
share.low="Niedrig";
share.medium="Mittel";
share.high="Hoch";
share.option="Optionen";
share.rule="Regel";
share.lan="LAN";
share.point2point="Punkt zu Punkt";
share.nat="NAT";
share.subnet="Subnetz Maske";
share.unmask="Unmask";
share.deflt="Std";  //don't use share.default !!!
share.all="Alles";
share.auto="Auto";
share.right="Rechts";
share.left="Links";
share.share_key="Shared Key";
share.inter="Interval (in Sekunden)";
share.srv="Service Name";
share.port_range="Port Bereich";
share.priority="Priorit�t";
share.gateway="Gateway";
share.intrface="Interface";  //don't use share.interface, Mozilla problem!!!
share.router="Router";
share.static_lease="Statische Zuweisungen";
share.srvip="Server IP";
share.localdns="Lokaler DNS";
share.minutes="Minuten";
share.oui="OUI Search";
share.sttic="Statisch";
share.connecting="Verbinden";
share.connect="Verbinden";
share.connected="Verbunden";
share.disconnect="Trennen";
share.disconnected="Getrennt";
share.info="Information";
share.state="Status";
share.mode="Modus";
share.encrypt="Verschl�sselung";
share.key="Schl�ssel";
share.wireless="WLAN";
share.dhcp="DHCP";
share.styl="Aussehen";



var sbutton = new Object();
sbutton.save="Einstellungen Speichern";
sbutton.saving="Gespeichert";
sbutton.cmd="Ausf�hren";
sbutton.cancel="Einstellungen zur�cknehmen";
sbutton.refres="Auffrischen";
sbutton.clos="Schlie�en";
sbutton.del="L�schen";
sbutton.continu="Fortfahren";
sbutton.add="Hinzuf�gen";
sbutton.remove="Entfernen";
sbutton.modify="Ver�ndern";
sbutton.deleted="Gel�scht";
sbutton.delall="L�sche Alles";
sbutton.autorefresh="Auto-Refresh ist An";
sbutton.backup="Sichern";
sbutton.restore="Wiederherstellen";
sbutton.cptotext="In den Text bereich kopieren";
sbutton.runcmd="Kommandos Ausf�hren";
sbutton.startup="Startup Speichern";
sbutton.firewall="Firewall Speichern";
sbutton.wol="Aufwecken";
sbutton.add_wol="Host hinzuf�gen";
sbutton.manual_wol="Manuelles Aufwecken";
sbutton.summary="Zusammenfassung";
sbutton.filterIP="Liste der PCs editieren";
sbutton.filterMac="MAC Filter Liste editieren";
sbutton.filterSer="Hinzuf�gen/Editieren Service";
sbutton.reboot="Router Neustarten";
sbutton.help="   Hilfe  ";
sbutton.wl_client_mac="WLAN Client MAC Liste";
sbutton.update_filter="Filter Liste updaten";
sbutton.join="Beitreten";
sbutton.log_in="Eingehendes Log";
sbutton.log_out="Ausgehendes Log";
sbutton.apply="Anwenden";
sbutton.edit_srv="Hinzuf�gen/Editieren Service";
sbutton.routingtab="Routen Tabelle Anzeigen";
sbutton.wanmac="Zeige aktuelle PC MAC-Adresse";
sbutton.dhcprel="DHCP Freigeben";
sbutton.dhcpren="DHCP Erneuern";
sbutton.survey="WLAN Suche";
sbutton.upgrading="Upgrading";
sbutton.upgrade="Upgrade";
sbutton.preview="Vorschau";




// ************************************************************ COMMON ERROR MESSAGES  ***************************************************************//
var errmsg = new Object();
errmsg.err0="Sie m�ssen einen Nutzernamen angeben.";
errmsg.err1="Sie m�ssen einen Routernamen angeben.";
errmsg.err2="Auserhalb des zul�ssigen Bereichs, bitte ver�ndern Sie die Start IP-Adresse oder die anzahl der Nutzer.";
errmsg.err3="Sie m�ssen einen Tag ausw�hlen."
errmsg.err4="Die End Zeit mu� gr��er als die Startzeit sein.";
errmsg.err5="Die MAC-Adressen l�nge ist nicht korrekt.";
errmsg.err6="Sie m�ssen einen Nutzernamen angeben.";
errmsg.err7="Sie m�ssen einen Hostnamen angeben.";
errmsg.err8="Sie m�ssen eine IP-Adresse oder einen Domainnamen angeben.";
errmsg.err9="Illegale DMZ IP-Adresse.";
errmsg.err10="Das Passwort zur Best�tigung entspricht nicht dem originalen. Bitte geben Sie es neu ein.";
errmsg.err11="In einem Passwort sind keine Leerzeichen erlaubt";
errmsg.err12="Sie m�ssen ein Kommando zum Ausf�hren angeben.";
errmsg.err13="Das Upgrade ist fehlgeschlagen.";
errmsg.err45="Diese Funktion ist nicht unter HTTPS verf�gbar! Bitte verwenden Sie den HTTP modus.";
errmsg.err46="Diese Funktion ist nicht unter HTTPS verf�gbar";


//common.js error messages
errmsg.err14=" wert ist au�erhalb des zul�ssigen Bereichs [";
errmsg.err15="Die WAN MAC-Adresse ist au�erhalb des g�ltigen Bereichs [00 - ff].";
errmsg.err16="Der zweite Wert der MAC-Adresse mu� eine gerade Zahl sein : [0, 2, 4, 6, 8, A, C, E].";
errmsg.err17="Diese MAC-Adresse ist nicht korrekt.";
errmsg.err18="Die l�nger der MAC-Adresse ist nicht korrekt.";
errmsg.err19="Die MAC Adresse kann nicht die Broadcast Adresse sein."
errmsg.err20="Geben Sie die MAC Adresse im Format (xx:xx:xx:xx:xx:xx) ein.";
errmsg.err21="Ung�ltiges MAC-Adressen Format.";
errmsg.err22="Die WAN MAC-Adresse ist nicht korrekt.";
errmsg.err23="Ung�ltiger HEX Wert ";
errmsg.err24=" in MAC Adresse gefunden ";
errmsg.err25="Der Schl�ssel Wert ist nicht korrekt.";
errmsg.err26="Die Schl�ssel l�nge ist nicht korrekt.";
errmsg.err27="Ung�ltige Subnetz Maske.";
errmsg.err28=" enth�lt ung�ltige Werte, diese m�ssen [ 0 - 9 ] sein.";
errmsg.err29=" enth�lt einen ung�lten Ascii Wert.";
errmsg.err30=" enth�lt illegale hexadezimale Werte.";
errmsg.err31=" ist ung�ltig.";
errmsg.err32="IP-Adresse und Gateway sind nicht im selben Netzbereich.";
errmsg.err33="IP-Adresse und Gateway k�nnen nicht die selben sein.";
errmsg.err34=" dark keine leerzeichen enthalten.";

//Wol.asp error messages
errmsg.err35="Sie m�ssen eine MAC-Adresse zum fortfahren eingeben.";
errmsg.err36="Sie m�ssen eine Netzwerk Broadcast Adresse angeben um fortzufahren.";
errmsg.err37="Sie m�ssen einen UDP Port angeben um fortzufahren.";

//WL_WPATable.asp error messages
//WPA.asp error messages
errmsg.err38="Bitte geben sie einen Shared Key an!";
errmsg.err39="Ung�ltiger Schl�ssel, er mu� aus 8 bis 63 ASCII Werten oder 64 hexadezimalen Zahlen bestehen";
errmsg.err40="Sie m�ssen einen Schl�ssel f�r Key angeben ";
errmsg.err41="Ung�ltige Schl�ssen l�nge ";
errmsg.err43="Rekey Interval";

//config.asp error messages
errmsg.err42="Bitte geben sie eine Konfiguration zum wiederherstellen an.";

//WL_ActiveTable.asp error messages
errmsg.err44="Die totale Anzahl der Checks �berschreitet 128.";

//Site_Survey.asp error messages
errmsg.err47="Ung�ltige SSID.";

//Wireless_WDS.asp error messages
errmsg.err48="WDS ist nicht kompatibel mit der aktuellen Konfiguration des Routers. Bitte pr�fen sie die folgenden Punkte :\n * WLAN Modus mu� AP sein \n * WPA2 wird vom WDS nicht unterst�tzt \n * WLAN Netzwerk Modus Nur-B wird von WDS nicht unterst�tzt";

//Wireless_radauth.asp error messages
errmsg.err49="Radius ist nur im AP Modus verf�gbar.";

//Wireless_Basic.asp error messages
errmsg.err50="Sie m�ssen eine SSID angeben.";

// Management.asp error messages
errmsg.err51="Der Router ist im Augenblick auf das Standard Kennwort konfiguriert. \
			Aus Sicherheitsgr�nden m�ssen sie das Kennwort �ndern befor sie das Remote Management feature aktivieren. \
			Klicken Sie den OK Button um das Kennwort zu �ndern. Klicken Sie den Abbrechen Button um die Remote Management Funktion ausgeschaltet zu lassen.";
errmsg.err52="Passwort �berpr�fung stimmt nicht �berein.";

// Port_Services.asp error messages
errmsg.err53="Klicken Sie nach dem abschluss aller Aktionen den Anwenden Button um die Einstellungen zu �bernehmen.";
errmsg.err54="Sie m�ssen einen Service Namen angeben.";
errmsg.err55="Der Service Name existiert bereits.";

// QoS.asp error messages
errmsg.err56="Port Wert ist au�erhalb des g�ltigen Bereichs [0 - 65535]";

// Routing.asp error messages
errmsg.err57="Eintrag l�schen ?";

// Status_Lan.asp error messages
errmsg.err58="Klicken um die Zuweisung zu l�schen";

//Status_Wireless.asp error messages
errmsg.err59="Nicht verf�gbar! Bitte schalten sie das WLAN Netzwerk an.";

//Upgrade.asp error messages
errmsg.err60="Bitte geben sie eine Datei zum upgraden an.";
errmsg.err61="Ung�ltiges Dateiformat.";


// **************************************************************  COMMON MENU ENTRIES  **********************************************************//
var bmenu= new Object();
bmenu.setup="Setup";
bmenu.setupbasic="Basis Setup";
bmenu.setupddns="DDNS";
bmenu.setupmacclone="MAC-Adresse Klonen";
bmenu.setuprouting="Erweitertes Routing";
bmenu.setupvlan="VLANs";

bmenu.wireless="WLAN";
bmenu.wirelessBasic="Basis Einstellungen";
bmenu.wirelessRadius="Radius";
bmenu.wirelessSecurity="WLAN Sicherheit";
bmenu.wirelessMac="MAC Filter";
bmenu.wirelessAdvanced="Erweiterte Einstellungen";
bmenu.wirelessWds="WDS";

bmenu.security="Sicherheit";
bmenu.firwall="Firewall";
bmenu.vpn="VPN";

bmenu.accrestriction="Zugriffs Beschr�nkung";
bmenu.webaccess="Internet Zugriff";


bmenu.applications="Applikationen &amp; Spiele";
bmenu.applicationsprforwarding="Port Bereichs Weiterleitung";
bmenu.applicationspforwarding="Port Weiterleitung";
bmenu.applicationsptriggering="Port Triggering";
bmenu.applicationsUpnp="UPnP";
bmenu.applicationsDMZ="DMZ";
bmenu.applicationsQoS="QoS";

bmenu.sipath="SIPatH";
bmenu.sipathoverview="�bersicht";
bmenu.sipathphone="Telefonbuch";
bmenu.sipathstatus="Status";



bmenu.admin="Administration";
bmenu.adminManagement="Management";
bmenu.adminHotspot="Hotspot";
bmenu.adminServices="Services";
bmenu.adminAlive="Lebenserhaltung";
bmenu.adminLog="Log";
bmenu.adminDiag="Diagnose";
bmenu.adminWol="WOL";
bmenu.adminFactory="Fabrik Einstellungen";
bmenu.adminUpgrade="Firmware Update";
bmenu.adminBackup="Backup";


bmenu.statu="Status";
bmenu.statuRouter="Router";
bmenu.statuLAN="LAN";
bmenu.statuSputnik="Sputnik Agent";
bmenu.statuWLAN="WLAN";
bmenu.statuSysInfo="Sys-Info";

// **************************************************************** Alive.asp **********************************************************************//

var alive = new Object();
alive.titl=" - Lebenserhaltung";
alive.h2="Lebenserhaltung";
alive.legend="Geplanter Neustart";
alive.sevr1="Reboot Planen";
alive.hour="Zu einem festen Zeitpunkt";
alive.legend2="WDS/Verbindungs�berwachung";
alive.sevr2="�berwachung Einschalten";
alive.IP="IP-Adressen";
alive.legend3="Proxy/Verbindungs�berwachung";
alive.sevr3="Proxy �berwachung Einschalten";
alive.IP2="Proxy IP-Adresse";
alive.port="Proxy Port";

//help container
var halive = new Object();
halive.right2="W�hlen Sie aus wann der Router neu starten soll. Cron mu� hierf�r auf dem Management Tab, eingeschaltet sein.";
halive.right4="Maximal 3 IP's (mit einem <em>Lehrzeichen</em> getrennt) sind erlaubt.<BR/>IP Format ist xxx.xxx.xxx.xxx.";

// **************************************************************** config.asp **********************************************************************//

var config = new Object();
config.titl=" - Backup & Wiederherstellung";
config.h2="Backup Konfiguration";
config.legend="Backup Einstellungen";
config.mess1="Klickem Sie den \"" + sbutton.backup + "\" Button um die Backup Datei zu downloaden.";
config.h22="Konfiguration Wiederherstellen";
config.legend2="Einstellungen Wiederherstellen";
config.mess2="Datei zur Wiederherstellung";
config.mess3="W A R N U N G";
config.mess4="Uploaden Sie nur Dateien die auch mit dieser Firmware auf dem selben Routermodell erstellt wurden.!";

//help container
var hconfig = new Object();
hconfig.right2="Sie k�nnen hier Ihre aktuelle Konfiguration Backupen f�r den Fall das sie Ihren Router einmal Reseten m�ssen.<br /><br />Klicken Sie den <em>Backup</em> Button um ihre Konfiguration zu sichern.";
hconfig.right4="Klicken Sie den <em>Durchsuchen...</em> Button um eine Konfigurations Datei zu auszuw�hlen die auf ihrem PC gespeichert ist.<br /><br />Klicken Sie den <em>" + sbutton.restore + "</em> Button um alle aktuellen Konfigurationseintr�ge mit ihrer neuen Konfiguration zu �berschreiben.";


// **************************************************************** DDNS.asp **********************************************************************//

var ddns = new Object();
ddns.titl=" - Dynamic DNS"
ddns.h2="Dynamic Domain Name System (DDNS)";
ddns.legend="DDNS";
ddns.srv="DDNS Service";
ddns.emailaddr="E-mail Adresse";
ddns.typ="Type";
ddns.dynamic="Dynamisch";
// ddns.static="Static"; Please note: Mozilla doesn't like ".static", use ".sttic" , Eko 22.mar.06
ddns.custom="Spezifisch";
ddns.wildcard="Wildcard";
ddns.statu="DDNS Status";

var ddnsm = new Object();
ddnsm.dyn_strange="Ungew�hnliche Server antwort, ist dies der Richtige Server ?";
ddnsm.dyn_good="DDNS erfolgreich geupdated";
ddnsm.dyn_noupdate="Diesmal ist kein update erforderlich";
ddnsm.dyn_nohost="Der hostname existiert nicht";
ddnsm.dyn_notfqdn="Der Hostname ist nicht korrekt";
ddnsm.dyn_yours="Der Host geh�rt ihnen nicht";
ddnsm.dyn_abuse="Der Host wurde wegen mi�brauchs blockiert";
ddnsm.dyn_nochg="Die IP-Adresse hat sich seit dem letzten update nicht ge�ndert";
ddnsm.dyn_badauth="Authentizifierung fehlgeschlagen (Nutzername oder Passwort)";
ddnsm.dyn_badsys="Ung��tiger System parameter";
ddnsm.dyn_badagent="Dieser Nutzeragent wurde blockiert";
ddnsm.dyn_numhost="Zu viele oder zu wenige Hosts gefunden";
ddnsm.dyn_dnserr="DNS interner fehler";
ddnsm.dyn_911="Unerwarteter Fehler 911";
ddnsm.dyn_999="Unerwarteter Fehler 999";
ddnsm.dyn_donator="Dieses Features ist nur f�r Spender verf�gbar, bitte spenden sie";
ddnsm.dyn_uncode="Unbekannter R�ckgabewert";

ddnsm.tzo_good="Operation erfolgreich ausgef�hrt ";
ddnsm.tzo_noupdate="Diesmal ist kein update notwendig";
ddnsm.tzo_notfqdn="Ung�ltiger Domainname";
ddnsm.tzo_notmail="Ung�ltige Email";
ddnsm.tzo_notact="Ung�ltige Aktion";
ddnsm.tzo_notkey="Ung�ltiger Schl�ssel";
ddnsm.tzo_notip="Ung�ltige IP-Adresse";
ddnsm.tzo_dupfqdn="Duplizierter Domainname";
ddnsm.tzo_fqdncre="Domainname wurde schon erstellt";
ddnsm.tzo_expired="Der Account ist abgelaufen";
ddnsm.tzo_error="Ein unerwarteter Server fehler";

ddnsm.zone_701="Die Zone ist nicht in diesem Account eingestellt";
ddnsm.zone_702="Update fehlgeschlagen";
ddnsm.zone_703="Einer der Parameter <em>zones</em> oder <em>host</em> wird ben�tigt";
ddnsm.zone_704="Die Zone mu� eine g�ltiger <em>dotted</em> Internet Name sein";
ddnsm.zone_705="Zone darf nicht leer sein";
ddnsm.zone_707="Doppeltes Update for den selben host/ip, bitte ver�ndern sie die client einstellungen";
ddnsm.zone_201="Keiner der Eintr�ge erfordert ein update";
ddnsm.zone_badauth="Authorisation fehlgeschlagen (Nutzername oder Passwort)";
ddnsm.zone_good="ZoneEdit wurde erfolgreich geupdatet";
ddnsm.zone_strange="Eigenartige Server Antwort! Sind Sie sicher das sie mit dem richtigen Server verbunden sind ?";

ddnsm.all_closed="DDNS server ist aktuelle geschlossen";
ddnsm.all_resolving="Domainnamen Aufl�sen";
ddnsm.all_errresolv="Domainname aufl�sung fehlgeschlagen";
ddnsm.all_connecting="Mit Server verbinden";
ddnsm.all_connectfail="Verbindung mit Server fehlgeschlagen";
ddnsm.all_disabled="DDNS funktion ist abgeschaltet";
ddnsm.all_noip="Keine Internet Verbindung";

//help container
var hddns = new Object();
hddns.right2="DDNS erlaubt ihnen den Zugriff auf ihr Netzwerk unter Zuhilfename eines Domainnamens anstatt einer IP-Adresse. \
	Der Service verwaltet und �ndert die IP-Adresse sowie die Domaininformation dynamisch.\
	Sie m�ssen sich f�r den Service bei TZO.com, ZoneEdit.com oder DynDNS.ORG anmelden.";








// **************************************************************** Diagnostics.asp **********************************************************************//

var diag = new Object();
diag.titl=" - Diagnose";
diag.h2="Diagnose";
diag.legend="Kommandozeile";
diag.cmd="Kommandos";
diag.startup="Startup";
diag.firewall="Firewall";

//help container
var hdiag = new Object();
hdiag.right2="Hier k�nnen Sie mit Hilfe des Web interfaces Kommandos auf der Kommandozeile starten. Geben Sie einfach ihre Kommandos in das Textfeld ein und Klicken Sie <em>" + sbutton.runcmd + "</em> um diese Auszuf�hren.";



// **************************************************************** DMZ.asp **********************************************************************//

var dmz = new Object();
dmz.titl=" - DMZ";
dmz.h2="Demilitarisierte Zone (DMZ)";
dmz.legend="DMZ";
dmz.serv="DMZ Nutzen";
dmz.host="DMZ Host IP-Adresse";


//help container
var hdmz = new Object();
hdmz.right2="Wenn Sie diese Option aktivieren wird der angegebende Host in das Internet durchgeschaltet. Alle Ports sind dann vom Internet aus erreichbar";



// **************************************************************** Factory_Defaults.asp **********************************************************************//

var factdef = new Object();
factdef.titl=" - Werks Einstellungen";
factdef.h2="Werks Einstellungen";
factdef.legend="Router Einstellungen zur�ck setzen";
factdef.restore="Werkseinstellungen Wiederherstellen";

factdef.mess1="Warnung! Wenn Sie OK Clicken wird das Ger�t auf Werks Einstellungen zur�cl gesetzt und alle vorherigen einstellunge gel�scht.";

//help container
var hfactdef = new Object();
hfactdef.right1="Diese Funktion setzt alle Einstellungen auf Werkszustand zur�ck. Alle ihre Konfigurationen werden gel�scht.";

// **************************************************************** FilterIP%AC.asp **********************************************************************//

var filterIP = new Object();
filterIP.titl=" - Liste der PCs";
filterIP.h2="Liste der PCs";
filterIP.h3="Geben Sie die MAC Adresse der PC in folgendem Format an: xx:xx:xx:xx:xx:xx";
filterIP.h32="Geben Sie die IP-Adresse des PC's ein";
filterIP.h33="Geben Sie den IP-Bereich der PC's an";
filterIP.ip_range="IP Bereich";


// **************************************************************** Filter.asp **********************************************************************//

var filter = new Object();
filter.titl=" - Zugriffs Beschr�nkung";
filter.h2="Internet Zugriff";
filter.legend="Zugriffs Richtlinie";
filter.restore="Werkseinstellungen Wiederherstellen";
filter.pol="Richtlinie";
filter.polname="Richtlinien Name";
filter.pcs="PCs";
filter.polallow="Internet zugriff w�rend der angegebenen Tage und Stunden.";
filter.legend2="Tage";
filter.time="Zeiten";
filter.h24="24 Stunden";
filter.legend3="Blockierte Services";
filter.catchall="Alle P2P Protokolle wegfangen";
filter.legend4="Webseite mit Hilfe einer URL blockieren";
filter.legend5="Webseite mit Hilfe von Schlagworten blockieren";

filter.mess1="L�schen der Richtlinie?";
filter.mess2="Sie m�ssen mindestens einen Tag angeben.";
filter.mess3="Die Endzeit mu� gr��er sein als die Startzeit.";

//help container
var hfilter = new Object();
hfilter.right2="Sie k�nnen bis zu 10 Zugriffs Richtlinien definieren. Klicken Sie <em>" + sbutton.del + "</em> um eine Richtlinie zu l�schen, oder <em>" + sbutton.summary + "</em> um sich die zusammenfassung der Richtlinie anzuschauen.";
hfilter.right4="Hier k�nnnen Sie eine Richtlinie Ein oder Ausschalten.";
hfilter.right6="Hier k�nnen Sie ihrer Richtlinie einen Namen zuweisen.";
hfilter.right8="Hier k�nnen Sie ein oder mehere Tage selektieren an dem die Richtlinie g�ltig ist.";
hfilter.right10="Hier k�nnen Sie die Tageszeit angeben an dem die Richtlinie g�ltig ist.";
hfilter.right12="Hier k�nnen Sie Service ausw�hlen die Blockiert werden sollen. Klicken Sie <em>" + sbutton.filterSer + "</em> um die Einstellungen zu �ndern.";
hfilter.right14="Hier k�nnen Sie diverse Webseiten mit Hilfe einer URL Blockieren.";
hfilter.right16="Hier k�nnen Sie mit hilfe von Schlagworten Webseiten blockieren die diese enthalten.";






// **************************************************************** FilterSummary.asp **********************************************************************//

var filterSum = new Object();
filterSum.titl=" - Zugriffs Beschr�nkung Zusammenfassung";
filterSum.h2="Internet Richtlinie Zusammenfassung";
filterSum.polnum="Nr.";
filterSum.polday="Uhrzeit";



// **************************************************************** Firewall.asp **********************************************************************//

var firewall = new Object();
firewall.titl=" - Firewall";
firewall.h2="Sicherheit";
firewall.legend="Firewall Schutz";
firewall.firewall="SPI Firewall";
firewall.legend2="Zus�tzliche Filter";
firewall.proxy="Filter Proxy";
firewall.cookies="Filter Cookies";
firewall.applet="Filter Java Applets";
firewall.activex="Filter ActiveX";
firewall.legend3="WAN Zugriffe Blockieren";
firewall.ping="Anonyme Internet Zugriffsversuche blockieren";
firewall.muticast="Filter Multicast";
filter.nat="Filter Internet NAT Redirection";
filter.port113="Filter IDENT (Port 113)";

//help container
var hfirewall = new Object();
hfirewall.right2="Schaltet die SPI firewall ein oder aus.";

// **************************************************************** Forward.asp **********************************************************************//

var prforward = new Object();
prforward.titl=" - Port Bereichs Weiterleitung";
prforward.h2="Port Bereichs Weiterleitung";
prforward.legend="Weiterleitungen";
prforward.app="Applikation";

//help container
var hprforward = new Object();
hprforward.right2="Einige Applikationen ben�tigen manchmal offene Ports damit diese korrekt funktionieren. \
	Beispiele f�r diese Applikationen sind Server oder einige Online Spiele. \
	Wenn eine Anfrage auf einem dieser Ports aus dem Internet kommt, wird der Router diese auf den entsprechend angegebenen Computer weiterleiten. \
	Aus Sicherheitsgr�nden, sollten die Weiterleitung nur auf Ports beschr�nken die Sie ben�tigen.";
	


// **************************************************************** ForwardSpec.asp **********************************************************************//

var pforward = new Object();
pforward.titl=" - Port Weiterleitung";
pforward.h2="Port Weiterleitung";
pforward.legend="Weiterleitungen";
pforward.app="Applikation";
pforward.from="Von Port";
pforward.to="Nach Port";

//help container
var hpforward = new Object();
hpforward.right2="Einige Applikationen ben�tigen manchmal offene Ports damit diese korrekt funktionieren. \
	Beispiele f�r diese Applikationen sind Server oder einige Online Spiele. \
	Wenn eine Anfrage auf einem dieser Ports aus dem Internet kommt, wird der Router diese auf den entsprechend angegebenen Computer weiterleiten. \
	Aus Sicherheitsgr�nden, sollten die Weiterleitung nur auf Ports beschr�nken die Sie ben�tigen.";








// **************************************************************** Hotspot.asp **********************************************************************//

var hotspot = new Object();
hotspot.titl=" - Hotspot";
hotspot.h2="Hotspot Portal";
hotspot.legend="Chillispot";
hotspot.hotspot="Chillispot";
hotspot.pserver="Prim�rer Radius Server IP/DNS";
hotspot.bserver="Secund�rer Radius Server IP/DNS";
hotspot.dns="DNS IP";
hotspot.url="Umleitungs URL";
hotspot.dhcp="DHCP Interface";
hotspot.radnas="Radius NAS ID";
hotspot.uam="UAM Secret";
hotspot.uamdns="UAM Any DNS";
hotspot.allowuam="UAM Allowed";
hotspot.macauth="MACauth";
hotspot.option="Zus�tzliche Chillispot Optionen";
hotspot.fon_chilli="Chillispot Lokales Nutzer Management";
hotspot.fon_user="Nutzer Liste";
hotspot.http_legend="HTTP Umleitung";
hotspot.http_srv="HTTP Umleitung";
hotspot.http_ip="HTTP Ziel IP";
hotspot.http_port="HTTP Ziel Port";
hotspot.http_net="HTTP Ursprungs Netzwerk";
hotspot.nocat_legend="NoCatSplash";
hotspot.nocat_srv="NoCatSplash";
hotspot.nocat_gateway="Gateway Name";
hotspot.nocat_home="Home Page";
hotspot.nocat_allowweb="Erlaubte Web Hosts";
hotspot.nocat_docroot="Dokumenten Pfad";
hotspot.nocat_splash="Splash URL";
hotspot.nocat_port="Ports Ausschlie�en";
hotspot.nocat_timeout="Login Timeout";
hotspot.nocat_verbose="Verbosity";
hotspot.nocat_route="Nur Routen";
hotspot.smtp_legend="SMTP Umleitung";
hotspot.smtp_srv="SMTP Umleitung";
hotspot.smtp_ip="SMTP Ziel IP";
hotspot.smtp_net="SMTP Ursprungs Netzwerk";
hotspot.shat_legend="Zero IP Config";
hotspot.shat_srv="Zero IP Config";
hotspot.shat_srv2="Zero IP Config eingeschaltet";
hotspot.sputnik_legend="Sputnik";
hotspot.sputnik_srv="Sputnik Agent";
hotspot.sputnik_id="Sputnik Server ID";
hotspot.sputnik_instant="Nutze Sputnik Instant Setup";
hotspot.sputnik_express="Nutze SputnikNet Express";
hotspot.sputnik_about="�ber Sputnik";



// **************************************************************** index_heartbeat.asp *********************************************************//

var idx_h = new Object();
idx_h.srv="Heart Beat Server";
idx_h.con_strgy="Verbindungsstrategie";
idx_h.max_idle="Auf bedarf Verbinden: Maximale Ruhezeit";
idx_h.alive="Lebenserhaltung: Wiederwahl Periode";



// **************************************************************** index_l2tp.asp *********************************************************//

var idx_l = new Object();
idx_l.srv="L2TP Server";



// **************************************************************** index_pppoe.asp *********************************************************//

var idx_pppoe = new Object();
idx_pppoe.use_rp="Nutze RP PPPoE";



// **************************************************************** index_pptp.asp *********************************************************//

var idx_pptp = new Object();
idx_pptp.srv="Nutze DHCP";
idx_pptp.wan_ip="Internet IP-Adresse";
idx_pptp.gateway="Gateway (PPTP Server)";
idx_pptp.encrypt="PPTP Verschl�sselung";



// **************************************************************** index_static.asp *********************************************************//

var idx_static = new Object();
idx_static.dns="Statischer DNS";



// **************************************************************** index.asp *********************************************************//

var idx = new Object();
idx.titl=" - Einstellungen";
idx.h2="Internet Einstellungen";
idx.h22="WLAN Einstellungen";
idx.legend="Internet Verbindungstyp";
idx.conn_type="Verbindungstyp";
idx.stp="STP";
idx.stp_mess="(F�r COMCAST Provider ausschalten)";
idx.optional="Zus�tzliche Einstellungen (Wird von einigen Providern ben�tigt)";
idx.mtu="MTU";
idx.h23="Netzwerk Einstellungen";
idx.routerip="Router IP";
idx.lanip="Lokale IP-Adresse";
idx.legend2="WAN Port";
idx.wantoswitch="WAN Port dem Switch zuweisen";
idx.legend3="Zeit Einstellungen";
idx.timeset="Zeitzone / Sommerzeit (DST)";
idx.localtime="Nutze Lokale Zeit";
idx.static_ip="Static IP";
idx.dhcp="Automatische Konfiguration - DHCP";
idx.dhcp_legend="Netzwerk Adress Server Einstellungen (DHCP)";
idx.dhcp_type="DHCP Typ";
idx.dhcp_srv="DHCP Server";
idx.dhcp_fwd="DHCP Weiterleitung";
idx.dhcp_start="Start IP-Adresse";
idx.dhcp_end="End IP-Adresse";		//used in Status_Lan.asp
idx.dhcp_maxusers="Maximale DHCP Nutzer";
idx.dhcp_lease="Client Lease Zeit";
idx.dhcp_dnsmasq="Nutze DNSMasq f�r DHCP";
idx.dns_dnsmasq="Nutze DNSMasq f�r DNS";
idx.auth_dnsmasq="DHCP-Authoritative";

//help container
var hidx = new Object();
hidx.right2="Diese Einstellung wird von fast allen Providern genutzt.";
hidx.right4="Geben Sie hier den Hostnamen ein, der Ihnen vom Provider zugewiesen wurde.";
hidx.right6="Geben Sie hier den Domainnamen ein, der Ihnen vom Provider zugewiesen wurde.";
hidx.right8="Dies ist die Lokale IP-Adresse ihres Routers im LAN.";
hidx.right10="Dies ist die Subnetz Maske ihres Routers.";
hidx.right12="Erlaubt dem Router ihre IP-Adressen zu verwalten.";
hidx.right14="Die erste IP-Adresse die vergeben werden soll.";
hidx.right16="Sie k�nnen hier die Anzahl der IP-Adressen einschr�nken die der Router maximal vergeben kann.";
hidx.right18="W�hlen Sie hier ihre Zeitzone und die Sommerzeit Einstellungen. Der Router kann auch die Lokalzeit nutzen.";



// **************************************************************** Join.asp **********************************************************************//

var join = new Object();

//sshd.webservices
join.titl=" - Beitreten";
join.mess1="Sie sind nun erfolgreich dem folgendem Netzerk als Client beigetreten: ";



// **************************************************************** Log_incoming.asp **********************************************************************//

var log_in = new Object();
log_in.titl=" - Eingehende Log Tabelle";
log_in.h2="Eingehende Log Tabelle";
log_in.th_ip="Ursprungs IP";
log_in.th_port="Ziel Port Nummer";



// **************************************************************** Log_outgoing.asp **********************************************************************//

var log_out = new Object();
log_out.titl=" - Ausgehende Log Tabelle";
log_out.h2="Ausgehende Log Tabelle";
log_out.th_lanip="LAN IP";
log_out.th_wanip="Ziel URL/IP";
log_out.th_port="Service/Port Nummer";



// **************************************************************** Log.asp **********************************************************************//

var log = new Object();
log.titl=" - Log";
log.h2="Log Verwaltung";
log.legend="Log";
log.lvl="Log Level";
log.drop="Abgewiesen";
log.reject="Zur�ckgewiesen";
log.accept="Akzeptiert";



// **************************************************************** Management.asp **********************************************************************//

var management = new Object();
management.titl=" - Administration";
management.h2="Router Management";

management.psswd_legend="Router Password";
management.psswd_user="Router Nutzername";
management.psswd_pass="Router Passwort";
management.pass_conf="Zum best�tigen Wiedereingeben";

management.remote_legend="Fernzugriff";
management.remote_gui="Web GUI Management";
management.remote_https="Nutze HTTPS";
management.remote_guiport="Web GUI Port";
management.remote_ssh="SSH Management";
management.remote_sshport="SSH Port";

management.web_legend="Web Zugriff";
management.web_refresh="Automatisches Auffrischen (in seconds)";
management.web_sysinfo="Info Seite einschalten";
management.web_sysinfopass="Info Seite Passwort Schutz";
management.web_sysinfomasq="Info Seite MAC Maskierung";

management.boot_legend="Beim Booten Warten";
management.boot_srv="Beim Booten Warten";

management.cron_legend="Cron";
management.cron_srvd="Cron";

management.loop_legend="Loopback";
management.loop_srv="Loopback";

management.wifi_legend="802.1x";
management.wifi_srv="802.1x";

management.ntp_legend="NTP Client";
management.ntp_srv="NTP";

management.rst_legend="Reset Knopf";
management.rst_srv="Reset Knopf";

management.routing_legend="Routing";
management.routing_srv="Routing";

management.wol_legend="Wake-On-LAN";
management.wol_srv="WOL";
management.wol_pass="SecureOn Passwort";
management.wol_mac="MAC-Adressen<br/>( Format: xx:xx:xx:xx:xx:xx )";

management.ipv6_legend="IPv6 Unterst�tzung";
management.ipv6_srv="IPv6";
management.ipv6_rad="Radvd eingeschaltet";
management.ipv6_radconf="Radvd Konfiguration";

management.jffs_legend="JFFS2 Unterst�tzung";
management.jffs_srv="JFFS2";
management.jffs_clean="JFFS2 L�schen";

management.lang_legend="Sprach Auswahl";
management.lang_srv="Sprache";
management.lang_bulgarian="Bulgarisch";
management.lang_chinese_traditional="Chinesisch Traditionell";
management.lang_chinese_simplified="Chinesisch Simplified";
management.lang_croatian="Kroatisch";
management.lang_czech="Czechisch";
management.lang_dutch="Niederl�ndisch";
management.lang_english="Englisch";
management.lang_portuguese_braz="Portugiesisch (Brasil.)";
management.lang_french="Franz�sisch";
management.lang_german="Deutsch";
management.lang_italian="Italienisch";
management.lang_brazilian="Portugiesisch (Braz.)";
management.lang_slovenian="Slowenisch";
management.lang_spanish="Spanisch";
management.lang_swedish="Schwedisch";

management.net_legend="IP Filter Einstellungen (F�r P2P bitte anpassen)";
management.net_port="Maximale Ports";
management.net_tcptimeout="TCP Timeout (in Sekunden)";
management.net_udptimeout="UDP Timeout (in Sekunden)";

management.clock_legend="�bertakten";
management.clock_frq="Frequenz";
management.clock_support="Nicht unterst�tzt";

management.mmc_legend="MMC/SD Karten unterst�tzung";
management.mmc_srv="MMC Ger�t";

management.samba_legend="Samba FS Automount";
management.samba_srv="SMB Filesystem";
management.samba_share="Share";
management.samba_stscript="Startscript";

management.SIPatH_srv="SIPatH";
management.SIPatH_port="SIP Port";
management.SIPatH_domain="SIP Domain";

management.gui_style="Router GUI Style";

//help container
var hmanagement = new Object();
hmanagement.right1="Automatisches Auffrischen:";
hmanagement.right2="Justiert das automatische auffrischen diverser GUI Elemente. 0 Schaltet dieses Feature ab.";



// **************************************************************** Port_Services.asp **********************************************************************//

var portserv = new Object();
portserv.titl=" - Port Services";
portserv.h2="Port Services";



// **************************************************************** QoS.asp **********************************************************************//

var qos = new Object();
qos.titl=" - Service Qualit�t";
qos.h2="Service Qualit�t (QoS)";
qos.legend="QoS Einstellungen";
qos.srv="Start QoS";
qos.type="Packet Scheduler";
qos.uplink="Uplink (kbps)";
qos.dnlink="Downlink (kbps)";
qos.gaming="F�r Spiele optimieren";
qos.legend2="Service Priorit�t";
qos.prio_x="Ausgenommen";
qos.prio_p="Premium";
qos.prio_e="Express";
qos.prio_s="Standard";
qos.prio_b="Bulk";
qos.legend3="Netzmasken Priorit�t";
qos.ipmask="IP/Maske";
qos.maxrate_b="Max Kbits";
qos.maxrate_o="Max Rate";
qos.legend4="MAC Priorit�t";
qos.legend5="Ethernet Port Priorit�t";
qos.legend6="Standard Bandbreiten Zuweisung";
qos.bandwith="Banbreite in Kbits";

//help container
var hqos = new Object();
hqos.right1="Uplink:";
hqos.right2="Setzen Sie dies auf 80%-95% (max) ihres totalen upload limits.";
hqos.right3="Downlink:";
hqos.right4="Setzen Sie dies auf 80%-100% ihres totalen download limits.";
hqos.right6="Hier konnen sie die Datenrate unter beachtung der Applikation die diese verwendet kontrollieren.";
hqos.right8="Hier k�nnen sie die Priorit�t allen Traffics IPs oder IP Bereichen zuweisen.";
hqos.right10="Hier k�nnen Sie die Priorit�t f�r jeden Traffik ihres Ger�tes in ihrem Netzwerk angeben, indem Sie von dem Ger�t die MAC Adresse, die Priorit�t und einen Namen angeben";
hqos.right12="Hier k�nnen Sie die Datenrate der Physikalischen LAN Ports an ihrem Router angeben. Sie k�nnen hier Priorit�ten f�r die Ger�te angeben die an Port 1 - 4 angeschlossen sind";



// **************************************************************** RouteTable.asp **********************************************************************//

var routetbl = new Object();
routetbl.titl=" - Routen Tabelle";
routetbl.h2="Routen Tabelle";
routetbl.th1="Ziel LAN IP";



// **************************************************************** Routing.asp **********************************************************************//

var route = new Object();
route.titl=" - Routing";
route.h2="Erweitertes Routing";
route.mod="Operations Modus";
route.bgp_legend="BGP Einstellungen";
route.bgp_ip="Nachbar IP";
route.bgp_as="Nachbar AS#";
route.rip2_mod="RIP2 Router";
route.ospf_mod="OSPF Router";
route.gateway_legend="Dynamisches Routing";
route.static_legend="Statisches Routing";
route.static_setno="W�hle Set Nummer";
route.static_name="Routen Name";
route.static_ip="Ziel LAN IP";

//help container
var hroute = new Object();
hroute.right2="Wenn ihr Router die Internet Verbindung bereit stellt, nutzen Sie bitte den <em>Gateway</em> Modus. Wenn ein anderer Router in ihrem Netzwerk existiert, nutzen Sie den <em>Router</em> Modus.";
hroute.right4="Dies ist eine einzigartige Routen Nummer, Sie k�nnen bis zu 20 Routen definieren.";
hroute.right6="Geben Sie einen Namen an den Sie der Router zuweisen m�chten.";
hroute.right8="Dies ist die IP-Adresse des Zielnetzes, dem die statische Route zugewiesen wird.";
hroute.right10="Setzt den angewanden Netzwerk Bereich.";




// **************************************************************** Site_Survey.asp **********************************************************************//

var survey = new Object();
survey.titl=" - Netzsuche";
survey.h2="Nachbarliche WLAN Netzwerke";
survey.thjoin="Netz Betreten";



// **************************************************************** Services.asp **********************************************************************//

var service = new Object();
service.titl=" - Services";
service.h2="Services Management";

//kaid
service.kaid_legend="XBOX Kaid";
service.kaid_srv="Start Kaid";
service.kaid_mac="Konsole Macs: (mu� mit einem ; enden)";

//DHCPd
service.dhcp_legend="DHCP Client";
service.dhcp_vendor="Setze Vendorclass";
service.dhcp_legend2="DHCP Server";
service.dhcp_srv="DHCP Daemon";
service.dhcp_jffs2="Nutze JFFS2 f�r Client zuweisungs DB)";
service.dhcp_domain="Genutzte Domain";
service.dhcp_landomain="LAN Domain";
service.dhcp_option="Zus�tzliche DHCPd Optionen";
service.dnsmasq_legend="DNSMasq";
service.dnsmasq_srv="DNSMasq";
service.dnsmasq_loc="Lokaler DNS";
service.dnsmasq_opt="Zus�tzliche DNS Optionen";

//pptp.webservices
service.pptp_legend="PPTP";
service.pptp_srv="PPTP Server";
service.pptp_client="Client IP(s)";
service.pptp_chap="CHAP-Secrets";

//syslog.webservices
service.syslog_legend="System Log";
service.syslog_srv="Syslogd";
service.syslog_ip="Remote Server";

//telnet.webservices
service.telnet_legend="Telnet";
service.telnet_srv="Telnet";

//pptpd_client.webservices
service.pptpd_legend="PPTP Client";
service.pptpd_option="PPTP Client Options";
service.pptpd_ipdns="Server IP oder DNS Name";
service.pptpd_subnet="Entferntes Subnetz";
service.pptpd_subnetmask="Entfernte Subnetz Maske";
service.pptpd_encry="MPPE Verschl�sselung";
service.pptpd_mtu="MTU";
service.pptpd_mru="MRU";
service.pptpd_nat="NAT";

//rflow.webservices
service.rflow_legend="RFlow / MACupd";
service.rflow_srv1="RFlow";
service.rflow_srv2="MACupd";

//pppoe-relay.webservices
service.pppoe_legend="PPPOE Relay";
service.pppoe_srv="Relay";

//snmp.webservices
service.snmp_legend="SNMP";
service.snmp_srv="SNMP";
service.snmp_loc="Standort";
service.snmp_contact="Kontakt";
service.snmp_name="Name";
service.snmp_read="RO Community";
service.snmp_write="RW Community";

//openvpn.webservices
service.vpn_legend="OpenVPN Client";
service.vpn_srv="Start OpenVPN";
service.vpn_ipname="Server IP/Name";
service.vpn_mtu="TUN MTU Einstellungen";
service.vpn_mru="TUN MTU Extra";
service.vpn_mss="TCP MSS";
service.vpn_compress="Nutze LZO Compression";
service.vpn_tunnel="Tunnel Protokol";
service.vpn_srvcert="�ffentliches Server Cert";
service.vpn_clicert="�ffentliches Client Cert";
service.vpn_clikey="Privater Client Schl�ssel";




//sshd.webservices
service.ssh_legend="Secure Shell";
service.ssh_srv="SSHd";
service.ssh_password="Passwort Login";
service.ssh_key="Authorisierte Schl�ssel";

// **************************************************************** Sipath.asp + cgi **********************************************************************//

var sipath = new Object();
sipath.titl=" - SiPath Overview";
sipath.phone_titl=" - Telefonbuch";
sipath.status_titl=" - Status";



// **************************************************************** Status_Lan.asp **********************************************************************//

var status_lan = new Object();
status_lan.titl=" - LAN Status";
status_lan.h2="Lokales Netzwerk";
status_lan.legend="LAN Status";
status_lan.h22="Dynamisches Host Konfigurations Protokol";
status_lan.legend2="DHCP Status";
status_lan.legend3="DHCP Clients";

//help container
var hstatus_lan = new Object();
hstatus_lan.right2="Dies ist die Router MAC-Adresse wie sie in ihrem lokalen Ethernet Netzwerk sichtbar ist.";
hstatus_lan.right4="Dies zeigt die Router IP-Adresse wie sie in ihrem lokalen Ethernet Netzwerk sichtbar ist.";
hstatus_lan.right6="Wenn der Router eine Subnetz Maske nutzt, wird diese hier angezeigt.";
hstatus_lan.right8="Wenn Sie den router als DHCP Server nutzen wird dies hier angezeigt.";
hstatus_lan.right10="Indem Sie auf irgendeine MAC-Adresse klicken erhalten Sie die OUI ID welche den Hersteller des ger�ts darstellt.";

// **************************************************************** Status_Router.asp **********************************************************************//

var status_router = new Object();
status_router.titl=" - Router Status";
status_router.h2="Router Information";
status_router.legend="System";
status_router.sys_model="Router Model";
status_router.sys_firmver="Firmware Version";
status_router.sys_time="Aktuelle Zeit";
status_router.sys_up="Uptime";
status_router.sys_load="Auslastung";
status_router.legend2="CPU";
status_router.cpu="CPU Model";
status_router.clock="Taktfrequenz";
status_router.legend3="Speicher";
status_router.mem_tot="Gesammt Verf�gbar";
status_router.mem_free="Frei";
status_router.mem_used="Genutzt";
status_router.mem_buf="Puffer";
status_router.mem_cached="Cached";
status_router.mem_active="Aktiv";
status_router.mem_inactive="Inaktiv";
status_router.legend4="Netzwerk";
status_router.net_maxports="IP Filter Maximale Ports";
status_router.net_conntrack="Aktive IP Verbindungen";
status_router.h22="Internet";
status_router.legend5="Konfigurationstyp";
status_router.www_login="Login Typ";
status_router.www_loginstatus="Login Status";

//help container
var hstatus_router = new Object();
hstatus_router.right2="Dies ist der Name des Routers wie er unter <i>Setup</i> gesetzt wurde.";
hstatus_router.right4="Dies ist die Router MAC-Adresse, Wie sie von ihrem Anbieter gesehen wird.";
hstatus_router.right6="Dies ist die aktuelle Firmware des Routers.";
hstatus_router.right8="Dies ist die Zeit wie sie unter <i>Setup</i> gesetzt wurde.";
hstatus_router.right10="Dies ist die seit dem Start des Routers gemessene Zeit.";
hstatus_router.right12="Diese wird in 3 Zahlen angeben, die die Systemauslastung w�rend der letzten Ein, F�nf, und F�nfzehn minuten, repr�sentieren.";
hstatus_router.right14="Dies zeigt die Informationen an die von ihrem Anbieter zum verbinden in das Internet ben�tigt werden. \
				Diese Informationen wurden auf dem Setup Tap eingegeben. Sie k�nnen hier auch ihre verbindung <em>Verbinden</em> oder <em>Trennen</em> indem Sie den entsprechenden button klicken.";


// **************************************************************** Status_SputnikAPD.asp **********************************************************************//

var status_sputnik = new Object();
status_sputnik.titl=" - Sputnik Agent Status";
status_sputnik.h2="Sputnik&reg; Agent&trade;";
status_sputnik.manage="Verwaltet von";
status_sputnik.license="SCC Lizenz Nr.";

//help container
var hstatus_sputnik = new Object();
hstatus_sputnik.right1="Sputnik Agent Status";
hstatus_sputnik.right2="Diese Seite zeigt ihnen den Status des Sputnik Agents an.";
hstatus_sputnik.right4="Das Sputnik Control Center (SCC) zu dem ihr Accesspoint verbunden ist.";
hstatus_sputnik.right6="Der aktuelle Agent Status.";
hstatus_sputnik.right8="Die Lizenz Nummer ihres Sputnik Control Centers.";



// **************************************************************** Status_Wireless.asp **********************************************************************//

var status_wireless = new Object();
status_wireless.titl=" - WLAN Status";
status_wireless.h2="WLAN";
status_wireless.legend="WLAN Status";
status_wireless.net="Network";
status_wireless.pptp="PPTP Status";
status_wireless.legend2="Packet Info";
status_wireless.rx="Empfangen (RX)";
status_wireless.tx="�bermittelt (TX)";
status_wireless.h22="WLAN Knoten";
status_wireless.legend3="Clients";
status_wireless.signal_qual="Signal Qualit�t";
status_wireless.wds="WDS Nodes";

//help container
var hstatus_wireless = new Object();
hstatus_wireless.right2="Dies ist die Router MAC Adresse wie sie in ihrem lokalen WLAN Netzwerk sichbar ist.";
hstatus_wireless.right4="Wie auf dem WLAN Tab angegeben, zeigt ihnen dies den Modus ihres WLAN Interface an (Mixed, Nur-G, or Ausgeschaltet).";




// **************************************************************** Triggering.asp **********************************************************************//

var trforward = new Object();
trforward.titl=" - Port Triggering";
trforward.h2="Port Triggering";
trforward.legend="Weiterleitungen";
trforward.trrange="Getriggerter Port Bereich";
trforward.fwdrange="Weitergeleiteter Port Bereich";
trforward.app="Applikation";

//help container
var htrforward = new Object();
htrforward.right2="Geben Sie den Applikationsnamen des Triggers ein.";
htrforward.right4="Gibt f�r jede Applikation den getriggerten Port Bereich an. Konsultieren Sie hierzu auch die Dokumentation ihrer Applikation.";
htrforward.right6="Gibt f�r jede Applikation den weitergeleiteten Port Bereich an. Konsultieren Sie hierzu auch die Dokumentation ihrer Applikation.";
htrforward.right8="Geben Sie den Start Port des Getriggerten und Weitergeleiteten Bereichs an.";
htrforward.right10="Geben Sie den End Port des Getriggertern und Weitergeleiteten Bereichs an.";

// **************************************************************** Upgrade.asp **********************************************************************//

var upgrad = new Object();
upgrad.titl=" - Firmware Upgrade";
upgrad.h2="Firmware Management";
upgrad.legend="Firmware Upgrade";
upgrad.info1="Nach dem Flashen zur�cksetze auf";
upgrad.resetOff="Kein Reset";
upgrad.resetOn="Standard Einstellungen";
upgrad.file="Firmware Upgrade Datei";
upgrad.warning="W A R N U N G";
upgrad.mess1="Das Upgraden der Firmware kann einige Minuten dauern.<br />Bitte schalten sie nicht das Ger�t ab und dr�cken Sie auch nicht den Reset Knopf!";

//help container
var hupgrad = new Object();
hupgrad.right2="Klicken Sie auf den <em>Durchsuchen...</em> button um eine Firmware auszuw�hlen die auf das Ger�t hochgeladen werden soll.<br /><br /> \
			Klicken Sie den <em>Upgrade</em> button um den Upgrade Prozess zu starten. Das Upgrade darf nicht unterbrochen werden.";




// **************************************************************** UPnP.asp **********************************************************************//

var upnp = new Object();
upnp.titl=" - UPnP";
upnp.h2="Universal Plug und Play (UPnP)";
upnp.legend="Weiterleitungen";
upnp.legend2="UPnP Konfiguration";
upnp.serv="UPnP Service";
upnp.clear="L�scht die Port Weiterleitungen beim Start";
upnp.url="Sende Pr�sentations URL";
upnp.msg1="Klicken sie um das Lease zu L�schen";
upnp.msg2="Alle Eintr�ge l�schen?";


//help container
var hupnp = new Object();
hupnp.right2="Klicken sie auf den Papierkorb um einen individuellen Eintrag zu l�schen.";
hupnp.right4="Erlaubt Applikationen eine Automatische Port weiterleitung auf dem Router durchzuf�hren.";

// ******************************************* VPN.asp *******************************************//

var vpn = new Object();
vpn.titl=" - VPN";
vpn.h2="Virtual Private Network (VPN)";
vpn.legend="VPN Durchleitung";
vpn.ipsec="IPSec Durchleitung";
vpn.pptp="PPTP Durchleitung";
vpn.l2tp="L2TP Durchleitung";

//help container
var hvpn = new Object();
hvpn.right1="Sie k�nnen hier IPsec,PPTP und/oder L2TP Durchleitung aktivieren im ihren Netzwerkger�ten die Kommuniktation via VPN zu erm�glichen.";



// **************************************************************** Vlan.asp **********************************************************************//

var vlan = new Object();
vlan.titl=" - Virtual LAN";
vlan.h2="Virtual Local Area Network (VLAN)";
vlan.legend="VLAN";
vlan.bridge="Der Bridge Zuweisen";
vlan.tagged="Tagged";
vlan.negociate="Auto-Negotiate";
vlan.aggregation="Link Aggregation<br>on Ports 3 & 4";
vlan.trunk="Trunk";


// **************************************************************** WEP.asp **********************************************************************//

var wep = new Object();
wep.defkey="Standard �bermittelungs Schl�ssel";
wep.passphrase="Schl�sselwort";


// **************************************************************** WOL.asp **********************************************************************//

var wol = new Object();
wol.titl=" - WOL";
wol.h2="Wake-On-LAN";
wol.legend="Verf�gbare Hosts";
wol.legend2="WOL Adressen";
wol.legend3="Ausgabe";
wol.legend4="Manuelles WOL";
wol.enable="WOL Einschalten?";
wol.add_wol="WOL Host Hinzuf�gen";
wol.restore="Werks Einstellungen Wiederherstellen";
wol.mac="MAC-Adresse(n)";
wol.broadcast="Netz Broadcast";
wol.udp="UDP Port";
wol.msg1="Klicken um einen WOL Host zu entfernen";

//help container
var hwol = new Object();
hwol.right2="Diese Seite erlaubt ihnen definierte Hosts in ihrem Netzwerk aufzuwecken (Insofern Sie mit ihrem Router lokal verbunden sind).";
hwol.right4="Die MAC-Adressen werden in folgendem Format angegeben xx:xx:xx:xx:xx:xx (Bsp. 01:23:45:67:89:AB)";
hwol.right6="IP-Adresse ist typischerweise die Broadcast Adresse f�r ihr lokales Netzwerk. Es kann aber auch eine Remote Adresse sein wenn der Host nicht in ihrem lokalen Netzwerk verbunden ist."

// **************************************************************** WanMAC.asp **********************************************************************//

var wanmac = new Object();
wanmac.titl=" - MAC-Adresse Klonen";
wanmac.h2="MAC-Adresse Klonen";
wanmac.legend="MAC-Klonen";
wanmac.wan="Klone WAN MAC";
wanmac.wlan="Klone WLAN MAC";

//help container
var hwanmac = new Object();
hwanmac.right2="Manche Anbieter setzen eine Registration ihrer MAC-Adresse vorraus. \
			Wenn Sie ihre MAC-Adresse nicht neu registrieren m�chten, k�nnen Sie hier ihre MAC-Adresse klonen so wie sie bei ihrem Anbieter registriert ist.";


// **************************************************************** WL_WPATable.asp **************************************************************//
// **************************************************************** WPA.asp **********************************************************************//

var wpa = new Object();
wpa.titl=" - WLAN Sicherheit";
wpa.h2="WLAN Sicherheit";
wpa.secmode="Sicherheitsmodus";
wpa.legend="WLAN Verschl�sselung";
wpa.auth_mode="Network Authentication";
wpa.psk="WPA Pre-Shared Key";
wpa.wpa="WPA";
wpa.radius="Radius";
wpa.gtk_rekey="WPA Group Rekey Interval";
wpa.rekey="Key Renewal Interval (in Sekunden)";
wpa.radius_ipaddr="RADIUS Server IP";
wpa.radius_port="RADIUS Server Port";
wpa.radius_key="RADIUS Key";
wpa.algorithms="WPA Algorithmus";
wpa.shared_key="WPA Shared Key";
wpa.rekeyInt="rekey interval";

//help container
var hwpa = new Object();
hwpa.right1="Sicherheits Modus:";
hwpa.right2="Hier k�nnen Sie zwischen Deaktiviert, WEP, WPA Pre-Shared Key, WPA RADIUS, oder RADIUS w�hlen. Alle Ger�te in ihrem Netzwerk m�ssen den selben Modus verwenden.";



// **************************************************************** WL_FilterTable.asp **************************************************************//

var wl_filter = new Object();
wl_filter.titl=" - MAC-Adressen Filter Liste";
wl_filter.h2="MAC-Adressen Filter Liste";
wl_filter.h3="Geben Sie die MAC-Adresse in folgendem Format an:&nbsp;&nbsp;&nbsp;xx:xx:xx:xx:xx:xx";



// **************************************************************** WL_ActiveTable.asp **************************************************************//

var wl_active = new Object();
wl_active.titl=" - WLAN Aktive Client MAC Liste";
wl_active.h2="WLAN Client MAC List";
wl_active.h3="Aktiviere MAC Filter";
wl_active.active="Aktive PC";
wl_active.inactive="Inaktive PC";



// **************************************************************** Wireless_WDS.asp ***************************************************************//

var wds = new Object();
wds.titl=" - WDS";
wds.h2="WLAN Distribution System";
wds.legend="WDS Einstellungen";
wds.label="Lazy WDS";
wds.label2="WDS Subnetz";
wds.wl_mac="WLAN MAC";
wds.lazy_default="Standard: Abgeschaltet";
wds.nat1="wLAN->WDS";
wds.nat2="WDS->wLAN";
wds.subnet="Subnetz";
wds.legend2="Extra Optionen";



// **************************************************************** Wireless_radauth.asp ***************************************************************//

var radius = new Object();
radius.titl=" - Radius";
radius.h2="Remote Authentication Dial-In User Service";
radius.legend="Radius";
radius.label="MAC Radius Client";
radius.label2="MAC Format";
radius.label3="Radius Server IP";
radius.label4="Radius Server Port";
radius.label5="Erlaubte Unauthentifizierte Nutzer";
radius.label6="Password Format";
radius.label7="RADIUS Shared Secret";
radius.label8="Umgehen wenn Server nicht verf�gbar";
radius.mac="MAC";



// **************************************************************** Wireless_MAC.asp ***************************************************************//

var wl_mac = new Object();
wl_mac.titl=" - MAC Filter";
wl_mac.h2="WLAN MAC Filter";
wl_mac.legend="MAC Filter";
wl_mac.label="Nutze Filter";
wl_mac.label2="Liste enth�lt...";
wl_mac.deny="WLAN-MAC Adressen die nicht auf das Netzwerk zugreifen d�rfen";
wl_mac.allow="WLAN-MAC Adressen die auf das Netzwerk zugreifen d�rfen";



// **************************************************************** Wireless_Basic.asp ***************************************************************//

var wl_basic = new Object();
wl_basic.titl=" - WLAN";
wl_basic.h2="WLAN";
wl_basic.legend="Basis Einstellungen";
wl_basic.label="WLAN Modus";
wl_basic.label2="WLAN Netzwerk Modus";
wl_basic.label3="WLAN Netzwerk Name (SSID)";
wl_basic.label4="WLAN Kanal";
wl_basic.label5="WLAN SSID Broadcast";
wl_basic.label6="Empfindlichkeits Bereich (ACK Timing)";
wl_basic.ap="AP";
wl_basic.client="Client";
wl_basic.clientBridge="Client Bridge";
wl_basic.adhoc="Adhoc";
wl_basic.mixed="Gemischt";
wl_basic.b="Nur-B";
wl_basic.g="Nur-G";
wl_basic.sensitivity="Standard: 20000 meter";

//help container
var hwl_basic = new Object();
hwl_basic.right2="Wenn Sie G Clients komplett ausschlie�en wollen, sollten Sie <em>Nur-B</em> w�hlen. Wenn Sie den WLAN Zugriff abschalten wollen, w�hlen Sie <em>Abschalten</em>.";
hwl_basic.right3="Empfindlichkeits Bereich: ";
hwl_basic.right4="Setzt das ACK Timing. 0 Schaltet das ACK timing komplett ab.";



// **************************************************************** Wireless_Advanced.asp ***************************************************************//

var wl_adv = new Object();
wl_adv.titl=" - Erweiterte WLAN Einstellungen";
wl_adv.h2="Erweiterte WLAN Einstellungen";
wl_adv.legend="Erweiterte Einstellungen";
wl_adv.legend2="WLAN Multimedia Unterst�tzung Einstellungen";
wl_adv.label="Authentication Typ";
wl_adv.label2="Basic Rate";
wl_adv.label3="Transmission Rate";
wl_adv.label4="CTS Protection Mode";
wl_adv.label5="Frame Burst";
wl_adv.label6="Beacon Interval";
wl_adv.label7="DTIM Interval";
wl_adv.label8="Fragmentation Threshold";
wl_adv.label9="RTS Threshold";
wl_adv.label10="Max Associated Clients";
wl_adv.label11="AP Isolation";
wl_adv.label12="TX Antenne";
wl_adv.label13="RX Antenne";
wl_adv.label14="Preamble";
wl_adv.reference="Noise Referenz";
wl_adv.label15="Xmit Power";
wl_adv.label16="Afterburner";
wl_adv.label17="WLAN GUI Zugriff";
wl_adv.label18="WMM Unterst�tzung";
wl_adv.label19="No-Acknowledgement";
wl_adv.table1="EDCA AP Parameter (AP nach Client)";
wl_adv.col1="CWmin";
wl_adv.col2="CWmax";
wl_adv.col3="AIFSN";
wl_adv.col4="TXOP(b)";
wl_adv.col5="TXOP(a/g)";
wl_adv.col6="Admin Forced";
wl_adv.row1="Background";
wl_adv.row2="Best Effort";
wl_adv.row3="Video";
wl_adv.row4="Voice";
wl_adv.table2="EDCA STA Parameters (Client to AP)";
wl_adv.lng="Long"; 					//************* don't use .long ! *************
wl_adv.shrt="Short"; 				//************* don't use .short ! **************

//help container
var hwl_adv = new Object();
hwl_adv.right1="Authenticationtyp:";
hwl_adv.right2="Sie k�nnen Auto oder Shared Key w�hlen. Die Shared Key Authentifizierung ist wesentlich sicherer, allerdings mu� dazu jedes Ger�t in ihrem Netzwerk diesen Betriebsmodus unterst�tzen.";



// **************************************************************** Fail_s.asp & Fail_u_s.asp & Fail.asp **********************************************************************//

var fail = new Object();
fail.mess1="Die Angegebenen Werte sind nicht korrekt. Bitte versuchen Sie es noch einmal.";
fail.mess2="Upgrade fehlgeschlagem.";



// **************************************************************** Success*.asp & Reboot.asp  **********************************************************************//

var success = new Object();
success.saved="Einstellungen gespeichert.";
success.restore="Einstellungen wiederher gestellt.<br/>Das Ger�t wird nun neu gestartet. Bitte warten Sie einen Moment...";
success.upgrade="Upgrade erfolgreich.<br/>Das Ger�t wird nun neu gestartet. Bitte warten Sie einen Moment...";
success.success_noreboot="Die Einstellungen wurden erfolgreich Gespeichert.";
success.success_reboot=success.success_noreboot + "<br />Das Ger�t wird nun neu gestartet. Bitte warten Sie einen Moment...";

success.alert_reset="Alle Konfigurationseintr�ge wurden auf ihre Standardwerte gesetzt.<br /><br />";
success.alert1="Bitte pr�fen sie nachfolgende Dinge bevor Sie sich neu verbinden:";
success.alert2="Sie haben die IP-Adresse Ihres Routers ge�ndert. Bitte erneuern Sie ihre Client-Adresse in ihrem Netzwerk.";
success.alert3="Wenn Sie via WLAN verbunden sind, treten Sie dem Netzwerk erneut bei und klicken Sie auf <em>Fortfahren</em>.";







// *****************************************************		OLD PAGES 		************************************************************************//
// **************************************************************** DHCPTable.asp **********************************************************************//

var dhcp = new Object();
dhcp.titl=" - DHCP Aktive IP-Tabelle";
dhcp.h2="DHCP Aktive IP-Tabelle";
dhcp.server="DHCP Server IP-Adresse :";
dhcp.tclient="Client Hostname";


var donate = new Object();
donate.mb="Sie k�nnen auch durch den Moneybookers Account mb@dd-wrt.com spenden";
