#include "../../test/test_fs/test_fs.h"
#include "stdio.h"
#include "../../drivers/screen.h"
#include "type.h"
#include "fs.h"
#include "../../include/os/string.h"
#include "../../include/os/time.h"

extern uint32_t current_line;

char read_block_buffer[BLOCK_SIZE];

inode_t tmp_inode_array[INODEBLOCK_NUM][INODE_NUM_PER_BLOCK];
dentry_t tmp_dentry_arr[DENTRY_NUM_PER_BLOCK];
char tmp_path[MAX_DIRECTORY_DEPTH][FILENAME_LENGTH];
uint32_t tmp_path_depth;        // depth of tmp path
char tmp_abs_path[MAX_DIRECTORY_DEPTH][FILENAME_LENGTH];
uint32_t tmp_abs_path_depth;

void init_tmp_path()
{
    uint32_t index1, index2;
    for(index1 = 0; index1 < MAX_DIRECTORY_DEPTH; index1++)
        for (index2 = 0; index2 < FILENAME_LENGTH; index2++)
            tmp_path[index1][index2] = '\0';
}

void load_dentry_arr(uint32_t block_num)
{
    // check if the new filename exists
    sdread(read_block_buffer, (START_ADDRESS_SD + (block_num)*BLOCK_SIZE), BLOCK_SIZE);
    os_memcpy((void *)tmp_dentry_arr, (void *)read_block_buffer, (DENTRY_NUM_PER_BLOCK*sizeof(dentry_t)));
}


inode_t * get_inode(uint32_t ino)
{
    inode_t *ret_inode;
    uint32_t inode_in_which_block, inode_in_block_index;
    inode_in_which_block = ino / INODE_NUM_PER_BLOCK;
    inode_in_block_index = ino % INODE_NUM_PER_BLOCK;

    //sdread(read_block_buffer, (tmp_spblk.inode_offset + inode_in_which_block)*BLOCK_SIZE, BLOCK_SIZE);
    //os_memcpy((void *)tmp_inode_array, (void *)read_block_buffer, (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
    ret_inode = &tmp_inode_array[inode_in_which_block][inode_in_block_index];
    return ret_inode;
}


