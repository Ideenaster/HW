#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include "DataStructure.h"
#include "pre_berth.cpp"
using namespace std;
//计算一个曼哈顿距离
int getdist(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}
//返回机器人和货物之间的距离1
int calculate_dist(int id, hw &hw)
{
    return getdist(robot[id].x, robot[id].y, hw.x, hw.y);
}
//返回货物的价值
int calculate_value(hw &hw)
{
    return hw.value;
}
//计算剩余的时间
int calculate_life_left(hw &hw)
{
    return hw.life;
}
//初步选货物。只考虑距离因素
vector<hw> robot_hw(int id)
{
    vector<hw> to_get;
    for (hw &i : cur_frame.hws)
    {
    	//这个距离是估计距离，实际距离会高于该距离
		//如果距离大于80即不可达，如果大于存货时间不可达 
        if (calculate_dist(id, i) < 40000 && calculate_life_left(i) > calculate_dist(id,i))
            to_get.push_back(i);
        // printf("life: %d dist: %d,to_get.size: %d\n",calculate_life_left(rot, i, cur_frame),calculate_dist(rot,i),to_get.size());
    }
    return to_get; //待选集合 考虑距离
}
//更新全局变量 cur_frame.hws
void remove_hw(hw &to_remove)
{
    auto it = find(cur_frame.hws.begin(), cur_frame.hws.end(), to_remove);
    if (it != cur_frame.hws.end())
    {
        cur_frame.hws.erase(it);
    }
}

hw &find_hw(int id, vector<hw> &to_get, hw_frame &cur_frame)
{
    int max_score = INT_MIN;
    hw *target = nullptr;
    for (hw &hw : to_get)
    {
    	//因为距离是估计的,当前参数为0
		//第二个是货物的价值
		//第三个是货物剩余时间，先消失的货物先拿 
        int fx = 0* calculate_dist(id, hw) + 10 * calculate_value(hw) - 2 * calculate_life_left(hw);
        if (fx > max_score)
        {
            max_score = fx;
            target = &hw;
        }
    }
    //得到当前分数最高的货物 
    return *target;
}
//如果robot.status==1，用astar找货物
//如果robot.status==3,先执行get，在执行用astar找泊位，在当前瞬间还需要
//如果robot.status==5,先制性pull，在执行astar去找货物
void go_work(int id)
{
    //空闲状态就跑去找货物，然后就run astar，找到目标开始work
    vector<hw> to_get = robot_hw(id);
    // printf("to_get: %d\n",to_get.size());
    if (!to_get.empty()){
        hw &target = find_hw(id, to_get, cur_frame);
        remove_hw(target); 
        robot[id].target_hw = &target;  //赋值robot类的属性
        robot[id].Tx = target.x;
        robot[id].Ty = target.y;    //0:空闲  1:寻货物路中 2:寻货物完毕1
                                // 3:已经到达货物目标位置
                                // 4:寻找泊位路中
                                // 5:到达泊位
                                //6：寻船舶完毕
        robot[id].status=2;//告诉他应该需要用astar
    }
} 

int find_best_berth(int id)
{   
    
    int index = -1;
    int min_pay = 2e9;
    for (int i = 0; i < 10; ++i)
    {   
       //暂时只有泊位到虚拟点的时间以及当前机器人到泊位的时间 
       int dist= get_robot_to_berth_dist(berth[i].id,robot[id].x,robot[id].y);
       int time=berth[i].time;
       int pay=1*dist+1*time;
       if(pay<min_pay) min_pay=pay,index=i;
        // 1泊位到虚拟点的时间 0装载速度 0货物数量 0是否被选择 1机器人到泊位的距离
        /* int dist = getdist(rot.x, rot.y, berth[i].x, berth[i].y);
        if (dist < min_dist)
        {
            min_dist = dist;
            index = i;
        } */
    }
    return index; //返回最近泊位的下标
}
// 如果robot.status==3 ,需要执行一次 get指令
//如果 robot.status==5,需要执行pull指令,执行完之后还要改变状态
void go_berth(int id){
    
    int target_index = find_best_berth(id);//匹配最优先船舶中 
    Berth *target_berth = &berth[target_index];
    robot[id].Tx = target_berth->x;
    robot[id].Ty = target_berth->y;
    robot[id].status=6;//告诉他应该使用astar寻船舶
   /*  target_berth->target_robots.push_back(i);  以该泊位为目标的机器人数组，但是没有清空*/
}
/* int count_nearby_robots(Robot &rot, Robot robots[10])
{
    int count = 0;
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(5, vector<bool>(5, false)); // 假设每个机器人周围5*5的范围
    q.push({0, 0});
    visited[0][0] = true;

    while (!q.empty())
    {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();

        // 检查当前位置是否有其他机器人
        for (int i = 0; i < 10; ++i)
        {
            if (robots[i].x == rot.x + cx && robots[i].y == rot.y + cy)
            {
                count++; // 找到一个机器人，数量加一
            }
        }

        // 向四个方向扩展
        vector<pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (auto &dir : dirs)
        {
            int nx = cx + dir.first;
            int ny = cy + dir.second;
            if (abs(nx) <= 2 && abs(ny) <= 2 && !visited[nx + 2][ny + 2])
            {
                q.push({nx, ny});
                visited[nx + 2][ny + 2] = true;
            }
        }
    }

    return count; // 返回找到的机器人数量
} */
void work()
{
    cur_frame.init();
    
	for(int i=0;i<10;i++){

		if(robot[i].status==0||robot[i].status==5){//空闲或者到达泊位时寻找货物 
			go_work(i);
		}
        if(robot[i].status==2&&robot[i].target_hw->life<=0)
            {   robot[i].target_hw=nullptr;
                go_work(i);
                while(!robot[i].MoveQueue.empty())
                robot[i].MoveQueue.pop();
            }
		else if(robot[i].status==3){//到达货物时寻找船舶 
			go_berth(i);
		}
	} 
    /*if (robot.Tx<=robot.x&&robot.x<robot.Tx+4&&robot.Ty<=robot.y&&robot.y<robot.Ty+4)
            robot.status = 5; 暂时没有整理逻辑标明机器人状态*/
}

// int main() {
//     Robot robots[10];
//     robots[0] = Robot(10, 20);
//     robots[1] = Robot(20, 50);
//     robots[2] = Robot(123, 21);
//     robots[3] = Robot(10, 21);
//     robots[4] = Robot(11, 19);

//     Berth berth[10];
//     berth[0] = Berth(1, 2, 5, 7);
//     berth[1] = Berth(2, 3, 5, 7);
//     berth[2] = Berth(3, 5, 7, 9);
//     berth[3] = Berth(13, 23, 42, 5);
//     berth[4] = Berth(21, 2, 21, 12);
//     berth[5] = Berth(12, 23, 42, 12);
//     berth[6] = Berth(22, 23, 23, 12);
//     berth[7] = Berth(21, 21, 34, 21);
//     berth[8] = Berth(11, 23, 23, 4);
//     berth[9] = Berth(23, 23, 21, 23);

//     hw_frame cur_frame;
//     cur_frame.frame_id = 1500;

//     cur_frame.hws.push_back(hw(1, 2, 12, 1232));
//     cur_frame.hws.push_back(hw(2, 3, 131, 1312));
//     cur_frame.hws.push_back(hw(23, 55, 213, 1332));
//     cur_frame.hws.push_back(hw(121, 111, 123, 1333));

//    work(robots,berth,cur_frame);
//     return 0;
// }

