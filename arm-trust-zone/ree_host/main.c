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
  TEEC_UUID hello_ta_uuid = HELLO_TA_UUID;
  TEEC_Operation op;
  uint32_t orig;

  /* Initialize a context. With help it we can connect to the TEE */
  res = TEEC_InitializeContext(ta_name, &hello_ta_ctx);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InitializeContext() failed with %x\n value", res);
  }

  /* 
   * Opena a session to my hello-ta. TA will code some to base64, when 
   * the session is created. 
   * 
   * These UUID values are used to identify Trusted Applications.
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

  /* We can execute some functions in the TA by invoking it */
  /* Initialize the TEEC_Operation struct and prepare the arguments to TrustZone */
  memset(&op, 0, sizeof(op));

  /*
   * Types for args. We use only firsrt args passed to TA, that's why 
   * other = TEEC_NONE
   */
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, 
                                   TEEC_NONE,
                                   TEEC_NONE,
                                   TEEC_NONE);
  /* this args are passed to TA*/
  op.params[0].value.a = 42;
  op.params[0].value.b = 42;

  /*
   * 'HELLO_TA_MULTIPLY_ARGS' is the actual function in the TA to be called.
   */
  printf("Invoking TA to HELLO_TA_MULTIPLY_ARGS %d\n", op.params[0].value.a);
  res = TEEC_InvokeCommand(&hello_ta_session, HELLO_TA_MULTIPLY_ARGS, &op, &orig);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InvokeCommand() failed with code 0x%x origin 0x%x", res, orig);
  }
  printf("TA multiply value to %d\n", op.params[0].value.a);
  
  /*
   * When all operation finished, we need close the session and 
   * destroy the context.
   */
  TEEC_CloseSession(&hello_ta_session);
  TEEC_FinalizeContext(&hello_ta_ctx);
  return 0;
}
