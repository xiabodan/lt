#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* loopThread(void *message)
{
    while(1){
        
    }    
}

void* eventThread(void *message)
{

}

int main(void)
{
	pthread_t thing1,thing2;
	const char *message1 = "thing 1";
	const char *message2 = "thing 2";	

	pthread_create(&thing1,NULL,loopThread,(void *) message1);
	pthread_create(&thing2,NULL,eventThread,(void *) message2);

	pthread_join(thing1,NULL);
	pthread_join(thing2,NULL);

	printf("return \n");  //nerver run this!!
	return 0;
}