void load_inode_array(uint32_t inode_offset)
{
    uint32_t index1;
    for (index1 = 0; index1 < INODEBLOCK_NUM; index1++)
    {
        sdread(read_block_buffer, (START_ADDRESS_SD +(inode_offset + index1)*BLOCK_SIZE), BLOCK_SIZE);
        os_memcpy((void *)(&(tmp_inode_array[index1])), (void *)read_block_buffer, (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
    }
}


void init_inode(inode_t * p)
{
    int index3 = 0;
    p->type = 0;
    p->last_update_time = 0;
    p->volumn = 0;
    p->parent_ino = 0;
    p->indirect_blocks = 0;
    p->double_indirect_blocks = 0;
    for (index3 = 0; index3 < DIRECT_LINK_NUM; index3 ++)    
    {
        p->direct_blocks[index3] = 0;
    }
}

void write_bitmap(uint32_t* bitmap_array, uint32_t arr_len, uint32_t num)
{
    uint32_t div, mod, tmp;
    div = num / 32;
    mod = num % 32;
    int index1 = 0;
    int index2;
    if(div > 0)
    {
        for(;index1 < div; index1++)
        {
            bitmap_array[index1] = 0xffffffff;
        }
        if(index1 >= arr_len)
        {
            printk("error. array overflow.\n");
            while(1);
        }
    }

    tmp = 0x0;
    for (index2 = 0; index2 < mod; index2++)
    {
        tmp = tmp << 1;
        tmp += 0x1;
    }    
    bitmap_array[index1] = tmp;
}


void refresh_bitmap(uint32_t * bitmap_array, uint32_t bit_no, int type)
{
    uint32_t index1, offset, t1, t2;
    index1 = bit_no / 32;
    offset = bit_no % 32;
    t1 = 1<<offset;
    if (type == BITMAP_ADD)
    {
            bitmap_array[index1] |= t1;    
    }
    else
    {
        t2 = ~t1;
        bitmap_array[index1] &= t2;
    }
}

uint32_t get_new_no(uint32_t base, uint32_t bit)
{
    uint32_t ino;
    uint32_t index1;
    uint32_t t1;

    /*for (index1 = 31; index1 > 0; index1--)
    {
        t1 = (bit & (1 << index1)) >> index1;
        if(!t1)
        {
            break;
        }
    }

    if(index1 == 0)
    {
        printk("error in get new ino.\n");
        while(1);
    }

    ino = base * 32 + (31 - index1);*/


    for (index1 = 0; index1 < 32; index1++)
    {
        t1 = bit >> index1;
        if((t1 %2) == 0)
        {
            break;
        }
    }
    ino = base * 32 + index1;
    return ino;
}

void do_mkfs()
{
    spblk_t new_spblk;
    current_line+=2;
    vt100_move_cursor(1, current_line);
    uint16_t exist = 0;
    int index1, index2;
    //uint32_t bitmap_block_num = ((BITMAP_ARRAY_LENGTH * 4)/4096) + 1;
    uint32_t bitmap_block_num = BLOCK_NUM_OF_BLOCK_BITMAP;

    
    uint32_t inode_bitmap_block_num = ((INODE_BITMAP_ARRAY_LENGTH * 4)/4096) + 1;
    //uint32_t inode_block_num = ((INODE_NUM * sizeof(inode_t))/4096) + 1;
    uint32_t inode_block_num = INODEBLOCK_NUM;

    
    void * tmp_pointer1, * tmp_pointer2;
    inode_t root_inode;
    
    

    printk("[FS] Checking file system...\n");current_line++;
    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&new_spblk, (void *)read_block_buffer, sizeof(spblk_t));
    if(new_spblk.magic_num == MAGIC_NUM)
    {
        printk("[FS] File system is already setted up.\n");current_line++;
        exist = 1;
    }
    else 
    {
        exist = 0;
        printk("[FS] Start initialize file system!\n");current_line++;
        printk("[FS] Setting superblock...\n");current_line++;

        
        new_spblk.magic_num = MAGIC_NUM;
        new_spblk.fs_size = BLOCK_NUM;
        new_spblk.map_offset = 1;

        // uint32_t bitmap_block_num = ((BITMAP_ARRAY_LENGTH * 4)/4096) + 1;
        new_spblk.inode_map_offset =  new_spblk.map_offset + bitmap_block_num;

        // uint32_t inode_bitmap_block_num = ((INODE_BITMAP_ARRAY_LENGTH * 4)/4096) + 1;
        new_spblk.inode_offset = new_spblk.inode_map_offset + inode_bitmap_block_num;

        // uint32_t inode_block_num = ((INODE_NUM * sizeof(inode_t))/4096) + 1;
        new_spblk.data_offset = new_spblk.inode_offset + inode_block_num;

        new_spblk.used_blk_num = new_spblk.data_offset + 1;
        //new_spblk.used_blk_num += 1; 
        // ��Ŀ¼��Ҫһ����洢Ŀ¼����������used_blk_num�Ѿ��ӽ�ȥ��
    // for example,
    // including root path, OS has used 1 block for superblock, 4 blocks for block bitmap, 
    // 1 block for inode bitmap, 4 blocks for inode, 1 block for root path
    // total : 11 blocks used.
        new_spblk.used_inode_num = 1; // root path

        os_memcpy((void *)read_block_buffer, (void *)&new_spblk, sizeof(spblk_t));    
        sdwrite(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    }

    printk("     magic: %x\n", new_spblk.magic_num);current_line++;
    printk("     size in block: %d blocks, start sector: %d\n", new_spblk.fs_size, (START_ADDRESS_SD/512));current_line++;
    printk("     block map offset: %d\n", new_spblk.map_offset);current_line++;
    printk("     inode map offset: %d\n", new_spblk.inode_map_offset);current_line++;
    printk("     inode offset: %d\n", new_spblk.inode_offset);current_line++;
    printk("     data offset: %d\n", new_spblk.data_offset);current_line++;
    printk("     inode entry size: %d, dir entry size: %d\n", sizeof(inode_t), sizeof(dentry_t));current_line++;
    
    if(!exist)
    {
/********************************************************************************/
        printk("[FS] Setting block-map...\n");current_line++;
        for(index1 = 0; index1 < BITMAP_ARRAY_LENGTH; index1++) // init bitmap array
        {
            block_bitmap[index1] = 0;
        }

        write_bitmap(block_bitmap, BITMAP_ARRAY_LENGTH, new_spblk.used_blk_num);
        tmp_pointer2 = (void *)block_bitmap;

        for(index2 = 0; index2 < bitmap_block_num; index2++)
        {
            os_memcpy((void *)read_block_buffer, tmp_pointer2, BLOCK_SIZE);
            sdwrite(read_block_buffer, (START_ADDRESS_SD + (new_spblk.map_offset + index2) * BLOCK_SIZE), BLOCK_SIZE);
            //tmp_pointer2 += BLOCK_SIZE*sizeof(uint32_t);
            tmp_pointer2 += BLOCK_SIZE;
        }

/********************************************************************************/
        printk("[FS] Setting inode-map...\n");current_line++;
        for(index1 = 0 ; index1 < INODE_BITMAP_ARRAY_LENGTH; index1++) // init bitmap array
        {
            inode_bitmap[index1] = 0;
        }

        //inode_bitmap[0] = 0x1; // root directory keeps the first bit of bit map
        write_bitmap(inode_bitmap, INODE_BITMAP_ARRAY_LENGTH, new_spblk.used_inode_num);

        
        if (inode_bitmap_block_num <= 1)
        {
            os_memcpy((void *)read_block_buffer, (void *)inode_bitmap, (sizeof(uint32_t) * INODE_BITMAP_ARRAY_LENGTH));    
            sdwrite(read_block_buffer, (START_ADDRESS_SD + new_spblk.inode_map_offset * BLOCK_SIZE), BLOCK_SIZE);        
        }       
        else
        {
            
            tmp_pointer1 = (void *)inode_bitmap;

            for (index2 = 0; index2 < inode_bitmap_block_num; index2++)
            {
                os_memcpy((void *)read_block_buffer, tmp_pointer1, BLOCK_SIZE);    
                sdwrite(read_block_buffer, (START_ADDRESS_SD + (new_spblk.inode_map_offset + index2)*BLOCK_SIZE ), BLOCK_SIZE);      
                //tmp_pointer1 += BLOCK_SIZE*sizeof(uint32_t);
                tmp_pointer1 += BLOCK_SIZE;
                
            }
        }



/********************************************************************************/        
        printk("[FS] Setting inode...\n");current_line++;
        //root_inode.mode = 777;
        root_inode.type = DIRECTORY_TYPE;
        //root_inode.entry_num = 0;
        //root_inode.file_sz = 0; // it is not a file
        root_inode.volumn = 2; // '.' and '..'
        root_inode.parent_ino = 0; // root do not have a parent
        
        root_inode.last_update_time = get_timer();
        
        for(index1 = 0; index1 < DIRECT_LINK_NUM; index1++)
        {
            root_inode.direct_blocks[index1] = 0;
        }

        root_inode.direct_blocks[0] = new_spblk.used_blk_num - 1; // ��һ��data������ø���Ŀ¼��Ŀ¼��

        root_inode.indirect_blocks = 0;
        root_inode.double_indirect_blocks = 0;

        os_memcpy((void *)read_block_buffer, (void *)&root_inode, sizeof(inode_t));    
        sdwrite(read_block_buffer, (START_ADDRESS_SD + BLOCK_SIZE* new_spblk.inode_offset), BLOCK_SIZE);
/********************************************************************************/   
        // writing directory array to the block
        for(index2 = 0; index2 < DENTRY_NUM_PER_BLOCK; index2 ++)
        {
            tmp_dentry_arr[index2].ino = 0;
            tmp_dentry_arr[index2].type = 0;
            tmp_dentry_arr[index2].file_name[0] = '\0';
        }
        tmp_dentry_arr[0].ino = 0; // '.' ==> root
        tmp_dentry_arr[1].ino = 0; // '..' ==> root, too
        os_memcpy(tmp_dentry_arr[0].file_name, "~", sizeof("~"));
        os_memcpy(tmp_dentry_arr[1].file_name, "~", sizeof("~"));
        
        //os_memcpy(tmp_dentry_arr[0].file_name, ".", sizeof("."));
        //os_memcpy(tmp_dentry_arr[1].file_name, "..", sizeof(".."));
        
        tmp_dentry_arr[0].type = DIRECTORY_TYPE;
        tmp_dentry_arr[1].type = DIRECTORY_TYPE;

        os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
        sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * root_inode.direct_blocks[0])), BLOCK_SIZE);

        printk("[FS] Setting filesystem finished!\n");current_line++;
        

        // initial some global var
        path_depth = 1;
        current_dir_ino = 0;
        os_memcpy((char *)(&current_path[0]), "~", sizeof("~"));
    }

    
    
    // TODO: init backup superblock

    // printk("asdsad\nsadsadsa\nsdasdsad\nasdsad");current_line++;current_line++;current_line++;
}

