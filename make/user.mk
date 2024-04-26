.POSIX:

user/core.o: user/core.c include/string.h include/mpx/serial.h \
  include/mpx/device.h include/processes.h include/sys_req.h
user/com_hand.o: user/com_hand.c include/com_hand.h include/mpx/serial.h \
  include/sys_req.h include/string.h include/stdlib.h \
  include/time_date.h include/PCB.h include/queue.h include/sys_call.h \
  include/processes.h include/mem-mgmt.h
user/time_date.o: user/time_date.c include/mpx/io.h include/mpx/serial.h \
  include/sys_req.h include/time_date.h include/mpx/interrupts.h include/memory.h \
  include/string.h

user/alarm.o: user/alarm.c include/alarm.h include/string.h include/sys_req.h \
  include/memory.h

USER_OBJECTS=\
	user/core.o\
	user/com_hand.o\
	user/time_date.o\
  user/alarm.o
