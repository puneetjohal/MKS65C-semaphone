#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

int * create()
{
  printf("okay, I'm gonna check if resources are available\n");
   
  int thing = open("./telephone.game", O_CREAT | O_TRUNC, 0666);
  key_t key = ftok("./telephone.game", 'R');
  int got = shmget(key, sizeof(int), 0644 | IPC_CREAT);
  int *ptr = shmat(got, 0, 0);
  semget(0xDABDAB, 1, IPC_CREAT);
  
  return ptr;
}
void remove()
{
  
}
