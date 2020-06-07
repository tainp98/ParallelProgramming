#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define INIT_BALANCE 50
#define NUM_TRANS 120
int balance = INIT_BALANCE;
int credits = 0;
int debits = 0;
pthread_mutex_t b_lock,c_lock,d_lock;
void * transactions(void * args){
	int i,v;
	srand((int)time(0));
	for(i=0;i<NUM_TRANS;i++){
//choose a random value	
		
		v = rand() % NUM_TRANS;
//randomnly choose to credit or debit
		if( rand()% 2){
//credit
			pthread_mutex_lock(&b_lock);
			pthread_mutex_lock(&c_lock);
			balance = balance + v;
			credits = credits + v;
			pthread_mutex_unlock(&b_lock);
			pthread_mutex_unlock(&c_lock);
			
		}else{
//debit
			pthread_mutex_lock(&b_lock);
			pthread_mutex_lock(&d_lock);
			balance = balance - v;
			debits = debits + v;
			pthread_mutex_unlock(&b_lock);
			pthread_mutex_unlock(&d_lock);
		}
		}
	return 0;
	}
int main(int argc, char * argv[]){
	int n_threads,i;
	pthread_t * threads;
	printf("number of threads : ");
	scanf("%d",&n_threads);
//error check
	if(n_threads < 2){
		fprintf(stderr, "ERROR: Require number of threads\n");
		exit(1);
		}
//error check
	if(n_threads <= 0){
		fprintf(stderr, "ERROR: Invalivd value for number of threads\n");
		exit(1);
		}
//allocate array of thread identifiers
	threads = calloc(n_threads, sizeof(pthread_t));
//start all threads
	for(i=0;i<n_threads;i++){
		pthread_create(&threads[i], NULL, transactions, NULL);
		}
//wait for all threads finish its jobs
	for(i=0;i<n_threads;i++){
		pthread_join(threads[i], NULL);
		}
	printf("\tCredits:\t%d\n", credits);
	printf("\t Debits:\t%d\n\n", debits);
	printf("%d+%d-%d= \t%d\n", INIT_BALANCE,credits,debits,INIT_BALANCE+credits-debits);
	printf("\t Balance:\t%d\n", balance);
//free array
	free(threads);
	pthread_mutex_destroy(&b_lock);
	pthread_mutex_destroy(&c_lock);
	pthread_mutex_destroy(&d_lock);
	return 0;
	}
