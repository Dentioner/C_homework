#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECTOR_SIZE 512


void write_bootblock(FILE *image, FILE *bbfile, Elf32_Phdr *Phdr);
Elf32_Phdr *read_exec_file(FILE *opfile, Elf32_Ehdr *ehdr);
Elf32_Ehdr *read_elf_head(FILE *opfile);
uint32_t count_kernel_sectors(Elf32_Phdr *Phdr);
uint32_t count_kernel_sectors_SD(Elf32_Phdr *Phdr);
void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz, int has_bootblock, int has_kernel, int sector_number);
void create_image_padding(uint32_t kernel_sector_number);

char temp[SECTOR_SIZE];

void create_image_padding(uint32_t kernel_sector_number)
{
	long offset;
	offset = (1 + (long)kernel_sector_number)*SECTOR_SIZE - 1 ;
	FILE *image = fopen("image", "wb");
	fseek(image, offset, SEEK_SET);
	fputc('\0', image);
	return;
}


Elf32_Ehdr *read_elf_head(FILE *opfile)
{
	Elf32_Ehdr *elf_head = (Elf32_Ehdr*)malloc(sizeof(Elf32_Ehdr));
	fread(elf_head, sizeof(Elf32_Ehdr), 1, opfile);	
	fseek(opfile, 0, SEEK_SET);//back to the front

	return elf_head;
}

Elf32_Phdr *read_exec_file(FILE *opfile, Elf32_Ehdr *ehdr)
{
	
	Elf32_Phdr *image_head = (Elf32_Phdr*)malloc(sizeof(Elf32_Phdr));
	fseek(opfile, (long)ehdr->e_phoff, SEEK_SET);
	fread(image_head, sizeof(Elf32_Phdr), 1, opfile);	
	fseek(opfile, 0, SEEK_SET);//back to the front
	return image_head;
}

uint32_t count_kernel_sectors(Elf32_Phdr *Phdr)
{
	//long int size =(long int) Phdr->p_filesz;
	long int size =(long int) Phdr->p_memsz;
	
	uint32_t sector_number = (uint32_t)(size/SECTOR_SIZE + 1);
	//uint8_t sector_number = (uint8_t)((size + SECTOR_SIZE - 1)/SECTOR_SIZE);
	return sector_number;	
}

uint32_t count_kernel_sectors_SD(Elf32_Phdr *Phdr)
{
	long int size =(long int) Phdr->p_filesz;
	
	
	uint32_t sector_number = (uint32_t)(size/SECTOR_SIZE + 1);
	//uint8_t sector_number = (uint8_t)((size + SECTOR_SIZE - 1)/SECTOR_SIZE);
	return sector_number;	
}


void write_bootblock(FILE *image, FILE *file, Elf32_Phdr *phdr)
{
	unsigned int mem_size, file_size;
	file_size = (unsigned int)phdr->p_filesz;
	mem_size = (unsigned int)phdr->p_memsz;
/*
mem_sz >= file_size because of the existence of .bss data
date in SD card just has the file_sz
but in memory the .bss data is also taken into consideration
so we should padding the file from file_sz to mem_sz;
*/
	fseek(file, phdr->p_offset, SEEK_SET);
	fread(temp, phdr->p_filesz, 1, file);

	fseek(image, 0, SEEK_SET);
	fwrite(temp, (size_t)(phdr->p_filesz), 1, image);
	fwrite('\0', sizeof('0'), (size_t)(mem_size - file_size), image);//padding

//write 0x55aa	
	fseek(image, (long)(SECTOR_SIZE - 2), SEEK_SET);
	fputc(0x55, image);
	fputc(0xaa, image);

	
	return;
}

void write_kernel(FILE *image, FILE *knfile, Elf32_Phdr *Phdr, uint32_t kernelsz, uint32_t kernelszSD)
{
	int index;
	
	fseek(image, SECTOR_SIZE, SEEK_SET);
	fseek(knfile, Phdr->p_offset, SEEK_SET);
	for(index = 0; index < Phdr->p_filesz/512; index++)
	{
		fread(temp, SECTOR_SIZE, 1, knfile);		
		fwrite(temp, SECTOR_SIZE, 1, image);
		//do not need to move file pointer artificially?
	//	fseek(image, SECTOR_SIZE, SEEK_CUR);
	//	fseek(knfile, SECTOR_SIZE, SEEK_CUR);
	}
	
	if((Phdr->p_filesz)%512)
	{
		memset(temp, 0, SECTOR_SIZE);
		fread(temp, 1, (Phdr->p_filesz)%512, knfile);
		fwrite(temp, 1, SECTOR_SIZE, image);
	}
	
	memset(temp, 0, SECTOR_SIZE);
	for(; index <Phdr->p_memsz / 512; index++)
	{
		//fread(temp, SECTOR_SIZE, 1, knfile);		
		fwrite(temp, SECTOR_SIZE, 1, image);
		//do not need to move file pointer artificially?
	//	fseek(image, SECTOR_SIZE, SEEK_CUR);
	//	fseek(knfile, SECTOR_SIZE, SEEK_CUR);
	}
	fwrite(temp, 1, (Phdr->p_memsz) % 512, image);
	return;

}

