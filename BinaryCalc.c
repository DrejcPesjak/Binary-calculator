// basic stuff to start working in AVRstudio
#include <avr/io.h>     
#include <util/delay.h>

//function prototypes(not needed in the arduini IDE)
void getKeyPress(void);
void keyData(int keyWord);

int keyRow,columnData,resulte,reset_loop;;
unsigned char matrix_data[]={0,0,0,0};
unsigned char word_1=0,word_2=0;

int main(void)
{
	DDRB = 0x0F;//pins PB0-PB3 outputs and pins PB4-PB7 inputs
  
	while(1)
	{   		
		getKeyPress();		
	}
	return 1;
}                

void getKeyPress() // checks what key is being pressed
{ 
    for(keyRow=0;keyRow<4;keyRow++) // scans the switch matrix(4 rows)
	{ 
		PORTB = (1<<keyRow); // sends a HIGH to one row at a time
		columnData = (PINB & 0xF0);// reads the state of each column
		_delay_us(100);// a small delay
		if(columnData != 0) // if something is pressed columndata is bigger then zero
		{
		keyData(columnData | (1<<keyRow));//goes to the data analysis function
		_delay_us(400);//a small delay like in the LED scanning loop
		}
    }
 }

void keyData(int keyInput) //function for determining what each button does
{ 
   switch (keyInput)
   {
		case 0b10000100:// first input row "1"
			word_1 = word_1 << 1;
			word_1 = word_1 | 0x01;
			matrix_data[3] = word_1;
			break;
		case 0b10000010://first input row "0"
			word_1 = word_1 << 1;
			matrix_data[3] = word_1;
			break;
		case 0b10001000://first input row "DEL"
			word_1 = 0;
			matrix_data[3] = word_1;
			break;
		case 0b01000100://second input row "1"				DELETE
			word_2 = word_2 << 1;
			word_2 = word_2 | 0x01;
			matrix_data[2] = word_2;
			break;
		case 0b01000010://second input row "0"				DELETE
			word_2 =  word_2<<1;
			matrix_data[2] = word_2;
			break;
		case 0b01001000://second input row "DEL"			DELETE
			word_2 = 0;
			matrix_data[2] = word_2;
			break;
		case 0b00010010://multiplication
			resulte = word_1 * word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00010100://subtraction
			resulte = word_1 - word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00011000://division 
			resulte = word_1 / word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00010001://addition
			resulte = word_1 + word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00100010://modulo							DELETE
			resulte = word_1 % word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00100100://XOR								DELETE
			resulte = word_1 ^ word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00101000://OR		
			resulte = word_1 | word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b00100001://AND
			resulte = word_1 & word_2;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b10000001://NOT
			resulte = ~resulte;
			matrix_data[1] = resulte & 0x00FF;
			matrix_data[0] = (resulte >> 8) & 0x00FF;
			break;
		case 0b01000001://DEL ALL
			word_1 = 0;
			word_2 = 0;
			resulte =0;
			for(reset_loop=0;reset_loop<4;reset_loop++)
				matrix_data[reset_loop] = 0;
			break;
		default:// do nothing at all if more then 1 button is pressed
			break;
    }
}

