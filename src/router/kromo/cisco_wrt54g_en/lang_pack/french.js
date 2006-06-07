//////////////////////////////////////////////////////////////////////////////////////////////
//				French translation DD-WRT V23 SP1 by Botho 17/05/2006						//
//////////////////////////////////////////////////////////////////////////////////////////////


// ******************************************* COMMON SHARE LABEL *******************************************//
var lang_charset = new Object();
lang_charset.set="iso-8859-1";

var donate = new Object();
donate.mb="Vous pouvez �galement faire un don sur le compte Moneybookers : mb@dd-wrt.com";

var share = new Object();
share.firmware="Micrologiciel";
share.time="Heure";
share.interipaddr="Adresse IP Internet";
share.more="Plus...";
share.help="Aide";
share.enable="Activer";
share.enabled="Activ�";
share.disable="D�sactiver";
share.disabled="D�sactiv�";
share.usrname="Nom d\'utilisateur";
share.passwd="Mot de passe";
share.hostname="Nom d\'h�te";
share.domainname="Nom de domaine";
share.wandomainname="Nom de domaine WAN";
share.landomainname="Nom de domaine LAN";
share.statu="Etat";
share.start="D�but";
share.end="Fin";
share.proto="Protocole";
share.ip="Adresse IP";
share.mac="Adresse MAC";
share.none="Aucun";
share.none2="aucune";
share.both="Les deux";
share.del="Supprimer";
share.remove="Enlever";
share.descr="Description";
share.from="De";
share.to="A";
share.about="A propos";
share.everyday="Tous les jours";
share.sun="Dimanche";
share.sun_s="Dim";
share.sun_s1="D";
share.mon="Lundi";
share.mon_s="Lun";
share.mon_s1="L";
share.tue="Mardi";
share.tue_s="Mar";
share.tue_s1="M";
share.wed="Mercredi";
share.wed_s="Mer";
share.wed_s1="M";
share.thu="Jeudi";
share.thu_s="Jeu";
share.thu_s1="J";
share.fri="Vendredi";
share.fri_s="Ven";
share.fri_s1="V";
share.sat="Samedi";
share.sat_s="Sam";
share.sat_s1="S";
share.expires="Expiration";
share.yes="Oui";
share.no="Non";
share.allow="Autoriser";
share.deny="Interdire";
share.range="Plage";
share.use="Utiliser";
share.mins="Min.";
share.secs="Sec.";
share.routername="Nom du routeur";
share.manual="Manuel";
share.port="Port";
share.ssid="SSID";
share.channel="Canal";
share.rssi="Rssi";
share.signal="Signal";
share.noise="Bruit";
share.beacon="beacon";
share.openn="Open";
share.dtim="dtim";
share.rates="Vitesse";
share.low="Faible";
share.medium="Moyen";
share.high="Haut";
share.option="Options";
share.rule="R�gle";
share.lan="LAN";
share.point2point="Point � Point";
share.nat="NAT";
share.subnet="Masque sous-r�seau";
share.unmask="Afficher";
share.deflt="D�faut";
share.all="Tous";
share.auto="Auto";
share.right="Droite";
share.left="Gauche";
share.share_key="Cl� partag�e";
share.inter="Intervalle (secondes)";
share.srv="Nom du service";
share.port_range="Plage de ports";
share.priority="Priorit�";
share.gateway="Passerelle";
share.intrface="Interface";
share.router="Routeur";
share.static_lease="R�servation d\'adresse";
share.srvip="IP Serveur";
share.localdns="DNS Local";
share.minutes="minutes";
share.oui="Recherche OUI";
share.sttic="Statique";
share.connecting="Connection en cours";
share.connect="Connecter";
share.connected="Connect�";
share.disconnect="D�connecter";
share.disconnected="D�connect�";
share.info="Information";
share.state="Etat";
share.mode="Mode";
share.encrypt="Cryptage";
share.key="Cl�";
share.wireless="Sans fil";
share.dhcp="DHCP";
share.styl="Skin";
share.err="erreur";
share.errs="erreurs";
share.meters="m�tres";


var sbutton = new Object();
sbutton.save="Enregistrer";
sbutton.saving="Enregistr�";
sbutton.cmd="Patientez ...";
sbutton.cancel="Annuler";
sbutton.refres="Actualiser";
sbutton.clos="Fermer";
sbutton.del="Supprimer";
sbutton.continu="Continuer";
sbutton.add="Ajouter";
sbutton.remove="Enlever";
sbutton.modify="Modifier";
sbutton.deleted="Supprim�";
sbutton.delall="Supprimer tout";
sbutton.autorefresh="Auto-Actualisation On";
sbutton.backup="Sauvegarder";
sbutton.restore="Restaurer";
sbutton.cptotext="Editer";
sbutton.runcmd="Ex�cutez les commandes";
sbutton.startup="Sauver D�marrage";
sbutton.firewall="Sauver Pare-feu";
sbutton.wol="R�veiller";
sbutton.add_wol="Ajouter l\'h�te";
sbutton.manual_wol="R�veil manuel";
sbutton.summary="R�capitulatif";
sbutton.filterIP="Liste des Ordinateurs";
sbutton.filterMac="Liste de filtrage MAC";
sbutton.filterSer="Ajouter/Editer un Service";
sbutton.reboot="Red�marrer";
sbutton.help="Aide";
sbutton.wl_client_mac="Liste des adresses MAC";
sbutton.update_filter="Liste des filtres";
sbutton.join="Rejoindre";
sbutton.log_in="Connexions entrantes";
sbutton.log_out="Connexions sortantes";
sbutton.apply="Valider";
sbutton.edit_srv="Ajouter/Editer Service";
sbutton.routingtab="Table de routage";
sbutton.wanmac="Obtenir l\'adresse MAC du PC";
sbutton.dhcprel="Lib�rer DHCP";
sbutton.dhcpren="Renouveler DHCP";
sbutton.survey="R�seaux sans fil � port�e";
sbutton.upgrading="Mise � jour ...";
sbutton.upgrade="Mettre � jour";
sbutton.preview="Visualiser";


// ******************************************* COMMON ERROR MESSAGES  *******************************************//
var errmsg = new Object();
errmsg.err0="Vous devez saisir un nom d\'utilisateur.";
errmsg.err1="Vous devez saisir un nom de Routeur.";
errmsg.err2="Hors de l\'intervalle, merci d\'ajuster l\'adresse IP de d�but ou le nombre d\'utilisateurs.";
errmsg.err3="Vous devez selectionner au moins un jour."
errmsg.err4="L\'heure de fin doit �tre sup�rieure � l\'heure de d�part.";
errmsg.err5="La longueur de l\'adresse MAC est incorrecte.";
errmsg.err6="Vous devez saisir un mot de passe.";
errmsg.err7="Vous devez saisir un nom d\'h�te.";
errmsg.err8="Vous devez saisir une adresse IP ou un nom de domaine.";
errmsg.err9="Adresse IP invalide.";
errmsg.err10="La confirmation du mot de passe ne correspond pas. Merci de saisir � nouveau le mot de passe.";
errmsg.err11="Aucun espace n\'est permis dans le mot de passe";
errmsg.err12="Vous devez saisir une commande � ex�cuter.";
errmsg.err13="La mise � jour a �chou�.";
errmsg.err45="Non disponible en HTTPS ! Merci de vous connecter en mode HTTP.";
errmsg.err46="Non disponible en HTTPS.";


//common.js error messages
errmsg.err14=" la valeur est en dehors de l\'intervalle [";
errmsg.err15="L\'adresse MAC WAN est en dehors de l\'intervalle [00 - ff].";
errmsg.err16="Le deuxi�me caract�re de l\'adresse MAC doit �tre un nombre pair : [0, 2, 4, 6, 8, A, C, E].";
errmsg.err17="L\'adresse MAC est incorrecte.";
errmsg.err18="La longueur de l\'adresse MAC est incorrecte.";
errmsg.err19="L\'adresse MAC ne peut pas �tre identique � l\'adresse de diffusion."
errmsg.err20="Sairir l\'adresse MAC au format (xx:xx:xx:xx:xx:xx).";
errmsg.err21="Format d\'adresse MAC invalide.";
errmsg.err22="L\'adresse MAC WAN est incorrecte.";
errmsg.err23="Valeur hexad�cimale invalide ";
errmsg.err24=" trouv�e dans l\'adresse MAC ";
errmsg.err25="La valeur de la cl� est incorrecte.";
errmsg.err26="La longueur de la cl� est incorrecte.";
errmsg.err27="Masque de sous-r�seau incorrect.";
errmsg.err28=" a un caract�re ill�gal, doit �tre [ 0 - 9 ].";
errmsg.err29=" a un code ascii ill�gal.";
errmsg.err30=" a des nombres hexad�cimaux ill�gaux.";
errmsg.err31=" a une valeur ill�gale.";
errmsg.err32="L\'adresse IP et la passerelle n\'utilisent pas le m�me masque de sous r�seau.";
errmsg.err33="L\'adresse IP et la passerelle ne peuvent pas �tre identiques.";
errmsg.err34=" ne doit pas contenir un espace.";