void record_kernel_sectors(FILE *image, uint32_t kernelsz)
{
	long int size = (long int)kernelsz ;
	long int offset_ker =(long int)( SECTOR_SIZE - 2 - sizeof(uint8_t));	
	uint32_t *ptr_of_kersz = (uint32_t*)malloc(sizeof(uint32_t));
	*ptr_of_kersz = kernelsz;

 	//fseek(image, offset_ker, SEEK_SET);
	//fwrite(ptr_of_kersz, sizeof(uint8_t), 1, image);
	fseek(image, 508, SEEK_SET);
	fwrite(ptr_of_kersz, 4, 1, image);

	return;	 
}

void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz, int has_bootblock, int has_kernel, int sector_number)
{
//????????????????????????????????????????????????
	printf("=============================================================\n");
	printf("My extent_opt function:\n");
	if(has_bootblock)
	{
		printf("%#010x: bootblock\n", Phdr_bb->p_vaddr);
		printf("\toffset %#06x\tvaddr %#010x\n", Phdr_bb->p_offset, Phdr_bb->p_vaddr);
		printf("\tfilesz %#06x\tmemsz %#06x\n", Phdr_bb->p_filesz, Phdr_bb->p_memsz);
		printf("\twriting %#06x bytes\n", Phdr_bb->p_memsz);
		printf("\tpadding up to %#06x\n", SECTOR_SIZE);
	}

	if(has_kernel)
	{
		//printf("%#010x: kernel\n", Phdr_k->p_vaddr);
		printf("%#010x: main\n", Phdr_k->p_vaddr);
		printf("\toffset %#06x\tvaddr %#010x\n", Phdr_k->p_offset, Phdr_k->p_vaddr);
		printf("\tfilesz %#06x\tmemsz %#06x\n", Phdr_k->p_filesz, Phdr_k->p_memsz);
		printf("\twriting %#06x bytes\n", Phdr_k->p_memsz);
		printf("\tsector_number %d\n", sector_number);
	}

	
	printf("=============================================================\n");
	return;
}

//int main()
int main(int argc, char* argv[])
{
	FILE* image,* bootblock,* kernel;

	Elf32_Ehdr * image_ehdr = NULL;
	Elf32_Ehdr * bootblock_ehdr = NULL;
	Elf32_Ehdr * kernel_ehdr = NULL;

	Elf32_Phdr * image_head = NULL;
	Elf32_Phdr * bootblock_head = NULL;
	Elf32_Phdr * kernel_head = NULL;

	uint32_t kernel_sector_number;
	uint32_t kernel_sector_number_SD;
	int kernel_size_int;//int? or uint8_t?
	//uint8_t kernel_size_u8t;	
	const char c_bootblock[10] = "bootblock";
	//const char c_kernel[7] = "kernel";
	const char c_kernel[5] = "main"; 

	memset(temp, 0, SECTOR_SIZE);

	bootblock = fopen("bootblock", "rb");
	//kernel = fopen("kernel", "rb");
	kernel = fopen("main", "rb");

	bootblock_ehdr = read_elf_head(bootblock);
	kernel_ehdr = read_elf_head(kernel);

	bootblock_head = read_exec_file(bootblock, bootblock_ehdr);
	kernel_head = read_exec_file(kernel, kernel_ehdr);

	kernel_sector_number = count_kernel_sectors(kernel_head);
	kernel_sector_number_SD = count_kernel_sectors_SD(kernel_head);


	kernel_size_int = (int)(kernel_head->p_filesz);	
	//kernel_size_u8t = (uint8_t)(kernel_head->p_filesz);	


	create_image_padding(kernel_sector_number);
	image = fopen("image", "wb");
	image_ehdr = read_elf_head(image);
	image_head = read_exec_file(image, image_ehdr);

	write_bootblock(image, bootblock, bootblock_head);
	write_kernel(image, kernel, kernel_head, kernel_sector_number, kernel_sector_number_SD);
	record_kernel_sectors(image, kernel_sector_number);
	
	if(argc > 1)
	{
		if (!strncmp(c_bootblock, argv[2], 9))
		{
			//if(!strncmp(c_kernel, argv[3], 6))
			if(!strncmp(c_kernel, argv[3], 4))
				extent_opt(bootblock_head, kernel_head, kernel_size_int, 1, 1, kernel_sector_number);//--extend bootblock kernel
			else
				extent_opt(bootblock_head, kernel_head, kernel_size_int, 1, 0, kernel_sector_number);//--extend bootblock
		}
		//else if(!strncmp(c_kernel, argv[2], 6))
		else if(!strncmp(c_kernel, argv[2], 4))			
		{
			if (!strncmp(c_bootblock, argv[3], 9))
				extent_opt(bootblock_head, kernel_head, kernel_size_int, 1, 1, kernel_sector_number);//--extend kernel bootblock
			else
				extent_opt(bootblock_head, kernel_head, kernel_size_int, 0, 1, kernel_sector_number);//--extend kernel
		}
	}
	fcloseall();
	


	return 0;
}

