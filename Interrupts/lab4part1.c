/*
Name of Group: 
Omid Azodi
Jeff Couch
Joseph Lira

Class: CS435 (Real-Time Concepts & Embedded Systems)
Professor: Dr. Springer

Program Specifications:
- Use 8 slider switches (SW 0-7) as inputs for entering numbers
- Use green lights (LEDG 0-7) to display number defined by slider switches
- Use red lights (LEDR 0-15) to display accumulated sum 
- When user pushes (KEY1) pushbutton displayed the accumulated sum
- Interrupt will trigger, only then will the slider switch values be read
*/

#include "nios2_ctrl_reg_macros.h"

//Addresses of Red & Green lights, slider switches, push button, seven segment display
#define RED_LED_BASE 0x10000000
#define SLIDER_SWITCH_BASE 0x10000040
#define PUSHBUTTON_BASE 0x10000050
#define SEVEN_SEGMENT_DISPLAY 0x10000020
#define GREEN_LED_BASE 0x10000010
#define PUSHBUTTON_EDGECAPTURE 0x1000005C

void Update_HEX_Display(unsigned short buffer);

int display_buffer = 0;//Used for Demo-Program when board first turned on
int sw_values = 0;//Used to read switch values 

/*Hex values for numbers (0-16)*/
char seven_seg_decode_table[] = 
{
  0x3F, 0x06, 0x5B, 0x4F,
  0x66, 0x6D, 0x7D, 0x07,
  0x7F, 0x67, 0x77, 0X7C,
  0x39, 0x5E, 0x79, 0x71,
};

int main(void) 
{
  volatile int * pushbutton_ptr = (int *)PUSHBUTTON_BASE;
  volatile int * slider_switch_ptr = (int *)SLIDER_SWITCH_BASE;
  volatile int * red_LED_ptr = (int *)RED_LED_BASE;
  volatile int * green_LED_ptr = (int*)GREEN_LED_BASE;
  volatile int* p_flag = (int*)PUSHBUTTON_EDGECAPTURE;
  int value = 0;

  unsigned int g_sum = 0x00000000;//Used for display total sum on (LEDR 0-15) 
  int g_sw_values = 0;//Used to read slider switch values
 
  *(p_flag) = 0;//Push button pressed, flag changes value from 0
  display_buffer = 0x76543210;
  *(pushbutton_ptr + 2) = 0xE;
  
  NIOS2_WRITE_IENABLE(0x2); // Set Nios II interrupt mask bit for pushbuttons
  NIOS2_WRITE_STATUS(1); // Enable Nios II interrupts

  //Continuously read data from switches & display values
  while (1) 
  {
    sw_values = *(slider_switch_ptr); // Read the slider switch values
    *(green_LED_ptr) = sw_values; // Light up the green LEDs
    
	/*p_flag changes values when push button is pressed */
	if (*(p_flag) != 0)  //Enter if the push button has been pressed (KEY1)
	{
	  value = sw_values; // Current value is slider switch value
      g_sum += value; // Sum is added with the current value (This is the accumulated sum)
      *(red_LED_ptr) = g_sum; // Red LEDR(0-15) is lighted up with accumulated sum value
      Update_HEX_Display(g_sum);// Accumulated sum displayed in HEX and on 7-Segment display
      *(p_flag) = 0;//Push Button value changed as if it were not pressed
    }
  }

  return 0;
}

void Update_HEX_Display(unsigned short buffer) 
{
	int i;//Loop counter
	unsigned int sum = 0;

	for (i = 0; i < 4; i++) 
	{
		sum <<= 8;
		//Sum now has a hex value
		sum |= seven_seg_decode_table[buffer >> 12];
		buffer <<= 4;
	}

	//Accumulated sum is stored on 7-segment display
	*(unsigned int *)SEVEN_SEGMENT_DISPLAY = sum;
}