void do_statfs()
{
    spblk_t tmp_spblk;



    current_line+=2;
    vt100_move_cursor(1, current_line);

    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&tmp_spblk, (void *)read_block_buffer, sizeof(spblk_t));


    printk("magic: %x\n", tmp_spblk.magic_num);current_line++;
    printk("used blocks: %d/%d, start sector: %d(%x)\n", tmp_spblk.used_blk_num, BLOCK_NUM, (START_ADDRESS_SD/512), START_ADDRESS_SD);current_line++;
    printk("inode map offset: %d, occupied block: %d, used: %d/%d\n", tmp_spblk.inode_map_offset, (tmp_spblk.inode_offset - tmp_spblk.inode_map_offset), tmp_spblk.used_inode_num, INODE_NUM);current_line++;
    printk("block map offset: %d, occupied block: %d\n", tmp_spblk.map_offset, (tmp_spblk.inode_map_offset - tmp_spblk.map_offset));current_line++;
    printk("inode offset: %d, occupied block: %d\n", tmp_spblk.inode_offset, (tmp_spblk.data_offset - tmp_spblk.inode_offset));current_line++;
    printk("data offset: %d, occupied block: %d\n", tmp_spblk.data_offset, (BLOCK_NUM - tmp_spblk.data_offset));current_line++;
    printk("inode entry size: %d, dir entry size: %d\n", sizeof(inode_t), sizeof(dentry_t));current_line++;
    
    if(tmp_spblk.magic_num != MAGIC_NUM)
    {
        printk("file system has not been established yet.\n");current_line++;
    }
}

