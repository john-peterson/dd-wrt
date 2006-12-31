/* atmdev.h - ATM device driver declarations and various related items */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 

#ifndef LINUX_ATMDEV_H
#define LINUX_ATMDEV_H


#include <linux/config.h>
#include <linux/atmapi.h>
#include <linux/atm.h>
#include <linux/atmioc.h>


#define ESI_LEN		6

#define ATM_OC3_PCR	(155520000/270*260/8/53)
			/* OC3 link rate:  155520000 bps
			   SONET overhead: /270*260 (9 section, 1 path)
			   bits per cell:  /8/53
			   max cell rate:  353207.547 cells/sec */
#define ATM_25_PCR	((25600000/8-8000)/54)
			/* 25 Mbps ATM cell rate (59111) */
#define ATM_OC12_PCR	(622080000/1080*1040/8/53)
			/* OC12 link rate: 622080000 bps
			   SONET overhead: /1080*1040
			   bits per cell:  /8/53
			   max cell rate:  1412830.188 cells/sec */
#define ATM_DS3_PCR	(8000*12)
			/* DS3: 12 cells in a 125 usec time slot */

#define ATM_PDU_OVHD	0	/* number of bytes to charge against buffer
				   quota per PDU */

#define ATM_SD(s)	((s)->sk->protinfo.af_atm)


#define __AAL_STAT_ITEMS \
    __HANDLE_ITEM(tx);			/* TX okay */ \
    __HANDLE_ITEM(tx_err);		/* TX errors */ \
    __HANDLE_ITEM(rx);			/* RX okay */ \
    __HANDLE_ITEM(rx_err);		/* RX errors */ \
    __HANDLE_ITEM(rx_drop);		/* RX out of memory */

struct atm_aal_stats {
#define __HANDLE_ITEM(i) int i
	__AAL_STAT_ITEMS
#undef __HANDLE_ITEM
};


struct atm_dev_stats {
	struct atm_aal_stats aal0;
	struct atm_aal_stats aal34;
	struct atm_aal_stats aal5;
} __ATM_API_ALIGN;


#define ATM_GETLINKRATE	_IOW('a',ATMIOC_ITF+1,struct atmif_sioc)
					/* get link rate */
#define ATM_GETNAMES	_IOW('a',ATMIOC_ITF+3,struct atm_iobuf)
					/* get interface names (numbers) */
#define ATM_GETTYPE	_IOW('a',ATMIOC_ITF+4,struct atmif_sioc)
					/* get interface type name */
#define ATM_GETESI	_IOW('a',ATMIOC_ITF+5,struct atmif_sioc)
					/* get interface ESI */
#define ATM_GETADDR	_IOW('a',ATMIOC_ITF+6,struct atmif_sioc)
					/* get itf's local ATM addr. list */
#define ATM_RSTADDR	_IOW('a',ATMIOC_ITF+7,struct atmif_sioc)
					/* reset itf's ATM address list */
#define ATM_ADDADDR	_IOW('a',ATMIOC_ITF+8,struct atmif_sioc)
					/* add a local ATM address */
#define ATM_DELADDR	_IOW('a',ATMIOC_ITF+9,struct atmif_sioc)
					/* remove a local ATM address */
#define ATM_GETCIRANGE	_IOW('a',ATMIOC_ITF+10,struct atmif_sioc)
					/* get connection identifier range */
#define ATM_SETCIRANGE	_IOW('a',ATMIOC_ITF+11,struct atmif_sioc)
					/* set connection identifier range */
#define ATM_SETESI	_IOW('a',ATMIOC_ITF+12,struct atmif_sioc)
					/* set interface ESI */
#define ATM_SETESIF	_IOW('a',ATMIOC_ITF+13,struct atmif_sioc)
					/* force interface ESI */
#define ATM_GETSTAT	_IOW('a',ATMIOC_SARCOM+0,struct atmif_sioc)
					/* get AAL layer statistics */
#define ATM_GETSTATZ	_IOW('a',ATMIOC_SARCOM+1,struct atmif_sioc)
					/* get AAL layer statistics and zero */
#define ATM_GETLOOP	_IOW('a',ATMIOC_SARCOM+2,struct atmif_sioc)
					/* get loopback mode */
#define ATM_SETLOOP	_IOW('a',ATMIOC_SARCOM+3,struct atmif_sioc)
					/* set loopback mode */
