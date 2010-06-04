#ifdef HAVE_MADWIFI_MIMO
#define VISUALSOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/statfs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <broadcom.h>
#include <cymac.h>
#include <wlutils.h>
#include <bcmparams.h>
#include <dirent.h>
#include <netdb.h>
#include <utils.h>
#include <wlutils.h>
#include <bcmnvram.h>

#include "wireless_generic.c"

#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>
#include <err.h>

#include "wireless_copy.h"
#include "../../madwifi.dev/madwifi_mimo.dev/core/net80211/ieee80211.h"
#include "../../madwifi.dev/madwifi_mimo.dev/core/net80211/ieee80211_crypto.h"
#include "../../madwifi.dev/madwifi_mimo.dev/core/net80211/ieee80211_ioctl.h"
static const char *ieee80211_ntoa(const uint8_t mac[IEEE80211_ADDR_LEN])
{
	static char a[18];
	int i;

	i = snprintf(a, sizeof(a), "%02x:%02x:%02x:%02x:%02x:%02x",
		     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return (i < 17 ? NULL : a);
}

int
ej_active_wireless_if_11n(webs_t wp, int argc, char_t ** argv,
			  char *ifname, int cnt, int turbo, int macmask)
{
	// unsigned char buf[24 * 1024];

	unsigned char *cp;
	int s, len;
	struct iwreq iwr;
	char nb[32];
	sprintf(nb, "%s_bias", ifname);
	int bias = atoi(nvram_default_get(nb, "0"));
	if (!ifexists(ifname)) {
		printf("IOCTL_STA_INFO ifresolv %s failed!\n", ifname);
		return cnt;
	}
	int state = get_radiostate(ifname);

	if (state == 0 || state == -1) {
		printf("IOCTL_STA_INFO radio %s not enabled!\n", ifname);
		return cnt;
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		fprintf(stderr, "socket(SOCK_DRAGM)\n");
		return cnt;
	}
	(void)memset(&iwr, 0, sizeof(struct iwreq));
	(void)strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	unsigned char *buf = (unsigned char *)malloc(24 * 1024);

	iwr.u.data.pointer = (void *)buf;
	iwr.u.data.length = 24 * 1024;
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0) {
		fprintf(stderr, "IOCTL_STA_INFO for %s failed!\n", ifname);
		free(buf);
		close(s);
		return cnt;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info)) {
		// fprintf(stderr,"IOCTL_STA_INFO len<struct %s failed!\n",ifname);
		free(buf);
		close(s);
		return cnt;
	}
	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		uint8_t *vp;

		si = (struct ieee80211req_sta_info *)cp;
		vp = (u_int8_t *)(si + 1);

		if (cnt)
			websWrite(wp, ",");
		cnt++;
		char mac[32];

		strcpy(mac, ieee80211_ntoa(si->isi_macaddr));
		if (nvram_match("maskmac", "1") && macmask) {
			mac[0] = 'x';
			mac[1] = 'x';
			mac[3] = 'x';
			mac[4] = 'x';
			mac[6] = 'x';
			mac[7] = 'x';
			mac[9] = 'x';
			mac[10] = 'x';
		}
		if (si->isi_noise == 0) {
			si->isi_noise = -95;
		}
		int qual = (si->isi_noise + si->isi_rssi) * 124 + 11600;
		qual /= 10;
		int rxrate = si->isi_rxrateKbps / 1000;
		int txrate = si->isi_txrateKbps / 1000;
		if (!rxrate)
			rxrate =
			    si->isi_rates[si->isi_rxrate] & IEEE80211_RATE_VAL;
		if (!txrate)
			txrate =
			    si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL;

		char rx[32];
		char tx[32];
		if (rxrate)
			sprintf(rx, "%3dM", rxrate);
		else
			sprintf(rx, "N/A");
		if (txrate)
			sprintf(tx, "%3dM", txrate);
		else
			sprintf(tx, "N/A");
		websWrite(wp,
			  "'%s','%s','%s','%s','%s','%d','%d','%d','%d'",
			  mac, ifname, UPTIME(si->isi_uptime),
			  tx, rx,
			  si->isi_noise + si->isi_rssi + bias,
			  si->isi_noise + bias, si->isi_rssi, qual);
		cp += si->isi_len;
		len -= si->isi_len;
	}
	while (len >= sizeof(struct ieee80211req_sta_info));
	free(buf);
	close(s);

	return cnt;
}

#endif
