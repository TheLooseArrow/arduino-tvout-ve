/*
TVPrint.cpp - Base class that provides print() and println()
Copyright (c) 2008 David A. Mellis.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Modified 23 November 2006 by David A. Mellis
*/

#include <math.h>
#include <avr/pgmspace.h>

#include "TVout.h"

#define CC_END_OF_START_BIT 78

void TVout::select_font(const unsigned char * f) {
	font = f;
}

/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void TVout::print_char(uint8_t x, uint8_t y, unsigned char c) {

	c -= pgm_read_byte(font+2);
	bitmap(x,y,font,(c*pgm_read_byte(font+1))+3,pgm_read_byte(font),pgm_read_byte(font+1));
}

void TVout::inc_txtline() {
	if (cursor_y >= (display.vres - pgm_read_byte(font+1)))
		shift(pgm_read_byte(font+1),UP);
	else
		cursor_y += pgm_read_byte(font+1);
}

/* default implementation: may be overridden */
void TVout::write(const char *str)
{
  while (*str)
    write(*str++);
}

/* default implementation: may be overridden */
void TVout::write(const uint8_t *buffer, uint8_t size)
{
  while (size--)
    write(*buffer++);
}

void TVout::write(uint8_t c) {
	switch(c) {
		case '\0':			//null
			break;
		case '\n':			//line feed
			cursor_x = 0;
			inc_txtline();
			break;
		case 8:				//backspace
			cursor_x -= pgm_read_byte(font);
			print_char(cursor_x,cursor_y,' ');
			break;
		case 13:			//carriage return !?!?!?!VT!?!??!?!
			cursor_x = 0;
			break;
		case 14:			//form feed new page(clear screen)
			//clear_screen();
			break;
		default:
			if (cursor_x >= (display.hres*8 - pgm_read_byte(font))) {
				cursor_x = 0;
				inc_txtline();
				print_char(cursor_x,cursor_y,c);
			}
			else
				print_char(cursor_x,cursor_y,c);
			cursor_x += pgm_read_byte(font);
	}
}

void TVout::print(const char str[])
{
  write(str);
}

void TVout::print(char c, int base)
{
  print((long) c, base);
}

void TVout::print(unsigned char b, int base)
{
  print((unsigned long) b, base);
}

void TVout::print(int n, int base)
{
  print((long) n, base);
}

void TVout::print(unsigned int n, int base)
{
  print((unsigned long) n, base);
}

void TVout::print(long n, int base)
{
  if (base == 0) {
    write(n);
  } else if (base == 10) {
    if (n < 0) {
      print('-');
      n = -n;
    }
    printNumber(n, 10);
  } else {
    printNumber(n, base);
  }
}

void TVout::print(unsigned long n, int base)
{
  if (base == 0) write(n);
  else printNumber(n, base);
}

void TVout::print(double n, int digits)
{
  printFloat(n, digits);
}

void TVout::println(void)
{
  print('\r');
  print('\n');  
}

void TVout::println(const char c[])
{
  print(c);
  println();
}

void TVout::println(char c, int base)
{
  print(c, base);
  println();
}

void TVout::println(unsigned char b, int base)
{
  print(b, base);
  println();
}

void TVout::println(int n, int base)
{
  print(n, base);
  println();
}

void TVout::println(unsigned int n, int base)
{
  print(n, base);
  println();
}

void TVout::println(long n, int base)
{
  print(n, base);
  println();
}

void TVout::println(unsigned long n, int base)
{
  print(n, base);
  println();
}

void TVout::println(double n, int digits)
{
  print(n, digits);
  println();
}

void TVout::printPGM(const char str[]) {
	char c;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}

void TVout::printPGM(uint8_t x, uint8_t y, const char str[]) {
	char c;
	cursor_x = x;
	cursor_y = y;
	while ((c = pgm_read_byte(str))) {
		str++;
		write(c);
	}
}

