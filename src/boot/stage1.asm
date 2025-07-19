[bits 16]
[org 0x7C00]

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    mov si, loading_msg
    call print_string
    
    mov ah, 0x02
    mov al, 8
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov bx, 0x1000
    int 0x13
    
    jc disk_error
    
    mov si, success_msg
    call print_string
    
    jmp 0x1000

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

disk_error:
    mov si, error_msg
    call print_string
    hlt

loading_msg db 'Loading stage 2...', 13, 10, 0
success_msg db 'Stage 2 loaded!', 13, 10, 0
error_msg db 'Disk read error!', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55