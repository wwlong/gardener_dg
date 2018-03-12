#include <stdint.h>
#include "device_status.h"

void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
/**
 * @brief 根据设备ID查询其实际状态
 * @param 
 *      char*device_id : 设备的ID号,关键值,唯一
 *      char *status : 此处返回设备的状态信息
 *                     on/off/standby
 * @return 
 *      success : 0
 *      failed : -1
 * @notes 
 */
int get_device_status(struct device_msg*dev)
{
    struct mg_mgr mgr;
    if(NULL == dev) {
        printf("NULL == dev\n");
        return -1;
    }
    int retry_times = 0;
    int total_retrytimes = 5;
    char cmd_to_serv[64];
    memset(cmd_to_serv, 0, sizeof(cmd_to_serv));
    sprintf(cmd_to_serv, "get_status");
    mg_mgr_init(&mgr, NULL);

    mg_connect_http(&mgr, ev_handler, dev->ip_port, NULL, cmd_to_serv);
    while (s_exit_flag == 0)
    {
        mg_mgr_poll(&mgr, 1000);
        retry_times ++;
        if(retry_times > total_retrytimes) {
            //表示获取信息超时
            mg_mgr_free(&mgr);
            return 1;
            //break;
        }
    }
    //清理资源,恢复现场
    mg_mgr_free(&mgr);
    //此时已经收到response
    s_exit_flag = 0;
    memcpy(dev->power_status, response_str, strlen(response_str));
    return 0;
}

/**
 * @brief 根据deviceid设置设备的状态
 * @param 
 *      char*device_id : 设备的ID号,关键值,唯一
 *      char *status : 此处为将要设置的设备的状态信息
 *                     on/off/standby 
 * @return 
 *      success : 0
 *      failed : -1
 * @notes 
 */
int set_device_status(struct device_msg*dev) 
{
    struct mg_mgr mgr;
    if (NULL == dev)
    {
        printf("NULL == dev\n");
        return -1;
    }
    int retry_times = 0;
    int total_retrytimes = 5;
    char cmd_to_serv[64];
    memset(cmd_to_serv, 0, sizeof(cmd_to_serv));
    sprintf(cmd_to_serv, "%s", dev->power_status);
    mg_mgr_init(&mgr, NULL);
    printf("cmd_to_dev :%s", cmd_to_serv);
    mg_connect_http(&mgr, ev_handler, dev->ip_port, NULL, cmd_to_serv);
    while (s_exit_flag == 0)
    {
        mg_mgr_poll(&mgr, 1000);
        retry_times ++;
        if(retry_times > total_retrytimes) {
            //表示获取信息超时
            mg_mgr_free(&mgr);
            s_exit_flag = 0;
            return 1;
            //break;
        }
    }
    //清理资源,恢复现场
    mg_mgr_free(&mgr);
    //此时已经收到response
    s_exit_flag = 0;
    memcpy(dev->power_status, response_str, strlen(response_str));
    return 0;
}