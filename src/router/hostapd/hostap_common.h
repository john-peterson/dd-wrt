#ifndef HOSTAP_COMMON_H
#define HOSTAP_COMMON_H

#define BIT(x) (1 << (x))

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"


#ifndef ETH_P_PAE
#define ETH_P_PAE 0x888E /* Port Access Entity (IEEE 802.1X) */
#endif /* ETH_P_PAE */

#define ETH_P_PREAUTH 0x88C7 /* IEEE 802.11i pre-authentication */



/* IEEE 802.11 defines */

#define WLAN_FC_PVER (BIT(1) | BIT(0))
#define WLAN_FC_TODS BIT(8)
#define WLAN_FC_FROMDS BIT(9)
#define WLAN_FC_MOREFRAG BIT(10)
#define WLAN_FC_RETRY BIT(11)
#define WLAN_FC_PWRMGT BIT(12)
#define WLAN_FC_MOREDATA BIT(13)
#define WLAN_FC_ISWEP BIT(14)
#define WLAN_FC_ORDER BIT(15)

#define WLAN_FC_GET_TYPE(fc) (((fc) & (BIT(3) | BIT(2))) >> 2)
#define WLAN_FC_GET_STYPE(fc) \
	(((fc) & (BIT(7) | BIT(6) | BIT(5) | BIT(4))) >> 4)

#define WLAN_GET_SEQ_FRAG(seq) ((seq) & (BIT(3) | BIT(2) | BIT(1) | BIT(0)))
#define WLAN_GET_SEQ_SEQ(seq) \
	(((seq) & (~(BIT(3) | BIT(2) | BIT(1) | BIT(0)))) >> 4)

#define WLAN_FC_TYPE_MGMT 0
#define WLAN_FC_TYPE_CTRL 1
#define WLAN_FC_TYPE_DATA 2

/* management */
#define WLAN_FC_STYPE_ASSOC_REQ 0
#define WLAN_FC_STYPE_ASSOC_RESP 1
#define WLAN_FC_STYPE_REASSOC_REQ 2
#define WLAN_FC_STYPE_REASSOC_RESP 3
#define WLAN_FC_STYPE_PROBE_REQ 4
#define WLAN_FC_STYPE_PROBE_RESP 5
#define WLAN_FC_STYPE_BEACON 8
#define WLAN_FC_STYPE_ATIM 9
#define WLAN_FC_STYPE_DISASSOC 10
#define WLAN_FC_STYPE_AUTH 11
#define WLAN_FC_STYPE_DEAUTH 12
#define WLAN_FC_STYPE_ACTION 13

/* control */
#define WLAN_FC_STYPE_PSPOLL 10
#define WLAN_FC_STYPE_RTS 11
#define WLAN_FC_STYPE_CTS 12
#define WLAN_FC_STYPE_ACK 13
#define WLAN_FC_STYPE_CFEND 14
#define WLAN_FC_STYPE_CFENDACK 15

/* data */
#define WLAN_FC_STYPE_DATA 0
#define WLAN_FC_STYPE_DATA_CFACK 1
#define WLAN_FC_STYPE_DATA_CFPOLL 2
#define WLAN_FC_STYPE_DATA_CFACKPOLL 3
#define WLAN_FC_STYPE_NULLFUNC 4
#define WLAN_FC_STYPE_CFACK 5
#define WLAN_FC_STYPE_CFPOLL 6
#define WLAN_FC_STYPE_CFACKPOLL 7

/* Authentication algorithms */
#define WLAN_AUTH_OPEN 0
#define WLAN_AUTH_SHARED_KEY 1

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_ESS BIT(0)
#define WLAN_CAPABILITY_IBSS BIT(1)
#define WLAN_CAPABILITY_CF_POLLABLE BIT(2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST BIT(3)
#define WLAN_CAPABILITY_PRIVACY BIT(4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE BIT(5)
#define WLAN_CAPABILITY_PBCC BIT(6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY BIT(7)
#define WLAN_CAPABILITY_SPECTRUM_MGMT BIT(8)
#define WLAN_CAPABILITY_SHORT_SLOT_TIME BIT(10)
#define WLAN_CAPABILITY_DSSS_OFDM BIT(13)

