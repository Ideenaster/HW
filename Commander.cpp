#include<vector>
#include<stack>
#include<queue>
#include "A_star.cpp"
//typedef std::stack<int> Commandstack;

//临时定义
struct Point{
    int x, y;
    Point* Parent;
    Point(int x, int y):x(x), y(y),Parent(nullptr){}
};

struct Path{  
    std::vector<Point*> path;
    int id;
};

struct Robot
{
    int x, y;
    bool goods;           //True:有货 False:无货
    int status;           //0:空闲 1:寻路 2:碰撞后恢复
    int Tx, Ty;           //目标坐标
    //是否响应了上一次的指令
    bool isResponded;     //True:响应了 False:未响应
    std::queue<int> MoveQueue;
    Robot() {}
    Robot(int startX, int startY) {
        x = startX;
        y = startY;
    }

    /*
    将寻路算法返回的路径Vector转换Move指令队列，只存Move指令的方向
    */
    void Robot::Point2Move(Path& Path)//调用例： robot[Path_x.id].Point2Move(Path_x)
    {
        vector<Point*>& path = Path.path;
        for(int i = 1; i < path.size() - 1; i++)//path[0]的Point* 为起点
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
    int Robot::MovePop(){
        if(MoveQueue.empty())
        {
            return -1;
        }
        int temp = MoveQueue.front();
        MoveQueue.pop();
        return temp;
    }
};

struct Commander{
    int worldline;
};




// 记录机器人的当前位置和所有的未来位置
// typedef std::queue<Point*> Fateline;
// struct Fate{
//     Fateline Library[10];
//     void Fate::NewLine(Path& Path){
//         //将Path中的Point*加入到Library中
//         for(int i = 0; i < Path.path.size(); i++)
//         {
//             Library[Path.id].push(Path.path[i]);
//         }
//     }
//     bool Fate::Check(){//检测未来一帧的位置是否有机器人出现重合，即碰撞
//         int to_be_visited[200][200];
//         for(int i = 0; i<10;i++){
//             if(Library[i].empty())
//             {
//                 continue;
//             }
//         }
//     }
// };