
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;

void* start_thread(void *message)
{
    sleep(1);
    printf("%s\n",(const char*)message);
    pthread_mutex_lock (&mutex);
    printf("thread lock\n");
    pthread_mutex_unlock(&mutex);
    return message;
}


int main(void)
{
    pthread_attr_t attr;
    
    pthread_mutex_init(&mutex, NULL);
   
    pthread_t thing1,thing2;
    const char *message1 = "thing 1";
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thing1,&attr,start_thread,(void *) message1);
    
    pthread_mutex_lock (&mutex);
    sleep(3);
    printf("main lock\n");
    pthread_mutex_unlock(&mutex);
    //pthread_join(thing2,NULL);
    pthread_exit(NULL);
    printf("return \n");  //nerver run this!!
    return 0;
}
