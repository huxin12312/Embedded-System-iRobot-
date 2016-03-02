/*
 * movement.c
 * implementation of basic movement functions.
 *
 * Created: 5/23/2013 12:31:42 PM
 *  @Author: newagain
 */ 
#include "open_interface.h"

/**
*   move forward the robot with the distance centimeters
*	@param sensor the robot to be moved
*   @param centimeters the distance robot moved.
**/
void move_forward(oi_t *sensor, int centimeters)
{
	char s1[] = "left object detect";
	char s2[] = "right object detect";
	char s3[]= "left cliff detect";
	char s4[]= "wall detect";
	char s5[] = "retrivial zone detect";
	char s7[] = "right cliff detect";

	int sum = 0;
	int i =0;

	oi_set_wheels(100,100);
	while(sum<10*centimeters)
	{
		if (sensor->cliff_frontleft||sensor->cliff_left)
		{
			USART_Transmit(13);
			USART_Transmit(10);
			oi_set_wheels(0,0);
			/// move backward 3 centimeters when detect the cliff.
			move_backforward(sensor,3);
			for (i;i<strlen(s3);i++)
			{
				USART_Transmit(s3[i]);
			}
			i=0;
			break;
			}
		if (sensor->cliff_frontright||sensor->cliff_right)
		{
			USART_Transmit(13);
			USART_Transmit(10);
			oi_set_wheels(0,0);
			/// move backward 3 centimeters when detect the cliff.
			move_backforward(sensor,3);
			for (i;i<strlen(s7);i++)
			{
				USART_Transmit(s7[i]);
			}
			i=0;
			break;
		}
		
		
		if (sensor->bumper_left)
			{
				USART_Transmit(13);
				USART_Transmit(10);
				oi_set_wheels(0,0);
				/// move backward 3 centimeters when detect the small object.
				move_backforward(sensor,3);
				for (i=0;i<strlen(s1);i++)
				{
					USART_Transmit(s1[i]);
				}
				i=0;
				break;
			}
			
			
			if (sensor->bumper_right)
			{
				USART_Transmit(13);
				USART_Transmit(10);
				oi_set_wheels(0,0);
				/// move backward 3 centimeters when detect the small object.
				move_backforward(sensor,3);
				for (i=0;i<strlen(s2);i++)
				{
					USART_Transmit(s2[i]);
				}
				i=0;
				break;
			}
			
			
			if (sensor->virtual_wall)
			{
				USART_Transmit(13);
				USART_Transmit(10);
				oi_set_wheels(0,0);
				/// move backward 3 centimeters when detect the virtual wall.
				move_backforward(sensor,3);
				for (i=0;i<strlen(s4);i++)
				{
					USART_Transmit(s4[i]);
				}
				i=0;
				break;
			}
			
		oi_update(sensor);
		sum+=sensor->distance;
	}
	///stop after the movement
	oi_set_wheels(0,0);//stop
}

/**
*   move backward the robot with the distance centimeters
*	@param sensor the robot to be moved
*   @param centimeters the distance robot moved.
**/

void move_backforward(oi_t *sensor, int centimeters)
{
	int sum = 0;
	oi_set_wheels(-100,-100);
	while(sum>-10*centimeters)
	{
		oi_update(sensor);
		sum+=sensor->distance;
	}
	///stop after the movement
	oi_set_wheels(0,0);
}

/**
*   used to turn the robot clockwise with degrees
*	@param sensor the robot to be moved.
*   @param degrees the degree of robot turned.
*/

void turn_clockwise(oi_t *sensor, int degrees)
{
	int ang=0;
	oi_set_wheels(150,-150);
	while(ang<degrees)
	{
	oi_update(sensor);
	ang+=sensor->angle;
	}
	oi_set_wheels(0,0);///stop
}

/**
*   used to turn the robot counter_clockwise with degrees
*	@param sensor the robot to be moved.
*   @param degrees the degree of robot turned.
*/
void turn_counterclockwise(oi_t *sensor, int degrees)
{
	int ang=0;
	oi_set_wheels(-150,150);
	while(ang>-degrees)
	{
		oi_update(sensor);
		ang+=sensor->angle;
	}
	///stop after the movement
	oi_set_wheels(0,0);
	
}	


