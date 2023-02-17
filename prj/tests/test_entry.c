#include "test_entry.h"
#include "CUnit/Basic.h"
#include "test_prov_msg.h"
#include "utils.h"

int32_t test_entry()
{
    int32_t ret = 0;

    /* init test framework */
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    /* add test suite */
    ret = test_prov_msg_entry();
    UTILS_CHECK_RET(ret, "test_prov_msg_entry add failed!\n");

    ret = test_pal_cipher_entry();
    UTILS_CHECK_RET(ret, "test_pal_cipher_entry add failed!\n");

    /* run the tests */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

finish:
    CU_cleanup_registry();
    return CU_get_error();
}