/**
 * Created by fangruibin
 * 测试死锁产生的场景
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>

//定义两把锁
pthread_mutex_t m_mutex1;
pthread_mutex_t m_mutex2;
int A = 0, B = 0;

//线程1
void* threadFunc1(void* p)
{
	//printf("thread 1 running..\n");
	pthread_mutex_lock(&m_mutex1);
	A = 1;
	printf("thread 1 write source A\n");
	usleep(100);
	
	pthread_mutex_lock(&m_mutex2);
	B = 1;
	printf("thread 1 write source B\n");
	
	//解锁，实际上是跑不到这里的，因为前面已经死锁了
	pthread_mutex_unlock(&m_mutex2);
	pthread_mutex_unlock(&m_mutex1);
	
	return NULL;
}

//线程2
void* threadFunc2(void* p)
{
	//printf("thread 2 running..\n");
	pthread_mutex_lock(&m_mutex2);
	B = 1;
	printf("thread 2 write source B\n");
	usleep(100);
	
	pthread_mutex_lock(&m_mutex1);
	A = 1;
	printf("thread 2 write source A\n");
	
	//解锁，实际上是跑不到这里的，因为前面已经死锁了
	pthread_mutex_unlock(&m_mutex1);
	pthread_mutex_unlock(&m_mutex2);
	
	return NULL;
}

int main()
{
	//初始化锁
	if (pthread_mutex_init(&m_mutex1, 0) != 0)
	{
		printf("init mutex 1 failed\n");
		return -1;
	}
	if (pthread_mutex_init(&m_mutex2, 0) != 0)
	{
		printf("init mutex 2 failed\n");
		return -1;
	}
	
	//初始化线程
	pthread_t hThread1;
	pthread_t hThread2;
	if (pthread_create(&hThread1, NULL, &threadFunc1, NULL) != 0)
	{
		printf("create thread 1 failed\n");
		return -1;
	}
	if (pthread_create(&hThread2, NULL, &threadFunc2, NULL) != 0)
	{
		printf("create thread 2 failed\n");
		return -1;
	}
	
	while (1)
	{
		sleep(1);
	}
	
	pthread_mutex_destroy(&m_mutex1);
	pthread_mutex_destroy(&m_mutex2);
	return 0;
}
