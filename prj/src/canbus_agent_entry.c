#include "hal_com.h"
#include "canbus_agent_entry.h"
#include "utils.h"
 #include <unistd.h>

int32_t canbus_agent_entry(void)
{
    int32_t ret = 0;
    size_t i = 0;
    HAL_COM_T com_ctx;
    uint8_t tx_buffer[1024] = {0};
    size_t tx_len = 1021;

    for (i = 0; i < 1024; i ++) {
        tx_buffer[i] = i & 0xFF;
    }

    ret = hal_com_init(&com_ctx, HAL_COM_NAME);
    UTILS_CHECK_RET(ret, "ERROR on hal_com_init\n");

    ret = hal_com_set_portnum(&com_ctx, HAL_COM_CAN_ID(1));
    UTILS_CHECK_RET(ret, "ERROR on hal_com_set_portnum\n");

    tx_len = 1021;
    i = 0;
    while (1) {
        UTILS_LOG("set frame id is %zu\n", i);
        ret = hal_com_set_portnum(&com_ctx, HAL_COM_CAN_ID(i));
        UTILS_CHECK_RET(ret, "ERROR on hal_com_set_portnum\n");

        ret = hal_com_write(&com_ctx, tx_buffer, tx_len);
        UTILS_CHECK_RET(ret, "ERROR on hal_com_write\b");
        sleep(1);
        UTILS_LOG("send tx can data %zu\n", tx_len);
        i ++;
    }
finish:
    return ret;
}