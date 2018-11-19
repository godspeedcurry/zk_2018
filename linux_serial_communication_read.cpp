//读串口
#include <unistd.h>
#include <bits/stdc++.h>
/*
fd 文件描述符
*buf 缓冲区 读取的数据会被放到这个缓冲区中
count 请求读取的字节数   读上来的数据保存在缓冲区buf中
同时文件的当前读写位置向后移动
*/
/*
read 默认为阻塞模式若在open操作中设置O_NONBLOCK则是非阻塞模式

阻塞模式读到数据会阻塞
非阻塞返回-1    
*/
/*ssize_t read(int fd, void *buf, size_t count){

}
ssize_t write(int fd, void *buf, size_t count){
}
*/
int main(){
    char buff[10];
    int nread = read(STDIN_FILENO, buff, 8);
    write(STDOUT_FILENO,buff,nread);
    printf("nread = %d, %s",nread,buff);
    printf("%lld\n",strlen(buff));
    return 0;
}
