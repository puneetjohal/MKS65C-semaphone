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
  int thing = open("./telephone.game", O_RDONLY, 0666); //open file
  char *story = malloc(1024 * sizeof(char)); //str to store last line
  int got = shmget(KEY, sizeof(int), 0644 | IPC_CREAT); //shared memory
  int *ptr = shmat(got, 0, 0); //shared memory pointer
  int offset = *ptr * -1 * sizeof(char) - 1; //offset from the end of the file (the end of the file has a space)
  lseek(thing, offset, SEEK_END); //move to the beginning of the last line
  read(thing, story, *ptr); //read it into the string story
  close(thing); //close the file
  printf("%s\n", story); //print the last line
  free(story); //free the pointer
}
char * prompt()
{
  char * line = malloc(1024 * sizeof(char)); //to store user input
  char * junk = malloc(sizeof(char)); //to store the newline
  printf("Please enter your line here (max 1023 characters): "); //a line of a story shouldn't take more than that
  fscanf(stdin, "%[^\n]s", line); //scan input until the newline
  fscanf(stdin, "%c", junk); //scan the newline
  return line; //return user input
}
void write_line(char * line)
{
  int thing = open("./telephone.game", O_WRONLY | O_APPEND, 0666); //open the file in append mode
  char * space; //adding a space to keep story formatted nicely
  *space = ' ';
  strncat(line, space, 1); //adding the space to the line
  write(thing, line, strlen(line) * sizeof(char)); //write to the file
  close(thing); //close the file
}
void update(int len)
{
  int got = shmget(KEY, sizeof(int), 0644); //accessing shared memory
  int *ptr = shmat(got, 0, 0);
  *ptr = len; //the length of the last input
}

int main()
{
  printf("I will check if resources are available.\n");
  //checking if resources are available
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
  sb->sem_op = 1;
  sb->sem_num = 0;
  sb->sem_flg = IPC_NOWAIT;
  semop(semd, sb, 1);
  printf("Thank you for playing semaphone.\n");
  free(line);
  return 0;
}
