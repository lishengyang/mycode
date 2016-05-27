#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define MAX_SIZE 32
#define END_PATH  "/home/pi/file.txt"
//#define END_PATH  "/home/pi/1.txt"
int main(void) 
{
    int fd;
    double temp = 0;
    char buf[MAX_SIZE];
    char command[256] = {0};
    float  val ;
    int fd1;
    ssize_t nrd;
    // ../sys/class/thermal/thermal_zone0/temp
    fd = open(TEMP_PATH, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open thermal_zone0/temp\n");
        return -1;
    }
    
    // ....
   // if (read(fd, buf, MAX_SIZE) < 0) {
    //    fprintf(stderr, "failed to read temp\n");
    //    return -1;
   // }
   fd1 = open(END_PATH, O_CREAT | O_WRONLY, S_IRUSR);
    if (fd1 < 0 ) {
       fprintf(stderr,"file not exist\n");
    }
    while(nrd = read(fd,buf,MAX_SIZE)){
        write(fd1,buf,nrd);
    }
    
    
    // ........
    temp = atoi(buf) / 1000.0;
    printf("temp: %.2f\n",temp);
    //val = (float)temp; 
    //snprintf(command, 256, "echo  %.2f  >> /home/pi/file.txt", val);
    //system(command);
       
    // ....
    close(fd);
    close(fd1);
}

