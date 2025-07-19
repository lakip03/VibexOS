[BITS 16]

global start
start:
    ; Clear screen
    mov ax, 0x0003
    int 0x10
    
    ; Display message using BIOS
    mov si, kernel_msg
    call print_string
    
    ; Test our printf functionality by switching to 32-bit
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode

print_string:
    pusha
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    popa
    ret

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    
    ; Now call our C kernel
    extern _start
    call _start
    
    jmp $

; GDT
gdt_start:
    dd 0x0, 0x0
    
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
    
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

kernel_msg: db "Kernel starting in 16-bit mode...", 0x0D, 0x0A, 0