/*
 * FinalProject.c
 * implementation of basic robot movement functions.
 * Created: 7/2/2013 11:28:30 AM
 *  Author: newagain
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "open_interface.h"
#include "lcd.h"
#include "util.h"
#include "uart.h"
#include <stdio.h>
#include <avr/sleep.h>
#include "movement.h"
#include "music.h"


volatile unsigned Raising_edge = 0;
volatile unsigned Falling_edge = 0;
volatile unsigned overflows1 = 0;
volatile unsigned overflows2 = 0;
volatile int update_flag = 0;
volatile char a = 0;
volatile unsigned time_range, pingDistance;
char s6[] = "retrival zone detect";
char s8[] = "Retrival pillar detect";


ISR (TIMER1_OVF_vect)
{
	overflows2 ++;
}

ISR (TIMER1_CAPT_vect)
{
	if((TCCR1B & 0x40) == 0x40)
	{
		Raising_edge = ICR1;
	}
	if((TCCR1B & 0x40) == 0x00)
	{
		Falling_edge = ICR1;
		update_flag = 1;
	}
	TCCR1B ^= 0x40;
}
/**
*	used to transmit the words to terminal.
*	@param str the string to print on the terminal.
**/
void serial_puts(char* str);

unsigned interval = 43000;

void serial_puts(char* str)
{
	USART_Transmit('\n');
	USART_Transmit('\r');
	int size = strlen(str);
	for(int i = 0; i < size; i++)
	{
		USART_Transmit(str[i]);
	}
}

/**
*	used to initialize the timer
**/
void timer_init(void)
{
	//TCCR1A  set all COM as zero,  set WGM all for 0 to use input capture
	TCCR1A = 0b00000000;
	//TCCR1B  set 7(ICNC1) for Noise canceller,  set 4 and 3 as zero for WGM, prescalar 8 bit 2 1 0 (010);
	TCCR1B = 0b10000010;
	//TIMSK   ubterrupt Enable bit 5, Overflow Interrupt enable bit 2
	TIMSK = 0b00100100;
	//TIFR bit 2 set to 1 to overflow interrupt enable
	//TIFR = 0b00100000;
	//sei();
	
}
/**
*	used to initialize the timer3
*/
void timer3_init(void)
{
	OCR3A = interval-1;	// number of cycles in the interval
	OCR3B = 2500-1;				// if you want to move servo to the middle
	TCCR3A = 0b10101011;		// set COM and WGM (bits 3 and 2)
	TCCR3B = 0b00011010;		// set WGM (bits 1 and 0) and CS
	TCCR3C = 0;
	
	DDRE |= _BV(4);				// set Port E pin 4 (OC3B) as output
}


unsigned timer(unsigned width)
{
	return width*34000/2000000;
}

/**
*	used to move the servo
*	@param degree the degree that the servo moved.
**/
void move_servo(unsigned degree)
{
	unsigned pulse_width;		// pulse width in cycles
	
	pulse_width = 1000 + 3500/180*degree;
	
	OCR3B = pulse_width-1;		// set pulse width
}

/**
*	used to enable the transmit of USART
**/
void transmit_pulse(void)
{
	cli();
	DDRD |= 0x10;			// set PD4 as output
	PORTD |= 0x10;			// set PD4 to high
	
	wait_ms(1);				// wait
	PORTD &= 0xEF;			// set PD4 to low
	DDRD &= 0xEF;			// set PD4 as input
	
}


/**
*	Used to convert analog to digital.
*	@param channel set channel ADC.
*	@return the result of ADC
**/
unsigned ADC_read(char channel)
{
	ADMUX |= (channel & 0x2F);
	ADCSRA |= _BV(ADSC);
	while (ADCSRA & _BV(ADSC))
{}
	return ADC;
}
/**
* used to initialize the ADC.
**/
void ADC_init(void)
{
	ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX1);
	ADCSRA = _BV(ADEN) | (7<<ADPS0);
}

