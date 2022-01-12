/**
 * Created by fangruibin
 * 测试读锁嵌套导致死锁的场景
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>


pthread_rwlock_t m_lock;
int A = 0, B = 0;

//线程1
void* threadFunc1(void* p)
{
  //printf("thread 1 running..\n");
  pthread_rwlock_rdlock(&m_lock);
  printf("thread 1 read source A=%d\n", A);
  usleep(100); // 等待100ms，此时线程2大概率会被唤醒并申请写锁
  
  pthread_rwlock_rdlock(&m_lock);
  printf("thread 1 read source B=%d\n", B);
  
  //释放读锁
  pthread_rwlock_unlock(&m_lock);
  pthread_rwlock_unlock(&m_lock);
  
  return NULL;
}

//线程2
void* threadFunc2(void* p)
{
  //printf("thread 2 running..\n");
  pthread_rwlock_wrlock(&m_lock);
  A = 1;
  B = 1;
  printf("thread 2 write source A and B\n");
  
  //释放写锁
  pthread_rwlock_unlock(&m_lock);
  
  return NULL;
}

int main()
{
  //初始化读写锁
  if (pthread_rwlock_init(&m_lock, 0) != 0)
  {
    printf("init rwlock failed\n");
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
  
  pthread_rwlock_destroy(&m_lock);
  return 0;
}

