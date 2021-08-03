#include <stdio.h>
#include <time.h>

int main(void){
	time_t timer;
	time(&timer);

	printf("local time is: %s",ctime(&timer));
}