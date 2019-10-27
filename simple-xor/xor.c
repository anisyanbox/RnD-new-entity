#include "xor.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static size_t fsize(const char *fname)
{
    struct stat info;

    stat(fname, &info);

    return (size_t)info.st_size;
}

static void xor_bare_data(char *d, size_t dlen, char *k, size_t klen)
{
    int i = 0;
    int j = 0;

    for (; i < dlen; ++i) {
        d[i] ^= k[j];

        ++j;
        if (j == klen)
            j = 0;
    }
}

static int check_key_and_fname(char *fname, char *key,
                size_t *klen, size_t *flen)
{
    if ((fname) && (key)) {
        *klen = strlen(key);
        *flen = fsize((const char *)fname);

        if (*klen > *flen) {
            ERR_PRNT("key length more than file size\n");

            return RET_ERR;
        }

        if (*flen > MAX_FILE_SIZE) {
            ERR_PRNT("file <%s> is large. Max is %d MB\n", 
                        fname, MAX_FILE_SIZE / PREFIX_FOR_MB);

            return RET_ERR;
        }
    } else {
        ERR_PRNT("file name or key is NULL");

        return RET_ERR;
    }

    return RET_SUCCESS;
}

static char *read_data_from_file(char *fname, size_t flen)
{
    FILE *fi;
    char *fdata = NULL;

    if ((fi = fopen(fname, "r")) == NULL) {
        ERR_PRNT("can't open <%s> file", fname);

        return NULL;
    }

    if ((fdata = (char *)malloc(sizeof(char) * flen)) == NULL) {
        ERR_PRNT("can't allocate memory for data of <%s>", fname);

        return NULL;
    }

    if (flen != fread((void *)fdata, sizeof(char), flen, fi)) {
        ERR_PRNT("can't read all file data\n");

        return NULL;
    }

    return fdata;
}

int xor_file(char *fname, char *key,  char **out, size_t *outlen)
{
    size_t klen;
    size_t flen;
    char *fdata;

    /* check input params */
    if (check_key_and_fname(fname, key, &klen, &flen) != RET_SUCCESS) {
        ERR_PRNT("check_key_and_fname() failed\n");

        return RET_ERR;
    }

    /* get data */
    if ((fdata = read_data_from_file(fname, flen)) == NULL) {
        ERR_PRNT("read_data_from_file() failed\n");

        return RET_ERR;
    }

    /* fdata will change after this */
    xor_bare_data(fdata, flen, key, klen);

    *out = fdata;
    *outlen = flen;

    return RET_SUCCESS;
}

int dexor_file(char *fname, char *key,  char **out, size_t *outlen)
{
    return xor_file(fname, key, out, outlen);
}

int break_xored_file(char *xored_file,  char **out, size_t *len)
{
    return RET_ERR;
}
