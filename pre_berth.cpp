#include "DataStructure.h"
#include "ctime"
extern clock_t sta;

unsigned short int berth_dist[10][200][200];
bool visited[200][200];
int dx[4]={0,0,-1,1},dy[4]={1,-1,0,0};

int get_robot_to_berth_dist(int id,int x,int y){
	int res=berth_dist[id][x][y];
    return res;
}

void pre_berth(Berth berth[]){
	for(int i=0;i<=9;i++){
		int x=berth[i].x,y=berth[i].y;
		memset(visited,false,sizeof visited);
		unsigned short int start = 0;
		std::queue<std::pair<unsigned short int,std::pair<int,int>>> q;
		visited[x][y]=true;
		q.push({start,{x,y}});
		while(q.size()){
			auto t=q.front();
			q.pop();
			unsigned short int predist=t.first;
			int prex=t.second.first,prey=t.second.second;
			berth_dist[i][prex][prey]=predist;
			for(int i=0;i<4;i++){
				int newx=prex+dx[i],newy=prey+dy[i];
				if(newx<0||newx>=200||newy<0||newy>=200||visited[newx][newy]) continue;
				visited[newx][newy]=true;
				q.push({predist+1,{newx,newy}});
			}
		}
	}
	printf("OK\n");
    fflush(stdout);
	sta = clock();
}
