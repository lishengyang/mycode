#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main(void)    
  {    
    int curr_val=60;
char command[256];
snprintf(command, 256, "echo -n %d > /home/pi/file.txt", curr_val);
system(command);
  }

