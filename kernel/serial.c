#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <R6.h>
#include <mpx/interrupts.h>
#include <memory.h>

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

 

dcb* newDcb;
IOQueue* IOQ;

static int initialized[4] = { 0 };
extern void serial_isr(void*);

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

/*• Initialize the serial port associated with the dev parameter
• Initialize the associated DCB
• Install the appropriate ISR
• Set the port speed to the speed parameter
• Set other line characteristics (8 data bits, no parity, 1 stop bit)
• Enable all necessary interrupts
• Return an integer indicating success or failure*/
int serial_open(device dev, int speed){
	//1. Ensure that the parameters are valid, and that the device is not currently open.
	if(dev != COM1){
		return -1;
	}

	//check if baud rate is valid
	if(!(speed == 110 || speed == 300 || speed == 600 || speed == 1200 || speed == 2400 || speed == 4800 || speed == 9600 || speed == 19200 || speed == 38400 || speed == 57600 || speed == 115200)){
		return 102;
	}

	//1. initialize serial port
	if(serial_init(dev) == -1){
		return -1;
	}	

	//2. initialize DCB
	newDcb = (dcb*)sys_alloc_mem(sizeof(dcb));
	IOQ = (IOQueue*)sys_alloc_mem(sizeof(IOQueue));
	if(newDcb == NULL){
		return -1;
	}

	//Initialize the DCB attributes
	newDcb->state = OPEN;      //0
	newDcb->status = status_IDLE;     //2
	newDcb->flag = INCOMPLETE; //0
	newDcb->ringin = 0;
	newDcb->ringout = 0;
	newDcb->ringcount = 0;
	newDcb->current_location = 0;
	newDcb->characters_transfered = 0;	

// 3. Install the new handler in the interrupt vector.
	idt_install(0x24, serial_isr);	//send the interrupt for COM1 to the serial_isr function
// 									//NATE EXPLANTION: COM1 interrupt sends a signal to the serial_isr function, 
// 									//   then serial isr function just calls serial_interrupt function below

// 4. Set the port speed to the speed parameter
	int baud_rate_div = 115200 / (long) speed;  //param speed should be 19,200
												//div should be 6

// 5.	store the value 0x80 in the Line Control Register. This allows the first two port addresses to access
// the Baud Rate Divisor register.
	// LCR = 3,	// Line Control
	outb(dev + LCR, 0x80);	//set line control register

/*6. Store the high order and low order bytes of the baud rate divisor into the MSB and LSB registers,
respectively.
*/
/*
    outb(dev + LCR, 0x03);  // 8 bits, no parity, one stop bit
*/
	outb(dev + DLL,baud_rate_div & 0xFF);	//set bsd least sig bit		//THESE 2 ARE NOT RIGHT/CONFIRMED. NEED TO FIND LOW/HIGH ORDER IN msb AND lsb TO SET TO DLL & DLM
	outb(dev + DLM, (baud_rate_div >> 4) & 0xFF);	//brd 		most significant bit

/*
7. Store the value 0x03 in the Line Control Register. This sets the line characteristics to 8 data bits, 1
stop bit, and no parity. It also restores normal functioning of the first two ports.
*/
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop

/*
8. Enable the appropriate level in the PIC mask register.
CURRENTLY NEED HELP WITH THIS
*/
	cli(); //first disable interrupts

// 	This code disables the interrupt associated with bit 8. Similar steps will be used
// to disable other interrupts.
	int mask = inb(0x21); // Get the current interrupt mask
	mask &= ~0x10; // Set the bit for IRQ
	outb (0x21 , mask);
	sti();	//re-enable interrupts

/*
9. Enable overall serial port interrupts by storing the value 0x08 in the Modem Control register
*/
	outb(dev + MCR, 0x08);	//enable interrupt

/*
10. Enable input ready interrupts only by storing the value 0x01 in the Interrupt Enable register.
*/
	outb(dev + IER, 0x01);	//enable interrupts

	return 0;
}




/*• End a serial port session
• Clear open indicator of associated DCB
• Disable associated interrupt in PIC mask
• Return an integer indicating success or failure*/
int serial_close(device dev){
	//checks if the device is there and exists
	int dno = serial_devno(dev);
	if(dno == -1){
		return -201;
	}

	//setting the DCB status
	newDcb->state = CLOSED;

	cli();
//1 left shifted 4
	int mask = inb(0x21); //reverse open 
	mask |= 0x10; // Set the bit for IRQ     // Clearing the 0th bit to enable IRQ0
	outb(0x21, mask); 

	sti();

	outb(dev + MSR, 0x00); //modem register
    outb(dev + IER, 0x00); //interrupt enable



	return 0;
}


 
// • Begin reading input from the serial device dev
// • Reads data into the buffer at buf
// • Reads no more than len bytes
// • If there are characters in the ring buffer, read until buf is full or no more
// characters in the ring buffer
// • If buf is full, return
// • Otherwise, change device states to reading and notify ISR to place
// characters in the the associated buffer instead of the ring buffer
int serial_read(device dev, char *buf, size_t len){
	//validate parameters
	if(dev != COM1){
		return -1;
	} 
	else if(buf == NULL){
		return -302;

	}else if(len < 1){
		return -1;

	}
	if(newDcb->state != OPEN){
		//port closed
		return -301;
	}
	if(newDcb->status != status_IDLE){
		//device busy
		return -304;
	}

	//initialize input buffer variables
	newDcb->input_buffer = buf;
	newDcb->buffer_size = len;
	newDcb->current_location = 0;

	//status to reading
	newDcb->status = status_READ;

	//clear the callers event flag
	newDcb->flag = INCOMPLETE;

	//disable interrupts
	cli();
	// Copy from ring buffer to provided buffer
	while (newDcb->ringcount > 0 && len > 0) {
		char c = newDcb->ring_buffer[newDcb->ringout];
		if (c == '\n') { // Stop if newline character is found
			break;
		}
		buf[newDcb->current_location] = c;
		newDcb->ringout = (newDcb->ringout + 1) % newDcb->buffer_size;
		newDcb->ringcount--;
		newDcb->current_location++;
		len--;
	}
	//reenable interrupts
	sti();

	// If more characters are needed, return
	if (len > 0) {
		return 0;
	}

	newDcb->status = status_IDLE;
	newDcb->flag = COMPLETE;

	return newDcb->current_location;
}

