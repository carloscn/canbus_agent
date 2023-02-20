#include <string.h>
#include "hal_com.h"
#include "hal_err.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <poll.h>

int32_t hal_com_init(HAL_COM_T *ctx, const char *parameters)
{
    int32_t ret = HAL_SUCCESS;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == parameters,
                     "parameters is NULL!\n");

    memset(ctx, 0, sizeof(HAL_COM_T));

    ctx->sock_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    UTILS_CHECK_CONDITION(ctx->sock_fd < 0,
                          HAL_ERR_FILE_ACCESS,
                          "Open can socket failed!\n");

    strcpy(ctx->ifr.ifr_name, parameters);

    ret = ioctl(ctx->sock_fd, SIOCGIFINDEX, &ctx->ifr);
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "IOCTL can socket failed!\n");

    ctx->can_addr.can_family = AF_CAN;
    ctx->can_addr.can_ifindex = ctx->ifr.ifr_ifindex;

    ret = bind(ctx->sock_fd,
               (struct sockaddr *)&ctx->can_addr,
               sizeof(ctx->can_addr));
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "bind can socket failed!\n");

    ret = setsockopt(ctx->sock_fd,
                     SOL_CAN_RAW,
                     CAN_RAW_FILTER,
                     &ctx->config.rfilter,
                     sizeof(ctx->config.rfilter));
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "setsockopt can socket failed!\n");

finish:
    return ret;
}

void hal_com_free(HAL_COM_T *ctx)
{
    if (NULL == ctx) {
        return;
    }
    if (ctx->sock_fd != 0) {
        close(ctx->sock_fd);
    }
    memset(ctx, 0, sizeof(HAL_COM_T));
}

int32_t hal_com_write(HAL_COM_T *ctx, uint8_t *buffer, size_t len)
{
    int32_t ret = HAL_SUCCESS;
    struct can_frame frame;
    size_t left = 0;
    ssize_t write_bytes = 0;
    size_t send_bytes = 0;
    size_t send_total_bytes = 0;

    UTILS_CHECK_LEN(len, "len is 0\n");
    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == buffer,
                     "buffer is NULL!\n");

    left = len;
    frame.can_id = ctx->current_can_id;

    do {
        send_bytes = (left <= CAN_MAX_DLEN) ? left : CAN_MAX_DLEN;
        memcpy((void *)&frame.data,
               (void *)(buffer + send_total_bytes),
               send_bytes);
        frame.can_dlc = send_bytes;
        write_bytes = write(ctx->sock_fd, &frame, sizeof(frame));
        UTILS_CHECK_CONDITION(write_bytes != sizeof(frame),
                              HAL_ERR_SEND_ERROR,
                              "The frame write bytes: %zu, the real write %d\n"
                              "The payload bytes: %zu\n",
                              sizeof(frame), write_bytes, send_bytes);
        left -= send_bytes;
        send_total_bytes += send_bytes;
        usleep(200);
    } while (left != 0);

finish:
    return ret;
}

int32_t hal_com_read(HAL_COM_T *ctx, uint8_t *buffer, size_t *len)
{
    int32_t ret = HAL_SUCCESS;
    struct can_frame frame;
    ssize_t left = 0;
    ssize_t recv_bytes = 0;
    ssize_t read_bytes = 0;
    ssize_t read_total_bytes = 0;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == buffer,
                     "buffer is NULL!\n");
    UTILS_ASSERT_MSG(NULL == len,
                     "len is NULL!\n");
    UTILS_CHECK_LEN(*len, "len is 0\n");

    ret = setsockopt(ctx->sock_fd,
                     SOL_CAN_RAW,
                     CAN_RAW_FILTER,
                     &ctx->config.rfilter,
                     sizeof(ctx->config.rfilter));
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "setsockopt can socket failed!\n");

    left = (ssize_t)*len;
    do {
        read_bytes = (left <= CAN_MAX_DLEN) ? left : CAN_MAX_DLEN;
        recv_bytes = read(ctx->sock_fd, &frame, sizeof(frame));
        if (recv_bytes < 0) {
            continue;
        } else if (recv_bytes == 0) {
            continue;
        }
        read_bytes = frame.can_dlc;
        ctx->current_can_id = frame.can_id;
        memcpy((void *)(buffer + read_total_bytes),
               (void *)&frame.data,
               read_bytes);
        left -= read_bytes;
        read_total_bytes += read_bytes;
    } while (left > 0);

    ret = 0;
    *len = (size_t)read_total_bytes;

