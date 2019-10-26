#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* used for returning from functions */
#define RET_SUCCESS 1
#define RET_ERR -1

/* do some xor operation or not */
#define DO_OP 1
#define NOT_DO_OP 0

#define ERR_PRFX_STR "[ERR]: "

struct what_xor_oper {
    int e_flag;
    int d_flag;
    int b_flag;
    int t_flag;
    FILE *fi;
    char *key;
};

static void usage(void)
{
    printf("Usage: ./xor_enc <flags> -i [FILE] -k [KEYWORD]\n");
    printf("flags:\n");
    printf("-i --> input file pointer\n");
    printf("-k --> keyword pointer\n");
    printf("-e --> encrypt [FILE] with [KEYWORD]\n");
    printf("-d --> decrypt [FILE] with [KEYWORD]\n");
    printf("-b --> break xored [FILE] ([KEYWORD] isn't used in this case)");
}

static int parse_args(int argc, char ** argv, struct what_xor_oper *op)
{
    return RET_ERR;
}

static void xor_break(FILE *xored_file)
{
    return;
}

static void xor_encrypt(FILE *fi, char *key)
{
    return;
}

static void xor_decrypt(FILE *fi, char *key)
{
    return;
}

int main(int argc, char **argv)
{
    struct what_xor_oper xor_oper;

    if (parse_args(argc, argv, &xor_oper) != RET_SUCCESS) {
        usage();

        return EXIT_FAILURE;
    }

    if (xor_oper.e_flag)
        xor_encrypt(xor_oper.fi, xor_oper.key);
    else if (xor_oper.d_flag)
        xor_decrypt(xor_oper.fi, xor_oper.key);
    else if (xor_oper.b_flag)
        xor_break(xor_oper.fi);

    return EXIT_SUCCESS;
}
