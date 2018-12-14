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
  int val; //used for SETVAL
  struct semid_ds *buf; //used for IPC_STAT and IPC_SET
  unsigned short *array; //used for SETALL
  struct seminfo *__buf;
};
			
void display_last_line()
{
  printf("here is the previous line:\n");
  int thing = open("./telephone.game", O_RDONLY, 0666);
  char *story = malloc(300000 * sizeof(char));
  int got = shmget(KEY, sizeof(int), 0644 | IPC_CREAT); 
  int *ptr = shmat(got, 0, 0);
  lseek(thing, *ptr, SEEK_END);
  read(thing, story, *ptr);
  close(thing);
  printf("%s\n", story);
  free(story);
  //perror("ERROR");
}
char * prompt()
{
  char * line = malloc(1024 * sizeof(char));
  char * junk = malloc(sizeof(char));
  printf("Please enter your line here (max 1023 characters): ");
  fscanf(stdin, "%[^\n]s", line);
  fscanf(stdin, "%c", junk);
  return line;
}
void write_line(char * line)
{
  int thing = open("./telephone.game", O_WRONLY | O_APPEND, 0666);
  write(thing, line, 1024 * sizeof(char));
  close(thing);
  //perror("Error");
}
void update(int len)
{
  int got = shmget(KEY, sizeof(int), 0644);
  int *ptr = shmat(got, 0, 0);
  *ptr = len;
}
void release(){}
int main(){
  int semd = semget(KEY, 1, 0);
  struct sembuf *sb;
  sb->sem_op = -1;
  sb->sem_num = 0;
  sb->sem_flg = IPC_NOWAIT;
  semop(semd, sb, 1);
  //display the last line added to the file
  display_last_line();
  //prompt the user for the next line
  char * line = prompt();
  //count length of next line
  int len = strlen(line);
  //writing the line to the file
  write_line(line);
  //updating the shared memory with the size of the last line
  update(len);
  //releasing the semaphore
  release();
  free(line);
  sb->sem_op = 1;
  sb->sem_num = 0;
  sb->sem_flg = IPC_NOWAIT;
  semop(semd, sb, 1);
  return 0;
}
