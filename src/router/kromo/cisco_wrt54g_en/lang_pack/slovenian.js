
// ******************************************* COMMON SHARE LABEL *******************************************//
var lang_charset = new Object();
lang_charset.set="iso-8859-2";

var share = new Object();
share.firmware="Strojna programska oprema";
share.time="�as";
share.interipaddr="Internetni IP naslov";
share.more="Ve�...";
share.help="Pomo�";
share.enable="Omogo�i";
share.enabled="Omogo�eno";
share.disable="Onemogo�i";
share.disabled="Onemogo�eno";
share.usrname="Uporabni�ko ime";
share.passwd="Geslo";
share.hostname="Ime gostitelja";
share.domainname="Domensko ime";
share.statu="Status";
share.start="Za�etek";
share.end="Konec";
share.proto="Protokol";
share.ip="IP naslov";
share.mac="MAC naslov";
share.none="Nobeden";
share.none2="ni�";
share.both="Oboje";
share.del="Izbri�i";
share.remove="Odstrani";
share.descr="Opis";
share.from="Od";
share.to="Do";
share.about="Vizitka";
share.everyday="Vsak dan";
share.sun="Nedelja";
share.sun_s="Ned";
share.sun_s1="N";
share.mon="Ponedeljek";
share.mon_s="Pon";
share.mon_s1="P";
share.tue="Torek";
share.tue_s="Tor";
share.tue_s1="T";
share.wed="Sreda";
share.wed_s="Sre";
share.wed_s1="S";
share.thu="�etrtek";
share.thu_s="�et";
share.thu_s1="�";
share.fri="Petek";
share.fri_s="Pet";
share.fri_s1="P";
share.sat="Sobota";
share.sat_s="Sob";
share.sat_s1="S";
share.expires="Pote�e";
share.yes="Da";
share.no="Ne";
share.allow="Dovoli";
share.deny="Prepre�i";
share.range="Obseg";
share.use="Uporabi";
share.mins="Min.";
share.secs="Sek.";
share.routername="Ime usmerjalnika";
share.manual="Ro�no";
share.port="Vrata";
share.ssid="SSID";
share.channel="Kanal";
share.rssi="Rssi";
share.signal="Signal";
share.noise="�um";
share.beacon="beacon";
share.openn="Odprto";
share.dtim="dtim";
share.rates="Hitrost";
share.low="Nizek";
share.medium="Srednji";
share.high="Visok";
share.option="Mo�nosti";
share.rule="Pravilo";
share.lan="LAN";
share.point2point="Povezava med to�kama";
share.nat="NAT";
share.subnet="Podmre�na maska";
share.unmask="Odkrij";
share.deflt="Privzeto";  //don't use share.default !!!
share.all="Vse";
share.auto="Samodejno";
share.right="Dasna";
share.left="Leva";
share.share_key="Skupni klju�";
share.inter="Interval (v sekundah)";
share.srv="Ime storitve";
share.port_range="Obseg vrat";
share.priority="Prioriteta";
share.gateway="Prehod";
share.intrface="Vmesnik";  //don't use share.interface, Mozilla problem!!!
share.router="Usmerjalnik";
share.static_lease="Stati�ni zakupi";
share.srvip="IP stre�nika";
share.localdns="Lokalni DNS";
share.minutes="minut";
share.oui="Iskanje OUI";
share.sttic="Stati�no";
share.connecting="Povezujem";
share.connect="Povezati";
share.connected="Povezano";
share.disconnect="Prekini";
share.disconnected="Prekinjeno";
share.info="Informacija";
share.state="Status";
share.mode="Na�in";
share.encrypt="�ifriranje";
share.key="Klju�";
share.wireless="Brez�i�no";
share.dhcp="DHCP";
share.styl="Stil";
share.err="napaka";
share.errs="napak";



var sbutton = new Object();
sbutton.save="Shrani nastavitve";
sbutton.saving="Shranjeno";
sbutton.cmd="Izvajam";
sbutton.cancel="Prekli�i spremembe";
sbutton.refres="Osve�i";
sbutton.clos="Zapri";
sbutton.del="Izbri�i";
sbutton.continu="Nadaljuj";
sbutton.add="Dodaj";
sbutton.remove="Odstrani";
sbutton.modify="Spremeni";
sbutton.deleted="Izbrisano";
sbutton.delall="Izbri�i vse";
sbutton.autorefresh="Samodejno osve�evanje je vklju�eno";
sbutton.backup="Varnostno kop.";
sbutton.restore="Obnovitev";
sbutton.cptotext="Kopiraj v polje z besedilom";
sbutton.runcmd="Za�eni ukaze";
sbutton.startup="Shrani zagon";
sbutton.firewall="Shrani po�arni zid";
sbutton.wol="Zbujanje";
sbutton.add_wol="Dodaj gostitelja";
sbutton.manual_wol="Ro�no zbujanje";
sbutton.summary="Povzetek";
sbutton.filterIP="Uredi seznam PCjev";
sbutton.filterMac="Uredi seznam MAC filtrov";
sbutton.filterSer="Dodaj/uredi storitev";
sbutton.reboot="Ponovni zagon usmerjalnika";
sbutton.help="   Pomo�  ";
sbutton.wl_client_mac="MAC seznam brez�i�nih odjamalcev";
sbutton.update_filter="Posodobi seznam filtrov";
sbutton.join="Pove�i";
sbutton.log_in="Vhodni dnevnik";
sbutton.log_out="Izhodni dnevnik";
sbutton.apply="Uporabi";
sbutton.edit_srv="Dodaj/uredi storitev";
sbutton.routingtab="Poka�i usmerjalno tabelo";
sbutton.wanmac="Prika�i trenutni PCjev MAC naslov";
sbutton.dhcprel="Sprosti DHCP";
sbutton.dhcpren="Obnovi DHCP";
sbutton.survey="Pregled podro�ja";
sbutton.upgrading="Posodabljanje";
sbutton.upgrade="Posodobi";
sbutton.preview="Predogled";


// ******************************************* COMMON ERROR MESSAGES  *******************************************//
var errmsg = new Object();
errmsg.err0="Vnesti morate uporabni�ko ime.";
errmsg.err1="Vnesti morate ime usmerjalnika.";
errmsg.err2="Izven dovoljenega obmo�ja. Prosimo, do popravite za�etni IP naslov ali �tevilo uporabnikov.";
errmsg.err3="Izbrati morate vsaj dan."
errmsg.err4="Kon�ni �as mora biti ve�ji od za�etnega �asa.";
errmsg.err5="Dol�ina naslova MAC ni pravilna.";
errmsg.err6="Vnesti morate geslo.";
errmsg.err7="Vnesti morate ime gostitelja.";
errmsg.err8="Vnesti morate IP naslov ali domensko ime.";
errmsg.err9="Nepravilni DMZ IP naslov.";
errmsg.err10="Potrditveno geslo se ne ujema z vne�enim geslom. Prosimo, da ponovite vnos.";
errmsg.err11="V geslu presledki niso dovoljeni";
errmsg.err12="Vnesti morate ukaz.";
errmsg.err13="Posodabljanje ni uspelo.";
errmsg.err45="Ni na voljo v HTTPS na�inu! Prosimo, da uporabite HTTP na�in.";
errmsg.err46="Ni na voljo v HTTPS na�inu";


//common.js error messages
errmsg.err14=" vrednost je izven dovoljenih meja [";
errmsg.err15="WAN MAC je izven dovoljenih meja [00 - ff].";
errmsg.err16="drugi znak v MAC mora biti sodo �tevilo : [0, 2, 4, 6, 8, A, C, E].";
errmsg.err17="MAC naslov ni pravilen.";
errmsg.err18="Dol�ina MAC naslova ni pravilna.";
errmsg.err19="MAC naslov ne more biti broadcast naslov."
errmsg.err20="Vnesite MAC naslov v (xx:xx:xx:xx:xx:xx) formatu.";
errmsg.err21="Nepravilni format MAC naslova.";
errmsg.err22="WAN MAC naslov ni pravilen.";
errmsg.err23="Nepravilna hex vrednost ";
errmsg.err24=" najdena v MAC naslovu ";
errmsg.err25="Vrednost klju�a ni pravilna.";
errmsg.err26="Do�ina klju�a ni pravilna.";
errmsg.err27="Nepravina podmre�na maska.";
errmsg.err28=" ima nepravilni znak, biti mora [ 0 - 9 ].";
errmsg.err29=" ima nepravilno ascii kodo.";
errmsg.err30=" ima nepravino hexadecimalno �tevilko.";
errmsg.err31=" vrednost je nepravilna.";
errmsg.err32="IP naslov in prehod nista v isti podmre�i.";
errmsg.err33="IP naslov in prehod ne moreta biti enaka.";
errmsg.err34=" ne sme vsebovati presledka.";

