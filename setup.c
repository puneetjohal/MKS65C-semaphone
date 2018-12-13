#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 0xDABDAB

union semun {
  int val; /* Value for SETVAL */
  struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO */
};
int * create()
{
  printf("okay, I'm gonna check if resources are available\n");
   
  int thing = open("./telephone.game", O_CREAT | O_TRUNC, 0666);
  key_t key = ftok("./telephone.game", 'R');
  int got = shmget(key, sizeof(int), 0644 | IPC_CREAT);
  int *ptr = shmat(got, 0, 0);
  int semd = semget(KEY, 1, IPC_CREAT); //semd is the semaphore descriptor
  semctl();
  return ptr;
}

void remove(int got)
{
  shmctl(got, IPC_RMID, NULL);
}
