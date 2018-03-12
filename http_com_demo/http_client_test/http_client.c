/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This program fetches HTTP URLs.
 */

#include "mongoose.h"
#include "device_status.h"
int s_exit_flag = 0;
static int s_show_headers = 1;
static const char *s_show_headers_opt = "--show-headers";


char response_str[128];
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  //printf("evnt : %u\n", ev);
  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) ev_data != 0) {
        fprintf(stderr, "connect() failed: %s\n", strerror(*(int *) ev_data));
        s_exit_flag = 1;
      }
      break;
    case MG_EV_HTTP_REPLY:
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      if (s_show_headers) {
        //fwrite(hm->message.p, 1, hm->message.len - hm->method.len, stdout);
      } else {
        // fwrite(hm->body.p, 1, hm->body.len, stdout);
        // printf("hahahahm->method.len : %lu\r\n", hm->method.len);
        // fwrite(hm->method.p, 1, hm->method.len, stdout);

      }
      struct mg_str *host_hdr = mg_get_http_header(hm, "ServerResponse");
      memset(response_str, 0, sizeof(response_str));
      strncpy(response_str, host_hdr->p, host_hdr->len);
      s_exit_flag = 1;
      break;
    case MG_EV_CLOSE:
      if (s_exit_flag == 0) {
        printf("Server closed connection\n");
        s_exit_flag = 1;
      }
      break;
    default:
      break;
  }
}
/**
 * @brief 返回两个字符串之间的最小长度值
 * @param 
 *  const char *a,const char *b 为比较长度的两个字符串
 * @return 
 *  比较的两个字符串的长度的最小值
 * @notes 
 */
int minimum_len(const char *a, const char *b) {
  if(NULL == a || NULL == b) {
    return 0;
  }
  return strlen(a) > strlen(b)?strlen(b): strlen(a);
}
/**
 * @brief 对用户输入的cmd进行解析,正常有get_status,on,off,stadby等4种,异常cmd会报错
 * @param 
 *  char *cmd : 用户输入的cmd
 *  struct device_msg *dev : 需要进行命令处理的device
 * @return 
 * @notes 
 */
int user_cmd_analyze(char *cmd, struct device_msg *dev)
{
  char *on = "on";
  char *off = "off";
  char *standby = "standby";
  int ret;
  if(NULL == cmd) {
    return -1;
  }

  if(0 == strncmp("get_status", cmd, minimum_len(on, cmd))) {
    memset(response_str, 0, sizeof(response_str));
    ret = get_device_status(dev);
    if (ret < 0)
    {
      printf("get_device_status failed\r\n");
      return -1;
    }
    printf("dev status : %s\n", dev->power_status);
  }
  else if(0 == strncmp(on, cmd, minimum_len(off, cmd)) ||
          0 == strncmp(off, cmd, minimum_len(off, cmd)) ||
          0 == strncmp(standby, cmd, minimum_len(standby, cmd)) ) {
    memset(response_str, 0, sizeof(response_str));
    memset(dev->power_status, 0, sizeof(dev->power_status));
    memcpy(dev->power_status, cmd, strlen(cmd));
    ret = set_device_status(dev);
    if (ret < 0)
    {
      printf("get_device_status failed\r\n");
      return -1;
    }
  }
  else {
    printf("unknown cmd : %s\n", cmd);
    return -1;
  }
  printf("cmd : [%s] exeute success\n\n", cmd);
  return 0;
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i;
  mg_mgr_init(&mgr, NULL);

  /* Process command line arguments */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], s_show_headers_opt) == 0) {
      s_show_headers = 1;
    } else if (strcmp(argv[i], "--hexdump") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else {
      break;
    }
  }

  if (i + 1 != argc) {
    fprintf(stderr, "Usage: %s [%s] [--hexdump <file>] <URL>\n", argv[0],
            s_show_headers_opt);
    exit(EXIT_FAILURE);
  }

  //char *post_data = "http://www.baidu.com";
  printf("argv[%d] : %s\n", i, argv[i]);
  struct device_msg dev;
  //init dev
  memset(&dev, 0, sizeof(struct device_msg));
  memset(response_str, 0, sizeof(response_str));
  memcpy(&dev.ip_port, argv[i], strlen(argv[i])); 

  while(1) {
    printf("changge dev status:on/off/stadby/get_status\ninput:");
    char cmd_tmp[32];
    memset(cmd_tmp, 0, sizeof(cmd_tmp));
    scanf("%s", cmd_tmp);
    user_cmd_analyze(cmd_tmp, &dev);
  }
  return 0;
}
