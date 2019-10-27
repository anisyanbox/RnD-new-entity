#ifndef _XOR_H_
#define _XOR_H_

#include <stdio.h>
#include <stddef.h>

/**
 * @param fname
 * name of file
 * 
 * @param key
 * key for xor encrypt/decrypt data
 *
 * @param out
 * pounter to xored/dexored data
 * 
 * @param outlen
 * pointer to xored/dexored data len
 * 
 * @return
 * RET_SUCCESS - success
 * RET_ERR - fault
 **/
int xor_file(char *fname, char *key, char **out, size_t *outlen);
int dexor_file(char *fname, char *key, char **out, size_t *outlen);


/**
 * @param xored_file
 * name of file with xored data
 * 
 * @param out
 * buffer with new dexored data
 * 
 * @param len
 * length of new dexored data 
 * 
 * @return
 * RET_SUCCESS - break xor success
 * RET_ERR - break xor fault
 * 
 **/
int break_xored_file(char *xored_file,  char **out, size_t *len);

#endif  // _XOR_H_
