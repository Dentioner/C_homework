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
        index2 = ((ip_entry_p->ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // ��ȡip�ĵ�index1λ
        if (tmp_mnode1->child[index2] == NULL) //˵������ڵ㻹û����
        {
            tmp_mnode2 = create_mnode();
            tmp_mnode2->parent = tmp_mnode1;
            tmp_mnode1->child[index2] = tmp_mnode2;
        }
        tmp_mnode1 = tmp_mnode1->child[index2];
    }

    // �����һ���ˣ����ǰ����ݷ�����һ��
    index2 = ((ip_entry_p->ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // ��ȡip�ĵ�index1λ
    if (index1 == ip_entry_p->prefix) // ���볤�ȸպú���
    {
        if (tmp_mnode1->child[index2] == NULL) //˵������ڵ㻹û����
        {
            tmp_mnode2 = create_mnode();
            tmp_mnode2->parent = tmp_mnode1;
            tmp_mnode1->child[index2] = tmp_mnode2;
            tmp_mnode2->val_p = ip_entry_p; // �Ǽ�
        }
        else // ��¼�Ѿ�����
        {
            tmp_mnode2 = tmp_mnode1->child[index2];
            if (tmp_mnode2->val_p->prefix < ip_entry_p->prefix) // �����ݵ�ǰ׺���������ȼ�����
            {
                tmp_mnode2->val_p = ip_entry_p; // ���µǼ�
            }
            else if (tmp_mnode2->val_p->port != ip_entry_p->port) // ǰ׺һ�������Ƕ˿ڲ�ͬ
            {
                printf("Error. Dataset has conflicting data.\n");
            }
        }
    }
    else // index1 > prefix����ʱ���볤�ȹ��̣���Ҫռ�ö��λ�ã�����PPT�ĵ�7ҳ��C��
    {
        int point_num = 1 << (CHUNK_SIZE - (ip_entry_p->prefix % CHUNK_SIZE));
        for (int index3 = 0; index3 < point_num; index3++)
        {
            if (tmp_mnode1->child[index2 + index3] == NULL)//˵������ڵ㻹û����
            {
                tmp_mnode2 = create_mnode();
                tmp_mnode2->parent = tmp_mnode1;
                tmp_mnode1->child[index2 + index3] = tmp_mnode2;
                tmp_mnode2->val_p = ip_entry_p; // �Ǽ�
            }
            else // ��¼�Ѿ�����
            {
                tmp_mnode2 = tmp_mnode1->child[index2 + index3];
                if (tmp_mnode2->val_p->prefix < ip_entry_p->prefix) // �����ݵ�ǰ׺���������ȼ�����
                {
                    tmp_mnode2->val_p = ip_entry_p; // ���µǼ�
                }
                else if (tmp_mnode2->val_p->port != ip_entry_p->port) // ǰ׺һ�������Ƕ˿ڲ�ͬ
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
        index2 = ((ip >> (32 - index1))& (MULTI_CHILD_NUM - 1)); // ��ȡip�ĵ�index1λ
        if (tmp_mnode1->child[index2] == NULL)
        {
            break;
        }
        tmp_mnode1 = tmp_mnode1->child[index2];
        index1 += CHUNK_SIZE;
    }

    // ��ʱ��tmp_mnode1����NULL
    // find = 0;
    ret = NULL;
    tmp_mnode2 = NULL;
    while (tmp_mnode1 != m_root)
    {
        // int point_num = 1 << (CHUNK_SIZE - (ip_entry_p->prefix % CHUNK_SIZE));
        for (index3 = 0; index3 < MULTI_CHILD_NUM; index3++) // ʹ���ǰ׺ƥ��
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

        if (ret != NULL) // ������ҵ����ʼ�¼��
        {
            return ret;
        }
        else // ��û�ҵ������ϲ�����
        {
            tmp_mnode1 = tmp_mnode1->parent; // �������
        }

    }

    // ���ʱ����Ѿ��Ҳ������ʵĽڵ��ˣ�����NULL˵��û�鵽
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
        else if (ret_ip != &(ip_db[index1])) // ����ȫһ��
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

    is_internal_node = 0; // ��ʾ��ǰ�ڵ�Ϊ�ڲ��ڵ�
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
        if ((m_node->child[index1] == NULL)) // �ӽڵ㲻����
        {
            if (m_node->val_p != NULL)
            {
                tmp = create_mnode();
                tmp->parent = m_node;
                m_node->child[index1] = tmp;
                tmp->val_p = m_node->val_p; // �Ǽ�
            }
        }
        else if (internal_node(m_node->child[index1])) // �ӽڵ����ڲ��ڵ�
        {
            tmp = m_node->child[index1];
            if ((tmp->val_p == NULL) && (m_node->val_p != NULL))
            {
                tmp->val_p = m_node->val_p;
            }
            leaf_pushing(tmp);
        }
        // else // �ӽڵ���Ҷ�ӽڵ�
    }

    return;
}