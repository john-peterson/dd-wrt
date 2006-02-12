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
  @file ecb_encrypt.c
  ECB implementation, encrypt a block, Tom St Denis
*/

#ifdef ECB

/**
  ECB encrypt
  @param pt     Plaintext
  @param ct     [out] Ciphertext
  @param len    The number of octets to process (must be multiple of the cipher block size)
  @param ecb    ECB state
  @return CRYPT_OK if successful
*/
int ecb_encrypt(const unsigned char *pt, unsigned char *ct, unsigned long len, symmetric_ECB *ecb)
{
   int err;
   LTC_ARGCHK(pt != NULL);
   LTC_ARGCHK(ct != NULL);
   LTC_ARGCHK(ecb != NULL);
   if ((err = cipher_is_valid(ecb->cipher)) != CRYPT_OK) {
       return err;
   }
   if (len % cipher_descriptor[ecb->cipher].block_length) {
      return CRYPT_INVALID_ARG;
   }

   /* check for accel */
   if (cipher_descriptor[ecb->cipher].accel_ecb_encrypt != NULL) {
      cipher_descriptor[ecb->cipher].accel_ecb_encrypt(pt, ct, len / cipher_descriptor[ecb->cipher].block_length, &ecb->key);
   } else {
      while (len) {
         cipher_descriptor[ecb->cipher].ecb_encrypt(pt, ct, &ecb->key);
         pt  += cipher_descriptor[ecb->cipher].block_length;
         ct  += cipher_descriptor[ecb->cipher].block_length;
         len -= cipher_descriptor[ecb->cipher].block_length;
      }
   }
   return CRYPT_OK;
}

#endif

/* $Source: /cvs/libtom/libtomcrypt/src/modes/ecb/ecb_encrypt.c,v $ */
/* $Revision: 1.5 $ */
/* $Date: 2005/05/05 14:35:59 $ */