//Wol.asp error messages
errmsg.err35="Vous devez saisir une adresse MAC.";
errmsg.err36="Vous devez saisir une adresse r�seau de diffusion.";
errmsg.err37="Vous devez saisir un port UDP.";

//WL_WPATable.asp error messages
//WPA.asp error messages
errmsg.err38="Merci de saisir une cl� partag�e !";
errmsg.err39="Cl� invalide, doit contenir entre 8 et 63 caract�res ASCII ou 64 nombres hexad�cimaux";
errmsg.err40="Vous devez saisir une cl� pour la Cl� ";
errmsg.err41="Longueur de cl� invalide ";
errmsg.err43="Saisissez de nouveau l\'intervalle.";

//config.asp error messages
errmsg.err42="Merci de s�lectionner une fichier de configuration � restaurer.";

//WL_ActiveTable.asp error messages
errmsg.err44="Le nombre total d\'adresses MAC filtr�es ne peut pas d�passer 128.";

//Site_Survey.asp error messages
errmsg.err47="SSID invalide.";

//Wireless_WDS.asp error messages
errmsg.err48="WDS n\'est pas compatible avec la configuration courante du routeur. \
	Merci de v�rifier les points suivants :\n * Le routeur doit fonctionner en mode AP \n * WPA2 n\'est pas support� sous WDS \n * Le mode sans fil B-Only n\'est pas support� sous WDS";

//Wireless_radauth.asp error messages
errmsg.err49="Radius n\'est seulement diponible qu\'en mode AP.";

//Wireless_Basic.asp error messages
errmsg.err50="Vous devez saisir un SSID.";

// Management.asp error messages
errmsg.err51="Le routeur dispose actuellement de son mot de passe par d�faut. \
	Par mesure de s�curit�, vous devez changer le mot de passe avant de pouvoir activer la fonctionnalit� de gestion distante. \
	Cliquez sur le bouton OK pour changer le mot de passe. Cliquez sur le bouton Annuler pour laisser la fonctionnalit� de gestion distante d�sactiv�e.";
errmsg.err52="Le mot de passe de confirmation ne correspond pas.";

// Port_Services.asp error messages
errmsg.err53="Apr�s avoir termin� toutes les actions, cliquez sur Valider pour enregistrer les modifications.";
errmsg.err54="Vous devez saisir un nom de service.";
errmsg.err55="Ce nom de service existe d�j�.";

// QoS.asp error messages
errmsg.err56="La valeur du port est en dehors de l\'intervalle [0 - 65535].";

// Routing.asp error messages
errmsg.err57="Effacer cette entr�e ?";

// Status_Lan.asp error messages
errmsg.err58="Cliquez pour effacer le bail.";

//Status_Wireless.asp error messages
errmsg.err59="Non disponible ! Merci d\'activer le r�seau sans fil.";

//Upgrade.asp error messages
errmsg.err60="Veuillez s�lectionner le fichier de mise � jour du routeur.";
errmsg.err61="Fichier incorrect.";

//Services.asp error messages
errmsg.err62=" is already defined as a static lease.";



// *******************************************  COMMON MENU ENTRIES  *******************************************//
var bmenu= new Object();
bmenu.setup="Configuration";
bmenu.setupbasic="Param�tres de base";
bmenu.setupddns="DDNS";
bmenu.setupmacclone="Adresse MAC dupliqu�e";
bmenu.setuprouting="Routage avanc�";
bmenu.setupvlan="LAN Virtuel";

bmenu.wireless="Sans fil";
bmenu.wirelessBasic="Param�tres de base";
bmenu.wirelessRadius="Radius";
bmenu.wirelessSecurity="S�curit�";
bmenu.wirelessMac="Filtrage MAC";
bmenu.wirelessAdvanced="Param�tres avanc�s";
bmenu.wirelessWds="WDS";

bmenu.security="S�curit�";
bmenu.firwall="Pare-feu";
bmenu.vpn="VPN";

bmenu.accrestriction="Restrictions d\'acc�s";
bmenu.webaccess="Acc�s Internet";


bmenu.applications="Applications &amp; Jeux";
bmenu.applicationsprforwarding="Redirection plage de port";
bmenu.applicationspforwarding="Redirection de port";
bmenu.applicationsptriggering="D�clenchement de connexion";
bmenu.applicationsUpnp="UPnP";
bmenu.applicationsDMZ="DMZ";
bmenu.applicationsQoS="QoS";

bmenu.sipath="SIPatH";
bmenu.sipathoverview="Vue d\'ensemble";
bmenu.sipathphone="Carnet d\'adresses";
bmenu.sipathstatus="Etat";

bmenu.admin="Administration";
bmenu.adminManagement="Gestion";
bmenu.adminHotspot="Hotspot";
bmenu.adminServices="Services";
bmenu.adminAlive="Keep Alive";
bmenu.adminLog="Journal";
bmenu.adminDiag="Shell";
bmenu.adminWol="WOL";
bmenu.adminFactory="Param�tres usine";
bmenu.adminUpgrade="Mise � niveau";
bmenu.adminBackup="Sauvegarde";


bmenu.statu="Etat";
bmenu.statuRouter="Routeur";
bmenu.statuLAN="LAN";
bmenu.statuSputnik="Agent Sputnik";
bmenu.statuWLAN="Sans fil";
bmenu.statuSysInfo="Syst�me";



// ******************************************* Alive.asp *******************************************//

var alive = new Object();
alive.titl=" - Maintenir Actif";
alive.h2="Maintenir Actif";
alive.legend="Red�marrage Programm� du Routeur";
alive.sevr1="Red�marrage programm�";
alive.hour="A une heure d�finie";
alive.legend2="WDS/Connection Watchdog";
alive.sevr2="Watchdog";
alive.IP="Adresses IP";
alive.legend3="Proxy/Connection Watchdog";
alive.sevr3="Proxy Watchdog";
alive.IP2="Adresse IP du proxy";
alive.port="Port du proxy";

//help container
var halive = new Object();
halive.right2="Choisissez quand le routeur doit red�marrer. <em>Cron</em> doit �tre activ� dans l\'onglet gestion.";
halive.right4="Un maximum de 3 IPs s�par�es par un <em>ESPACE</em> est autoris�.<BR/>Le format des IPs est xxx.xxx.xxx.xxx.";



// ******************************************* config.asp *******************************************//

var config = new Object();
config.titl=" - Sauvegarde & Restauration";
config.h2="Sauvegarde";
config.legend="Sauvergarder la Configuration";
config.mess1="Cliquez sur le bouton <em>\"" + sbutton.backup + "\"</em> pour enregistrer dans un fichier sur votre ordinateur les param�tres de configuration du routeur.";
config.h22="Restauration";
config.legend2="Restaurer les R�glages";
config.mess2="Veuillez s�lectionner un fichier de sauvegarde � restaurer";
config.mess3="A T T E N T I O N";
config.mess4="Ne restaurer que des fichiers sauvegard�s par cette version de micrologiciel.<br />Ne restaurer pas un fichier qui n\'aurait pas �t� cr�� par cette interface !";

//help container
var hconfig = new Object();
hconfig.right2="Vous devriez sauvegarder votre configuration courante au cas o� vous auriez besoin de r�initialiser votre routeur � ses param�tres usine.<br /><br />Cliquez sur le bouton <em>\"" + sbutton.backup + "\"</em> pour enregistrer la configuration actuelle.";
hconfig.right4="Cliquez sur le bouton <em>\"Parcourir...\"</em> pour s�lectionner le fichier de configuration � restaurer.<br /><br />Cliquez sur le bouton <em>\"" + sbutton.restore + "\"</em> pour �craser la configuration courante par celle du fichier de configuration.";



// ******************************************* DDNS.asp *******************************************//

