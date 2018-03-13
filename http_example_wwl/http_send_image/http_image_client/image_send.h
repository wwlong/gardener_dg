#ifndef IMAGE_SEND_H_
#define IMAGE_SEND_H_

#include "linux_list.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
    http image send module
    此处设置为从文件读取image
    读取的内容塞入队列
    另一侧从队列读取image,base64编码,通过http发送
*/
typedef struct image_data {
    int type;
    int size;
    char *data;
    struct list_head list; 
}image_data_t;
//
int image_get_from_file(const char *path);

#ifdef __cplusplus
}
#endif

#endif