#define ATM_QUERYLOOP	_IOW('a',ATMIOC_SARCOM+4,struct atmif_sioc)
					/* query supported loopback modes */
#define ATM_SETSC	_IOW('a',ATMIOC_SPECIAL+1,int)
					/* enable or disable single-copy */
#define ATM_SETBACKEND	_IOW('a',ATMIOC_SPECIAL+2,atm_backend_t)
					/* set backend handler */
#ifdef CONFIG_MIPS_BRCM
#define ATM_NEWBACKENDIF _IOW('a',ATMIOC_SPECIAL+3,atm_backend_t)
                                       /* use backend to make new if */
#endif

/*
 * These are backend handkers that can be set via the ATM_SETBACKEND call
 * above.  In the future we may support dynamic loading of these - for now,
 * they're just being used to share the ATMIOC_BACKEND ioctls
 */
#define ATM_BACKEND_RAW		0	
#define ATM_BACKEND_PPP		1	/* PPPoATM - RFC2364 */
#define ATM_BACKEND_BR_2684	2	/* Bridged RFC1483/2684 */
#ifdef CONFIG_MIPS_BRCM
#define ATM_BACKEND_RT2684	3	/* Routed RFC1483/2684 */
#endif

/* for ATM_GETTYPE */
#define ATM_ITFTYP_LEN	8	/* maximum length of interface type name */

/*
 * Loopback modes for ATM_{PHY,SAR}_{GET,SET}LOOP
 */

/* Point of loopback				CPU-->SAR-->PHY-->line--> ... */
#define __ATM_LM_NONE	0	/* no loop back     ^     ^     ^      ^      */
#define __ATM_LM_AAL	1	/* loop back PDUs --'     |     |      |      */
#define __ATM_LM_ATM	2	/* loop back ATM cells ---'     |      |      */
/* RESERVED		4	loop back on PHY side  ---'		      */
#define __ATM_LM_PHY	8	/* loop back bits (digital) ----'      |      */
#define __ATM_LM_ANALOG 16	/* loop back the analog signal --------'      */

/* Direction of loopback */
#define __ATM_LM_MKLOC(n)	((n))	    /* Local (i.e. loop TX to RX) */
#define __ATM_LM_MKRMT(n)	((n) << 8)  /* Remote (i.e. loop RX to TX) */

#define __ATM_LM_XTLOC(n)	((n) & 0xff)
#define __ATM_LM_XTRMT(n)	(((n) >> 8) & 0xff)

#define ATM_LM_NONE	0	/* no loopback */

#define ATM_LM_LOC_AAL	__ATM_LM_MKLOC(__ATM_LM_AAL)
#define ATM_LM_LOC_ATM	__ATM_LM_MKLOC(__ATM_LM_ATM)
#define ATM_LM_LOC_PHY	__ATM_LM_MKLOC(__ATM_LM_PHY)
#define ATM_LM_LOC_ANALOG __ATM_LM_MKLOC(__ATM_LM_ANALOG)

#define ATM_LM_RMT_AAL	__ATM_LM_MKRMT(__ATM_LM_AAL)
#define ATM_LM_RMT_ATM	__ATM_LM_MKRMT(__ATM_LM_ATM)
#define ATM_LM_RMT_PHY	__ATM_LM_MKRMT(__ATM_LM_PHY)
#define ATM_LM_RMT_ANALOG __ATM_LM_MKRMT(__ATM_LM_ANALOG)

/*
 * Note: ATM_LM_LOC_* and ATM_LM_RMT_* can be combined, provided that
 * __ATM_LM_XTLOC(x) <= __ATM_LM_XTRMT(x)
 */


struct atm_iobuf {
	int length;
	void *buffer;
};

/* for ATM_GETCIRANGE / ATM_SETCIRANGE */

#define ATM_CI_MAX      -1              /* use maximum range of VPI/VCI */
 
struct atm_cirange {
	char	vpi_bits;		/* 1..8, ATM_CI_MAX (-1) for maximum */
	char	vci_bits;		/* 1..16, ATM_CI_MAX (-1) for maximum */
};

/* for ATM_SETSC; actually taken from the ATM_VF number space */

#define ATM_SC_RX	1024		/* enable RX single-copy */
#define ATM_SC_TX	2048		/* enable TX single-copy */