var ddns = new Object();
ddns.titl=" - DNS Dynamique"
ddns.h2="Dynamic Domain Name System (DDNS)";
ddns.legend="DNS Dynamique";
ddns.srv="Service DDNS";
ddns.emailaddr="Adresse E-mail";
ddns.typ="Type";
ddns.dynamic="Dynamique";
ddns.custom="Personnalis�";
ddns.wildcard="Wildcard";
ddns.statu="Etat";

var ddnsm = new Object();
ddnsm.dyn_strange="Erreur inconnue. V�rifier que vous �tes connect� au bon serveur.";
ddnsm.dyn_good="DDNS a �t� mis � jour avec succ�s.";
ddnsm.dyn_noupdate="Aucune mise � jour n�cessaire pour le moment.";
ddnsm.dyn_nohost="Nom d&#39;h�te inconnu.";
ddnsm.dyn_notfqdn="Nom d&#39;h�te incorrect.";
ddnsm.dyn_yours="Ce nom d&#39;h�te ne vous appartient pas.";
ddnsm.dyn_abuse="H�te bloqu� pour abus.";
ddnsm.dyn_nochg="Adresse IP inchang�e depuis la derni�re mise � jour.";
ddnsm.dyn_badauth="L&#39;authentification a �chou� (nom d&#39;utilisateur ou mot de passe incorrect).";
ddnsm.dyn_badsys="Param�tre syst�me incorrect.";
ddnsm.dyn_badagent="Utilisateur bloqu�.";
ddnsm.dyn_numhost="Trop ou pas assez d&#39;h�te trouv�s";
ddnsm.dyn_dnserr="Erreur interne DNS.";
ddnsm.dyn_911="Erreur inattendue 911.";
ddnsm.dyn_999="Erreur inattendue 999.";
ddnsm.dyn_donator="A feature requested is only available to donators, please donate.";
ddnsm.dyn_uncode="Erreur inconnue.";

ddnsm.tzo_good="Mise � jour termin�e avec succ�s.";
ddnsm.tzo_noupdate="Aucune mise � jour n�cessaire pour le moment.";
ddnsm.tzo_notfqdn="Nom d&#39;h�te incorrect.";
ddnsm.tzo_notmail="Adresse E-mail invalide.";
ddnsm.tzo_notact="Action invalide.";
ddnsm.tzo_notkey="Cl� invalide.";
ddnsm.tzo_notip="Adresse IP invalide.";
ddnsm.tzo_dupfqdn="Nom d&#39;h�te incorrect.";
ddnsm.tzo_fqdncre="Ce nom d&#39;h�te ne vous appartient pas.";
ddnsm.tzo_expired="Le compte utilisateur est expir�.";
ddnsm.tzo_error="Erreur inconnue.";

ddnsm.zone_701="Zone is not set up in this account";
ddnsm.zone_702="Echec de la mise � jour.";
ddnsm.zone_703="One of either parameters <em>zones</em> or <em>host</em> are required";
ddnsm.zone_704="Zone must be a valid <em>dotted</em> internet name";
ddnsm.zone_705="Zone cannot be empty";
ddnsm.zone_707="Duplicate updates for the same host/ip, adjust client settings";
ddnsm.zone_201="No records need updating";
ddnsm.zone_badauth="Authorization fails (username or passwords)";
ddnsm.zone_good="ZoneEdit is updated successfully";
ddnsm.zone_strange="Strange server response, are you connecting to the right server ?";

ddnsm.all_closed="Le serveur DDNS est actuellement ferm�.";
ddnsm.all_resolving="R�solution du nom de domaine en cours ...";
ddnsm.all_errresolv="La r�solution du nom de domaine a �chou�.";
ddnsm.all_connecting="Connection en cours...";
ddnsm.all_connectfail="La connection au serveur a �chou�";
ddnsm.all_disabled="DDNS est d�sactiv�";
ddnsm.all_noip="Connection Internet non d�tect�e";

//help container
var hddns = new Object();
hddns.right2="DDNS vous permet d\'attribuer un nom de domaine et d\'h�te fixe � une adresse IP Internet dynamique.\
	Cela peut s\'av�rer utile si vous h�bergez votre propre site Web, un serveur FTP ou tout autre type de serveur \
	derri�re le routeur. <br /><br />Avant d\'opter pour cette fonctionnalit�, vous devez souscrire � un service DDNS aupr�s de fournisseurs sp�cialis�s, \
	tels que DynDNS.org, TZO.com ou ZoneEdit.com.";



// ******************************************* Diagnostics.asp *******************************************//

var diag = new Object();
diag.titl=" - Shell";
diag.h2="Commandes Shell";
diag.legend="Invite de commandes";
diag.cmd="Commande(s)";
diag.startup="D�marrage";
diag.firewall="Pare-feu";

//help container
var hdiag = new Object();
hdiag.right2="Vous pouvez lancer des lignes de commandes gr�ce � la console d\'administration. \
	Saisissez dans la zone de texte les commandes � ex�cuter et cliquez sur le bouton <em>\"" + sbutton.runcmd + "\"</em> pour les soumettre.";



// ******************************************* DMZ.asp *******************************************//

var dmz = new Object();
dmz.titl=" - DMZ";
dmz.h2="Zone d�militaris�e (DMZ)";
dmz.legend="DMZ";
dmz.serv="DMZ";
dmz.host="Adresse IP de l\'h�te de la DMZ";


//help container
var hdmz = new Object();
hdmz.right2="L\'activation de cette option va exposer l\'h�te � Internet. \
	Tous les ports vont �tre accessibles depuis Internet et redirig�s vers cette adresse IP (non recommand�).";



// ******************************************* Factory_Defaults.asp *******************************************//

var factdef = new Object();
factdef.titl=" - Param�tres usine";
factdef.h2="Param�tres usine";
factdef.legend="R�initialiser les r�glages du routeur";
factdef.restore="R�initialiser les param�tres usine";

factdef.mess1="Attention ! Si vous cliquez sur OK, le routeur va r�initialiser les param�tres usine et tous les r�glages seront effac�s.";

//help container
var hfactdef = new Object();
hfactdef.right1="Cliquez sur <em>Oui</em> pour r�tablir les valeurs par d�faut de tous les param�tres de configuration, puis \
	cliquez sur <em>\"" + sbutton.save + "\"</em>. Tous les param�tres enregistr�s pr�c�demment seront \
	perdus une fois les param�tres usine restaur�s. Par d�faut, cette fonctionnalit� est d�sactiv�e.";



// ******************************************* FilterIP%AC.asp *******************************************//

var filterIP = new Object();
filterIP.titl=" - Liste des Ordinateurs";
filterIP.h2="Liste des Ordinateurs";
filterIP.h3="Saisissez l\'adresse MAC des ordinateurs au format: xx:xx:xx:xx:xx:xx";
filterIP.h32="Saisissez l\'adresse IP des ordinateurs";
filterIP.h33="Saisissez l\'intervalle d\'adresses IP des ordinateurs";
filterIP.ip_range="Intervalle IP";



// ******************************************* Filter.asp *******************************************//

var filter = new Object();
filter.titl=" - Restrictions d\'acc�s";
filter.h2="Acc�s Internet";
filter.legend="Strat�gie d\'acc�s � Internet";
filter.pol="Strat�gie";
filter.polname="Nom de la strat�gie";
filter.pcs="Ordinateurs";
filter.polallow="Acc�s Internet pendant les horaires ci-dessous.";
filter.legend2="Calendrier";
filter.time="Plage Horaire";
filter.h24="24 Heures";
filter.legend3="Services Bloqu�s";
filter.catchall="Interdire P2P";
filter.legend4="Blocage de site Web par adresse URL";
filter.legend5="Blocage de site Web par mot cl�";

filter.mess1="Effacer la strat�gie ?";
filter.mess2="Veuillez s�lectionner au moins un jour du calendrier.";
filter.mess3="L\'heure de fin doit �tre post�rieure � l\'heure de d�but.";

//help container
var hfilter = new Object();
hfilter.right2="Vous pouvez contr�ler l\'acc�s � Internet � l\'aide d\'une strat�gie. \
	Pour supprimer une strat�gie, s�lectionnez son num�ro, puis cliquez sur \
	le bouton <em>" + sbutton.del + "</em>. Pour afficher l\'ensemble des strat�gies, cliquez sur le bouton <em>" + sbutton.summary + "</em>."
