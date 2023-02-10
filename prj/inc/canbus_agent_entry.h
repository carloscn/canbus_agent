#ifndef _CANBUS_AGENT_ENTRY_
#define _CANBUS_AGENT_ENTRY_

#include "hal_com.h"

typedef struct _can_agent_t CAN_AGENT_T;

int32_t canbus_agent_entry(void);
int32_t test_hal_com_write(void);
int32_t test_hal_com_read(void);
#endif /* _CANBUS_AGENT_ENTRY_ */