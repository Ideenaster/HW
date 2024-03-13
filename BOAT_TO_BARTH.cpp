#include <queue>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include "DataStructure.h"
extern Commander Cmd;
extern int capacity;
using namespace std;

//���ڲ�λ֮���ת��ʱ����500֡ 
 
int berth_w[10]={1,-100,-1}; 
bool berth_cmp(Berth x,Berth y){//f=��ֵ+��������+ʱ�� 
	int fx=berth_w[0]*x.value+berth_w[1]*x.num+berth_w[2]*x.time;
	int fy=berth_w[0]*y.value+berth_w[1]*y.num+berth_w[2]*y.time;
	if(x.num==0) fx=-2e9;
	if(y.num==0) fy=-2e9;//�������Ϊ0������Ϊ-INF 
	return fx>fy;
}
 
const int FAST_BOAT=4;
const int MAXZHEN=14999;//����ʱ�� 
//��ѡ��λ 
void boat_to_berth(){
	sort(berth,berth+10,berth_cmp);//�޸��˴˴���berth_cmp����
	for(int i=0;i<FAST_BOAT;i++){//ǰ��FAST_BOAT����ֻҪ�л�������� 
	    if(boat[i].location==-2) continue;//-2��ʾ��ǰ������ʹ�� 
		if(boat[i].location==-1){
			for(int j=0;j<10;j++){
				if(!berth[j].is_choose){
					if(berth[j].num==0) break;
					berth[j].is_choose=true;
					Cmd.PushCommand(3,i,berth[j].id);//3��shipָ��,��һ�������Ǵ���id,�ڶ����ǲ�λ�ģ���ʾ���Ų�λ���� 
					boat[i].location=-2;
					break;
				}
			}                           
		}
		else{
			int pos0=0;//pos0��ʾ��ǰ���ڵĲ�λ��id��ǰ�������λ�� 
			for(int j=0;j<10;j++){
				if(boat[i].location==berth[j].id){
					pos0=j;
					break;
				}
			}
			int zhen=cur_frame.frame_id;
			if(zhen+berth[pos0].time==MAXZHEN){//��ǰ֡ǡ�ÿ��Է��������,ֱ�ӷ��� 
			    berth[pos0].is_choose=false;
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ������� 
				boat[i].location=-2;
				continue;
			}
			//��ǰ��λ���л��� 
			if(berth[pos0].num>0&&boat[i].num<capacity){
				int idx=berth[pos0].loading_speed;
				while(berth[pos0].num>0&&idx>0){
					idx--;
					auto good=berth[pos0].goods.front();//�ӻ��������ȡ������
					berth[pos0].goods.pop(); 
					berth[pos0].value-=good; 
					berth[pos0].num--;
					boat[i].value+=good;
					boat[i].num++;//ȡ���� 
				}
			}
			//��������
			if(boat[i].num==capacity){
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ�������
				berth[pos0].is_choose=false;
				boat[i].location=-2;
				continue;
			}
			if(berth[pos0].num>0){
				continue;
			} 
			//��ǰ��λ�Ѿ�ȡ����� 
			// int pos1;//pos1��ʾ��ǰ���ò�λ������ֵ��λ��
			for(int j=0;j<10;j++){
				if(berth[j].is_choose){//��λ�Ѿ���ѡ�� 
					continue;
				}
				if(berth[pos0].time+berth[j].time<=500){//�ӵ�ǰ��λ��������ٵ��ò�λ��ʱ�仹Ҫ�� 
					continue;
				}
				if(zhen+500+berth[j].time>MAXZHEN){//TLE;ʵ�ʻ��л���ʱ��,���MAXZHENҪ��ȥһ��ֵ 
					continue;
				} 
				//ʣ������ 
				int leftcapacity=max(0,capacity-boat[i].num);
				if(berth[j].num==0) continue;
				//����ط�һ������Ϊ0
				//��Ź�����ȡ�õļ�ֵ 
				int othervalue = min(berth[j].value*leftcapacity/berth[j].num,berth[j].value);
				//����Ĺ�ʽ���ɳ������
				//�����ǰ���ļ�ֵ���ϵ�ǰ��pos0����������ʱ��С�ڵ��ڵ�j��ļ�ֵ�ȶ���ʱ��
				//��ôѡ���berth 
				if(2*othervalue*berth[pos0].time>=boat[i].value*(500+berth[j].time-berth[pos0].time)){
					Cmd.PushCommand(3,i,berth[j].id);//3��shipָ��,��һ�������Ǵ���id,�ڶ����ǲ�λ�ģ���ʾ���Ų�λ����
					berth[j].is_choose=true;
					berth[pos0].is_choose=false;
					boat[i].location=-2;
					break;
				} 
			}
			//����������㲻֪�ã���ôֱ�ӷ�������� 
			if(berth[pos0].is_choose){
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ�������
				berth[pos0].is_choose=false;
				boat[i].location=-2;
			}
		} 
	} 
	//���涼������,�������������ֵ�Ļ�����FAST_BOAT=5�Ϳ����� 
	for(int i=FAST_BOAT;i<=4;i++){
		if(boat[i].location==-2) continue;
		if(boat[i].location==-1){
			for(int j=0;j<10;j++){
				if(!berth[j].is_choose&&(berth[j].value>=1000||2*berth[j].num>=capacity)){//����λ�ϵļ�ֵ����1000���߻����������ڵ���1/2()ʱ 
					berth[j].is_choose=true;
					Cmd.PushCommand(3,i,berth[j].id);//3��shipָ��,��һ�������Ǵ���id,�ڶ����ǲ�λ�ģ���ʾ���Ų�λ���� 
					boat[i].location=-2;//���ò�ƶ���??? 
					break;
				}
			}                           
		}
		else{
			int pos0=0;//pos0��ʾ��ǰ���ڵĲ�λ��id��ǰ�������λ�� 
			for(int j=0;j<10;j++){
				if(boat[i].location==berth[j].id){
					pos0=j;
					break;
				}
			}
			int zhen=cur_frame.frame_id;
			if(zhen+berth[pos0].time==MAXZHEN){//��ǰ֡ǡ�ÿ��Է��������,ֱ�ӷ��� 
			    berth[pos0].is_choose=false;
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ������� 
				boat[i].location=-2;
				continue;
			}
			//��ǰ��λ���л��� 
			if(berth[pos0].num>0&&boat[i].num<capacity){
				int idx=berth[pos0].loading_speed;
				while(berth[pos0].num>0&&idx>0){
					idx--;
					auto good=berth[pos0].goods.front();//�ӻ��������ȡ������
					berth[pos0].goods.pop(); 
					berth[pos0].value-=good; 
					berth[pos0].num--;
					boat[i].value+=good;
					boat[i].num++;//ȡ���� 
				}
			}
			if(boat[i].num==capacity){
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ�������
				berth[pos0].is_choose=false;
				boat[i].location=-2;
				continue;
			}
			if(berth[pos0].num!=0){
				continue;
			} 
			//��ǰ��λû�л����� 
			// int pos1;//pos1��ʾ��ǰ���ò�λ������ֵ��λ��
			for(int j=0;j<10;j++){
				if(berth[j].is_choose){//��λ�Ѿ���ѡ�� 
					continue;
				}
				if(berth[pos0].time+berth[j].time<=500){//�ӵ�ǰ��λ��������ٵ��ò�λ��ʱ�仹Ҫ�� 
					continue;
				}
				if(zhen+500+berth[j].time>MAXZHEN){//TLE;ʵ�ʻ��л���ʱ��,���MAXZHENҪ��ȥһ��ֵ 
					continue;
				} 
				//ʣ������ 
				int leftcapacity=max(0,capacity-boat[i].num);
				//��Ź�����ȡ�õļ�ֵ 
				if(berth[j].num==0) continue;
				int othervalue = min(berth[j].value*leftcapacity/berth[j].num,berth[j].value);
				//����Ĺ�ʽ���ɳ������
				//�����ǰ���ļ�ֵ���ϵ�ǰ��pos0����������ʱ��С�ڵ��ڵ�j��ļ�ֵ�ȶ���ʱ��
				//��ôѡ���berth 
				if(2*othervalue*berth[pos0].time>=boat[i].value*(500+berth[j].time-berth[pos0].time)){
					Cmd.PushCommand(3,i,berth[j].id);//3��shipָ��,��һ�������Ǵ���id,�ڶ����ǲ�λ�ģ���ʾ���Ų�λ����
					berth[j].is_choose=true;
					boat[i].location=-2;
					berth[pos0].is_choose=false;
					break;
				}
			}
			//����������㲻֪�ã���ôֱ�ӷ�������� 
			if(berth[pos0].is_choose){
				Cmd.PushCommand(4,i);//4��goָ���һ�������Ǵ���id����ʾ�������
				berth[pos0].is_choose=false;
				boat[i].location=-2;
			}
		} 
	}
}

