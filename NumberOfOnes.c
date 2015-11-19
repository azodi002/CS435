/* Count the number of 1s in a 32-bit word */

#include <stdio.h>
unsigned int x, TEST_DATA, RESULT;
int main()
{
  while(1)
    {
      printf("Enter a number: ");
      scanf("%d", &TEST_DATA);
      printf("\n");
      RESULT = 0;
      x = TEST_DATA;
      while (x != 0)
	{ 
	  if ((x & 0x80000000) > 0) RESULT++;
	  x = x << 1;
	}
      printf( "The number of 1's is: %d \n", RESULT );
    }

  return 0;
    
}
