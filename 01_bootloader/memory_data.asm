; Hello world!

[org 0x7c00]
mov ah, 0x0e

mov al, x_char
int 0x10

mov al, [x_char]
int 0x10

mov bx, x_char
add bx, 0x7c00
mov al, [bx]
int 0x10

mov al, [0x7c1d]
int 0x10



jmp $


x_char: 
	db "X"

times 510 - ($ - $$) db 0

dw 0xaa55