//Wol.asp error messages
errmsg.err35="Vnesti morate MAC naslov za zagon.";
errmsg.err36="Vnesti morate mre�ni broadcast naslov za zagon.";
errmsg.err37="Vnesti morate UDP vrata za zagon.";

//WL_WPATable.asp error messages
//WPA.asp error messages
errmsg.err38="Prosimo, da vnesete skupni klju�!";
errmsg.err39="Nepravilen klju�, biti mora med 8 in 63 ASCII znaki ali 64 hexadecimalnih znakov";
errmsg.err40="Vnesti morate klju� za Klju� ";
errmsg.err41="Nepravilna dol�ina klju�a ";
errmsg.err43="Interval za izmenjavo klju�a";

//config.asp error messages
errmsg.err42="Prosimo, da izberete datoteko za obnovitev.";

//WL_ActiveTable.asp error messages
errmsg.err44="Skupno �tevilo je preseglo 128.";

//Site_Survey.asp error messages
errmsg.err47="nepravilen SSID.";

//Wireless_WDS.asp error messages
errmsg.err48="WDS ni zdru�ljiv s trenutno konfiguracijo usmerjalnika. Prosimo, da preverite naslednje :\n * Brez�i�ni na�in mora biti AP \n * WPA2 ni podprta pod WDS \n * Bre�i�no omre�je v na�inu samo-B ni podprto pod WDS";

//Wireless_radauth.asp error messages
errmsg.err49="Radius je na volju samo v na�inu AP.";

//Wireless_Basic.asp error messages
errmsg.err50="Vnesti morate SSID.";

// Management.asp error messages
errmsg.err51="Usmerjalnik trenutno uporablja privzeto geslo. \
			Zaradi varnosti morate spremeniti geslo preden omogo�ite oddaljeni dostop. \
			Kliknite gumb OK za spremembo gesla. Kliknite gumb Prekli�i za onemogo�en oddaljeni dostop.";
errmsg.err52="Geslo se ne ujema.";

// Port_Services.asp error messages
errmsg.err53="Ko kon�ate, kliknite gumb Uporabi, da shranite nastavitve.";
errmsg.err54="Vnesti morate ime storitve.";
errmsg.err55="Ime storitve �e obstaja.";

// QoS.asp error messages
errmsg.err56="Vrednost vrat je izven obmo�ja [0 - 65535]";

// Routing.asp error messages
errmsg.err57="Zbri�em vnos ?";

// Status_Lan.asp error messages
errmsg.err58="Kliknite za brisane zakupa";

//Status_Wireless.asp error messages
errmsg.err59="Ni na voljo! Prosimo, da omogo�ite brez�i�no omre�je.";

//Upgrade.asp error messages
errmsg.err60="Prosimo, da izberite datoteko za posodobitev.";
errmsg.err61="Nepravilna datoteka.";

// *******************************************  COMMON MENU ENTRIES  *******************************************//
var bmenu= new Object();
bmenu.setup="Namestitev";
bmenu.setupbasic="Osnovna namestitev";
bmenu.setupddns="DDNS";
bmenu.setupmacclone="Kloniranje MAC naslova";
bmenu.setuprouting="Napredno usmerjanje";
bmenu.setupvlan="VLAN";

bmenu.wireless="Brez�i�no";
bmenu.wirelessBasic="Osnovne nastavitve";
bmenu.wirelessRadius="Radius";
bmenu.wirelessSecurity="Brez�i�na za��ita";
bmenu.wirelessMac="Filter MAC";
bmenu.wirelessAdvanced="Napredne nastavitve";
bmenu.wirelessWds="WDS";

bmenu.security="Za��ita";
bmenu.firwall="Po�arni zid";
bmenu.vpn="VPN";

bmenu.accrestriction="Omejitve dostopa";
bmenu.webaccess="Dostop do medmre�ja";


bmenu.applications="Programi &amp; Igre";
bmenu.applicationsprforwarding="Posredovanje obsega vrat";
bmenu.applicationspforwarding="Posredovanje vrat";
bmenu.applicationsptriggering="Pro�enje vrat";
bmenu.applicationsUpnp="UPnP";
bmenu.applicationsDMZ="DMZ";
bmenu.applicationsQoS="QoS";

bmenu.sipath="SIPatH";
bmenu.sipathoverview="Pregled";
bmenu.sipathphone="Imenik";
bmenu.sipathstatus="Status";

bmenu.admin="Administracija";
bmenu.adminManagement="Upravljanje";
bmenu.adminHotspot="Hotspot";
bmenu.adminServices="Storitve";
bmenu.adminAlive="Ohrani �ivo";
bmenu.adminLog="Dnevnik";
bmenu.adminDiag="Diagnostika";
bmenu.adminWol="WOL";
bmenu.adminFactory="Privzete nast.";
bmenu.adminUpgrade="Posodabljanje";
bmenu.adminBackup="Varnostno kop.";


bmenu.statu="Status";
bmenu.statuRouter="Usmerjalnik";
bmenu.statuLAN="LAN";
bmenu.statuSputnik="Agent Sputnik";
bmenu.statuWLAN="Brez�i�no";
bmenu.statuSysInfo="Sis-Info";


// ******************************************* Alive.asp *******************************************//

var alive = new Object();
alive.titl=" - Ohrani �ivo";
alive.h2="Ohrani �ivo";
alive.legend="Razpored ponovnega zagona";
alive.sevr1="Razpored ponovnega zagona";
alive.time="�as (v sekundah)";
alive.hour="Ob nastavljenem �asu";
alive.legend2="WDS/Stra�ni mehanizem";
alive.sevr2="Omogo�i stra�ni mehanizem";
alive.IP="IP naslovi";
alive.legend3="Proksi/Stra�ni mehanizem";
alive.sevr3="Omogo�i proksi stra�ni mehanizem";
alive.IP2="Proksi IP naslov";
alive.port="Proksi vrata";

//help container
var halive = new Object();
halive.right2="Izbira ponovnega zagona usmerjalnika. Cron mora biti omogo�en pod zavihkom Upravljanje.";
halive.right4="Dovoljeni so najve� trije IP naslovi lo�eni s <em>PRESLEDEK</em>.<BR/>Format IP je xxx.xxx.xxx.xxx.";



// ******************************************* config.asp *******************************************//

var config = new Object();
config.titl=" - Varnostno kopiranje & Obnovitev";
config.h2="Konfiguracija varnostnega kopiranja";
config.legend="Varnostno kopiranje nastavitev";
config.mess1="Kliknite na gumb \"" + sbutton.backup + "\" za pronos varnostne kopije na va� ra�unalnik.";
config.h22="Obnovi konfiguracijo";
config.legend2="Obnovi nastavitve";
config.mess2="Izberite datoteko za obnovitev";
config.mess3="O P O Z O R I L O";
config.mess4="Obnovitev je mo�na samo z datoteko ustvarjeno na istem modelu usmerjalnika in isto verzijo strojne programske opreme.<br />Ne nalagajte datotek, ki niso bile ustvarjene s tem uporabni�kim vmesnikom!";

//help container
var hconfig = new Object();
hconfig.right2="Varnostno kopijo lahko naredite v primeru, da morate usmerjalnik ponastaviti na tovarni�ko privzete nastavitve.<br /><br />Kliknite gumb <em>Varnostno kopiranje</em> za varnostno kopiranje trenutne konfiguracije.";
hconfig.right4="Kliknite gumb <em>Prebrskaj...</em> za iskanje varnostne kopije, ki je trenutno na va�em PCju.<br /><br />Kliknite gumb <em>" + sbutton.restore + "</em> za nalaganje varnstne kopije. To bo nadomestilo trenutno konfiguracijo s konfiguracijo shranjeno v varnostni kopiji.";



// ******************************************* DDNS.asp *******************************************//

var ddns = new Object();
ddns.titl=" - Dinami�ni DNS"
ddns.h2="Dynamic Domain Name System (DDNS)";
ddns.legend="DDNS";
ddns.srv="DDNS servis";
ddns.emailaddr="email naslov";
ddns.typ="Tip";
ddns.dynamic="Dinami�ni";
// ddns.static="Static"; Please note: Mozilla doesn't like ".static", use ".sttic" , Eko 22.mar.06
ddns.custom="Prirejen";
ddns.wildcard="Nadomestni znak";
ddns.statu="DDNS status";