/* Status codes */
#define WLAN_STATUS_SUCCESS 0
#define WLAN_STATUS_UNSPECIFIED_FAILURE 1
#define WLAN_STATUS_CAPS_UNSUPPORTED 10
#define WLAN_STATUS_REASSOC_NO_ASSOC 11
#define WLAN_STATUS_ASSOC_DENIED_UNSPEC 12
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG 13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION 14
#define WLAN_STATUS_CHALLENGE_FAIL 15
#define WLAN_STATUS_AUTH_TIMEOUT 16
#define WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA 17
#define WLAN_STATUS_ASSOC_DENIED_RATES 18
/* 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21
/* IEEE 802.11i */
#define WLAN_STATUS_INVALID_IE 40
#define WLAN_STATUS_GROUP_CIPHER_NOT_VALID 41
#define WLAN_STATUS_PAIRWISE_CIPHER_NOT_VALID 42
#define WLAN_STATUS_AKMP_NOT_VALID 43
#define WLAN_STATUS_UNSUPPORTED_RSN_IE_VERSION 44
#define WLAN_STATUS_INVALID_RSN_IE_CAPAB 45
#define WLAN_STATUS_CIPHER_REJECTED_PER_POLICY 46

/* Reason codes */
#define WLAN_REASON_UNSPECIFIED 1
#define WLAN_REASON_PREV_AUTH_NOT_VALID 2
#define WLAN_REASON_DEAUTH_LEAVING 3
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#define WLAN_REASON_DISASSOC_AP_BUSY 5
#define WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA 6
#define WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA 7
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8
#define WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH 9
/* IEEE 802.11i */
#define WLAN_REASON_INVALID_IE 13
#define WLAN_REASON_MICHAEL_MIC_FAILURE 14
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT 15
#define WLAN_REASON_GROUP_KEY_UPDATE_TIMEOUT 16
#define WLAN_REASON_IE_IN_4WAY_DIFFERS 17
#define WLAN_REASON_GROUP_CIPHER_NOT_VALID 18
#define WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID 19
#define WLAN_REASON_AKMP_NOT_VALID 20
#define WLAN_REASON_UNSUPPORTED_RSN_IE_VERSION 21
#define WLAN_REASON_INVALID_RSN_IE_CAPAB 22
#define WLAN_REASON_IEEE_802_1X_AUTH_FAILED 23
#define WLAN_REASON_CIPHER_SUITE_REJECTED 24


/* Information Element IDs */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_FH_PARAMS 2
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_CHALLENGE 16
#define WLAN_EID_ERP_INFO 42
#define WLAN_EID_RSN 48
#define WLAN_EID_EXT_SUPP_RATES 50
#define WLAN_EID_GENERIC 221
#define WLAN_EID_VENDOR_SPECIFIC 221


