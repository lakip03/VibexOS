; Stage 1 Bootloader - Pure ASM
; Loads Stage 2 (C bootloader) and transfers control

[BITS 16]
[ORG 0x7C00]

start:
    ; Initialize segments
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
    
    ; Print stage 1 message
    mov si, stage1_msg
    call print_string
    
    ; Load stage 2 bootloader (C code)
    mov ah, 0x02        ; Read sectors
    mov al, 10          ; Read 10 sectors for stage 2
    mov ch, 0           ; Cylinder 0
    mov cl, 2           ; Start from sector 2
    mov dh, 0           ; Head 0
    mov dl, [boot_drive]
    mov bx, 0x1000      ; Load stage 2 at 0x1000
    int 0x13
    jc disk_error
    
    ; Verify sectors read
    cmp al, 10
    jne disk_error
    
    ; Print success message
    mov si, stage2_loaded_msg
    call print_string
    
    ; Jump to stage 2 (C bootloader)
    jmp 0x1000

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
stage1_msg: db "VibexOS Stage 1 Bootloader", 0x0D, 0x0A, "Loading Stage 2...", 0x0D, 0x0A, 0
stage2_loaded_msg: db "Stage 2 loaded successfully!", 0x0D, 0x0A, "Transferring control...", 0x0D, 0x0A, 0
error_msg: db "FATAL: Disk read error in Stage 1!", 0x0D, 0x0A, 0

; Pad to 510 bytes
times 510-($-$$) db 0
dw 0xAA55