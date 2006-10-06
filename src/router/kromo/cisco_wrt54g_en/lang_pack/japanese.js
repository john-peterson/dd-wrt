// Not working in IE & Opera ?
//************ Include the current language file after english.js ************//
//  var Head = document.getElementsByTagName('head')[0];
//  var head_inc = document.createElement('script');
//  head_inc.setAttribute('type','text/javascript');
//  head_inc.setAttribute('src',"lang_pack/language.js");
//  Head.appendChild(head_inc);

//////////////////////////////////////////////////////////////////////////////////////////////
//      English reference translation file - DD-WRT V23 SP1 by Botho 17/05/2006             //
//////////////////////////////////////////////////////////////////////////////////////////////


// ******************************************* COMMON SHARE LABEL *******************************************//
var lang_charset = new Object();
lang_charset.set="EUC-JP";

var share = new Object();
share.firmware="�t�@�[���E�F�A";
share.time="����";
share.interipaddr="�C���^�[�l�b�gIP�A�h���X";
share.more="������...";
share.help="�w���v";
share.enable="�L���ɂ���";
share.enabled="�L��";
share.disable="�����ɂ���";
share.disabled="����";
share.usrname="���[�U�[��";
share.passwd="�p�X���[�h";
share.hostname="�z�X�g��";
share.domainname="�h���C����";
share.wandomainname="WAN�h���C����";
share.landomainname="LAN�h���C����";
share.statu="�X�e�[�^�X";
share.start="�X�^�[�g";
share.end="�G���h";
share.proto="�v���g�R��";
share.ip="IP�A�h���X";
share.mac="MAC�A�h���X";
share.none="�Ȃ�";
share.none2="no";
share.both="����";
share.del="�폜";
share.remove="����";
share.descr="����";
share.from="From";
share.to="To";
share.about="About";
share.everyday="����";
share.sun="���j��";
share.sun_s="��";
share.sun_s1="S";
share.mon="���j��";
share.mon_s="��";
share.mon_s1="M";
share.tue="�Ηj��";
share.tue_s="��";
share.tue_s1="T";
share.wed="���j��";
share.wed_s="��";
share.wed_s1="W";
share.thu="�ؗj��";
share.thu_s="��";
share.thu_s1="T";
share.fri="���j��";
share.fri_s="��";
share.fri_s1="F";
share.sat="�y�j��";
share.sat_s="�y";
share.sat_s1="S";
share.expires="�L������";
share.yes="�C�G�X";
share.no="�m�[";
share.allow="����";
share.deny="����";
share.range="�����W";
share.use="Use";
share.mins="��.";
share.secs="�b.";
share.routername="���[�^�[��";
share.manual="�}�j���A��";
share.port="�|�[�g";
share.ssid="SSID";
share.channel="�`�����l��";
share.rssi="Rssi";
share.signal="�V�O�i��";
share.noise="�m�C�Y";
share.beacon="�r�[�R��";
share.openn="�J��";
share.dtim="dtim";
share.rates="Rate";
share.low="��";
share.medium="��";
share.high="��";
share.option="�I�v�V����";
share.rule="���[��";
share.lan="LAN";
share.point2point="Point to Point";
share.nat="NAT";
share.subnet="�T�u�l�b�g�}�X�N";
share.unmask="Unmask";
share.deflt="�f�B�t�H���g";  //don't use share.default !!!
share.all="�S";
share.auto="����";
share.right="�E";
share.left="��";
share.share_key="Shared Key";
share.inter="�Ԋu (�b)";
share.srv="�T�[�r�X��";
share.port_range="�|�[�g�͈�";
share.priority="�v���C�I���e�B";
share.gateway="�Q�[�g�E�F�C";
share.intrface="�C���^�[�t�F�[�X";  //don't use share.interface, Mozilla problem!!!
share.router="���[�^�[";
share.static_lease="Static Leases";
share.srvip="�T�[�o�[IP";
share.localdns="���[�J��DNS";
share.minutes="minutes";
share.oui="OUI����";
share.sttic="Static";
share.connecting="�ڑ���";
share.connect="�ڑ�";
share.connected="�ڑ����܂���";
share.disconnect="�ؒf";
share.disconnected="�ؒf���܂���";
share.info="�C���t�H�[���[�V����";
share.state="���";
share.mode="���[�h";
share.encrypt="�Í���";
share.key="�L�[";
share.wireless="���C�A���X";
share.dhcp="DHCP";
share.styl="�X�^�C��";
share.err="�G���[";
share.errs="�G���[";
share.meters="���[�^�[";



var sbutton = new Object();
sbutton.save="�ݒ�̕ۑ�";
sbutton.saving="�ۑ����܂���";
sbutton.cmd="���s��";
sbutton.cancel="�ύX�̃L�����Z��";
sbutton.refres="���t���b�V��";
sbutton.clos="����";
sbutton.del="�폜";
sbutton.continu="���s";
sbutton.add="�o�^";
sbutton.remove="����";
sbutton.modify="�ύX";
sbutton.deleted="�폜����܂���";
sbutton.delall="�S�폜";
sbutton.autorefresh="�����X�V��On�ł�";
sbutton.backup="�o�b�N�A�b�v";
sbutton.restore="����";
sbutton.cptotext="�ҏW";
sbutton.runcmd="�R�}���h�̎��s";
sbutton.startup="Save Startup";
sbutton.firewall="�t�@�C�A�E�H�[���ۑ�";
sbutton.wol="�N��";
sbutton.add_wol="�z�X�g�ǉ�";
sbutton.manual_wol="�}�j���A���N��";
sbutton.summary="�܂Ƃ�";
sbutton.filterIP="PC���X�g�̕ҏW";
sbutton.filterMac="MAC�t�B���^�[���X�g�̕ҏW";
sbutton.filterSer="�T�[�r�X�̒ǉ��ƕҏW";
sbutton.reboot="���{�b�g���[�^�[";
sbutton.help="   �w���v  ";
sbutton.wl_client_mac="���C�A���XMAC�N���C�A���g���X�g";
sbutton.update_filter="�t�B���^�[���X�g�̍X�V";
sbutton.join="Join";
sbutton.log_in="Incoming Log";
sbutton.log_out="Outgoing Log";
sbutton.apply="�K�p";
sbutton.edit_srv="�T�[�r�X�̒ǉ��ƕҏW";
sbutton.routingtab="���[�e�B���O�e�[�u���̕\��";
sbutton.wanmac="��PC��MAC�A�h���X";
sbutton.dhcprel="DHCP�����[�X";
sbutton.dhcpren="DHCP�X�V";
sbutton.survey="�T�C�g�̒���";
sbutton.upgrading="�A�b�v�O���[�h��";
sbutton.upgrade="�A�b�v�O���[�h";
sbutton.preview="�v���r���[";


// ******************************************* COMMON ERROR MESSAGES  *******************************************//
var errmsg = new Object();
errmsg.err0="���[�U�[������͂���Ă��܂���B";
errmsg.err1="���^�[�������͂���Ă��܂���B";
errmsg.err2="�͈͊O�ł��B�X�^�[�gIP�A�h���X�A���邢�̓��[�U�[�ԍ����C�����Ă��������B";
errmsg.err3="�Œ����ȏ�I������K�v������܂��B"
errmsg.err4="�I�����Ԃ͊J�n���Ԃ��x���K�v������܂��B";
errmsg.err5="MAC�A�h���X�������قȂ�܂��B";
errmsg.err6="�p�X���[�h����͂��Ă��������B";
errmsg.err7="�z�X�g������͂��Ă��������B";
errmsg.err8="IP�A�h���X���h���C��������͂��Ă��������B";
errmsg.err9="�s����DMZ��IP�A�h���X�ł��B";
errmsg.err10="�m�F�p�X���[�h���قȂ��Ă��܂��B������x�p�X���[�h����͂��Ă��������B";
errmsg.err11="�p�X���[�h�ɋ󔒂������Ă��܂��B";
errmsg.err12="���s����R�}���h����͂��Ă�������";
errmsg.err13="�X�V�Ɏ��s���܂����B";
errmsg.err45="HTTPS�ɂ��ڑ��͂ł��܂���BHTTP���[�h�Őڑ����Ă�������";
errmsg.err46="HTTPS�ɂ��ڑ��͂ł��܂���B";


//common.js error messages
errmsg.err14=" �l���͈͊O�ł� [";
errmsg.err15="WAN����MAC�A�h���X���͈͊O�ł� [00 - ff].";
errmsg.err16="MAC��2�����ڂ͋����ł� : [0, 2, 4, 6, 8, A, C, E].";
errmsg.err17="MAC�A�h���X�ɖ�肪����܂��B";
errmsg.err18="MAC�A�h���X���ɖ�肪����܂��B";
errmsg.err19="MAC�A�h���X�ɂ̓u���[�h�L���X�g�A�h���X�͎g���܂���B"
errmsg.err20="MAC�A�h���X��(xx:xx:xx:xx:xx:xx)�̃t�H�[�}�b�g�œ��͂��Ă��������B";
errmsg.err21="MAC�A�h���X�t�H�[�}�b�g�ɖ�肪����܂��B";
errmsg.err22="WAN����MAC�A�h���X�ɖ�肪����܂��B";
errmsg.err23="HEX�l�ł͂���܂���";
errmsg.err24=" found in MAC address ";
errmsg.err25="�L�[�l�ɖ�肪����܂��B";
errmsg.err26="�L�[�����ɖ�肪����܂��B";
errmsg.err27="�s���ȃT�u�l�b�g�}�X�N�ł��B";
errmsg.err28=" �s���ȕ����ł��B[ 0 - 9 ]�œ��͂��Ă��������B";
errmsg.err29=" �s����ASCII�R�[�h�ł��B";
errmsg.err30=" �s����16�i���ł��B";
errmsg.err31=" �s���Ȓl�ł��B";
errmsg.err32="IP�A�h���X�ƃQ�[�g�E�F�C���Ⴄ�T�u�l�b�g�}�X�N�ɂ���܂��B";
errmsg.err33="IP�A�h���X�ƃQ�[�g�E�F�C�͓����l�ɂ͂ł��܂���B";
errmsg.err34="�X�y�[�X�i�󔒁j���܂ނ��Ƃ͂ł��܂���B";

