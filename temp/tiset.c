#include <stdlib.h>
#include <string.h>
#include "tiset.h"

static uint8_t _get_value_encode(int64_t value);
static uint8_t _get_position_by_value(tiset_struct * tiset_p, int64_t value, uint32_t * position);
static tiset_struct * resize_tiset_memory(tiset_struct * tiset_p, uint32_t length);
static void _tiset_set(tiset_struct * tiset_p, uint32_t index, int64_t value);

/*
 * @desc : 创建一个空的set
 * */
tiset_struct * tiset_init()
{
    tiset_struct * tiset_p;
    tiset_p = (tiset_struct *)malloc(sizeof(tiset_struct));
    //tiset_p->length = 0;
    memset(tiset_p, '\0', sizeof(tiset_struct));
    return tiset_p;
}

/*
 * @desc : 插入数据
 * */
tiset_struct * tiset_add(tiset_struct * tiset_p, int64_t value)
{
    // 初始化变量
    uint8_t  value_encode;
    uint8_t  ret;
    uint32_t position;

    value_encode = _get_value_encode(value);
    //tiset_p->encode = value_encode;

    // ENCODE升级.
    if (value_encode > tiset_p->encode) {

    // ENCODE不需要升级
    } else {
        // 一、首先二分查找当前value是否在set中，如果在返回pos位置index信息
        // 不做任何操作，直接返回set指针
        ret = _get_position_by_value(tiset_p, value, &position);
        if (ret) {
            return tiset_p;
        }
        // 二、当前set中不存在该value，申请新的大空间
        resize_tiset_memory(tiset_p, (tiset_p->length + 1));
        // 三、将value添加到被分配内存大空间后的区域中
    }
    _tiset_set(tiset_p, position, value);
    tiset_p->length++;
    return tiset_p;
}

/*
 * @desc : 判断插入value的int范围类型
 * */
static uint8_t _get_value_encode(int64_t value)
{
    if (value < INT32_MIN || value > INT32_MAX)
        return ENCODE_INT64;
    else if (value < INT16_MIN || value > INT16_MAX)
        return ENCODE_INT32;
    else
        return ENCODE_INT16;
}

/*
 * @desc : 二分查找.如果没有找到，position返回0；同时返回position指出方向
 * */
static uint8_t _get_position_by_value(tiset_struct * tiset_p, int64_t value, uint32_t * position)
{
    uint32_t min, max, mid;
    int32_t current;
    min = 0;
    max = tiset_p->length - 1;
    // 压根无数据.
    if (0 == tiset_p->length) {
        *position = 0;
        return 0;
    }
    // value已经大于了set中的最大值
    if (value > get_value_by_index(tiset_p, max)) {
        *position = max;
        return 0;
    }
    // value已经小于了set中的最小值
    if (value < get_value_by_index(tiset_p, min)) {
        *position = min;
        return 0;
    }

    // 开始二分法搜寻数据.
    while (min <= max) {
        mid = (min + max) >> 1;
        current = get_value_by_index(tiset_p, mid);
        // value落到 | 左侧 | 右侧 | 中的右侧
        if (value > current) {
            min = mid;
        // value落到 | 左侧 | 右侧 | 中的左侧
        } else if (value < current) {
            max = mid;
        } else {
            break;
        }
    }

    if (current == value) {
        *position = mid;
        return 1;
    } else {
        *position = min;
        return 0;
    }
}

/*
 * @desc : 重新分配内存.
 * */
static tiset_struct * resize_tiset_memory(tiset_struct * tiset_p, uint32_t length)
{
    //realloc(tiset_p, (tiset_p->length + 1));
    tiset_struct * new_tiset_p;
    new_tiset_p = realloc(tiset_p, sizeof(tiset_p) + length * tiset_p->encode);
    if (NULL == new_tiset_p)
        return NULL;
    free(tiset_p);
    return new_tiset_p;
}

/*
 * @desc : 返回index位置上的数值
 * */
int32_t get_value_by_index(tiset_struct * tiset_p, uint32_t index)
{
    int32_t value;
    return tiset_p->array[index];
}

/*
 * @desc : 在index位置上设置数值
 * */
static void _tiset_set(tiset_struct * tiset_p, uint32_t index, int64_t value) {
    if (tiset_p->encode == ENCODE_INT16)
        ((int16_t *)tiset_p->array)[index] = value;
    else if (tiset_p->encode == ENCODE_INT32)
        ((int32_t *)tiset_p->array)[index] = value;
    else
        ((int64_t *)tiset_p->array)[index] = value;
}