void do_mkdir(uint32_t arg_filename)
{
    //int index1;
    char *my_filename = (char *)arg_filename;
    inode_t tmp_inode1;
    inode_t *tmp_parent_inode_p;
    inode_t * tmp_inode2_p;
    spblk_t tmp_spblk;
    uint32_t inode_num_all = INODE_NUM;
    uint32_t inode_in_which_block; // inode���ĸ�������
    uint32_t inode_in_block_index; // inode���������ĸ�λ��

    uint32_t tmp_parent_ino;
    uint32_t index1, index2;
    
    uint32_t new_ino, new_blk_num;
    void * tmp_block_bitmap_p;
    char parent_name[FILENAME_LENGTH];
    void * tmp_pointer1;
    uint32_t index3;


    current_line+=2;
    vt100_move_cursor(1, current_line);

    // check fs
    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&tmp_spblk, (void *)read_block_buffer, sizeof(spblk_t));
    if (tmp_spblk.magic_num != MAGIC_NUM)
    {
        printk("Error! File Symtem doesn't exist.\n");//current_line++;
        return;
    }

    // load inode array
    load_inode_array(tmp_spblk.inode_offset);


    // get current path inode
    tmp_parent_inode_p = get_inode(current_dir_ino);
    if (tmp_parent_inode_p->type == FILE_TYPE)
    {
        printk("ERROR, partent type is file.");
        while(1);
    }

    // check if the new filename exists
    load_dentry_arr(tmp_parent_inode_p->direct_blocks[0]);
    
    if(tmp_parent_inode_p->volumn > 2)
    {
        for (index2 = 0; index2 < tmp_parent_inode_p->volumn; index2 ++)
        {
            if((tmp_dentry_arr[index2].type == DIRECTORY_TYPE) && (!(my_strncmp(tmp_dentry_arr[index2].file_name, my_filename, FILENAME_LENGTH))))
            { // ����������1.��Ŀ¼��2.Ŀ¼����ͬ��
                // ���������ͬ�����������ļ�����ʱ���ǿ��Դ���ͬ����Ŀ¼��
                printk("directory has already established.\n");//current_line++;
                return;
            }
        }
    }

    // search inode bitmap for idle inode
    sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.inode_map_offset * BLOCK_SIZE)), BLOCK_SIZE);
    os_memcpy((void *)inode_bitmap, read_block_buffer, sizeof(inode_bitmap));

    for(index2 = 0; index2 < INODE_BITMAP_ARRAY_LENGTH; index2++)
    {
        if(inode_bitmap[index2] != 0xffffffff)
        {
            break;
        }
    }

    if(index2 == INODE_BITMAP_ARRAY_LENGTH)
    {
        printk("error. not enough inode for new directory.\n");
        while(1);
    }

    new_ino = get_new_no(index2, inode_bitmap[index2]);    
    tmp_inode2_p = get_inode(new_ino);
    tmp_parent_ino = tmp_dentry_arr[0].ino; // ���¸�Ŀ¼��ino������ᶪʧ����Ϣ
    os_memcpy(parent_name, tmp_dentry_arr[0].file_name, sizeof(tmp_dentry_arr[0].file_name)); // ��¼�¸�Ŀ¼������


    // initialize new inode
    tmp_inode2_p->type = DIRECTORY_TYPE;
    tmp_inode2_p->last_update_time = get_timer();
    tmp_inode2_p->volumn = 2;
    tmp_inode2_p->parent_ino = tmp_parent_ino;
    tmp_inode2_p->indirect_blocks = 0;
    tmp_inode2_p->double_indirect_blocks = 0;



    for (index2 = 0; index2 < DIRECT_LINK_NUM; index2++)
    {
        tmp_inode2_p->direct_blocks[index2] = 0;
    }

        // read block bitmap
    tmp_block_bitmap_p = (void *)block_bitmap;
    for (index2 = 0; index2 < BLOCK_NUM_OF_BLOCK_BITMAP; index2++)
    {
        sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.map_offset + index2) * BLOCK_SIZE), BLOCK_SIZE);
        os_memcpy((void *)tmp_block_bitmap_p, read_block_buffer, BLOCK_SIZE);
        tmp_block_bitmap_p += BLOCK_SIZE;
    }

    for (index2 = 0; index2 < BITMAP_ARRAY_LENGTH; index2++)
    {
        if(block_bitmap[index2] != 0xffffffff)
        {
            break;
        }
    }
    if(index2 == BITMAP_ARRAY_LENGTH)
    {
        printk("error. not enough block for new directory or file.\n");
        while(1);
    }

    new_blk_num = get_new_no(index2, block_bitmap[index2]);

    tmp_inode2_p->direct_blocks[0] = new_blk_num;



    // update block bitmap/inode bitmap/inode array/superblock/parent dentry array/self dentry array ...
    refresh_bitmap(block_bitmap, new_blk_num, BITMAP_ADD);
    refresh_bitmap(inode_bitmap, new_ino, BITMAP_ADD);
    
    tmp_pointer1 = (void *)block_bitmap;
    for(index2 = 0; index2 < BLOCK_NUM_OF_BLOCK_BITMAP; index2++)
    {
        os_memcpy((void *)read_block_buffer, tmp_pointer1, BLOCK_SIZE);
        sdwrite(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.map_offset + index2) * BLOCK_SIZE), BLOCK_SIZE);
        tmp_pointer1 += BLOCK_SIZE;
    }

    os_memcpy(read_block_buffer, (char *)inode_bitmap, BLOCK_SIZE);
    sdwrite(read_block_buffer, (START_ADDRESS_SD + tmp_spblk.inode_map_offset * BLOCK_SIZE), BLOCK_SIZE);        
    


    //inode array���ظ��£���Ϊ֮ǰ�ĳ�ʼ����inode��������ָ���Ͻ��еģ�ֱ��ָ���inode array��Ӧ�ĳ�Ա
    tmp_parent_inode_p->volumn++;

    for (index2 = 0; index2 < INODEBLOCK_NUM; index2++)
    {
        os_memcpy((void *)read_block_buffer, (void *)(&(tmp_inode_array[index2])), (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
        sdwrite(read_block_buffer, (START_ADDRESS_SD +( tmp_spblk.inode_offset + index2)*BLOCK_SIZE), BLOCK_SIZE);    
    }
    
    
    tmp_spblk.used_blk_num++;
    tmp_spblk.used_inode_num++;
    os_memcpy((void *)read_block_buffer, (void *)&tmp_spblk, sizeof(spblk_t));    
    sdwrite(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);

    // ���¸�Ŀ¼
    load_dentry_arr(tmp_parent_inode_p->direct_blocks[0]);
    
    for(index2 = 0; index2 < DENTRY_NUM_PER_BLOCK; index2++)
    {
        if((index2 == 0 ) || (index2 == 1))
            continue; // skip '.' and '..'
        
        if (tmp_dentry_arr[index2].ino == 0)
        {
            break;
        }
    }

    if(index2 == DENTRY_NUM_PER_BLOCK)
    {
        printk("ERROR.....\n");
        while(1);
    }

    tmp_dentry_arr[index2].ino = new_ino;
    tmp_dentry_arr[index2].type = DIRECTORY_TYPE;
    os_memcpy((char *)&(tmp_dentry_arr[index2].file_name), my_filename, sizeof(my_filename));

    
    os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
    sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * tmp_parent_inode_p->direct_blocks[0])), BLOCK_SIZE);

    // ��ʼ���µ�Ŀ¼
    for(index2 = 0; index2 < DENTRY_NUM_PER_BLOCK; index2++)
    {
        tmp_dentry_arr[index2].ino = 0;
        tmp_dentry_arr[index2].type = 0;
        for (index3 = 0; index3< FILENAME_LENGTH; index3++)
        {
            tmp_dentry_arr[index2].file_name[index3] = '\0';
        }
    }
    tmp_dentry_arr[0].ino = new_ino; // '.' ==> root
    tmp_dentry_arr[1].ino = tmp_parent_ino; // '..' ==> parent
    os_memcpy(tmp_dentry_arr[0].file_name, my_filename, sizeof(my_filename));
    os_memcpy(tmp_dentry_arr[1].file_name, parent_name, sizeof(parent_name));
    tmp_dentry_arr[0].type = DIRECTORY_TYPE;
    tmp_dentry_arr[1].type = DIRECTORY_TYPE;

    os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
    sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * (tmp_inode2_p->direct_blocks[0]))), BLOCK_SIZE);
    

    printk("[FS] mkdir succeed!\n");//current_line++;

}