//Wol.asp error messages
errmsg.err35="���s����ɂ�MAC�A�h���X��o�^���Ă��������B";
errmsg.err36="���s����ɂ̓l�b�g���[�N�u���[�h�L���X�g�A�h���X��o�^���Ă��������B";
errmsg.err37="���s����ɂ�UDP�|�[�g��o�^���Ă��������B";

//WL_WPATable.asp error messages
//WPA.asp error messages
errmsg.err38="���L�L�[����͂��Ă��������I";
errmsg.err39="�s���ȃL�[�ł��B�L�[��8�`63������ASCII�A���邢��64����16�i���ł��B";
errmsg.err40="��You have to enter a key for Key ";
errmsg.err41="�s���L�[�� ";
errmsg.err43="Rekey interval";

//config.asp error messages
errmsg.err42="��������ݒ�t�@�C����I��ł��������B";

//WL_ActiveTable.asp error messages
errmsg.err44="The total checks exceed 128 counts.";

//Site_Survey.asp error messages
errmsg.err47="�s����SSID�ł��B";

//Wireless_WDS.asp error messages
errmsg.err48="WDS�͌��݂̃��[�^�[�ݒ�ƌ݊���������܂���B�ȉ��̓_�ɂ��ă`�F�b�N���Ă��������B\n�@���@���C�A���X���[�h��AP�ɐݒ肳��Ă���B\n�@���@WPA2��WDS�ŃT�|�[�g����Ă���B\n�@���@���C�A���X�l�b�g���[�N��B-Only���[�h��WDS�ŃT�|�[�g����Ă��Ȃ��B";

//Wireless_radauth.asp error messages
errmsg.err49="Radius��AP���[�h�݂̂ł��B";

//Wireless_Basic.asp error messages
errmsg.err50="SSID����͂���K�v������܂��B";

// Management.asp error messages
errmsg.err51="���[�^�[�̃p�X���[�h�̓f�t�H���g�l�̂܂܂ł��B�Z�L�����e�B�ی�̂��߁A�����[�g�Ǘ����g�p����O�Ƀp�X���[�h��ύX����K�v������܂��B�n�j�{�^���������ƃp�X���[�h�̕ύX���ł��܂��B�L�����Z���{�^���������ƃ����[�g�Ǘ��͎d�l�s�̂܂܂ɂȂ�܂��B";
errmsg.err52="�m�F�p�̃p�X���[�h���قȂ�܂�";

// Port_Services.asp error messages
errmsg.err53="�S�Ă̑��삪�I�������AApply�{�^���������Đݒ��ۑ����܂��B";
errmsg.err54="�T�[�r�X������͂��Ă��������B";
errmsg.err55="�T�[�r�X���͂��łɑ��݂��܂��B";

// QoS.asp error messages
errmsg.err56="�|�[�g�l�͎��͈̔͂Ŏw�肵�Ă�������[0 - 65535]�B";

// Routing.asp error messages
errmsg.err57="�G���g�����폜���܂����H";

// Status_Lan.asp error messages
errmsg.err58="�N���b�N���ă��[�X���������Ă�������";

//Status_Wireless.asp error messages
errmsg.err59="���p�ł��܂���I���C�A���X�l�b�g���[�N���m�F���Ă��������B";

//Upgrade.asp error messages
errmsg.err60="�A�b�v�O���[�h����t�@�C����I�����Ă��������B";
errmsg.err61="�s���ȃC���[�W�t�@�C���ł��B";

//Services.asp error messages
errmsg.err62="�͐ÓI���[�X�ɐݒ肳��Ă��܂��B";

// *******************************************  COMMON MENU ENTRIES  *******************************************//
var bmenu= new Object();
bmenu.setup="�ݒ�";
bmenu.setupbasic="��{�ݒ�";
bmenu.setupddns="DDNS";
bmenu.setupmacclone="MAC�A�h���X�N���[��";
bmenu.setuprouting="�A�h�o���X���[�e�B���O";
bmenu.setupvlan="VLANs";

bmenu.wireless="���C�A���X";
bmenu.wirelessBasic="��{�ݒ�";
bmenu.wirelessRadius="Radius";
bmenu.wirelessSecurity="���C�A���X�Z�L�����e�B";
bmenu.wirelessMac="MAC�t�B���^�[";
bmenu.wirelessAdvanced="�A�h�o���X�ݒ�";
bmenu.wirelessWds="WDS";

bmenu.security="�Z�L�����e�B";
bmenu.firwall="�t�@�C�A�E�H�[��";
bmenu.vpn="VPN";

bmenu.accrestriction="�A�N�Z�X����";
bmenu.webaccess="�C���^�[�l�b�g�A�N�Z�X";


bmenu.applications="�A�v���P�[�V���� &amp; �Q�[��";
bmenu.applicationsprforwarding="�|�[�g�����W�]��";
bmenu.applicationspforwarding="�|�[�g�]��";
bmenu.applicationsptriggering="�|�[�g�g���K�[�����O";
bmenu.applicationsUpnp="UPnP";
bmenu.applicationsDMZ="DMZ";
bmenu.applicationsQoS="QoS";

bmenu.sipath="SIPatH";
bmenu.sipathoverview="Overview";
bmenu.sipathphone="Phonebook";
bmenu.sipathstatus="Status";

bmenu.admin="�Ǘ���";
bmenu.adminManagement="�Ǘ�";
bmenu.adminHotspot="�z�b�g�X�|�b�g";
bmenu.adminServices="�T�[�r�X";
bmenu.adminAlive="Keep Alive";
bmenu.adminLog="Log";
bmenu.adminDiag="�R�}���h";
bmenu.adminWol="WOL";
bmenu.adminFactory="�����ݒ�";
bmenu.adminUpgrade="�t�@�[���E�F�A�A�b�v�O���[�h";
bmenu.adminBackup="�o�b�N�A�b�v";


bmenu.statu="�X�e�[�^�X";
bmenu.statuRouter="���[�^�[";
bmenu.statuLAN="LAN";
bmenu.statuSputnik="Sputnik�G�[�W�F���g";
bmenu.statuWLAN="���C�A���X";
bmenu.statuSysInfo="Sys-Info";


// ******************************************* Alive.asp *******************************************//

var alive = new Object();
alive.titl=" - Keep Alive";
alive.h2="Keep Alive";
alive.legend="�X�P�W���[�����u�[�g";
alive.sevr1="�X�P�W���[�����u�[�g";
alive.hour="At a set Time";
alive.legend2="WDS/Connection Watchdog";
alive.sevr2="Watchdog��L���ɂ���";
alive.IP="IP�A�h���X";
alive.legend3="�v���N�V�[/�ڑ�Watchdog";
alive.sevr3="�v���N�V�[Watchdog��L���ɂ���";
alive.IP2="�v���N�V�[IP�A�h���X";
alive.port="�v���N�V�[�|�[�g";

//help container
var halive = new Object();
halive.right2="���[�^�[�����u�[�g���鎞�Ԃ�I��ł��������B�Ǘ��^�u��Cron���ݒ肳��Ă���K�v������܂��B";
halive.right4="IP��<em>��</em>�ŋ�؂�ōő�łR�܂łł��BIP�t�H�[�}�b�g�� xxx.xxx.xxx.xxx �ł�";



// ******************************************* config.asp *******************************************//

var config = new Object();
config.titl=" - �o�b�N�A�b�v & ����";
config.h2="�o�b�N�A�b�v�@��\��";
config.legend="�o�b�N�A�b�v�ݒ�";
config.mess1="\"" + sbutton.backup + "\"�{�^�����N���b�N���āA�@��\���o�b�N�A�b�v�t�@�C���������̃R���s���[�^�[�Ƀ_�E�����[�h���Ă��������B";
config.h22="�����\��";
config.legend2="�����ݒ�";
config.mess2="��������t�@�C����I�����Ă�������";
config.mess3="����W A R N I N G";
config.mess4="�{�@��Ɠ����t�@�[���E�F�A�ƃ��f���̃o�b�N�A�b�v�t�@�C���݂̂��A�b�v���[�h���Ă��������B<br />�@�{�ݒ��ʂō쐬����Ă��Ȃ��t�@�C�����A�b�v���[�h���Ȃ��ł��������I";

