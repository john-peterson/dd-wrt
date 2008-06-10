/*
 */
#include <string.h>
#include <unistd.h>

#include <typedefs.h>
#include <bcmutils.h>
#include <wlutils.h>
#include <shutils.h>
#include <utils.h>
#include <bcmnvram.h>

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>


#if  defined(HAVE_AR531X) || defined(HAVE_LSX)

void
set_gpio (int gpio, int value)
{
  FILE *in;
  char buf[64];
  sprintf (buf, "/proc/gpio/%d_dir", gpio);
  in = fopen (buf, "wb");
  if (in==NULL)
    return;
  fprintf (in, "1");
  fclose (in);
  sprintf (buf, "/proc/gpio/%d_out", gpio);
  in = fopen (buf, "wb");
  if (in==NULL)
    return;
  fprintf (in, "%d", value);
  fclose (in);
}

int
get_gpio (int gpio)
{
  FILE *in;
  int ret;
  char buf[64];
  sprintf (buf, "/proc/gpio/%d_dir", gpio);
  in = fopen (buf, "wb");
  if (in==NULL)
    return 0;
  fprintf (in, "0");
  fclose (in);
  sprintf (buf, "/proc/gpio/%d_in", gpio);
  in = fopen (buf, "rb");
  if (in==NULL)
    return 0;
  fscanf (in, "%d", &ret);
  fclose (in);
  return ret;
}

#elif HAVE_XSCALE
#define u8 unsigned char
#define u32 unsigned long

//#include <linux/ixp425-gpio.h>

#include <asm/hardware.h>
#include <asm-arm/arch-ixp4xx/ixp4xx-regs.h>

#define IXP4XX_GPIO_OUT 		0x1
#define IXP4XX_GPIO_IN  		0x2

struct gpio_bit
{
  unsigned char bit;
  unsigned char state;
};



char *filename = "/dev/gpio";





void
set_gpio (int gpio, int value)
{
  int file;
  struct gpio_bit _bit;

  /* open device */
  if ((file = open (filename, O_RDWR)) == -1)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
//      fprintf (stderr, "Error: could not open %s (%d)\n", filename, errno);
      return;
    }

  /* Config bit as output */
  _bit.bit = gpio;
  _bit.state = IXP4XX_GPIO_OUT;
  if (ioctl (file, GPIO_SET_CONFIG, (long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
//      fprintf (stderr, "Error: ioctl failed: %s (%d)\n", strerror (errno),
//             errno);
      return;
    }

  /* Write data */
  _bit.bit = gpio;
  _bit.state = value;
  if (ioctl (file, GPIO_SET_BIT, (unsigned long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
//      fprintf (stderr, "Error: ioctl failed: %s (%d)\n", strerror (errno),
//             errno);
      return;
    }

  close (file);

}

int
get_gpio (int gpio)
{
  int file;
  struct gpio_bit _bit;

  /* open device */
  if ((file = open (filename, O_RDONLY)) == -1)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  /* Config pin as input */
  _bit.bit = gpio;
  _bit.state = IXP4XX_GPIO_IN;
  if (ioctl (file, GPIO_SET_CONFIG, (long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  /* Read data */
  _bit.bit = gpio;
  if (ioctl (file, GPIO_GET_BIT, (long) &_bit) < 0)
    {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return 1;
    }

  close (file);
  return _bit.state;
}


#else

void
set_gpio (int pin, int value)
{
  int gpioouten = open ("/dev/gpio/outen", O_RDWR);
  int gpioout = open ("/dev/gpio/out", O_RDWR);
  unsigned int gpio;

  read (gpioouten, &gpio, sizeof (gpio));
  gpio |= 1<<pin;
  write (gpioouten, &gpio, sizeof (gpio));

  read (gpioout, &gpio, sizeof (gpio));
  if (value)
    {
      gpio |= (1<<pin);
    }
  else
    {
      gpio &= ~(1<<pin);
    }
  write (gpioout, &gpio, sizeof (gpio));
  close (gpioout);
  close(gpioouten);
}

int
get_gpio (int pin)
{
  unsigned int gpio;
  int gpioouten = open ("/dev/gpio/outen", O_RDWR);
  int gpioin = open ("/dev/gpio/in", O_RDWR);
  read (gpioouten, &gpio, sizeof (gpio));
  gpio &= ~(1<<pin);
  write (gpioouten, &gpio, sizeof (gpio));
  read (gpioin, &gpio, sizeof (gpio));
  gpio = (gpio & (1<<pin)) ? 1 : 0;
  close (gpioin);
  close(gpioouten);
  return gpio;
}

#endif
