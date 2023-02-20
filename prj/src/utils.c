#include "utils.h"
#include <string.h>


#define DUMP_WIDTH	16

static int32_t utils_convert_char_to_nibble(uint8_t inchar, uint8_t *num) {
  int32_t ret = 0;

  UTILS_ASSERT_MSG(NULL == num, "num is NULL!\n");

  /* Convert the char to nibble */
  if ((inchar >= (uint8_t)'0') && (inchar <= (uint8_t)'9')) {
    *num = inchar - (uint8_t)'0';
  } else if ((inchar >= (uint8_t)'a') && (inchar <= (uint8_t)'f')) {
    *num = inchar - (uint8_t)'a' + 10U;
  } else if ((inchar >= (uint8_t)'A') && (inchar <= (uint8_t)'F')) {
    *num = inchar - (uint8_t)'A' + 10U;
  } else {
    ret = -1;
  }

  return ret;
}

int32_t utils_memcmp(void *a, void *b, size_t len)
{
    if (NULL == a || NULL == b) {
        return -1;
    }

    if (0 == len) {
        return 0;
    }

    while (len --) {
        if (*(char*)a == *(char*)b) {
            a = (char*)a + 1;
            b = (char*)b + 1;
        } else {
            if (*(char*)a > *(char*)b) {
                return 1;
            } else {
                return -1;
            }
        }
    }
    return 0;
}

int32_t utils_convert_str_to_be(const char *str, uint8_t *buf,
                                       size_t ascii_len) {
  int32_t ret = -1;
  size_t c_len = 0;
  uint8_t lower_nibble, upper_nibble;

  UTILS_ASSERT_MSG(NULL == str, "str is NULL!\n");
  UTILS_ASSERT_MSG(NULL == buf, "buf is NULL!\n");

  /* Len has to be multiple of 2 */
  if ((0 == ascii_len) || (ascii_len % 2 == 1)) {
    goto finish;
  }

  c_len = 0;
  while (c_len < ascii_len) {
    if (utils_convert_char_to_nibble(str[c_len], &upper_nibble) == 0) {
      if (utils_convert_char_to_nibble(str[c_len + 1], &lower_nibble) == 0) {
        buf[c_len / 2] = (upper_nibble << 4) | lower_nibble;
      } else {
        goto finish;
      }
    } else {
      goto finish;
    }
    c_len += 2;
  }
  ret = 0;

finish:
  return ret;
}

static void bio_dump(const char *s, int len)
{
    char buf[160+1] = {0};
    char tmp[20] = {0};
    unsigned char ch;
    int32_t i, j, rows;

#ifdef TRUNCATE
    int32_t trunc = 0;
    for(; (len > 0) && ((s[len-1] == ' ') || (s[len-1] == '\0')); len--)
        trunc++;
#endif

    rows = (len / DUMP_WIDTH);
    if ((rows * DUMP_WIDTH) < len)
        rows ++;
    for (i = 0; i < rows; i ++) {
        /* start with empty string */
        buf[0] = '\0';
        sprintf(tmp, "%04x - ", i * DUMP_WIDTH);
        strcpy(buf, tmp);
        for (j = 0; j < DUMP_WIDTH; j ++) {
            if (((i * DUMP_WIDTH) + j) >= len) {
                strcat(buf,"   ");
            } else {
                ch = ((unsigned char)*(s + i * DUMP_WIDTH + j)) & 0xff;
                sprintf(tmp, "%02x%c" , ch, j == 7 ? '-':' ');
                strcat(buf, tmp);
            }
        }
        strcat(buf, "  ");
        for(j = 0;j < DUMP_WIDTH;j ++) {
            if (((i * DUMP_WIDTH) + j) >= len)
                break;
            ch = ((unsigned char)*(s + i * DUMP_WIDTH + j)) & 0xff;
            sprintf(tmp, "%c", ((ch >= ' ')&&(ch <= '~')) ? ch : '.');
            strcat(buf, tmp);
        }
        strcat(buf, "\n");
        printf("%s", buf);
    }
#ifdef TRUNCATE
    if (trunc > 0) {
        sprintf(buf,"%04x - <SPACES/NULS>\n",len+trunc);
        printf("%s", buf);
    }
#endif
}

void utils_print_array(uint8_t *buffer, size_t len, char* msg)
{
    printf("\n");
    printf("%s: the len is %zu\n", msg, len);
    bio_dump((const char *)buffer, len);
    printf("\n");
}

void utils_print_rcv_frame(uint8_t *buffer, size_t len, int32_t id)
{
    printf("ID=%X DLC=%zu ", id, len);
    utils_print_array(buffer, len, "payload");
    printf("\n");
}