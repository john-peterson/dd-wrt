// eCos memory layout - Mon Jul 23 11:49:04 2001

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <stddef.h>

#endif
#define CYGMEM_REGION_sram (0)
#define CYGMEM_REGION_sram_SIZE (0x8000)
#define CYGMEM_REGION_sram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#define CYGMEM_REGION_ram (0x606000)
#define CYGMEM_REGION_ram_SIZE (0x40000)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE (0x640000 - (size_t) CYG_LABEL_NAME (__heap1))
