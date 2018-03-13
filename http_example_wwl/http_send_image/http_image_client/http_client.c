/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This program fetches HTTP URLs.
 */
/* Includes ------------------------------------------------------------------*/
#include "mongoose.h"
//#include "image_send.h"
#include "macro_debug.h"
/* Private typedef -----------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
int s_exit_flag = 0;
int s_show_headers = 0;
const char *s_show_headers_opt = "--show-headers";
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static char* image_get(const char *path);
/* Private functions ---------------------------------------------------------*/

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

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
        fwrite(hm->message.p, 1, hm->message.len, stdout);
      } else {
        fwrite(hm->body.p, 1, hm->body.len, stdout);
      }
      putchar('\n');
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
  /*
    初始化
  */
  char *jpeg_path = "./snap_jpg22.jpg";
  char *jpeg_tmp = image_get(jpeg_path);
  if(NULL == jpeg_tmp) {
    flag_dbg("image_get failed\n");
    return -1;
  }
  char*headers = "Content-Type: image/jpeg";
  flag_dbg("request : %s\n", argv[i]);
  mg_connect_http(&mgr, ev_handler, argv[i], headers, jpeg_tmp);
  flag_dbg("wait for response\n");
  while (s_exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}

/**
 * @brief  从path读取一张图片 
 * @param 
 *      const char *path,picture的具体路径
 * @return 
 *      成功返回0
 *      失败返回-1
 * @notes 
 *
 */ 
static char* image_get(const char *path) {
    if(NULL == path) {
        printf("path is NULL\n");
        return NULL;
    } 
    //struct list_head *pos, *q;
    /*
     *  打开图片文件获取内容
     * */
//#define PIC_BUF_LEN 64*1024
//    char pic_buf[PIC_BUF_LEN];
    char *pic_buf = NULL;
    FILE *fp = fopen(path, "rb");
    if(NULL == fp) {
        printf("file : %s open failed\n", path);
        return NULL;
    }
    /*
     *  获取整个文件的大小
     * */
    fseek(fp ,0 ,SEEK_END);
    long file_size = ftell(fp);
    pic_buf = (char *)malloc(sizeof(file_size));
    if(NULL == pic_buf) {
        flag_dbg("pic buf malloc failed\n");
        fclose(fp);
        return NULL;
    }
    memset(pic_buf, 0, sizeof(file_size));
    fread(pic_buf, 1, file_size, fp);
    fclose(fp);

    return pic_buf;
    /*
        目前不进行base64加密
    */
    /*
        填充数据
        塞入队列
    */
    // image_data_t *data_tmp = (image_data_t *)malloc(sizeof(image_data_t));
    // if (NULL == data_tmp)
    // {
    //     flag_dbg("malloc failed\n");
    //     return -1;
    // }
    // data_tmp->data = pic_buf;
    // pthread_mutex_lock(&image_queue_lock);
    // list_add_tail(&(data_tmp->list), &(image_data_queue->list));
    // pthread_mutex_unlock(&image_queue_lock);
    /*
        通过网络直接发送
    */
}