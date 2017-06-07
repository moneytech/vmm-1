#ifndef CONSTS_H
#define CONSTS_H

#define BOOTLOADER_SIZE 0x800
#define PAGING_PHYS_ADDR 0x100000
#define PAGING_VMA_ADDR 0x100000
#define GDT_PHYS_ADDR 0x501000
#define GDT_VMA_ADDR 0xffff800000000000
#define KERNEL_LOAD_ADDR 0x8600
#define KERNEL_PHYS_ADDR 0x408000
#define HHKERN_PHYS_ADDR 0x400000
#define HHKERN_VMA_ADDR 0xffff800000000000
#define KERNEL_VMA_ADDR 0xffff800000008000
#define KERNEL_SIZE 0x10000
#define KERNEL_STACK_PHYS_ADDR 0x407000
#define KERNEL_STACK_VMA_ADDR 0xffff800000007000
#define HEAP_VMA_ADDR 0xffff800000010000
#define HEAP_SIZE 0x100000
#define ENTERLM_ADDR 0x8400
#define ENTERLM_SIZE 0x200
#define PML4_OFF 0
#define PDP_OFF 0x1000
#define PD_OFF 0x2000
#define PDP_KERNEL_OFF 0x7000
#define PD_KERNEL_OFF 0x8000
#define AP_ENTER_LOAD 0x10600
#define AP_PHYS_LOAD 0x7000

#endif