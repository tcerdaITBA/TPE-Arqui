
GLOBAL write

section .text

write:
	push rbp
	mov rbp,rsp
	mov rax,4    ; syscall write
	mov rbx,rdi  ; file descriptor
	mov rcx,rsi  ; puntero a buffer
		    ; en rdx ya se encuentra la cantidad de bytes a escribir
	int 80h
	mov rsp,rbp
	pop rbp
	ret
