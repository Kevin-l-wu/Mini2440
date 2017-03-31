#ifndef NAND_H_
#define NAND_H_

#define BYTES_PER_PAGE 2048
#define PAGES_PER_BLOCK 64
#define BLOCK_PER_DEVICE 2048

#define SPARE_BYTES_PER_SECTOR 16
#define SPARE_BYTES_PER_PAGE 64

#define TACLS 3
#define TWRPH0 4
#define TWRPH1 3

#define DEBUG_NAND_FLASH 1

#define NFCONF (*(volatile unsigned long*)0x4E000000) 
#define NFCONT (*(volatile unsigned long*)0x4E000004)		//Nand Control Register

#define NFCMMD (*(volatile unsigned long*)0x4E000008)
#define NFADDR (*(volatile unsigned long*)0x4E00000C)
#define NFDATA (*(volatile unsigned long*)0x4E000010)

#define NFSTAT (*(volatile unsigned long*)0x4E000020)		//Nand Status Register


#define NAND_COMMAND_HELP "\
Nand test\n\
	-r : Read a page. nand -r pageAddress(hex)\n\
	-p : Write a page. nand -p pageAddress(hex) value(hex)\n\
	-e : Erase a block. nand -e pageAddress(hex)\n\
	-t : Random read. nand -t address(hex)\n\
	-a : Random write. nand -a address(hex) value(hex)\n\
	-i : Read nand ID. nand -i\n\
	-c : Bad block check. nand -c\
	"

void NandPageRead(unsigned pageAddr, unsigned* readData);

int NandProgramPage(unsigned pageAddr, unsigned* writeData);
int NandEraseBlock(unsigned pageAddr);

MINI2440_STATUS NandTest(int argc, char (*argv)[MAX_COMMAND_LENGTH]);


#endif
