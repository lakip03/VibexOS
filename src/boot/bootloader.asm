; Minimal x86_64 Bootloader
; Displays "I'm sorry Dave, I am afraid I can't let you do that"
; Loads at 0x7C00 (MBR standard)

[BITS 16]           ; 16-bit real mode
[ORG 0x7C00]        ; Boot sector loads at 0x7C00

start:
    ; Save boot drive number (DL register contains it)
    mov [boot_drive], dl
    
    ; Initialize segment registers
    xor ax, ax      ; Clear AX
    mov ds, ax      ; Set DS to 0
    mov es, ax      ; Set ES to 0
    mov ss, ax      ; Set SS to 0
    mov sp, 0x7C00  ; Set stack pointer below bootloader

    ; Clear screen
    mov ax, 0x0003  ; Set video mode 3 (80x25 text mode)
    int 0x10        ; BIOS video interrupt

    ; Reset disk system
    mov ah, 0x00    ; Reset disk function
    mov dl, [boot_drive]  ; Use saved boot drive
    int 0x13        ; BIOS disk interrupt
    jc disk_error   ; Jump if reset failed

    ; Load kernel from disk (sector 2 onwards)
    mov ah, 0x02    ; BIOS read sectors function
    mov al, 1       ; Number of sectors to read (kernel is small)
    mov ch, 0       ; Cylinder number
    mov cl, 2       ; Starting sector (sector 2, after bootloader)
    mov dh, 0       ; Head number
    mov dl, [boot_drive]  ; Use saved boot drive
    mov bx, 0x8000  ; Buffer address to load kernel
    int 0x13        ; BIOS disk interrupt
    
    jc disk_error   ; Jump if carry flag set (error)
    
    ; Verify we read the correct number of sectors
    cmp al, 1       ; Check if 1 sector was read
    jne disk_error  ; Jump if not all sectors read
    
    ; Switch to protected mode before jumping to kernel
    cli                 ; Disable interrupts
    lgdt [gdt_descriptor] ; Load GDT
    
    ; Enable protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to flush pipeline and enter protected mode
    jmp 0x08:protected_mode

disk_error:
    ; Display error message
    mov si, error_message
    call print_string
    
    ; Infinite loop
hang:
    hlt             ; Halt the processor
    jmp hang        ; Jump back to hang if interrupted

; Print string function
; Input: SI = pointer to null-terminated string
print_string:
    pusha           ; Save all general-purpose registers
    mov ah, 0x0E    ; BIOS teletype output function

.next_char:
    lodsb           ; Load byte from [SI] into AL, increment SI
    test al, al     ; Test if AL is 0 (end of string)
    jz .done        ; Jump if zero (end of string)
    int 0x10        ; BIOS video interrupt to print character
    jmp .next_char  ; Continue with next character

.done:
    popa            ; Restore all general-purpose registers
    ret             ; Return to caller

[BITS 32]
protected_mode:
    ; Set up data segments for protected mode
    mov ax, 0x10        ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000    ; Set up stack
    
    ; Jump to kernel
    jmp 0x8000

[BITS 16]
; GDT (Global Descriptor Table)
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 10011010b    ; Access byte
    db 11001111b    ; Granularity
    db 0x00         ; Base (bits 24-31)

    ; Data segment descriptor
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 10010010b    ; Access byte
    db 11001111b    ; Granularity
    db 0x00         ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size
    dd gdt_start                ; Offset

; Data section
boot_drive: db 0        ; Storage for boot drive number
error_message: db "Disk read error!", 0x0D, 0x0A, 0

; Fill remaining space with zeros to reach 510 bytes
; This leaves room for future expansion
times 510-($-$$) db 0

; Boot signature (must be at bytes 510-511)
dw 0xAA55