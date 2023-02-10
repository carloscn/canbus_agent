#include <stdio.h>
#include "hal_com.h"
#include "utils.h"
#include "config.h"
#include "canbus_agent_entry.h"

#define UNIT_TEST 1

int main(void)
{
    int32_t ret = 0;

    ret = canbus_agent_entry();
    UTILS_CHECK_CONDITION(ret != 0,
                          ret,
                          "failed on canbus_agent_entry()!\n");
#if UNIT_TEST
    // ret = test_hal_com_write();
    // UTILS_CHECK_CONDITION(ret != 0,
    //                       ret,
    //                       "failed on test_hal_com_write()!\n");
    test_hal_com_read();
#endif
finish:
    return ret;
}
