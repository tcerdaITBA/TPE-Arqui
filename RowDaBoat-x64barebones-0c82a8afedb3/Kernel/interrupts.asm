
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL int_08_hand
GLOBAL _lidt
GLOBAL haltcpu

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler


EXTERN irqDispatcher

%macro irqHandlerMaster 1
	push ds
	push es
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi

	mov rax, %1
	mov rdi, rax ; pasaje de parametro
	call irqDispatcher
	
	;signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop es
	pop ds
	iret
%endmacro



SECTION .text

_cli:
	cli
	ret


_sti:
	sti
	ret


picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, [ss:rbp+8]
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, [ss:rbp+8]  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

_lidt:				; Carga el IDTR
    push    rbp
    mov     rbp, rsp
    push    rbx
    mov     rbx, [ss: rbp + 10] ; ds:bx = puntero a IDTR
	rol	rbx,16
	lidt    [ds: rbx]          ; carga IDTR
    pop     rbx
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0
	
;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3
	
;Serial Port 1 and 3	
_irq04Handler:
	irqHandlerMaster 4
	
;USB
_irq05Handler:
	irqHandlerMaster 5

haltcpu:
	cli
	hlt
	ret
