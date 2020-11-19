// 链表entry
typedef struct tilink_entry_struct {
    tilink_node_struct * head;  // 链表尾node
    tilink_node_struct * tail;  // 链表头node
    unsigned long length;  // 当前链表一共多少个元素(size_t与unsigned long区别)
} tilink_entry_struct;
// 链表节点
typedef struct tilink_node_struct {
    tilink_node_struct * prev;
    tilink_node_struct * next;
    void * value;  // 该node的value保存到此
} tilink_node_struct;

// 创建一个空的list，不带任何node
tilink_entry_struct * link_init();
// 初始化一个node
tilink_node_struct * node_init();
// 添加一个head node
void * add_head(tilink_entry_struct *, tilink_node_struct *);
// 添加一个tail node
void * add_tail(tilink_entry_struct *, tilink_node_struct *);
// 添加到指定位置node
// 根据value获取node
tilink_node_struct * get_node_by_value(tilink_entry_struct *, void *);