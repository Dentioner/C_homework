#include <stdlib.h>
#include <stdio.h>
#include "head.h"



multitrie_node_t* create_mnode()
{
    multitrie_node_t* p = (multitrie_node_t*)malloc(sizeof(multitrie_node_t));
    p->parent = NULL;
    for (int index1 = 0; index1 < MULTI_CHILD_NUM; index1++)
        p->child[index1] = NULL;
    p->val_p = NULL;
    multitrie_space_counter++;
    return p;
}


void insert_multiple_trie(ip_entry_t* ip_entry_p)
{
    int index1, index2;
    multitrie_node_t* tmp_mnode1 = m_root;
    multitrie_node_t* tmp_mnode2 = NULL;

    for (index1 = CHUNK_SIZE; index1 < ip_entry_p->prefix; index1 += CHUNK_SIZE)
    {
        index2 = ((ip_entry_p->ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // 获取ip的第index1位
        if (tmp_mnode1->child[index2] == NULL) //说明这个节点还没建立
        {
            tmp_mnode2 = create_mnode();
            tmp_mnode2->parent = tmp_mnode1;
            tmp_mnode1->child[index2] = tmp_mnode2;
        }
        tmp_mnode1 = tmp_mnode1->child[index2];
    }

    // 到最后一层了，就是把数据放在这一层
    index2 = ((ip_entry_p->ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // 获取ip的第index1位
    if (index1 == ip_entry_p->prefix) // 掩码长度刚好合适
    {
        if (tmp_mnode1->child[index2] == NULL) //说明这个节点还没建立
        {
            tmp_mnode2 = create_mnode();
            tmp_mnode2->parent = tmp_mnode1;
            tmp_mnode1->child[index2] = tmp_mnode2;
            tmp_mnode2->val_p = ip_entry_p; // 登记
        }
        else // 记录已经存在
        {
            tmp_mnode2 = tmp_mnode1->child[index2];
            if (tmp_mnode2->val_p->prefix < ip_entry_p->prefix) // 新数据的前缀更长，优先级更高
            {
                tmp_mnode2->val_p = ip_entry_p; // 重新登记
            }
            else if (tmp_mnode2->val_p->port != ip_entry_p->port) // 前缀一样，但是端口不同
            {
                printf("Error. Dataset has conflicting data.\n");
            }
        }
    }
    else // index1 > prefix，此时掩码长度过短，需要占用多个位置，例如PPT的第7页的C点
    {
        int point_num = 1 << (CHUNK_SIZE - (ip_entry_p->prefix % CHUNK_SIZE));
        for (int index3 = 0; index3 < point_num; index3++)
        {
            if (tmp_mnode1->child[index2 + index3] == NULL)//说明这个节点还没建立
            {
                tmp_mnode2 = create_mnode();
                tmp_mnode2->parent = tmp_mnode1;
                tmp_mnode1->child[index2 + index3] = tmp_mnode2;
                tmp_mnode2->val_p = ip_entry_p; // 登记
            }
            else // 记录已经存在
            {
                tmp_mnode2 = tmp_mnode1->child[index2 + index3];
                if (tmp_mnode2->val_p->prefix < ip_entry_p->prefix) // 新数据的前缀更长，优先级更高
                {
                    tmp_mnode2->val_p = ip_entry_p; // 重新登记
                }
                else if (tmp_mnode2->val_p->port != ip_entry_p->port) // 前缀一样，但是端口不同
                {
                    printf("Error. Dataset has conflicting data.\n");
                }
            }
        }
    }

}

void init_multiple_trie(ip_entry_t* ip_db)
{

    multitrie_space_counter = 0;
    m_root = create_mnode();

    int index1;
    for (index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        insert_multiple_trie(&ip_db[index1]);
    }

    return;
}

ip_entry_t* lookup_mtrie(u32 ip)
{
    int index1, index2, index3;
    // int not_finish = 0;
    // int find;
    multitrie_node_t* tmp_mnode1 = m_root;
    multitrie_node_t* tmp_mnode2 = NULL;
    ip_entry_t* ret = NULL;

    index1 = CHUNK_SIZE;
    while (tmp_mnode1 != NULL)
    {
        index2 = ((ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // 获取ip的第index1位
        if (tmp_mnode1->child[index2] == NULL)
        {
            break;
        }
        tmp_mnode1 = tmp_mnode1->child[index2];
        index1 += CHUNK_SIZE;
    }

    // 这时候tmp_mnode1不是NULL
    // find = 0;
    ret = NULL;
    tmp_mnode2 = NULL;
    while (tmp_mnode1 != m_root)
    {
        // int point_num = 1 << (CHUNK_SIZE - (ip_entry_p->prefix % CHUNK_SIZE));
        for (index3 = 0; index3 < MULTI_CHILD_NUM; index3++) // 使用最长前缀匹配
        {
            tmp_mnode2 = tmp_mnode1->child[index3];
            if ((tmp_mnode2 != NULL) && (tmp_mnode2->val_p != NULL) && (ip >> (32 - tmp_mnode2->val_p->prefix)) == (tmp_mnode2->val_p->ip >> (32 - tmp_mnode2->val_p->prefix)))
            {
                if (ret == NULL)
                {
                    ret = tmp_mnode2->val_p;
                }
                else if (ret->prefix < tmp_mnode2->val_p->prefix)
                {
                    ret = tmp_mnode2->val_p;
                }
            }
        }

        if (ret != NULL) // 这个点找到合适记录了
        {
            return ret;
        }
        else // 还没找到，向上层找找
        {
            tmp_mnode1 = tmp_mnode1->parent; // 反向遍历
        }

    }

    // 这个时候就已经找不到合适的节点了，返回NULL说明没查到
    return NULL;
}

void verify_mtrie(ip_entry_t* ip_db)
{
    int index1;
    ip_entry_t* ret_ip = NULL;
    int counter = 0;
    for (index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        ret_ip = lookup_mtrie(ip_db[index1].ip);
        if (ret_ip == NULL)
        {
            printf("Error. Test case not found in mtrie.\n");
            return;
        }
        else if (ret_ip != &(ip_db[index1])) // 不完全一致
        {
            //printf("input :ip=%x, prefix=%u, port=%u\n", ip_db[index1].ip, ip_db[index1].prefix, ip_db[index1].port);
            //printf("output:ip=%x, prefix=%u, port=%u\n", ret_ip->ip, ret_ip->prefix, ret_ip->port);
            counter++;
            if (ret_ip->prefix < ip_db[index1].prefix) //debug
            {
                printf("Error.\n");
                printf("input :ip=%x, prefix=%u, port=%u\n", ip_db[index1].ip, ip_db[index1].prefix, ip_db[index1].port);
                printf("output:ip=%x, prefix=%u, port=%u\n", ret_ip->ip, ret_ip->prefix, ret_ip->port);
            }
        }

        if (index1 == 106)
        {
            printf("input :ip=%x, prefix=%u, port=%u\n", ip_db[index1].ip, ip_db[index1].prefix, ip_db[index1].port);
            printf("output:ip=%x, prefix=%u, port=%u\n", ret_ip->ip, ret_ip->prefix, ret_ip->port);
        }
    }

    printf("mtrie total:%d cases are changed.\n", counter);

    return;
}

void mtrie_statistic(multitrie_node_t* node_p)
{
    int index1;
    u8 is_internal_node;

    is_internal_node = 0; // 表示当前节点为内部节点
    if (node_p == NULL)
        return;
    for (index1 = 0; index1 < MULTI_CHILD_NUM; index1++)
    {
        if (node_p->child[index1] != NULL)
        {
            is_internal_node = 1;
            mtrie_statistic(node_p->child[index1]);
        }
    }

    if (is_internal_node == 1)
    {
        m_node_num++;
    }
    else
    {
        m_leaf_num++;
    }


}

int internal_node(multitrie_node_t* m_node)
{
    int index1 = 0;
    int is_internal_node = 0;
    for (index1 = 0; index1 < MULTI_CHILD_NUM; index1++)
    {
        if (m_node->child[index1] != NULL)
        {
            is_internal_node = 1;
            break;
        }
    }
    return is_internal_node;
}

void leaf_pushing(multitrie_node_t* m_node)
{
    int index1, index2;
    multitrie_node_t* tmp;

    for (index1 = 0; index1 < MULTI_CHILD_NUM; index1++)
    {
        if ((m_node->child[index1] == NULL)) // 子节点不存在
        {
            if (m_node->val_p != NULL)
            {
                tmp = create_mnode();
                tmp->parent = m_node;
                m_node->child[index1] = tmp;
                tmp->val_p = m_node->val_p; // 登记
            }
        }
        else if (internal_node(m_node->child[index1])) // 子节点是内部节点
        {
            tmp = m_node->child[index1];
            if ((tmp->val_p == NULL) && (m_node->val_p != NULL))
            {
                tmp->val_p = m_node->val_p;
            }
            leaf_pushing(tmp);
        }
        // else // 子节点是叶子节点
    }

    return;
}