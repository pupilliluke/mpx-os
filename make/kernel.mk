.POSIX:

kernel/serial.o: kernel/serial.c include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h

kernel/kmain.o: kernel/kmain.c include/mpx/gdt.h include/mpx/interrupts.h \
  include/mpx/serial.h include/mpx/device.h include/mpx/vm.h \
  include/sys_req.h include/string.h include/memory.h

kernel/core-c.o: kernel/core-c.c include/mpx/gdt.h include/mpx/panic.h \
  include/mpx/interrupts.h include/mpx/io.h include/mpx/serial.h \
  include/mpx/device.h include/sys_req.h include/string.h \
  include/mpx/vm.h

kernel/PCB.o: kernel/PCB.c include/stdlib.h include/memory.h include/PCB.h \
  include/string.h include/queue.h

kernel/queue.o: kernel/queue.c include/stdlib.h include/memory.h

kernel/R4.o: kernel/R4.c include/memory.h include/string.h include/PCB.h \
	include/sys_call.h include/processes.h include/com_hand.h include/R4.h \
	include/queue.h include/sys_req.h include/stdlib.h include/alarm.h 

KERNEL_OBJECTS=\
	kernel/core-asm.o\
	kernel/sys_call_isr.o\
	kernel/sys_call.o\
	kernel/serial.o\
	kernel/kmain.o\
	kernel/core-c.o\
  	kernel/PCB.o\
  	kernel/queue.o\
	kernel/R4.o\
	kernel/mem-mgmt.o