var ddnsm = new Object();
ddnsm.dyn_strange="Nepri�akovani odziv stre�nika; ali se priklju�ujete na pravilni stre�nik ?";
ddnsm.dyn_good="DDNS posodobitev je uspela";
ddnsm.dyn_noupdate="Posodobitev trenutno ni potrebna";
ddnsm.dyn_nohost="Ime gostitelja ne obstaja";
ddnsm.dyn_notfqdn="Ime gostitelja ni pravilno";
ddnsm.dyn_yours="Ime gostitelja ni va�a last";
ddnsm.dyn_abuse="Gostitelj je bil blokiran zaradi zlorabe";
ddnsm.dyn_nochg="IP naslov se ni spremenil od zadnje posodobitve";
ddnsm.dyn_badauth="Overjanje ni uspelo (uporabni�ko ime ali geslo)";
ddnsm.dyn_badsys="Neveljavni sistemski parameter";
ddnsm.dyn_badagent="Ta uporabni�ki agent je bil blokiran";
ddnsm.dyn_numhost="Najdeno preve� ali premalo gostiteljev";
ddnsm.dyn_dnserr="DNS notranja napaka";
ddnsm.dyn_911="Nepri�akovana napaka 911";
ddnsm.dyn_999="Nepri�akovana napaka 999";
ddnsm.dyn_donator="Zahtevana funkcija je na voljo samo za donatorje, prosimo, da darujete";
ddnsm.dyn_uncode="Neznana povratna koda";

ddnsm.tzo_good="Operacija zaklju�ena";
ddnsm.tzo_noupdate="Posodobitev trenutno ni potrebna";
ddnsm.tzo_notfqdn="Napa�no domensko ime";
ddnsm.tzo_notmail="Napa�en email";
ddnsm.tzo_notact="Napa�no dejanje";
ddnsm.tzo_notkey="Napa�en klju�";
ddnsm.tzo_notip="Nepravilen IP naslov";
ddnsm.tzo_dupfqdn="Podvojeno domensko ime";
ddnsm.tzo_fqdncre="Domensko ime je �e bilo ustvarjeno za to domensko ime";
ddnsm.tzo_expired="Ra�un je potekel";
ddnsm.tzo_error="Nepri�akovana napaka na stre�niku";

ddnsm.zone_701="Cona ni nastavljena za ta ra�un";
ddnsm.zone_702="Posodobitev ni uspela";
ddnsm.zone_703="Potreben vsaj en parameter: <em>cona</em> ali <em>gostitelj</em>";
ddnsm.zone_704="Cona mora biti veljaveno internetno ime s <em>pikami</em>";
ddnsm.zone_705="Cona ne more biti prazna";
ddnsm.zone_707="Podvojena posodobitev za istega gostitelja/IP, prilagodi odjemal�eve nastavitve";
ddnsm.zone_201="Zapis ne potrebuje posodobitve";
ddnsm.zone_badauth="Overjanje ni uspelo (uporabni�ko ime ali geslo)";
ddnsm.zone_good="ZoneEdit je uspe�no posodobljen";
ddnsm.zone_strange="Nepri�akovani odziv stre�nika; ali se priklju�ujete na pravilni stre�nik ?";

ddnsm.all_closed="DDNS stre�nik je trenutno zaprt";
ddnsm.all_resolving="Rezre�evanje domenskega imena";
ddnsm.all_errresolv="Rezre�evanje domenskega imena ni uspelo";
ddnsm.all_connecting="Priklju�evanje na stre�nik";
ddnsm.all_connectfail="Priklju�evanje na stre�nik ni uspelo";
ddnsm.all_disabled="DDNS funkcija je onemogo�ena";
ddnsm.all_noip="Ni medmre�ne povezave";

//help container
var hddns = new Object();
hddns.right2="DDNS omogo�a dostop do va�e mre�e z domenskim imenom namesto z IP naslovom. \
	Ta storitev upravlja s spremembami va�ega IP naslova in dinami�no posodobi podatke o va�i domeni. \
	Prijaviti se morate pri DynDNS.org, TZO.com ali ZoneEdit.com.";



// ******************************************* Diagnostics.asp *******************************************//

var diag = new Object();
diag.titl=" - Diagnostika";
diag.h2="Diagnostika";
diag.legend="Ukazna lupina";
diag.cmd="Ukazi";
diag.startup="Zagon";
diag.firewall="Po�arni zid";

//help container
var hdiag = new Object();
hdiag.right2="Ukaze lahko za�enete preko uporabni�kega vmesnika. Vnesite ukaze v textovno polje in kliknite <em>" + sbutton.runcmd + "</em>.";



// ******************************************* DMZ.asp *******************************************//

var dmz = new Object();
dmz.titl=" - DMZ";
dmz.h2="Demilitarizirano obmoc�je (DMZ)";
dmz.legend="DMZ";
dmz.serv="Uporabi DMZ";
dmz.host="DMZ gostiteljev IP naslov";


//help container
var hdmz = new Object();
hdmz.right2="Omogo�anje te opcije bo izpostavilo gostitelja medmre�ju. Vsa vrata bodo dostopna z medmre�ja.";



// ******************************************* Factory_Defaults.asp *******************************************//

var factdef = new Object();
factdef.titl=" - Privzete nastavitve";
factdef.h2="Privzete nastavitve";
factdef.legend="Ponastavi nastavitve usmerjalnika";
factdef.restore="Obnovi privzete nastavitve";

factdef.mess1="Opozorilo! �e kliknete OK, bodo nastavljene privzete nastavitve in vse prej�nje nastavitve bodo izbrisane.";

//help container
var hfactdef = new Object();
hfactdef.right1="To bo ponastavilo privzete nastavitve. Vse va�e nastavitve bodo izbrisane.";



// ******************************************* FilterIP%AC.asp *******************************************//

var filterIP = new Object();
filterIP.titl=" - Seznam PCjev";
filterIP.h2="Seznam PCjev";
filterIP.h3="Vnesi PCjev MAC v tem formatu formatu: xx:xx:xx:xx:xx:xx";
filterIP.h32="Vnesi PCjev IP naslov";
filterIP.h33="Vnesi PCjev obseg IP naslovov";
filterIP.ip_range="IP obseg";



// ******************************************* Filter.asp *******************************************//

var filter = new Object();
filter.titl=" - Omejitve dostopa";
filter.h2="Dostop do medmre�ja";
filter.legend="Smernica dostopa";
filter.restore="Obnovi privzete nastavitve";
filter.pol="Smernica";
filter.polname="Ime smernice";
filter.pcs="PCji";
filter.polallow="Dostop do medmre�ja med izbranimi dnevi in urami.";
filter.legend2="Dnevi";
filter.time="�asi";
filter.h24="24 ur";
filter.legend3="Zaprte storitve ";
filter.catchall="Ujemi vse protokole P2P";
filter.legend4="Zapiranje spleti�� z URL naslovom";
filter.legend5="Zapiranje spleti�� s klju�no besedo";

filter.mess1="Zbri�i smernico?";
filter.mess2="Izbrati morate vsaj dan.";
filter.mess3="Kon�ni �as mora biti ve�ji od za�etnega.";

//help container
var hfilter = new Object();
hfilter.right2="Dolo�ite lahko do 10 smernic dostopa. Kliknite <em>" + sbutton.del + "</em>, da zbri�ete smernico, ali <em>" + sbutton.summary + "</em> za povzetek smernice.";
hfilter.right4="Omogo�i ali onemogo�i smernico.";
hfilter.right6="Smernici lahko dolo�ite ime.";
hfilter.right8="Dolo�ite dan v tednu, ko bo smernica uporabljena.";
hfilter.right10="Vnesite �as, ko bo smernica uporabljena.";
hfilter.right12="Zaprete lahko dolo�ene storitve. Kliknite <em>" + sbutton.filterSer + "</em> za spremembo nastavitev.";
hfilter.right14="Zaprete lahko dolo�ena spleti��a tako, da vnesete njihov URL.";
hfilter.right16="Zaprete lahko dolo�ena spleti��a tako, da vnesete klju�no besedo vsebovano v njihovi spletni starni.";



// ******************************************* FilterSummary.asp *******************************************//

var filterSum = new Object();
filterSum.titl=" - Povtetek omejitev dostopa";
filterSum.h2="Povzetek smernice dostopa do medmre�ja";
filterSum.polnum="�t.";
filterSum.polday="�as dneva";



// ******************************************* Firewall.asp *******************************************//

