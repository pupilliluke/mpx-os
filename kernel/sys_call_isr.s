bits 32
global sys_call_isr

;;; System call interrupt handler. To be implemented in Module R3.
extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
	mov eax, -1		;; DELETE THIS LINE WHEN YOU IMPLEMENT R3
	iret
