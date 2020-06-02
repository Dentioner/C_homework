#include <stdlib.h>
#include <stdio.h>
//#include <sys/time.h>
#include "head.h"




ip_entry_t* read_data()
{
    ip_entry_t* ip_db = (ip_entry_t*)malloc(IP_ENTRY_NUM * sizeof(ip_entry_t));
    u32 ip_part0, ip_part1, ip_part2, ip_part3;
    u32 tmp_prefix;
    u32 tmp_port;
    int index1;

    FILE* txt = fopen("forwarding-table.txt", "r");

    for (index1 = 0; index1 < IP_ENTRY_NUM; index1++)
    {
        fscanf(txt, "%u.%u.%u.%u %u %u", &ip_part3, &ip_part2, &ip_part1, &ip_part0, &tmp_prefix, &tmp_port);
        ip_db[index1].ip = (ip_part3 << 24) | (ip_part2 << 16) | (ip_part1 << 8) | (ip_part0);
        ip_db[index1].port = (u8)tmp_port;
        ip_db[index1].prefix = (u8)tmp_prefix;
    }

    fclose(txt);

    return ip_db;
}


int main()
{
    //struct timeval tv1, tv2;
    ip_entry_t* ip_db = NULL;
    ip_db = read_data();


    // binary tree
    init_binary_tree(ip_db);
    //gettimeofday(&tv1, NULL);
    verify_btree(ip_db);
    //gettimeofday(&tv2, NULL);

    //printf("binary tree mem cost: %lu Bytes\n", sizeof(btree_node_t) * binary_space_counter);
    //printf("binary tree time cost: %.2lf ns.\n", (1000.0 * (tv2.tv_sec * 1000000 + tv2.tv_usec - tv1.tv_sec * 1000000 - tv1.tv_usec)) / IP_ENTRY_NUM);

    // multiple trie
    init_multiple_trie(ip_db);
    //gettimeofday(&tv1, NULL);
    verify_mtrie(ip_db);
    //gettimeofday(&tv2, NULL);

    //printf("multiple trie mem cost: %lu Bytes\n", sizeof(multitrie_node_t) * multitrie_space_counter);
    //printf("multiple trie time cost: %.2lf ns.\n", (1000.0 * (tv2.tv_sec * 1000000 + tv2.tv_usec - tv1.tv_sec * 1000000 - tv1.tv_usec)) / IP_ENTRY_NUM);

    m_node_num = 0;
    m_leaf_num = 0;
    mtrie_statistic(m_root);
    printf("debug: node num = %d, leaf num = %d, total = %ld\n", m_node_num, m_leaf_num, multitrie_space_counter);

    leaf_pushing(m_root);
    //test
    //gettimeofday(&tv1, NULL);
    verify_mtrie(ip_db);
    //gettimeofday(&tv2, NULL);

    //printf("multiple trie mem cost: %lu Bytes\n", sizeof(multitrie_node_t) * multitrie_space_counter);
    //printf("multiple trie time cost: %.2lf ns.\n", (1000.0 * (tv2.tv_sec * 1000000 + tv2.tv_usec - tv1.tv_sec * 1000000 - tv1.tv_usec)) / IP_ENTRY_NUM);

    m_node_num = 0;
    m_leaf_num = 0;
    mtrie_statistic(m_root);
    printf("debug: node num = %d, leaf num = %d, total = %ld\n", m_node_num, m_leaf_num, multitrie_space_counter);
    //test


    // poptrie
    init_poptrie(ip_db);
    //gettimeofday(&tv1, NULL);
    verify_ptrie(ip_db);
    //gettimeofday(&tv2, NULL);

    //printf("poptrie mem cost: %lu Bytes\n", (sizeof(poptrie_node_t) * p_node_num + sizeof(u32) * p_leaf_num));
    //printf("poptrie time cost: %.2lf ns.\n", (1000.0 * (tv2.tv_sec * 1000000 + tv2.tv_usec - tv1.tv_sec * 1000000 - tv1.tv_usec)) / IP_ENTRY_NUM);

    //debug
    /*
    if (p_node_num != m_node_num || p_leaf_num != m_leaf_num)
    {
        printf("pnode:%d, p_leaf:%d, m_node:%d, m_leaf:%d\n", p_node_num, p_leaf_num, m_node_num, m_leaf_num);
    }

    //debug
    
    for (int index1 = 0; index1 < p_node_num; index1++)
    {
        printf("N[%d]:leafvec=%x, vec=%x, base0=%u, base1=%u\n", index1, N[index1].leafvec, N[index1].vector, N[index1].base0, N[index1].base1);
    }

    printf("mnode=%d, mleaf=%d, pnode=%d, pleaf=%d\n", m_node_num, m_leaf_num, p_node_num, p_leaf_num);
    */
    
    
    free(ip_db);
    return 0;
}