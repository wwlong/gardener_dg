#ifndef DEVICE_STATUS_H_
#define DEVICE_STATUS_H_
#include "mongoose.h"

/*
  global var
*/
extern char response_str[128];

extern int s_exit_flag;
/*
  定义操作的状态
  device_id:on
  device_id:off
  device_id:standby
*/
enum device_status_e {
  device_on,
  device_off,
  device_stanby
};
/*
  设备的信息
  设备的ID
  设备的当前电源状态
*/
struct device_msg{
  //char id[32];
  char power_status[16];
  char ip_port[128];
  bool get_response; //服务器返回了查询的信息
};

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
int get_device_status(struct device_msg*dev);

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
int set_device_status(struct device_msg*dev);

#endif 