hfilter.right4="Par d�faut, les strat�gies sont activ�es. Pour activer une strat�gie, s�lectionnez son num�ro dans le menu d�roulant, \
	puis cliquez sur le bouton radio en regard de l\'option <em>Activer</em>.";
hfilter.right6="Saisissez le nom de la strat�gie.";
hfilter.right8="D�finissez les jours pendant lesquels vous souhaitez appliquer cette strat�gie. S�lectionnez \
	individuellement les jours pendant lesquels la strat�gie doit �tre en vigueur ou bien s�lectionnez l\'option <em>Tous les jours</em>.";
hfilter.right10="Saisissez une plage d\'heures et de minutes pendant laquelle la strat�gie sera appliqu�e ou bien s�lectionnez l\'option <em>24 heures</em>.";
hfilter.right12="Vous pouvez filtrer l\'acc�s � divers services accessibles par Internet, notamment FTP ou Telnet, en choisissant ces services \
	dans les menus d�roulants en regard de l\'option <em>Services bloqu�s</em>. Vous pouvez bloquer jusqu\'� 20 services. \
	Saisissez ensuite l\'intervalle des ports � filtrer.";
hfilter.right14="Si vous souhaitez bloquer des sites Web dot�s d\'adresses URL sp�cifiques, saisissez chaque URL dans un \
	champ distinct en regard de la section <em>Blocage de site Web par adresse URL</em>.";
hfilter.right16="Si vous souhaitez bloquer des sites Web � l\'aide de mots cl�s sp�cifiques, saisissez chaque mot cl� dans un \
	champ distinct en regard de la section <em>Blocage de site Web par mot cl�</em>.";



// ******************************************* FilterSummary.asp *******************************************//

var filterSum = new Object();
filterSum.titl=" - R�capitulatif des Restrictions d\'acc�s";
filterSum.h2="R�capitulatif de la Strat�gie d\'acc�s � Internet";
filterSum.polnum="N�.";
filterSum.polday="Plage Horaire";



// ******************************************* Firewall.asp *******************************************//

var firewall = new Object();
firewall.titl=" - Pare-Feu";
firewall.h2="S�curit�";
firewall.legend="Protection par Pare-Feu";
firewall.firewall="Pare-Feu SPI";
firewall.legend2="Filtres Suppl�mentaires";
firewall.proxy="Filtre Proxy";
firewall.cookies="Filtre Cookies";
firewall.applet="Filtre Applets Java";
firewall.activex="Filtre ActiveX";
firewall.legend3="Blocage des requ�tes WAN";
firewall.ping="Bloquer les requ�tes Internet anonymes";
firewall.muticast="Filtre Multidiffusion";
filter.nat="Filtre de redirection NAT Internet";
filter.port113="Filtre IDENT (port 113)";

//help container
var hfirewall = new Object();
hfirewall.right1="Firewall Protection:";
hfirewall.right2="Activez cette fonctionnalit� pour utiliser la technologie SPI (Stateful Packet Inspection) \
		et proc�der � une v�rification plus pouss�e des paquets de donn�es infiltrant votre environnement r�seau.";



// ******************************************* Forward.asp *******************************************//

var prforward = new Object();
prforward.titl=" - Redirection plage de port";
prforward.h2="Redirection plage de port";
prforward.legend="Transfert de connexion";
prforward.app="Application";

//help container
var hprforward = new Object();
hprforward.right2="Certaines applications n�cessitent d\'ouvrir des ports sp�cifiques pour fonctionner correctement. \
	Il peut s\'agir de certains jeux en ligne, les serveurs type FTP, messagerie, web etc ... \
	Certaines applications Internet peuvent n\'exiger aucun transfert. \
	Quand une demande sur un port bien pr�cis venant d\'Internet se pr�sente au niveau du routeur, celui-ci transfert les donn�es vers l\'ordinateur ad�quate. \
	En raison des probl�matiques de s�curit�, vous pouvez activer ou d�sactiver les transferts de connexion quand ils ne sont pas n�cessaires \
	gr�ce � la case � cocher <em>" + share.enable +"</em>.";



// ******************************************* ForwardSpec.asp *******************************************//

var pforward = new Object();
pforward.titl=" - Redirection de port";
pforward.h2="Redirection de port";
pforward.legend="Transfert de connexion";
pforward.app="Application";
pforward.from="du Port";
pforward.to="vers le Port";

//help container
var hpforward = new Object();
hpforward.right2="Certaines applications n�cessitent d\'ouvrir des ports sp�cifiques pour fonctionner correctement. \
	Il peut s\'agir de certains jeux en ligne, les serveurs type FTP, messagerie, web etc ... \
	Certaines applications Internet peuvent n\'exiger aucun transfert. \
	Quand une demande sur un port bien pr�cis venant d\'Internet se pr�sente au niveau du routeur, celui-ci transfert les donn�es vers l\'ordinateur ad�quate. \
	En raison des probl�matiques de s�curit�, vous pouvez activer ou d�sactiver les transferts de connexion quand ils ne sont pas n�cessaires \
	gr�ce � la case � cocher <em>" + share.enable +"</em>.";



// ******************************************* Hotspot.asp *******************************************//

var hotspot = new Object();
hotspot.titl=" - Hotspot";
hotspot.h2="Portail Hotspot";
hotspot.legend="Chillispot";
hotspot.hotspot="Chillispot";
hotspot.nowifibridge="S�parer Wifi du Pont LAN";
hotspot.pserver="IP/DNS Serveur Radius primaire";
hotspot.bserver="IP/DNS Serveur Radius secondaire";
hotspot.dns="IP / DNS";
hotspot.url="URL de redirection";
hotspot.dhcp="Interface DHCP";
hotspot.radnas="Radius NAS ID";
hotspot.uam="Secret UAM";
hotspot.uamdns="UAM Any DNS";
hotspot.allowuam="UAM Autoris�";
hotspot.macauth="MACauth";
hotspot.option="Options additionnelles";
hotspot.fon_chilli="Gestion des Utilisateurs Locaux";
hotspot.fon_user="Liste des Utilisateurs";
hotspot.http_legend="Redirection HTTP";
hotspot.http_srv="Redirection HTTP";
hotspot.http_ip="IP HTTP de destination";
hotspot.http_port="Port HTTP de destination";
hotspot.http_net="R�seau HTTP de d�part";
hotspot.nocat_legend="NoCatSplash";
hotspot.nocat_srv="NoCatSplash";
hotspot.nocat_gateway="Nom de la Passerelle";
hotspot.nocat_home="Page d\'accueil";
hotspot.nocat_allowweb="Sites Web autoris�s";
hotspot.nocat_docroot="Racine fichier";
hotspot.nocat_splash="Splash URL";
hotspot.nocat_port="Ports exclus";
hotspot.nocat_timeout="Login Timeout";
hotspot.nocat_verbose="Verbosit�";
hotspot.nocat_route="Route uniquement";
hotspot.smtp_legend="Redirection SMTP";
hotspot.smtp_srv="Redirection SMTP";
hotspot.smtp_ip="IP SMTP de Destination";
hotspot.smtp_net="R�seau SMTP de d�part";
hotspot.shat_legend="Zero IP Config";
hotspot.shat_srv="Zero IP Config";
hotspot.shat_srv2="Zero IP Config enabled";
hotspot.sputnik_legend="Sputnik";
hotspot.sputnik_srv="Sputnik Agent";
hotspot.sputnik_id="ID Serveur Sputnik";
hotspot.sputnik_instant="Sputnik Instant Setup";
hotspot.sputnik_express="SputnikNet Express";
hotspot.sputnik_about="� propos de Sputnik";



// ******************************************* Info.htm *******************************************//

var info = new Object();
info.titl=" - Information";
info.h2="Information du Syst�me";
info.wlanmac="WLAN MAC";
info.srv="Services";
info.ap="Points d\'Access";


// ******************************************* index_heartbeat.asp *******************************************//

var idx_h = new Object();
idx_h.srv="Serveur Heart Beat";
idx_h.con_strgy="Strat�gie de connexion";
idx_h.max_idle="Connexion � la demande : d�lai d\'inactivit� maximal";
idx_h.alive="Maintenir la connexion : rappel apr�s";



// ******************************************* index_l2tp.asp *******************************************//

var idx_l = new Object();
idx_l.srv="Serveur L2TP";



// ******************************************* index_pppoe.asp *******************************************//

var idx_pppoe = new Object();
idx_pppoe.use_rp="Utiliser RP-PPPoE";



