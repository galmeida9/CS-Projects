segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
;; before body
	push	dword 2
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	pop	eax
	add	dword [esp], eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
segment	.rodata
align	4
_L1:
	db	"string", 0
segment	.text
	push	dword $_L1
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 4
	pop	eax
	add	dword [esp], eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword 3
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 8
	pop	eax
	add	dword [esp], eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	leave
	ret
;; after body
segment	.text
align	4
g:
	push	ebp
	mov	ebp, esp
	sub	esp, 8
;; before body
	call	readd
	sub	esp, byte 8
	fstp	qword [esp]
	fld	qword [esp]
	add	esp, byte 8
	leave
	ret
;; after body
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
;; before body
	call	g
	add	esp, -8
	sub	esp, byte 8
	fstp	qword [esp]
	call	printd
	add	esp, 8
	push	dword 0
	pop	eax
	add	esp, 8
	leave
	ret
;; after body
extern	argc
extern	argv
extern	envp
extern	printd
extern	readd