var firewall = new Object();
firewall.titl=" - Po�arni zid";
firewall.h2="Varnost";
firewall.legend="Za��ita z po�arnim zidom";
firewall.firewall="SPI po�arni zid";
firewall.legend2="Dodatni filtri";
firewall.proxy="Filter za proksi";
firewall.cookies="Filter za pi�kotke";
firewall.applet="Filter za Java aplete";
firewall.activex="Filter za ActiveX";
firewall.legend3="Prepre�i WAN zahtevo";
firewall.ping="Prepre�i anonimno zahtevo z medmre�ja";
firewall.muticast="Filter za multicast";
filter.nat="Filter za NAT preusmeritev";
filter.port113="Filter za IDENT (vrata 113)";

//help container
var hfirewall = new Object();
hfirewall.right2="Omogo�i ali onemogo�i za��ito s SPI po�arnim zidom.";



// ******************************************* Forward.asp *******************************************//

var prforward = new Object();
prforward.titl=" - Posredovanje obsega vrat";
prforward.h2="Posredovanje obsega vrat";
prforward.legend="Posredovanje";
prforward.app="Aplikacija";

//help container
var hprforward = new Object();
hprforward.right2="Nekatere aplikacije zahtevajo, da so dolo�ena vrata odprta za njihovo pravilno delovanje. \
	Primeri takih aplikacij so stre�niki in nekatere igre. \
	Ko z medmre�ja pride zahteva za dolo�ena vrata, bo usmerjalnik usmeril podatke do dolo�enega ra�unalnika. \
	Zaradi varnosti omejite posredovanje vrat samo natista vrata, ki ji uporabljate. ";


// ******************************************* ForwardSpec.asp *******************************************//

var pforward = new Object();
pforward.titl=" - Posredovanje vrat";
pforward.h2="Posredovanje vrat";
pforward.legend="Posredovanje";
pforward.app="Aplikacija";
pforward.from="Od vrat";
pforward.to="Do vrat";

//help container
var hpforward = new Object();
pforward.right2="Nekatere aplikacije zahtevajo, da so dolo�ena vrata odprta za njihovo pravilno delovanje. \
	Primeri takih aplikacij so stre�niki in nekatere igre. \
	Ko z medmre�ja pride zahteva za dolo�ena vrata, bo usmerjalnik usmeril podatke do dolo�enega ra�unalnika. \
	Zaradi varnosti omejite posredovanje vrat samo natista vrata, ki ji uporabljate. ";



// ******************************************* Hotspot.asp *******************************************//

var hotspot = new Object();
hotspot.titl=" - Hotspot";
hotspot.h2="Hotspot portal";
hotspot.legend="Chillispot";
hotspot.hotspot="Chillispot";
hotspot.pserver="Glavni Radius Server IP/DNS";
hotspot.bserver="Nadomestni Radius Server IP/DNS";
hotspot.dns="DNS IP";
hotspot.url="Preusmerjeni URL";
hotspot.dhcp="DHCP vmesnik";
hotspot.radnas="Radius NAS ID";
hotspot.uam="UAM skrivnost";
hotspot.uamdns="UAM vsak DNS";
hotspot.allowuam="UAM dopu��eni";
hotspot.macauth="MACauth";
hotspot.option="Dodatne Chillispot mo�nosti";
hotspot.fon_chilli="Chillispot lokalno uporabni�ko upravljanje";
hotspot.fon_user="Seznam uporabnikov";
hotspot.http_legend="HTTP preusmeritev";
hotspot.http_srv="HTTP preusmeritev";
hotspot.http_ip="HTTP ciljni IP";
hotspot.http_port="HTTP ciljna vrata";
hotspot.http_net="HTTP izvorna mre�a";
hotspot.nocat_legend="NoCatSplash";
hotspot.nocat_srv="NoCatSplash";
hotspot.nocat_gateway="Ime prohoda";
hotspot.nocat_home="Dama�a stran";
hotspot.nocat_allowweb="Dovoljeni gostitelji svetovnega spleta";
hotspot.nocat_docroot="Korenski imenik dokumenta";
hotspot.nocat_splash="Splash URL";
hotspot.nocat_port="Izlo�i vrata";
hotspot.nocat_timeout="�asovna omejitev prijave";
hotspot.nocat_verbose="Ob�irnost";
hotspot.nocat_route="Samo usmerjanje";
hotspot.smtp_legend="Preusmeritev SMTP";
hotspot.smtp_srv="Preusmeritev SMTP";
hotspot.smtp_ip="SMTP ciljni IP";
hotspot.smtp_net="SMTP izvorna mre�a";
hotspot.shat_legend="Zero IP nastavitev";
hotspot.shat_srv="Zero IP nastavitev";
hotspot.shat_srv2="Zero IP nastavitev omogo�ena";
hotspot.sputnik_legend="Sputnik";
hotspot.sputnik_srv="Sputnik zastopnik";
hotspot.sputnik_id="Sputnik stre�nikov ID";
hotspot.sputnik_instant="Uporabi Sputnik Instant Setup";
hotspot.sputnik_express="Uporabi SputnikNet Express";
hotspot.sputnik_about="o Sputniku";



// ******************************************* Info.htm *******************************************//

var info = new Object();
info.titl=" - Info";
info.h2="Sistemske informacije";
info.wlanmac="Brez�i�ni MAC";
info.srv="Storitve";
info.ap="Dostopna to�ka";



// ******************************************* index_heartbeat.asp *******************************************//

var idx_h = new Object();
idx_h.srv="Heart Beat stre�nik";
idx_h.con_strgy="Strategija povezovanja";
idx_h.max_idle="Pove�i po potrebi: najve�ji nedejavni �as";
idx_h.alive="Ohrani zvezo: �as ponovnega klicanja";



// ******************************************* index_l2tp.asp *******************************************//

var idx_l = new Object();
idx_l.srv="L2TP stre�nik";



// ******************************************* index_pppoe.asp *******************************************//

var idx_pppoe = new Object();
idx_pppoe.use_rp="Uporabi RP PPPoE";



// ******************************************* index_pptp.asp *******************************************//

var idx_pptp = new Object();
idx_pptp.srv="Uporabi DHCP";
idx_pptp.wan_ip="IP naslov";
idx_pptp.gateway="Prehod (PPTP stre�nik)";
idx_pptp.encrypt="PPTP �ifrirnje";



// ******************************************* index_static.asp *******************************************//

var idx_static = new Object();
idx_static.dns="Stati�ni DNS";



// ******************************************* index.asp *******************************************//

var idx = new Object();
idx.titl=" - Namestitev";
idx.h2="Namestitev medmre�ja";
idx.h22="Namestitev brez�i�nega omre�ja";
idx.legend="Na�in dostopa do medmre�ja";
idx.conn_type="Na�in dostopa";
idx.stp="STP";
idx.stp_mess="(onemogo�i za COMCAST ISP)";
idx.optional="Dodatne nastavitve (potrebno za nekatere ISPje)";
idx.mtu="MTU";
idx.h23="Nastavitev mre�e";
idx.routerip="IP naslov usmerjalnika";
idx.lanip="Lokalni IP naslov";
idx.legend2="WAN vrata";
idx.wantoswitch="dodeli WAN vrata k stikalu";
idx.legend3="Nastavitev �asa";
idx.timeset="�asovna cona / Poletni �as";
idx.localtime="Uporabi krajevni �as";
idx.static_ip="Stati�ni IP";
idx.dhcp="Avtomati�na namestitev - DHCP";
idx.dhcp_legend="Nastavitev stre�nika mre�nih naslovov (DHCP)";
idx.dhcp_type="Tip DHCP";
idx.dhcp_srv="DHCP stre�nik";
idx.dhcp_fwd="DHCP posredovalnik";
idx.dhcp_start="Za�etni IP naslov";
idx.dhcp_end="Kon�ni IP naslov";		//used in Status_Lan.asp
idx.dhcp_maxusers="Najve�je �t. DHCP uporabnikov";
idx.dhcp_lease="Odjemal�ev �as zakupa";
idx.dhcp_dnsmasq="Uporabi DNSMasq za DHCP";
idx.dns_dnsmasq="Uporabi DNSMasq za DNS";
idx.auth_dnsmasq="DHCP-Authoritative";



