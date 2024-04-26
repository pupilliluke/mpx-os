bits 32
global serial_isr

;;; R6 asm code for calling c function (interupt handling)
extern serial_interrupt			; The C function that serial_isr.s will call
serial_isr:
;call c function that we want 
call serial_interrupt



; Return from ISR
    iret