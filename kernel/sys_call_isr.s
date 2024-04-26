bits 32
global sys_call_isr

;;; System call interrupt handler. To be implemented in Module R3.
extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
; Push general-purpose registers in the reverse order
    push EBP
    push EDI
    push ESI
    push EDX
    push ECX
    push EBX
    push EAX
    push ESP

; Push segment registers (DS, ES, FS, GS) in the reverse order
    push SS
    push GS
    push FS
    push ES
    push DS

; Push the current ESP value, stack pointer
    push ESP
    call sys_call
    mov ESP, EAX

; Pop segments
    pop DS
    pop ES
    pop FS
    pop GS
    pop SS

; Pop registers
    add ESP, 4
    pop EAX
    pop EBX
    pop ECX
    pop EDX
    pop ESI
    pop EDI
    pop EBP

; Return from ISR
    iret
