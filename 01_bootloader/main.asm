; main.asm

[org 0x7c00]

; init stack
mov bp, 0x800
mov sp, bp

mov bx, boot_msg
call print_string
call print_newline
mov bx, hex_msg
call print_string
call print_newline
mov dx, 0xa91f
call print_hex



jmp $


%include "printer.asm"

boot_msg:
	db "Booting loader", 0

hex_msg:
    db "printing a hex message", 0



times   510 - ($-$$) db 0

dw 0xaa55

