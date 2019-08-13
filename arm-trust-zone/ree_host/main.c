#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdint.h>
#include <stdlib.h>

#include <tee_client_api.h>
#include <tee_client_api_extensions.h>
#include <teec_trace.h>

#include <hello_ta_uuid.h>

static const char * ta_cap = "optee-tz";

struct InArgs {
  int x;
  int y;
  bool xsign;    // 1 --> negative;
  bool ysign;    // 1 --> negative;
  bool ans_sing; // 1 --> negative;
};

// -----------------------------------------------------------------------------
static int ValidateInputArgs(int argc, char * argv[], struct InArgs * in_arg) {
  if (argc != 3)  {
    return 1; 
  }
  
  in_arg->ans_sing = 0;
  in_arg->x = atoi(argv[1]);
  in_arg->y = atoi(argv[2]);

  if (in_arg->x < 0) {
    in_arg->xsign = 1;
    in_arg->x = abs(in_arg->x);
  }
  
  if (in_arg->y < 0) {
    in_arg->ysign = 1;
    in_arg->y= abs(in_arg->y);
  }

  if (in_arg->xsign || in_arg->ysign) {
    in_arg->ans_sing = 1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
static void PrintResult(struct InArgs xy, uint32_t res) {
  printf("%s%d * %s%d = %s%d\n", xy.xsign ? "-" : "" ,
                                 xy.x,
                                 xy.ysign ? "-" : "" ,
                                 xy.y,
                                 xy.ans_sing ? "-" : "" ,
                                 res);
}

// -----------------------------------------------------------------------------
int main(int argc, char * argv[]) {
  (void)argc;
  (void)argv;
  TEEC_Result res;
  TEEC_Context hello_ta_ctx;
  TEEC_Session hello_ta_session = {};
  TEEC_UUID hello_ta_uuid = HELLO_TA_UUID;
  TEEC_Operation op = {};
  uint32_t orig;
  struct InArgs in_args;

  if (ValidateInputArgs(argc, argv, &in_args)) {
    errx(1, "ValidateInputArgs() failed\n");
  }

  /* Initialize a context. With help it we can connect to the TEE */
  res = TEEC_InitializeContext(ta_cap, &hello_ta_ctx);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InitializeContext() failed with %x value\n", res);
  }

  /* 
   * Open a session to my hello-ta.
   * UUID values are used to identify Trusted Applications.
   */
  res = TEEC_OpenSession(&hello_ta_ctx,
                         &hello_ta_session,
                         (const TEEC_UUID *)&hello_ta_uuid,
                         TEEC_LOGIN_PUBLIC,
                         NULL,
                         &op,
                         &orig);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_OpenSession() failed with %x value\n", res);
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
  op.params[0].value.a = (uint32_t)in_args.x;
  op.params[0].value.b = (uint32_t)in_args.y;

  /*
   * 'HELLO_TA_MULTIPLY_ARGS' is the actual function in the TA to be called.
   */
  printf("Invoking TA to multiply some values:\n");
  res = TEEC_InvokeCommand(&hello_ta_session, HELLO_TA_MULTIPLY_ARGS, &op, &orig);
  if (res != TEEC_SUCCESS) {
    errx(1, "TEEC_InvokeCommand() failed with code 0x%x origin 0x%x\n", res, orig);
  }
  PrintResult(in_args, op.params[0].value.a);
  
  /*
   * When all operation finished, we need close the session and 
   * destroy the context.
   */
  TEEC_CloseSession(&hello_ta_session);
  TEEC_FinalizeContext(&hello_ta_ctx);
  return 0;
}