//help container
var hidx = new Object();
hidx.right2="Ta nastavitev se pogosto uporablja za kabelske ponudnike (ISP).";
hidx.right4="Vnesi uporabni�ko ime dodeljeno s strani va�ega ISP.";
hidx.right6="Vnesi domensko ime dodeljeno s strani va�ega ISP.";
hidx.right8="To je naslov usmerjalnika.";
hidx.right10="To je maska podmre�e usmerjalnika.";
hidx.right12="Dovoli usmerjalniku, da upravlja z va�imi IP naslovi.";
hidx.right14="�eljeni za�etni naslov.";
hidx.right16="Omejite lahko �tevilo naslovov, ki jih dodeli va� usmerjalnik.";
hidx.right18="Izberite va�o �asovno cono in obdobje poletnega �asa. Usmerjalnik lahko uparablja lokalni �as ali �as UTC.";



// ******************************************* Join.asp *******************************************//

var join = new Object();

//sshd.webservices
join.titl=" - Pove�i";
join.mess1="Kot odjemalec ste se uspe�no povezali s slede�o mre�o: ";



// ******************************************* Log_incoming.asp *******************************************//

var log_in = new Object();
log_in.titl=" - Tabela vhodnega dnevnika";
log_in.h2="Tabela vhodnega dnevnika";
log_in.th_ip="Izvorni IP";
log_in.th_port="�t. naslovnih vrat";



// ******************************************* Log_outgoing.asp *******************************************//

var log_out = new Object();
log_out.titl=" - Tabela izhodnega dnevnika";
log_out.h2="Tabela izhodnega dnevnika";
log_out.th_lanip="LAN IP";
log_out.th_wanip="Naslovni URL/IP";
log_out.th_port="Storitev/�t. vrat";



// ******************************************* Log.asp *******************************************//

var log = new Object();
log.titl=" - Dnevnik";
log.h2="Upravljanje dnevnika";
log.legend="Dnevnik";
log.lvl="Nivo dnevnika";
log.drop="Izvr�eni";
log.reject="Zavrnjeni";
log.accept="Sprejeti";



// ******************************************* Management.asp *******************************************//

var management = new Object();
management.titl=" - Administracija";
management.h2="Upravljanje usmerjalnika";

management.psswd_legend="Geslo usmerjalnika";
management.psswd_user="Uporabni�ko ime usmerjalnika";
management.psswd_pass="Geslo usmerjalnika";
management.pass_conf="Ponovno vnesi za potrditev";

management.remote_legend="Oddaljeni dostop";
management.remote_gui="Upravljanje uporabni�kega vmesnika";
management.remote_https="Uporabi HTTPS";
management.remote_guiport="Vrata uporabni�kega vmesnika";
management.remote_ssh="Upravljanje SSH";
management.remote_sshport="SSH vrata";

management.web_legend="Dostop do uporabni�kega vmesnika";
management.web_refresh="Samodejno osve�evanje (v sekundah)";
management.web_sysinfo="Omogo�i Info stran";
management.web_sysinfopass="Info stran za��itena z geslom";
management.web_sysinfomasq="Info stran: skrij MAC naslove";

management.boot_legend="Zagonska pavza";
management.boot_srv="Zagonska pavza";

management.cron_legend="Cron";
management.cron_srvd="Cron";

management.dsn_legend="DNSMasq";
management.dsn_srv="DNSMasq";
management.dsn_loc="Lokalni DNS";
management.dsn_opt="Dodatne DNS mo�nosti";

management.loop_legend="Povratna zanka";
management.loop_srv="Povratna zanka";

management.wifi_legend="802.1x";
management.wifi_srv="802.1x";

management.ntp_legend="NTP odjemalec";
management.ntp_srv="NTP";

management.rst_legend="Gumb za ponastavitev";
management.rst_srv="Gumb za ponastavitev";

management.routing_legend="Usmerjanje";
management.routing_srv="Usmerjanje";

management.wol_legend="Wake-On-LAN";
management.wol_srv="WOL";
management.wol_pass="SecureOn geslo";
management.wol_mac="MAC naslovi<br/>( format: xx:xx:xx:xx:xx:xx )";

management.ipv6_legend="IPv6 podpora";
management.ipv6_srv="IPv6";
management.ipv6_rad="Radvd omogo�en";
management.ipv6_radconf="Radvd nastavitev";

management.jffs_legend="JFFS2 podpora";
management.jffs_srv="JFFS2";
management.jffs_clean="O�isti JFFS2";

management.lang_legend="Izbira jezika";
management.lang_srv="Jezik";
management.lang_bulgarian="bulgar��ina";
management.lang_chinese_traditional="tradicionalna kitaj��ina";
management.lang_chinese_simplified="poenostavljena kitaj��ina";
management.lang_croatian="hrva��ina";
management.lang_czech="�e��ina";
management.lang_dutch="nizozem��ina";
management.lang_portuguese="portugal��ina";
management.lang_english="angle��ina";
management.lang_french="franco��ina";
management.lang_german="nem��ina";
management.lang_italian="italijan��ina";
management.lang_brazilian="braziljska portugal��ina";
management.lang_slovenian="sloven��ina";
management.lang_spanish="�pan��ina";
management.lang_swedish="�ved��ina";

management.net_legend="Nastavitve IP filtra (nastavite za P2P)";
management.net_port="Najve�je �t. vrat";
management.net_tcptimeout="TCP �asovna omejitev (v sekundah)";
management.net_udptimeout="UDP �asovna omejitev (v sekundah)";

management.clock_legend="Overclocking";
management.clock_frq="Frekvenca";
management.clock_support="Ni podprto";

management.mmc_legend="podpora za MMC/SD kartice";
management.mmc_srv="MMC naprava";

management.samba_legend="Samba FS avtomatski priklop";
management.samba_srv="SMB datote�ni sistemm";
management.samba_share="Skupni imenik";
management.samba_stscript="Za�etni skript";

management.SIPatH_srv="SIPatH";
management.SIPatH_port="SIP vrata";
management.SIPatH_domain="SIP domena";

management.gui_style="Stil usmerjalnikovega uporabni�kega vmesnika";



//help container
var hmanagement = new Object();
hmanagement.right1="Samodejno osve�evanje:";
hmanagement.right2="Nastavite interval samodejnega osve�evanja. 0 popolnoma onemogo�i samodejno osve�evanje.";



// ************ Port_Services.asp (used by Filters.asp and QoS.asp, QOSPort_Services.asp not used anymor) *****************************************//

var portserv = new Object();
portserv.titl=" - Storitve vrat";
portserv.h2="Storitve vrat";



// ******************************************* QoS.asp *******************************************//

var qos = new Object();
qos.titl=" - Kakovost storitev (QoS)";
qos.h2="Kakovost storitev (QoS)";
qos.legend="QoS nastavitve";
qos.srv="Za�eni QoS";
qos.type="Paketni na�rt";
qos.uplink="Hitrost prenosa od vas (kbps)";
qos.dnlink="Hitrost prenosa k vam (kbps)";
qos.gaming="Optimiziraj za igre";
qos.legend2="Prioriteta storitev";
qos.prio_x="Exempt";
qos.prio_p="Premium";
qos.prio_e="Express";
qos.prio_s="Standard";
qos.prio_b="Bulk";
qos.legend3="Prioriteta maske omre�ja";
qos.ipmask="IP/maska";
qos.maxrate_b="Maks. Kbits";
qos.maxrate_o="Maks. hitrost";
qos.legend4="MAC prioriteta";
qos.legend5="Prioriteta ethernet vrat";
qos.legend6="Privzeti nivo pasovne �irine";
qos.bandwith="Pasovna �irina v Kbits";

//help container
var hqos = new Object();
hqos.right1="Hitrost prenosa od vas:";
hqos.right2="Nastavite na 80%-95% (maks.) va�e skupnege hitrosti prenosa od vas.";
hqos.right3="Hitrost prenosa k vam:";
hqos.right4="Nastavite na 80%-100% (maks.) va�e skupne hitrosti prenosa k vam.";
hqos.right6="Nadzirate lahko hitrost prenosa glede na aplikacijo, ki uporablja pasovno �irino.";
hqos.right8="Dolo�ite lahko prioriteto za ves promet z dolo�enega IPja ali obsega IPjev.";
hqos.right10="Dolo�ite lahko hitrost za ves promet z dolo�ene naprave na va�i mre�i tako, da dolo�ite ime naprave, \
			dolo�ite prioriteto in vnesete MAC naslov.";
hqos.right12="Nadzirate lahko hitrost za ves promet glede na to na kateri fi�i�ni port je va�a naprava priklju�ena. \
			Dolo�ite lahko prioriteto za naprave priklju�ene na LAN vrata 1 do 4";



// ******************************************* RouteTable.asp *******************************************//

var routetbl = new Object();
routetbl.titl=" - Usmerjalna tabela";
routetbl.h2="Seznam vnosov v usmerjalno tabelo";
routetbl.th1="Ciljni LAN IP";



