#ifndef INCLUDE_REAL_FS_H
#define INCLUDE_REAL_FS_H

#include "type.h"


#define START_ADDRESS_SD 0x20000000 // start address of file system in SD card
#define FILENAME_LENGTH 56
#define BLOCK_SIZE 0x1000 // 4KB block size
#define BLOCK_NUM 0x20000 // 1024*128 blocks
#define BITMAP_ARRAY_LENGTH 0x1000 // 1024*128 bit = 0x20000 bit = 0x4000 B = 0x1000 uint32_t

#define INODE_NUM  0x100     // num of file & directory
#define INODE_BITMAP_ARRAY_LENGTH 8  // 0x100 bit = 32 B = 8 uint32_t

#define MAGIC_NUM 0x114514

#define DIRECT_LINK_NUM 10

#define MAX_DIRECTORY_DEPTH 4 

#define INODE_NUM_PER_BLOCK (BLOCK_SIZE/sizeof(inode_t))
#define INODEBLOCK_NUM ((INODE_NUM * sizeof(inode_t))/BLOCK_SIZE)

#define DENTRY_NUM_PER_BLOCK (BLOCK_SIZE/sizeof(dentry_t))

#define BLOCK_NUM_OF_BLOCK_BITMAP ((BITMAP_ARRAY_LENGTH * sizeof(uint32_t))/BLOCK_SIZE)
/*file system:

|superblock|sector/block map|inode map|inode|data|

*/

typedef struct superblock
{
    uint32_t magic_num;             // magic number
    uint32_t fs_size;               // size of file system
    uint32_t map_offset;            // block map offset in SD card
    uint32_t inode_map_offset;      // inode map offset in SD card
    uint32_t inode_offset;          // inode offset in SD card
    uint32_t data_offset;           // data offset in SD card

    uint32_t used_blk_num;          // num of used blocks
    uint32_t used_inode_num;        // num of used inodes
    //char padding[];                 // padding to B
}spblk_t;

typedef struct inode
{
    uint32_t type;                                  // file or directory
    //uint32_t mode;                                // authority of this file
    uint32_t last_update_time;                      // last update time
    uint32_t volumn;                                // if type is file, this means file size
                                                    // if type is directory, this means entry num
    //uint32_t file_sz;                               // file size when inode type is a file
    //uint32_t entry_num;                             // the num of entries when inode type is a directory. "." and ".." are not counted
    uint32_t parent_ino;                            // ino of parent directory
    uint32_t direct_blocks[DIRECT_LINK_NUM];        // block number of direct blocks
    uint32_t indirect_blocks;                       // block number of 1-floor indirect blocks
    uint32_t double_indirect_blocks;                // block number of 2-floor indirect blocks
    // char padding[];
}inode_t;

enum{
    FILE_TYPE,
    DIRECTORY_TYPE,
};

enum{
    BITMAP_ADD,
    BITMAP_DEL,
};

typedef struct directory_entry
{
    char file_name[FILENAME_LENGTH];
    uint32_t type;
    uint32_t ino;
}dentry_t;


typedef struct file_descriptor
{
    //uint32_t self_index;    // index in file descrpitor array
    uint32_t ino;           // inode num
    uint32_t avail;         // information about user authority
    uint32_t read_offset;
    uint32_t write_offset;
}fd_t;


uint32_t block_bitmap[BITMAP_ARRAY_LENGTH]; // need padding

uint32_t inode_bitmap[INODE_BITMAP_ARRAY_LENGTH]; // need padding

char current_path[MAX_DIRECTORY_DEPTH][FILENAME_LENGTH];
uint32_t path_depth;        // depth of current path
uint32_t current_dir_ino; // inode number of current directory

//uint32_t inode_num_per_block; 

void do_mkfs();
void do_statfs();
void do_mkdir(uint32_t arg_filename);
void do_rmdir(uint32_t arg_filename);
void do_cd(uint32_t arg_filename);
void do_ls();



#endif