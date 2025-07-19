; Simple, stable bootloader that works
[BITS 16]
[ORG 0x7C00]

start:
    ; Setup segments
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    
    ; Save boot drive
    mov [boot_drive], dl
    
    ; Set video mode
    mov ax, 0x0003
    int 0x10
    
    ; Print message
    mov si, boot_msg
    call print_string
    
    ; Load kernel
    mov ah, 0x02        ; Read sectors
    mov al, 15          ; Read 15 sectors (enough for kernel)
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start from sector 2
    mov dh, 0           ; Head 0
    mov dl, [boot_drive]
    mov bx, 0x8000      ; Load at 0x8000
    int 0x13
    jc disk_error
    
    ; Success message
    mov si, success_msg
    call print_string
    
    ; Jump directly to kernel (skip protected mode for now)
    jmp 0x8000

disk_error:
    mov si, error_msg
    call print_string
    jmp hang

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

hang:
    hlt
    jmp hang

; Data
boot_drive: db 0
boot_msg: db "Simple VibexOS Bootloader", 0x0D, 0x0A, "Loading kernel...", 0x0D, 0x0A, 0
success_msg: db "Kernel loaded! Starting...", 0x0D, 0x0A, 0
error_msg: db "Disk error!", 0x0D, 0x0A, 0

; Boot signature
times 510-($-$$) db 0
dw 0xAA55