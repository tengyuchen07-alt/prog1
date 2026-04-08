#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;
int main()
{
    char s[4][4];
    memset(s, '_', sizeof(s)); // 把s填入 '_'
    
    const char *name = "chessboard";//共享記憶體的名字
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(s));
    void *ptr = mmap(0, sizeof(s), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    memcpy(ptr, s, sizeof(s));//把s複製到ptr指向的共享記憶體
    char (*shared_board)[4] = (char (*)[4]) ptr;
    if(ptr == MAP_FAILED){
        perror("mmap");
        return 1;
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
                printf("%c ", shared_board[i][j]);
            }
        printf("\n");
    }
    /*int p2c[2], c2p[2];
    if(pipe(p2c) == -1 || pipe(c2p) == -1){
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if(pid==-1){
        perror("fork");
        return 1;
    }
    switch(pid){
        case 0: //child
            close(p2c[1]);
            close(c2p[0]);
            
        default:
            close(p2c[0]);
            close(c2p[1]);
    }*/
}