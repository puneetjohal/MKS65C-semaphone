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
#include <errno.h>

#define KEY 0xFADEDDAB

union semun {
  int val; /* Value for SETVAL */
  struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO */
};

void create()
{
  int r;
  int v;
  int thing = open("./telephone.game", O_CREAT | O_TRUNC, 0666); //making file
  key_t key = ftok("./telephone.game", 'R');
  int got = shmget(key, sizeof(int), 0644 | IPC_CREAT); //make shared memory
  int *ptr = shmat(got, 0, 0);
  int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644); //semd is the semaphore descriptor (make semaphore)
  if (semd == -1)
    printf("error %d: %s\n", errno, strerror(errno));
  else
    {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
      //printf("semctl returned: %d\n", r);
    }
}

void flag_remove()
{
  int r;
  int thing = open("./telephone.game", O_RDONLY, 0666);
  key_t key = ftok("./telephone.game", 'R');
  int got = shmget(key, sizeof(int), 0644 | IPC_CREAT);
  shmctl(got, IPC_RMID, NULL); //remove shared memory
  int semd = semget(KEY, 1, 0644); //semd is the semaphore descriptor
  union semun us;
  r = semctl(semd, 0, IPC_RMID, us); //remove semaphore
  char *story = malloc(300000 * sizeof(char));
  read(thing, story, 300000 * sizeof(char));
  remove("./telephone.game");
  printf("%s\n", story);
}
void view()
{

}
int main(int argc, char *argv[])
{
  if(argc < 2)
    printf("Error: Please type -c to create, -r to remove, or -v to view.\n");
  else if(!strcmp(argv[1], "-c"))
      create();
  else if(!strcmp(argv[1], "-r"))
      flag_remove();
  else if(!strcmp(argv[1], "-v"))
      view();
  return 0;
}
