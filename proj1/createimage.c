/* Author(s): John McSpedon <mcspedon>, Rob Sami <rsami> 
 * COS 318, Fall 2013: Project 1 Bootloader
 * Creates operating system image suitable for placement on a boot disk
*/
/* Largely unimplemented */
#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512       /* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
// more defines...

/* Reads in an executable file in ELF format*/
Elf32_Phdr * read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr){
  size_t numread;
  Elf32_Phdr *phdr = malloc(sizeof(Elf32_Phdr)); //ELF Program header
  // TODO: error stuff
  
  // read in ELF header i.e. read sizeof(Elf32_Ehdr) bytes into buffer ehdr
  numread = fread(*ehdr, 1, sizeof(Elf32_Ehdr), *execfile);  
  //TODO: error stuff

  //Debug
  printf("I think e_phentsize is %d", (**ehdr).e_phentsize);
  
  // populate fields
  (*phdr).p_type = NULL;
  (*phdr).p_offset = NULL;
  (*phdr).p_vaddr = NULL;
  (*phdr).p_paddr = NULL;
  (*phdr).p_filesz = NULL;
  (*phdr).p_memsz = NULL;
  (*phdr).p_flags = NULL;
  (*phdr).p_align = NULL;

  return NULL;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile, FILE *bootfile, Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
 

}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){

 
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
    
  return 0;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec){
    
}


/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec){

  /* print number of disk sectors used by the image */

  
  /*bootblock segment info */
 

  /* print kernel segment info */
  

  /* print kernel size in sectors */
}

// more helper functions...

/* MAIN */
// ignore the --vm argument when implementing (project 1)
int main(int argc, char **argv){
 /* argv = (argc == 3/4)
 * 	-/1: '--extended'
 * 	1/2: './bootblock'
 * 	2/3: './kernel'
 */

  FILE *kernelfile, *bootfile,*imagefile;  //file pointers for bootblock,kernel and image
  Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));//bootblock ELF header
  Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr));//kernel ELF header

  Elf32_Phdr *boot_phdr; //bootblock ELF program header
  Elf32_Phdr *kernel_phdr; //kernel ELF program header

  /* check for legal num of args */
  if (argc != 3 && argc !=4) {
	fprintf(stderr, "Error: call as '%s (--extended) bootblock-location kernel-location'\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* open image file for writing*/
  if(!(imagefile = fopen(IMAGE_FILE, "w+"))) {
    fprintf(stderr, "Error creating %s: %s\n", IMAGE_FILE, strerror(errno));
    exit(EXIT_FAILURE);
  }


  /* read executable bootblock file   
   *     note that here filename = argv[argc-2]; */
  if(!(bootfile = fopen(argv[argc-2], "r"))){
    fprintf(stderr, "Error reading %s: %s\n", argv[argc-2], strerror(errno));
    exit(EXIT_FAILURE);
  }
  boot_phdr = read_exec_file(&bootfile, argv[argc-2], &boot_header);

  /* write bootblock */  
  write_bootblock(&imagefile, bootfile, boot_header, boot_phdr);

  /* read executable kernel file */

  /* write kernel segments to image */

  /* tell the bootloader how many sectors to read to load the kernel */

  /* check for  --extended option */
  if(!strncmp(argv[1],"--extended",11)){
	/* print info */
  }
  
  // TODO: destroy boot header, kernel_header, boot_program_header, kernel_program_header
  
  return 0;
} // ends main()



