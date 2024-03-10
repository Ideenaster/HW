
#include <vector>
#include <iostream>
#include<algorithm>
#include<queue>
#include <cstdlib> // 包含标准库以使用 abs 函数
using namespace std;
struct hw {
    int x, y;        // 货物的横纵坐标
    int value;       // 货物的价值
    int life = 1000; // 货物生命值
    int birth;       // 货物刷新时间

    // 默认构造函数
    hw() {}

    // 带参数的构造函数
     hw(int startX, int startY, int startValue, int startBirth, int frame_id)
        : x(startX), y(startY), value(startValue), birth(startBirth), life(1000 - frame_id + startBirth) {}

    bool operator==(const hw& other) const {
        return x == other.x && y == other.y && value == other.value && life == other.life && birth == other.birth;
    }
};


struct hw_frame {
    int frame_id;           // 帧序号
    int money;              // 金钱
    int add_num;            // 新增货物数量
    static std::vector<hw> hws; // 当前帧所有的货物 //这个是不是应该是全局的
};

// 定义全局的货物容器
std::vector<hw> hw_frame::hws;
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
    int cum_count;//泊位当前有多少货物
}berth[10];
//泊位间的移动时间是500帧
//一共有10个泊位

struct Robot {
    int x, y, goods;
    int status;
    int Tx, Ty; // 目标坐标
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }
    //应该有一个指向货物的指针，选中货物就把这个指针指向货物
    hw* target_hw;
    int count_robot=0;
};


// 距离
//临时废物函数
int getdist(int x1,int y1,int x2,int y2);

int calculate_dist(Robot& rot,hw& hw);//计算距离凯开销

int calculate_value(Robot& rot,hw& hw);//计算价值开销

int calculate_life_left(Robot& rot,hw& hw);

std::vector<hw> robot_hw(Robot& rot, std::vector<hw>& hws, hw_frame& cur_frame);//初步筛选，可达为标准

void remove_hw(std::vector<hw>& hws, hw& to_remove); //更新场上货物组

void go_work(vector<Robot>robots); //循环跑每个机器人，更新这个序列

int find_best_berth(Robot& rot,Berth berth[]){
    int index=-1;
    int min_dist=1000;
    for(int i=0;i<10;i++){
        int dist=getdist(rot.x,rot.y,berth[i].x,berth[i].y);
        if(dist<min_dist) {min_dist=dist;index=i;}
    }
    return index;
}
void go_berth(vector<Robot>&robots,Berth berth[10]){
        for(Robot robot:robots){
            Berth* target_berth=&berth[find_best_berth(robot,berth)];
            robot.Tx=target_berth->x;
            robot.Ty=target_berth->y;
        }
}
bool bfs_search(Robot& rot, std::vector<Robot>& robots) {
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(5, vector<bool>(5, false)); // 假设每个机器人周围5*5的范围
    q.push({0, 0});
    visited[0][0] = true;

    while (!q.empty()) {
        int cx = q.front().first;
        int cy = q.front().second;
        q.pop();

        // 检查当前位置是否有其他机器人
        for (Robot& robot : robots) {
            if (&robot == &rot) // 忽略自己
                continue;
            if (robot.x == rot.x + cx && robot.y == rot.y + cy) {
                return true;
            }
        }

        // 向四个方向扩展
        vector<pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (auto& dir : dirs) {
            int nx = cx + dir.first;
            int ny = cy + dir.second;
            if (abs(nx) <= 2 && abs(ny) <= 2 && !visited[nx + 2][ny + 2]) {
                q.push({nx, ny});
                visited[nx + 2][ny + 2] = true;
            }
        }
    }

    return false; // 没有找到其他机器人
}
bool check_in_berth(Robot& robot,Berth berth)
{
    if(berth.x<=robot.x<=berth.x+4&&berth.y<=robot.y<=robot.y+4)
    return true;
    else return false;
}
void check_robots_near(vector<Robot>& robot){
    for(Robot rot:robot){
        for(int i=0;i<10;i++){
            if(rot.x==robot[i].x&&rot.y==robot[i].y)
                continue;
            else{
                if(getdist(rot.x,rot.y,robot[i].x,robot[i].y)<10)
                rot.count_robot++;
            }
        }
    }
}




