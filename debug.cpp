// #include "std_msgs/UInt8MultiArray.h"
// #include "ros/ros.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// 参考链接
// 源码 https://github.com/javierdiazp/myros/blob/7571febdfa881872cae6378bf7266deca7901529/indigo/include/std_msgs/UInt8MultiArray.h
// https://github.com/ros-industrial/siemens_tutorials/blob/4e7ff34c7864d6a3b3223ef8f51fe084acfaddea/siemens_cp1616_io_device_tutorial/doc/doc_siemens_cp1616_io_device_tutorial_node.cpp
void PrintROSUint8MultiArray(std_msgs::UInt8MultiArray Data,int Size,int RorW){
	char tmp[2000];
	memset(tmp,0,sizeof tmp);
	if(RorW==0){
		strcat(tmp,"Read:");
	}
	else strcat(tmp,"Write:");
	for(int i=0;i<Data.data.size();++i){
		char myhex[10];
		sprintf(myhex," 0x%02x",Data.data[i]);
		strcat(tmp,myhex);
	}
	ROS_INFO("%s",tmp);
}
int main(){
	
	std_msgs::UInt8MultiArray val;
	val.data.push_back(0xF1);
	val.data.push_back(0xF2);
	val.data.push_back(0xF3);
	val.data.push_back(0xF4);
	PrintROSUint8MultiArray(val,4,0);
	return 0;
}
