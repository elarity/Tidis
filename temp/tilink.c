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
 * @desc : 根据value获取node
 * */
tilink_node_struct * get_node_by_value(tilink_entry_struct * tilink_entry_point, void * value)
{
    unsigned long length;
    length = tilink_entry_point->length;


}