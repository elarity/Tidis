#include <stdlib.h>
#include <string.h>
#include "tilink.h"

/*
 * @desc : 初始化一个空的link-entry，里边一个node都没有
 * */
tilink_entry_struct * link_init()
{
    tilink_entry_struct * tilink_entry_struct_point;
    tilink_entry_struct_point = (tilink_entry_struct *)malloc(sizeof(tilink_entry_struct));
    tilink_entry_struct_point->length = 0;
    tilink_entry_struct_point->head   = NULL;
    tilink_entry_struct_point->tail   = NULL;
    return tilink_entry_struct_point;
}

/*
 * @desc : 创建一个node，并赋值value
 * */
tilink_node_struct * node_init()
{
    tilink_node_struct * tilink_node_struct_point;
    tilink_node_struct_point = (tilink_node_struct *)malloc(sizeof(tilink_node_struct));
    return tilink_node_struct_point;
}

/*
 * @desc : 将node追加到head
 * */
void * add_head(tilink_entry_struct * tilink_entry_point, tilink_node_struct * new_node)
{
    if (0 == tilink_entry_point->length) {
        tilink_entry_point->tail = new_node;
        tilink_entry_point->head = new_node;
        tilink_entry_point->length++;
    } else {
        tilink_node_struct * head_node;
        head_node = tilink_entry_point->head;
        new_node->next = head_node;
        tilink_entry_point->head = new_node;
        tilink_entry_point->length++;
    }
}

/*
 * @desc : 将node追加到tail
 * */
void * add_tail(tilink_entry_struct * tilink_entry_point, tilink_node_struct * new_node)
{
    if (0 == tilink_entry_point->length) {
        tilink_entry_point->tail = new_node;
        tilink_entry_point->head = new_node;
        tilink_entry_point->length++;
    } else {
        tilink_node_struct * tail_node;
        tail_node = tilink_entry_point->tail;
        new_node->prev = tail_node;
        tilink_entry_point->tail = new_node;
        tilink_entry_point->length++;
    }
}

/*
 * @desc : 根据index获取node, O(N)
 * */
tilink_node_struct * get_node_by_index(tilink_entry_struct * tilink_entry_point, unsigned long index)
{
    tilink_node_struct * node;
    unsigned long pos = 1;
    unsigned long length;
    length = get_link_length(tilink_entry_point);
    // 验证下长度
    if (index > length) {
        return NULL;
    }
    node = tilink_entry_point->head;
    while(pos < index) {
        node = node->next;
        pos++;
    }
    return node;
}

/*
 * @desc : 根据value获取node
 * */
tilink_node_struct * get_node_by_value(tilink_entry_struct * tilink_entry_point, void * value)
{
    tilink_node_struct * node;
    tilink_iterator_struct iterator;
    reset_link_2_head(tilink_entry_point, &iterator);
    // 开始遍历整个link，与value数值进行对比
    while((node = link_next(&iterator)) != NULL) {
        if (value == node->value) {
            return node;
        }
    }
    return NULL;
}

/*
 * @desc : 一般用于while不断next
 * */
tilink_node_struct * link_next(tilink_iterator_struct * tilink_iterator_point)
{
    tilink_node_struct * current_node;
    current_node = tilink_iterator_point->next;
    if (current_node != NULL) {
        if (ITERATOR_TAIL_TO_HEAD == tilink_iterator_point->direction)
            tilink_iterator_point->next = current_node->next;
        else
            tilink_iterator_point->next = current_node->prev;
    }
    return current_node;
}

/*
 * @desc : rewind link-list到head处
 * */
void reset_link_2_head(tilink_entry_struct * tilink_entry_point, tilink_iterator_struct * tilink_iterator_point)
{
    tilink_iterator_point->next      = tilink_entry_point->head;
    tilink_iterator_point->direction = 0;
}

/*
 * @desc : 返回头部node
 * */
tilink_node_struct * get_link_head(tilink_entry_struct * tilink_entry_point)
{
    return tilink_entry_point->head;
}

/*
 * @desc : 返回尾巴node
 * */
tilink_node_struct * get_link_tail(tilink_entry_struct * tilink_entry_point)
{
    return tilink_entry_point->tail;
}

/*
 * @desc : 获取link长度
 * */
unsigned long get_link_length(tilink_entry_struct * tilink_entry_point)
{
    return tilink_entry_point->length;
}