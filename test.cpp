#include <queue>
#include <algorithm>
struct Point{
    int x, y;
    int f, g, h;
    Point* parent;
    Point(int x, int y): x(x), y(y), f(0), g(0), h(0), parent(nullptr){}
    void calculateHeuristic(Point *target)
    {
        h = std::abs(target->x - x) + abs(target->y - y); // 计算 曼哈顿距离
    }
};
/*
 * @brief: 要求Astar算法返回的路径结构体
 * @param: path: 存储路径的vector
 * @param: id:   机器人id
 */
// struct Path{  
//     std::vector<Point*> path;
//     int id;
// };

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
    int goods = 0;            //goods表示机器人是否携带货物
    int status = 0;           //0:空闲 1:寻货物路中 2：已分配 3：已经到达货物目标位置 4:寻找泊位路中 5:到达泊位
    int Tx = 0, Ty = 0;           //目标坐标
    int current_target_x = 0;   //当前目标点x坐标
    int current_target_y = 0;   //当前目标点y坐标
    std::queue<int> MoveQueue;
    hw* target_hw;
    int count_robot = 0;
    int goods_val = 0;
    bool flag=0; //找货物中
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    /*
    将寻路算法返回的路径Vector转换Move指令队列，只存Move指令的方向
    */
    void Point2Move(std::vector<Point*> path)//调用例： robot[Path_x.id].Point2Move(Path_x)
    {
        if(path.size()==0)return;
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
int main(){
    Robot robot[10];
    int i = 0;
    if(robot[i].MoveQueue.empty()){
        return 1;
    }
    robot[i].x = robot[i].xmove();
}