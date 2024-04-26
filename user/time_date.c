#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <time_date.h>
#include <mpx/interrupts.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
//use inb and any of the regesters from the intel paper
//Index | Standard Bank
//0x00 | Seconds
//0x01 | Seconds – Alarm
//0x02 | Minutes
//0x03 | Minutes – Alarm
//0x04 | Hours
//0x05 | Hours – Alarm
//0x06 | Day of Week
//0x07 | Day of Month
//0x08 | Month
//0x09 | Year

//need a bcd to int fuction
//get time will convert 

/**
* @ breaf the converts the binary into a decamal.
*/
int BCDtoDec(int BCD) {
	return ((BCD >> 4) * 10) + ( BCD & 0x0f);
}

//reverses BCDtoDec, %10 first 4, /10 next 4
int DecToBCD(int Dec)
{
	return (((Dec / 10) % 10) << 4) | (Dec % 10);
}


int getSeconds(void) {
	outb(0x70, 0x00);
	int s = inb(0x71);
	return s;
}

/**
* @discription grabs the minutes in binary form
*/
//0x02
int getMinutes(void) {
	outb(0x70, 0x02);
	int m = inb(0x71);
return m;
}

/**
* @discription grabs the hours in binary form
*/
int getHours(void) {
	outb(0x70, 0x04);
	int h = inb(0x71);
return h;
}

int getDay(void){
	outb(0x70, 0x07);
	unsigned char d = inb(0x71);
	return d;
}

int getMonth(void){
	outb(0x70, 0x08);
	unsigned char m = inb(0x71);
	return m;
}

int getYear(void){
	outb(0x70, 0x09);
	unsigned char y = inb(0x71);
	return y;
}



void getTime(void){
	 int seconds = BCDtoDec(getSeconds());
	 int minutes = BCDtoDec(getMinutes());
	 int hours = BCDtoDec(getHours());
	 char secStr[4];
     char minStr[4];
     char hourStr[4];

	
	 itoa(hours, hourStr);
	 itoa(seconds, secStr);
     itoa(minutes, minStr);

	 sys_req(WRITE, COM1, hourStr, 3);
	 sys_req(WRITE, COM1, ":", 2);
	 sys_req(WRITE, COM1, minStr, 4);
	 sys_req(WRITE, COM1, ":", 2);
	 sys_req(WRITE, COM1, secStr, 4);
     sys_req(WRITE, COM1, "\n", 2);


}

void getDate(void){
	 int day = BCDtoDec(getDay());
	 int month = BCDtoDec(getMonth());
	 int year = BCDtoDec(getYear());
    char dayStr[4];
    char monthStr[4];
    char yearStr[4];

    itoa(day, dayStr);
    itoa(month, monthStr);
    itoa(year, yearStr);

	 sys_req(WRITE, COM1, monthStr, 4);
	 sys_req(WRITE, COM1, "/", 2);
	 sys_req(WRITE, COM1, dayStr, 4);
	 sys_req(WRITE, COM1, "/", 2);
     sys_req(WRITE, COM1, "20", 3);
	 sys_req(WRITE, COM1, yearStr, 4);
     sys_req(WRITE, COM1, "\n", 2);
}

void setTime(int sec, int min, int hr){
	cli();

//dec to bcd ->
	int s = DecToBCD(sec);
	int m = DecToBCD(min);
	int h = DecToBCD(hr);

	outb(0x70, 0x00);
	outb(0x71, s);

	outb(0x70, 0x02);
	outb(0x71, m);

	outb(0x70, 0x04);
	outb(0x71, h);

	sti();
}

void setDate(int day, int month, int year){
	cli();

    int d = DecToBCD(day);
    int m = DecToBCD(month);
    int y = DecToBCD(year);

	outb(0x70, 0x07);
	outb(0x71, d);

	outb(0x70, 0x08);
	outb(0x71, m);

	outb(0x70, 0x09);
	outb(0x71, y);

	sti();
}
