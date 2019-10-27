#ifndef _MAIN_H_
#define _MAIN_H_

/* max file size - 10 MB*/
#define PREFIX_FOR_MB (1024 * 1024)
#define MAX_FILE_SIZE (10 * PREFIX_FOR_MB)

/* used for returning from functions */
#define RET_SUCCESS 1
#define RET_ERR -1

/* do some xor operation or not */
#define DO_OP 1
#define NOT_DO_OP 0

/* logging defines */
#ifdef DBG_LOG_EN
#define DBG_PRFX_STR "[DBG]: "
#define DBG_PRNT(...) printf(DBG_PRFX_STR); printf(__VA_ARGS__)
#else
#define DBG_PRNT(...)
#endif

#ifdef ERR_LOG_EN
#define ERR_PRFX_STR "[ERR]: "
#define ERR_PRNT(...) printf(ERR_PRFX_STR); printf(__VA_ARGS__)
#else
#define ERR_PRNT(...)
#endif

#endif  // _MAIN_H_