void do_rmdir(uint32_t arg_filename)
{
    uint32_t index1, index2, index3;
    char *my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);
    spblk_t tmp_spblk;
    uint32_t inode_in_which_block, inode_in_block_index;
    inode_t *tmp_parent_inode_p, *tmp_inode1_p;
    uint32_t delete_ino, delete_blk_num;
    void * tmp_block_bitmap_p;
    void * tmp_pointer1;



    // check fs
    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&tmp_spblk, (void *)read_block_buffer, sizeof(spblk_t));
    if (tmp_spblk.magic_num != MAGIC_NUM)
    {
        printk("Error! File Symtem doesn't exist.\n");//current_line++;
        return;
    }

    // load inode array
    load_inode_array(tmp_spblk.inode_offset);

    // get current path inode
    tmp_parent_inode_p = get_inode(current_dir_ino);    
    if (tmp_parent_inode_p->type == FILE_TYPE)
    {
        printk("ERROR, partent type is file.");
        while(1);
    }

    // check if the filename exists
    load_dentry_arr(tmp_parent_inode_p->direct_blocks[0]);
    
    for (index2 = 0; index2 < tmp_parent_inode_p->volumn; index2 ++)
    {
        if((tmp_dentry_arr[index2].type == DIRECTORY_TYPE) && (!(my_strncmp(tmp_dentry_arr[index2].file_name, my_filename, FILENAME_LENGTH))))
        { // ����������1.��Ŀ¼��2.Ŀ¼����ͬ��
            // ���������ͬ�����������ļ�����ʱ��Ӧɾ�����ͬ�����ļ�
            break;
        }
    }
    
    if (index2 == tmp_parent_inode_p->volumn)
    {
        printk("error. directory doesn't exist.\n");
        return;
    }

        
    // get the information of the deleted inode
    // but do not init it here
    delete_ino = tmp_dentry_arr[index2].ino;
    tmp_inode1_p = get_inode(delete_ino);
    delete_blk_num = tmp_inode1_p->direct_blocks[0]; // �ȼ�ס��Ҫ��Ȼ���¾�û��

    // �ڴ�ֱ�Ӹ��¸��б�����Ͳ������ظ�����
    tmp_dentry_arr[index2].ino = 0;
    tmp_dentry_arr[index2].type = 0;
    for(index3 = 0; index3 < FILENAME_LENGTH; index3++)
    {
        tmp_dentry_arr[index2].file_name[index3] = '\0';
    }
    os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
    sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * tmp_parent_inode_p->direct_blocks[0])), BLOCK_SIZE);




    
    // load bitmap
    tmp_block_bitmap_p = (void *)block_bitmap;
    for (index2 = 0; index2 < BLOCK_NUM_OF_BLOCK_BITMAP; index2++)
    {
        sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.map_offset + index2) * BLOCK_SIZE), BLOCK_SIZE);
        os_memcpy((void *)tmp_block_bitmap_p, read_block_buffer, BLOCK_SIZE);
        tmp_block_bitmap_p += BLOCK_SIZE;
    }

    sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.inode_map_offset * BLOCK_SIZE)), BLOCK_SIZE);
    os_memcpy((void *)inode_bitmap, read_block_buffer, sizeof(inode_bitmap));

    // update block bitmap/inode bitmap/inode array/superblock/parent dentry array/self dentry array ...
    refresh_bitmap(block_bitmap, tmp_inode1_p->direct_blocks[0], BITMAP_DEL);
    refresh_bitmap(inode_bitmap, delete_ino, BITMAP_DEL);
    tmp_pointer1 = (void *)block_bitmap;
    for(index2 = 0; index2 < BLOCK_NUM_OF_BLOCK_BITMAP; index2++)
    {
        os_memcpy((void *)read_block_buffer, tmp_pointer1, BLOCK_SIZE);
        sdwrite(read_block_buffer, (START_ADDRESS_SD + (tmp_spblk.map_offset + index2) * BLOCK_SIZE), BLOCK_SIZE);
        tmp_pointer1 += BLOCK_SIZE;
    }

    os_memcpy(read_block_buffer, (char *)inode_bitmap, BLOCK_SIZE);
    sdwrite(read_block_buffer, (START_ADDRESS_SD + tmp_spblk.inode_map_offset * BLOCK_SIZE), BLOCK_SIZE);        
    
    // ����inode ����
    
    tmp_parent_inode_p->volumn--;
    init_inode(tmp_inode1_p);
    for (index2 = 0; index2 < INODEBLOCK_NUM; index2++)
    {
        os_memcpy((void *)read_block_buffer, (void *)(&(tmp_inode_array[index2])), (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
        sdwrite(read_block_buffer, (START_ADDRESS_SD +( tmp_spblk.inode_offset + index2)*BLOCK_SIZE), BLOCK_SIZE);    
    }

    // ���³�����
    tmp_spblk.used_blk_num--;
    tmp_spblk.used_inode_num--;
    os_memcpy((void *)read_block_buffer, (void *)&tmp_spblk, sizeof(spblk_t));    
    sdwrite(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);

    

    // ɾ������ʼ�����Լ���Ŀ¼

    for(index2 = 0; index2 < DENTRY_NUM_PER_BLOCK; index2++)
    {
        tmp_dentry_arr[index2].ino = 0;
        tmp_dentry_arr[index2].type = 0;
        for (index3 = 0; index3< FILENAME_LENGTH; index3++)
        {
            tmp_dentry_arr[index2].file_name[index3] = '\0';
        }
    }
    os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
    sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * delete_blk_num)), BLOCK_SIZE);


    printk("[FS] rmdir succeed!\n");//current_line++;
}