void TVout::set_cursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void TVout::print(uint8_t x, uint8_t y, const char str[]) {
	cursor_x = x;
	cursor_y = y;
	write(str);
	
}
void TVout::print(uint8_t x, uint8_t y, char c, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) c, base);
}
void TVout::print(uint8_t x, uint8_t y, unsigned char b, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) b, base);
}
void TVout::print(uint8_t x, uint8_t y, int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((long) n, base);
}
void TVout::print(uint8_t x, uint8_t y, unsigned int n, int base) {
	cursor_x = x;
	cursor_y = y;
	print((unsigned long) n, base);
}
void TVout::print(uint8_t x, uint8_t y, long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void TVout::print(uint8_t x, uint8_t y, unsigned long n, int base) {
	cursor_x = x;
	cursor_y = y;
	print(n,base);
}
void TVout::print(uint8_t x, uint8_t y, double n, int digits) {
	cursor_x = x;
	cursor_y = y;
	print(n,digits);
}

void TVout::println(uint8_t x, uint8_t y, const char c[])
{
	cursor_x = x;
	cursor_y = y;
	print(c);
	println();
}

void TVout::println(uint8_t x, uint8_t y, char c, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(c, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, unsigned char b, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(b, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, int n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, unsigned int n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, long n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, unsigned long n, int base)
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
	println();
}

void TVout::println(uint8_t x, uint8_t y, double n, int digits)
{
	cursor_x = x;
	cursor_y = y;
	print(n, digits);
	println();
}

void TVout::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

void TVout::printFloat(double number, uint8_t digits) 
{ 
  // Handle negative numbers
  if (number < 0.0)
  {
     print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint; 
  } 
}

void TVout::printCC(char firstChar, char secondChar)
{
  uint8_t pointer = CC_END_OF_START_BIT;//The end of the third start bit
  
  pointer = ccPixelGen(firstChar, pointer);
  ccPixelGen(secondChar, pointer);
  
  cc_enable();
  
  while(!cc_is_finished())
  {
	 //Need this nop or else it gets stuck in this loop
	 //Dunno why. Probably "optimizing" the loop
	 __asm__("nop\n\t");
  }
  
  //Again dont know why but I need to 
  //Send the caption line on 2 frames
  cc_enable();
  
  while(!cc_is_finished())
  {
	 //Need this nop or else it gets stuck in this loop
	 //Dunno why. Probably "optimizing" the loop
	 __asm__("nop\n\t");
  }
 
}

uint8_t TVout::ccPixelGen(char character, uint8_t pointer)
{
  uint8_t parity = 0;
  
  //This mess below turns each bit of the "character" byte
  //into 8 bits each and stores them in the ccLineBuffer just after the run-in 
  //clock and 3 start bits that are hard coded into the buffer. Each bit in 
  //this buffer will eventually be rendered into a pixel in the VBI on line 21 

  //Each pixel is asserted for approx 250ns so 8 pixels is approx 2us
  //and 2 us is how wide the CC decoder in the TV expects each bit to be
  
  
  //this loop iterates over the first 7 bits of the byte
  for(uint8_t i = 0; i < 7; i++)
  {
    //each bit on the waveform requires ~2us
    //Putting a line of 8 pixels gives us ~2us
    for(uint8_t j = 0; j < 8; j++)
    {
      //if the "i"th bit is a 1
      if(((character >> i)&(0x01)) == 1)
      {
        ccLineBuffer[(pointer/8)] |= 0x80 >> (pointer&7);
      }
      else 
      {
        ccLineBuffer[(pointer/8)] &= ~0x80 >> (pointer&7);
      }  
      pointer++;
    }
	
    //Calculate the parity
    parity += ((character >> i)&(0x01));
  }
  
  //place the parity bit
  for(uint8_t i=0; i < 8; i++)
  {
    
    //Is the parity even 
    if(!(parity%2))
    {
      //if its even assert the odd parity bit
      ccLineBuffer[(pointer/8)] |= 0x80 >> (pointer&7);
    }
    else 
    {
      ccLineBuffer[(pointer/8)] &= ~0x80 >> (pointer&7);
    }
	
    pointer++;
  }
  
  return pointer;
  
}

void TVout::ccOverlayMode()
{
  cc_overlay_mode();
}

void TVout::ccTVOutMode()
{
  cc_tvout_mode();
}
