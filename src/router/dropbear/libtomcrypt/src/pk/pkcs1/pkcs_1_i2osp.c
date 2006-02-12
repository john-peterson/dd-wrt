/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtomcrypt.org
 */
#include "tomcrypt.h"

/** 
  @file pkcs_1_i2osp.c
  Integer to Octet I2OSP, Tom St Denis 
*/

#ifdef PKCS_1

/* always stores the same # of bytes, pads with leading zero bytes
   as required
 */

/**
   PKCS #1 Integer to binary
   @param n             The integer to store
   @param modulus_len   The length of the RSA modulus
   @param out           [out] The destination for the integer
   @return CRYPT_OK if successful
*/
int pkcs_1_i2osp(mp_int *n, unsigned long modulus_len, unsigned char *out)
{
   int err;
   unsigned long size;

   size = mp_unsigned_bin_size(n);

   if (size > modulus_len) {
      return CRYPT_BUFFER_OVERFLOW;
   }

   /* store it */
   zeromem(out, modulus_len);
   if ((err = mp_to_unsigned_bin(n, out+(modulus_len-size))) != MP_OKAY) {
      return mpi_to_ltc_error(err);
   }
   return CRYPT_OK;
}

#endif /* PKCS_1 */


/* $Source: /cvs/libtom/libtomcrypt/src/pk/pkcs1/pkcs_1_i2osp.c,v $ */
/* $Revision: 1.3 $ */
/* $Date: 2005/05/05 14:35:59 $ */