#define ATM_BACKLOG_DEFAULT 32 /* if we get more, we're likely to time out
				  anyway */

/* MF: change_qos (Modify) flags */

#define ATM_MF_IMMED	 1	/* Block until change is effective */
#define ATM_MF_INC_RSV	 2	/* Change reservation on increase */
#define ATM_MF_INC_SHP	 4	/* Change shaping on increase */
#define ATM_MF_DEC_RSV	 8	/* Change reservation on decrease */
#define ATM_MF_DEC_SHP	16	/* Change shaping on decrease */
#define ATM_MF_BWD	32	/* Set the backward direction parameters */

#define ATM_MF_SET	(ATM_MF_INC_RSV | ATM_MF_INC_SHP | ATM_MF_DEC_RSV | \
			  ATM_MF_DEC_SHP | ATM_MF_BWD)

/*
 * ATM_VS_* are used to express VC state in a human-friendly way.
 */

#define ATM_VS_IDLE	0	/* VC is not used */
#define ATM_VS_CONNECTED 1	/* VC is connected */
#define ATM_VS_CLOSING	2	/* VC is closing */
#define ATM_VS_LISTEN	3	/* VC is listening for incoming setups */
#define ATM_VS_INUSE	4	/* VC is in use (registered with atmsigd) */
#define ATM_VS_BOUND	5	/* VC is bound */

#define ATM_VS2TXT_MAP \
    "IDLE", "CONNECTED", "CLOSING", "LISTEN", "INUSE", "BOUND"

#define ATM_VF2TXT_MAP \
    "ADDR",	"READY",	"PARTIAL",	"REGIS", \
    "RELEASED", "HASQOS",	"LISTEN",	"META", \
    "256",	"512",		"1024",		"2048", \
    "SESSION",	"HASSAP",	"BOUND",	"CLOSE"


#ifndef __KERNEL__
#undef __AAL_STAT_ITEMS
#else

#include <linux/sched.h> /* wait_queue_head_t */
#include <linux/time.h> /* struct timeval */
#include <linux/net.h>
#include <linux/skbuff.h> /* struct sk_buff */
#include <linux/uio.h>
#include <net/sock.h>
#include <asm/atomic.h>

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif


struct k_atm_aal_stats {
#define __HANDLE_ITEM(i) atomic_t i
	__AAL_STAT_ITEMS
#undef __HANDLE_ITEM
};


struct k_atm_dev_stats {
	struct k_atm_aal_stats aal0;
	struct k_atm_aal_stats aal34;
	struct k_atm_aal_stats aal5;
};


enum {
	ATM_VF_ADDR,		/* Address is in use. Set by anybody, cleared
				   by device driver. */
	ATM_VF_READY,		/* VC is ready to transfer data. Set by device
				   driver, cleared by anybody. */
	ATM_VF_PARTIAL,		/* resources are bound to PVC (partial PVC
				   setup), controlled by socket layer */
	ATM_VF_REGIS,		/* registered with demon, controlled by SVC
				   socket layer */
	ATM_VF_BOUND,		/* local SAP is set, controlled by SVC socket
				   layer */
	ATM_VF_RELEASED,	/* demon has indicated/requested release,
				   controlled by SVC socket layer */
	ATM_VF_HASQOS,		/* QOS parameters have been set */
	ATM_VF_LISTEN,		/* socket is used for listening */
	ATM_VF_META,		/* SVC socket isn't used for normal data
				   traffic and doesn't depend on signaling
				   to be available */
	ATM_VF_SESSION,		/* VCC is p2mp session control descriptor */
	ATM_VF_HASSAP,		/* SAP has been set */
	ATM_VF_CLOSE,		/* asynchronous close - treat like VF_RELEASED*/
};


#define ATM_VF2VS(flags) \
    (test_bit(ATM_VF_READY,&(flags)) ? ATM_VS_CONNECTED : \
     test_bit(ATM_VF_RELEASED,&(flags)) ? ATM_VS_CLOSING : \
     test_bit(ATM_VF_LISTEN,&(flags)) ? ATM_VS_LISTEN : \
     test_bit(ATM_VF_REGIS,&(flags)) ? ATM_VS_INUSE : \
     test_bit(ATM_VF_BOUND,&(flags)) ? ATM_VS_BOUND : ATM_VS_IDLE)


