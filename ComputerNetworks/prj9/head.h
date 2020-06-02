#ifndef __HEAD__
#define __HEAD__


#include <stdint.h>

#define IP_ENTRY_NUM 697882
#define CHUNK_SIZE 4 // IP地址分段，每段的长度
#define MULTI_CHILD_NUM (1 << CHUNK_SIZE) // multiple trie的分支数量

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


typedef struct
{
    u8 prefix;
    u8 port;
    u32 ip;
} ip_entry_t;

// binary tree
typedef struct btree_node
{
    ip_entry_t * val_p; // 指向ip数据库的指针，如果为NULL说明这个点不能匹配（PPT上面的黄色点）
    struct btree_node *parent; // 父节点
    struct btree_node *child[2]; // 2个子节点
}btree_node_t;


void init_binary_tree(ip_entry_t * ip_db);
void verify_btree(ip_entry_t *ip_db);
void insert_binary_tree(ip_entry_t *ip_entry_p);
btree_node_t *create_bnode();
ip_entry_t * lookup_btree(u32 ip);

u64 binary_space_counter;
btree_node_t * b_root;

// multiple trie
typedef struct multiple_trie_node
{
    ip_entry_t * val_p;
    struct multiple_trie_node *parent;
    struct multiple_trie_node *child[MULTI_CHILD_NUM];
} multitrie_node_t;

multitrie_node_t * m_root;
multitrie_node_t *create_mnode();
void insert_multiple_trie(ip_entry_t *ip_entry_p);
void init_multiple_trie(ip_entry_t * ip_db);
ip_entry_t * lookup_mtrie(u32 ip);
void verify_mtrie(ip_entry_t *ip_db);
u64 multitrie_space_counter;
u32 m_node_num;
u32 m_leaf_num;
void mtrie_statistic(multitrie_node_t * node_p);
void leaf_pushing(multitrie_node_t * m_node);

// poptrie
typedef struct poptrie_node // k=4, 向量长度为2^k=16 bit
{
    u16 leafvec;
    u16 vector;
    u32 base0;
    u32 base1;
} poptrie_node_t;

poptrie_node_t * p_root;
poptrie_node_t * N;
u64 * L;

void init_poptrie(ip_entry_t * ip_db);
poptrie_node_t *create_pnode();
void create_poptrie(multitrie_node_t *m_entry, poptrie_node_t *p_entry);
ip_entry_t * lookup_ptrie(u32 ip);
void verify_ptrie(ip_entry_t *ip_db);
// u64 poptrie_space_counter;
u32 p_node_num;
u32 p_leaf_num;
#endif