// ******************************************* index_pptp.asp *******************************************//

var idx_pptp = new Object();
idx_pptp.srv="Utiliser DHCP";
idx_pptp.wan_ip="Adresse IP Internet";
idx_pptp.gateway="Passerelle";
idx_pptp.encrypt="Cryptage PPTP";



// ******************************************* index_static.asp *******************************************//

var idx_static = new Object();
idx_static.dns="DNS Statique";



// ******************************************* index.asp *******************************************//

var idx = new Object();
idx.titl=" - Configuration";
idx.h2="Configuration Internet";
idx.h22="Configuration Sans fil";
idx.legend="Type de connexion Internet";
idx.conn_type="Type de connexion";
idx.stp="STP";
idx.stp_mess="(D�sactiver pour le FAI COMCAST)";
idx.optional="Param�tres facultatifs (exig�s par certains FAI)";
idx.mtu="MTU";
idx.h23="Configuration r�seau";
idx.routerip="Adresse IP du routeur";
idx.lanip="Adresse IP";
idx.legend2="Port WAN";
idx.wantoswitch="Ajouter le port WAN au Switch";
idx.legend3="R�glage de l\'heure";
idx.timeset="Fuseau horaire / Heure d\'�t�";
idx.localtime="Utiliser l\'heure locale";
idx.static_ip="Adresse IP statique";
idx.dhcp="Configuration automatique - DHCP";
idx.dhcp_legend="Param�tres du serveur d\'adresse de r�seau (DHCP)";
idx.dhcp_type="Type de DHCP";
idx.dhcp_srv="Serveur DHCP";
idx.dhcp_fwd="Transfert de DHCP";
idx.dhcp_start="Adresse IP de d�but";
idx.dhcp_end="Adresse IP de fin";
idx.dhcp_maxusers="Nombre maximal d\'utilisateurs DHCP";
idx.dhcp_lease="Dur�e de connexion du client";
idx.dhcp_dnsmasq="Utiliser DNSMasq pour DHCP";
idx.dns_dnsmasq="Utiliser DNSMasq pour DNS";
idx.auth_dnsmasq="Autorit� DHCP";




//help container
var hidx = new Object();
hidx.right2="Vous devez conserver cette option uniquement si votre FAI prend en charge le protocole DHCP ou si vous vous connectez via une adresse IP dynamique (cas des c�blo-op�rateurs).";
hidx.right4="Saisissez le nom d\'h�te du routeur qui vous est fourni pour votre FAI. Dans la plupart des cas, vous pourrez laisser ce champ vide.";
hidx.right6="Saisissez le nom de domaine du routeur qui vous est fourni pour votre FAI. Dans la plupart des cas, vous pourrez laisser ce champ vide.";
hidx.right8="Saisissez l\'dresse IP du routeur.";
hidx.right10="Saisissez le masque de sous-r�seau du routeur.";
hidx.right12="Vous pouvez utiliser le routeur en tant que serveur DHCP de votre r�seau. Un serveur DHCP permet d\'attribuer automatiquement une adresse IP � chaque ordinateur de votre r�seau.";
hidx.right14="Saisissez une valeur de d�but pour la publication d\'adresses IP sur le serveur DHCP.";
hidx.right16="Saisissez le nombre maximal d\'ordinateurs auxquels le serveur DHCP doit attribuer des adresses IP. Ce nombre ne peut �tre sup�rieur � 253.";
hidx.right18="Changez le fuseau horaire de votre r�seau � partir du menu d�roulant. Le routeur peut utiliser l\'heure UTC ou l\'heure locale.";



// ******************************************* Join.asp *******************************************//

var join = new Object();

//sshd.webservices
join.titl=" - Rejoindre";
join.mess1="Vous avez rejoint avec succ�s le r�seau sans fil : ";



// ******************************************* Log_incoming.asp *******************************************//

var log_in = new Object();
log_in.titl=" - Journal des connexions entrantes";
log_in.h2="Journal des connexions entrantes";
log_in.th_ip="Adresse IP source";
log_in.th_port="Port de destination";



// ******************************************* Log_outgoing.asp *******************************************//

var log_out = new Object();
log_out.titl=" - Journal des connexions sortantes";
log_out.h2="Journal des connexions sortantes";
log_out.th_lanip="Adresse IP locale";
log_out.th_wanip="URL/IP destination";
log_out.th_port="Service/Port";



// ******************************************* Log.asp *******************************************//

var log = new Object();
log.titl=" - Journal";
log.h2="Gestion du journal";
log.legend="Journal";
log.lvl="Niveau de d�tail";
log.drop="Ignor�";
log.reject="Rejet�";
log.accept="Accept�";



// ******************************************* Management.asp *******************************************//

var management = new Object();
management.titl=" - Administration";
management.h2="Administration du Routeur";

management.psswd_legend="Administrateur";
management.psswd_user="Nom d\'utilisateur";
management.psswd_pass="Mot de passe";
management.pass_conf="Confirmation du mot de passe";

management.remote_legend="Acc�s Distant au Routeur";
management.remote_gui="Acc�s � la console";
management.remote_https="Utilisation de HTTPS";
management.remote_guiport="Port de la console";
management.remote_ssh="Acc�s SSH";
management.remote_sshport="Port SSH";

management.web_legend="Console d\'Administration";
management.web_refresh="Auto-Actualisation (en secondes)";
management.web_sysinfo="Activer page de d�marrage";
management.web_sysinfopass="Page de d�marrage prot�g�e";
management.web_sysinfomasq="Masquer adresses MAC";

management.boot_legend="Temporisation de D�marrage";
management.boot_srv="Boot Wait";

management.cron_legend="Crontab";
management.cron_srvd="Cron";

management.loop_legend="Loopback";
management.loop_srv="Loopback";

management.wifi_legend="802.1x";
management.wifi_srv="802.1x";

management.ntp_legend="Client NTP";
management.ntp_srv="NTP";

management.rst_legend="Bouton d\'initialisation";
management.rst_srv="Bouton d\'initialisation";

management.routing_legend="Routage";
management.routing_srv="Routage";

management.wol_legend="Wake-On-LAN";
management.wol_srv="WOL";
management.wol_pass="mot de passe SecureOn";
management.wol_mac="Adresses MAC<br/>( Format: xx:xx:xx:xx:xx:xx )";

management.ipv6_legend="Support IPv6";
management.ipv6_srv="IPv6";
management.ipv6_rad="Activation Radvd";
management.ipv6_radconf="Configuration Radvd";

management.jffs_legend="Support JFFS2";
management.jffs_srv="JFFS2";
management.jffs_clean="Effacer JFFS2";

management.lang_legend="Langue";
management.lang_srv="Langue";
management.lang_bulgarian="bulgare";
management.lang_chinese_traditional="chinois traditionnel";
management.lang_chinese_simplified="chinois simplifi�";
management.lang_croatian="croate";
management.lang_czech="tch�que";
management.lang_dutch="hollandais";
management.lang_portuguese_braz="Portugais (Br�sil)";
management.lang_english="anglais";
management.lang_french="francais";
management.lang_german="allemand";
management.lang_italian="italien";
management.lang_brazilian="br�silien";
management.lang_slovenian="slov�ne";
management.lang_spanish="espagnol";
management.lang_swedish="su�dois";

management.net_legend="Filtrage IP (modifier pour une utilisation P2P)";
management.net_port="Nombre de connexions maximum";
management.net_tcptimeout="TCP Timeout (en secondes)";
management.net_udptimeout="UDP Timeout (en secondes)";

management.clock_legend="Overclocking";
management.clock_frq="Fr�quence";
management.clock_support="Non support�";

management.mmc_legend="Support de carte MMC/SD";
management.mmc_srv="Dispositif MMC";

management.samba_legend="Syst�me de fichier Samba";
management.samba_srv="Syst�me de fichier Samba";
management.samba_share="Partage";
management.samba_stscript="Script de d�marrage";

management.SIPatH_srv="SIPatH";
management.SIPatH_port="Port SIP";
management.SIPatH_domain="Domaine SIP";

management.gui_style="Skin de la console";


//help container
var hmanagement = new Object();
hmanagement.right1="Auto-Actualisation:";
hmanagement.right2="Saisissez l\'intervalle de rafraichissement automatique de la console d\'administration \
	(seules certaines pages b�n�ficient de cette fonction). La valeur 0 d�sactive cette fonctionnalit� et bascule le rafraichissement \
	en mode manuel.";