enum {
	ATM_DF_CLOSE,		/* close device when last VCC is closed */
};


#define ATM_PHY_SIG_LOST    0	/* no carrier/light */
#define ATM_PHY_SIG_UNKNOWN 1	/* carrier/light status is unknown */
#define ATM_PHY_SIG_FOUND   2	/* carrier/light okay */

#define ATM_ATMOPT_CLP	1	/* set CLP bit */


typedef struct { unsigned long bits; } atm_vcc_flags_t;


struct atm_vcc {
	atm_vcc_flags_t flags;		/* VCC flags (ATM_VF_*) */
	unsigned char	family;		/* address family; 0 if unused */
	short		vpi;		/* VPI and VCI (types must be equal */
					/* with sockaddr) */
	int 		vci;
	unsigned long	aal_options;	/* AAL layer options */
	unsigned long	atm_options;	/* ATM layer options */
	struct atm_dev	*dev;		/* device back pointer */
	struct atm_qos	qos;		/* QOS */
	struct atm_sap	sap;		/* SAP */
	atomic_t	tx_inuse,rx_inuse; /* buffer space in use */
	void (*push)(struct atm_vcc *vcc,struct sk_buff *skb);
	void (*pop)(struct atm_vcc *vcc,struct sk_buff *skb); /* optional */
	struct sk_buff *(*alloc_tx)(struct atm_vcc *vcc,unsigned int size);
					/* TX allocation routine - can be */
					/* modified by protocol or by driver.*/
					/* NOTE: this interface will change */
	int (*push_oam)(struct atm_vcc *vcc,void *cell);
	int (*send)(struct atm_vcc *vcc,struct sk_buff *skb);
	void		*dev_data;	/* per-device data */
	void		*proto_data;	/* per-protocol data */
	struct timeval	timestamp;	/* AAL timestamps */
	struct sk_buff_head recvq;	/* receive queue */
	struct k_atm_aal_stats *stats;	/* pointer to AAL stats group */
	wait_queue_head_t sleep;	/* if socket is busy */
	struct sock	*sk;		/* socket backpointer */
	struct atm_vcc	*prev,*next;
	/* SVC part --- may move later ------------------------------------- */
	short		itf;		/* interface number */
	struct sockaddr_atmsvc local;
	struct sockaddr_atmsvc remote;
	void (*callback)(struct atm_vcc *vcc);
	struct sk_buff_head listenq;
	int		backlog_quota;	/* number of connection requests we */
					/* can still accept */
	int		reply;		/* also used by ATMTCP */
	/* Multipoint part ------------------------------------------------- */
	struct atm_vcc	*session;	/* session VCC descriptor */
	/* Other stuff ----------------------------------------------------- */
	void		*user_back;	/* user backlink - not touched by */
					/* native ATM stack. Currently used */
					/* by CLIP and sch_atm. */
};


struct atm_dev_addr {
	struct sockaddr_atmsvc addr;	/* ATM address */
	struct atm_dev_addr *next;	/* next address */
};


typedef struct { unsigned int bits; } atm_dev_flags_t;


struct atm_dev {
	const struct atmdev_ops *ops;	/* device operations; NULL if unused */
	const struct atmphy_ops *phy;	/* PHY operations, may be undefined */
					/* (NULL) */
	const char	*type;		/* device type name */
	int		number;		/* device index */
	struct atm_vcc	*vccs;		/* VCC table (or NULL) */
	struct atm_vcc	*last;		/* last VCC (or undefined) */
	void		*dev_data;	/* per-device data */
	void		*phy_data;	/* private PHY date */
	atm_dev_flags_t flags;		/* device flags (ATM_DF_*) */
	struct atm_dev_addr *local;	/* local ATM addresses */
	unsigned char	esi[ESI_LEN];	/* ESI ("MAC" addr) */
	struct atm_cirange ci_range;	/* VPI/VCI range */
	struct k_atm_dev_stats stats;	/* statistics */
	char		signal;		/* signal status (ATM_PHY_SIG_*) */
	int		link_rate;	/* link rate (default: OC3) */
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *proc_entry; /* proc entry */
	char *proc_name;		/* proc entry name */
#endif
	struct atm_dev	*prev,*next;	/* linkage */
};


