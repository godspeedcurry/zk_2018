/*
steps:
1.start
2.open the serial port
3.initialize
4.read or write
5.close the serial port
6.end
 */
#include<termios.h>  
#include<unistd.h>
#include <bits/stdc++.h>
using namespace std;
#include<fcntl.h>
#define ERROR -1
/*
过程

调用终端
|
获取串口状态
|
测试是否为串口终端

必选
O_RDONLY只读模式
O_WRONLY只写模式
O_RDWR读写模式

可选
O_APPEND每次写操作都写入文件的末尾
O_CREAT如果指定文件不存在，则创建这个文件
O_EXCL如果要创建的文件已存在，则返回 -1，并且修改 errno 的值
O_TRUNC如果文件存在，并且以只写/读写方式打开，则清空文件全部内容
O_NOCTTY如果路径名指向终端设备，不要把这个设备用作控制终端。
O_NONBLOCK如果路径名指向 FIFO/块文件/字符文件，则把文件的打开和后继 I/O设置为非阻塞模式（nonblocking mode）

可选
O_DSYNC等待物理 I/O 结束后再 write。在不影响读取新写入的数据的前提下，不等待文件属性更新。
O_RSYNC读(read)等待所有写入同一区域的写操作完成后再进行
O_SYNC等待物理 I/O 结束后再 write，包括更新文件属性的 I/O
 */
int open_port(){
	int fd;
	fd = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);//O_NONBLOCK设置为非阻塞模式，在read时不会阻塞住，在读的时候将read放在while循环中
	if(fd==-1){
		perror("Can't open the serial port");
	}
	return fd;
}
/*
更加健壮的打开版本
 */
int open_port(int fd,int comport){
        char *dev[]={"/dev/ttyUSB0","/dev/ttyS1","/dev/ttyS2"};
	if (comport==1){//串口1
		fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd){ 
			perror("Can't Open Serial Port"); 
			return ERROR; 
		} 
	} 
	else if(comport==2){//串口2 
	fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd){ 
			perror("Can't Open Serial Port"); 
			return ERROR; 
		} 
	} 
    else if (comport==3){//串口3 
		fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY); 
		if (-1 == fd){ 
			perror("Can't Open Serial Port"); 
			return ERROR; 
		} 
	} 
     /*恢复串口为阻塞状态*/ 
	if(fcntl(fd, F_SETFL, 0)<0) 
		printf("fcntl failed!\n"); 
	else 
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0)); 
     /*测试是否为终端设备*/ 
	if(isatty(STDIN_FILENO)==0) 
		printf("standard input is not a terminal device\n"); 
	else 
		printf("isatty success!\n"); 
	printf("fd-open=%d\n",fd); 
	return fd; 
}
/*

串口初始化
设置波特率
|
设置数据流控制
|
设置帧的格式
 */
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) { 
	struct termios newtio,oldtio; 
/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/ 
	if(tcgetattr( fd,&oldtio)!=0){
		perror("SetupSerial 1");
		printf("tcgetattr( fd,&oldtio) -> %d\n",tcgetattr( fd,&oldtio)); 
		return -1; 
	} 
 	bzero( &newtio, sizeof( newtio ) ); 
/*步骤一，设置字符大小*/ 
	newtio.c_cflag |= CLOCAL|CREAD;
	newtio.c_cflag &= ~CSIZE;  
/*设置停止位*/ 
	switch( nBits ) { 
		case 7: 
			newtio.c_cflag |= CS7; 
			break; 
		case 8: 
			newtio.c_cflag |= CS8; 
			break; 
	} 
/*设置奇偶校验位*/ 
	switch( nEvent ) { 
		case 'o':
		case 'O': //奇数 
		    newtio.c_cflag |= PARENB; 
		    newtio.c_cflag |= PARODD; 
		    newtio.c_iflag |= (INPCK | ISTRIP); 
		    break; 
    	case 'e':
    	case 'E': //偶数 
		    newtio.c_iflag |= (INPCK | ISTRIP); 
		    newtio.c_cflag |= PARENB; 
		    newtio.c_cflag &= ~PARODD; 
		    break;
    	case 'n':
    	case 'N':  //无奇偶校验位 
		    newtio.c_cflag &= ~PARENB; 
		    break;
   		default:
			break;
	} 
/*设置波特率*/ 
	switch(nSpeed){ 
		case 2400: 
			cfsetispeed(&newtio, B2400); 
			cfsetospeed(&newtio, B2400); 
			break; 
		case 4800: 
			cfsetispeed(&newtio, B4800); 
			cfsetospeed(&newtio, B4800); 
			break; 
		case 9600: 
			cfsetispeed(&newtio, B9600); 
			cfsetospeed(&newtio, B9600); 
			break; 
		case 115200: 
			cfsetispeed(&newtio, B115200); 
			cfsetospeed(&newtio, B115200); 
			break; 
		case 460800: 
			cfsetispeed(&newtio, B460800); 
			cfsetospeed(&newtio, B460800); 
			break; 
		default: 
			cfsetispeed(&newtio, B9600); 
			cfsetospeed(&newtio, B9600); 
			break; 
	} 
  /*设置停止位*/ 
	if( nStop == 1 ) 
		newtio.c_cflag &=  ~CSTOPB; 
	else if ( nStop == 2 ) 
		newtio.c_cflag |=  CSTOPB; 
  /*设置等待时间和最小接收字符*/ 
	newtio.c_cc[VTIME]  = 0; 
	newtio.c_cc[VMIN] = 0; 
	  /*处理未接收字符*/ 
	tcflush(fd,TCIFLUSH); 
	  /*激活新配置*/ 
	if((tcsetattr(fd,TCSANOW,&newtio))!=0) { 
		perror("com set error"); 
		return -1; 
	} 
	printf("set done!\n"); 
	return 0; 
} 


int main(){



	return 0;
}
