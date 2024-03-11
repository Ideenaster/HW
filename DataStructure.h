#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

//C++ STL头文件引用区
#include <vector>
#include <iostream>
#include <queue>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>


//常量定义区
const int n = 200;       // 地图大小
const int robot_num = 10;// 机器人数量
const int berth_num = 10;// 泊位数量
const int N = 210;      

//结构体的声明
struct Robot;
struct Commander;
extern Robot robot[10];
extern char Map[200][200];
struct Path;
struct Point;

//A_star算法数据结构定义区-----------------------------------------------------------------------------------------------
Path aStar(Point *start, Point *target, int robotid);


/*
 * @brief: Astar算法中的单元格结构体
 */
struct Point{
    int x, y;
    int f, g, h;
    Point* parent;
    Point(int x, int y): x(x), y(y), f(0), g(0), h(0), parent(nullptr){}
    void calculateHeuristic(Point *target)
    {
        h = abs(target->x - x) + abs(target->y - y); // 计算 曼哈顿距离
    }
};
/*
 * @brief: 要求Astar算法返回的路径结构体
 * @param: path: 存储路径的vector
 * @param: id:   机器人id
 */
struct Path{  
    std::vector<Point*> path;
    int id;
};

//货物结构体定义区-------------------------------------------------------------------------------------------------------
struct hw {
    int x, y;        // 货物的横纵坐标
    int value;       // 货物的价值
    int life = 1000; // 货物生命值
    int birth = 0;       // 货物刷新时间

    // 默认构造函数
    hw() {}

    // 带参数的构造函数
    hw(int startX, int startY, int startValue, int startBirth)
        : x(startX), y(startY), value(startValue), birth(startBirth) {}

    bool operator==(const hw& other) const {
        return x == other.x && y == other.y && value == other.value && life == other.life && birth == other.birth;
    }
};

struct hw_frame {
    int frame_id;           // 帧序号
    // int money;              // 金钱
    // int add_num;            // 新增货物数量
    std::vector<hw> hws;         // 当前帧所有的货物
    void init()
    {
        for(hw hw:hws){
            hw.life=frame_id-hw.birth;
        }
    }
};

//机器人结构体定义区-----------------------------------------------------------------------------------------------------

