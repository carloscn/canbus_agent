#include "test_prov_msg.h"
#include "prov_msg.h"
#include "utils.h"
#include "CUnit/Basic.h"

static int32_t test_prov_msg_init(void)
{
    return 0;
}

static void test_prov_msg_cleanup(void)
{

}

static void test_prov_msg_pack_and_repack_20480(void)
{
#define TEST_PAYLOAD_LEN 20480
    int32_t ret = 0;
    uint8_t array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    uint8_t origin_array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    size_t len = TEST_PAYLOAD_LEN;
    uint32_t crypto = 0;
    uint32_t crc32 = 0;
    size_t i = 0;

    for (i = 0; i < TEST_PAYLOAD_LEN; i ++) {
        array[i] = i;
        origin_array[i] = i;
    }

    ret = prov_msg_pack_req(PROV_MSG_CRYPTO_AES_CBC, 0x6878f43a, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    ret = prov_msg_depack_req(&crypto, &crc32, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(crypto, PROV_MSG_CRYPTO_AES_CBC);
    CU_ASSERT_EQUAL(crc32, 0x6878f43a);
    CU_ASSERT_EQUAL(utils_memcmp(array, origin_array, TEST_PAYLOAD_LEN), 0);
}

static void test_prov_msg_pack_and_repack_1024(void)
{
#define TEST_PAYLOAD_LEN 1024
    int32_t ret = 0;
    uint8_t array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    uint8_t origin_array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    size_t len = TEST_PAYLOAD_LEN;
    uint32_t crypto = 0;
    uint32_t crc32 = 0;
    size_t i = 0;

    for (i = 0; i < TEST_PAYLOAD_LEN; i ++) {
        array[i] = i;
        origin_array[i] = i;
    }

    ret = prov_msg_pack_req(PROV_MSG_CRYPTO_AES_CBC, 0x6878f43a, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    ret = prov_msg_depack_req(&crypto, &crc32, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(crypto, PROV_MSG_CRYPTO_AES_CBC);
    CU_ASSERT_EQUAL(crc32, 0x6878f43a);
    CU_ASSERT_EQUAL(utils_memcmp(array, origin_array, TEST_PAYLOAD_LEN), 0);
}

static void test_prov_msg_pack_and_repack_223(void)
{
#define TEST_PAYLOAD_LEN 223
    int32_t ret = 0;
    uint8_t array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    uint8_t origin_array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    size_t len = TEST_PAYLOAD_LEN;
    uint32_t crypto = 0;
    uint32_t crc32 = 0;
    size_t i = 0;

    for (i = 0; i < TEST_PAYLOAD_LEN; i ++) {
        array[i] = i;
        origin_array[i] = i;
    }

    ret = prov_msg_pack_req(PROV_MSG_CRYPTO_AES_CBC, 0x6878f43a, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    ret = prov_msg_depack_req(&crypto, &crc32, array, &len);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_EQUAL(crypto, PROV_MSG_CRYPTO_AES_CBC);
    CU_ASSERT_EQUAL(crc32, 0x6878f43a);
    CU_ASSERT_EQUAL(utils_memcmp(array, origin_array, TEST_PAYLOAD_LEN), 0);
}

static void test_prov_msg_return_len(void)
{
    int32_t ret = 0;
    uint8_t array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    uint8_t origin_array[TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN] = {0};
    size_t len = TEST_PAYLOAD_LEN;
    uint32_t crypto = 0;
    uint32_t crc32 = 0;

    ret = prov_msg_pack_req(crypto, crc32, array, &len);
    CU_ASSERT_EQUAL(len, TEST_PAYLOAD_LEN + PROV_MSG_HEADER_LEN);
}

int32_t test_prov_msg_entry()
{
    int32_t ret = 0;
    void *rp = NULL;
    CU_pSuite p_prov_msg = NULL;

    p_prov_msg = CU_add_suite("Prov Msg", test_prov_msg_init, test_prov_msg_cleanup);
    UTILS_CHECK_CONDITION(NULL == p_prov_msg, -1, "p_prov_msg add suite failed!\n");

    rp = CU_add_test(p_prov_msg, "test of test_prov_msg_return_len ()", test_prov_msg_return_len);
    UTILS_CHECK_CONDITION(NULL == rp, -1, "CU_add_test failed!\n");

    rp = CU_add_test(p_prov_msg, "test of test_prov_msg_pack_and_repack_20480 ()", test_prov_msg_pack_and_repack_20480);
    UTILS_CHECK_CONDITION(NULL == rp, -1, "CU_add_test failed!\n");

    rp = CU_add_test(p_prov_msg, "test of test_prov_msg_pack_and_repack_1024 ()", test_prov_msg_pack_and_repack_1024);
    UTILS_CHECK_CONDITION(NULL == rp, -1, "CU_add_test failed!\n");

    rp = CU_add_test(p_prov_msg, "test of test_prov_msg_pack_and_repack_223 ()", test_prov_msg_pack_and_repack_223);
    UTILS_CHECK_CONDITION(NULL == rp, -1, "CU_add_test failed!\n");

finish:
    return ret;
}
