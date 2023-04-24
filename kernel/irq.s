bits 32
global rtc_isr, sys_call_isr, serial_isr

; RTC interrupt handler
; Tells the slave PIC to ignore interrupts from the RTC
rtc_isr:
	cli
	push ax
	
	; Tell the PIC this is EOI
	; This really should be done
	; at the RTC level -- but this is
	; okay for now...
	mov al, 0x20
	out 0xA0, al
	
	pop ax
	sti
	iret

;;; System call interrupt handler. To be implemented in Module R3.
extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
    push eax
    push ecx
    push edx
    push ebx
    push ebp
    push esi
    push edi
	push ss
	push ds
	push es
	push fs
	push gs
	push esp
	call sys_call
	mov esp, eax
	pop gs
	pop fs
	pop es
	pop ds
	pop ss
	pop edi
	pop esi
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax
	iret

extern serial_interrupt
;;; Serial port ISR. To be implemented in Module R6
serial_isr:
    cli
    call serial_interrupt
	sti
	iret

