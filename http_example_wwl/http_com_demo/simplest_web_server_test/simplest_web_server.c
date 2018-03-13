// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "device_status.h"
static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static char dev_status[32];
/**
 * @brief 
 * @param 
 * @return 
 * @notes 
 */
int minimum_len(const char *a, const char *b) {
  if(NULL == a || NULL == b) {
    return 0;
  }
  return strlen(a) > strlen(b)?strlen(b): strlen(a);
}
/**
 * @brief 对来自client的请求进行解析,处理
 * @param 
 *    const char *request : 来自client的请求
 *    char *response : 保存处理的response信息
 *    int *response_len : 值-结果参数,入为reponse缓存大小,出为response有效数据字节数
 * @return 
 * @notes 
 */
int request_analyze(const char *request, char *response, int *response_len)
{
  // char *on = "on";
  // char *off = "off";
  // char *standy = "standby";
  if(NULL == request || NULL == response || *response_len < 32) {
    return -1;
  }
  memset(response, 0, *response_len);
  if(0 == strncmp("on", request, minimum_len("on", request))) {
      sprintf(response, "ServerResponse:%s", request);
      *response_len = strlen(request);
      sprintf(dev_status, "%s", "on");
  }
  else if(0 == strncmp("off", request, minimum_len("off", request))) {
      sprintf(response, "ServerResponse:%s", request);
      *response_len = strlen(request);
      sprintf(dev_status, "%s", "off");
  }
  else if(0 == strncmp("standby", request, minimum_len("standby", request))) {
      sprintf(response, "ServerResponse:%s", request);
      *response_len = strlen(request);
      sprintf(dev_status, "%s", "standby");
  }
  else if(0 == strncmp("get_status", request, minimum_len("get_status", request))) {
      sprintf(response, "ServerResponse:%s", dev_status);
      *response_len = strlen(request);
  }
  else {
      sprintf(response, "ServerResponse:%s", "unknows_request");
      *response_len = strlen("unknows_request");
  }
  return 0;
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  //printf("evnt : %u\n", ev);
  if (ev == MG_EV_HTTP_REQUEST) {
    //printf("request from client : %s -- %lu\n",((struct http_message *)p)->body.p, ((struct http_message *)p)->body.len );
    char request[64];
    char response[32];
    int response_len = 32;
    int ret; 

    memset(request, 0, sizeof(request));
    memcpy(request, ((struct http_message *)p)->body.p, ((struct http_message *)p)->body.len);
    printf("request : %s\r\n", request);
    //1.  分析消息和封装response
    ret = request_analyze(request, response, &response_len);
    if(ret < 0) {
      //
    }
    //反馈处理结果
    printf("response: %s\r\n", response);
    mg_send_response_line(nc, 200, response);
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
    memset(response, 0, sizeof(response));
    //返回一个 I'm fine 
  }

}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  //设定默认状态
  memset(dev_status, 0, sizeof(dev_status));
  sprintf(dev_status, "%s", "off");
  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
