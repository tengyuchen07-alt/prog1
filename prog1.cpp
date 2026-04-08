#include <iostream>
#include <unistd.h>
using namespace std;
int main()
{
    int p2c[2], c2p[2];
    if(pipe(p2c) == -1 || pipe(c2p) == -1)
    {
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
            while(true){

            }
        default:
            close(p2c[0]);
            close(c2p[1]);
    }
}