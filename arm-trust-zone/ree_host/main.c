#include <stdio.h>
#include <string.h>
#include <err.h>

#include <tee_client_api.h>
#include <tee_client_api_extensions.h>
#include <teec_trace.h>

#include <hello_ta_uuid.h>

static const char * ta_name = "hello-ta";

int main(int argc, char * argv[]) {
  (void)argc;
  (void)argv;
  TEEC_Result res;
  TEEC_Context hello_ta_ctx;
  TEEC_Session hello_ta_session;
  TEEC_UUID hello_ta_uuid = {};
  TEEC_Operation op = {};
  uint32_t orig;

  /* Initialize a context. With help it we can connect to the TEE */
  res = TEEC_InitializeContext(ta_name, &hello_ta_ctx);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InitializeContext() failed with %x\n value", res);
  }

  /* 
   * Opena a session to my hello-ta. TA will code some to base64, when 
   * the session is created. 
   */
  res = TEEC_OpenSession(&hello_ta_ctx,
                         &hello_ta_session,
                         (const TEEC_UUID *)&hello_ta_uuid,
                         TEEC_LOGIN_PUBLIC,
                         NULL,
                         &op,
                         &orig);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_OpenSession() failed with %x\n value", res);
  }

  /*
   * When all operation finished, we need close the session and 
   * destroy the context.
   */
  TEEC_CloseSession(&hello_ta_session);
  TEEC_FinalizeContext(&hello_ta_ctx);
  return 0;
}
