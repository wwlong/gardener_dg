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

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
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

  char*headers = "Content-Type: image/jpeg\r\n";
  flag_dbg("request : %s\n", argv[i]);
  //mg_connect_http(&mgr, ev_handler, argv[i], NULL, NULL);
  mg_connect_http(&mgr, ev_handler, argv[i], headers, jpeg_tmp);
  while (s_exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  free(jpeg_tmp);
  printf("end of free\n");
  return 0;
}
unsigned long long timeout_ms;
int save_to_file(const char *data, const int data_len) {
    char *data_buf = (char *)malloc(data_len);
    if(NULL == data_buf) {
        printf("data_buf malloc failed\n");
        return -1;
    }
    memset(data_buf, 0, data_len);
    mg_base64_decode((const unsigned char *)data, data_len, data_buf);
    struct timeval tmp;
    gettimeofday(&tmp, NULL);
    timeout_ms = tmp.tv_sec * 1000*1000+tmp.tv_usec;
    char file_name[64] = {0};
    snprintf(file_name, sizeof(file_name), "%llu.jpg", timeout_ms);
    FILE *fp = fopen(file_name, "wb");
    if(NULL == fp) {
        printf("file open failed\n");
        return -1;
    }
    fwrite(data_buf, 1, data_len, fp);
    fflush(fp);
    safe_free(data_buf);
    fclose(fp);
    return 0;
}

int save_to_file_no_base64(const char *data, const int data_len) {
    struct timeval tmp;
    gettimeofday(&tmp, NULL);
    timeout_ms = tmp.tv_sec * 1000*1000+tmp.tv_usec;
    char file_name[64] = {0};
    snprintf(file_name, sizeof(file_name), "%llu.jpg", timeout_ms);
    FILE *fp = fopen(file_name, "wb");
    if(NULL == fp) {
        printf("file open failed\n");
        return -1;
    }
    fwrite(data, 1, data_len, fp);
    fflush(fp);
    fclose(fp);
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
    pic_buf = (char *)malloc(file_size);
    if(NULL == pic_buf) {
        flag_dbg("pic buf malloc failed\n");
        fclose(fp);
        return NULL;
    }
    memset(pic_buf, 0, file_size);
    fseek(fp ,0 ,SEEK_SET);
    fread(pic_buf, 1, file_size, fp);
    fclose(fp);
    /*
      将pic_buf内容保存到文件看是否是图片
    */
    save_to_file_no_base64(pic_buf, file_size);
    /* 对jpeg进行base64编码*/
    char *base64_buf = (char *)malloc(file_size * 2);
    if(NULL == base64_buf) {
      printf("base64_buf malloc failed\n");
      safe_free(pic_buf);
      return NULL;
    }
    memset(base64_buf, 0, file_size*2);
    mg_base64_encode((unsigned char *)pic_buf, file_size, base64_buf);
    /*
      就地base64_decode
    */
    save_to_file(base64_buf, file_size*2);
    safe_free(pic_buf);
    return base64_buf;
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