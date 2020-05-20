
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _syscallHandler

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN systemCall

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

;se llenan
;%rdi, %rsi, %rdx, %rcx, %r8 y %r9 
;necesito
;%rax	%rdi	%rsi	%rdx	%r10	%r8	   %r9
;syscalls 
_syscallHandler:
	push    rbp
    mov     rbp, rsp

	;mov [regsiters],rax
	;mov [regsiters+8],rdi
	;mov [regsiters+16],rsi
	;mov [regsiters+24],rdx
	;mov [regsiters+32],r10
	;mov [regsiters+40],r8
	;mov [regsiters+48],r9
	;mov rdi,registers

	push rcx
	mov rcx,rax
	push r10
	
	call systemCall

	pop r10
	pop rcx
	
	leave
	ret

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


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

haltcpu:
	cli
	hlt
	ret


SECTION .bss
	aux resq 1

	registers:
		rrax resq 1
		rrdi resq 1
		rrsi resq 1
		rrdx resq 1
		rr10 resq 1
		rr8 resq 1
		rr9 resq 1
	