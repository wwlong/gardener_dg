#ifndef MACRO_DEBUG_H_
#define MACRO_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define flag_dbg(fmt...)   \
    do {\
        printf("\e[1;31m [%s] -- [%d]: \e[0m", __FUNCTION__, __LINE__);\
        printf(fmt);\
    }while(0)
    
#define safe_free(ptr) \
do{\
    if(NULL != ptr) \
    {\
        free(ptr);\
    }\
}while(0)

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif