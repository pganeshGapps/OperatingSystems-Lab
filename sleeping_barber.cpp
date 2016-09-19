#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

queue<int> osbook, filmfare;

int numberOfCustomers = 1; // i'th customer... initialising with first

pthread_t cust_thread;
pthread_t barber_thread[3];
pthread_t cash_thread;

pthread_mutex_t lock;

sem_t filmfarefree, osbookfree, barberfree, chairfree, cust_ready, leavechair, payment, receipt, seatsfree, cuthair, receive, paid;

/** Utility Functions **/

void entershop(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" entered into the shop\n";
	pthread_mutex_unlock(&lock);
}

void readbook(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is reading an OS book\n";
	pthread_mutex_unlock(&lock);
}
void readfilmfare(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is reading a Filmfare Magazine\n";
	pthread_mutex_unlock(&lock);
}
void sitinbarberchair(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is sitting in chair\n";
	pthread_mutex_unlock(&lock);
}
void cutHair(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Barber "<<id<<" is cutting hair...\n";
	pthread_mutex_unlock(&lock);
}
void pay(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is paying\n";
	pthread_mutex_unlock(&lock);
}
void acceptPayment(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Barber "<<id<<" is accepting payment\n";
	pthread_mutex_unlock(&lock);
}
void takereceipt(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is receiving receipt\n";
	pthread_mutex_unlock(&lock);
}
void leave(int id)
{
	pthread_mutex_lock(&lock);
	cout<<"Customer "<<id<<" is leaving the shop\n";
	pthread_mutex_unlock(&lock);
}
void finishedHairCut(int id){
   		pthread_mutex_lock(&lock);
		cout<<endl<<"Customer "<<id<<" finished hair cutting."<<endl;
		pthread_mutex_unlock(&lock);
}
void * customer(int id)
{
	while(osbook.front() != id); //***wait until customer is at front of osbook queue

	sem_wait(&filmfarefree);
            //acquire resources in filmfare queue
	    filmfare.push(id);
	    readfilmfare(id);
            //get out of osbook queue
	    sem_post(&osbookfree);
	    osbook.pop();

            sem_wait(&barberfree);
	while(filmfare.front() != id); //wait untill customer is at front of filmfare queue
	    //acquire resources for hair cutting and leave filmfare queue
	    sem_wait(&chairfree);
	         sem_post(&filmfarefree);
	         sitinbarberchair(id);//just printing
	                sem_post(&cust_ready);//freeing from Ready Queue#########
	                	filmfare.pop();
	                	sem_wait(&cuthair);
	                	sleep(4);// 4s is hair cut time
                                finishedHairCut(id);
	           	    sem_post(&leavechair);
	    sem_post(&chairfree);

	pay(id);
	sem_post(&payment);
	sem_wait(&receipt);
	takereceipt(id);
	leave(id);
	sem_post(&seatsfree);
}

void * barber(int id)
{	
	
	while(true)
	{       
		sem_wait(&cust_ready);
		sem_post(&cuthair);
                cutHair(id);
		sem_wait(&receive);
		acceptPayment(id);
		sem_post(&paid);
		sem_post(&barberfree);
		cout<<"Barber "<<id<<" is free"<<endl;
	}
}
void * cashCounter()
{
	while(true)
	{
		sem_wait(&leavechair);//#######
		sem_wait(&payment);
		sem_post(&receive);
		sem_wait(&paid);
		sem_post(&receipt);
	}
}
int main()
{
	int j;
	sem_init(&filmfarefree, 0, 4);
	sem_init(&osbookfree, 0, 3);//as the higher priority 4 customers are given Filmfare ,only 3 remains
	sem_init(&barberfree, 0, 3);
	sem_init(&chairfree, 0, 3);
	sem_init(&cust_ready, 0, 0);
	sem_init(&leavechair, 0, 0);
	sem_init(&payment, 0, 0);
	sem_init(&receipt, 0, 0);
	sem_init(&receive, 0, 0);
	sem_init(&seatsfree, 0, 10); //total seats
	sem_init(&cuthair, 0, 0);
	sem_init(&paid, 0, 0);

	for(int i = 0;i < 3;i++)
	{
		pthread_create(&barber_thread[i], NULL, (void *(*) (void *)) barber, (void *) (i + 1));
	}
	pthread_create(&cash_thread, NULL, (void *(*) (void *)) cashCounter, NULL);
	cout<<"Press 'Enter' anytime to indicate an incoming customer...(Enter 'C' or 'c' to Close Entries)\n\n";
	while(numberOfCustomers <= 50)
	{
		char ch;
		ch = getchar();
		if(ch == '\n')
		{
			sem_wait(&seatsfree);
			entershop(numberOfCustomers);
			osbook.push(numberOfCustomers);
			sem_wait(&osbookfree);
			readbook(numberOfCustomers);
			pthread_create(&cust_thread, NULL, (void *(*) (void *)) customer, (void *) (numberOfCustomers));
			numberOfCustomers++;
		}
		if(ch=='C' || ch=='c'){break;}
	}

	pthread_join(cust_thread, (void **)&j);
	cout<<"\n-------Day Finished-------\n\n";
	return 0;
}