// ************ Port_Services.asp (used by Filters.asp and QoS.asp, QOSPort_Services.asp not used anymor) ******************************//

var portserv = new Object();
portserv.titl=" - Gestion des Protocoles";
portserv.h2="Gestion des Protocoles";



// ******************************************* QoS.asp *******************************************//

var qos = new Object();
qos.titl=" - Qualit� de Service";
qos.h2="Qualit� de Service (QoS)";
qos.legend="Configuration QoS";
qos.srv="QoS";
qos.type="Ordonanceur de Packet";
qos.uplink="D�bit montant (kbps)";
qos.dnlink="D�bit descendant (kbps)";
qos.gaming="Optimiser pour le jeu";
qos.legend2="Priorit� des Services";
qos.prio_x="Exempt";
qos.prio_p="Premium";
qos.prio_e="Express";
qos.prio_s="Standart";
qos.prio_b="Bulk";
qos.legend3="Priorit� Sous-r�seau";
qos.ipmask="IP/Masque";
qos.maxrate_b="Max Kbits";
qos.maxrate_o="Max Rate";
qos.legend4="Priorit� MAC";
qos.legend5="Priorit� de Port Ethernet";
qos.legend6="Default Bandwith Level";
qos.bandwith="Bandwith in Kbits";

//help container
var hqos = new Object();
hqos.right1="D�bit montant:";
hqos.right2="Doit �tre entre 80% et 95% (max) du d�bit montant maximum de la connexion.";
hqos.right3="D�bit descendant:";
hqos.right4="Doit �tre entre 80% et 100% du d�bit descendant maximum de la connexion.";
hqos.right6="Vous pouvez contr�ler la priorit� de plusieurs types d'applications pour optimiser la bande passante.";
hqos.right8="Vous pouvez contr�ler la priorit� du trafic pour les adresses IP sp�cifi�es ou une plage d'adresses.";
hqos.right10="Vous pouvez contr�ler la priorit� du trafic en fonction de l\'adresse MAC des clients.";
hqos.right12="Vous pouvez contr�ler la priorit� du trafic en fonction du port ethernet auquel sont connect�s les clients.";



// ******************************************* RouteTable.asp *******************************************//

var routetbl = new Object();
routetbl.titl=" - Table de Routage";
routetbl.h2="Table de Routage";
routetbl.th1="IP Destination sur le LAN";



// ******************************************* Routing.asp *******************************************//

var route = new Object();
route.titl=" - Routage";
route.h2="Routage Avanc�";
route.mod="Mode de Fonctonnement";
route.bgp_legend="Param�tres BGP";
route.bgp_ip="Neighbor IP";
route.bgp_as="Neighbor AS#";
route.rip2_mod="Routeur RIP2";
route.ospf_mod="Routeur OSPF";
route.gateway_legend="Routage Dynamique";
route.static_legend="Routage Statique";
route.static_setno="Choisir l\'itin�raire";
route.static_name="Nom de la Route";
route.static_ip="IP de Destination sur le LAN";

//help container
var hroute = new Object();
hroute.right2="S�lectionnez le mode sous lequel le routeur doit op�rer. \
	Si le routeur concern� h�berge la connexion de votre r�seau � Internet, s�lectionnez l\'option <em>" + share.gateway + "</em>. \
	Si un autre routeur existe sur le r�seau, s�lectionnez <em>Routeur</em>. Si vous choisissez l\'option <em>Routeur</em>, \
	la section <em>" + route.gateway_legend + "</em> est activ�e.";
hroute.right4="Pour d�finir un itin�raire statique entre le routeur et un autre r�seau, \
	choisissez un nombre dans la liste d�roulante de la section <em>" + route.static_legend + "</em>. Un itin�raire statique identifie un \
	chemin pr�-d�fini que les informations du r�seau doivent emprunter pour atteindre un h�te ou un r�seau sp�cifique. \
	Saisissez les informations d�crites ci-dessous pour d�finir un nouvel itin�raire statique. Cliquez sur le bouton <em>" + sbutton.del + "</em> \
	pour supprimer un itin�raire statique.";
hroute.right6="Saisissez dans ce champ le nom de l\'itin�raire.";
hroute.right8="Cette option identifie l\'adresse du r�seau distant, ou h�te, auquel vous souhaitez attribuer un itin�raire statique.";
hroute.right10="Cette option d�termine quelle portion de l\'adresse IP du r�seau local (LAN) \
	de destination correspond au r�seau et quelle portion correspond � l\'h�te.";


// ******************************************* Site_Survey.asp *******************************************//

var survey = new Object();
survey.titl=" - R�seaux Sans Fil � Port�e";
survey.h2="R�seaux Sans Fil � Port�e";
survey.thjoin="Rejoindre le r�seau";



// ******************************************* Services.asp *******************************************//

var service = new Object();

service.titl=" - Services";
service.h2="Gestion des Services";

//kaid
service.kaid_legend="XBOX Kaid";
service.kaid_srv="D�marrer Kaid";
service.kaid_mac="Console Macs: (must end with;)";

//DHCPd
service.dhcp_legend="DHCP Client";
service.dhcp_vendor="Set Vendorclass";
service.dhcp_legend2="DHCP Server";
service.dhcp_srv="DHCP Daemon";
service.dhcp_jffs2="Use JFFS2 for client lease DB";
service.dhcp_domain="Used Domain";
service.dhcp_landomain="LAN Domain";
service.dhcp_option="Additional DHCPd Options";
service.dnsmasq_legend="DNSMasq";
service.dnsmasq_srv="DNSMasq";
service.dnsmasq_loc="DNS Local";
service.dnsmasq_opt="Options DNS";

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
service.pptpd_ipdns="Server IP or DNS Name";
service.pptpd_subnet="Remote Subnet";
service.pptpd_subnetmask="Remote Subnet Mask";
service.pptpd_encry="MPPE Encryption";
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
service.snmp_loc="Location";
service.snmp_contact="Contact";
service.snmp_name="Name";
service.snmp_read="RO Community";
service.snmp_write="RW Community";

//openvpn.webservices
service.vpn_legend="OpenVPN Client";
service.vpn_srv="Start OpenVPN";
service.vpn_ipname="Server IP/Name";
service.vpn_mtu="TUN MTU Setting";
service.vpn_mru="TUN MTU Extra";
service.vpn_mss="TCP MSS";
service.vpn_compress="Use LZO Compression";
service.vpn_tunnel="Tunnel Protocol";
service.vpn_srvcert="Public Server Cert";
service.vpn_clicert="Public Client Cert";
service.vpn_certtype="nsCertType";
service.vpn_clikey="Private Client Key";

//sshd.webservices
service.ssh_legend="Secure Shell";
service.ssh_srv="SSHd";
service.ssh_password="Password Login";
service.ssh_key="Authorized Keys";



// ******************************************* Sipath.asp + cgi *******************************************//

var sipath = new Object();
sipath.titl=" - G�n�ralit�s SiPath";
sipath.phone_titl=" - R�pertoire";
sipath.status_titl=" - Etat";



// ******************************************* Status_Lan.asp *******************************************//

var status_lan = new Object();
status_lan.titl=" - Etat LAN";
status_lan.h2="R�seau Local";
status_lan.legend="Etat LAN";
status_lan.h22="Dynamic Host Configuration Protocol (DHCP)";
status_lan.legend2="Etat DHCP";
status_lan.legend3="Clients DHCP";

//help container
var hstatus_lan = new Object();
hstatus_lan.right2="C\'est l\'adresse MAC de l\'interface Ethernet du routeur.";
hstatus_lan.right4="Adresse IP du routeur sur le r�seau local.";
hstatus_lan.right6="When the Router is using a Subnet Mask, it is shown here.";
hstatus_lan.right8="If you are using the Router as a DHCP server, that will be displayed here.";
hstatus_lan.right10="By clicking on any MAC address, you will obtain the Organizationally Unique Identifier of the network interface (IEEE Standards OUI database search).";



// ******************************************* Status_Router.asp *******************************************//

