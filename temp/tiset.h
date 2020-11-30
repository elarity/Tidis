#include <stdint.h>
#define ENCODE_INT16 (sizeof(int16_t))
#define ENCODE_INT32 (sizeof(int32_t))
#define ENCODE_INT64 (sizeof(int64_t))

typedef struct tiset_struct {
    int32_t encode;
    int32_t length;
    int8_t array[];
} tiset_struct;

// 创建一个set
tiset_struct * tiset_init();
// 插入数据
tiset_struct * tiset_add(tiset_struct *, int64_t);
// 根据index获取value
int32_t get_value_by_index(tiset_struct *, uint32_t);