/* HFA384X Configuration RIDs */
#define HFA384X_RID_CNFPORTTYPE 0xFC00
#define HFA384X_RID_CNFOWNMACADDR 0xFC01
#define HFA384X_RID_CNFDESIREDSSID 0xFC02
#define HFA384X_RID_CNFOWNCHANNEL 0xFC03
#define HFA384X_RID_CNFOWNSSID 0xFC04
#define HFA384X_RID_CNFOWNATIMWINDOW 0xFC05
#define HFA384X_RID_CNFSYSTEMSCALE 0xFC06
#define HFA384X_RID_CNFMAXDATALEN 0xFC07
#define HFA384X_RID_CNFWDSADDRESS 0xFC08
#define HFA384X_RID_CNFPMENABLED 0xFC09
#define HFA384X_RID_CNFPMEPS 0xFC0A
#define HFA384X_RID_CNFMULTICASTRECEIVE 0xFC0B
#define HFA384X_RID_CNFMAXSLEEPDURATION 0xFC0C
#define HFA384X_RID_CNFPMHOLDOVERDURATION 0xFC0D
#define HFA384X_RID_CNFOWNNAME 0xFC0E
#define HFA384X_RID_CNFOWNDTIMPERIOD 0xFC10
#define HFA384X_RID_CNFWDSADDRESS1 0xFC11 /* AP f/w only */
#define HFA384X_RID_CNFWDSADDRESS2 0xFC12 /* AP f/w only */
#define HFA384X_RID_CNFWDSADDRESS3 0xFC13 /* AP f/w only */
#define HFA384X_RID_CNFWDSADDRESS4 0xFC14 /* AP f/w only */
#define HFA384X_RID_CNFWDSADDRESS5 0xFC15 /* AP f/w only */
#define HFA384X_RID_CNFWDSADDRESS6 0xFC16 /* AP f/w only */
#define HFA384X_RID_CNFMULTICASTPMBUFFERING 0xFC17 /* AP f/w only */
#define HFA384X_RID_UNKNOWN1 0xFC20
#define HFA384X_RID_UNKNOWN2 0xFC21
#define HFA384X_RID_CNFWEPDEFAULTKEYID 0xFC23
#define HFA384X_RID_CNFDEFAULTKEY0 0xFC24
#define HFA384X_RID_CNFDEFAULTKEY1 0xFC25
#define HFA384X_RID_CNFDEFAULTKEY2 0xFC26
#define HFA384X_RID_CNFDEFAULTKEY3 0xFC27
#define HFA384X_RID_CNFWEPFLAGS 0xFC28
#define HFA384X_RID_CNFWEPKEYMAPPINGTABLE 0xFC29
#define HFA384X_RID_CNFAUTHENTICATION 0xFC2A
#define HFA384X_RID_CNFMAXASSOCSTA 0xFC2B /* AP f/w only */
#define HFA384X_RID_CNFTXCONTROL 0xFC2C
#define HFA384X_RID_CNFROAMINGMODE 0xFC2D
#define HFA384X_RID_CNFHOSTAUTHENTICATION 0xFC2E /* AP f/w only */
#define HFA384X_RID_CNFRCVCRCERROR 0xFC30
#define HFA384X_RID_CNFMMLIFE 0xFC31
#define HFA384X_RID_CNFALTRETRYCOUNT 0xFC32
#define HFA384X_RID_CNFBEACONINT 0xFC33
#define HFA384X_RID_CNFAPPCFINFO 0xFC34 /* AP f/w only */
#define HFA384X_RID_CNFSTAPCFINFO 0xFC35
#define HFA384X_RID_CNFPRIORITYQUSAGE 0xFC37
#define HFA384X_RID_CNFTIMCTRL 0xFC40
#define HFA384X_RID_UNKNOWN3 0xFC41 /* added in STA f/w 0.7.x */
#define HFA384X_RID_CNFTHIRTY2TALLY 0xFC42 /* added in STA f/w 0.8.0 */
#define HFA384X_RID_CNFENHSECURITY 0xFC43 /* AP f/w or STA f/w >= 1.6.3 */
#define HFA384X_RID_CNFDBMADJUST 0xFC46 /* added in STA f/w 1.3.1 */
#define HFA384X_RID_GENERICELEMENT 0xFC48 /* added in STA f/w 1.7.0;
					   * write only */
