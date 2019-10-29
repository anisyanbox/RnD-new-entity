#ifndef _XOR_H_
#define _XOR_H_

#include <stdio.h>
#include <stddef.h>

/**
 * @param d
 * Data for xoring (It is array with bare data).
 * This array will changed coresponding with k.
 * 
 * @param dlen
 * Lengh of data for xor
 *
 * @param k
 * Key for xor
 * 
 * @param klen
 * Key length
 * 
 * @return
 * RET_SUCCESS - success
 * RET_ERR - fault
 **/
int xor_bare_data(char *d, size_t dlen, const char *k, size_t klen);

/**
 * Function xored data from 'finame' file with 'key' and save new data
 * in file with 'finame+fo_end' name.
 * 
 * @param finame
 * Name of input file
 * 
 * @param key
 * Key for xor/dexor data
 *
 * @return
 * RET_SUCCESS - success
 * RET_ERR - fault
 **/
int xor_file(const char *finame, const char *key, const char *fo_end);
int dexor_file(const char *finame, const char *key, const char *fo_end);

/**
 * @param finame_xored
 * Name of file with xored data
 * 
 * @param out
 * Buffer with new dexored data
 * 
 * @param len
 * Length of new dexored data 
 * 
 * @return
 * RET_SUCCESS - break xor success
 * RET_ERR - break xor fault
 * 
 **/
int break_xored_file(const char *finame_xored,  char **out, size_t *len);

#endif  // _XOR_H_
