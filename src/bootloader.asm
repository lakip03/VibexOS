; Minimal x86_64 Bootloader
; Displays "I'm sorry Dave, I am afraid I can't let you do that"
; Loads at 0x7C00 (MBR standard)

[BITS 16]           ; 16-bit real mode
[ORG 0x7C00]        ; Boot sector loads at 0x7C00

start:
    ; Initialize segment registers
    xor ax, ax      ; Clear AX
    mov ds, ax      ; Set DS to 0
    mov es, ax      ; Set ES to 0
    mov ss, ax      ; Set SS to 0
    mov sp, 0x7C00  ; Set stack pointer below bootloader

    ; Clear screen
    mov ax, 0x0003  ; Set video mode 3 (80x25 text mode)
    int 0x10        ; BIOS video interrupt

    ; Display message
    mov si, message ; Load message address into SI
    call print_string

    ; Infinite loop to prevent processor from executing random memory
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

; Data section
message: db "I'm sorry Dave, I am afraid I can't let you do that", 0x0D, 0x0A, 0

; Fill remaining space with zeros to reach 510 bytes
; This leaves room for future expansion
times 510-($-$$) db 0

; Boot signature (must be at bytes 510-511)
dw 0xAA55