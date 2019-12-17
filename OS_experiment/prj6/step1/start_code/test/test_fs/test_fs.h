#ifndef INCLUDE_TEST_FS_H_
#define INCLUDE_TEST_FS_H_

#include "type.h"

#define START_ADDRESS_SD 0x20000000 // start address of file system in SD card
#define FILENAME_LENGTH 50
#define BLOCK_SIZE 0x1000 // 4KB block size
#define BLOCK_NUM 0x20000 // 1024*128 blocks
#define BITMAP_SIZE 0x1000 // 1024*128 bit = 0x20000 bit = 0x4000 B = 0x1000 uint32_t
/*file system:

|superblock|sector/block map|inode map|inode|data|

*/

typedef struct superblock
{
    uint32_t magic_num;             // magic number
    uint32_t fs_size;               // size of file system
    uint32_t map_offset;            // sector/block map offset in SD card
    uint32_t inode_map_offset;      // inode map offset in SD card
    uint32_t inode_offset;          // inode offset in SD card
    uint32_t data_offset;           // data offset in SD card
}spblk_t;

typedef struct inode
{
    /* data */
}inode_t;

typedef struct 
{
    /* data */
};


uint32_t block_bitmap[BITMAP_SIZE];

void test_fs();

void do_mkfs();
void do_statfs();
void do_mkdir(uint32_t arg_filename);
void do_rmdir(uint32_t arg_filename);
void do_cd(uint32_t arg_filename);
void do_ls();

#endif