#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/* pthread variable */
pthread_t philosopher[10];
pthread_mutex_t count;
pthread_mutex_t mutex[10];
pthread_cond_t cond[10];

/* parameter for main */
int number_of_philosophers;
int number_of_msec;
int number_of_think_eat_cycles;

/* check cycle and states */
int current_cycle = 0;
int forks[10];
char philo[10][20];
char name[11][4] = {{"P1"},{"P2"},{"P3"},{"P4"},{"P5"},{"P6"},{"P7"},{"P8"},{"P9"},{"P10"}};

/* For count cycle */
void
count_cycle(){
	pthread_mutex_lock(&count);
	current_cycle++;
	pthread_mutex_unlock(&count);
}

/* For check cycle */
int
check_cycle(){
	
	int n = 0;
	pthread_mutex_lock(&count);
	if(current_cycle > number_of_think_eat_cycles){
		n = 1;
	}
	pthread_mutex_unlock(&count);
	
	return n;
}

/* fork allocated from philosopher*/
void
allocated_fork(int _fork){

        pthread_mutex_lock(&mutex[_fork]);
        while(forks[_fork] == 1){ 
            pthread_cond_wait(&cond[_fork], &mutex[_fork]);
        }
        forks[_fork] = 1;
        pthread_mutex_unlock(&mutex[_fork]);
}
/* fork deallocated from philosopher*/
void
deallocated_fork(int _fork){

        pthread_mutex_lock(&mutex[_fork]);
		forks[_fork] = 0;
        pthread_cond_signal(&cond[_fork]);
        pthread_mutex_unlock(&mutex[_fork]);
}

/* Dining Philosophers Algorithm */
void *
p_dining(int n){
	
	int left_fork;
	int right_fork;

	while(1){
		
		if(check_cycle() == 1){
			break;
		}
		
		left_fork = n;
		right_fork = (n + 1) % number_of_philosophers;
		if(check_cycle() == 1){
			return NULL;
		}
		/* Eating */
		/* even number of philosophers */
		if(n % 2 == 0){
			allocated_fork(left_fork);
			allocated_fork(right_fork);
		}
		/* odd number of philosophers */
		else{ 
			allocated_fork(right_fork);
			allocated_fork(left_fork);	
		}
		
		strcpy(philo[n],"Eating");
		count_cycle();
		if(check_cycle() == 1){
			return NULL;
		}
		/* print who eating and thinking */
		for(int i = 0; i < number_of_philosophers; i++){
			printf("%-10s", philo[i]);
		}
		printf("\n");
		
		sleep(number_of_msec);

 		/* Thinking */
		/* even number of philosophers */
		if(n % 2 == 0){
			deallocated_fork(right_fork);
			deallocated_fork(left_fork);
		}
		/* odd number of philosophers */
		else{ 
			deallocated_fork(left_fork);
			deallocated_fork(right_fork);
		}
		strcpy(philo[n],"Thinking");
		sleep(number_of_msec);
	}

	return NULL;
}


int 
main(int argv, char *argc[]){

    int i;
	
	/* check exception */
	number_of_philosophers = atoi(argc[1]);
	if(number_of_philosophers < 1 || number_of_philosophers > 11){
		printf("wrong number of philosophers\n");
		return -1;
	}
	number_of_msec = atoi(argc[2]);
	if(number_of_msec < 10 || number_of_msec > 1000){
		printf("wrong number of msec\n");
		return -1;
	}
	number_of_think_eat_cycles = atoi(argc[3]);
	if(number_of_think_eat_cycles < 1 || number_of_think_eat_cycles > 100){
		printf("wrong number of think eat cycles\n");
		return -1;
	}
	
	/* print name */
    for(i = 0; i < number_of_philosophers; i++){
		printf("%-10s",name[i]);
	}
	printf("\n");
	
    /* philo init thinking */
    for(i = 0; i < number_of_philosophers; i++){
		strcpy(philo[i], "Thinking");
	}

    /* pthread_mutex_init is used to create a mutex */
    /* pthread_cond_init is used to create a cond */
    for(i = 0; i < number_of_philosophers; i++){
        pthread_mutex_init(&mutex[i], NULL);
        pthread_cond_init(&cond[i], NULL);
    }
    
    for(i = 0; i < number_of_philosophers; i++){
    /* pthread_create is used to create a thread */
        pthread_create(&philosopher[i], NULL, (void *)p_dining, (void *)i );
    
    }
	
    /* pthread_wait is used to wait a thread finish*/
    for(i = 0; i < number_of_philosophers; i++){
        pthread_join(philosopher[i], NULL);
	}

    /* pthread_mutex_destroy is used to erase a mutex*/
    /* pthread_cond_destroy is used to erase a cond*/
    for(i = 0; i < number_of_philosophers; i++){
        pthread_mutex_destroy(&mutex[i]);
        pthread_cond_destroy(&cond[i]);
	}

    return 0;
}

