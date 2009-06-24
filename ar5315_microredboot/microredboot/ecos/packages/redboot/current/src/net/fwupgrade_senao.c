/*
firmware upgrade code for senao webflash images
*/

#include <redboot.h>
#include <cyg/io/flash.h>
#include <fis.h>
#include <flash_config.h>
#include "fwupgrade.h"

/* some variables from flash.c */
extern void *flash_start, *flash_end;
extern int flash_block_size, flash_num_blocks;
#ifdef CYGOPT_REDBOOT_FIS
extern void *fis_work_block;
extern void *fis_addr;
extern int fisdir_size;		// Size of FIS directory.
#endif
//extern void _show_invalid_flash_address(CYG_ADDRESS flash_addr, int stat); 
extern void fis_update_directory(void);

//#define TRACE diag_printf("DBG: %s:%d\n", __FUNCTION__, __LINE__)

#define TRACE


extern void fis_init(int argc, char *argv[], int force);

int fw_check_image_senao(unsigned char *addr, unsigned long maxlen,
			 int do_flash)
{
	unsigned char *base = (unsigned char *)addr+10;
	if (strncmp(addr,"AP51-3660",9)) {
		diag_printf("SENAO_FW: bad header\n");
		return -1;
	}

	if (do_flash) {
		char *arg[] = { "fis", "init" };
		fis_init(2, arg, 1);
		void *err_addr;
		flash_read(fis_addr, fis_work_block, fisdir_size,
			   (void **)&err_addr);
		struct fis_image_desc *img = NULL;
		int i, stat;
		img = fis_lookup("RedBoot", &i);
		if (i != 0) {
			diag_printf
			    ("SENAO_FW: RedBoot partition is not the first partition\n");
			return -1;
		}
		unsigned int flash_addr = img->flash_base + img->size;
		if ((stat =
		     flash_erase((void *)flash_addr, maxlen,
				 (void **)&err_addr)) != 0) {
			diag_printf("SENAO_FW: Can't erase region at %p: %s\n",
				    err_addr, flash_errmsg(stat));
			return -1;
		}
		if ((stat =
		     flash_program((void *)flash_addr,
				   (void *)(base),
				   maxlen, (void **)&err_addr)) != 0) {
			diag_printf
			    ("SENAO_FW: Can't program region at %p: %s\n",
			     err_addr, flash_errmsg(stat));
			return -1;
		}
		img = (struct fis_image_desc *)fis_work_block;
			for (i = 0; i < fisdir_size / sizeof(*img); i++, img++) {
				if (img->name[0] == (unsigned char)0xFF) {
					break;
				}
			}
			
		unsigned int filesyssize = 0x3f0000;
		unsigned int linuxsize = 0xa0000;
		unsigned int cfgsize = 0x20000;
		if (maxlen==3670026) // detect 4M images (EAP3660 etc.)
		    {
		    filesyssize = 0x2f0000;
		    }
		strcpy(img->name, "rootfs");
		img->flash_base = flash_addr;
		img->mem_base = 0x80041000;
		img->entry_point = 0;
		img->size = filesyssize;
		img->data_length = filesyssize;
		img++;
		strcpy(img->name, "vmlinux.bin.l7");
		img->flash_base = flash_addr+filesyssize;
		img->mem_base = 0x80041000;
		img->entry_point = 0x80041798;
		img->size = linuxsize;
		img->data_length = linuxsize;
		img++;
		strcpy(img->name, "cfg");
		img->flash_base = flash_addr+filesyssize+linuxsize;
		img->mem_base = 0x80041000;
		img->entry_point = 0;
		img->size = cfgsize;
		img->data_length = cfgsize;
		
		fis_update_directory();
		diag_printf("SENAO_FW: flashing done\n");
	}
	return 0;
}
