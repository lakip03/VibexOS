[bits 16]
[org 0x1000]

stage2_start:
    mov si, stage2_msg
    call print_string
    
    call enable_a20
    
    call load_kernel
    
    cli
    lgdt [gdt_descriptor]
    
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    jmp CODE_SEG:protected_mode_start

enable_a20:
    mov si, a20_msg
    call print_string
    
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

load_kernel:
    mov si, kernel_load_msg
    call print_string
    
    mov ah, 0x02
    mov al, 64
    mov ch, 0
    mov cl, 10
    mov dh, 0
    mov bx, 0x8000
    int 0x13
    
    jc kernel_load_error
    ret

kernel_load_error:
    mov si, kernel_error_msg
    call print_string
    hlt

print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

[bits 32]
protected_mode_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    
    jmp 0x8000

gdt_start:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

stage2_msg db 'Stage 2 bootloader loaded!', 13, 10, 0
a20_msg db 'Enabling A20 line...', 13, 10, 0
kernel_load_msg db 'Loading kernel...', 13, 10, 0
kernel_error_msg db 'Kernel load error!', 13, 10, 0

times 4096-($-$$) db 0