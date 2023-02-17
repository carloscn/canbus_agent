#ifndef _PAL_CIPHER_
#define _PAL_CIPHER_

#include <stdint.h>
#include <stddef.h>

#define PROV_AES_COM_KEY "60EAE9193555278880261C1995531F87B5097E494684F0CE1DBB50ABF625A82F"
#define PROV_AES_COM_IV  "98389947B91F81C8F70FD8196A269D77"

typedef enum aes_type_t {
    AES_TYPE_CBC_128,
    AES_TYPE_CBC_256,
    AES_TYPE_GCM_256
} AES_TYPE_T;

#define OPSSL_OK 1
#define PAL_CIPHER_AES_CBC_128_KEY_BYTES_LEN (16) /*bytes*/
#define PAL_CIPHER_AES_CBC_256_KEY_BYTES_LEN (32) /*bytes*/
#define PAL_CIPHER_AES_CBC_IV_BYTES_LEN (16)
#define PAL_CIPHER_AES_CBC_BLOCK_SIZE (16)

int32_t pal_crc32(uint8_t *buffer, size_t len, uint32_t *crc32_out);
int32_t pal_cipher_decrypt_cbc(uint8_t *cipher_array,
                               size_t cipher_len,
                               uint8_t *key,
                               size_t key_len,
                               uint8_t *iv,
                               size_t iv_len,
                               uint8_t *out,
                               size_t *out_len);
int32_t pal_cipher_encrypt_cbc(uint8_t *plain_array,
                               size_t plain_len,
                               uint8_t *key,
                               size_t key_len,
                               uint8_t *iv,
                               size_t iv_len,
                               uint8_t *out,
                               size_t *out_len);
#endif /* _PAL_CIPHER_ */
