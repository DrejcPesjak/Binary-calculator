#include<avr/io.h>  // oznake vhodov in izhodov

#include<inttypes.h>
#include<avr/delay.h> //zakasnitvene metode
#include<avr/interrupt.h> //prekinitveni vektorji

#include "lcd.c"

#define F_CPU 1000000UL  //oscilator 1MHz

void getKeyPress(void);
void keyData(int keyWord);
void Convert(int bin, char *str);
void int2bin(int n, char *s);

int keyRow,columnData,resulte,counter=0,show_res=0;
char word1[1],word2[1], result[1];
unsigned char word_1=0,word_2=0;		//8-bit unsigned integer

int main(void)
{ 
	DDRB = 0x0F;//pins PB0-PB3 outputs and pins PB4-PB7 inputs

	lcd_init();
	lcd_on();
	lcd_clear();		

	while(1)
	{ 
		getKeyPress();
		//_delay_ms(100);
		if(show_res==0)
		{
			lcd_set_cursor(8,0);
			Convert(word_1, word1);
			lcd_puts(word1);		
			lcd_puts("                                ");
			Convert(word_2,word2);
			lcd_puts(word2);
		}					
	}
}

void getKeyPress() // checks what key is being pressed
{ 
    for(keyRow=0;keyRow<4;keyRow++) // scans the switch matrix(4 rows)
	{ 
		PORTB = (1<<keyRow); // sends a HIGH to one row at a time
		columnData = (PINB & 0xF0);// reads the state of each column
		_delay_us(100);// a small delay
		Convert(columnData,word1);		
		if(columnData != 0) // if something is pressed columndata is bigger then zero
		{
			_delay_ms(40);
			PORTB = (1<<keyRow); 
			columnData = (PINB & 0xF0);
		
			if(columnData != 0) 
			{
				keyData(columnData | (1<<keyRow));//goes to the data analysis function
				_delay_us(400);//a small delay like in the LED scanning loop
			}
		}
    }
 }

void keyData(int keyInput) //function for determining what each button does
{ 
   switch (keyInput)
   {
		case 0b10001000:	//input into current row "1"
			if(counter<8)
			{
				lcd_clear();
				word_1 = word_1 << 1;
				word_1 = word_1 | 0x01;
				counter++;
			}
			else if(counter<16)
			{
				lcd_clear();
				word_2 = word_2 << 1;
				word_2 = word_2 | 0x01;
				counter++;
			}
			show_res = 0;
			break;
		case 0b10000100:	//input into current row "0"
			if(counter<8)
			{
				lcd_clear();
				word_1 = word_1 << 1;
				counter++;
			}
			else if(counter<16)
			{
				lcd_clear();
				word_2 = word_2<<1;
				counter++;				
			}
			show_res = 0;						
			break;
		case 0b10000010:	//current row "DEL"
			if(counter<=8)
			{
				lcd_clear();
				word_1 = counter = 0;				
			}
			else if(counter<=16)
			{
				lcd_clear();
				word_2 = 0;
				counter = 8;				
			}
			show_res = 0;					
			break;	
		case 0b01000100://multiplication
			lcd_clear();
			resulte = word_1 * word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);	
			show_res = 1;
			break;
		case 0b01000010://subtraction
			lcd_clear();
			resulte = word_1 - word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);	
			show_res = 1;
			break;
		case 0b01001000://division 
			lcd_clear();
			resulte = word_1 / word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);
			show_res = 1;
			break;
		case 0b01000001://addition
			lcd_clear();
			resulte = word_1 + word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);
			show_res = 1;
			break;										
		case 0b00100010://OR
			lcd_clear();
			resulte = word_1 | word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);
			show_res = 1;
			break;
		case 0b00100001://AND
			lcd_clear();
			resulte = word_1 & word_2;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);
			show_res = 1;
			break;
		case 0b00100100://NOT
			lcd_clear();
			resulte = ~word_1;
			lcd_set_cursor(0,1);
			int2bin(resulte,result);					
			lcd_puts(result);
			show_res = 1;
			break;
		case 0b10000001://DEL ALL
			lcd_clear();
			word_1 = 0;
			word_2 = 0;
			resulte =0;
			show_res = 0;
			counter = 0;			
			break;
		default:// do nothing at all if more then 1 button is pressed
			break;
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

void int2bin(int n, char *s) 
{
    // determine the number of bits needed 
    int nbits = 16;  
    s[nbits] = '\0';
    // forcing evaluation as an unsigned value prevents complications
    // with negative numbers at the left-most bit
    unsigned int u = *(unsigned int*)&n;
    int i;
    unsigned int mask = 1 << (nbits-1); // fill in values right-to-left
    for (i = 0; i < nbits; i++, mask >>= 1)
    {
        s[i] = ((u & mask) != 0) + '0';
    }
}
