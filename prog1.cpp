#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <random>
#include <time.h>
#include <sys/wait.h>

using namespace std;
bool play(char board[4][4], int rand_num, char symbol){
    return 0;
}
int main()
{
    char s[4][4];
    memset(s, '_', sizeof(s)); // 把s填入 '_'
    
    const char *name = "chessboard";//共享記憶體的名字
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(s));
    void *ptr = mmap(0, sizeof(s), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    memcpy(ptr, s, sizeof(s));//把s複製到ptr指向的共享記憶體
    char (*shared_board)[4] = (char (*)[4]) ptr;//把ptr轉換成二維陣列的指標

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
    int p2c[2], c2p[2];
    if(pipe(p2c) == -1 || pipe(c2p) == -1){
        perror("pipe");
        return 1;
    }

    int p,c;
    cin>>p>>c;

    pid_t pid = fork();

    switch(pid){
        case -1:
            perror("fork");
            return 1;
        case 0:{ //child
            srand(c);
            int random_c=rand();//隨機數

            char baton;

            close(p2c[1]);
            close(c2p[0]);
            while(play(shared_board, random_c, 'C')){
                read(p2c[0], &baton, 1);
                write(c2p[1], "T", 1);
            }
            close(p2c[0]);
            close(c2p[1]);
            exit(0);
        }
        default:{
            cout<<'['<<getpid()<<"Parent]:create a child "<<pid<<endl;
            srand(p);
            int random_p=rand();

            char baton;
            
            close(p2c[0]);
            close(c2p[1]);
            while(play(shared_board, random_p, 'P')){
                write(p2c[1], "T", 1);
                read(c2p[0], &baton, 1);
            }
            close(p2c[1]);
            close(c2p[0]);
            wait(NULL);
            break;
        }
    }
     return 0;
}