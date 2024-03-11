#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include "DataStructure.h"
using namespace std;

int getdist(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

int calculate_dist(Robot &rot, hw &hw)
{
    return getdist(rot.x, rot.y, hw.x, hw.y);
}

int calculate_value(Robot &rot, hw &hw)
{
    return hw.value;
}

int calculate_life_left(Robot &rot, hw &hw, hw_frame &cur_frame)
{
    return hw.life;
}

vector<hw> robot_hw(Robot &rot, hw_frame &cur_frame)
{
    vector<hw> to_get;
    for (hw &i : cur_frame.hws)
    {
        if (calculate_dist(rot, i) < 80 && calculate_life_left(rot, i, cur_frame) > 80)
            to_get.push_back(i);
    }
    return to_get;
}

void remove_hw(hw_frame &cur_frame, hw &to_remove)
{
    auto it = find(cur_frame.hws.begin(), cur_frame.hws.end(), to_remove);
    if (it != cur_frame.hws.end())
    {
        cur_frame.hws.erase(it);
    }
}

hw &find_hw(Robot &rot, vector<hw> &to_get, hw_frame &cur_frame)
{
    int max_score = INT_MIN;
    hw *target = nullptr;
    for (hw &hw : to_get)
    {
        int fx = 7 * calculate_dist(rot, hw) + 10 * calculate_value(rot, hw) - 2 * calculate_life_left(rot, hw, cur_frame);
        if (fx > max_score)
        {
            max_score = fx;
            target = &hw;
        }
    }
    return *target;
}

void go_work(Robot robots[10], hw_frame &cur_frame)
{
    for (int i = 0; i < 10; ++i)
    {
        Robot &robot = robots[i];
        if (robot.flag != 0)
            continue;
        vector<hw> to_get = robot_hw(robot, cur_frame);
        if (!to_get.empty())
        {
            hw &target = find_hw(robot, to_get, cur_frame);
            remove_hw(cur_frame, target);
            robot.target_hw = &target;
            robot.Tx = target.x;
            robot.Ty = target.y;
            if (robot.Tx == robot.x && robot.Ty == robot.y)
                robot.status = 0;
            if ((robot.Tx != robot.x) || (robot.Ty != robot.y))
                robot.status = 1;
        }
    }
}

int find_best_berth(Robot &rot, Berth berth[])
{
    int index = -1;
    int min_dist = 1000;
    for (int i = 0; i < 10; ++i)
    {
        int dist = getdist(rot.x, rot.y, berth[i].x, berth[i].y);
        if (dist < min_dist)
        {
            min_dist = dist;
            index = i;
        }
    }
    return index;
}

void go_berth(Robot robots[10], Berth berth[10])
{
    for (int i = 0; i < 10; ++i)
    {
        Robot &robot = robots[i];
        if (robot.flag != 1)
            continue;
        int target_index = find_best_berth(robot, berth);
        Berth *target_berth = &berth[target_index];
        robot.Tx = target_berth->x;
        robot.Ty = target_berth->y;
        if (robot.Tx == robot.x && robot.Ty == robot.y)
            robot.status = 0;
        if ((robot.Tx != robot.x) || (robot.Ty != robot.y))
            robot.status = 1;
        target_berth->target_robots.push_back(i);
    }
}
int count_nearby_robots(Robot &rot, Robot robots[10])
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
}
void work(Robot robot[10], Berth berth[10], hw_frame cur_frame)
{
    go_work(robot, cur_frame);
    go_berth(robot, berth);
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