finish:
    return ret;
}

int32_t hal_com_read_timeout(HAL_COM_T *ctx, uint8_t *buffer, size_t *len, ssize_t timeout_usec)
{
    int32_t ret = HAL_SUCCESS;
    struct can_frame frame;
    ssize_t left = 0;
    ssize_t recv_bytes = 0;
    ssize_t read_bytes = 0;
    ssize_t read_total_bytes = 0;
    struct pollfd pfd[1];
    bool timeout_flag = false;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == buffer,
                     "buffer is NULL!\n");
    UTILS_ASSERT_MSG(NULL == len,
                     "len is NULL!\n");
    UTILS_CHECK_LEN(*len, "len is 0\n");

    ret = setsockopt(ctx->sock_fd,
                     SOL_CAN_RAW,
                     CAN_RAW_FILTER,
                     &ctx->config.rfilter,
                     sizeof(ctx->config.rfilter));
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "setsockopt can socket failed!\n");

    pfd[0].fd = ctx->sock_fd;
    pfd[0].events = POLLIN;

    left = (ssize_t)*len;
    do {
        ret = poll(pfd, 1, timeout_usec);
        UTILS_CHECK_CONDITION(ret < 0,
                              HAL_ERR_FILE_IOCTL,
                              "poll failed!\n");
        if (ret == 0) {
            timeout_flag = true;
        } else if(pfd[0].revents & POLLIN) {
            read_bytes = (left <= CAN_MAX_DLEN) ? left : CAN_MAX_DLEN;
            recv_bytes = read(ctx->sock_fd, &frame, sizeof(frame));
            if (recv_bytes < 0) {
                UTILS_ERR("Read io failed\n");
                continue;
            } else if (recv_bytes == 0) {
                continue;
            }
            read_bytes = frame.can_dlc;
            ctx->current_can_id = frame.can_id;
            memcpy((void *)(buffer + read_total_bytes),
                   (void *)&frame.data,
                   read_bytes);
            left -= read_bytes;
            read_total_bytes += read_bytes;
        }
        //UTILS_LOG("left = %zd, read total = %zd!!!\n", left, read_total_bytes);
    } while (left > 0 && timeout_flag == false);

    ret = 0;
    *len = (size_t)read_total_bytes;
    UTILS_LOG("leave the hal_com_read_timeout!!!\n");
finish:
    return ret;
}

int32_t hal_com_set_config(HAL_COM_T *ctx, HAL_COM_CONFIG_T config)
{
    int32_t ret = HAL_SUCCESS;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");

    memcpy(&(ctx->config), &config, sizeof(HAL_COM_CONFIG_T));

    return ret;
}

int32_t hal_com_set_portnum(HAL_COM_T *ctx, int32_t port)
{
    int32_t ret = HAL_SUCCESS;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");

    ctx->current_can_id = (canid_t)port;

    return ret;
}

int32_t hal_com_get_portnum(HAL_COM_T *ctx, int32_t *port)
{
    int32_t ret = HAL_SUCCESS;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == port,
                     "port is NULL!\n");

    *port = (int32_t) ctx->current_can_id;

    return ret;
}

int32_t hal_com_get_fd(HAL_COM_T *ctx, int32_t *fd)
{
    int32_t ret = 0;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == fd,
                     "fd is NULL!\n");

    *fd = ctx->sock_fd;

    return ret;
}