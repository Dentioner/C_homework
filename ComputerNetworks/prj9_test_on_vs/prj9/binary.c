#include <stdlib.h>
#include <stdio.h>
#include "head.h"


btree_node_t* b_root;

btree_node_t* create_bnode()
{
    btree_node_t* p = (btree_node_t*)malloc(sizeof(btree_node_t));
    p->parent = NULL;
    p->child[0] = NULL;
    p->child[1] = NULL;
    p->val_p = NULL;
    binary_space_counter++;
    return p;
}

void insert_binary_tree(ip_entry_t* ip_entry_p)
{
    int index1, index2;
    btree_node_t* tmp_bnode1 = b_root;
    btree_node_t* tmp_bnode2 = NULL;
    for (index1 = 1; index1 < ip_entry_p->prefix; index1++)
    {
        index2 = ((ip_entry_p->ip >> (32 - index1)) & 0x1); // ��ȡip�ĵ�index1λ��0����1
        if (tmp_bnode1->child[index2] == NULL) //˵������ڵ㻹û����
        {
            tmp_bnode2 = create_bnode();
            tmp_bnode2->parent = tmp_bnode1;
            tmp_bnode1->child[index2] = tmp_bnode2;
        }
        tmp_bnode1 = tmp_bnode1->child[index2];

    }

    // �����һ���ˣ����ǰ����ݷ�����һ��
    index2 = ((ip_entry_p->ip >> (32 - index1)) & 0x1); // ��ȡip�ĵ�index1λ��0����1
    if (tmp_bnode1->child[index2] == NULL) //˵������ڵ㻹û����
    {
        tmp_bnode2 = create_bnode();
        tmp_bnode2->parent = tmp_bnode1;
        tmp_bnode1->child[index2] = tmp_bnode2;
        tmp_bnode2->val_p = ip_entry_p; // �Ǽ�
    }
    else // ��¼�Ѿ�����
    {
        tmp_bnode2 = tmp_bnode1->child[index2];
        if (tmp_bnode2->val_p->port != ip_entry_p->port)
        {
            printf("Error. Dataset has conflicting data.\n");
        }
    }

}

void init_binary_tree(ip_entry_t* ip_db)
{

    binary_space_counter = 0;
    b_root = create_bnode();


    int index1;
    for (index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        insert_binary_tree(&ip_db[index1]);
    }

    return;
}

ip_entry_t* lookup_btree(u32 ip)
{
    int index1, index2;
    // int not_finish = 0;

    btree_node_t* tmp_bnode1 = b_root;
    btree_node_t* tmp_bnode2 = NULL;
    for (index1 = 1; (tmp_bnode1 != NULL) && (index1 <= 32); index1++)
    {
        index2 = ((ip >> (32 - index1)) & 0x1); // ��ȡip�ĵ�index1λ��0����1
        if (tmp_bnode1->child[index2] == NULL) //˵����һ���㻹û��������Ҫ��ǰ��ֹ
        {
            break;
            // not_finish = 1;
        }
        else
        {
            tmp_bnode1 = tmp_bnode1->child[index2];
        }
    }

    while (tmp_bnode1 != b_root)
    {
        if (tmp_bnode1->val_p != NULL) // ������м�¼
        {
            return tmp_bnode1->val_p;
        }
        else
        {
            tmp_bnode1 = tmp_bnode1->parent; // �������
        }

    }

    // ���ʱ����Ѿ��Ҳ������ʵĽڵ��ˣ�����NULL˵��û�鵽
    return NULL;
}

void verify_btree(ip_entry_t* ip_db)
{
    int index1;
    ip_entry_t* ret_ip = NULL;
    int counter = 0;
    for (index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        ret_ip = lookup_btree(ip_db[index1].ip);
        if (ret_ip == NULL)
        {
            printf("Error. Test case not found in btree.\n");
            return;
        }
        else if (ret_ip != &(ip_db[index1])) // ����ȫһ��
        {
            //printf("input :ip=%x, prefix=%u, port=%u\n", ip_db[index1].ip, ip_db[index1].prefix, ip_db[index1].port);
            //printf("output:ip=%x, prefix=%u, port=%u\n", ret_ip->ip, ret_ip->prefix, ret_ip->port);
            counter++;
        }
    }

    printf("btree total:%d cases are changed.\n", counter);

    return;
}
