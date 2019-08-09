#include <stdio.h>
#include <string.h>
#include <err.h>

#include <tee_client_api.h>
#include <tee_client_api_extensions.h>
#include <teec_trace.h>

#include <hello_ta_uuid.h>

/*
  In principle the commands are called in this order:

  TEEC_InitializeContext(...)
  TEEC_OpenSession(...)
  TEEC_InvokeCommand(...)
  TEEC_CloseSession(...)
  TEEC_FinalizeContext(...)
*/

int main(int argc, char * argv[]) {
  return 0;
}
