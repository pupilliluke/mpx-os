#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int serial_poll(device dev, char *buffer, size_t len)
{
	//total count of characters
	int count =0;
	//curser location
	int cursor_loc = 0;
	//while the buffer is not full
	while(count != (int)len){
		//check LSR byte for incoming data
		char LSRbyte = inb(COM1 + LSR);
		if (LSRbyte & 1){
			char key = inb(COM1);
			//if return is key
			if(key == '\r'){
				//return key
				outb(dev, '\r');
				//newline
				outb(dev, '\n');
				buffer[count] = 0;
				//send buffer to com_hand 
				break;
			//if backspace is key
			}else if(key == '\x7F'){
				//curser must be at location 0 or greater
				if(cursor_loc > 0){
					//backspace
					outb(dev, '\b');
					cursor_loc --;
					//clear the character at curser_loc by setting it to a space
					outb(dev, ' ');
					//backspace the newly created space to clear the character
					outb(dev, '\b');
					count --;
					//clear the buffer at space count
					buffer[count] = 0;
				}
				//check of intered character is between A-Z, a-z, and 0-9
			}else if((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >='0' && key <='9')){
				buffer[cursor_loc] = key;
				cursor_loc ++;
				outb(dev, key);
				count ++;
			//if key is space
			}else if(key == ' '){
				buffer[cursor_loc] = key;
				cursor_loc ++;
				outb(dev, key);
				count++;
			//Left arrow key
			}else if(key == '\x5B'){
				char c = inb(COM1);
				if(c == 'D'){
					if(cursor_loc > 0){
						cursor_loc --;
						outb(dev, '\b');
						buffer[count] = 0;
					}
				}else if(c == 'C'){
					if(cursor_loc <= count){
						outb(dev, ' ');
						cursor_loc ++;
					}
				}
			}

			
		}
	}
	return count;
}
