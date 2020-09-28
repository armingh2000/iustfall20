; printer.asm


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