/*
 * ioctl, getsockopt, setsockopt, and sg_send are optional and can be set to
 * NULL. */

/* OF: send_Oam Flags */

#define ATM_OF_IMMED	1	/* Attempt immediate delivery */
#define ATM_OF_INRATE	2	/* Attempt in-rate delivery */

struct atmdev_ops { /* only send is required */
	void (*dev_close)(struct atm_dev *dev);
	int (*open)(struct atm_vcc *vcc,short vpi,int vci);
	void (*close)(struct atm_vcc *vcc);
	int (*ioctl)(struct atm_dev *dev,unsigned int cmd,void *arg);
	int (*getsockopt)(struct atm_vcc *vcc,int level,int optname,
	    void *optval,int optlen);
	int (*setsockopt)(struct atm_vcc *vcc,int level,int optname,
	    void *optval,int optlen);
	int (*send)(struct atm_vcc *vcc,struct sk_buff *skb);
	int (*sg_send)(struct atm_vcc *vcc,unsigned long start,
	    unsigned long size);
#if 0 /* keep the current hack for now */
	int (*send_iovec)(struct atm_vcc *vcc,struct iovec *iov,int size,
	    void (*discard)(struct atm_vcc *vcc,void *user),void *user);
#endif
	int (*send_oam)(struct atm_vcc *vcc,void *cell,int flags);
	void (*phy_put)(struct atm_dev *dev,unsigned char value,
	    unsigned long addr);
	unsigned char (*phy_get)(struct atm_dev *dev,unsigned long addr);
	void (*feedback)(struct atm_vcc *vcc,struct sk_buff *skb,
	    unsigned long start,unsigned long dest,int len);
	int (*change_qos)(struct atm_vcc *vcc,struct atm_qos *qos,int flags);
	void (*free_rx_skb)(struct atm_vcc *vcc, struct sk_buff *skb);
		/* @@@ temporary hack */
	int (*proc_read)(struct atm_dev *dev,loff_t *pos,char *page);
	struct module *owner;
};


struct atmphy_ops {
	int (*start)(struct atm_dev *dev);
	int (*ioctl)(struct atm_dev *dev,unsigned int cmd,void *arg);
	void (*interrupt)(struct atm_dev *dev);
	int (*stop)(struct atm_dev *dev);
};

struct atm_skb_data {
	struct atm_vcc	*vcc;		/* ATM VCC */
	int		iovcnt;		/* 0 for "normal" operation */
	unsigned long	atm_options;	/* ATM layer options */
};

#define ATM_SKB(skb) (((struct atm_skb_data *) (skb)->cb))

struct atm_dev *atm_dev_register(const char *type,const struct atmdev_ops *ops,
    int number,atm_dev_flags_t *flags); /* number == -1: pick first available */
struct atm_dev *atm_find_dev(int number);
void atm_dev_deregister(struct atm_dev *dev);
void shutdown_atm_dev(struct atm_dev *dev);
void bind_vcc(struct atm_vcc *vcc,struct atm_dev *dev);


/*
 * This is approximately the algorithm used by alloc_skb.
 *
 */

static __inline__ int atm_guess_pdu2truesize(int pdu_size)
{
	return ((pdu_size+15) & ~15) + sizeof(struct sk_buff);
}


static __inline__ void atm_force_charge(struct atm_vcc *vcc,int truesize)
{
	atomic_add(truesize+ATM_PDU_OVHD,&vcc->rx_inuse);
}


static __inline__ void atm_return(struct atm_vcc *vcc,int truesize)
{
	atomic_sub(truesize+ATM_PDU_OVHD,&vcc->rx_inuse);
}


static __inline__ int atm_may_send(struct atm_vcc *vcc,unsigned int size)
{
	return size+atomic_read(&vcc->tx_inuse)+ATM_PDU_OVHD < vcc->sk->sndbuf;
}


int atm_charge(struct atm_vcc *vcc,int truesize);
struct sk_buff *atm_alloc_charge(struct atm_vcc *vcc,int pdu_size,
    int gfp_flags);
int atm_find_ci(struct atm_vcc *vcc,short *vpi,int *vci);
int atm_pcr_goal(struct atm_trafprm *tp);

void atm_async_release_vcc(struct atm_vcc *vcc,int reply);

#endif /* __KERNEL__ */

#endif