// ******************************************* Routing.asp *******************************************//

var route = new Object();
route.titl=" - Usmerjanje";
route.h2="Napredno usmerjanje";
route.mod="Na�in";
route.bgp_legend="BGP nastavitve";
route.bgp_ip="Sosednji IP";
route.bgp_as="Sosednji AS#";
route.rip2_mod="RIP2 usmerjalnik";
route.ospf_mod="OSPF usmerjalnik";
route.gateway_legend="Dinami�no usmerjanje";
route.static_legend="Stati�no usmerjanje";
route.static_setno="Izberi �t. niza";
route.static_name="Ime smeri";
route.static_ip="Ciljni LAN IP";

//help container
var hroute = new Object();
hroute.right1="Na�in:";
hroute.right2="�e usmerjalnik gosti va�o povezavo v medmere�je, izberite na�in <em>Prehod</em>. �e na va�i mre�i obstaja kak drug usmerjevalnik, izberite na�in <em>Usmerjevalnik</em>.";
hroute.right3="Izberite �t. niza:";
hroute.right4="To je edinstvena �tevilka, dolo�ite lahko do 20 smeri.";
hroute.right5="Ime smeri:";
hroute.right6="Vnesite ime, ki ga dolo�ite za smer.";
hroute.right7="Ciljni LAN IP:";
hroute.right8="To je oddaljeni gostitelj do katerage dolo�ate stati�no smer.";
hroute.right9="Maska podmre�e:";
hroute.right10="Dolo�i gostitelja in mre�o.";


// ******************************************* Site_Survey.asp *******************************************//

var survey = new Object();
survey.titl=" - Pregled podro�ja";
survey.h2="Sosednja brez�i�na omre�ja";
survey.thjoin="Pove�i";



// ******************************************* Services.asp *******************************************//

var service = new Object();

service.titl=" - Storitve";
service.h2="Upravljanje storitev";

//kaid
service.kaid_legend="XBOX Kaid";
service.kaid_srv="Za�ni Kaid";
service.kaid_mac="MAC konzole: (kon�ati se mora z ;)";

//DHCPd
service.dhcp_legend="DHCP odjemalec";
service.dhcp_vendor="Nastavitev Vendorclass";
service.dhcp_legend2="DHCP stre�nik";
service.dhcp_srv="DHCP demon";
service.dhcp_jffs2="Uporabi JFFS2 za bazo zakupov";
service.dhcp_domain="Uporabljena domena";
service.dhcp_landomain="LAN domena";
service.dhcp_option="Dodatne DHCPd opcije";

//pptp.webservices
service.pptp_legend="PPTP";
service.pptp_srv="PPTP stre�nik";
service.pptp_client="IPji odjemalcev";
service.pptp_chap="CHAP-skrivnosti";

//syslog.webservices
service.syslog_legend="Sistemski dnevnik";
service.syslog_srv="Syslogd";
service.syslog_ip="Oddaljeni stre�nik";

//telnet.webservices
service.telnet_legend="Telnet";
service.telnet_srv="Telnet";

//pptpd_client.webservices
service.pptpd_legend="PPTP odjemalec";
service.pptpd_option="PPTP opcije odjemalca";
service.pptpd_ipdns="Stre�nikov IP ali ime DNS";
service.pptpd_subnet="Oddaljena podmre�a";
service.pptpd_subnetmask="Oddaljena maska podmre�e";
service.pptpd_encry="MPPE �ifriranje";
service.pptpd_mtu="MTU";
service.pptpd_mru="MRU";
service.pptpd_nat="NAT";

//rflow.webservices
service.rflow_legend="RFlow / MACupd";
service.rflow_srv1="RFlow";
service.rflow_srv2="MACupd";

//pppoe-relay.webservices
service.pppoe_legend="PPPOE posredovanje";
service.pppoe_srv="posredovanje";

//snmp.webservices
service.snmp_legend="SNMP";
service.snmp_srv="SNMP";
service.snmp_loc="Lokacija";
service.snmp_contact="Zveza";
service.snmp_name="Ime";
service.snmp_read="RO skupnost";
service.snmp_write="RW skupnost";

//openvpn.webservices
service.vpn_legend="OpenVPN odjemalec";
service.vpn_srv="Za�ni OpenVPN";
service.vpn_ipname="Stre�nikov IP/Ime";
service.vpn_mtu="TUN MTU nastavitev";
service.vpn_mru="TUN MTU dodatno";
service.vpn_mss="TCP MSS";
service.vpn_compress="Uporabi LZO stiskanje";
service.vpn_tunnel="Tunelski protokol";
service.vpn_srvcert="Javno stre�ni�ko potrdilo";
service.vpn_clicert="Javno odjemal�evo potrdilo";
service.vpn_clikey="Zasebni odjemal�ev klju�";

//sshd.webservices
service.ssh_legend="Varna ukazna lupina";
service.ssh_srv="SSHd";
service.ssh_password="Prijava z geslom";
service.ssh_key="Poobla��eni klju�i";



// ******************************************* Sipath.asp + cgi *******************************************//

var sipath = new Object();
sipath.titl=" - SiPath pregled";
sipath.phone_titl=" - Imenik";
sipath.status_titl=" - Status";



// ******************************************* Status_Lan.asp *******************************************//

var status_lan = new Object();
status_lan.titl=" - LAN status";
status_lan.h2="Lokalna mre�a";
status_lan.legend="LAN status";
status_lan.h22="DHCP - Protokol za dinamic�nego konfiguracijo gostitelja";
status_lan.legend2="DHCP status";
status_lan.legend3="DHCP odjemalci";

//help container
var hstatus_lan = new Object();
hstatus_lan.right2="To je usmerjalnikov MAC naslov, kot je viden v va�i lokalni mre�i.";
hstatus_lan.right4="To je usmerjalnikov IP naslov, kot je viden v va�i lokalni mre�i.";
hstatus_lan.right6="To je maska podmre�e, kadar jo usmerjalnik uporablja.";
hstatus_lan.right8="Kadar uporabljate usmerjalnik kot DHCP stre�nik, je to vidno tukaj.";
hstatus_lan.right10="S klikom na MAC naslov boste dobili ime iz Organizationally Unique Identifier of the network interface (IEEE Standards OUI baze podatkov).";



// ******************************************* Status_Router.asp *******************************************//

var status_router = new Object();
status_router.titl=" - Status usmerjalnika";
status_router.h2="Informacije usmerjalnika";
status_router.legend="Sistem";
status_router.sys_model="Model usmerjalnika";
status_router.sys_firmver="Verzija strojne programske opreme";
status_router.sys_time="Trenutni �as";
status_router.sys_up="Neprekinjeno delovanje";
status_router.sys_load="Povpre�na obremenitev";
status_router.legend2="Procesor";
status_router.cpu="Model procesorja";
status_router.clock="Ura procesorja";
status_router.legend3="Spomin";
status_router.mem_tot="Skupaj na voljo";
status_router.mem_free="Prosto";
status_router.mem_used="Uporabljeno";
status_router.mem_buf="Medpomnilniki";
status_router.mem_cached="Predpmnilnik";
status_router.mem_active="Dejavno";
status_router.mem_inactive="Nedejavno";
status_router.legend4="Mre�a";
status_router.net_maxports="IP filter: najve�je �tevilo vrat";
status_router.net_conntrack="Aktivne IP povezave";
status_router.h22="Medmre�je";
status_router.legend5="Na�in povezave";
status_router.www_login="Na�in prijave";
status_router.www_loginstatus="Status prijave";

//help container
var hstatus_router = new Object();
hstatus_router.right2="To je ime usmerjalnika, kot ste ga dolo�ili v zavihku <i>Nemestitev</i>.";
hstatus_router.right4="To je usmerjalnikov MAC naslov, kot ga vidi va� ponudnik medmre�ja.";
hstatus_router.right6="To je usmerjalnikova trenutna strojna programska oprema.";
hstatus_router.right8="To je trenutni �as.";
hstatus_router.right10="To je �as neprekinjenega delovanja.";
hstatus_router.right12="To so povpre�ne obremenitve v zadnji 1, 5 in 15 minutah.";
hstatus_router.right14="To ka�e podatke, potrebne za priklju�itev v medmre�je. \
				Te podatke ste vnesli v zavihku <em>Namestitev</em>. Tu lahko s klikom na <em>Pove�i</em> ali <em>Prekini</em> pove�ete ali prekinete va�o povezavo.";



// ******************************************* Status_SputnikAPD.asp *******************************************//

