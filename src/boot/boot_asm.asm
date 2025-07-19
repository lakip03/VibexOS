; Assembly support functions for Stage 2 bootloader

[BITS 16]

global bios_disk_read
global enable_a20
global switch_to_protected_mode
global start

extern stage2_main

; Entry point for stage 2
section .text
start:
    ; Setup segments for C code
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000      ; Set stack for C code
    
    ; Load kernel first before calling C code
    mov ah, 0x02        ; Read sectors
    mov al, 20          ; Read 20 sectors
    mov ch, 0           ; Cylinder 0
    mov cl, 12          ; Start from sector 12
    mov dh, 0           ; Head 0
    mov dl, 0x80        ; First hard drive
    mov bx, 0x8000      ; Load kernel at 0x8000
    int 0x13
    jc disk_error
    
    ; Call C main function
    call stage2_main
    
    ; Should never reach here
    jmp $

disk_error:
    ; Display error using BIOS
    mov si, disk_error_msg
    call print_bios_string
    jmp hang_here

print_bios_string:
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

hang_here:
    hlt
    jmp hang_here

disk_error_msg: db "Disk error in stage 2!", 0x0D, 0x0A, 0

; BIOS disk read function
; Parameters: drive, head, cylinder, sector, count, buffer
bios_disk_read:
    push bp
    mov bp, sp
    pusha
    
    ; Get parameters from stack
    mov dl, [bp + 4]    ; drive
    mov dh, [bp + 6]    ; head  
    mov ch, [bp + 8]    ; cylinder
    mov cl, [bp + 10]   ; sector
    mov al, [bp + 12]   ; count
    mov bx, [bp + 14]   ; buffer
    
    ; Set up for BIOS call
    mov ah, 0x02        ; Read sectors function
    int 0x13            ; BIOS disk interrupt
    
    ; Return error code in AX
    jc .error
    xor ax, ax          ; Success
    jmp .done
.error:
    mov ax, 1           ; Error
.done:
    mov [bp - 16], ax   ; Store return value
    popa
    mov ax, [bp - 16]   ; Restore return value
    pop bp
    ret

; Enable A20 line
enable_a20:
    pusha
    
    ; Method 1: Fast A20
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ; Method 2: Keyboard controller (backup)
    call a20_wait
    mov al, 0xAD
    out 0x64, al
    
    call a20_wait
    mov al, 0xD0
    out 0x64, al
    
    call a20_wait2
    in al, 0x60
    push ax
    
    call a20_wait
    mov al, 0xD1
    out 0x64, al
    
    call a20_wait
    pop ax
    or al, 2
    out 0x60, al
    
    call a20_wait
    mov al, 0xAE
    out 0x64, al
    
    call a20_wait
    popa
    ret

a20_wait:
    in al, 0x64
    test al, 2
    jnz a20_wait
    ret

a20_wait2:
    in al, 0x64
    test al, 1
    jz a20_wait2
    ret

; Switch to protected mode and jump to kernel
switch_to_protected_mode:
    cli                 ; Disable interrupts
    
    ; Load GDT
    lgdt [gdt_descriptor]
    
    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to flush pipeline
    jmp 0x08:protected_mode_entry

[BITS 32]
protected_mode_entry:
    ; Set up data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000    ; Set up stack
    
    ; Jump to kernel at 0x8000
    jmp 0x8000

[BITS 16]
; GDT
gdt_start:
    ; Null descriptor
    dd 0x0, 0x0

gdt_code:
    ; Code segment
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low)
    db 0x00         ; Base (middle)
    db 10011010b    ; Access
    db 11001111b    ; Granularity
    db 0x00         ; Base (high)

gdt_data:
    ; Data segment  
    dw 0xFFFF       ; Limit
    dw 0x0000       ; Base (low)
    db 0x00         ; Base (middle)
    db 10010010b    ; Access
    db 11001111b    ; Granularity
    db 0x00         ; Base (high)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start