//help container
var hconfig = new Object();
hconfig.right2="�H��o�ׂ̏����l�Ƀ��[�^�[�̐ݒ��߂��K�v������ꍇ�A���݂̐ݒ���o�b�N�A�b�v���Ă��������B<br /><br /><em>�o�b�N�A�b�v</em>�{�^�����N���b�N���Č��݂̐ݒ���o�b�N�A�b�v���܂��B";
hconfig.right4="<em>�u���E�Y...</em>�{�^����������PC�ɕۑ�����Ă���ݒ�t�@�C���ꗗ��\�����܂��B<br /><br /><em>" + sbutton.restore + "</em>�{�^�����N���b�N����ƁA�S�Ă̋@��ݒ肪�ݒ�t�@�C���ɕۑ����ꂽ�l�ɏ㏑������܂��B";



// ******************************************* DDNS.asp *******************************************//

var ddns = new Object();
ddns.titl=" - �_�C�i�~�b�NDNS"
ddns.h2="�_�C�i�~�b�N �h���C�����V�X�e��(DDNS)";
ddns.legend="DDNS";
ddns.srv="DDNS�T�[�r�X";
ddns.emailaddr="E-mail�A�h���X";
ddns.typ="�^�C�v";
ddns.dynamic="�_�C�i�~�b�N";
ddns.custom="�J�X�^��";
ddns.wildcard="���C���h�J�[�h";
ddns.statu="DDNS�X�e�[�^�X";
ddns.system="DNS�V�X�e��";
ddns.options="�ǉ�DDNS�I�v�V����";

var ddnsm = new Object();
ddnsm.dyn_strange="�T�[�o�[����̉����ɖ�肪����܂��B�������T�[�o�[�ɐڑ����Ă��邩�m�F���Ă��������B";
ddnsm.dyn_good="DDNS�̃A�b�v�f�[�g�ɐ������܂���";
ddnsm.dyn_noupdate="���݁A�A�b�v�f�[�g�͕K�v����܂���";
ddnsm.dyn_nohost="�z�X�g��������܂���";
ddnsm.dyn_notfqdn="�z�X�g���ɖ�肪�Ԉ���Ă��܂�";
ddnsm.dyn_yours="Host is not under your control";
ddnsm.dyn_abuse="Host has been blocked for abuse";
ddnsm.dyn_nochg="�Ō�̃A�b�v�f�[�g����IP�A�h���X�͕ω�����܂���";
ddnsm.dyn_badauth="�F�؂Ɏ��s���܂��� (username ���邢�� passwords)";
ddnsm.dyn_badsys="�s���ȃV�X�e���p�����[�^�[";
ddnsm.dyn_badagent="���̃��[�U�[�G�[�W�F���g�̓u���b�N����܂���";
ddnsm.dyn_numhost="�������邩���Ȃ�����z�X�g��������܂���";
ddnsm.dyn_dnserr="DNS�����G���[";
ddnsm.dyn_911="�\�z�O�̃G���[911";
ddnsm.dyn_999="�\�z�O�̃G���[999";
ddnsm.dyn_donator="��t�҂̂ݗ��p�ł���@�\�ł��B��t�����肢���܂��B";
ddnsm.dyn_uncode="�\�z�O�̖߂�R�[�h";

ddnsm.tzo_good="���슮��";
ddnsm.tzo_noupdate="���݁A�A�b�v�O���[�h�͕K�v����܂���";
ddnsm.tzo_notfqdn="�s���ȃh���C����";
ddnsm.tzo_notmail="�s����Email";
ddnsm.tzo_notact="�s���ȑ���";
ddnsm.tzo_notkey="�s����Key";
ddnsm.tzo_notip="�s����IP�A�h���X";
ddnsm.tzo_dupfqdn="�����h���C����";
ddnsm.tzo_fqdncre="�h���C�����͂��łɍ쐬����Ă��܂�";
ddnsm.tzo_expired="�A�J�E���g�͊����؂�ł�";
ddnsm.tzo_error="�\�z�O�̃T�[�o�[�G���[";

ddnsm.zone_701="���̃A�J�E���g�̓]�[�����Z�b�g�A�b�v����Ă��܂���";
ddnsm.zone_702="�A�b�v�f�[�g���s";
ddnsm.zone_703="<em>zones</em> ���邢�� <em>host</em> �̂ǂ��炩���K�v�ł�";
ddnsm.zone_704="�]�[���͗L����<em>dotted</em>�C���^�[�l�b�g���łȂ���΂����܂���";
ddnsm.zone_705="�]�[���͋�ł͂����܂���";
ddnsm.zone_707="�����z�X�g�IP�ɓ���̃A�b�v�f�[�g�ł��B�N���C�A���g�̐ݒ���C�����Ă�������";
ddnsm.zone_201="���R�[�h�̃A�b�v�f�[�g�͕K�v����܂���";
ddnsm.zone_badauth="�F�؂Ɏ��s���܂��� (username ���邢�� passwords)";
ddnsm.zone_good="ZoneEdit�̃A�b�v�f�[�g���������܂���";
ddnsm.zone_strange="�s���ȃT�[�o�[�����ł��B�������T�[�o�[�ɐڑ����Ă��邩�m�F���Ă��������B";

ddnsm.all_closed="DDNS�T�[�o�[�͌��݁A������Ă��܂�";
ddnsm.all_resolving="�h���C�������������Ă��܂�";
ddnsm.all_errresolv="�h���C�����̉����Ɏ��s���܂���";
ddnsm.all_connecting="�T�[�o�[�ɐڑ����Ă��܂�";
ddnsm.all_connectfail="�T�[�o�[�ւ̐ڑ��Ɏ��s���܂���";
ddnsm.all_disabled="DDNS�@�\�͎g�p�s�ɂȂ��Ă��܂�";
ddnsm.all_noip="�C���^�[�l�b�g�ڑ��ł��܂���";

//help container
var hddns = new Object();
hddns.right2="DDNS���g����IP�A�h���X�̑���Ƀh���C�����Ńl�b�g���[�N�ɃA�N�Z�X�ł��܂��BIP�A�h���X�̕ύX�ɑΉ����āAIP�A�h���X�ƃh���C�����𓮓I�ɃA�b�v�f�[�g���܂��BDynDNS�Afreedns.afraid.org�AZoneEdit.com�ANo-IP.com�ACustom�Ȃǂ̃T�[�r�X�ƌ_�񂵂Ă��������B";



// ******************************************* Diagnostics.asp *******************************************//

var diag = new Object();
diag.titl=" - �f�f";
diag.h2="�f�f";
diag.legend="�R�}���h�V�F��";
diag.cmd="�R�}���h";
diag.startup="�X�^�[�g�A�b�v";
diag.firewall="�t�@�C�A�E�H�[��";

//help container
var hdiag = new Object();
hdiag.right2="�E�F�u��ʂ���R�}���h�𔭍s���邱�Ƃ��ł��܂��B�e�L�X�g�G���A�ɃR�}���h��ł����݁A<em>" + sbutton.runcmd + "</em>�������Ă��������B";



// ******************************************* DMZ.asp *******************************************//

var dmz = new Object();
dmz.titl=" - DMZ";
dmz.h2="Demilitarized Zone (DMZ)";
dmz.legend="DMZ";
dmz.serv="DMZ���g�p����";
dmz.host="DMZ�z�X�gIP�A�h���X";


//help container
var hdmz = new Object();
hdmz.right2="���̃I�v�V�������g�p����ƁA�I�������z�X�g���C���^�[�l�b�g�ɂ��炳��܂��B�S�Ẵ|�[�g���C���^�[�l�b�g����A�N�Z�X�ł���悤�ɂȂ�܂��B";



// ******************************************* Factory_Defaults.asp *******************************************//

var factdef = new Object();
factdef.titl=" - �����ݒ�";
factdef.h2="�����ݒ�";
factdef.legend="���[�^�[�ݒ�̃��Z�b�g";
factdef.restore="�����ݒ�𕜌�����";

factdef.mess1="���ӁI�n�j���N���b�N����Ƌ@��̐ݒ�͏����ݒ�ɖ߂�A���܂ł̐ݒ�͑S�ď�������܂��B";

//help container
var hfactdef = new Object();
hfactdef.right1="�S�Ă̐ݒ�������ݒ�ɖ߂��܂��B���܂ł̐ݒ�͏�������܂��B";



// ******************************************* FilterIP%AC.asp *******************************************//

var filterIP = new Object();
filterIP.titl=" - PC���X�g";
filterIP.h2="PC���X�g";
filterIP.h3="PC��MAC�A�h���X�� xx:xx:xx:xx:xx:xx �̃t�H�[�}�b�g�œ��͂��Ă�������";
filterIP.h32="PC��IP�A�h���X����͂��Ă�������";
filterIP.h33="PC��IP�����W����͂��Ă�������";
filterIP.ip_range="IP�����W";



// ******************************************* Filter.asp *******************************************//

var filter = new Object();
filter.titl=" - �A�N�Z�X����";
filter.h2="�C���^�[�l�b�g�A�N�Z�X";
filter.legend="�A�N�Z�X�|���V�[";
filter.restore="�����ݒ�̕���";
filter.pol="�|���V�[";
filter.polname="�|���V�[��";
filter.pcs="PCs";
filter.polallow="�I�����������̃C���^�[�l�b�g�A�N�Z�X�B";
filter.legend2="��";
filter.time="��";
filter.h24="24����";
filter.legend3="�u���b�N���ꂽ�T�[�r�X";
filter.catchall="�S�Ă�P2P�v���g�R�����L���b�`����";
filter.legend4="URL�A�h���X�ɂ��E�F�u�T�C�g�u���b�N";
filter.legend5="�L�[���[�h�ɂ��ɂ��E�F�u�T�C�g�u���b�N";

