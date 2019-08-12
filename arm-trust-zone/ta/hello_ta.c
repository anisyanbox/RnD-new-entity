#include <tee_ta_api.h>
#include <trace.h>
#include "hello_ta_uuid.h"

/*
 * Trusted Application Entry Points
 */

/* Called each time a new instance is created */
TEE_Result TA_CreateEntryPoint(void) {
  DMSG("has been called");
  return TEE_SUCCESS;
}

/* Called each time an instance is destroyed */
void TA_DestroyEntryPoint(void) {
  DMSG("has been called");
}

/* Called each time a session is opened */
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes,
                                    TEE_Param pParams[4],
                                    void **ppSessionContext) {
  uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                            TEE_PARAM_TYPE_NONE,
                                            TEE_PARAM_TYPE_NONE,
                                            TEE_PARAM_TYPE_NONE);
  
  DMSG("has been called");
  IMSG("Hello, from TrustZone!\n");

  if (nParamTypes != exp_param_types) {
    return TEE_ERROR_BAD_PARAMETERS;
  }

  /* Unused parameters */
  (void)pParams;
  (void)ppSessionContext;

  /* If return value != TEE_SUCCESS the session will not be created. */
  return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext) {
  (void)pSessionContext;

  IMSG("Goodbye!\n");
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
              uint32_t nCommandID, uint32_t nParamTypes,
              TEE_Param pParams[4]) {
  /* Unused parameters */
  (void)pSessionContext;

  switch (nCommandID) {
  case HELLO_TA_MULTIPLY_ARGS:
    pParams[0].value.a = pParams[0].value.a * pParams[0].value.b; 
    break;
  
  default:
    break;
  }
  return TEE_SUCCESS;
}
