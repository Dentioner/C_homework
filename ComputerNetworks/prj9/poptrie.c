#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "head.h"
#define popcnt(v) __builtin_popcount(v)
#define extract(key, off, len) ((key >> (32 - off)) & ((1 << len) - 1))

/*
u32 popcnt(u32 x)
{
    u32 ans = 0;
    while (x)
    {
        ans++;
        x -= x & -x;
    }
    return ans;
}
*/

poptrie_node_t *create_pnode()
{
    // poptrie_node_t *p = (poptrie_node_t *)malloc(sizeof(poptrie_node_t));
    
    poptrie_node_t *p = &N[p_node_num];
    p->vector = 0;
    p->leafvec = 0;
    p->base0 = 0;
    p->base1 = 0;
    p_node_num++;
    return p;
}

void init_poptrie(ip_entry_t * ip_db)
{
    //poptrie_space_counter = 0;
    p_node_num = 0;
    p_leaf_num = 0;
    N = (poptrie_node_t *)malloc(sizeof(poptrie_node_t) * m_node_num);
    L = (u64 *) malloc(sizeof(u64) * m_leaf_num);

    memset((void *)N, 0, sizeof(N));
    memset((void *)L, 0, sizeof(L));

    //p_root = create_pnode();
    p_root = &N[p_node_num];
    p_node_num++;

    create_poptrie(m_root, p_root);

    return;
}

void create_poptrie(multitrie_node_t *m_entry, poptrie_node_t *p_entry)
{
    int index1, index2, index3;
    multitrie_node_t * m_tmp = NULL;
    u8 is_internal_node;// ��ʾ��ǰ�ڵ�Ϊ�ڲ��ڵ�

    u32 bc, base;
    
    p_entry->base0 = p_leaf_num;
    p_entry->base1 = p_node_num;

    // �ο�����3.1�ڵ�һ��
    for(index1 = 0; index1 < MULTI_CHILD_NUM; index1++) // ������mtrie�ڵ�����ж��ӣ��ԵǼǶ�Ӧptrie�ڵ������vector
    {
        m_tmp = m_entry->child[index1];
        if(m_tmp != NULL) // �������ΪNULL�Ļ�����û��������ӣ�������ӼȲ���Ҷ��Ҳ�����ڲ��ڵ�
        {
            is_internal_node = 0; // �ο��ղ�ͳ��mtrie�нڵ����͵��߼�
            for(index2 = 0; index2 < MULTI_CHILD_NUM; index2++)
            {
                if(m_tmp->child[index2] != NULL)
                {
                    is_internal_node = 1;
                }
            }

            if(is_internal_node) // m_entryΪ��Ҷ�ӽڵ㣬��m_entry�ĵ�index1������Ҳ�Ƿ�Ҷ�ӽڵ�
            {
                p_entry->vector |= (1 << index1);
                // N[p_node_num] = (u32)p_entry;
                p_node_num++;
            }
            else // m_entryΪ��Ҷ�ӽڵ㣬��m_entry�ĵ�index1��������Ҷ�ӽڵ�
            {
                p_entry->leafvec |= (1 << index1);
                L[p_leaf_num] = (u64)(m_tmp->val_p);
                p_leaf_num++;
            }
            
        }// ������if����δִ�й�����ô˵��m_entryΪҶ�ӽڵ㡣������Ϣ�����ĸ��׽ڵ��ʱ���Ѿ��Ǽ��ˣ�����Ͳ��Ǽ�����

    }

    for(index1 = 0; index1 < MULTI_CHILD_NUM; index1++) // �ݹ�ִ��
    {
        m_tmp = m_entry->child[index1];
        if(m_tmp != NULL) // ģ��poptrie��lookup�����ҵ���һ���ӽڵ�
        {
            is_internal_node = 0;
            for(index2 = 0; index2 < MULTI_CHILD_NUM; index2++)
            {
                if(m_tmp->child[index2] != NULL)
                {
                    is_internal_node = 1;
                }
            }
            if(is_internal_node)
            {
                base = p_entry->base1;
                bc = popcnt(p_entry->vector & ((2ULL << index1) -1));
                index3 = base + bc - 1;
                create_poptrie(m_tmp, &N[index3]);
            }

        }
    }
}

ip_entry_t *lookup_ptrie(u32 ip) // �����㷨1+2
{
    
    u32 bc, base;
    u16 vector;
    //u16 leafvec;
    u16 v;
    u32 index1;
    u32 offset;

    index1 = 0;
    offset = CHUNK_SIZE;
    vector = N[index1].vector;
    //leafvec = N[index1].leafvec;

    v = extract(ip, offset, CHUNK_SIZE);
    
    while(vector & (1ULL << v))
    {
        base = N[index1].base1;
        bc = popcnt(vector & ((2ULL << v) -1));
        index1 = base + bc - 1;
        vector = N[index1].vector;
        offset += CHUNK_SIZE;
        v = extract(ip, offset, CHUNK_SIZE);
    }

    base = N[index1].base0;
    bc = popcnt((N[index1].leafvec) & ((2ULL << v) -1));

    return (ip_entry_t *)(L[base + bc - 1]);
}

void verify_ptrie(ip_entry_t *ip_db)
{
    int index1;
    ip_entry_t * ret_ip = NULL;
    int counter = 0;
    for(index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        ret_ip = lookup_ptrie(ip_db[index1].ip);
        if(ret_ip == NULL)
        {
            printf("Error. Test case not found in ptrie.\n");
            return;
        }
        else if (ret_ip != &(ip_db[index1])) // ����ȫһ��
        {
            counter++;
            if (ret_ip->prefix < ip_db[index1].prefix) //debug
            {
                printf("Error.\n");
                printf("input :ip=%x, prefix=%u, port=%u\n", ip_db[index1].ip, ip_db[index1].prefix, ip_db[index1].port);
                printf("output:ip=%x, prefix=%u, port=%u\n", ret_ip->ip, ret_ip->prefix, ret_ip->port);
            }
        }
    }

    printf("ptrie total:%d cases are changed.\n", counter);

    return;
}