filter.mess1="�|���V�[���폜���܂����H";
filter.mess2="�Œ�1���͑I�����Ă��������B";
filter.mess3="�X�^�[�g���Ԃ͏I�����ԈȑO���w�肵�Ă��������B";

//help container
var hfilter = new Object();
hfilter.right2="�ő�10�̃A�N�Z�X�|���V�[���`�ł��܂��B<em>" + sbutton.del + "</em>���N���b�N���ă|���V�[���폜���邩�A<em>" + sbutton.summary + "</em>���N���b�N���ă|���V�[�̈ꗗ��\�����Ă��������B";
hfilter.right4="�|���V�[��L�����邢�͖����ɂ���B";
hfilter.right6="�|���V�[�ɖ��O�����邱�Ƃ��ł��܂��B";
hfilter.right8="�|���V�[��L���ɂ���j����I�����Ă��������B";
hfilter.right10="�|���V�[��L���ɂ��鎞������͂��Ă�������";
hfilter.right12="�������̃T�[�r�X�ւ̃A�N�Z�X���u���b�N���邱�Ƃ��ł��܂��B<em>" + sbutton.filterSer + "</em>���N���b�N���Đݒ��ύX���Ă�������";
hfilter.right14="URL�Ŏw�肵���E�F�u�T�C�g�ւ̃A�N�Z�X���u���b�N�ł��܂��B";
hfilter.right16="�w�肵���L�[���[�h���܂ރE�F�u�T�C�g�ւ̃A�N�Z�X���u���b�N�ł��܂��B";



// ******************************************* FilterSummary.asp *******************************************//

var filterSum = new Object();
filterSum.titl=" - �A�N�Z�X�����T�}���[";
filterSum.h2="�C���^�[�l�b�g �|���V�[ �T�}���[";
filterSum.polnum="No.";
filterSum.polday="����";



// ******************************************* Firewall.asp *******************************************//

var firewall = new Object();
firewall.titl=" - �t�@�C�A�E�H�[��";
firewall.h2="�Z�L�����e�B";
firewall.legend="�t�@�C�A�E�H�[���ɂ��ی�";
firewall.firewall="SPI�t�@�C�A�E�H�[��";
firewall.legend2="�ǉ��t�B���^�[";
firewall.proxy="�v���N�V�[���t�B���^�[";
firewall.cookies="�N�b�L�[���t�B���^�[";
firewall.applet="Java�A�v���b�g���t�B���^�[";
firewall.activex="ActiveX���t�B���^�[";
firewall.legend3="WAN���N�G�X�g�u���b�N";
firewall.ping="�����̃C���^�[�l�b�g�A�N�Z�X���u���b�N";
firewall.muticast="�}���`�L���X�g���t�B���^�[";
filter.nat="�C���^�[�l�b�gNAT���_�C���N�V�������t�B���^�[";
filter.port113="IDENT (Port 113)���t�B���^�[";

//help container
var hfirewall = new Object();
hfirewall.right2="SPI�t�@�C�A�E�H�[����L��������ɂ���";



// ******************************************* Forward.asp *******************************************//

var prforward = new Object();
prforward.titl=" - Port Range Forwarding";
prforward.h2="Port Range Forward";
prforward.legend="�]��";
prforward.app="�A�v���P�[�V����";

//help container
var hprforward = new Object();
hprforward.right2="�A�v���P�[�V�����ɂ���Ă̓|�[�g���J������K�v������܂��B��Ƃ��ăT�[�o�[��I�����C���Q�[���Ȃǂ�����܂��B�C���^�[�l�b�g���炠��|�[�g�ւ̗v���������ꍇ�A���[�^�[�͎w�肳�ꂽ�R���s���[�^�[�փf�[�^�����[�g���܂��B�Z�L�����e�B�̂��ߎg�p���Ă���|�[�g�փ|�[�g�t�H���[�f�B���O�����邱�Ƃ��ł��܂��B�I�����<em>" + share.enable +"</em>�̃`�F�b�N���O���Ă��������B";



// ******************************************* ForwardSpec.asp *******************************************//

var pforward = new Object();
pforward.titl=" - �|�[�g�t�H���[�f�B���O";
pforward.h2="�|�[�g�]��";
pforward.legend="�]��";
pforward.app="�A�v���P�[�V����";
pforward.from="��t�|�[�g";
pforward.to="�o�̓|�[�g";

//help container
var hpforward = new Object();
hpforward.right2="�A�v���P�[�V�����ɂ���Ă̓|�[�g���J������K�v������܂��B��Ƃ��ăT�[�o�[��I�����C���Q�[���Ȃǂ�����܂��B�C���^�[�l�b�g���炠��|�[�g�ւ̗v���������ꍇ�A���[�^�[�͎w�肳�ꂽ�R���s���[�^�[�փf�[�^�����[�g���܂��B�Z�L�����e�B�̂��ߎg�p���Ă���|�[�g�փ|�[�g�t�H���[�f�B���O�����邱�Ƃ��ł��܂��B�I�����<em>" + share.enable +"</em>�̃`�F�b�N���O���Ă��������B";



// ******************************************* Hotspot.asp *******************************************//

var hotspot = new Object();
hotspot.titl=" - �z�b�g�X�|�b�g";
hotspot.h2="�z�b�g�X�|�b�g Portal";
hotspot.legend="�`���X�|�b�g";
hotspot.nowifibridge="Wifi��LAN�u���b�W��؂藣��";
hotspot.hotspot="�`���X�|�b�g";
hotspot.pserver="Primary Radius Server IP/DNS";
hotspot.bserver="Backup Radius Server IP/DNS";
hotspot.dns="DNS IP";
hotspot.url="URL���_�C���N�g";
hotspot.dhcp="DHCP�C���^�[�t�F�[�X";
hotspot.radnas="Radius NAS ID";
hotspot.uam="UAM Secret";
hotspot.uamdns="UAM Any DNS";
hotspot.allowuam="UAM������";
hotspot.macauth="MACauth";
hotspot.option="�ǉ��`���X�|�b�g�I�v�V����";
hotspot.fon_chilli="�`���X�|�b�g Local ���[�U�[�Ǘ�";
hotspot.fon_user="���[�U�[�ꗗ";
hotspot.http_legend="HTTP���_�C���N�g";
hotspot.http_srv="HTTP���_�C���N�g";
hotspot.http_ip="HTTP Destination IP";
hotspot.http_port="HTTP Destination Port";
hotspot.http_net="HTTP Source Network";
hotspot.nocat_legend="NoCatSplash";
hotspot.nocat_srv="NoCatSplash";
hotspot.nocat_gateway="Gateway Name";
hotspot.nocat_home="�z�[���y�[�W";
hotspot.nocat_allowweb="�����ꂽ�E�F�u�z�X�g";
hotspot.nocat_docroot="Document Root";
hotspot.nocat_splash="Splash URL";
hotspot.nocat_port="���OPorts";
hotspot.nocat_timeout="Login Timeout";
hotspot.nocat_verbose="�`��x";
hotspot.nocat_route="Route Only";
hotspot.smtp_legend="SMTP���_�C���N�g";
hotspot.smtp_srv="SMTP���_�C���N�g";
hotspot.smtp_ip="SMTP Destination IP";
hotspot.smtp_net="SMTP Source Network";
hotspot.shat_legend="Zero IP �ݒ�";
hotspot.shat_srv="Zero IP �ݒ�";
hotspot.shat_srv2="Zero IP �ݒ�͗L���ł�";
hotspot.sputnik_legend="Sputnik";
hotspot.sputnik_srv="Sputnik �G�[�W�F���g";
hotspot.sputnik_id="Sputnik �T�[�o�[ ID";
hotspot.sputnik_instant="Use Sputnik �ȒP�ݒ�";
hotspot.sputnik_express="Use SputnikNet Express";
hotspot.sputnik_about="Sputnik�ɂ���";



// ******************************************* Info.htm *******************************************//

var info = new Object();
info.titl=" - ���";
info.h2="�V�X�e�����";
info.wlanmac="���C�A���XMAC";
info.srv="�T�[�r�X";
info.ap="�A�N�Z�X�|�C���g";



// ******************************************* index_heartbeat.asp *******************************************//

var idx_h = new Object();
idx_h.srv="Heart Beat Server";
idx_h.con_strgy="�ڑ���p";
idx_h.max_idle="�v���ɉ������ڑ�: �A�C�h�����Ԃ��ő�";
idx_h.alive="Keep Alive: Redial Period";



// ******************************************* index_l2tp.asp *******************************************//

var idx_l = new Object();
idx_l.srv="L2TP�T�[�o�[";



// ******************************************* index_pppoe.asp *******************************************//

var idx_pppoe = new Object();
idx_pppoe.use_rp="RP PPPoE��p����";



// ******************************************* index_pptp.asp *******************************************//

var idx_pptp = new Object();
idx_pptp.srv="Use DHCP";
idx_pptp.wan_ip="�C���^�[�l�b�gIP�A�h���X";
idx_pptp.gateway="�Q�[�g�E�F�C (PPTP�T�[�o�[)";
idx_pptp.encrypt="PPTP�Í���";



// ******************************************* index_static.asp *******************************************//

var idx_static = new Object();
idx_static.dns="�X�^�e�B�b�NDNS";



