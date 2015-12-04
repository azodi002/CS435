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
*/

//Addresses of Red & Green lights, slider switches, push button
#define RED_LED_BASE 0x10000000
#define GREEN_LED_BASE 0x10000010
#define SLIDER_SWITCH_BASE 0x10000040
#define PUSHBUTTON_BASE 0x10000050
#define PUSHBUTTON_EDGECAPTURE 0x1000005C

int g_display_buffer = 0; //Used for Demo-Program when board first turned on
unsigned int g_sum = 0x00000000; //Used for display total sum on (LEDR 0-15)
int g_sw_values = 0; //Used to read switch values

int main(void)
{

  volatile int* p_pushbutton = (int*) PUSHBUTTON_BASE;
  volatile int* p_slider_switch = (int*) SLIDER_SWITCH_BASE;
  volatile int* p_red_LED = (int*) RED_LED_BASE;
  volatile int* p_green_LED = (int*) GREEN_LED_BASE;
  volatile int* p_flag = (int*) PUSHBUTTON_EDGECAPTURE;
  int value = 0;

  *(p_flag) = 0; //Push button pressed, flag changes value from 0 
  g_display_buffer = 0x76543210;
  *(p_pushbutton + 2) = 0xE;

  //Continuously read data from switches & display values
  while(1)
   {
     g_sw_values = *(p_slider_switch); //Slider switch values stored
     *(p_green_LED) = g_sw_values; //Slider switch values shown on green LEDG(0-7)
     
     /*p_flag changes values when push button is pressed */ 
     if(*(p_flag) != 0) //Enter if the push button has been pressed (KEY1)
       {
	 value = g_sw_values; //Green LEDG(0-7) value is stored 
	 g_sum += value; //Sum is incremented
	 *(p_red_LED) = g_sum; //Sum is displayed on LEDR(0-15)
	 *(p_flag) = 0; //Push Button value changed as if it were not pressed
       }
    }

  return 0;
}
