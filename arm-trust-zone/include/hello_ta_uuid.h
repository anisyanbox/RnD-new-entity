#ifndef _INCLUDED_HELLO_TA_UUID_H_
#define _INCLUDED_HELLO_TA_UUID_H_

/*
 * UUID generator at https://www.uuidgenerator.net/
 */
#define HELLO_TA_UUID \
    { 0x4f5a5ab6, 0x0ecc, 0x4ad4, \
       { 0x83, 0x7c, 0xd9, 0xfd, 0x7b, 0x70, 0x89, 0x78} }

/*
 * Commands implemented by the TA
 */
enum {
  HELLO_TA_MULTIPLY_ARGS = 0,
};

#endif  // _INCLUDED_HELLO_TA_UUID_H_
