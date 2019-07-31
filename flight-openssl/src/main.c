#include <stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "main.h"

// -----------------------------------------------------------------------------
static int OpenSSLInitLib(void) {
  return 0;
}

// -----------------------------------------------------------------------------
int main (int argc, char * argv[]) {
  if (!OpenSSLInitLib()) {
    printf("Hello, OpenSSL!\n");
  } else {
    printf("Init OpenSSL error!\n");
  }
  return 0;
}