// ******************************************* index.asp *******************************************//

var idx = new Object();
idx.titl=" - �ݒ�";
idx.h2="�C���^�[�l�b�g�ݒ�";
idx.h22="���C�A���X�ݒ�";
idx.legend="�C���^�[�l�b�g�ڑ��^�C�v";
idx.conn_type="�ڑ��^�C�v";
idx.stp="STP";
idx.stp_mess="(COMCAST ISP�̂��ߖ����ɂ���)";
idx.optional="�I�v�V�����ݒ�(ISP�ɂ��K�v)";
idx.mtu="MTU";
idx.h23="�l�b�g���[�N�ݒ�";
idx.routerip="���[�^�[IP";
idx.lanip="���[�J��IP�A�h���X";
idx.legend2="WAN�|�[�g";
idx.wantoswitch="�X�C�b�`����WAN�|�[�g���w��";
idx.legend3="�����ݒ�";
idx.timeset="�^�C���]�[��/�Ď���(DST)";
idx.localtime="���[�J�����Ԃ�p����";
idx.static_ip="�X�^�e�B�b�NIP";
idx.dhcp="�����ݒ� - DHCP";
idx.dhcp_legend="�l�b�g���[�N �A�h���X �T�[�o�[�ݒ� (DHCP)";
idx.dhcp_type="DHCP�^�C�v";
idx.dhcp_srv="DHCP�T�[�o�[";
idx.dhcp_fwd="DHCP Forwarder";
idx.dhcp_start="IP�A�h���X���J�n";
idx.dhcp_end="IP�A�h���X���I��";        //used in Status_Lan.asp
idx.dhcp_maxusers="�ő�DHCP���[�U�[��";
idx.dhcp_lease="�N���C�A���g���[�X����";
idx.dhcp_dnsmasq="DHCP��DNSMasq���g��";
idx.dns_dnsmasq="DHCP��DNSMasq���g��";
idx.auth_dnsmasq="DHCP-Authoritative";



//help container
var hidx = new Object();
hidx.right2="���̐ݒ�́A�P�[�u����Ђł����Ƃ��L���g���Ă��܂��B";
hidx.right4="Enter the host name provided by your ISP����w�肳�ꂽ�z�X�g�������";
hidx.right6="Enter the domain name provided by your ISP����w�肳�ꂽ�h���C���������";
hidx.right8="���[�^�[�̃A�h���X�ł�";
hidx.right10="T���[�^�[�̃T�u�l�b�g�}�X�N(subnet mask)�ł�";
hidx.right12="���[�^�[��IP�A�h���X�̊Ǘ���������";
hidx.right14="�J�n�̍ۂɎg�������A�h���X";
hidx.right16="���[�^�[���w�肷��A�h���X�𐧌����邱�Ƃ��ł��܂��B";
hidx.right18="�^�C���]�[���ƉĎ��ԁiDST�j���w�肵�Ă��������B���[�^�[�����[�J�����Ԃ�UTC���Ԃ��g���܂��B";



// ******************************************* Join.asp *******************************************//

var join = new Object();

//sshd.webservices
join.titl=" - �W���C��";
join.mess1="���̃l�b�g���[�N�ƃN���C�A���g�Ƃ��ăW���C���ɐ������܂���: ";



// ******************************************* Log_incoming.asp *******************************************//

var log_in = new Object();
log_in.titl=" - Incoming���O�e�[�u��";
log_in.h2="Incoming���O�e�[�u��";
log_in.th_ip="Source IP";
log_in.th_port="Destination Port Number";



// ******************************************* Log_outgoing.asp *******************************************//

var log_out = new Object();
log_out.titl=" - Outgoing���O�e�[�u��";
log_out.h2="Outgoing���O�e�[�u��";
log_out.th_lanip="LAN IP";
log_out.th_wanip="Destination URL/IP";
log_out.th_port="�T�[�r�X/�|�[�g�ԍ�";



// ******************************************* Log.asp *******************************************//

var log = new Object();
log.titl=" - ���O";
log.h2="���O�Ǘ�";
log.legend="���O";
log.lvl="���O���x��";
log.drop="�h���b�v";
log.reject="����";
log.accept="�A�N�Z�v�g";



// ******************************************* Management.asp *******************************************//

var management = new Object();
management.titl=" - �Ǘ���";
management.h2="���[�^�[�Ǘ�";

management.psswd_legend="���[�^�[�p�X���[�h";
management.psswd_user="���[�^�[���[�U�[��";
management.psswd_pass="���[�^�[�p�X���[�h";
management.pass_conf="�m�F�̂��ߍē���";

management.remote_legend="�����[�g�A�N�Z�X";
management.remote_gui="�E�F�u�Ǘ����";
management.remote_https="HTTPS���g�p";
management.remote_guiport="�E�F�u�Ǘ���ʃ|�[�g";
management.remote_ssh="SSH�Ǘ�";
management.remote_sshport="SSH�����[�g�|�[�g";

management.web_legend="�E�F�u�A�N�Z�X";
management.web_refresh="�������t���b�V��(�b)";
management.web_sysinfo="Info Site��L���ɂ���";
management.web_sysinfopass="Info Site �p�X���[�h�ی�";
management.web_sysinfomasq="Info Site MAC�}�X�J���[�h";

management.boot_legend="Boot Wait";
management.boot_srv="Boot Wait";

management.cron_legend="Cron";
management.cron_srvd="Cron";

management.loop_legend="Loopback";
management.loop_srv="Loopback";

management.wifi_legend="802.1x";
management.wifi_srv="802.1x";

management.ntp_legend="NTP�N���C�A���g";
management.ntp_srv="NTP";

management.rst_legend="���Z�b�g�{�^��";
management.rst_srv="���Z�b�g�{�^��";

management.routing_legend="���[�e�B���O";
management.routing_srv="���[�e�B���O";

management.wol_legend="Wake-On-LAN";
management.wol_srv="WOL";
management.wol_pass="SecureOn�p�X���[�h";
management.wol_mac="MAC�A�h���X<br/>( �t�H�[�}�b�g: xx:xx:xx:xx:xx:xx )";

management.ipv6_legend="IPv6�T�|�[�g";
management.ipv6_srv="IPv6";
management.ipv6_rad="Radvd��L���ɂ���";
management.ipv6_radconf="Radvd�ݒ�";

management.jffs_legend="JFFS2�T�|�[�g";
management.jffs_srv="JFFS2";
management.jffs_clean="Clean JFFS2";

management.lang_legend="����I��";
management.lang_srv="����";
management.lang_bulgarian="�u���K���A��";
management.lang_chinese_traditional="������i�ɑ̎��j";
management.lang_chinese_simplified="������i�ȑ̎��j";
management.lang_croatian="�N���A�`�A��";
management.lang_czech="�`�F�R��";
management.lang_dutch="�I�����_��";
management.lang_portuguese_braz="�|���g�K���� (�u���W��)";
management.lang_english="�p��";
management.lang_french="����";
management.lang_german="�ƌ�";
management.lang_italian="�ɑ�������";
management.lang_brazilian="�u���W����";
management.lang_slovenian="�X���x�j�A��";
management.lang_spanish="�X�y�C����";
management.lang_swedish="�X�E�F�[�f����"; // management.lang_japanese="���{��";

management.net_legend="IP�t�B���^�[�ݒ� (P2P�p�̐ݒ�)";
management.net_port="�ő�Port";
management.net_tcptimeout="TCP Timeout (�b)";
management.net_udptimeout="UDP Timeout (�b)";

management.clock_legend="�I�[�o�[�N���b�N";
management.clock_frq="���g��";
management.clock_support="�T�|�[�g�O�ł�";

management.mmc_legend="MMC/SD�J�[�h�T�|�[�g";
management.mmc_srv="MMC�f�o�C�X";

management.samba_legend="Samba FS Automount";
management.samba_srv="SMB�t�@�C���V�X�e��";
management.samba_share="Share";
management.samba_stscript="Startscript";

management.SIPatH_srv="SIPatH";
management.SIPatH_port="SIP�|�[�g";
management.SIPatH_domain="SIP�h���C��";

management.gui_style="���[�^�[GUI�X�^�C��";



//help container
var hmanagement = new Object();
hmanagement.right1="Auto-Refresh:";
hmanagement.right2="�E�F�uGUI�̎����X�V���Ԃ𒲐����܂��B0�͖����ɂ��܂��B";



// ************ Port_Services.asp (used by Filters.asp and QoS.asp, QOSPort_Services.asp not used anymor) *****************************************//

var portserv = new Object();
portserv.titl=" - �|�[�g�T�[�r�X";
portserv.h2="�|�[�g�T�[�r�X";



// ******************************************* QoS.asp *******************************************//

var qos = new Object();
qos.titl=" - Quality of Service";
qos.h2="Quality Of Service (QoS)";
qos.legend="QoS�ݒ�";
qos.srv="QoS�J�n";
qos.type="�p�P�b�g �X�P�W���[���[";
qos.uplink="Uplink (kbps)";
qos.dnlink="Downlink (kbps)";
qos.gaming="�Q�[���p�ɍœK��";
qos.legend2="�T�[�r�X �v���C�I���e�B";
qos.prio_x="�Ə�";
qos.prio_p="�v���~�A��";
qos.prio_e="�G�N�X�v���X";
qos.prio_s="�X�^���_�[�h";
qos.prio_b="�o���N";
qos.legend3="�l�b�g�}�X�N �v���C�I���e�B";
qos.ipmask="IP/Mask";
qos.maxrate_b="�ő�Kbits";
qos.maxrate_o="�ő�Rate";
qos.legend4="MAC �v���C�I���e�B";
qos.legend5="Ethernet�|�[�g �v���C�I���e�B";
qos.legend6="�f�B�t�H���g����e�ʃ��x��";
qos.bandwith="����e�ʁiKbits�j";