var status_sputnik = new Object();
status_sputnik.titl=" - Status Sputnik agenta";
status_sputnik.h2="Sputnik&reg; agent&trade;";
status_sputnik.manage="Upravljan od";
status_sputnik.license="SCC �t. licence.";

//help container
var hstatus_sputnik = new Object();
hstatus_sputnik.right1="Status Sputnik agenta";
hstatus_sputnik.right2="Ta zaslon prikazuje status Sputnik agenta.";
hstatus_sputnik.right4="Sputnikov kontrolni center na katerega je ta usmerjalnik povezan.";
hstatus_sputnik.right6="Trenutni status Sputnik agenta.";
hstatus_sputnik.right8="�t. licence va�ega Sputnik kontrolnega centra.";



// ******************************************* Status_Wireless.asp *******************************************//

var status_wireless = new Object();
status_wireless.titl=" - Brez�i�ni status";
status_wireless.h2="Brez�i�no";
status_wireless.legend="Brez�i�ni status";
status_wireless.net="Mre�a";
status_wireless.pptp="PPTP status";
status_wireless.legend2="Info o paketih";
status_wireless.rx="Sprejeti (RX)";
status_wireless.tx="Oddani (TX)";
status_wireless.h22="Brez�i�na vozli��a";
status_wireless.legend3="Brez�i�ni odjemalci";
status_wireless.signal_qual="kakovost signala";
status_wireless.wds="WDS vozli��a";

//help container
var hstatus_wireless = new Object();
hstatus_wireless.right2="To je usmerjalnikov MAC naslov, kot je viden v va�i lokalni brez�i�ni mre�i..";
hstatus_wireless.right4="Tu je prikazan brez�i�ni na�in, kot je izbran v zavihku Brez�i�no (Me�ano, Samo-G, Samo-B ali Onemogo�eno) used by the network.";



// ******************************************* Triggering.asp *******************************************//

var trforward = new Object();
trforward.titl=" - Pro�enje vrat";
trforward.h2="Pro�enje vrat";
trforward.legend="Posredovanja";
trforward.trrange="Obseg pro�enja vrat";
trforward.fwdrange="Obseg posredovanja vrat";
trforward.app="Aplikacija";

//help container
var htrforward = new Object();
htrforward.right2="Vnesite ime aplikacije za pro�enje.";
htrforward.right4="Za vsako aplikacijo vnesite obseg pro�enja vrat. Preverite dokumentacijo za potrebne �tevilke vrat.";
htrforward.right6="Za vsako aplikacijo vnesite obseg posredovanja vrat. Preverite dokumentacijo za potrebne �tevilke vrat.";
htrforward.right8="Vnesite za�etna vrata za obseg pro�enja ali posredovanja vrat.";
htrforward.right10="Vnesite kon�na vrata za obseg pro�enja ali posredovanja vrat.";



// ******************************************* Upgrade.asp *******************************************//

var upgrad = new Object();
upgrad.titl=" - Posodabljanje strojne programske opreme";
upgrad.h2="Upravljanje s strojno programsko opremo";
upgrad.legend="Posodabljanje strojne programske opreme";
upgrad.info1="Po posodobitvi ponastavi na";
upgrad.resetOff="Brez ponastavitve";
upgrad.resetOn="Privzete nastavitve";
upgrad.file="Prosimo, da izberite datoteko za posodobitev";
upgrad.warning="O P O Z O R I L O";
upgrad.mess1="Posodabljanje lahko traja nekaj minut.<br />Ne izlju�ujte usmerjalnika in ne pritiskajte na gumb za ponastavitev!";

//help container
var hupgrad = new Object();
hupgrad.right2="Kliknite na gumb <em>Prebrskaj...</em> za izbiro datoteke za posodobitev.<br /><br /> \
			Kliknite na gumb <em>Posodobi</em> za za�etek posodabljanja. Posodobitve ne smete prekiniti.";



// ******************************************* UPnP.asp *******************************************//

var upnp = new Object();
upnp.titl=" - UPnP";
upnp.h2="Universal Plug and Play (UPnP)";
upnp.legend="Posredovanja";
upnp.legend2="Konfiguracija UPnP";
upnp.serv="UPnP storitev";
upnp.clear="Ob zagonu izbri�i posredovanja";
upnp.url="Po�lji predstavitveni URL";
upnp.msg1="Kliknite za izbris zakupov";
upnp.msg2="Izbri�i vse vnose?";


//help container
var hupnp = new Object();
hupnp.right2="Kliknite na ko� za izbris posameznega vnosa.";
hupnp.right4="Dovoli aplikacijam, da samostojno nastavijo posredovanja.";



// ******************************************* VPN.asp *******************************************//

var vpn = new Object();
vpn.titl=" - VPN";
vpn.h2="Navidezno zasebno omre�je (VPN)";
vpn.legend="VPN prehajanje";
vpn.ipsec="IPSec prehajanje";
vpn.pptp="PPTP prehajanje";
vpn.l2tp="L2TP prehajanje";

//help container
var hvpn = new Object();
hvpn.right1="Izberete lahko IPSec, PPTP in/ali L2TP prehajanje, da se lahko va�e naprave pove�ejo v VPN.";


// ******************************************* Vlan.asp *******************************************//

var vlan = new Object();
vlan.titl=" - Virtualni LAN";
vlan.h2="Virtualno loaklno omre�je (VLAN)";
vlan.legend="VLAN";
vlan.bridge="Dodeli k<br />Mostu";
vlan.tagged="Zna�ka";
vlan.negociate="Samodejno pogajanje";
vlan.aggregation="Zdru�itev zveze<br>na vratih 3 & 4";
vlan.trunk="Povezava";


// ******************************************* WEP.asp *******************************************//

var wep = new Object();
wep.defkey="Privzeti oddajni klju�";
wep.passphrase="Geslo";



// ******************************************* WOL.asp *******************************************//

var wol = new Object();
wol.titl=" - WOL";
wol.h2="Wake-On-LAN";
wol.legend="Razpolo�ljivi gostitelji";
wol.legend2="WOL naslovi";
wol.legend3="Izhod";
wol.legend4="Ro�ni WOL";
wol.enable="Omogo�i WOL?";
wol.add_wol="Dodaj WOL gostitelja";
wol.restore="Obnovi privzete nastavitve";
wol.mac="MAC naslov(i)";
wol.broadcast="Mre�ni broadcast";
wol.udp="UDP vrata";
wol.msg1="Kliknite za odstranitev WOL gostitelja";

//help container
var hwol = new Object();
hwol.right2="Ta stran omogo�a da <em>zbudite</em> na va�i lokalni mre�i (t.j. priklju�ene na va� usmerjalnik).";
hwol.right4="MAC naslove vnesite v formatu xx:xx:xx:xx:xx:xx (t.j. 01:23:45:67:89:AB)";
hwol.right6="IP naslov je ponavadi broadcast naslov za lokalno mre�o.Lahko pa je oddaljeni naslov, �e gostitelj ni priklju�enj na va�o lokalno mre�o."



// ******************************************* WanMAC.asp *******************************************//

var wanmac = new Object();
wanmac.titl=" - Kloniranje MAC naslova";
wanmac.h2="Kloniranje MAC naslova";
wanmac.legend="Kloniranje MAC";
wanmac.wan="Kloniraj WAN MAC";
wanmac.wlan="Kloniraj brez�i�ni MAC";

//help container
var hwanmac = new Object();
hwanmac.right2="nekateri ponudniki medmere�ja zahtevajo, da prijavite va� MAC naslov. \
			�e ne �elite prijaviti novega MAC naslova lahko klonirate naslov, ki je �e prijavljen pri va�em ponudniku medmere�ja.";



// ******************************************* WL_WPATable.asp / WPA.asp / Radius.asp *******************************************//

var wpa = new Object();
wpa.titl=" - Brez�i�na za��ita";
wpa.h2="Brez�i�na za��ita";
wpa.secmode="Na�in za��ite";
wpa.legend="Brez�i�no �ifriranje";
wpa.auth_mode="Mre�no overovljenje";
wpa.psk="WPA predhodno deljeni klju�";
wpa.wpa="WPA";
wpa.radius="Radius";
wpa.gtk_rekey="WPA interval izmenjave klju�ev";
wpa.rekey="Interval izmenjave klju�ev (v sekundaj)";
wpa.radius_ipaddr="RADIUS naslov stre�nika";
wpa.radius_port="RADIUS vrata stre�nika";
wpa.radius_key="RADIUS klju�";
wpa.algorithms="WPA algoritm";
wpa.shared_key="WPA deljeni klju�";
wpa.rekeyInt="Interval izmenjave klju�ev";

