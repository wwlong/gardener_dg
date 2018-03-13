#include <stdio.h>
#include <pthread.h>
#include "image_send.h"
#include "linux_list.h"
pthread_mutex_t image_queue_lock = PTHREAD_MUTEX_INITIALIZER;
/**
 * @brief   image_send_init 
 *          初始化一个链表
 * @param 
 *          void
 * @return
 *          0 -- success
 *          -1 -- failed
 * @notes
 *
 */
image_data_t *image_data_queue;
image_data_t *image_send_init(void)
{
    image_data_t *image_data_first = (image_data_t *)malloc(sizeof(image_data_t));
    if (NULL == image_data_first)
    {
        printf("%s init failed\n", __FUNCTION__);
        return -1;
    }
    INIT_LIST_HEAD(&(image_data_first->list));
    return image_data_first;
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
char* image_get(const char *path) {
    if(NULL == path) {
        printf("path is NULL\n");
        return -1;
    } 
    struct list_head *pos, *q;
    /*
     *  打开图片文件获取内容
     * */
//#define PIC_BUF_LEN 64*1024
//    char pic_buf[PIC_BUF_LEN];
    char *pic_buf = NULL;
    FILE *fp = fopen(path, "rb");
    if(NULL == fp) {
        printf("file : %s open failed\n", path);
        return -1;
    }
    /*
     *  获取整个文件的大小
     * */
    fseek(fp ,0 ,SEEK_END);
    long file_size = ftell(fp);
    pic_buf = (char *)malloc(sizeof(file_size));
    if(NULL == pic_buf) {
        flag_dbg("pic buf malloc failed\n");
        return -1;
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

/**
 * @brief
 * @param
 * @return
 * @notes
 *
 */

/**
 * @brief   image_send_deinit
 *          解初始化
 * @param 
 *          void
 * @return
 *          0 -- success
 *          -1 -- failed
 * @notes
 *
 */
int image_send_deinit(void) {
    struct list_head *pos, *q;
}