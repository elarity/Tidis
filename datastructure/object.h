#ifndef __TIDIS_OBJECT_H
#define __TIDIS_OBJECT_H

// tidis object struct
typedef struct tidis_object_struct {
    int type;    // 类型，目前只有字符串.
    void * ptr;  // 指向的数据指针
} tidis_object_struct;

#endif