#define HFA384X_RID_PROPAGATIONDELAY 0xFC49 /* added in STA f/w 1.7.6 */
#define HFA384X_RID_GROUPADDRESSES 0xFC80
#define HFA384X_RID_CREATEIBSS 0xFC81
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD 0xFC82
#define HFA384X_RID_RTSTHRESHOLD 0xFC83
#define HFA384X_RID_TXRATECONTROL 0xFC84
#define HFA384X_RID_PROMISCUOUSMODE 0xFC85
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD0 0xFC90 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD1 0xFC91 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD2 0xFC92 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD3 0xFC93 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD4 0xFC94 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD5 0xFC95 /* AP f/w only */
#define HFA384X_RID_FRAGMENTATIONTHRESHOLD6 0xFC96 /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD0 0xFC97 /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD1 0xFC98 /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD2 0xFC99 /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD3 0xFC9A /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD4 0xFC9B /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD5 0xFC9C /* AP f/w only */
#define HFA384X_RID_RTSTHRESHOLD6 0xFC9D /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL0 0xFC9E /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL1 0xFC9F /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL2 0xFCA0 /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL3 0xFCA1 /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL4 0xFCA2 /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL5 0xFCA3 /* AP f/w only */
#define HFA384X_RID_TXRATECONTROL6 0xFCA4 /* AP f/w only */
#define HFA384X_RID_CNFSHORTPREAMBLE 0xFCB0
#define HFA384X_RID_CNFEXCLUDELONGPREAMBLE 0xFCB1
#define HFA384X_RID_CNFAUTHENTICATIONRSPTO 0xFCB2
#define HFA384X_RID_CNFBASICRATES 0xFCB3
#define HFA384X_RID_CNFSUPPORTEDRATES 0xFCB4
#define HFA384X_RID_CNFFALLBACKCTRL 0xFCB5 /* added in STA f/w 1.3.1 */
#define HFA384X_RID_WEPKEYDISABLE 0xFCB6 /* added in STA f/w 1.3.1 */
#define HFA384X_RID_WEPKEYMAPINDEX 0xFCB7 /* ? */
#define HFA384X_RID_BROADCASTKEYID 0xFCB8 /* ? */
#define HFA384X_RID_ENTSECFLAGEYID 0xFCB9 /* ? */
#define HFA384X_RID_CNFPASSIVESCANCTRL 0xFCBA /* added in STA f/w 1.5.0 */
#define HFA384X_RID_SSNHANDLINGMODE 0xFCBB /* added in STA f/w 1.7.0 */
#define HFA384X_RID_MDCCONTROL 0xFCBC /* added in STA f/w 1.7.0 */
#define HFA384X_RID_MDCCOUNTRY 0xFCBD /* added in STA f/w 1.7.0 */
#define HFA384X_RID_TXPOWERMAX 0xFCBE /* added in STA f/w 1.7.0 */
#define HFA384X_RID_CNFLFOENABLED 0xFCBF /* added in STA f/w 1.6.3 */
#define HFA384X_RID_CAPINFO 0xFCC0 /* added in STA f/w 1.7.0 */
#define HFA384X_RID_LISTENINTERVAL 0xFCC1 /* added in STA f/w 1.7.0 */
#define HFA384X_RID_SW_ANT_DIV 0xFCC2 /* added in STA f/w 1.7.0; Prism3 */
#define HFA384X_RID_LED_CTRL 0xFCC4 /* added in STA f/w 1.7.6 */
#define HFA384X_RID_HFODELAY 0xFCC5 /* added in STA f/w 1.7.6 */
#define HFA384X_RID_DISALLOWEDBSSID 0xFCC6 /* added in STA f/w 1.8.0 */
#define HFA384X_RID_TICKTIME 0xFCE0
#define HFA384X_RID_SCANREQUEST 0xFCE1
#define HFA384X_RID_JOINREQUEST 0xFCE2
#define HFA384X_RID_AUTHENTICATESTATION 0xFCE3 /* AP f/w only */
#define HFA384X_RID_CHANNELINFOREQUEST 0xFCE4 /* AP f/w only */
#define HFA384X_RID_HOSTSCAN 0xFCE5 /* added in STA f/w 1.3.1 */

