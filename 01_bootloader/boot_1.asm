; boot 1 example

loop:
	jmp loop


times 510 - ($ - $$) db 0


dw 0xaa55