#include "hal_com.h"
#include "canbus_agent_entry.h"
#include "utils.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include "prov_msg.h"
#include "pal_cipher.h"
#include <string.h>

#define EPOLL_SIZE 1024
#define CAN_AGENT_RECV_BUFFER_SIZE (1024u)

struct _can_agent_t {
    int32_t epoll_fd;
    int32_t can_fd;
    HAL_COM_T com;
    pthread_t can_thread;
    pthread_mutex_t recv_mutex;
};

int32_t can_agent_init(CAN_AGENT_T *ctx, const char *can_name, void* recv_func_entry)
{
    int32_t ret = 0;
    HAL_COM_T *com_ctx = NULL;
    struct epoll_event event;

    UTILS_ASSERT_MSG(NULL == ctx,
                     "ctx is NULL!\n");
    UTILS_ASSERT_MSG(NULL == can_name,
                     "can_name is NULL!\n");
    UTILS_ASSERT_MSG(NULL == recv_func_entry,
                     "recv_func_entry is NULL!\n");

    com_ctx = &ctx->com;

    /* init hal com */
    ret = hal_com_init(com_ctx, can_name);
    UTILS_CHECK_RET(ret, "ERROR on hal_com_init\n");

    /* init epoll_fd for recv thread. */
    ctx->epoll_fd = epoll_create(EPOLL_SIZE);
    UTILS_CHECK_CONDITION(ctx->epoll_fd < 0,
                          HAL_ERR_FILE_IOCTL,
                          "ERROR on epoll_create\n");

    ret = hal_com_get_fd(com_ctx, &ctx->can_fd);
    UTILS_CHECK_RET(ret, "ERROR on hal_com_get_fd\n");

    event.data.fd = ctx->can_fd;
    event.events = EPOLLIN;

    ret = epoll_ctl(ctx->epoll_fd,
                    EPOLL_CTL_ADD,
                    ctx->can_fd,
                    &event);
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_FILE_IOCTL,
                          "ERROR on epoll_ctl\n");

    /* create recv thread. */
    ctx->can_thread = -1;
    ret = pthread_create(&ctx->can_thread, NULL, recv_func_entry, ctx);
    UTILS_CHECK_CONDITION(ret < 0,
                          HAL_ERR_DEVICE_BUSY,
                          "ERROR on epoll_ctl\n");
finish:
    return ret;
}

void can_agent_free(CAN_AGENT_T *ctx)
{
    if (NULL == ctx) {
        return;
    }
    if (ctx->epoll_fd != 0) {
        close(ctx->epoll_fd);
    }

    hal_com_free(&ctx->com);
}

static void canrecive(int can_id, uint8_t *buf, int buf_len)
{
    utils_print_rcv_frame(buf, buf_len, can_id);
}

static void *canbus_recv_msg(void *param)
{
    int32_t ret = 0;
    int32_t nfds = 0;
    int32_t timeout = 2;
    size_t nbytes = HAL_COM_DATA_BLOCK_SIZE;
    uint8_t buffer[CAN_AGENT_RECV_BUFFER_SIZE];
    int32_t frameid = 0;
    CAN_AGENT_T *ctx = param;
    struct epoll_event events[EPOLL_SIZE];

    UTILS_ASSERT_MSG(NULL == ctx,
                     "The ctx is NULL!\n");

    UTILS_LOG("The canbus_recv_msg is called, join the recv thread.\n");

    while (1) {
        nfds = epoll_wait(ctx->epoll_fd, events, EPOLL_SIZE, timeout);
        UTILS_CHECK_CONDITION(nfds < 0,
                              HAL_ERR_FILE_IOCTL,
                              "Failed on epoll_wait!\n");

        for (int i = 0; i < nfds; i ++) {
            if (events[i].events & EPOLLIN) {
                ret = hal_com_read_timeout(&ctx->com, buffer, &nbytes, 200);
                UTILS_CHECK_RET(ret, "Read data failed\n");
                ret = hal_com_get_portnum(&ctx->com, &frameid);
                UTILS_CHECK_RET(ret, "Get frame id failed\n");
                if (nbytes) {
                    canrecive(frameid, buffer, nbytes);
                }
                nbytes = HAL_COM_DATA_BLOCK_SIZE;
            }
        }
    }

finish:
    return;
}

int32_t canbus_agent_entry(void)
{
    int32_t ret = 0;

    // ret = test_hal_com_write();
    // UTILS_CHECK_CONDITION(ret != 0,
    //                       ret,
    //                       "failed on test_hal_com_write()!\n");


finish:
    return ret;
}

int32_t test_hal_com_write(void)
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
    hal_com_free(&com_ctx);
    return ret;
}

int32_t test_hal_com_read(void)
{
    int32_t ret = 0;
    CAN_AGENT_T ctx;

    ret = can_agent_init(&ctx, HAL_COM_NAME, canbus_recv_msg);
    UTILS_CHECK_RET(ret, "ERROR on hal_com_init\n");

    while (1) {
        UTILS_LOG("wait for recv !\n");
        sleep(3);
    }
finish:
    can_agent_free(&ctx);
    return ret;
}
