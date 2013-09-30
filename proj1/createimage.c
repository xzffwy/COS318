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
  size_t numread; //used for error checks
  Elf32_Phdr *phdr = malloc(sizeof(Elf32_Phdr)); //ELF Program header
  // TODO: error stuff
  
  //open file
  printf("Opening file %s\n", filename); //DEBUG
  if(!(*execfile = fopen(filename, "r"))){
    fprintf(stderr, "Error reading %s: %s\n", filename, strerror(errno));
    exit(EXIT_FAILURE);
  }

  // read in ELF header
  numread = fread(*ehdr, 1, sizeof(Elf32_Ehdr), *execfile);  
  //TODO: error stuff
  //printf("I think e_phentsize is %d", (**ehdr).e_phentsize); //Debug
  
  // read in 1st program header
  assert((**ehdr).e_phentsize == sizeof(Elf32_Phdr)); // check we're reading correct amount
    //set read position to e_phoff
  fseek(*execfile, (**ehdr).e_phoff, SEEK_SET);
    // perform read
  numread = fread(phdr, 1, sizeof(Elf32_Phdr), *execfile);
  //TODO: error stuff
  //printf("I think p_memsz for %s is %x\n", filename, (*phdr).p_memsz); //Debug
  //printf("I think p_offset for %s is %x\n", filename, (*phdr).p_offset); //Debug

  
  return phdr;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile, FILE *bootfile, Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
  size_t required_padding; // amount of padding required to bring bootloader code to full sector size in image
  size_t numremaining, num2read, numread; // number of bytes remaining to r/w, to r/w this turn, and r/w during this loop

  char buffer[SECTOR_SIZE]; // buffer for copying a sector's worth of bytes

  /* set file offsets */
  fseek(*imagefile, 0, SEEK_SET);
  fseek(bootfile, (*boot_phdr).p_offset, SEEK_SET); // this is where first byte of program resides

  /* copy in bootloader code (p_filesz bytes) */
  numremaining = (*boot_phdr).p_filesz;
  required_padding = (SECTOR_SIZE - numremaining) % SECTOR_SIZE;
  //printf("Hi. My file size is %d bytes and I need %d bytes of padding\n", numremaining, required_padding);
  while(numremaining > 0) {
    // read bytes   ( num2read = min(numremaining, SECTOR_SIZE) )
    num2read = (numremaining < SECTOR_SIZE) ? numremaining : SECTOR_SIZE;
    numread = fread(buffer, 1, num2read, bootfile);
    assert(numread == num2read); //TODO
    // write bytes
    numread = fwrite(buffer, 1, num2read, *imagefile);
    assert(numread == num2read); //TODO
    // decrement counters
    numremaining -= numread;
  }

  /* pad with zeroes up to 0x1fe */
  while(required_padding > 0) {
    fputc(0, *imagefile);
    required_padding--;
  }

  /* write signature to end of first sector */
  fseek(*imagefile, BOOTLOADER_SIG_OFFSET, SEEK_SET);
  fputc(0x55, *imagefile);
  fputc(0xAA, *imagefile);
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile, FILE *kernelfile, Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  size_t required_padding; // amount of padding required to bring kernel code to full sector size in image
  size_t numremaining, num2read, numread; // number of bytes remaining to r/w, to r/w this turn, and r/w during this loop
  char buffer[SECTOR_SIZE]; // buffer for copying a sector's worth of bytes


  /* set file offset to second segment */
  fseek(*imagefile, SECTOR_SIZE, SEEK_SET);
  fseek(kernelfile, (*kernel_phdr).p_offset, SEEK_SET); // this is where first byte of program resides

  /* copy kernel image (p_filesz bytes) */
  numremaining = (*kernel_phdr).p_filesz;
  required_padding = (SECTOR_SIZE - numremaining) % SECTOR_SIZE;
  printf("Hi. My file size is %d bytes and I need %d bytes of padding\n", numremaining, required_padding);
  while(numremaining > 0) {
    // read bytes   ( num2read = min(numremaining, SECTOR_SIZE) )
    num2read = (numremaining < SECTOR_SIZE) ? numremaining : SECTOR_SIZE;
    numread = fread(buffer, 1, num2read, kernelfile);
    assert(numread == num2read); //TODO
    // write bytes
    numread = fwrite(kernelfile, 1, num2read, *imagefile);
    assert(numread == num2read); //TODO
    // decrement counters
    numremaining -= numread;
  }


  /* pad to next sector */
  while(required_padding > 0) {
    fputc(0, *imagefile);
    required_padding--;
  }
 
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  size_t kernel_size = (*kernel_phdr).p_filesz;
  return (int) kernel_size / SECTOR_SIZE + ((kernel_size % SECTOR_SIZE == 0)? 0 : 1);
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec) {
    /* write over bytes 3-6 inclusive of file */
    fseek(*imagefile, 2, SEEK_SET);  //TODO: make 2 a constant
    fwrite(&num_sec, sizeof(int), 1, imagefile);
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
  int num_sectors;

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
  boot_phdr = read_exec_file(&bootfile, argv[argc-2], &boot_header);

  /* write bootblock */  
  write_bootblock(&imagefile, bootfile, boot_header, boot_phdr);

  /* read executable kernel file
   *     note that here filename = argv[argc-1]; */
  kernel_phdr = read_exec_file(&kernelfile, argv[argc-1], &kernel_header);

  /* write kernel segments to image */
  write_kernel(&imagefile, kernelfile, kernel_header, kernel_phdr);

  /* tell the bootloader how many sectors to read to load the kernel */
  num_sectors = count_kernel_sectors(kernel_header, kernel_phdr);
  printf("My kernel image occupies %d sectors!\n", num_sectors);
  //record_kernel_sectors(imagefile, kernel_header, kernel_phdr, num_sectors);


  /* check for  --extended option */
  if(!strncmp(argv[1],"--extended",11)){
    printf("Extend what?!?\n");
//	  extended_opt(boot_phdr, (int) (*kernel_header).e_phnum, kernel_phdr, num_sectors);
  }
  
  /*// Free Memory
  free(boot_header);
  free(kernel_header);
  free(boot_phdr);
  free(kernel_phdr);

  // Close Files
  fclose(kernelfile);
  fclose(bootfile);
  fclose(imagefile);*/
  
  return 0;
} // ends main()



