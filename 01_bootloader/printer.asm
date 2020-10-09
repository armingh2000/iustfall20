; printer.asm

n_table:
    db "0123456789abcdef", 0

print_hex:

    mov ah, 0x0e
    mov al, '0'
    int 0x10

    mov al, 'x'
    int 0x10
    
    mov ax, dx
    mov bh, ah
    mov ah, 0x0e
    and bh, 0xf0
    shr bh, 4
    movsx bx, bh
    add bx, n_table
    mov al, [bx]
    int 0x10

    mov ax, dx
    mov bh, ah
    mov ah, 0x0e
    and bh, 0x0f
    movsx bx, bh
    add bx, n_table
    mov al, [bx]
    int 0x10

    mov ax, dx
    mov bh, al
    mov ah, 0x0e
    and bh, 0xf0
    shr bh, 4
    movsx bx, bh
    add bx, n_table
    mov al, [bx]
    int 0x10

    mov ax, dx
    mov bh, al
    mov ah, 0x0e
    and bh, 0x0f
    movsx bx, bh
    add bx, n_table
    mov al, [bx]
    int 0x10


    ret



print_newline:

    mov ah, 0x0e
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10
    ret

print_string:

    mov ah, 0x0e
    start1:
        mov al, [bx]
        cmp al, 0x0
        je end1
        int 0x10
        inc bx
        jmp start1

    end1:
        ret


