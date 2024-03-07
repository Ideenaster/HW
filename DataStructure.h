#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

const int n = 200;       // 地图大小
const int robot_num = 10;// 机器人数量
const int berth_num = 10;// 泊位数量
const int N = 210;      

struct Robot
{
    int x, y, goods;
    int status;
    int mbx, mby;
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
}robot[robot_num + 10];

struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
}berth[berth_num + 10];



struct Boat
{
    int num, pos, status;
}boat[10];

int money, boat_capacity, id;
char Map[N][N];
/*
‘.’ ： 空地
‘*’ ： 海洋
‘#’ ： 障碍
‘A’ ： 机器人起始位置，总共 10 个。
‘B’ ： 大小为 4*4，表示泊位的位置,泊位标号在后泊位处初始化。
*/
int gds[N][N];



#endif // DATASTRUCTURE_H