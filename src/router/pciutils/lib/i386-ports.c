/*
 *	$Id: i386-ports.c,v 1.3 2002/03/30 15:39:25 mj Exp $
 *
 *	The PCI Library -- Direct Configuration access via i386 Ports
 *
 *	Copyright (c) 1997--1999 Martin Mares <mj@ucw.cz>
 *
 *	Can be freely distributed and used under the terms of the GNU GPL.
 */

#include <unistd.h>

#ifdef __GLIBC__
#include <sys/io.h>
#else
#include <asm/io.h>
#endif

#include "internal.h"

static int intel_iopl_set = -1;

static int
intel_setup_io(void)
{
  if (intel_iopl_set < 0)
    intel_iopl_set = (iopl(3) < 0) ? 0 : 1;
  return intel_iopl_set;
}

static void
conf12_init(struct pci_access *a)
{
  if (!intel_setup_io())
    a->error("You need to be root to have access to I/O ports.");
}

static void
conf12_cleanup(struct pci_access * UNUSED a)
{
  iopl(3);
  intel_iopl_set = -1;
}

/*
 * Before we decide to use direct hardware access mechanisms, we try to do some
 * trivial checks to ensure it at least _seems_ to be working -- we just test
 * whether bus 00 contains a host bridge (this is similar to checking
 * techniques used in XFree86, but ours should be more reliable since we
 * attempt to make use of direct access hints provided by the PCI BIOS).
 *
 * This should be close to trivial, but it isn't, because there are buggy
 * chipsets (yes, you guessed it, by Intel and Compaq) that have no class ID.
 */

static int
intel_sanity_check(struct pci_access *a, struct pci_methods *m)
{
  struct pci_dev d;

  a->debug("...sanity check");
  d.bus = 0;
  d.func = 0;
  for(d.dev = 0; d.dev < 32; d.dev++)
    {
      u16 class, vendor;
      if (m->read(&d, PCI_CLASS_DEVICE, (byte *) &class, sizeof(class)) &&
	  (class == cpu_to_le16(PCI_CLASS_BRIDGE_HOST) || class == cpu_to_le16(PCI_CLASS_DISPLAY_VGA)) ||
	  m->read(&d, PCI_VENDOR_ID, (byte *) &vendor, sizeof(vendor)) &&
	  (vendor == cpu_to_le16(PCI_VENDOR_ID_INTEL) || vendor == cpu_to_le16(PCI_VENDOR_ID_COMPAQ)))
	{
	  a->debug("...outside the Asylum at 0/%02x/0", d.dev);
	  return 1;
	}
    }
  a->debug("...insane");
  return 0;
}

/*
 *	Configuration type 1
 */

#define CONFIG_CMD(bus, device_fn, where)   (0x80000000 | (bus << 16) | (device_fn << 8) | (where & ~3))

static int
conf1_detect(struct pci_access *a)
{
  unsigned int tmp;
  int res = 0;

  if (!intel_setup_io())
    {
      a->debug("...no I/O permission");
      return 0;
    }
  outb (0x01, 0xCFB);
  tmp = inl (0xCF8);
  outl (0x80000000, 0xCF8);
  if (inl (0xCF8) == 0x80000000)
    res = 1;
  outl (tmp, 0xCF8);
  if (res)
    res = intel_sanity_check(a, &pm_intel_conf1);
  return res;
}

static int
conf1_read(struct pci_dev *d, int pos, byte *buf, int len)
{
  int addr = 0xcfc + (pos&3);
  outl(0x80000000 | ((d->bus & 0xff) << 16) | (PCI_DEVFN(d->dev, d->func) << 8) | (pos&~3), 0xcf8);

  switch (len)
    {
    case 1:
      buf[0] = inb(addr);
      break;
    case 2:
      ((u16 *) buf)[0] = cpu_to_le16(inw(addr));
      break;
    case 4:
      ((u32 *) buf)[0] = cpu_to_le32(inl(addr));
      break;
    default:
      return pci_generic_block_read(d, pos, buf, len);
    }
  return 1;
}

static int
conf1_write(struct pci_dev *d, int pos, byte *buf, int len)
{
  int addr = 0xcfc + (pos&3);
  outl(0x80000000 | ((d->bus & 0xff) << 16) | (PCI_DEVFN(d->dev, d->func) << 8) | (pos&~3), 0xcf8);

  switch (len)
    {
    case 1:
      outb(buf[0], addr);
      break;
    case 2:
      outw(le16_to_cpu(((u16 *) buf)[0]), addr);
      break;
    case 4:
      outl(le32_to_cpu(((u32 *) buf)[0]), addr);
      break;
    default:
      return pci_generic_block_write(d, pos, buf, len);
    }
  return 1;
}

/*
 *	Configuration type 2. Obsolete and brain-damaged, but existing.
 */

static int
conf2_detect(struct pci_access *a)
{
  if (!intel_setup_io())
    {
      a->debug("...no I/O permission");
      return 0;
    }

  /* This is ugly and tends to produce false positives. Beware. */

  outb(0x00, 0xCFB);
  outb(0x00, 0xCF8);
  outb(0x00, 0xCFA);
  if (inb(0xCF8) == 0x00 && inb(0xCFA) == 0x00)
    return intel_sanity_check(a, &pm_intel_conf2);
  else
    return 0;
}

static int
conf2_read(struct pci_dev *d, int pos, byte *buf, int len)
{
  int addr = 0xc000 | (d->dev << 8) | pos;

  if (d->dev >= 16)
    /* conf2 supports only 16 devices per bus */
    return 0;
  outb((d->func << 1) | 0xf0, 0xcf8);
  outb(d->bus, 0xcfa);
  switch (len)
    {
    case 1:
      buf[0] = inb(addr);
      break;
    case 2:
      ((u16 *) buf)[0] = cpu_to_le16(inw(addr));
      break;
    case 4:
      ((u32 *) buf)[0] = cpu_to_le32(inl(addr));
      break;
    default:
      outb(0, 0xcf8);
      return pci_generic_block_read(d, pos, buf, len);
    }
  outb(0, 0xcf8);
  return 1;
}

static int
conf2_write(struct pci_dev *d, int pos, byte *buf, int len)
{
  int addr = 0xc000 | (d->dev << 8) | pos;

  if (d->dev >= 16)
    d->access->error("conf2_write: only first 16 devices exist.");
  outb((d->func << 1) | 0xf0, 0xcf8);
  outb(d->bus, 0xcfa);
  switch (len)
    {
    case 1:
      outb(buf[0], addr);
      break;
    case 2:
      outw(le16_to_cpu(* (u16 *) buf), addr);
      break;
    case 4:
      outl(le32_to_cpu(* (u32 *) buf), addr);
      break;
    default:
      outb(0, 0xcf8);
      return pci_generic_block_write(d, pos, buf, len);
    }
  outb(0, 0xcf8);
  return 1;
}

struct pci_methods pm_intel_conf1 = {
  "Intel-conf1",
  NULL,					/* config */
  conf1_detect,
  conf12_init,
  conf12_cleanup,
  pci_generic_scan,
  pci_generic_fill_info,
  conf1_read,
  conf1_write,
  NULL,					/* init_dev */
  NULL					/* cleanup_dev */
};

struct pci_methods pm_intel_conf2 = {
  "Intel-conf2",
  NULL,					/* config */
  conf2_detect,
  conf12_init,
  conf12_cleanup,
  pci_generic_scan,
  pci_generic_fill_info,
  conf2_read,
  conf2_write,
  NULL,					/* init_dev */
  NULL					/* cleanup_dev */
};