/* HFA384X Information RIDs */
#define HFA384X_RID_MAXLOADTIME 0xFD00
#define HFA384X_RID_DOWNLOADBUFFER 0xFD01
#define HFA384X_RID_PRIID 0xFD02
#define HFA384X_RID_PRISUPRANGE 0xFD03
#define HFA384X_RID_CFIACTRANGES 0xFD04
#define HFA384X_RID_NICSERNUM 0xFD0A
#define HFA384X_RID_NICID 0xFD0B
#define HFA384X_RID_MFISUPRANGE 0xFD0C
#define HFA384X_RID_CFISUPRANGE 0xFD0D
#define HFA384X_RID_CHANNELLIST 0xFD10
#define HFA384X_RID_REGULATORYDOMAINS 0xFD11
#define HFA384X_RID_TEMPTYPE 0xFD12
#define HFA384X_RID_CIS 0xFD13
#define HFA384X_RID_STAID 0xFD20
#define HFA384X_RID_STASUPRANGE 0xFD21
#define HFA384X_RID_MFIACTRANGES 0xFD22
#define HFA384X_RID_CFIACTRANGES2 0xFD23
#define HFA384X_RID_PRODUCTNAME 0xFD24 /* added in STA f/w 1.3.1;
					* only Prism2.5(?) */
#define HFA384X_RID_PORTSTATUS 0xFD40
#define HFA384X_RID_CURRENTSSID 0xFD41
#define HFA384X_RID_CURRENTBSSID 0xFD42
#define HFA384X_RID_COMMSQUALITY 0xFD43
#define HFA384X_RID_CURRENTTXRATE 0xFD44
#define HFA384X_RID_CURRENTBEACONINTERVAL 0xFD45
#define HFA384X_RID_CURRENTSCALETHRESHOLDS 0xFD46
#define HFA384X_RID_PROTOCOLRSPTIME 0xFD47
#define HFA384X_RID_SHORTRETRYLIMIT 0xFD48
#define HFA384X_RID_LONGRETRYLIMIT 0xFD49
#define HFA384X_RID_MAXTRANSMITLIFETIME 0xFD4A
#define HFA384X_RID_MAXRECEIVELIFETIME 0xFD4B
#define HFA384X_RID_CFPOLLABLE 0xFD4C
#define HFA384X_RID_AUTHENTICATIONALGORITHMS 0xFD4D
#define HFA384X_RID_PRIVACYOPTIONIMPLEMENTED 0xFD4F
#define HFA384X_RID_DBMCOMMSQUALITY 0xFD51 /* added in STA f/w 1.3.1 */
#define HFA384X_RID_CURRENTTXRATE1 0xFD80 /* AP f/w only */
#define HFA384X_RID_CURRENTTXRATE2 0xFD81 /* AP f/w only */
#define HFA384X_RID_CURRENTTXRATE3 0xFD82 /* AP f/w only */
#define HFA384X_RID_CURRENTTXRATE4 0xFD83 /* AP f/w only */
#define HFA384X_RID_CURRENTTXRATE5 0xFD84 /* AP f/w only */
#define HFA384X_RID_CURRENTTXRATE6 0xFD85 /* AP f/w only */
#define HFA384X_RID_OWNMACADDR 0xFD86 /* AP f/w only */
#define HFA384X_RID_SCANRESULTSTABLE 0xFD88 /* added in STA f/w 0.8.3 */
#define HFA384X_RID_HOSTSCANRESULTS 0xFD89 /* added in STA f/w 1.3.1 */
#define HFA384X_RID_AUTHENTICATIONUSED 0xFD8A /* added in STA f/w 1.3.4 */
#define HFA384X_RID_CNFFAASWITCHCTRL 0xFD8B /* added in STA f/w 1.6.3 */
#define HFA384X_RID_ASSOCIATIONFAILURE 0xFD8D /* added in STA f/w 1.8.0 */
#define HFA384X_RID_PHYTYPE 0xFDC0
#define HFA384X_RID_CURRENTCHANNEL 0xFDC1
#define HFA384X_RID_CURRENTPOWERSTATE 0xFDC2
#define HFA384X_RID_CCAMODE 0xFDC3
#define HFA384X_RID_SUPPORTEDDATARATES 0xFDC6
#define HFA384X_RID_LFO_VOLT_REG_TEST_RES 0xFDC7 /* added in STA f/w 1.7.1 */
#define HFA384X_RID_BUILDSEQ 0xFFFE
#define HFA384X_RID_FWID 0xFFFF


