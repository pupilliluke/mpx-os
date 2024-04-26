.POSIX:

lib/string.o: lib/string.c include/string.h

lib/stdlib.o: lib/stdlib.c include/stdlib.h include/ctype.h

lib/core.o: lib/core.c include/mpx/serial.h include/mpx/device.h \
  include/mpx/vm.h include/memory.h include/string.h

lib/ctype.o: lib/ctype.c include/ctype.h

LIB_OBJECTS=\
	lib/string.o\
	lib/stdlib.o\
	lib/core.o\
	lib/ctype.o