void do_cd(uint32_t arg_filename)
{
    uint32_t index1, index2, index3;
    char *my_filename = (char *)arg_filename;
    spblk_t tmp_spblk;
    uint32_t inode_in_which_block, inode_in_block_index;
    inode_t *tmp_parent_inode_p, *target_inode_p, *tmp_inode1_p, *tmp_inode2_p;
    uint32_t target_ino, start_ino;
    char * c_pointer1;
    
    current_line+=2;
    vt100_move_cursor(1, current_line);
    
    // check fs
    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&tmp_spblk, (void *)read_block_buffer, sizeof(spblk_t));
    if (tmp_spblk.magic_num != MAGIC_NUM)
    {
        printk("Error! File Symtem doesn't exist.\n");//current_line++;
        return;
    }

    // load inode array
    load_inode_array(tmp_spblk.inode_offset);

    // get current path inode    
    tmp_parent_inode_p = get_inode(current_dir_ino);
    if (tmp_parent_inode_p->type == FILE_TYPE)
    {
        printk("ERROR, partent type is file.");
        while(1);
    }

    // load directory array
    load_dentry_arr(tmp_parent_inode_p->direct_blocks[0]);

    // copy abs tmp path
    for (index1 = 0; index1 < MAX_DIRECTORY_DEPTH; index1++)
    {
        for(index2 = 0; index2 < FILENAME_LENGTH; index2++)
        {
            tmp_abs_path[index1][index2] = current_path[index1][index2];
        }
    }
    tmp_abs_path_depth = path_depth;


    // split the filename
    // but do not recognize '.' & '..' now
    // just fill them in the tmp_path[MAX_DIRECTORY_DEPTH][FILENAME_LENGTH];
    c_pointer1 = my_filename;
    tmp_path_depth = 0;
    index1 = 0;
    init_tmp_path();

    while((*c_pointer1 != '\0') && (*c_pointer1 != '\r') && (*c_pointer1 != '\n'))
    {
        if(*c_pointer1 == '/')
        {
            tmp_path_depth++;
            index1 = 0;
        }
        else
        {
            tmp_path[tmp_path_depth][index1] = *c_pointer1;
            index1++;
        }
        c_pointer1++;        
    }
    tmp_path_depth++;

    
    
    // recognize the path
    tmp_inode2_p = tmp_parent_inode_p;

    for(index1 = 0; index1 < tmp_path_depth; index1++)
    {
        if(!my_strncmp((char *)&(tmp_path[index1]), "..", 2)) // ��⵽�����ϼ����Ѱַ
        { // ע�⣬�����ϼ����Ѱַ��pattern���б������Ѱַ������ȼ���ϼ��ȽϺ���
            tmp_inode2_p = get_inode(tmp_dentry_arr[1].ino); // Ŀ¼�����1�Ź̶�Ϊ�ϲ�
            load_dentry_arr(tmp_inode2_p->direct_blocks[0]); // ���ϲ�Ŀ¼���س���
            // ���¾���·��
            tmp_abs_path_depth--;
            for(index2 = 0; index2 < FILENAME_LENGTH; index2++)
            {
                tmp_abs_path[tmp_abs_path_depth][index2] = '\0';
            }
        }
        else if (!my_strncmp((char *)&(tmp_path[index1]), ".", 1)) // ��⵽���б������Ѱַ
        {
            ; // ɶ������������Ϊ�ʼ�����˱���Ŀ¼������Ҳ����Ҫ�䶯Ŀ¼
        }

        else if ((!my_strncmp((char *)&(tmp_path[index1]), "~", 1)) && (index1 == 0))// ����Ѱַ
        {
            tmp_inode2_p = get_inode(0); // ���־���Ѱַ����ø�Ŀ¼inode
            load_dentry_arr(tmp_inode2_p->direct_blocks[0]); // ���ظ�Ŀ¼
            // ���¾���·��
            tmp_abs_path_depth = 1;
            os_memcpy((char *)(&tmp_abs_path[0]), "~", sizeof("~"));
        }
        else if (index1 == 0) // �����һ��·�������������������ô·�����Ϸ�
        {
            printk("error, invalid path.\n");
            return;
        }
        else // ����Ѱַ
        {
            for(index2 = 0; index2 < tmp_inode2_p->volumn; index2++)
            {
                if ((index2 == 0) || (index2 == 1))
                    continue;
                
                if((tmp_dentry_arr[index2].type == DIRECTORY_TYPE) && (!(my_strncmp(tmp_dentry_arr[index2].file_name, (char *)&(tmp_path[index1]), FILENAME_LENGTH))))
                { // ����������1.��Ŀ¼��2.Ŀ¼����ͬ��
                    // ���������ͬ�����������ļ�����ʱ��Ӧ������
                    break;
                }                
            }
            if (index2 == tmp_inode2_p->volumn)
            {
                printk("error. directory doesn't exist.\n");
                return;
            }

            tmp_inode2_p = get_inode(tmp_dentry_arr[index2].ino); 
            load_dentry_arr(tmp_inode2_p->direct_blocks[0]); // ����һ��Ŀ¼���س���
            
            // ���¾���·��
            for (index3 = 0; index3 < FILENAME_LENGTH; index3++)
            {
                tmp_abs_path[tmp_abs_path_depth][index3] = tmp_path[index1][index3];
            }
            tmp_abs_path_depth++;        
        }


        if (tmp_abs_path_depth <= 0)
        {
            printk("error, invalid path.\n");
            return;
        }
        
    }

    // ����Ϊֹ��ִ������ȫ���Ĳ�ѯ��ַ�Ĺ�����Ŀǰ��tmp_dentry_arr�������¼���Ŀ¼��
    // ��ʱ��tmp_abs_path�Ϳ�����Ϊcurrent_path��
    
    for (index1 = 0; index1 < MAX_DIRECTORY_DEPTH; index1++)
    {
        for(index2 = 0; index2 < FILENAME_LENGTH; index2++)
        {
            current_path[index1][index2] = tmp_abs_path[index1][index2];
        }
    }

    // update global var
    path_depth = tmp_abs_path_depth;
    current_dir_ino = tmp_dentry_arr[0].ino;
    
    //printk("[FS] cd succeed!\n");//current_line++;
}

