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

void displayBoard(char board[4][4], char msg,int x,int y){
    const char* identity = (msg == 'P') ? "Parent" : "Child";
    printf("[%d] %s plays at (%d, %d)\n", getpid(), identity, x, y);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
                printf("%c ", board[i][j]);
            }
        printf("\n");
    }
    fflush(stdout); // 確保內容立刻印出，不卡在緩衝區
}

void play(char board[4][4], int rand_num, char symbol){
    int x=rand_num%4;
    int y=(rand_num/4)%4;
    while(board[x][y]!='_'){
        rand_num=rand();
        x=rand_num%4;
        y=(rand_num/4)%4;
    }
    board[x][y]=symbol;
    displayBoard(board,symbol, x, y);//顯示棋盤
}

bool isWin(char board[4][4]){
    for(int i=0;i<4;i++){//橫排
        for(int j=0;j<2;j++){
            if(board[i][j]==board[i][j+1] && board[i][j+1]==board[i][j+2] && board[i][j]!='_'){
                return true;
            }
        }
    }
    for(int j=0;j<4;j++){//直排
        for(int i=0;i<2;i++){
            if(board[i][j]==board[i+1][j] && board[i+1][j]==board[i+2][j] && board[i][j]!='_'){
                return true;
            }
        }
    }
    for(int i=0;i<2;i++){//左上到右下
        for(int j=0;j<2;j++){
            if(board[i][j]==board[i+1][j+1] && board[i+1][j+1]==board[i+2][j+2] && board[i][j]!='_'){
                return true;
            }
        }
    }
    for(int i=0;i<2;i++){//右上到左下
        for(int j=2;j<4;j++){
            if(board[i][j]==board[i+1][j-1] && board[i+1][j-1]==board[i+2][j-2] && board[i][j]!='_'){
                return true;
            }
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    int m1=atoi(argv[1]);
    int m2=atoi(argv[2]);
    if(m1<1||m1>10||m2<1||m2>10){
        cout<<"Please enter a number between 1 and 10."<<endl;
        return 1;
    }

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

    int p2c[2], c2p[2];
    if(pipe(p2c) == -1 || pipe(c2p) == -1){
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    switch(pid){
        case -1:
            perror("fork");
            return 1;
        case 0:{ //child
            srand(m2);
            int random_c=rand();//隨機數

            char baton;

            close(p2c[1]);
            close(c2p[0]);
            for(int i=0;i<8;i++){
                read(p2c[0], &baton, 1);
                play(shared_board, random_c, 'C');
                if(isWin(shared_board)){
                    cout<<"Child wins!"<<endl;
                    break;
                }
                write(c2p[1], "T", 1);
            }
            close(p2c[0]);
            close(c2p[1]);
            exit(0);
        }
        default:{
            cout<<'['<<getpid()<<"Parent]:create a child "<<pid<<endl;
            srand(m1);
            int random_p=rand();

            char baton;
            
            close(p2c[0]);
            close(c2p[1]);
           for(int i=0;i<8;i++){
                play(shared_board, random_p, 'P');
                if(isWin(shared_board)){
                    cout<<"Parent wins!"<<endl;
                    wait(NULL);
                    break;
                }
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