var status_router = new Object();
status_router.titl=" - Router Status";
status_router.h2="Router Information";
status_router.legend="System";
status_router.sys_model="Router Model";
status_router.sys_firmver="Firmware Version";
status_router.sys_time="Current Time";
status_router.sys_up="Uptime";
status_router.sys_load="Load Average";
status_router.legend2="CPU";
status_router.cpu="CPU Model";
status_router.clock="CPU Clock";
status_router.legend3="Memory";
status_router.mem_tot="Total Available";
status_router.mem_free="Free";
status_router.mem_used="Used";
status_router.mem_buf="Buffers";
status_router.mem_cached="Cached";
status_router.mem_active="Active";
status_router.mem_inactive="Inactive";
status_router.legend4="Network";
status_router.net_maxports="IP Filter Maximum Ports";
status_router.net_conntrack="Active IP Connections";
status_router.h22="Internet";
status_router.legend5="Configuration Type";
status_router.www_login="Login Type";
status_router.www_loginstatus="Login Status";

//help container
var hstatus_router = new Object();
hstatus_router.right2="This is the specific name for the router, which you set on the <i>Setup</i> tab.";
hstatus_router.right4="This is the router\'s MAC Address, as seen by your ISP.";
hstatus_router.right6="This is the router\'s current firmware.";
hstatus_router.right8="This is time received from the ntp server set on the <em>" + bmenu.admin + " | " + bmenu.adminManagement + "</em> tab.";
hstatus_router.right10="This is a measure of the time the router has been \"up\" and running.";
hstatus_router.right12="This is given as three numbers that represent the system load during the last one, five, and fifteen minute periods.";
hstatus_router.right14="This shows the information required by your ISP for connection to the Internet. \
				This information was entered on the Setup Tab. You can <em>Connect</em> or <em>Disconnect</em> your connection here by clicking on that button.";



// ******************************************* Status_SputnikAPD.asp *******************************************//

var status_sputnik = new Object();
status_sputnik.titl=" - Sputnik Agent Status";
status_sputnik.h2="Sputnik&reg; Agent&trade;";
status_sputnik.manage="Managed By";
status_sputnik.license="SCC License No.";

//help container
var hstatus_sputnik = new Object();
hstatus_sputnik.right1="Sputnik Agent Status";
hstatus_sputnik.right2="This screen displays the status of the Sputnik Agent process.";
hstatus_sputnik.right4="The Sputnik Control Center that this access point is connected to.";
hstatus_sputnik.right6="The current Agent status.";
hstatus_sputnik.right8="The license number of your Sputnik Control Center.";



// ******************************************* Status_Wireless.asp *******************************************//

var status_wireless = new Object();
status_wireless.titl=" - Etat Sans Fil";
status_wireless.h2="Sans Fil";
status_wireless.legend="Etat Sans Fil";
status_wireless.net="R�seau";
status_wireless.pptp="Etat PPTP";
status_wireless.legend2="Information Paquets";
status_wireless.rx="Recus (RX)";
status_wireless.tx="Transmis (TX)";
status_wireless.h22="Noeuds Sans fil";
status_wireless.legend3="Clients Sans fil";
status_wireless.signal_qual="Qualit� du Signal";
status_wireless.wds="Noueds WDS";

//help container
var hstatus_wireless = new Object();
hstatus_wireless.right2="This is the Router\'s MAC Address, as seen on your local, wireless network.";
hstatus_wireless.right4="As selected from the Wireless tab, this will display the wireless mode (Mixed, G-Only, or Disabled) used by the network.";



// ******************************************* Triggering.asp *******************************************//

var trforward = new Object();
trforward.titl=" - Port Triggering";
trforward.h2="Port Triggering";
trforward.legend="Forwards";
trforward.trrange="Triggered Port Range";
trforward.fwdrange="Forwarded Port Range";
trforward.app="Application";

//help container
var htrforward = new Object();
htrforward.right2="Enter the application name of the trigger.";
htrforward.right4="For each application, list the triggered port number range. Check with the Internet application documentation for the port number(s) needed.";
htrforward.right6="For each application, list the forwarded port number range. Check with the Internet application documentation for the port number(s) needed.";
htrforward.right8="Enter the starting port number of the Triggered and Forwarded Range.";
htrforward.right10="Enter the ending port number of the Triggered and Forwarded Range.";



// ******************************************* Upgrade.asp *******************************************//

var upgrad = new Object();
upgrad.titl=" - Firmware Upgrade";
upgrad.h2="Firmware Management";
upgrad.legend="Firmware Upgrade";
upgrad.info1="After flashing, reset to";
upgrad.resetOff="No reset";
upgrad.resetOn="Default settings";
upgrad.file="Please select a file to upgrade";
upgrad.warning="W A R N I N G";
upgrad.mess1="Upgrading firmware may take a few minutes.<br />Do not turn off the power or press the reset button !";

//help container
var hupgrad = new Object();
hupgrad.right2="Click on the <em>Browse...</em> button to select the firmware file to be uploaded to the router.<br /><br /> \
			Click the <em>Upgrade</em> button to begin the upgrade process. Upgrade must not be interrupted.";



// ******************************************* UPnP.asp *******************************************//

var upnp = new Object();
upnp.titl=" - UPnP";
upnp.h2="Universal Plug and Play (UPnP)";
upnp.legend="Forwards";
upnp.legend2="UPnP Configuration";
upnp.serv="UPnP Service";
upnp.clear="Clear port forwards at startup";
upnp.url="Send presentation URL";
upnp.msg1="Click to delete lease";
upnp.msg2="Delete all entries?";


//help container
var hupnp = new Object();
hupnp.right2="Click the trash can to delete an individual entry.";
hupnp.right4="Allows applications to automatically setup port forwardings.";



// ******************************************* VPN.asp *******************************************//

var vpn = new Object();
vpn.titl=" - VPN";
vpn.h2="R�seau Priv� Virtuel (VPN)";
vpn.legend="Intercommunication VPN";
vpn.ipsec="Interconnexion IPSec";
vpn.pptp="Interconnexion PPTP";
vpn.l2tp="Interconnexion L2TP";

//help container
var hvpn = new Object();
hvpn.right1="Utilisez les param�tres de cet onglet pour permettre � des tunnels VPN utilisant les protocoles IPSec, PPTP ou L2TP de traverser le pare-feu du routeur.";



// ******************************************* Vlan.asp *******************************************//

var vlan = new Object();
vlan.titl=" - Virtual LAN";
vlan.h2="Virtual Local Area Network (VLAN)";
vlan.legend="VLAN";
vlan.bridge="Assigned To<br />Bridge";
vlan.tagged="Tagged";
vlan.negociate="Auto-Negotiate";
vlan.aggregation="Link Aggregation<br>on Ports 3 & 4";
vlan.trunk="Trunk";


// ******************************************* WEP.asp *******************************************//

var wep = new Object();
wep.defkey="Default Transmit Key";
wep.passphrase="Passphrase";



// ******************************************* WOL.asp *******************************************//

var wol = new Object();
wol.titl=" - WOL";
wol.h2="Wake-On-LAN";
wol.legend="Available Hosts";
wol.legend2="WOL Addresses";
wol.legend3="Output";
wol.legend4="Manual WOL";
wol.enable="Enable WOL?";
wol.add_wol="Add WOL Host";
wol.restore="Restore Factory Defaults";
wol.mac="MAC Address(es)";
wol.broadcast="Net Broadcast";
wol.udp="UDP Port";
wol.msg1="Click to remove WOL host";

//help container
var hwol = new Object();
hwol.right2="This page allows you to <em>Wake Up</em> hosts on your local network (i.e. locally connected to your router).";
hwol.right4="MAC Addresses are entered in the format xx:xx:xx:xx:xx:xx (i.e. 01:23:45:67:89:AB)";
hwol.right6="IP Address is typically the broadcast address for the local network, but could be a remote address if the target host is not connected to the router\'s local network."



// ******************************************* WanMAC.asp *******************************************//

var wanmac = new Object();
wanmac.titl=" - MAC Address Clone";
wanmac.h2="MAC Address Clone";
wanmac.legend="MAC Clone";
wanmac.wan="Clone WAN MAC";
wanmac.wlan="Clone Wireless MAC";

//help container
var hwanmac = new Object();
hwanmac.right2="Some ISP will require you to register your MAC address. \
			If you do not wish to re-register your MAC address, you can have the router clone the MAC address that is registered with your ISP.";



// ******************************************* WL_WPATable.asp / WPA.asp / Radius.asp *******************************************//

