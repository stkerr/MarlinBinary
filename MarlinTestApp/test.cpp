#define DUMMY_FUNC( x ) int function ## x (){int a = x; int b = x; return a * b;}

#define DUMMY_JUMP_FUNC( x ) int jump_function ## x (){ return function ## x () ; }

#include <stdio.h>

DUMMY_FUNC(1)
DUMMY_FUNC(2)
DUMMY_FUNC(3)
DUMMY_FUNC(4)
DUMMY_FUNC(5)
DUMMY_FUNC(6)
DUMMY_FUNC(7)
DUMMY_FUNC(8)
DUMMY_FUNC(9)
DUMMY_FUNC(10)


DUMMY_JUMP_FUNC(1)
DUMMY_JUMP_FUNC(2) 
DUMMY_JUMP_FUNC(3) 
DUMMY_JUMP_FUNC(4) 
DUMMY_JUMP_FUNC(5) 
DUMMY_JUMP_FUNC(6) 
DUMMY_JUMP_FUNC(7) 
DUMMY_JUMP_FUNC(8) 
DUMMY_JUMP_FUNC(9) 
DUMMY_JUMP_FUNC(10) 

int main()
{
	function1();
	function2();
	function3();
	function4();
	function5();
	function6();
	function7();
	function8();
	function9();
	function10();
		
	int val[10];
	 val[0] = jump_function1();
	 val[1] = jump_function2();
	 val[2] = jump_function3();
	 val[3] = jump_function4();
	 val[4] = jump_function5();
	 val[5] = jump_function6();
	 val[6] = jump_function7();
	 val[7] = jump_function8();
	 val[8]  = jump_function9();
	 val[9] = jump_function10();
	

	int(*myfp)(void) = function1;
	
	int sum = 0;
	for(int i = 0; i < 10; i++)
	{
		printf("Value %d: %d\n", i, val[i]);
		sum += val[i];
	}

	printf("Sum: %d\n", sum); 

	return myfp();
}
