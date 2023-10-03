
#include "main.h"

#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>

//char name_thread[MAX_LEN_NAME] = "Wait_process";
void *threads_loader_life(void *param); /* stream function loader */
void *threads_buyer_life(void *param); /* stream function buyer */

unsigned int needs_stores[5] = {1000, 1000, 1000, 1000, 1000};
pthread_mutex_t acses_store[5] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};
char flag_block[5] = {0, 0, 0, 0, 0};

unsigned int counter_live_buyers = NUMBER_BUYER;
int buyers_need[NUMBER_BUYER];

int main(int argc , char *argv[])
{
    unsigned int temp_mas[NUMBER_BUYER];
    int *result;
    
    printf("Great 5 stores\n");
    for(unsigned int i = 0; i < 5; ++i){printf("Stores %d = %d\n", i, needs_stores[i]);}
    
    
    for(unsigned int i = 0; i < NUMBER_BUYER; ++i){buyers_need[i] = 10000;}
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    pthread_t thread_loader;	 /* thread ID */
    if(0 != pthread_create(&thread_loader, NULL, threads_loader_life, NULL)){perror ("PTHREAD_CREATE");return (-1);}	/* create a new thread */
    printf("Init loader\n");
    //////////////////////////////////////////////////////////////////////////////////////////////
    pthread_t thread_buyer[NUMBER_BUYER];	 /* thread ID */
    for(unsigned int i = 0; i < NUMBER_BUYER; ++i)
    {
        temp_mas[i] = i;
        if(0 != pthread_create(&thread_buyer[i], NULL, threads_buyer_life, (void *) &temp_mas[i])){perror ("PTHREAD_CREATE");return (-1);}	/* create a new thread */
        printf("Boarn buyer %d = %d\n", i, buyers_need[i]);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    while(0 != counter_live_buyers)
    {
    }
    
    
    
    
    
    
    
    pthread_join(thread_loader, (void *) &result);    /* wait end work thread */
    for(unsigned int i = 0; i < NUMBER_BUYER; ++i)
    {
        pthread_join(thread_buyer[i], (void *) &result);    /* wait end work thread */
    }
    
    printf("The End\n");
    return 0;
}





/* function loader */
void *threads_loader_life(void *param) 
{
    while(0 != counter_live_buyers)
    {
        for(unsigned int i = 0; i < 5; ++i)
        {
            if(0 == flag_block[i])
            {
                pthread_mutex_lock(&acses_store[i]);
                flag_block[i] = 1;
                
                printf("Loader in store %d, products befor = %d", i, needs_stores[i]);
                needs_stores[i] = needs_stores[i] + 500;
                printf(", products after = %d\n", needs_stores[i]);
                
                pthread_mutex_unlock(&acses_store[i]);
                flag_block[i] = 0;
                printf("Loader sleep\n");
                sleep(2);
            }
        }
    }
    printf("Loader is go away\n");
    pthread_exit(0);
}

/* function buyer*/
void *threads_buyer_life(void *param) 
{
//    unsigned int *num_buyer = (unsigned int *) param;
    unsigned int loc_num = (*((unsigned int *) param));
    while(0 < buyers_need[loc_num])
    {
        for(unsigned int i = 0; i < 5; ++i)
        {
            if(0 == flag_block[i])
            {
                pthread_mutex_lock(&acses_store[i]);
                if(0 != needs_stores[i])
                {
                    flag_block[i] = 1;
                    
                    printf("Buyer %d in store %d, products instore = %d, buyers_need befor = %d", loc_num, i, needs_stores[i], buyers_need[loc_num]);
                    buyers_need[loc_num] = buyers_need[loc_num] - needs_stores[i];
                    needs_stores[i] = 0;
                    printf(", buyers_need after = %d\n", buyers_need[loc_num]);
                    
                    pthread_mutex_unlock(&acses_store[i]);
                    flag_block[i] = 0;
                    printf("Buyer %d sleep\n", loc_num);
                    sleep(1);
                }
                else
                {
                    pthread_mutex_unlock(&acses_store[i]);
                }
            }
        }
    }
    printf("Buyer %d died\n", loc_num);
    --counter_live_buyers;
    pthread_exit(0);
}






