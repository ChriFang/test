/**
 * Created by fangruibin
 * 测试取消线程操作
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m_mutex;


//清理函数
void cleanProc(void* arg)
{
  pthread_t tid = pthread_self();
  printf("thread %lx was canceled\n", tid);
}

//线程函数
void* threadFunc(void* p)
{
  //设置可取消
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  //设置线程清理回调函数，如果线程是被cancel的，而不是自然结束的，那么回调函数会被执行
  pthread_cleanup_push(cleanProc, NULL);

  printf("thread %lx is running..\n", pthread_self());
  while (1)
  {
    //设置cancelation point
    pthread_testcancel();
    sleep(1);
    pthread_testcancel();
  }

  //取消线程清理回调函数，如果线程是自然结束的，那么回调函数是不会被执行的，需要把它取消掉
  pthread_cleanup_pop(0);

  printf("thread %lx exit normally\n", pthread_self());
  return NULL;
}

int main()
{
  //初始化线程
  pthread_t hThread;
  if (pthread_create(&hThread, NULL, &threadFunc, NULL) != 0)
  {
    printf("create thread failed\n");
    return -1;
  }

  sleep(5);
  pthread_cancel(hThread); //取消线程

  pthread_join(hThread, NULL);
  sleep(1);
  printf("main exit\n");

  return 0;
}


