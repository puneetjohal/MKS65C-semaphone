#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char*argv[])
{
  printf("okay, I'm gonna check if resources are available\n");

  if(!strcmp(argv[1], "-c")){
    int thing = open("./telephone.game", O_CREAT | O_TRUNC, 0666);
    key_t key = ftok("./telephone.game", 'R');
    int got = shmget(key, 305000 * sizeof(char), 0644 | IPC_CREAT);
    char *junk = malloc(sizeof(char));
    char *ptr = shmat(got, 0, 0);
    char *line = malloc(1024 * sizeof(char));

    printf("okay then how do you wanna start the story?\n");
    //get the input
    fscanf(stdin, "%[^\n]s", line);
    fscanf(stdin, "%c", junk);

    //if the first letter is n, it's probably a no
    if(!strncmp(line, "n", 1) || !strncmp(line, "N", 1))
      printf("okay, i won't change anything.\n");

    //if the first letter is y, it's probably a yes
    else if(!strncmp(line, "y", 1) || !strncmp(line, "Y", 1))
      {
	printf("okay, what do you want in the string?\n");
	//take user input
	fscanf(stdin, "%[^\n]s", line);
	fscanf(stdin, "%c", junk);
	//put it in the shared memory
	strncpy(ptr, line, 200);
	//diagnostic print statement
	//printf("now the memory has this in it: %s\n", ptr);
      }
    else
      printf("...i'll just take that as a no.\n");
  
    printf("do you want to delete the segment(y/n)?\n");

    fscanf(stdin, "%[^\n]s", line);
    fscanf(stdin, "%c", junk);

    if(!strncmp(line, "n", 1) || !strncmp(line, "N", 1))
      printf("okay, it stays.\n");

    else if(!strncmp(line, "y", 1) || !strncmp(line, "Y", 1))
      {
	shmctl(got, IPC_RMID, NULL);
	printf("okay, it's gone.\n");
      }
  
    else
      printf("...i'll just take that as a no.\n");
    free(line);
    free(junk);
    return 0;
}
