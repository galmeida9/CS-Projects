; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 4
	push	ebp
	mov	ebp, esp
	sub	esp, 4
;; before body
; INT 100
	push	dword 100
; INT 100
	push	dword 100
; DIV
	pop	ecx
	pop	eax
	cdq
	idiv	ecx
	push	eax
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; INT 0
	push	dword 0
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
;; after body
; EXTERN argc
extern	argc
; EXTERN argv
extern	argv
; EXTERN envp
extern	envp
; EXTERN printi
extern	printi
