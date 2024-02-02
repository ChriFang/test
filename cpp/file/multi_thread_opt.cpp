/**
 * Created by fangruibin
 * 测试多线程读写文件
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include<sys/file.h>

const char* fileName = "test.txt";
pthread_mutex_t m_mutex;

void processRead()
{
  pthread_mutex_lock(&m_mutex);
  FILE* fp = fopen(fileName, "r");
  if (fp == NULL)
  {
    printf("open file for read failed\n");
    return;
  }
  
  // 获取文件长度
  long startPos = ftell(fp);
  fseek(fp, 0, SEEK_END);
  long endPos = ftell(fp);
  long fileSize = endPos - startPos;
  fseek(fp, 0, SEEK_SET);
  
  char* pBuf = (char*)malloc(fileSize + 1);
  size_t readSize = fread(pBuf, 1, fileSize, fp);
  if ((long)readSize != fileSize)
  {
    printf("read not complete %ld\n", (long)readSize);
  }
  pBuf[fileSize] = '\0';
  if (strlen(pBuf) == 0)
  {
    printf("read empty\n");
  }
//  printf("read:\n%s\n", pBuf);
  
  free(pBuf);
  fclose(fp);
  pthread_mutex_unlock(&m_mutex);
}

void processWrite()
{
  pthread_mutex_lock(&m_mutex);
  FILE* fp = fopen(fileName, "w");
  if (fp == NULL)
  {
    printf("open file for write failed\n");
    return;
  }
  
  const char* pData = "2222222222222222222222222222\n2222222222222222222222222222\n2222222222222222222222222222\n";
  size_t dataLen = strlen(pData);
  size_t writeLen = fwrite(pData, 1, dataLen, fp);
  if (dataLen != writeLen)
  {
    printf("write not complete %ld\n", (long)writeLen);
  }
  fclose(fp);
  pthread_mutex_unlock(&m_mutex);
}

//线程1
void* threadFunc1(void* p)
{
  while (1)
  {
    processRead();
    usleep(100 * 1000);
  }
  
  return NULL;
}

//线程2
void* threadFunc2(void* p)
{
  while (1)
  {
    processWrite();
    usleep(100 * 1000);
  }
  
  return NULL;
}

int main()
{
  if (pthread_mutex_init(&m_mutex, 0) != 0)
  {
    printf("init mutex failed\n");
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
  
  pthread_mutex_destroy(&m_mutex);
  return 0;
}