//help container
var hqos = new Object();
hqos.right1="Uplink:";
hqos.right2="�A�b�v���[�h������80%-95% (max)�ɐݒ肵�Ă�������";
hqos.right3="Downlink:";
hqos.right4="�g�[�^���_�E�����[�h������80%-100% (max)�ɐݒ肵�Ă�������";
hqos.right6="�ʐM�ш������Ă���A�v���P�[�V�����ɂ��ăf�[�^���[�g�𐧌�ł��܂��B";
hqos.right8="IP�A�h���X���邢��IP�A�h���X�͈̔͂ɂ��ĒʐM�̃v���C�I���e�B��ݒ�ł��܂��B";
hqos.right10="�f�o�C�X����MAC�A�h���X���w�肵�āA�l�b�g���[�N��ɂ���f�o�C�X����̒ʐM�̃v���C�I���e�B��ݒ�ł��܂��B";
hqos.right12="�f�o�C�X���ڑ����Ă��镨���ILAN�|�[�g�����ɁA�f�[�^���[�g�𐧌�ł��܂��BLAN�|�[�g���ƂɂP����S�̃v���C�I���e�B���w��ł��܂��B";



// ******************************************* RouteTable.asp *******************************************//

var routetbl = new Object();
routetbl.titl=" - ���[�e�B���O �e�[�u��";
routetbl.h2="���[�e�B���O �e�[�u�� ���̓��X�g";
routetbl.th1="Destination LAN IP";



// ******************************************* Routing.asp *******************************************//

var route = new Object();
route.titl=" - ���[�e�B���O";
route.h2="Advanced ���[�e�B���O";
route.mod="���샂�[�h";
route.bgp_legend="BGP�ݒ�";
route.bgp_ip="Neighbor IP";
route.bgp_as="Neighbor AS#";
route.rip2_mod="RIP2���[�^�[";
route.ospf_mod="OSPF���[�^�[";
route.gateway_legend="���I���[�^�[";
route.static_legend="�ÓI���[�^�[";
route.static_setno="set number��I��";
route.static_name="���[�g��";
route.static_ip="Destination LAN IP";

//help container
var hroute = new Object();
hroute.right2="���[�^�[��p���ăC���^�[�l�b�g�ڑ��������Ƃ��Ă���ꍇ�́A<em>�Q�[�g�E�F�C</em>���[�h��I�����Ă��������B���̃��[�^�[���l�b�g���[�N��ɂ���ꍇ�A<em>���[�^�[</em>���[�h��I�����Ă��������B";
hroute.right4="����̓��j�[�N�ȃ��[�g�ԍ��ł��B�ő�20�܂ł̃��[�g�ɂ��ݒ�ł��܂��B";
hroute.right6="���̃��[�g�ɂ��閼�O����͂��Ă��������B";
hroute.right8="����̓X�^�e�B�b�N���[�g�����蓖�Ă郊���[�g�z�X�g�ł��B";
hroute.right10="�z�X�g�ƃl�b�g���[�N�|�[�V���������肵�܂��B";


// ******************************************* Site_Survey.asp *******************************************//

var survey = new Object();
survey.titl=" - �T�C�g����";
survey.h2="Neighbor&#39;s Wireless Networks";
survey.thjoin="�T�C�g�̃W���C��";



// ******************************************* Services.asp *******************************************//

var service = new Object();

service.titl=" - �T�[�r�X";
service.h2="�T�[�r�X�Ǘ�";

//kaid
service.kaid_legend="XBOX Kaid";
service.kaid_srv="Kaid�J�n";
service.kaid_mac="�R���\�[��Macs: (�u;�v�ŏI�����邱��)";

//DHCPd
service.dhcp_legend="DHCP�N���C�A���g";
service.dhcp_vendor="�x���_�[�N���X�̐ݒ�";
service.dhcp_legend2="DHCP�T�[�o�[";
service.dhcp_srv="DHCP Daemon";
service.dhcp_jffs2="�N���C�A���g���[�XDB��JFFS2���g��";
service.dhcp_domain="�g�p����Ă���h���C��";
service.dhcp_landomain="LAN�h���C��";
service.dhcp_option="Additional DHCPd�I�v�V����";
service.dnsmasq_legend="DNSMasq";
service.dnsmasq_srv="DNSMasq";
service.dnsmasq_loc="���[�J��DNS";
service.dnsmasq_opt="�ǉ�DNS�I�v�V����";

//pptp.webservices
service.pptp_legend="PPTP";
service.pptp_srv="PPTP�T�[�o�[";
service.pptp_client="�N���C�A���gIP(s)";
service.pptp_chap="CHAP-Secrets";

//syslog.webservices
service.syslog_legend="�V�X�e�����O";
service.syslog_srv="Syslogd";
service.syslog_ip="�����[�g �T�[�o�[";

//telnet.webservices
service.telnet_legend="Telnet";
service.telnet_srv="Telnet";

//pptpd_client.webservices
service.pptpd_legend="PPTP�N���C�A���g";
service.pptpd_option="PPTP�N���C�A���g �I�v�V����";
service.pptpd_ipdns="�T�[�o�[IP or DNS��";
service.pptpd_subnet="�����[�gSubnet";
service.pptpd_subnetmask="�����[�gSubnet�}�X�N";
service.pptpd_encry="MPPE�Í���";
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
service.vpn_legend="OpenVPN�N���C�A���g";
service.vpn_srv="OpenVPN�̊J�n";
service.vpn_ipname="�T�[�o�[IP/��";
service.vpn_mtu="TUN MTU�ݒ�";
service.vpn_mru="TUN MTU Extra";
service.vpn_mss="TCP MSS";
service.vpn_compress="LZO���k���g��";
service.vpn_tunnel="Tunnel�v���g�R��";
service.vpn_srvcert="�p�u���b�N �T�[�o�[ �ؖ���";
service.vpn_clicert="�p�u���b�N �N���C�A���g �ؖ���";
service.vpn_certtype="nsCertType";
service.vpn_clikey="�v���C�x�[�g �N���C�A���g �L�[";

//sshd.webservices
service.ssh_legend="�Z�L���A�V�F��";
service.ssh_srv="SSHd";
service.ssh_password="�p�X���[�h���O�C��";
service.ssh_key="Authorized Keys";



// ******************************************* Sipath.asp + cgi *******************************************//

var sipath = new Object();
sipath.titl=" - SiPath �T�v";
sipath.phone_titl=" - �t�H�[���u�b�N";
sipath.status_titl=" - �X�e�[�^�X";



// ******************************************* Status_Lan.asp *******************************************//

var status_lan = new Object();
status_lan.titl=" - LAN�X�e�[�^�X";
status_lan.h2="���[�J�� �l�b�g���[�N";
status_lan.legend="LAN�X�e�[�^�X";
status_lan.h22="Dynamic Host Configuration Protocol";
status_lan.legend2="DHCP�X�e�[�^�X";
status_lan.legend3="DHCP�N���C�A���g";

//help container
var hstatus_lan = new Object();
hstatus_lan.right2="���[�J��Ethernet�l�b�g���[�N�ɂ����郋�[�^�[��MAC�A�h���X�ł��B";
hstatus_lan.right4="���[�J��Ethernet�l�b�g���[�N�ɂ����郋�[�^�[��IP�A�h���X�ł��B";
hstatus_lan.right6="���[�^�[��Subnet�}�X�N���g�p���Ă���ꍇ�A�����ɕ\������܂��B";
hstatus_lan.right8="���[�^�[��DHCP�T�[�o�[�Ƃ��Ďg�p���Ă���ꍇ�A�����ɕ\������܂��B";
hstatus_lan.right10="MAC�A�h���X���N���b�N����ƁA�l�b�g���[�N�C���^�[�t�F�[�X��Organizationally Unique Identifier (IEEE�W����OUI�f�[�^�x�[�X����)��������܂��B";



// ******************************************* Status_Router.asp *******************************************//

var status_router = new Object();
status_router.titl=" - ���[�^�[ �X�e�[�^�X";
status_router.h2="���[�^�[���";
status_router.legend="�V�X�e��";
status_router.sys_model="���[�^�[���f��";
status_router.sys_firmver="�t�@�[���E�F�A �o�[�W����";
status_router.sys_time="���ݎ���";
status_router.sys_up="Uptime";
status_router.sys_load="���σ��[�h";
status_router.legend2="CPU";
status_router.cpu="CPU���f��";
status_router.clock="CPU�N���b�N";
status_router.legend3="������";
status_router.mem_tot="Total Available";
status_router.mem_free="�t���[";
status_router.mem_used="�g�p";
status_router.mem_buf="�o�b�t�@�[";
status_router.mem_cached="�L���b�V��";
status_router.mem_active="�L��";
status_router.mem_inactive="����";
status_router.legend4="�l�b�g���[�N";
status_router.net_maxports="IP�t�B���^�[�ő�|�[�g";
status_router.net_conntrack="�L����IP�ڑ�";
status_router.h22="�C���^�[�l�b�g";
status_router.legend5="�ݒ�^�C�v";
status_router.www_login="���O�C���^�C�v";
status_router.www_loginstatus="���O�C�� �X�e�[�^�X";