/**
*	Used to play the sound in the retrieval zone.
**/
void load_songs(void) 
 {
	    unsigned char NumberofNotes = 58;
	    unsigned char Notesofsongs[58]    = {120, 1, 120, 2, 120, 3, 120, 1, 2, 3, 120,  1, 127, 1, 2, 3, 78, 1, 79, 2, 70, 3, 120, 3, 132, 2, 128, 1, 70, 1, 2, 3, 127, 1, 127, 2, 77, 3, 78, 4, 75, 5, 75, 6, 75, 7, 73, 5, 6, 7, 77, 1, 120, 1, 70, 2, 75, 2, 75, 3, 77, 3,77};
	    unsigned char Durationofsongs[58] = {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
	    oi_load_song(songings, NumberofNotes, Notesofsongs, Durationofsongs);
 }


/**
*	Used to control the robot.
*	Receive and transmit data, measure the distance from object and navigate to the retrieval zone.
**/
int main(void)
{
	lcd_init();
	timer3_init();
	timer_init();
	ADC_init();
	USART_Init();
	oi_t *sensor_data = oi_alloc();
	oi_init(sensor_data);//should turn the iRobot Create's power LED yellow
  
	lcd_init();
	serial_puts("Start");
	//USART_Transmit(13);
	//USART_Transmit(10);
	
	
    int TempAngle[4] = {0,0,0,0};
	int TempIR[4] = {0,0,0,0};
	int pos[4] = {0,0,0,0};
	int AddIR[4] = {0,0,0,0};
	int count[4] = {0,0,0,0};
	int found = 0;
	int x1 = 0;
	int x2 = 0;
	int x3 = 0;
	int x4 = 0;
	unsigned angle = 0;
	unsigned char IR = 0;
	volatile int i=0;
	volatile int x = 0;
	char command;
    char display[100];
	char display1[20];
	char display2[20];
	char display3[20];
	char display4[20];
	char display5[100];
	char display6[100];	
			
	while (1)
	{
		command = USART_Recieve();
		USART_Transmit(command);
		//USART_Transmit(13);
		//USART_Transmit(10);
		
		if (command == '1')
		{
			found = 0;
			angle = 0;
			int t;
			int TempAngle[6] = {0, 0,0,0,0,0};
			int TempIR[6] = {0, 0,0,0,0,0};
			int pos[6] = {0, 0,0,0,0,0};
			int AddIR[6] = {0, 0, 0,0,0,0};
			int count[6] = {0, 0, 0,0,0,0};
			for (angle = 0;angle < 181;angle++)
			{	
				move_servo(angle);
				wait_ms(20);				
				
				IR = 0;
				IR = 42800*pow(ADC_read(2),-1.23);
				
				sprintf(display6, "Angle: %5d		IR: %5d",angle,IR);
				serial_puts(display6);

				if (IR < 80)
				{
				   TempAngle[found]++;
				   count[found]++;
				   AddIR[found]+=IR;
				   TempIR[found]=AddIR[found]/count[found];
				}
				
				else
				{
					if(TempAngle[found] < 5)
					{
						TempAngle[found] = 0;
					}
					
					else
					{
						pos[found] = angle- TempAngle[found]/2;
						if (TempIR[found]*TempAngle[found]< 460)
						{
							USART_Transmit(13);
							USART_Transmit(10);
							for (int i = 0;i<strlen(s8);i++)
							{
								USART_Transmit(s8[i]);
							}
							
							sprintf(display5, "object position: %5d",pos[found]);
							serial_puts(display5);
						}
						sprintf(display, "object position: %5d		IR: %5d		object size: %5d",pos[found],TempIR[found],TempAngle[found]);
						serial_puts(display);
						USART_Transmit(13);
						USART_Transmit(10);
						found++;
					}	
			     }
			}
			OCR3B = 1000-1;				//return to 0 degree	
		}				

		 if (command == 'w')
		 {
			 move_forward(sensor_data,20);
		 }
		 if (command == 's')
		 {
			 move_backforward(sensor_data,20);
		 }
		 if (command == 'a')
		 {
			turn_clockwise(sensor_data,82);
		 }
		 if (command == 'd')
		 {
			turn_counterclockwise(sensor_data,82);
		 }
		 if (command == 'q')
		 {
			 turn_clockwise(sensor_data,38);
		 }
		 if (command == 'e')
		 {
			 turn_counterclockwise(sensor_data, 38);
		 }
		 if (command == '8')
		 {
			 move_forward(sensor_data,5);
		 }
		 if (command == '5')
		 {
			 move_backforward(sensor_data,5);
		 }
		 if (command == 'p')
		 {
			 oi_t* sensor = oi_alloc();
			 oi_init(sensor);
			 
			 load_songs();
			 oi_play_song(songings);
		 }
		 if(command == 'k')
		 {
			oi_update(sensor_data);
			x1 = sensor_data->cliff_left_signal;
			x2 = sensor_data->cliff_right_signal;
			x3 = sensor_data->cliff_frontleft_signal;
			x4 = sensor_data->cliff_frontright_signal; 
			sprintf (display1, "left        = %d",x1);
			sprintf (display2, "right       = %d",x2);
			sprintf (display3, "front left  = %d",x3);
			sprintf (display4, "front right = %d",x4);
			USART_Transmit(13);
			USART_Transmit(10);
	        serial_puts(display1);
	        serial_puts(display3);
	        serial_puts(display4);			
			serial_puts(display2);
			if (x1>500||x2>500||x3>500||x4>500)
			{
				USART_Transmit(13);
				USART_Transmit(10);
				for (int i=0;i<strlen(s6);i++)
				{
					USART_Transmit(s6[i]);
				}
			}						
		}
	}	
}		
