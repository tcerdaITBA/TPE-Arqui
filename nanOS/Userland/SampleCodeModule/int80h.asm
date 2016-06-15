
GLOBAL _int80h

section .text

_int80h:
    push rbp
    mov rbp,rsp
    mov rax,rdi    ; syscall
    mov rbx,rsi
    push rcx
    mov rcx,rdx
	  pop rdx
    int 80h
    mov rsp,rbp
    pop rbp
    ret
