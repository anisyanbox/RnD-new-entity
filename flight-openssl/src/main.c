#include <stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include "main.h"

// -----------------------------------------------------------------------------
static int LibcryptoInit(void) {
  int res = 0;

  /* Load the human readable error strings for libcrypto */
  if (ERR_load_crypto_strings()) {
    res += 1;
  }

  /* Load all digest and cipher algorithms */
  if (OpenSSL_add_all_algorithms()) {
    res += 1;
  }
  
  if (res < 2) {
    return 0;
  } else {
    return res;
  }
}

// -----------------------------------------------------------------------------
static void LibcryptoCleanupAll(void) {
  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO 
  (low level API) for e.g. base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  ERR_free_strings();
}

// -----------------------------------------------------------------------------
int main (int argc, char * argv[]) {
  if (LibcryptoInit()) {
    printf("Hello, OpenSSL!\n");
  } else {
    printf("Init OpenSSL error!\n");
  }

  LibcryptoCleanupAll();
  return 0;
}
