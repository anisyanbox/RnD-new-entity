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

static int check_key_and_fname(const char *fname, const char *key,
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

static char *read_data_from_file(const char *fname, size_t flen)
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

static int save_xored_data_to_file(const char *orig_fname,
            const char *end, char *d, size_t dlen)
{
    FILE *fo;
    char *fo_name;
    size_t fo_len = strlen(orig_fname) + strlen(end);

    /* memory for save new file name */
    fo_name = (char *)malloc(sizeof(char) * fo_len);
    
    if (fo_name) {
        sprintf(fo_name, "%s%s", orig_fname, end);

        DBG_PRNT("output file is: <%s>\n", fo_name);
    } else {
        ERR_PRNT("can't allocate data for output file name\n");

        return RET_ERR;
    }

    if ((fo = fopen(fo_name, "w")) == NULL) {
        ERR_PRNT("can't create out file <%s>\n", fo_name);

        free(fo_name);

        return RET_ERR;
    }

    (void)fwrite(d, sizeof(char), dlen, fo);

    free(fo_name);
    (void)fclose(fo);

    return RET_SUCCESS;
}

int xor_bare_data(char *d, size_t dlen, const char *k, size_t klen)
{
    if (klen > dlen) {
        ERR_PRNT("key length more data\n");

        return RET_ERR;
    }

    int i = 0;
    int j = 0;

    for (; i < dlen; ++i) {
        d[i] ^= k[j];

        ++j;
        if (j == klen)
            j = 0;
    }

    return RET_SUCCESS;
}

int xor_file(const char *fname, const char *key, const char *fo_end)
{
    size_t klen;
    size_t flen;
    char *fdata;
    int ret = RET_SUCCESS;

    /* check input params */
    if ((check_key_and_fname(fname, key, &klen, &flen)) != RET_SUCCESS) {
        ERR_PRNT("check_key_and_fname() failed\n");

        return RET_ERR;
    }

    /* get data */
    if ((fdata = read_data_from_file(fname, flen)) == NULL) {
        ERR_PRNT("read_data_from_file() failed\n");

        return RET_ERR;
    }

    /* fdata will change after this */
    if ((xor_bare_data(fdata, flen, key, klen)) != RET_SUCCESS) {
        ERR_PRNT("xor_bare_data() failed\n");

        ret = RET_ERR;
        goto end;
    }

    if ((save_xored_data_to_file(fname, fo_end, fdata, flen)) != RET_SUCCESS) {
        ERR_PRNT("save_xored_data_to_file() failed\n");

        ret = RET_ERR;
        goto end;
    }

end:
    free(fdata);
    return ret;
}

int dexor_file(const char *fname, const char *key, const char *fo_end)
{
    return xor_file(fname, key, fo_end);
}

int break_xored_file(const char *finame_xored,  char **out, size_t *len)
{
    return RET_ERR;
}
