#include<avr/io.h>  // oznake vhodov in izhodov

#include<inttypes.h>
#include<avr/delay.h> //zakasnitvene metode
#include<avr/interrupt.h> //prekinitveni vektorji

#include "lcd.c"


#define F_CPU 1000000UL  //oscilator 1MHz

void Convert(int bin, char *str);

int main(void){ //glavna metoda
lcd_init();
lcd_on();
lcd_clear();
//lcd_set_cursor(1,1);

unsigned char word_1=0b11010;
char g[1];
Convert(word_1, g);
//lcd_set_right_to_left();


while(1){ // neskonèna zanka
lcd_set_cursor(10,0);
lcd_puts("I HOPE                        Ireally");
//lcd_set_cursor(0,2);
lcd_puts(g);
_delay_ms(3000);

}
}

void Convert(int bin, char *str)
{
    unsigned int mask;      // used to check each individual bit, 
                            //    unsigned to alleviate sign 
                            //    extension problems

    mask = 0b10000000;      // Set only the high-end bit
    while (mask)            // Loop until MASK is empty
    {
        if (bin & mask)     // test the masked bit
              *str = '1';   // if true, value is 1
          else 
              *str = '0';   // if false, value is 0
        str++;              // next character
        mask >>= 1;         // shift the mask 1 bit
    }
    *str = 0;               // add the trailing null 
}
