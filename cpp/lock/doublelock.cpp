/**
 * Created by fangruibin
 * 测试死锁产生的场景
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m_mutex;

//线程1
void* threadFunc1(void* p)
{
  pthread_mutex_lock(&m_mutex);
  pthread_mutex_lock(&m_mutex);
  printf("thread 1 exit\n");
  usleep(100);
  pthread_mutex_unlock(&m_mutex);

  return NULL;
}

int main()
{
  //初始化锁
  pthread_mutexattr_t mutexAttr;
  pthread_mutexattr_init(&mutexAttr);
  pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE); //同线程可多次进入
  if (pthread_mutex_init(&m_mutex, &mutexAttr) != 0)
  {
    printf("init mutex 1 failed\n");
    return -1;
  }

  //初始化线程
  pthread_t hThread1;
  if (pthread_create(&hThread1, NULL, &threadFunc1, NULL) != 0)
  {
    printf("create thread 1 failed\n");
    return -1;
  }

  while (1)
  {
    sleep(1);
  }

  pthread_mutex_destroy(&m_mutex);
  return 0;
}

