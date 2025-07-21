#include "../include/idt.h"

extern void idt_load(unsigned int);
extern void keyboard_interrupt(void);

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base = (unsigned int)&idt;
    
    // Clear the IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }
    
    // Set up keyboard interrupt (IRQ1 = INT 0x21)
    idt_set_gate(0x21, (unsigned long)keyboard_interrupt, 0x08, 0x8E);
    
    // Load the IDT
    idt_load((unsigned int)&idtp);
    
    // Remap PIC
    // Master PIC
    asm volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0x20));  // ICW1
    asm volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x21));  // ICW2 - IRQ0-7 -> INT 0x20-0x27
    asm volatile("outb %%al, %%dx" : : "a"(0x04), "d"(0x21));  // ICW3
    asm volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0x21));  // ICW4
    
    // Slave PIC
    asm volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0xA0));  // ICW1
    asm volatile("outb %%al, %%dx" : : "a"(0x28), "d"(0xA1));  // ICW2 - IRQ8-15 -> INT 0x28-0x2F
    asm volatile("outb %%al, %%dx" : : "a"(0x02), "d"(0xA1));  // ICW3
    asm volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0xA1));  // ICW4
    
    // Unmask keyboard interrupt (IRQ1)
    asm volatile("outb %%al, %%dx" : : "a"(0xFD), "d"(0x21));  // Enable IRQ1 only
}