struct hfa384x_comp_ident
{
	u16 id;
	u16 variant;
	u16 major;
	u16 minor;
} __attribute__ ((packed));

#define HFA384X_COMP_ID_PRI 0x15
#define HFA384X_COMP_ID_STA 0x1f
#define HFA384X_COMP_ID_FW_AP 0x14b

struct hfa384x_sup_range
{
	u16 role;
	u16 id;
	u16 variant;
	u16 bottom;
	u16 top;
} __attribute__ ((packed));


struct hfa384x_build_id
{
	u16 pri_seq;
	u16 sec_seq;
} __attribute__ ((packed));

/* FD01 - Download Buffer */
struct hfa384x_rid_download_buffer
{
	u16 page;
	u16 offset;
	u16 length;
} __attribute__ ((packed));

/* BSS connection quality (RID FD43 range, RID FD51 dBm-normalized) */
struct hfa384x_comms_quality {
	u16 comm_qual; /* 0 .. 92 */
	u16 signal_level; /* 27 .. 154 */
	u16 noise_level; /* 27 .. 154 */
} __attribute__ ((packed));


/* netdevice private ioctls (used, e.g., with iwpriv from user space) */

/* New wireless extensions API - SET/GET convention (even ioctl numbers are
 * root only)
 */
#define PRISM2_IOCTL_PRISM2_PARAM (SIOCIWFIRSTPRIV + 0)
#define PRISM2_IOCTL_GET_PRISM2_PARAM (SIOCIWFIRSTPRIV + 1)
#define PRISM2_IOCTL_WRITEMIF (SIOCIWFIRSTPRIV + 2)
#define PRISM2_IOCTL_READMIF (SIOCIWFIRSTPRIV + 3)
#define PRISM2_IOCTL_MONITOR (SIOCIWFIRSTPRIV + 4)
#define PRISM2_IOCTL_RESET (SIOCIWFIRSTPRIV + 6)
#define PRISM2_IOCTL_INQUIRE (SIOCIWFIRSTPRIV + 8)
#define PRISM2_IOCTL_WDS_ADD (SIOCIWFIRSTPRIV + 10)
#define PRISM2_IOCTL_WDS_DEL (SIOCIWFIRSTPRIV + 12)
#define PRISM2_IOCTL_SET_RID_WORD (SIOCIWFIRSTPRIV + 14)
#define PRISM2_IOCTL_MACCMD (SIOCIWFIRSTPRIV + 16)
#define PRISM2_IOCTL_ADDMAC (SIOCIWFIRSTPRIV + 18)
#define PRISM2_IOCTL_DELMAC (SIOCIWFIRSTPRIV + 20)
#define PRISM2_IOCTL_KICKMAC (SIOCIWFIRSTPRIV + 22)

/* following are not in SIOCGIWPRIV list; check permission in the driver code
 */
#define PRISM2_IOCTL_DOWNLOAD (SIOCDEVPRIVATE + 13)
#define PRISM2_IOCTL_HOSTAPD (SIOCDEVPRIVATE + 14)