//help container
var hwpa = new Object();
hwpa.right1="Varnostni na�in:";
hwpa.right2="Izberete lahko med: Onemogo�eno, WEP, WPA predhodno deljeni klju�, WPA RADIUS, or RADIUS. Vse naprave na va�i mre�i morajo uporabljanti isti na�in.";



// ******************************************* WL_FilterTable.asp *******************************************//

var wl_filter = new Object();
wl_filter.titl=" - Seznam filtra MAC naslovov";
wl_filter.h2="Seznam filtra MAC naslovov";
wl_filter.h3="Vnesi MAC naslov v tem formatu&nbsp;:&nbsp;&nbsp;&nbsp;xx:xx:xx:xx:xx:xx";



// ******************************************* WL_ActiveTable.asp *******************************************//

var wl_active = new Object();
wl_active.titl=" - Seznam MAC aktivnih brez�i�nih odjemalcev";
wl_active.h2="Seznam MAC brez�i�nih odjemalcev";
wl_active.h3="Omogo�i MAC filter";
wl_active.active="Aktivni PCji";
wl_active.inactive="Neaktivni PCji";



// ******************************************* Wireless_WDS.asp *******************************************//

var wds = new Object();
wds.titl=" - WDS";
wds.h2="Brez�i�ni distribucijski sistem (WDS)";
wds.legend="WDS nastavitve";
wds.label="Leni WDS";
wds.label2="WDS podmre�a";
wds.wl_mac="Brez�i�ni MAC";
wds.lazy_default="Privzeto: Onemogo�eno";
wds.nat1="wLAN->WDS";
wds.nat2="WDS->wLAN";
wds.subnet="Podmre�a";
wds.legend2="Dodatne opcije";



// ******************************************* Wireless_radauth.asp *******************************************//

var radius = new Object();
radius.titl=" - Radius";
radius.h2="Remote Authentication Dial-In User Service";
radius.legend="Radius";
radius.label="MAC Radius odjemalec";
radius.label2="MAC format";
radius.label3="Radius stre�nikov IP";
radius.label4="Radius odjemal�eva vrata";
radius.label5="Najve�je �t. neoverovljenih uporabnikov";
radius.label6="Format gesla";
radius.label7="RADIUS deljena skrivnost";
radius.label8="razveljavi Radius, �e stre�nik ni na voljo";
radius.mac="MAC";



// ******************************************* Wireless_MAC.asp *******************************************//

var wl_mac = new Object();
wl_mac.titl=" - MAC filter";
wl_mac.h2="Brez�i�ni filter MAC";
wl_mac.legend="MAC filter";
wl_mac.label="Uporabi filter";
wl_mac.label2="Na�in filtra";
wl_mac.deny="Prepre�i PCjem na seznamu dostop do brez�i�nega omre�ja";
wl_mac.allow="Dovoli PCjem na seznamu dostop do brez�i�nega omre�ja";



// ******************************************* Wireless_Basic.asp *******************************************//

var wl_basic = new Object();
wl_basic.titl=" - Brez�i�no";
wl_basic.h2="Brez�i�no";
wl_basic.legend="Osnovne nastavitve";
wl_basic.label="Brez�i�ni na�in";
wl_basic.label2="Na�in brez�i�ne mre�e";
wl_basic.label3="Ime brez�i�ne mre�e (SSID)";
wl_basic.label4="Brez�i�ni kanal";
wl_basic.label5="Brez�i�no oddajanje SSID";
wl_basic.label6="Obmo�je ob�utljivosti (ACK u�asenje)";
wl_basic.ap="AP";
wl_basic.client="Odjemalec";
wl_basic.clientBridge="Odjemalec-most";
wl_basic.adhoc="Adhoc";
wl_basic.mixed="Me�ano";
wl_basic.b="Samo-B";
wl_basic.g="Samo-G";
wl_basic.sensitivity="Privzeto: 20000 metrov";

//help container
var hwl_basic = new Object();
hwl_basic.right2="�e �elite prepovedati vstop odjemalcem G, izberite <em>Samo-B</em> na�in. �e �elite izklju�iti brez�i�no omre�je, izberite <em>Onemogo�ie</em>.";
hwl_basic.right3="Obmo�je ob�utljivosti: ";
hwl_basic.right4="Prilagodi ACK u�asenje. 0 popolnoma onemogo�i ACK u�asenje.";



// ******************************************* Wireless_Advanced.asp *******************************************//

var wl_adv = new Object();
wl_adv.titl=" - Napredne brez�i�ne nastavitve";
wl_adv.h2="Napredne brez�i�ne nastavitve";
wl_adv.legend="Napredne nastavitve";
wl_adv.legend2="Nastavitve za podporo brez�i�nim multimedijem";
wl_adv.label="Na�in overovljanja";
wl_adv.label2="Osnovna hitrost";
wl_adv.label3="Hitrost oddajanja";
wl_adv.label4="CTS na�in za��ite";
wl_adv.label5="Frame Burst";
wl_adv.label6="Signalni interval";
wl_adv.label7="DTIM interval";
wl_adv.label8="Prag drobitve";
wl_adv.label9="RTS prag";
wl_adv.label10="Najve�je �t. pridru�enih odjemalcev";
wl_adv.label11="AP osamitev";
wl_adv.label12="Oddajna antena";
wl_adv.label13="Sprejemna antena";
wl_adv.label14="Preambula";
wl_adv.reference="Referenca �uma";
wl_adv.label15="Oddajna mo�";
wl_adv.label16="Afterburner";
wl_adv.label17="Brez�i�ni dostop do uporabni�kega vmesnika";
wl_adv.label18="WMM podpora";
wl_adv.label19="Brez potrditve";
wl_adv.table1="EDCA AP parameteri (AP do odjemalca)";
wl_adv.col1="CWmin";
wl_adv.col2="CWmaks";
wl_adv.col3="AIFSN";
wl_adv.col4="TXOP(b)";
wl_adv.col5="TXOP(a/g)";
wl_adv.col6="Upravni�ko prisiljen";
wl_adv.row1="Ozadje";
wl_adv.row2="Najbolj�i trud";
wl_adv.row3="Slika";
wl_adv.row4="Glas";
wl_adv.table2="EDCA STA parametri (odjemalec do AP)";
wl_adv.lng="Dolga"; 					//************* don't use .long ! *************
wl_adv.shrt="Kratka"; 				//************* don't use .short ! **************

//help container
var hwl_adv = new Object();
hwl_adv.right1="Na�in overovljanja:";
hwl_adv.right2="Izberete lahko med Avto ali Deljeni klju�. Overovljanje z deljenim klju�em je bolj varno, toda vse naprave na va�i mre�i morajo podpirati ta na�in.";



// ******************************************* Fail_s.asp / Fail_u_s.asp / Fail.asp *******************************************//

var fail = new Object();
fail.mess1="Vne�ene vrednosti so neveljavne. Prosimo, da poskusite znova.";
fail.mess2="Posodabljanje ni uspelo.";



// ******************************************* Success*.asp / Reboot.asp  *******************************************//

var success = new Object();
success.saved="Nastavitve so shranjene.";
success.restore="Nastavitve so obnovljene.<br/>Ponovni zagon naprave. Prosimo, da po�akate trenutek...";
success.upgrade="Posodabljanje je uspelo.<br/>Ponovni zagon naprave. Prosimo, da po�akate trenutek...";
success.success_noreboot="Nastavitve so uspele.";
success.success_reboot=success.success_noreboot + "<br />Ponovni zagon naprave. Prosimo, da po�akate trenutek...";

success.alert_reset="Vse nastavitve so bile nastavljene na privezete vrednosti.<br /><br />";
success.alert1="Pred ponovnim priklopom, prosimo, da preverite nasledje:";
success.alert2="�e ste spremenili usmerjalnikov IP naslov, morate tudi sprostiti/obnoviti odjemal�eve IP naslove.";
success.alert3="�e ste povezani brez�i�no, se morate ponovno povezati v omre�je, in nato klikniti <em>Nadaljuj</em>.";

// *****************************************************		OLD PAGES 		************************************************************************//
// **************************************************************** DHCPTable.asp **********************************************************************//

var dhcp = new Object();
dhcp.titl=" - DHCP tabela aktivnih IP";
dhcp.h2="DHCP tabela aktivnih IP";
dhcp.server="DHCP stre�nikov IP naslov :";
dhcp.tclient="Odjemal�evo gostiteljevo ime";

var donate = new Object();
donate.mb="Darujete lahko tudi preko Moneybookerjevega ra�una mb@dd-wrt.com";
