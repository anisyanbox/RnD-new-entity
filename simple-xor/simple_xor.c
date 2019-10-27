#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

/* used for returning from functions */
#define RET_SUCCESS 1
#define RET_ERR -1

/* do some xor operation or not */
#define DO_OP 1
#define NOT_DO_OP 0

/* logging defines */
#ifdef DEBUG_MODE
#define ERR_PRFX_STR "[ERR]: "
#define DBG_PRFX_STR "[DBG]: "
#define ERR_PRNT(...) printf(ERR_PRFX_STR); printf(__VA_ARGS__)
#define DBG_PRNT(...) printf(DBG_PRFX_STR); printf(__VA_ARGS__)
#else
#define ERR_PRNT(...)
#define DBG_PRNT(...)
#endif

struct what_xor_oper {
    int e_flag;
    int d_flag;
    int b_flag;
    int t_flag;
    char *fi;
    char *key;
};

static void run_test(void);

static void usage(void)
{
    printf("Usage: ./xor_enc.a <flags> -i [FILE] -k [KEYWORD]\n");
    printf("flags:\n");
    printf("-i --> input file pointer\n");
    printf("-k --> keyword pointer\n");
    printf("-t --> test mode (if it is on, other options are ignored)\n");
    printf("-e --> encrypt [FILE] with [KEYWORD]\n");
    printf("-d --> decrypt [FILE] with [KEYWORD]\n");
    printf("-b --> break xored [FILE] ([KEYWORD] isn't used in this case)");
}

static int parse_args(int argc, char ** argv, struct what_xor_oper **op)
{
    int c;
    static struct what_xor_oper op_struct;

    while ((c = getopt(argc, argv, "edbti:k:")) != RET_ERR) {
        switch (c) {
        case 'i':
            op_struct.fi = optarg;
            break;
        case 'k':
            op_struct.key = optarg;
            break;
        case 'e':
            op_struct.e_flag = DO_OP;
            break;
        case 'd':
            op_struct.d_flag = DO_OP;
            break;
        case 'b':
            op_struct.b_flag = DO_OP;
            break;
        case 't':
            op_struct.t_flag = DO_OP;
            return RET_SUCCESS;
        case '?':
            if ((optopt == 'i') || (optopt == 'k')) {
                ERR_PRNT("need value with -%c option\n\n", optopt);

                return RET_ERR;
            } else if (isprint(optopt)) {
                ERR_PRNT("unknown option `-%c'\n\n", optopt);

                return RET_ERR;
            } else {
                ERR_PRNT("unknown option character `\\x%x'\n\n", optopt);

                return RET_ERR;
            }
        default: 
            break;
        }
    }

    DBG_PRNT("File - %s\n", op_struct.fi);
    DBG_PRNT("Key - %s\n", op_struct.key);
    DBG_PRNT("e - %c\n", op_struct.e_flag ? '1' : '0');
    DBG_PRNT("d - %c\n", op_struct.d_flag ? '1' : '0');
    DBG_PRNT("b - %c\n", op_struct.b_flag ? '1' : '0');
    DBG_PRNT("t - %c\n", op_struct.t_flag ? '1' : '0');

    if (op_struct.t_flag == DO_OP)
        return RET_SUCCESS;

    if ((op_struct.e_flag | op_struct.d_flag | op_struct.b_flag) == NOT_DO_OP) {
        ERR_PRNT("you should set at least the one flag\n");

        return RET_ERR;
    }

    if ((op_struct.e_flag & op_struct.d_flag) || (op_struct.e_flag & op_struct.b_flag) || \
        (op_struct.d_flag & op_struct.b_flag)) {
        ERR_PRNT("mutual exclusion operations\n");

        return RET_ERR;
    }
    if (access((const char *)op_struct.fi, F_OK) == -1 ) {
        ERR_PRNT("file %s not exists\n", op_struct.fi);

        return RET_ERR;
    }

    if (access((const char *)op_struct.fi, R_OK) == -1 ) {
        ERR_PRNT("you can't access to read the %s file", op_struct.fi);

        return RET_ERR;
    }

    *op = &op_struct;

    return RET_SUCCESS;
}

static void xor_break(char *xored_file)
{
    return;
}

static void xor_encrypt(char *fi, char *key)
{
    return;
}

static void xor_decrypt(char *fi, char *key)
{
    return;
}

int main(int argc, char **argv)
{
    struct what_xor_oper *xor_oper;

    if (parse_args(argc, argv, &xor_oper) != RET_SUCCESS) {
        usage();

        return EXIT_FAILURE;
    }

    if (xor_oper->t_flag) {
        run_test();
    }
    else if (xor_oper->e_flag) {
        xor_encrypt(xor_oper->fi, xor_oper->key);
    }
    else if (xor_oper->d_flag) {
        xor_decrypt(xor_oper->fi, xor_oper->key);
    }
    else if (xor_oper->b_flag) {
        xor_break(xor_oper->fi);
    }

    return EXIT_SUCCESS;
}

static void run_test(void)
{
    return;
}
