#define ITERATOR_HEAD_TO_TAIL 0
#define ITERATOR_TAIL_TO_HEAD 1
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
// 遍历用的临时节点结构
typedef struct tilink_iterator_struct {
    tilink_node_struct * next;
    int direction;  // 表示方向 0表示从头到尾，1表示从尾到头
} tilink_iterator_struct;

// 创建一个空的list，不带任何node
tilink_entry_struct * link_init();
// 初始化一个node
tilink_node_struct * node_init();
// 添加一个head node
void add_head(tilink_entry_struct *, tilink_node_struct *);
// 添加一个tail node
void add_tail(tilink_entry_struct *, tilink_node_struct *);
// 添加到指定位置node
// 根据index获取node
tilink_node_struct * get_node_by_index(tilink_entry_struct *, unsigned long);
// 根据value获取node
tilink_node_struct * get_node_by_value(tilink_entry_struct *, void *);
// 遍历link的公用方法，双向
tilink_node_struct * link_next(tilink_iterator_struct *);
// rewind link-list到head处
void reset_link_2_head(tilink_entry_struct *, tilink_iterator_struct *);
// 返回头部node
tilink_node_struct * get_link_head(tilink_entry_struct *);
// 返回尾巴node
tilink_node_struct * get_link_tail(tilink_entry_struct *);
// 获取link长度
unsigned long get_link_length(tilink_entry_struct *);