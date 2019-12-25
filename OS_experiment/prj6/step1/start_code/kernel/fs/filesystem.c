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
        // 根目录需要一个块存储目录项，不过上面的used_blk_num已经加进去了
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

        root_inode.direct_blocks[0] = new_spblk.used_blk_num - 1; // 第一个data块就是用给根目录的目录项

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
        tmp_dentry_arr[0].type = DIRECTORY_TYPE;
        tmp_dentry_arr[1].type = DIRECTORY_TYPE;

        os_memcpy((void *)read_block_buffer, (void *)tmp_dentry_arr, sizeof(tmp_dentry_arr));
        sdwrite(read_block_buffer, (START_ADDRESS_SD + (BLOCK_SIZE * root_inode.direct_blocks[0])), BLOCK_SIZE);

        printk("[FS] Setting filesystem finished!\n");current_line++;
        

        // initial some global var
        path_depth = 0;
        current_dir_ino = 0;
        
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
    uint32_t inode_in_which_block; // inode在哪个块里面
    uint32_t inode_in_block_index; // inode在这个块的哪个位置

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

    for (index1 = 0; index1 < INODEBLOCK_NUM; index1++)
    {
        sdread(read_block_buffer, (START_ADDRESS_SD +( tmp_spblk.inode_offset + index1)*BLOCK_SIZE), BLOCK_SIZE);
        os_memcpy((void *)(&(tmp_inode_array[index1])), (void *)read_block_buffer, (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
    }


    // get current path inode    
    inode_in_which_block = current_dir_ino / INODE_NUM_PER_BLOCK;
    inode_in_block_index = current_dir_ino % INODE_NUM_PER_BLOCK;

    //sdread(read_block_buffer, (tmp_spblk.inode_offset + inode_in_which_block)*BLOCK_SIZE, BLOCK_SIZE);
    //os_memcpy((void *)tmp_inode_array, (void *)read_block_buffer, (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
    tmp_parent_inode_p = &tmp_inode_array[inode_in_which_block][inode_in_block_index];
    //tmp_parent_ino = tmp_inode1.parent_ino;
    if (tmp_parent_inode_p->type == FILE_TYPE)
    {
        printk("ERROR, partent type is file.");
        while(1);
    }

    // check if the new filename exists
    sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_parent_inode_p->direct_blocks[0])*BLOCK_SIZE), BLOCK_SIZE);
    os_memcpy((void *)tmp_dentry_arr, (void *)read_block_buffer, (DENTRY_NUM_PER_BLOCK*sizeof(dentry_t)));
    
    
    if(tmp_parent_inode_p->volumn > 2)
    {
        for (index2 = 0; index2 < tmp_parent_inode_p->volumn; index2 ++)
        {
            if((tmp_dentry_arr[index2].type == DIRECTORY_TYPE) && (!(my_strncmp(tmp_dentry_arr[index2].file_name, my_filename, FILENAME_LENGTH))))
            { // 两个条件：1.是目录；2.目录出现同名
                // 如果仅仅是同名，但是是文件，此时还是可以创建同名的目录的
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
    inode_in_which_block = new_ino / INODE_NUM_PER_BLOCK;
    inode_in_block_index = new_ino % INODE_NUM_PER_BLOCK;
    tmp_inode2_p = &tmp_inode_array[inode_in_which_block][inode_in_block_index];


    tmp_parent_ino = tmp_dentry_arr[0].ino; // 记下父目录的ino，否则会丢失此信息
    os_memcpy(parent_name, tmp_dentry_arr[0].file_name, sizeof(tmp_dentry_arr[0].file_name)); // 记录下父目录的名字


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
    


    //inode array不必更新，因为之前的初始化新inode操作都是指针上进行的，直接指向的inode array对应的成员
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

    // 更新父目录
    sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_parent_inode_p->direct_blocks[0])*BLOCK_SIZE), BLOCK_SIZE);
    os_memcpy((void *)tmp_dentry_arr, (void *)read_block_buffer, (DENTRY_NUM_PER_BLOCK*sizeof(dentry_t)));
    
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

    // 初始化新的目录
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
    for (index1 = 0; index1 < INODEBLOCK_NUM; index1++)
    {
        sdread(read_block_buffer, (START_ADDRESS_SD +( tmp_spblk.inode_offset + index1)*BLOCK_SIZE), BLOCK_SIZE);
        os_memcpy((void *)(&(tmp_inode_array[index1])), (void *)read_block_buffer, (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
    }

    // get current path inode    
    inode_in_which_block = current_dir_ino / INODE_NUM_PER_BLOCK;
    inode_in_block_index = current_dir_ino % INODE_NUM_PER_BLOCK;
    
    tmp_parent_inode_p = &tmp_inode_array[inode_in_which_block][inode_in_block_index];
    //tmp_parent_ino = tmp_inode1.parent_ino;
    if (tmp_parent_inode_p->type == FILE_TYPE)
    {
        printk("ERROR, partent type is file.");
        while(1);
    }

    // check if the filename exists
    sdread(read_block_buffer, (START_ADDRESS_SD + (tmp_parent_inode_p->direct_blocks[0])*BLOCK_SIZE), BLOCK_SIZE);
    os_memcpy((void *)tmp_dentry_arr, (void *)read_block_buffer, (DENTRY_NUM_PER_BLOCK*sizeof(dentry_t)));
    
    
    for (index2 = 0; index2 < tmp_parent_inode_p->volumn; index2 ++)
    {
        if((tmp_dentry_arr[index2].type == DIRECTORY_TYPE) && (!(my_strncmp(tmp_dentry_arr[index2].file_name, my_filename, FILENAME_LENGTH))))
        { // 两个条件：1.是目录；2.目录出现同名
            // 如果仅仅是同名，但是是文件，此时不应删除这个同名的文件
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
    inode_in_which_block = delete_ino / INODE_NUM_PER_BLOCK;
    inode_in_block_index = delete_ino % INODE_NUM_PER_BLOCK;
    tmp_inode1_p = &tmp_inode_array[inode_in_which_block][inode_in_block_index];
    delete_blk_num = tmp_inode1_p->direct_blocks[0]; // 先记住，要不然等下就没了

    // 在此直接更新该列表，后面就不用再重复做了
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
    
    // 更新inode 数组
    
    tmp_parent_inode_p->volumn--;
    init_inode(tmp_inode1_p);
    for (index2 = 0; index2 < INODEBLOCK_NUM; index2++)
    {
        os_memcpy((void *)read_block_buffer, (void *)(&(tmp_inode_array[index2])), (INODE_NUM_PER_BLOCK*sizeof(inode_t)));
        sdwrite(read_block_buffer, (START_ADDRESS_SD +( tmp_spblk.inode_offset + index2)*BLOCK_SIZE), BLOCK_SIZE);    
    }

    // 更新超级块
    tmp_spblk.used_blk_num--;
    tmp_spblk.used_inode_num--;
    os_memcpy((void *)read_block_buffer, (void *)&tmp_spblk, sizeof(spblk_t));    
    sdwrite(read_block_buffer, START_ADDRESS_SD, BLOCK_SIZE);

    

    // 删除（初始化）自己的目录

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
    int index1;
    char *my_filename = (char *)arg_filename;
    spblk_t tmp_spblk;
    
    
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






    printk("in cd. filename:\n");current_line++;
    for(index1 = 0; (index1 < FILENAME_LENGTH) && (my_filename[index1] != '\0'); index1++)
    {
        printk("%c", my_filename[index1]);
    }
}

void do_ls()
{   
    current_line+=2;
    vt100_move_cursor(1, current_line);





    
    printk("in do_ls().\n");

}