/* PRISM2_IOCTL_PRISM2_PARAM ioctl() subtypes: */
enum {
	/* PRISM2_PARAM_PTYPE = 1, */ /* REMOVED 2003-10-22 */
	PRISM2_PARAM_TXRATECTRL = 2,
	PRISM2_PARAM_BEACON_INT = 3,
	PRISM2_PARAM_PSEUDO_IBSS = 4,
	PRISM2_PARAM_ALC = 5,
	/* PRISM2_PARAM_TXPOWER = 6, */ /* REMOVED 2003-10-22 */
	PRISM2_PARAM_DUMP = 7,
	PRISM2_PARAM_OTHER_AP_POLICY = 8,
	PRISM2_PARAM_AP_MAX_INACTIVITY = 9,
	PRISM2_PARAM_AP_BRIDGE_PACKETS = 10,
	PRISM2_PARAM_DTIM_PERIOD = 11,
	PRISM2_PARAM_AP_NULLFUNC_ACK = 12,
	PRISM2_PARAM_MAX_WDS = 13,
	PRISM2_PARAM_AP_AUTOM_AP_WDS = 14,
	PRISM2_PARAM_AP_AUTH_ALGS = 15,
	PRISM2_PARAM_MONITOR_ALLOW_FCSERR = 16,
	PRISM2_PARAM_HOST_ENCRYPT = 17,
	PRISM2_PARAM_HOST_DECRYPT = 18,
	PRISM2_PARAM_BUS_MASTER_THRESHOLD_RX = 19,
	PRISM2_PARAM_BUS_MASTER_THRESHOLD_TX = 20,
	PRISM2_PARAM_HOST_ROAMING = 21,
	PRISM2_PARAM_BCRX_STA_KEY = 22,
	PRISM2_PARAM_IEEE_802_1X = 23,
	PRISM2_PARAM_ANTSEL_TX = 24,
	PRISM2_PARAM_ANTSEL_RX = 25,
	PRISM2_PARAM_MONITOR_TYPE = 26,
	PRISM2_PARAM_WDS_TYPE = 27,
	PRISM2_PARAM_HOSTSCAN = 28,
	PRISM2_PARAM_AP_SCAN = 29,
	PRISM2_PARAM_ENH_SEC = 30,
	PRISM2_PARAM_IO_DEBUG = 31,
	PRISM2_PARAM_BASIC_RATES = 32,
	PRISM2_PARAM_OPER_RATES = 33,
	PRISM2_PARAM_HOSTAPD = 34,
	PRISM2_PARAM_HOSTAPD_STA = 35,
	PRISM2_PARAM_WPA = 36,
	PRISM2_PARAM_PRIVACY_INVOKED = 37,
	PRISM2_PARAM_TKIP_COUNTERMEASURES = 38,
	PRISM2_PARAM_DROP_UNENCRYPTED = 39,
	PRISM2_PARAM_SCAN_CHANNEL_MASK = 40,
};

enum { HOSTAP_ANTSEL_DO_NOT_TOUCH = 0, HOSTAP_ANTSEL_DIVERSITY = 1,
       HOSTAP_ANTSEL_LOW = 2, HOSTAP_ANTSEL_HIGH = 3 };


/* PRISM2_IOCTL_MACCMD ioctl() subcommands: */
enum { AP_MAC_CMD_POLICY_OPEN = 0, AP_MAC_CMD_POLICY_ALLOW = 1,
       AP_MAC_CMD_POLICY_DENY = 2, AP_MAC_CMD_FLUSH = 3,
       AP_MAC_CMD_KICKALL = 4 };


/* PRISM2_IOCTL_DOWNLOAD ioctl() dl_cmd: */
enum {
	PRISM2_DOWNLOAD_VOLATILE = 1 /* RAM */,
	/* Note! Old versions of prism2_srec have a fatal error in CRC-16
	 * calculation, which will corrupt all non-volatile downloads.
	 * PRISM2_DOWNLOAD_NON_VOLATILE used to be 2, but it is now 3 to
	 * prevent use of old versions of prism2_srec for non-volatile
	 * download. */
	PRISM2_DOWNLOAD_NON_VOLATILE = 3 /* FLASH */,
	PRISM2_DOWNLOAD_VOLATILE_GENESIS = 4 /* RAM in Genesis mode */,
	/* Persistent versions of volatile download commands (keep firmware
	 * data in memory and automatically re-download after hw_reset */
	PRISM2_DOWNLOAD_VOLATILE_PERSISTENT = 5,
	PRISM2_DOWNLOAD_VOLATILE_GENESIS_PERSISTENT = 6,
};

