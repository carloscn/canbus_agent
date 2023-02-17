#include "hal_file.h"

int32_t hal_file_write(const char *path, uint8_t *buffer, size_t len)
{
    int32_t ret = 0;
    FILE *fp = NULL;
    ssize_t left = (ssize_t)len;
    ssize_t write_bytes = 0;
    ssize_t written_bytes = 0;

    if (!left) {
        goto finish;
    }

    UTILS_ASSERT_MSG(NULL == path,
                     "path is NULL!\n");
    UTILS_ASSERT_MSG(NULL == buffer,
                     "buffer is NULL!\n");

    fp = fopen(path, "wb+");
    UTILS_CHECK_CONDITION(NULL == fp,
                          HAL_ERR_FILE_ACCESS,
                          "open file %s fail\n",
                          path);
    do {
        write_bytes = (left <= HAL_FILE_BLOCK_SIZE) ? left : HAL_FILE_BLOCK_SIZE;
        written_bytes = fwrite((void *)buffer,
                                sizeof(uint8_t),
                                write_bytes,
                                fp);
        UTILS_CHECK_CONDITION(write_bytes != written_bytes,
                              HAL_ERR_SEND_ERROR,
                              "The write bytes: %zu, the real write %zu\n",
                              write_bytes, written_bytes);
        left -= written_bytes;
        buffer += written_bytes;
    } while (left != 0);

finish:
    if (fp != NULL) {
        fclose(fp);
    }
    fp = NULL;
    return ret;
}