int serial_write(device dev, char *buf, size_t len)
{
//validate parameters
	if(dev != COM1){
		return -1;
	} 
	else if(buf == NULL){
		return -402;
	}
	else if(len < 1){ 
		return -403;
	}
	if(newDcb->state != OPEN){
		//port closed
		return -401;
	}
	if(newDcb->status != status_IDLE){
		//device busy
		return -404;
	}

/*.
	THESE STEPS ARE NOT FINISHED
5. Get the first character from the requestor’s buffer and store it in the output register.
6. Enable write interrupts by setting bit 1 of the Interrupt Enable register. This must be done by setting
the register to the logical OR of its previous contents and 0x02.
*/

	
	newDcb->input_buffer = buf;
	newDcb->buffer_size = len;
	newDcb->status = status_WRITE;
	newDcb->flag = INCOMPLETE;
	outb(dev, buf[newDcb->current_location++]); // change 0 to count++ //make sure dev is output register
	outb(COM1+IER, inb(COM1+IER) || 0x02);

	return 0;

}


/*• Disable interrupts
• Obtain the correct DCB
• Read the Interrupt ID Register to determine the exact type of interrupt
• Based on the type (input, output), pass the handling to a second function
• Issue EOI to the PIC
• Reenable interrupts*/

void serial_interrupt(void){
	//diable interrupts
	cli();
	//read in interrupt type, 00 = modem interrput, 01 = output interrupt, 
	//10 = input interrupt, 11 = line status interrupt
	int type = inb(0x00 + IIR);
	//pass the interrupt to a secondary function
	if(type == 1)
	{
		serial_output_interrupt(newDcb);
	}
	else if(type == 2)
	{
		serial_input_interrupt(newDcb);
	}
	//Issue EOI to the PIC
	outb(0x20, 0x20);
	//reenable interrupts
	sti();

}


// • Read the character from the device
// • If the DCB state is reading, store the character in the appropriate IOCB
// buffer
// • If the buffer is now full, or input was new-line, signal completion
// • Otherwise, attempt to store the character in the ring buffer
void serial_input_interrupt(dcb *dcbPtr){
	char errorChar = dcbPtr->input_buffer[sizeof(dcbPtr->input_buffer) - 1];
	if(dcbPtr->status == status_READ)
	{
		//iterate through IOCB list to find correct IOCB, then store the char 
		// in the appropriate IOCB buffer
		IOQ->tail->buffer[IOQ->tail->bufsize++] = errorChar;  //add char to buff at last index
		IOQ->tail->bufsize++;
	}
	if((sizeof(dcbPtr->input_buffer) == dcbPtr->buffer_size)|| errorChar == '\n')
	{
		//change completion flag in the IOCB
		iocb* temp = IOQ->tail;
		IOQ->tail = temp->PrevPtr;
		IOQ->tail->NextPtr = NULL;
		sys_free_mem(temp);
		dcbPtr->flag = COMPLETE;
	}
	else{
	//attempt to store the character in the ring buffer
		dcbPtr->ring_buffer[dcbPtr->ringin] = errorChar;
	}// 	dcb* newDcb = dcbPtr;
}



// • If the DCB state is writing, check for additional characters in the
// appropriate IOCB buffer
// • If there is additional data, write the next character to the device
// • If you have just written the last character from the IOCB, disable write
// interrupts and signal completion
void serial_output_interrupt(dcb *dcbPtr){

	if(dcbPtr->status == status_WRITE)
	{
		if(IOQ->tail->bufsize > 1){
			outb(COM1, IOQ->tail->buffer[IOQ->tail->bufsize]);
			IOQ->tail->bufsize--;
			if(IOQ->tail->bufsize == 0)
			{
				outb(COM1 + IER, (COM1 + IER) || 0x00);
				iocb* temp = IOQ->tail;
				IOQ->tail = temp->PrevPtr;
				IOQ->tail->NextPtr = NULL;
				sys_free_mem(temp);
			}
			
		}
	}
	// dcb* newDcb = dcbPtr;
}

//Return the dcb for COM1
dcb* getDcb(void){
	return newDcb;
}

IOQueue* getIOQ(void)
{
	return IOQ;
}
