#ifndef _PROV_MSG_
#define _PROV_MSG_

#include <stdint.h>
#include <stddef.h>

#define PROV_MSG_MAGIC (0x5052544cU) /* PRTL */
#define PROV_MSG_VERSION(_x) (_x)
#define PROV_MSG_CRYPTO_NONE (0x0u)
#define PROV_MSG_CRYPTO_AES_CBC (0x01u)
#define PROV_MSG_CRYPTO_AES_GCM (0x02u)
#define PROV_MSG_BLOCK_SIZE (128)

typedef struct _prov_req_msg_t {
    uint32_t magic;
    uint32_t version;
    uint32_t crypto;
    uint32_t res[4];
    uint32_t crc;
    uint32_t payload_len;
    uint8_t *payload_buf;
} __attribute__((aligned(4))) RPOV_REQ_MSG_T;
#define PROV_MSG_PADDING (sizeof(RPOV_REQ_MSG_T))
#define PROV_MSG_HEADER_LEN (sizeof(RPOV_REQ_MSG_T) - sizeof(uint32_t))

int32_t prov_msg_pack_req(uint32_t crypto, uint32_t crc, uint8_t *payload, size_t *io_len);
int32_t prov_msg_depack_req(uint32_t *crypto, uint32_t *crc, uint8_t *payload, size_t *io_len);
#endif /* _PROV_MSG_ */