    #include <pthread.h>  
    #include <unistd.h>  
      
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;  
    static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  
   
    int head = 0;   
      
    /*[thread_func]*/  
    static void *thread_func(void *arg)  
    {  
      
        while (1) {  
            pthread_mutex_lock(&mtx);           //这个mutex主要是用来保证pthread_cond_wait的并发性  
            printf("thread %lu I locked = %d\n",pthread_self(), head);  
            if (head == 0)  
	    {               //这个while要特别说明一下，单个pthread_cond_wait功能很完善，为何这里要有一个while (head == NULL)呢？因为pthread_cond_wait里的线程可能会被意外唤醒，如果这个时候head != NULL，则不是我们想要的情况。这个时候，应该让线程继续进入pthread_cond_wait  
                printf("thread %lu I am begin wait head = %d\n",pthread_self(), head);  
                pthread_cond_wait(&cond, &mtx);         // pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，然后阻塞在等待对列里休眠，直到再次被唤醒（大多数情况下是等待的条件成立而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源  
                pthread_mutex_unlock(&mtx);             //临界区数据操作完毕，释放互斥锁  
                                                        //用这个流程是比较清楚的/*block-->unlock-->wait() return-->lock*/  
            }else{  
                printf("thread %lu Got %d from front of queue\n",pthread_self(), head);  
	        head = 0;	
                pthread_mutex_unlock(&mtx);             //临界区数据操作完毕，释放互斥锁  
            }
        }  
        return 0;  
    }  
      
    int main(void)  
    {  
        pthread_t tid[8];  
        int i;
	for(i=0;i<8;i++){
            pthread_create(&tid[i], NULL, thread_func, NULL);   //子线程会一直等待资源，类似生产者和消费者，但是这里的消费者可以是多个消费者，而不仅仅支持普通的单个消费者，这个模型虽然简单，但是很强大
        }  
        /*[tx6-main]*/  
        //sleep(1);  
        for (i = 0; i < 100000; i++) {  
            pthread_mutex_lock(&mtx);             //需要操作head这个临界资源，先加锁， 
	    head = i+1;
            printf("send %d to front of queue\n", head);  
            pthread_cond_signal(&cond);  
            pthread_mutex_unlock(&mtx);           //解锁  
            sleep(1);  
        }  
        printf("thread 1 wanna end the line.So cancel thread 2.\n");  
        //pthread_cancel(tid);             //关于pthread_cancel，有一点额外的说明，它是从外部终止子线程，子线程会在最近的取消点，退出线程，而在我们的代码里，最近的取消点肯定就是pthread_cond_wait()了。关于取消点的信息，有兴趣可以google,这里不多说了  
        for(i=0;i<8;i++){
            pthread_join(&tid[i], NULL);  
        }
        printf("All done -- exiting\n");  
        return 0;  
    }  
