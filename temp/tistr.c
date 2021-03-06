#include <stdlib.h>
#include <string.h>
#include "tistr.h"
/*
 * @desc : 初始化tistr struct结构体
 * @param : 来源二进制buf
 */
tistr_struct * tistr_init(char * buf)
{
    // 初始化各种变量
    size_t buf_length;
    tistr_struct * tistr_struct_p
    // 一、初始化好tistr_struct
    tistr_struct_p = (tistr_struct *)malloc(sizeof(tistr_struct));
    // 二、注意：初始化了tistr_struct但是并没有为其中的buf成员分配内存
    // sizeof并不记录末尾null也就是空字节的长度
    buf_length = sizeof(buf);
    tistr_struct_p->length = 0;
    tistr_struct_p->free   = buf_length;
    tistr_struct_p->buf    = (char *)malloc(buf_length + 1);
    // 三、拼接字符串
    tistr_strcat(tistr_struct_p, buf);
    return tistr_struct_p;
}

/*
 * @desc : 将二进制字符们复制到tistr结构中去
 * */
void tistr_strcat(tistr_struct * tistr_struct_pointer, char * source_buf)
{
    // 初始化各种变量
    size_t buf_length;
    char * new_buf;
    char * old_buf;
    // 一、根据tistr-struct结构体free判断空间是否足够分配
    buf_length = sizeof(source_buf);
    if (tistr_struct_pointer->free >= buf_length) {
        tistr_struct_pointer->free   = tistr_struct_pointer->free - buf_length;
        tistr_struct_pointer->length = tistr_struct_pointer->length + buf_length;
        strcat(tistr_struct_pointer->buf, source_buf);
    // 二、如果tistr-struct中空间不足以放下buf，就涉及到扩容了
    } else {
        new_buf = tistr_expansion(tistr_struct_pointer, buf_length);
        strcpy(new_buf, tistr_struct_pointer->buf);
        strcat(new_buf, source_buf);
        // 释放之前旧的buf内存
        old_buf = tistr_struct_pointer->buf;
        free(old_buf);
        // 走上新开辟的大buf
        tistr_struct_pointer->buf = new_buf;
    }
}

/*
 * @desc : 将字符们复制到tistr结构中去
 * */
void tistr_strcpy(tistr_struct * tistr_struct_pointer, char * source_buf)
{
    //初始化各种变量
    size_t source_buf_length;
    char * new_buf;
    char * old_buf;
    // 一、根据tistr-struct结构体free判断空间是否足够分配
    source_buf_length = sizeof(source_buf);
    // 空间刚好够用.
    if ((tistr_struct_pointer->free + tistr_struct_pointer->length) >= source_buf_length) {
        memset(tistr_struct_pointer->buf, '\0', tistr_struct_pointer->free + tistr_struct_pointer->length);
        strcpy(tistr_struct_pointer->buf, source_buf);
        tistr_struct_pointer->length = source_buf_length;
        tistr_struct_pointer->free   = 0;
    } else {
        new_buf = tistr_expansion(tistr_struct_pointer, source_buf_length);
        strcpy(new_buf, source_buf);
        // 释放之前旧的buf内存
        old_buf = tistr_struct_pointer->buf;
        free(old_buf);
        // 走上新开辟的大buf
        tistr_struct_pointer->buf = new_buf;
    }
}

/*
 * @desc : 扩容算法
 * */
char * tistr_expansion(tistr_struct * tistr_struct_pointer, size_t buf_length)
{
    char * new_buf;
    size_t new_length;
    size_t new_free;
    //1048576B = 1MB
    new_length = tistr_struct_pointer->length + buf_length;
    // new_length小于等于1MB
    // new_length大于1MB
    new_buff = tistr_struct_pointer->length + buf_length <= 1048576 ? new_length : 1048576 ;
    char * new_buf = (char *)malloc(new_length + new_free + 1);
    tistr_struct_pointer->length = new_length;
    tistr_struct_pointer->free   = new_free;
    return new_buf;
}

/*
 * @desc : 获取已占用长度（不包括末尾\0）
 * */
size_t tistr_get_length(tistr_struct * tistr_struct_pointer)
{
    return tistr_struct_pointer->length;
}

/*
 * @desc : 获取剩余可用长度
 * */
size_t tistr_get_free(tistr_struct * tistr_struct_pointer)
{
    return tistr_struct_pointer->free;
}

/*
 * @desc : 清空一个tistr结构体，并且释放内存空间
 */
void tistr_truncate(tistr_struct * tistr_struct_pointer)
{
    free(tistr_struct_pointer->buf);
    tistr_struct_pointer->length = 0;
    tistr_struct_pointer->free   = 0;
    free(tistr_struct_pointer);
}

/*
 * @desc : 清空一个tistr结构体的buf，但不释放空间
 */
void tistr_empty(tistr_struct * tistr_struct_pointer)
{
    tistr_struct_pointer->length = 0;
    tistr_struct_pointer->free   = 0;
    free(tistr_struct_pointer->buf);
}