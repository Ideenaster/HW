#include <queue>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include "DataStructure.h"
extern Commander Cmd;
extern int capacity;
using namespace std;

//船在泊位之间的转移时间是500帧 
 
int berth_w[10]={1,-100,-1}; 
inline bool berth_cmp(Berth x,Berth y){//f=价值+货物数量+时间 
	int fx=berth_w[0]*x.value+berth_w[1]*x.num+berth_w[2]*x.time;
	int fy=berth_w[0]*y.value+berth_w[1]*y.num+berth_w[2]*y.time;
	if(x.num==0) fx=-2e9;
	if(y.num==0) fy=-2e9;//如果数量为0，设置为-INF 
	return fx>fy;
}
 
const int FAST_BOAT=4;
const int MAXZHEN=14999;//最后的时间 
//船选择泊位 
inline void boat_to_berth(){
	sort(berth,berth+10,berth_cmp);//修改了此处的berth_cmp函数
	for(int i=0;i<FAST_BOAT;i++){//前面FAST_BOAT条船只要有货物就运行 
	    if(boat[i].location==-2) continue;//-2表示当前船不可使用 
		if(boat[i].location==-1){
			for(int j=0;j<10;j++){
				if(!berth[j].is_choose){
					berth[j].is_choose=true;
					Cmd.PushCommand(3,i,berth[j].id);//3是ship指令,第一个参数是船的id,第二个是泊位的，表示向着泊位进行 
					boat[i].location=-2;
					break;
				}
			}                           
		}
		else{
			int pos0=0;//pos0表示当前船在的泊位的id当前在数组的位置 
			for(int j=0;j<10;j++){
				if(boat[i].location==berth[j].id){
					pos0=j;
					break;
				}
			}
			int zhen=cur_frame.frame_id;
			if(zhen+berth[pos0].time==MAXZHEN){//当前帧恰好可以返回虚拟点,直接返回 
			    berth[pos0].is_choose=false;
				Cmd.PushCommand(4,i);//4是go指令，第一个参数是船的id，表示回虚拟点 
				continue;
			}
			//当前泊位还有货物 
			if(berth[pos0].num>0&&boat[i].num<capacity){
				int idx=berth[pos0].velocity;
				while(berth[pos0].num>0&&idx>0){
					idx--;
					auto good=berth[pos0].goods.front();//从货物队列中取出货物
					berth[pos0].goods.pop(); 
					berth[pos0].value-=good; 
					berth[pos0].num--;
					boat[i].value+=good;
					boat[i].num++;//取货物 
				}
			}
			if(berth[pos0].num!=0){
				continue;
			} 
			//当前泊位已经取完货物 
			// int pos1;//pos1表示当前可用泊位的最大价值的位置
			for(int j=0;j<10;j++){
				if(berth[j].is_choose){//泊位已经被选择 
					continue;
				}
				if(berth[pos0].time+berth[j].time<=500){//从当前泊位回虚拟点再到该泊位的时间还要短 
					continue;
				}
				if(zhen+500+berth[j].time>MAXZHEN){//TLE;实际还有货物时间,这个MAXZHEN要减去一定值 
					continue;
				} 
				//剩余容量 
				int leftcapacity=max(0,capacity-boat[i].num);
				//大概估计能取得的价值 
				int othervalue = min(berth[j].value*leftcapacity/berth[j].num,berth[j].value);
				//这里的公式是由除法变的
				//如果当前船的价值比上当前点pos0返回虚拟点的时间小于等于到j点的价值比额外时间
				//那么选择该berth 
				if(2*othervalue*berth[pos0].time>=boat[i].value*(500+berth[j].time-berth[pos0].time)){
					Cmd.PushCommand(3,i,berth[j].id);//3是ship指令,第一个参数是船的id,第二个是泊位的，表示向着泊位进行
					berth[j].is_choose=true;
					berth[pos0].is_choose=false;
					break;
				}
			}
			//如果到其他点不知得，那么直接返回虚拟点 
			if(berth[pos0].is_choose){
				Cmd.PushCommand(4,i);//4是go指令，第一个参数是船的id，表示回虚拟点
				berth[pos0].is_choose=false;
			}
		} 
	} 
	//后面都是慢船,如果发现慢船不值的话可以FAST_BOAT=5就可以了 
	for(int i=FAST_BOAT;i<=4;i++){
		if(boat[i].location==-2) continue;
		if(boat[i].location==-1){
			for(int j=0;j<10;j++){
				if(!berth[j].is_choose&&(berth[j].value>=1000||2*berth[j].num>=capacity)){//当泊位上的价值大于1000或者货物数量大于等于1/2()时 
					berth[j].is_choose=true;
					Cmd.PushCommand(3,i,berth[j].id);//3是ship指令,第一个参数是船的id,第二个是泊位的，表示向着泊位进行 
					boat[i].location=-2;//这个貌似多余??? 
					break;
				}
			}                           
		}
		else{
			int pos0=0;//pos0表示当前船在的泊位的id当前在数组的位置 
			for(int j=0;j<10;j++){
				if(boat[i].location==berth[j].id){
					pos0=j;
					break;
				}
			}
			int zhen=cur_frame.frame_id;
			if(zhen+berth[pos0].time==MAXZHEN){//当前帧恰好可以返回虚拟点,直接返回 
			    berth[pos0].is_choose=false;
				Cmd.PushCommand(4,i);//4是go指令，第一个参数是船的id，表示回虚拟点 
				continue;
			}
			//当前泊位还有货物 
			if(berth[pos0].num>0&&boat[i].num<capacity){
				int idx=berth[pos0].velocity;
				while(berth[pos0].num>0&&idx>0){
					idx--;
					auto good=berth[pos0].goods.front();//从货物队列中取出货物
					berth[pos0].goods.pop(); 
					berth[pos0].value-=good; 
					berth[pos0].num--;
					boat[i].value+=good;
					boat[i].num++;//取货物 
				}
			}
			if(berth[pos0].num!=0){
				continue;
			} 
			//当前泊位没有货物了 
			// int pos1;//pos1表示当前可用泊位的最大价值的位置
			for(int j=0;j<10;j++){
				if(berth[j].is_choose){//泊位已经被选择 
					continue;
				}
				if(berth[pos0].time+berth[j].time<=500){//从当前泊位回虚拟点再到该泊位的时间还要短 
					continue;
				}
				if(zhen+500+berth[j].time>MAXZHEN){//TLE;实际还有货物时间,这个MAXZHEN要减去一定值 
					continue;
				} 
				//剩余容量 
				int leftcapacity=max(0,capacity-boat[i].num);
				//大概估计能取得的价值 
				int othervalue = min(berth[j].value*leftcapacity/berth[j].num,berth[j].value);
				//这里的公式是由除法变的
				//如果当前船的价值比上当前点pos0返回虚拟点的时间小于等于到j点的价值比额外时间
				//那么选择该berth 
				if(2*othervalue*berth[pos0].time>=boat[i].value*(500+berth[j].time-berth[pos0].time)){
					Cmd.PushCommand(3,i,berth[j].id);//3是ship指令,第一个参数是船的id,第二个是泊位的，表示向着泊位进行
					berth[j].is_choose=true;
					berth[pos0].is_choose=false;
					break;
				}
			}
			//如果到其他点不知得，那么直接返回虚拟点 
			if(berth[pos0].is_choose){
				Cmd.PushCommand(4,i);//4是go指令，第一个参数是船的id，表示回虚拟点
				berth[pos0].is_choose=false;
			}
		} 
	}
}
