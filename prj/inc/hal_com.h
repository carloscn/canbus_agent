#ifndef _HAL_COM_
#define _HAL_COM_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "hal_err.h"

#define HAL_COM_NAME ("can0")
#define HAL_COM_CAN_ID(_x) (_x)

typedef void (*callback_fn)(int32_t, uint8_t*, int32_t);

typedef struct _hal_com_config_t {
    struct can_filter rfilter;
} HAL_COM_CONFIG_T;

typedef struct _hal_com_ctx_t {
    int32_t sock_fd;
    int32_t epoll_fd;
    struct sockaddr_can can_addr;
    struct ifreq ifr;
    struct can_frame frame;
    canid_t current_can_id;
    HAL_COM_CONFIG_T config;
    callback_fn recive_callback;
} HAL_COM_T;

int32_t hal_com_init(HAL_COM_T *ctx, const char *parameters);
void hal_com_free(HAL_COM_T *ctx);
int32_t hal_com_read(HAL_COM_T *ctx, uint8_t *buffer, size_t *io_len);
int32_t hal_com_write(HAL_COM_T *ctx, uint8_t *buffer, size_t len);
int32_t hal_com_set_portnum(HAL_COM_T *ctx, int32_t port);
int32_t hal_com_get_portnum(HAL_COM_T *ctx, int32_t *port);
int32_t hal_com_set_config(HAL_COM_T *ctx, HAL_COM_CONFIG_T config);

#endif /* _HAL_COM_ */