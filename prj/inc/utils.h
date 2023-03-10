#ifndef _UTILS_
#define _UTILS_

#include <errno.h>
#include <stdio.h>
#include <stdint.h>

#define _PROMPT "[canbus_agent]"
#define ENABLE_DBG 0

#define UTILS_LOG(format, ...)                                      \
        do {                                                        \
            printf(_PROMPT);                                        \
            printf(format, ##__VA_ARGS__);                          \
        } while(0)
#define UTILS_ERR(format, ...)                                      \
        do {                                                        \
            printf(_PROMPT);                                        \
            printf(format, ##__VA_ARGS__);                          \
        } while(0)

#define UTILS_ASSERT(_x)                                                       \
    do {                                                                       \
        if ((_x)) {                                                            \
            UTILS_ERR("ASSERT FAILURE:\n");                                    \
            UTILS_ERR("%s (%d): %s\n", __func__, __LINE__, __FUNCTION__);      \
            while (1) /* loop */                                               \
                ;                                                              \
        }                                                                      \
    } while (0)

#define UTILS_CHECK_RET(_r, format, ...)                                       \
    do {                                                                       \
        if ((0) != (_r)) {                                                     \
            UTILS_ERR("%s line:%d. Error number is 0x%x \n",                   \
                        __func__, __LINE__, _r);                               \
            UTILS_ERR(format, ##__VA_ARGS__);                                  \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

#define UTILS_CHECK_LEN(len, format, ...)                                      \
    do {                                                                       \
        if ((len) == 0) {                                                      \
            UTILS_LOG(format, ##__VA_ARGS__);                                  \
            ret = 0;                                                           \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

#define UTILS_CHECK_CONDITION(err_condition, ret_code, format, ...)            \
    do {                                                                       \
        if ((err_condition)) {                                                 \
            UTILS_ERR("%s line:%d.Error number is 0x%x \n", __func__, __LINE__,\
                ret);                                                          \
            UTILS_ERR(format, ##__VA_ARGS__);                                  \
            ret = ret_code;                                                    \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

#define UTILS_ASSERT_MSG(_x, fmt, ...)                                         \
    do {                                                                       \
        if ((_x)) {                                                            \
            UTILS_ERR("ASSERT FAILURE:\n");                                    \
            UTILS_ERR("%s (%d): %s\n", __func__, __LINE__, __FUNCTION__);      \
            UTILS_ERR(fmt, ##__VA_ARGS__);                                     \
            while (1) /* loop */                                               \
                ;                                                              \
        }                                                                      \
    } while (0)

#define UTILS_SAFE_FREE(_p)                                                    \
    do {                                                                       \
        if ((_p) != NULL) {                                                    \
            free((_p));                                                        \
            (_p) = NULL;                                                       \
        }                                                                      \
    } while(0)
/* MACRO define */
#define UTILS_ARRAY_SIZE(_a_) ((size_t)(sizeof(_a_)/sizeof((_a_)[0])))
#define UTILS_MAX(__a, __b) (_a_, _b_) (((_a_)>(_b_))?(_a_):(_b_))
#define UTILS_MIN(__a, __b) (_a_, _b_) (((_a_)<(_b_))?(_a_):(_b_))
#define UTILS_ASCII_TO_INT(__x) (((__x) - '0'))
#define UTILS_INT_TO_ASCII(__x) (((__x) + '0'))
#define UTILS_ROUNDUP(x, n) ((x+(n-1))&(~(n-1)))

int32_t utils_memcmp(void *a, void *b, size_t len);
void utils_print_rcv_frame(uint8_t *buffer, size_t len, int32_t id);
void utils_print_array(uint8_t *buffer, size_t len, char* msg);
int32_t utils_convert_str_to_be(const char *str, uint8_t *buf,
                                       size_t ascii_len);
#endif /* _UTILS_ */