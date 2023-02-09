#include <stdio.h>
#include "hal_com.h"
#include "utils.h"
#include "config.h"
#include "canbus_agent_entry.h"

int main(void)
{
    int32_t ret = 0;

    ret = canbus_agent_entry();
    UTILS_CHECK_CONDITION(ret != 0,
                          ret,
                          "failed on canbus_agent_entry()!\n");
finish:
    return ret;
}