struct Robot
{ int x, y;
    int goods;            //goods表示机器人是否携带货物
    int status;           //0:空闲  1:寻路中 2:碰撞后恢复  相当于表示已经放下货物或者已经装载完毕货物
    int Tx, Ty;           //目标坐标
    int current_target_x;   //当前目标点x坐标
    int current_target_y;   //当前目标点y坐标
    std::queue<int> MoveQueue;
    hw* target_hw;
    int count_robot=0;
    bool flag=0; //找货物中
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    /*
    将寻路算法返回的路径Vector转换Move指令队列，只存Move指令的方向
    */
    void Point2Move(Path& Path)//调用例： robot[Path_x.id].Point2Move(Path_x)
    {
        std::vector<Point*>& path = Path.path;
        for(unsigned i = 1; i < path.size() - 1; i++)//path[0]的Point* 为起点
        {
            if(path[i]->x == path[i-1]->x)
            {
                if(path[i]->y > path[i-1]->y)
                {
                    MoveQueue.push(0);
                }
                else
                {
                    MoveQueue.push(1);
                }
            }
            else
            {
                if(path[i]->x < path[i-1]->x)
                {
                    MoveQueue.push(2);
                }
                else
                {
                    MoveQueue.push(3);
                }
            }
        }

    }
    /*
    响应指令机发出的指令，弹出一个Move指令方向
    */
    int MovePop(){
        if(MoveQueue.empty())
        {
            return -1;
        }
        int temp = MoveQueue.front();
        MoveQueue.pop();
        return temp;
    }
    /*
    下一步指令导致x变化的坐标
    */
    int xmove(){
        if(MoveQueue.empty())
        {
            return 0;
        }
        int temp = MoveQueue.front();
        if(temp == 0 || temp == 1)
        {
            return 0;
        }
        else if(temp == 2)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    /*
    下一步指令导致y变化的坐标
    */
    int ymove(){
        if(MoveQueue.empty())
        {
            return 0;
        }
        int temp = MoveQueue.front();
        if(temp == 2 || temp == 3)
        {
            return 0;
        }
        else if(temp == 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    int Move2x(int move){
        if(move == 0 || move == 1)
        {
            return 0;
        }
        else if(move == 2)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    int Move2y(int move){
        if(move == 2 || move == 3)
        {
            return 0;
        }
        else if(move == 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    
    }
};
//机器人避障类
struct RobotAvoidance
{
    // 优先级产生的权重参数
    int w1 = 1;
    int w2 = 2;
    // 冲突对象vector放在struct里面存储
    std::vector<int> conflict;

    void check_conflict()
    {
        // 检测之前清空冲突列表
        conflict.clear();
        // 检查机器人是否会发生碰撞
        for (int i = 0; i < 10; i++)
        {
            for (int j = i + 1; j < 10; j++)
            {
                if (robot[i].x + robot[i].xmove() == robot[j].x + robot[j].xmove() && robot[i].y + robot[i].ymove() == robot[j].y + robot[j].ymove()) // 注意此处使用全局的Robot
                {
                    conflict.push_back(i);
                    conflict.push_back(j);
                }
            }
        }
        auto last = std::unique(conflict.begin(), conflict.end());
        conflict.erase(last, conflict.end());
    }
    // 对check函数返回的冲突机器人列表进行优先级排序
    void Priorsort()
    {
        // 计算每个机器人当前坐标到目标坐标的曼哈顿距离和机器人货物的价值进行加权和
        int weight[10];
        for (auto i : conflict)
        {
            weight[i] = abs(robot[i].x - robot[i].Tx) + abs(robot[i].y - robot[i].Ty) * w1 + robot[i].goods * w2;
        }
        // 对冲突机器人列表进行排序
        std::sort(conflict.begin(), conflict.end(), [&](int a, int b)
                  { return weight[a] > weight[b]; });
    }
    //根据当前的试探指令探测是否已被占用或将被更高优先级占用
    bool isMoveOK(int move,int robotid){
        //首先根据move指令试探机器人的下一个位置
        int x = robot[robotid].x + robot[robotid].Move2x(move);
        int y = robot[robotid].y + robot[robotid].Move2y(move);
        //首先检测是否是障碍物或者超出地图边界
        if(Map[x][y] == '#' || x < 0 || x >= 200 || y < 0 || y >= 200){
            return false;
        }
        int mark[10];
        for(int i = 0;i<10;i++){
            mark[i] = 0;
        }
        //先考虑在冲突列表中而且优先级比当前机器人高的机器人
        for(unsigned i = 0;i<conflict.size();i++){
            mark[conflict[i]] = 1;           //标记一下，后面这些就不用考虑
            if(conflict[i] == robotid) break;//优先级比当前机器人低的机器人不必考虑
            if(robot[conflict[i]].x+robot[conflict[i]].xmove() == x && robot[conflict[i]].y+robot[conflict[i]].ymove() == y){
                return false;
            }
        }
        //然后考虑不在冲突列表中的机器人
        for(int i = 0;i<10;i++){
            if(mark[i] == 1) continue;
            if(robot[i].x+robot[i].xmove() == x && robot[i].y+robot[i].ymove() == y){
                return false;
            }
        }
        //之所以这里要这么迂回的处理是因为希望不考虑低优先级的机器人，给高优先一点权力选择
    }
    //根据当前的冲突解决进程返回一个机器人的移动命令
    int GetMove(int idx){
        //此时旧的MoveQueue应该还没删，去除那个方向的试探
        int move = robot[idx].MoveQueue.front();
        for(int i = 0;i<4;i++){
            if(i == move) continue;
            if(isMoveOK(i,idx)){
                return i;
            }
        }
        //此处留下可能的机器人没有选择的接口
        //TODO::机器人没有选择，做额外的处理
    }
    // 根据冲突优先级重写冲突对象的MoveQueue
    // conflict[0]不在冲突之中
    void rewriteMoveQueue()
    {
        // 对冲突机器人列表进行优先级排序
        Priorsort();
        // 对冲突机器人列表进行遍历
        for (auto i : conflict)
        {
            // 如果冲突机器人的MoveQueue为空，则不需要重写
            if (robot[i].MoveQueue.empty())
            {
                continue;
            }
            // 如果冲突机器人的MoveQueue不为空，则需要重写
            else
            {
                //顺序维护即可，优先级越高的越自由，保证优先级越高的机器人的未来位置不和其他机器人的当前位置重合即可
                for(unsigned i = 1;i<conflict.size();i++){
                    //既然冲突那么MoveQueue就不为空，这里为空上面的TODO就得做了
                    int move = GetMove(conflict[i]);
                    //清空MoveQueue
                    while(!robot[conflict[i]].MoveQueue.empty()){
                        robot[conflict[i]].MoveQueue.pop();
                    }
                    robot[conflict[i]].MoveQueue.push(move);
                    //这里激进一点，由于免费A_star，所以这里可以直接用A_star来重新计算路径
                    Point* start = new Point(robot[conflict[i]].x,robot[conflict[i]].y);
                    Point* target = new Point(robot[conflict[i]].Tx,robot[conflict[i]].Ty);
                    auto NewPath = aStar(start,target,conflict[i]);
                    robot[NewPath.id].Point2Move(NewPath);
                }
            }
        }
    }
};

//泊位结构体定义区-------------------------------------------------------------------------------------------------------
struct GOOD{
	int value;
};

struct Berth
{
    int id;
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int time;//泊位到虚拟点的时间1-1000
	int velocity;//Velocity表示泊位的装载速度,单位个/帧1-5
	int value;
	int num;//货物数量 
	bool is_choose;//为1表示已经被选择过
    std::vector<int> target_robots; // 存储目标为该泊位的机器人的索引下标

	std::queue<GOOD> goods;//暂时不考虑 
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
    bool operator==(const Berth& other) const {
        return x == other.x && y == other.y;
    }

    void reset() {
        target_robots.clear(); // 清空目标机器人索引下标的向量
    }
}berth[berth_num];

struct Boat
{   
    int value;//当前船上的价值
    int location;//-2-9之间，-1表示在虚拟点，-2表示当前不可用 
    int pos, status;
    int num;//当前船上的货物数量
}boat[5];


//指令机结构体定义区-----------------------------------------------------------------------------------------------------
struct Commander {
    std::string CommandSet;
    const char* CommandName[5] = {"move", "get", "pull", "ship", "go"};

    void PushCommand(int CommandType, int Param1, int Param2 = -1) {
        char command[20];
        if(Param2 == -1)
        {
            std::snprintf(command, sizeof(command), "%s %d\n", CommandName[CommandType], Param1);
        }
        else
        {
            std::snprintf(command, sizeof(command), "%s %d %d\n", CommandName[CommandType], Param1, Param2);
        }
        CommandSet += command;
    }
    /*
    此函数一次性输出所有CommandSet中的指令，并清空CommandSet
    */
    void PopCommand() {
        // 输出CommandSet中的指令
        std::printf("%sOK", CommandSet.c_str());
        // 强制刷新输出缓冲区
        std::fflush(stdout);
        // 清空CommandSet
        CommandSet.clear();
    }
};

//全局变量定义区---------------------------------------------------------------------------------------------------------
int money, capacity, id;
char Map[200][200];         //创建全局地图数组
Robot robot[robot_num];     //创建全局机器人数组
int gds[N][N];
hw_frame cur_frame;         //记录截至目前的全部货物
RobotAvoidance avoid;
Commander Cmd;

#endif // DATASTRUCTURE_H