var wpa = new Object();
wpa.titl=" - Wireless Security";
wpa.h2="Wireless Security";
wpa.secmode="Security Mode";
wpa.legend="Wireless Encryption";
wpa.auth_mode="Network Authentication";
wpa.psk="WPA Pre-Shared Key";
wpa.wpa="WPA";
wpa.radius="Radius";
wpa.gtk_rekey="WPA Group Rekey Interval";
wpa.rekey="Key Renewal Interval (in seconds)";
wpa.radius_ipaddr="RADIUS Server Address";
wpa.radius_port="RADIUS Server Port";
wpa.radius_key="RADIUS Key";
wpa.algorithms="WPA Algorithms";
wpa.shared_key="WPA Shared Key";
wpa.rekeyInt="rekey interval";

//help container
var hwpa = new Object();
hwpa.right1="Security Mode:";
hwpa.right2="You may choose from Disable, WEP, WPA Pre-Shared Key, WPA RADIUS, or RADIUS. All devices on your network must use the same security mode.";



// ******************************************* WL_FilterTable.asp *******************************************//

var wl_filter = new Object();
wl_filter.titl=" - MAC Address Filter List";
wl_filter.h2="MAC Address Filter List";
wl_filter.h3="Enter MAC Address in this format&nbsp;:&nbsp;&nbsp;&nbsp;xx:xx:xx:xx:xx:xx";



// ******************************************* WL_ActiveTable.asp *******************************************//

var wl_active = new Object();
wl_active.titl=" - Wireless Active Client MAC List";
wl_active.h2="Wireless Client MAC List";
wl_active.h3="Enable MAC Filter";
wl_active.active="Active PC";
wl_active.inactive="Inactive PC";



// ******************************************* Wireless_WDS.asp *******************************************//

var wds = new Object();
wds.titl=" - WDS";
wds.h2="Wireless Distribution System";
wds.legend="WDS Settings";
wds.label="Lazy WDS";
wds.label2="WDS Subnet";
wds.wl_mac="Wireless MAC";
wds.lazy_default="Default: Disable";
wds.nat1="wLAN->WDS";
wds.nat2="WDS->wLAN";
wds.subnet="Subnet";
wds.legend2="Extra Options";



// ******************************************* Wireless_radauth.asp *******************************************//

var radius = new Object();
radius.titl=" - Radius";
radius.h2="Remote Authentication Dial-In User Service";
radius.legend="Radius";
radius.label="MAC Radius Client";
radius.label2="MAC Format";
radius.label3="Radius Server IP";
radius.label4="Radius Server Port";
radius.label5="Maximum Unauthenticated Users";
radius.label6="Password Format";
radius.label7="RADIUS Shared Secret";
radius.label8="Override Radius if Server is unavailable";
radius.mac="MAC";



// ******************************************* Wireless_MAC.asp *******************************************//

var wl_mac = new Object();
wl_mac.titl=" - MAC Filter";
wl_mac.h2="Wireless MAC Filter";
wl_mac.legend="MAC Filter";
wl_mac.label="Use Filter";
wl_mac.label2="Filter Mode";
wl_mac.deny="Prevent PCs listed from accessing the wireless network";
wl_mac.allow="Permit only PCs listed to access the wireless network";



// ******************************************* Wireless_Basic.asp *******************************************//

var wl_basic = new Object();
wl_basic.titl=" - Sans Fil";
wl_basic.h2="Sans Fil";
wl_basic.legend="Param�tres de base";
wl_basic.label="Mode sans fil";
wl_basic.label2="Mode r�seau sans fil";
wl_basic.label3="Nom du r�seau sans fil (SSID)";
wl_basic.label4="Canal sans fil";
wl_basic.label5="Diffusion SSID sans fil";
wl_basic.label6="Sensibilit� (ACK Timing)";
wl_basic.ap="AP";
wl_basic.client="Client";
wl_basic.clientBridge="Pont Client";
wl_basic.adhoc="Adhoc";
wl_basic.mixed="Mixte";
wl_basic.b="B-uniquement";
wl_basic.g="G-uniquement";

//help container
var hwl_basic = new Object();
hwl_basic.right2="Si vous disposez � la fois des p�riph�riques 802.11g et 802.11b sur votre r�seau, \
	conservez le param�tre par d�faut <em>Mixte</em>. Si vous utilisez uniquement des p�riph�riques 802.11g, \
	s�lectionnez <em>G-uniquement</em>. Si vous travaillez uniquement avec des p�riph�riques 802.11b, \
	s�lectionnez <em>B-uniquement</em>. Si votre r�seau n\'est muni d\'aucun p�riph�rique sans fil, s�lectionnez l\'option <em>D�sactiver</em>.";
hwl_basic.right3="Sensibilit�: ";
hwl_basic.right4="Permet de r�gler le <em>ack timing</em>. 0 d�sactive compl�tement cette fonctionnalit�.";



// ******************************************* Wireless_Advanced.asp *******************************************//

var wl_adv = new Object();
wl_adv.titl=" - Param�tres sans fil avanc�s";
wl_adv.h2="Param�tres sans fil avanc�s";
wl_adv.legend="Param�tres Avanc�s";
wl_adv.legend2="Param�tres Sans Fil Multimedia (WMM)";
wl_adv.label="Type d\'authentification";
wl_adv.label2="Taux de base";
wl_adv.label3="Taux de transmission";
wl_adv.label4="Mode de protection CTS";
wl_adv.label5="Rafale de trames";
wl_adv.label6="Intervalle de transmission de balise";
wl_adv.label7="Intervalle DTIM";
wl_adv.label8="Seuil de fragmentation";
wl_adv.label9="Seuil RTS";
wl_adv.label10="Clients sans fil max.";
wl_adv.label11="Mode isolation AP";
wl_adv.label12="Antenne TX";
wl_adv.label13="Antenne RX";
wl_adv.label14="Pr�ambule";
wl_adv.reference="R�f�rence de bruit";
wl_adv.label15="Puissance Xmit";
wl_adv.label16="SpeedBooster";
wl_adv.label17="Acc�s � la console an sans fil";
wl_adv.label18="Support de WMM";
wl_adv.label19="No-Acknowledgement";
wl_adv.table1="Param�tres EDCA AP (AP vers Client)";
wl_adv.col1="CWmin";
wl_adv.col2="CWmax";
wl_adv.col3="AIFSN";
wl_adv.col4="TXOP(b)";
wl_adv.col5="TXOP(a/g)";
wl_adv.col6="Admin. Forc�";
wl_adv.row1="Arri�re plan";
wl_adv.row2="Meilleurs effort";
wl_adv.row3="Vid�o";
wl_adv.row4="Voix";
wl_adv.table2="Param�tres EDCA STA (Client vers AP)";
wl_adv.lng="Long";
wl_adv.shrt="Court";

//help container
var hwl_adv = new Object();
hwl_adv.right2="La valeur <em>Auto</em> d�finie par d�faut vous permet de choisir entre une authentification <em>Syst�me ouvert</em> ou <em>Cl� partag�e</em>. \
	En mode d\'authentification <em>Syst�me ouvert</em>, l\'exp�diteur et le destinataire n\'utilisent pas de cl� WEP pour l\'authentification. \
	En revanche, ils utilisent ce type de cl� en mode <em>Cl� partag�e</em>.";



// ******************************************* Fail_s.asp / Fail_u_s.asp / Fail.asp *******************************************//

var fail = new Object();
fail.mess1="Les param�tres sont incorrects. Veuillez essayer de nouveau.";
fail.mess2="La mise � jour a �chou�.";



// ******************************************* Success*.asp / Reboot.asp  *******************************************//

var success = new Object();
success.saved="Les param�tres ont �t� enregistr�s avec succ�s.";
success.restore="Les param�tres ont �t� restaur�s avec succ�s.<br/>Le routeur red�marre. Patientez un moment SVP ...";
success.upgrade="Mise � jour r�ussie.<br/>Le routeur red�marre. Patientez un moment SVP ...";
success.success_noreboot="Les param�tres ont �t� enregistr�s avec succ�s.";
success.success_reboot=success.success_noreboot + "<br />Le routeur red�marre. Patientez un moment SVP ...";

success.alert_reset="Tous les param�tres ont �t� initialis�s � leur valeur par d�faut.<br /><br />";
success.alert1="Veuillez v�rifier les points suivants avant de vous connecter de nouveau :";
success.alert2="Si l\'adresse IP du routeur a chang�, veuillez SVP renouveler les adresses IP de tous les clients connect�s.";
success.alert3="Si vous �tes connect� au routeur par une connexion sans fil, veuillez SVP vous connecter de nouveau avant de cliquer sur le bouton <em>Continuer</em>.";