//help container
var hstatus_router = new Object();
hstatus_router.right2="���[�^�[�̖��O�ł��B�^�u��<i>�Z�b�g�A�b�v</i>�^�u����ݒ�ł��܂��B";
hstatus_router.right4="ISP���猩���郋�[�^�[��MAC�A�h���X�ł��B";
hstatus_router.right6="���݂̃��[�^�[�̃t�@�[���E�F�A�ł�";
hstatus_router.right8="This is time received from the ntp server set on the <em>" + bmenu.admin + " | " + bmenu.adminManagement + "</em> tab.";
hstatus_router.right10="���[�^�[�����삵�ċ�̌o�ߎ��Ԃł��B";
hstatus_router.right12="����1���A5���A15���ɂ�����V�X�e���̃��[�h��3�̐����ŕ\���܂��B";
hstatus_router.right14="���g����ISP���g���ăC���^�[�l�b�g�ڑ��ɕK�v�ȏ��ł��B�ݒ�^�u������͂��ꂽ���ł��B���݂̒ʐM��<em>�ڑ�</em>���邢��<em>�ؒf</em>�ł��܂��B";



// ******************************************* Status_SputnikAPD.asp *******************************************//

var status_sputnik = new Object();
status_sputnik.titl=" - Sputnik�G�[�W�F���g �X�e�[�^�X";
status_sputnik.h2="Sputnik&reg; �G�[�W�F���g&trade;";
status_sputnik.manage="Managed By";
status_sputnik.license="SCC���C�Z���X�ԍ�";

//help container
var hstatus_sputnik = new Object();
hstatus_sputnik.right1="Sputnik�G�[�W�F���g �X�e�[�^�X";
hstatus_sputnik.right2="Sputnik�G�[�W�F���g�v���Z�X�̃X�e�[�^�X��\�����Ă��܂��B";
hstatus_sputnik.right4="�A�N�Z�X�|�C���g���ڑ����Ă���Sputnik�R���g���[���Z���^�[";
hstatus_sputnik.right6="���݂̃G�[�W�F���g �X�e�[�^�X";
hstatus_sputnik.right8="Sputnik�R���g���[���Z���^�[�̃��C�Z���X�ԍ�";



// ******************************************* Status_Wireless.asp *******************************************//

var status_wireless = new Object();
status_wireless.titl=" - ���C�A���X �X�e�[�^�X";
status_wireless.h2="���C�A���X";
status_wireless.legend="���C�A���X �X�e�[�^�X";
status_wireless.net="�l�b�g���[�N";
status_wireless.pptp="PPTP�X�e�[�^�X";
status_wireless.legend2="���C�A���X �p�P�b�g���";
status_wireless.rx="Received (RX)";
status_wireless.tx="Transmitted (TX)";
status_wireless.h22="���C�A���X �m�[�h";
status_wireless.legend3="�N���C�A���g";
status_wireless.signal_qual="�V�O�i���i��";
status_wireless.wds="WDS�m�[�h";

//help container
var hstatus_wireless = new Object();
hstatus_wireless.right2="���[�J�����C�A���X�l�b�g���[�N�ɂ����郋�[�^�[��MAC�A�h���X�ł��B";
hstatus_wireless.right4="���C�A���X�^�u�Ŏw�肳�ꂽ�A�l�b�g���[�N�Ŏg�p���郏�C�A���X���[�h��\�����܂��i���݁AG�̂݁AB�̂݁A�����j";



// ******************************************* Triggering.asp *******************************************//

var trforward = new Object();
trforward.titl=" - �|�[�gTriggering";
trforward.h2="�|�[�gTriggering";
trforward.legend="Forwards";
trforward.trrange="Triggered�|�[�g�����W";
trforward.fwdrange="Forwarded�|�[�g�����W";
trforward.app="�A�v���P�[�V����";

//help container
var htrforward = new Object();
htrforward.right2="trigger�ƂȂ�A�v���P�[�V�����������";
htrforward.right4="�e�A�v���P�[�V�������ƂɁATriggered�ԍ��͈̔͂��ꗗ���܂��B�C���^�[�l�b�g�A�v���P�[�V�����̃h�L�������g����K�v�Ƃ����|�[�g�ԍ����m�F���Ă��������B";
htrforward.right6="�e�A�v���P�[�V�������ƂɁAForwarded�ԍ��͈̔͂��ꗗ���܂��B�C���^�[�l�b�g�A�v���P�[�V�����̃h�L�������g����K�v�Ƃ����|�[�g�ԍ����m�F���Ă��������B";
htrforward.right8="Triggered and Forwarded�͈͂̎n�܂�̃|�[�g�ԍ�����͂��Ă��������B";
htrforward.right10="Triggered and Forwarded�͈͂̏I���̃|�[�g�ԍ�����͂��Ă��������B ";



// ******************************************* Upgrade.asp *******************************************//

var upgrad = new Object();
upgrad.titl=" - �t�@�[���E�F�A �A�b�v�O���[�h";
upgrad.h2="�t�@�[���E�F�A�Ǘ�";
upgrad.legend="�t�@�[���E�F�A �A�b�v�O���[�h";
upgrad.info1="After flashing, reset to";
upgrad.resetOff="���Z�b�g�s��";
upgrad.resetOn="�����ݒ�";
upgrad.file="�A�b�v�O���[�h����t�@�C�����w�肵�Ă�������";
upgrad.warning="W A R N I N G";
upgrad.mess1="�t�@�[���E�F�A�̃A�b�v�O���[�h�ɂ͐���������ꍇ������܂��B<br />���̊ԁA���Z�b�g������d����؂�Ȃ��ł��������I";

//help container
var hupgrad = new Object();
hupgrad.right2="<em>Browse...</em>�{�^�����N���b�N����ƃA�b�v���[�h����t�@�[���E�F�A�t�@�C����I���ł��܂��B<br /><br /><em>Upgrade</em>�{�^���������ƃA�b�v�O���[�h���n�܂�܂��B�A�b�v�O���[�h�����͒��f���Ȃ��ł��������B";



// ******************************************* UPnP.asp *******************************************//

var upnp = new Object();
upnp.titl=" - UPnP";
upnp.h2="Universal Plug and Play (UPnP)";
upnp.legend="�]��";
upnp.legend2="UPnP�ݒ�";
upnp.serv="UPnP�T�[�r�X";
upnp.clear="�J�n���̃|�[�g�]�����N���A";
upnp.url="Send presentation URL";
upnp.msg1="�N���b�N���ē��͂��폜";
upnp.msg2="�S�Ă̓��͂��폜���܂����H";


//help container
var hupnp = new Object();
hupnp.right2="�S�~�����N���b�N����Ɗe���͂��폜�ł��܂��B";
hupnp.right4="�����ŃA�v���P�[�V�������|�[�g�]����ݒ肷�邱�Ƃ������܂��B";



// ******************************************* VPN.asp *******************************************//

var vpn = new Object();
vpn.titl=" - VPN";
vpn.h2="Virtual Private Network (VPN)";
vpn.legend="VPN�p�X�X���[";
vpn.ipsec="IPSec�p�X�X���[";
vpn.pptp="PPTP�p�X�X���[";
vpn.l2tp="L2TP�p�X�X���[";

//help container
var hvpn = new Object();
hvpn.right1="VPN���g���ăl�b�g���[�N�f�o�C�X�Ƃ̒ʐM�������邽�߁AIPSec�APPTP�A���邢��L2TP�p�X�X���[��L���ɂł��܂��B";


// ******************************************* Vlan.asp *******************************************//

var vlan = new Object();
vlan.titl=" - Virtual LAN";
vlan.h2="Virtual Local Area Network (VLAN)";
vlan.legend="VLAN";
vlan.bridge="Assigned To<br />�u���b�W";
vlan.tagged="Tagged";
vlan.negociate="Auto-Negotiate";
vlan.aggregation="Link Aggregation<br>on Ports 3 & 4";
vlan.trunk="Trunk";


// ******************************************* WEP.asp *******************************************//

var wep = new Object();
wep.defkey="�f�t�H���gTransmit Key";
wep.passphrase="�p�X�t���[�Y";



// ******************************************* WOL.asp *******************************************//

var wol = new Object();
wol.titl=" - WOL";
wol.h2="Wake-On-LAN";
wol.legend="�z�X�g�ꗗ";
wol.legend2="WOL�A�h���X";
wol.legend3="�o��";
wol.legend4="�}�j���A��WOL";
wol.enable="WOL��L���ɂ���?";
wol.mac="MAC�A�h���X";
wol.broadcast="�l�b�g�u���[�h�L���X�g";
wol.udp="UDP�|�[�g";
wol.msg1="�N���b�N�����WOL�z�X�g���폜���܂�";

//help container
var hwol = new Object();
hwol.right2="This page allows you to <em>Wake Up</em> hosts on your local network (i.e. locally connected to your router).";
hwol.right4="MAC Addresses are entered in the format xx:xx:xx:xx:xx:xx (i.e. 01:23:45:67:89:AB)";
hwol.right6="IP Address is typically the broadcast address for the local network, but could be a remote address if the target host is not connected to the router's local network."