struct prism2_download_param {
	u32 dl_cmd;
	u32 start_addr;
	u32 num_areas;
	struct prism2_download_area {
		u32 addr; /* wlan card address */
		u32 len;
		caddr_t ptr; /* pointer to data in user space */
	} data[0];
};

#define PRISM2_MAX_DOWNLOAD_AREA_LEN 131072
#define PRISM2_MAX_DOWNLOAD_LEN 262144


/* PRISM2_IOCTL_HOSTAPD ioctl() cmd: */
enum {
	PRISM2_HOSTAPD_FLUSH = 1,
	PRISM2_HOSTAPD_ADD_STA = 2,
	PRISM2_HOSTAPD_REMOVE_STA = 3,
	PRISM2_HOSTAPD_GET_INFO_STA = 4,
	/* REMOVED: PRISM2_HOSTAPD_RESET_TXEXC_STA = 5, */
	PRISM2_SET_ENCRYPTION = 6,
	PRISM2_GET_ENCRYPTION = 7,
	PRISM2_HOSTAPD_SET_FLAGS_STA = 8,
	PRISM2_HOSTAPD_GET_RID = 9,
	PRISM2_HOSTAPD_SET_RID = 10,
	PRISM2_HOSTAPD_SET_ASSOC_AP_ADDR = 11,
	PRISM2_HOSTAPD_SET_GENERIC_ELEMENT = 12,
	PRISM2_HOSTAPD_MLME = 13,
	PRISM2_HOSTAPD_SCAN_REQ = 14,
	PRISM2_HOSTAPD_STA_CLEAR_STATS = 15,
};

#define PRISM2_HOSTAPD_MAX_BUF_SIZE 1024
#define PRISM2_HOSTAPD_RID_HDR_LEN \
((int) (&((struct prism2_hostapd_param *) 0)->u.rid.data))
#define PRISM2_HOSTAPD_GENERIC_ELEMENT_HDR_LEN \
((int) (&((struct prism2_hostapd_param *) 0)->u.generic_elem.data))

/* Maximum length for algorithm names (-1 for nul termination) used in ioctl()
 */
#define HOSTAP_CRYPT_ALG_NAME_LEN 16


struct prism2_hostapd_param {
	u32 cmd;
	u8 sta_addr[ETH_ALEN];
	union {
		struct {
			u16 aid;
			u16 capability;
			u8 tx_supp_rates;
		} add_sta;
		struct {
			u32 inactive_sec;
		} get_info_sta;
		struct {
			u8 alg[HOSTAP_CRYPT_ALG_NAME_LEN];
			u32 flags;
			u32 err;
			u8 idx;
			u8 seq[8]; /* sequence counter (set: RX, get: TX) */
			u16 key_len;
			u8 key[0];
		} crypt;
		struct {
			u32 flags_and;
			u32 flags_or;
		} set_flags_sta;
		struct {
			u16 rid;
			u16 len;
			u8 data[0];
		} rid;
		struct {
			u8 len;
			u8 data[0];
		} generic_elem;
		struct {
#define MLME_STA_DEAUTH 0
#define MLME_STA_DISASSOC 1
			u16 cmd;
			u16 reason_code;
		} mlme;
		struct {
			u8 ssid_len;
			u8 ssid[32];
		} scan_req;
	} u;
};

#define HOSTAP_CRYPT_FLAG_SET_TX_KEY BIT(0)
#define HOSTAP_CRYPT_FLAG_PERMANENT BIT(1)

#define HOSTAP_CRYPT_ERR_UNKNOWN_ALG 2
#define HOSTAP_CRYPT_ERR_UNKNOWN_ADDR 3
#define HOSTAP_CRYPT_ERR_CRYPT_INIT_FAILED 4
#define HOSTAP_CRYPT_ERR_KEY_SET_FAILED 5
#define HOSTAP_CRYPT_ERR_TX_KEY_SET_FAILED 6
#define HOSTAP_CRYPT_ERR_CARD_CONF_FAILED 7


#endif /* HOSTAP_COMMON_H */
