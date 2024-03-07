#include <bits/stdc++.h>
#include "DataStructure.h"
using namespace std;
void Init()
{
    for(int i = 1; i <= n; i ++)
        scanf("%s", Map[i] + 1);        // 读入地图
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[5];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &id, &money);         //id为判题器返回的帧编号，money为当前金币数
    int num;                            //num为新增的货物数量
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);  //各货物的坐标和价值
    }
    for(int i = 0; i < robot_num; i ++)
    {
        int sts;                        //goods为机器人当前是否携带货物，x和y为机器人当前位置
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &sts);
    }                                   //sts为当前机器人状态，0为恢复状态（发生碰撞），1为正常状态
    for(int i = 0; i < 5; i ++)
        scanf("%d%d\n", &boat[i].status, &boat[i].pos);
    char okk[5];                        //输入运货船的各项状态
    scanf("%s", okk);
    return id;
}

int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input();

        puts("OK");
        fflush(stdout);
    }

    return 0;
}