// ******************************************* WanMAC.asp *******************************************//

var wanmac = new Object();
wanmac.titl=" - MAC�A�h���X�N���[��";
wanmac.h2="MAC�A�h���X�N���[��";
wanmac.legend="MAC�N���[��";
wanmac.wan="�N���[��WAN MAC";
wanmac.wlan="�N���[�� ���C�A���XMAC";

//help container
var hwanmac = new Object();
hwanmac.right2="Some ISPs will require you to register your MAC address. If you do not wish to re-register your MAC address, you can have the router clone the MAC address that is registered with your ISP.";



// ******************************************* WL_WPATable.asp / WPA.asp / Radius.asp *******************************************//

var wpa = new Object();
wpa.titl=" - ���C�A���X �Z�L�����e�B";
wpa.h2="���C�A���X �Z�L�����e�B";
wpa.secmode="�Z�L�����e�B ���[�h";
wpa.legend="���C�A���X �Í���";
wpa.auth_mode="�l�b�g���[�N�F��";
wpa.wpa="WPA";
wpa.radius="Radius";
wpa.gtk_rekey="WPA Group Rekey Interval";
wpa.rekey="Key Renewal Interval (in seconds)";
wpa.radius_ipaddr="RADIUS �T�[�o�[�A�h���X";
wpa.radius_port="RADIUS �T�[�o�[�|�[�g";
wpa.radius_key="RADIUS Key";
wpa.algorithms="WPA�A���S���Y��";
wpa.shared_key="WPA Shared Key";

//help container
var hwpa = new Object();
hwpa.right2="You may choose from Disable, WEP, WPA Pre-Shared Key, WPA RADIUS, or RADIUS. All devices on your network must use the same security mode.";



// ******************************************* WL_FilterTable.asp *******************************************//

var wl_filter = new Object();
wl_filter.titl=" - MAC�A�h���X �t�B���^�[�ꗗ";
wl_filter.h2="MAC�A�h���X �t�B���^�[�ꗗ";
wl_filter.h3="Enter MAC Address in this format&nbsp;:&nbsp;&nbsp;&nbsp;xx:xx:xx:xx:xx:xx";



// ******************************************* WL_ActiveTable.asp *******************************************//

var wl_active = new Object();
wl_active.titl=" - ���C�A���X Active �N���C�A���gMAC�ꗗ";
wl_active.h2="���C�A���X �N���C�A���gMAC�ꗗ";
wl_active.h3="MAC�t�B���^�[��L���ɂ���";
wl_active.active="Active PC";
wl_active.inactive="Inactive PC";



// ******************************************* Wireless_WDS.asp *******************************************//

var wds = new Object();
wds.titl=" - WDS";
wds.h2="Wireless Distribution System";
wds.legend="WDS�ݒ�";
wds.label="Lazy WDS";
wds.label2="WDS�T�u�l�b�g";
wds.wl_mac="���C�A���XMAC";
wds.lazy_default="�����l: ����";
wds.nat1="wLAN->WDS";
wds.nat2="WDS->wLAN";
wds.subnet="�T�u�l�b�g";
wds.legend2="�ǉ��I�v�V����";



// ******************************************* Wireless_radauth.asp *******************************************//

var radius = new Object();
radius.titl=" - Radius";
radius.h2="Remote Authentication Dial-In User Service";
radius.legend="Radius";
radius.label="MAC Radius �N���C�A���g";
radius.label2="MAC �t�H�[�}�b�g";
radius.label3="Radius �T�[�o�[IP";
radius.label4="Radius �T�[�o�[�|�[�g";
radius.label5="�F�؂̂Ȃ��ő僆�[�U�[��";
radius.label6="�p�X���[�h�t�H�[�}�b�g";
radius.label7="RADIUS Shared Secret";
radius.label8="�T�[�o�[�����p�ł��Ȃ��ꍇRadius�𖳌��ɂ���";



// ******************************************* Wireless_MAC.asp *******************************************//

var wl_mac = new Object();
wl_mac.titl=" - MAC �t�B���^�[";
wl_mac.h2="���C�A���X MAC �t�B���^�[";
wl_mac.legend="MAC �t�B���^�[";
wl_mac.label="�t�B���^�[���g�p����";
wl_mac.label2="�t�B���^�[ ���[�h";
wl_mac.deny="�ꗗ��PC�̃��C�A���X�l�b�g���[�N�ւ̐ڑ��������Ȃ��B";
wl_mac.allow="�ꗗ��PC�̃��C�A���X�l�b�g���[�N�ւ̐ڑ���������B";



// ******************************************* Wireless_Basic.asp *******************************************//

var wl_basic = new Object();
wl_basic.titl=" - ���C�A���X";
wl_basic.h2="���C�A���X";
wl_basic.legend="��{�ݒ�";
wl_basic.label="���C�A���X ���[�h";
wl_basic.label2="���C�A���X �l�b�g���[�N ���[�h";
wl_basic.label3="���C�A���X �l�b�g���[�N�� (SSID)";
wl_basic.label4="���C�A���X �`�����l��";
wl_basic.label5="���C�A���X SSID �u���[�h�L���X�g";
wl_basic.label6="Sensitivity Range (ACK Timing)";
wl_basic.ap="AP";
wl_basic.client="�N���C�A���g";
wl_basic.repeater="Repeater";
wl_basic.clientBridge="�N���C�A���g �u���b�W";
wl_basic.adhoc="Adhoc";
wl_basic.mixed="����";
wl_basic.b="B-Only";
wl_basic.a="A-Only";
wl_basic.g="G-Only";

//help container
var hwl_basic = new Object();
hwl_basic.right2="���C�A���XG�N���C�A���g��r���������ꍇ�́A<em>B-Only</em>���[�h��I�����Ă��������B���C�A���X�ɂ��A�N�Z�X�𖳌��ɂ������ꍇ��<em>Disable</em>��I�����Ă�������";
hwl_basic.right3="Sensitivity Range: ";
hwl_basic.right4="ack�^�C�~���O�𒲐����܂��B0��ack�^�C�~���O�����S�ɖ����ɂ��܂��B";



// ******************************************* Wireless_Advanced.asp *******************************************//

var wl_adv = new Object();
wl_adv.titl=" - �A�h�o���X�g ���C�A���X �ݒ�";
wl_adv.h2="�A�h�o���X�g ���C�A���X �ݒ�";
wl_adv.legend="�A�h�o���X�g �ݒ�";
wl_adv.legend2="���C�A���X �}���`���f�B�A �T�|�[�g�ݒ�";
wl_adv.label="�F�؃^�C�v";
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
wl_adv.label12="TX Antenna";
wl_adv.label13="RX Antenna";
wl_adv.label14="Preamble";
wl_adv.reference="Noise Reference";
wl_adv.label15="Xmit Power";
wl_adv.label16="Afterburner";
wl_adv.label17="���C�A���X GUI�A�N�Z�X";
wl_adv.label18="WMM �T�|�[�g";
wl_adv.label19="No-Acknowledgement";
wl_adv.table1="EDCA AP Parameters (AP to Client)";
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
wl_adv.lng="Long";                  //************* don't use .long ! *************
wl_adv.shrt="Short";                //************* don't use .short ! **************

//help container
var hwl_adv = new Object();
hwl_adv.right2="Auto���邢��Shared Key��I�ׂ܂��BShared Key�F�؂̂ق����Z�L�����e�B�������Ȃ�܂����A�l�b�g���[�N��̑S�Ẵf�o�C�X��Sheared Key�F�؂��T�|�[�g���Ă���K�v������܂��B";



// ******************************************* Fail_s.asp / Fail_u_s.asp / Fail.asp *******************************************//

var fail = new Object();
fail.mess1="�s���Ȓl�ł��B������x���͂��Ă��������B";
fail.mess2="�A�b�v�O���[�h���s�B";



// ******************************************* Success*.asp / Reboot.asp  *******************************************//

var success = new Object();
success.saved="�ݒ��ۑ����܂����B";
success.restore="�ݒ�𕜌����܂����B<br/>���u�[�g���J�n���܂��B�������҂���������...";
success.upgrade="�A�b�v�O���[�h���܂����B<br/>���u�[�g���J�n���܂��B�������҂���������...";
success.success_noreboot="�ݒ�ɖ�肠��܂���B";
success.success_reboot=success.success_noreboot + "<br />���u�[�g���J�n���܂��B�������҂���������...";

success.alert_reset="�S�Ă̐ݒ�������l�ɕ������܂����B<br /><br />";
success.alert1="�ڑ�����O�Ɏ��̃`�F�b�N���s���Ă��������F";
success.alert2="���[�^�[��IP�A�h���X��ύX�����ꍇ�A�l�b�g���[�N��̃N���C�A���g�̃A�h���X�������[�X���邢�͍X�V����K�v������܂��B";
success.alert3="WLAN�Őڑ����Ă���ꍇ�A�l�b�g���[�N���W���C�����āA<em>������</em>�������Ă��������B";

// *****************************************************        OLD PAGES       ************************************************************************//
// **************************************************************** DHCPTable.asp **********************************************************************//

var dhcp = new Object();
dhcp.titl=" - DHCP Active IP Table";
dhcp.h2="DHCP Active IP Table";
dhcp.server="DHCP Server IP Address :";
dhcp.tclient="Client Host Name";

var donate = new Object();
donate.mb="You may also donate through the Moneybookers account mb@dd-wrt.com";
