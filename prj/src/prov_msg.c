#include "prov_msg.h"
#include "utils.h"
#include <string.h>
#include "hal_err.h"
#include <stdlib.h>

static inline size_t get_req_size(RPOV_REQ_MSG_T *msg)
{
    return msg->payload_len + PROV_MSG_HEADER_LEN;
}

int32_t prov_msg_pack_req(uint32_t crypto, uint32_t crc, uint8_t *payload, size_t *io_len)
{
    int32_t ret = 0;
    RPOV_REQ_MSG_T msg;
    uint8_t *buffer = NULL;

    UTILS_ASSERT_MSG(NULL == payload,
                     "payload is NULL!\n");
    UTILS_ASSERT_MSG(NULL == io_len,
                     "io_len is NULL!\n");
    UTILS_CHECK_LEN(*io_len,
                    "the input len is 0!");

    buffer = (uint8_t *)malloc(*io_len);
    UTILS_CHECK_CONDITION(NULL == buffer,
                          HAL_ERR_MEM_ERROR,
                          "Malloc failed!\n");
    memcpy(buffer, payload, *io_len);

    msg.magic = PROV_MSG_MAGIC;
    msg.version = PROV_MSG_VERSION(1);
    msg.crypto = crypto;
    msg.res[0] = 0;
    msg.res[1] = 0;
    msg.res[2] = 0;
    msg.res[3] = 0;
    msg.crc = crc;
    msg.payload_len = *io_len;
    msg.payload_buf = buffer;

    memcpy(payload, &msg, PROV_MSG_HEADER_LEN);
    memcpy(payload + PROV_MSG_HEADER_LEN, buffer, *io_len);

    *io_len = get_req_size(&msg);

finish:
    UTILS_SAFE_FREE(buffer);
    return ret;
}

int32_t prov_msg_depack_req(uint32_t *crypto, uint32_t *crc, uint8_t *payload, size_t *io_len)
{
    int32_t ret = 0;
    size_t payload_len = 0;
    RPOV_REQ_MSG_T msg;

    memset(&msg, 0, sizeof(RPOV_REQ_MSG_T));

    UTILS_ASSERT_MSG(NULL == payload,
                     "payload is NULL!\n");
    UTILS_ASSERT_MSG(NULL == crypto,
                     "crypto is NULL!\n");
    UTILS_ASSERT_MSG(NULL == crc,
                     "crc is NULL!\n");
    UTILS_ASSERT_MSG(NULL == io_len,
                     "io_len is NULL!\n");
    UTILS_CHECK_LEN(*io_len,
                    "the input len is 0!\n");

    memcpy(&msg, payload, PROV_MSG_HEADER_LEN);

    UTILS_CHECK_CONDITION(msg.magic != PROV_MSG_MAGIC,
                          HAL_ERR_COM_MAGIC,
                          "The error magic!\n");
    UTILS_CHECK_CONDITION(msg.version != PROV_MSG_VERSION(1),
                          HAL_ERR_COM_VERSION,
                          "The error version!\n");
    UTILS_CHECK_CONDITION(msg.payload_len > *io_len,
                          HAL_ERR_BUFFER_TOO_SMALL,
                          "The buffer is too small!\n");
    *crypto = msg.crypto;
    *crc = msg.crc;
    *io_len = msg.payload_len;
    memmove(payload, payload + PROV_MSG_HEADER_LEN, msg.payload_len);

finish:
    return ret;
}