int getdist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
int calculate_dist(Robot& rot, hw& hw) {
    return getdist(rot.x, rot.y, hw.x, hw.y);
}

// 价值
int calculate_value(Robot& rot, hw& hw) {
    return hw.value;
}

// 生命
int calculate_life_left(Robot& rot, hw& hw, hw_frame& cur_frame) {
    
    return hw.life;
}

// 保证距离比较近且不会消失
std::vector<hw> robot_hw(Robot& rot, std::vector<hw>& hws, hw_frame& cur_frame) {
    std::vector<hw> to_get;
    for (hw& i : hws) {
        if (calculate_dist(rot, i) < 80 && calculate_life_left(rot, i, cur_frame)>80) // 80步之内能拿
            to_get.push_back(i);
    }
    return to_get;
}

// 机器人拿到了某个货物，把这个货物从 vector 中删除
void remove_hw(std::vector<hw>& hws, hw& to_remove) {
    // 在容器中查找要删除的货物
    auto it = std::find(hws.begin(), hws.end(), to_remove);
    // 如果找到了，就从容器中删除该货物
    if (it != hws.end()) {
        hws.erase(it);
    }
}
// 机器人在 to_get 向量中寻找收益最高的货物
// 机器人在 to_get 向量中寻找收益最高的货物
hw& find_hw(Robot& rot, std::vector<hw>& to_get, hw_frame& cur_frame) {
    int max_score = INT_MIN; // 最高分数
    hw* target = nullptr;
    for (hw& hw : to_get) {
        int fx = 7 * calculate_dist(rot, hw) + 10 * calculate_value(rot, hw) - 2 * calculate_life_left(rot, hw, cur_frame);
        if (fx > max_score) {
            max_score = fx;
            target = &hw;
        }
    }
    return *target;
}
void go_work(vector<Robot>& robots, hw_frame& cur_frame)
{
    for (auto& robot : robots) {
        // 检查附近是否有货物，并选择收益最高的货物
        vector<hw> to_get=robot_hw(robot,cur_frame.hws,cur_frame);
        hw target = find_hw(robot, to_get, cur_frame);
        
        // 如果找到了目标货物，则将其从列表中移除，并将其指定为机器人的目标货物
        remove_hw(cur_frame.hws, target);
        robot.target_hw = &target;
        
        // 如果货物列表为空，则退出循环
        if (cur_frame.hws.empty())
            break;
    }
}




int main() {
       // 创建一个机器人对象，初始位置为 (10, 20)
    vector<Robot> robots;
    Robot rot(11,19);
    robots.push_back(Robot(10, 20));
    robots.push_back(Robot(20,50));
    robots.push_back(Robot(123,21));
    robots.push_back(Robot(10,21));
    robots.push_back(rot);
    for(Robot& i:robots){
        cout<<bfs_search(i,robots)<<endl;
    }
    system("pause");
    hw_frame cur_frame; // 创建一个货物帧对象
    cur_frame.frame_id = 1500;

    // 使用带参数的构造函数创建一个 hw 对象，并将其添加到 cur_frame.hws 向量中
    cur_frame.hws.push_back(hw(1, 2, 12, 1232,cur_frame.frame_id));
    cur_frame.hws.push_back(hw(2,3,131,1312,cur_frame.frame_id));
    cur_frame.hws.push_back(hw(23,55,213,1332,cur_frame.frame_id));
    cur_frame.hws.push_back(hw(121,111,123,1333,cur_frame.frame_id));
    vector<hw> to_get=robot_hw(rot,cur_frame.hws,cur_frame);
    cout<<to_get.size()<<endl;
    //取到目标
    hw target=find_hw(rot,to_get,cur_frame);
    cout<<target.x<<endl;
    cout<<target.y<<endl;
    for(auto i:to_get){
        cout<<i.x<<endl;
        cout<<i.y<<endl;
        cout<<i.life<<endl;
        cout<<"*****************"<<endl;
    }
    remove_hw(cur_frame.hws,target);
    cout<<endl;
    cout<<endl;
    for(auto i:cur_frame.hws){
        cout<<i.x<<endl;
        cout<<i.y<<endl;
        cout<<i.life<<endl;
        cout<<"*****************"<<endl;
    }
    return 0;
}