void do_ls()
{   
    current_line+=2;
    vt100_move_cursor(1, current_line);

    inode_t * tmp_inode_p;
    uint32_t index1, index2;
    spblk_t tmp_spblk;


    // check fs
    sdread(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);
    os_memcpy((void *)&tmp_spblk, (void *)read_block_buffer, sizeof(spblk_t));
    if (tmp_spblk.magic_num != MAGIC_NUM)
    {
        printk("Error! File Symtem doesn't exist.\n");//current_line++;
        return;
    }
    
    // load inode array
    load_inode_array(tmp_spblk.inode_offset);

    tmp_inode_p = get_inode(current_dir_ino);
    load_dentry_arr(tmp_inode_p->direct_blocks[0]);

    for(index1 = 0; index1 < tmp_inode_p->volumn - 1; index1++)
    {
        printk("%d  ", tmp_dentry_arr[index1].ino);
        if (index1 == 0)
            printk(".");
        else if(index1 == 1)
            printk("..");
        else
            printk(tmp_dentry_arr[index1].file_name);
        
        printk("\n");current_line++;
    }
    printk("%d  ", tmp_dentry_arr[index1].ino);
    if (index1 == 0)
        printk(".");
    else if(index1 == 1)
        printk("..");
    else
        printk(tmp_dentry_arr[index1].file_name);


    
    //printk("in do_ls().\n");

}


void do_touch(uint32_t arg_filename)
{
    char * my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);

    printk("in touch:\n");current_line++;
    while (*my_filename != '\0')
    {
        printk("%c", *my_filename);
        my_filename++;
    }
    
}

void do_cat(uint32_t arg_filename)
{
        char * my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);

    printk("in cat:\n");current_line++;
    while (*my_filename != '\0')
    {
        printk("%c", *my_filename);
        my_filename++;
    }
}

void do_fopen(char *name, int access)
{
    ;
}

void do_fread(int fd, char * buff, int size)
{
    ;
}

void do_fwrite(int fd, char *buff, int size)
{
    ;
}

void do_fclose(int fd)
{
    ;
}