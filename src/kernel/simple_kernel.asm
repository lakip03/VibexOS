[BITS 16]
[ORG 0x8000]

start:
    ; Setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    ; Clear screen
    mov ax, 0x0003
    int 0x10
    
    ; Print messages directly using BIOS
    mov si, welcome_msg
    call print_string
    
    mov si, version_msg
    call print_string
    
    mov si, char_test_msg
    call print_string
    
    mov si, string_test_msg
    call print_string
    
    mov si, hex_test_msg
    call print_string
    
    mov si, complete_msg
    call print_string
    
    ; Proper halt
    cli
    hlt

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

; Messages
welcome_msg: db "Welcome to VibexOS version 1", 0x0D, 0x0A, 0
version_msg: db "16-bit kernel running!", 0x0D, 0x0A, 0
char_test_msg: db "Character test: A", 0x0D, 0x0A, 0
string_test_msg: db "String test: Hello World", 0x0D, 0x0A, 0
hex_test_msg: db "Hex test: 0xDEAD", 0x0D, 0x0A, 0
complete_msg: db "All tests complete! System halted.", 0x0D, 0x0A, 0

times 512-($-$$) db 0