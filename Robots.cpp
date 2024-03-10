#include "A_star_ori.cpp"
#include "Commander.cpp"

/*
维护注意事项
Robot全局数组在此文件中命名为Robot
地图全局数组在此文件中命名为Grid
*/
char Grid[200][200];



// 机器人避障类
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
                if (Robot[i].x + Robot[i].xmove() == Robot[j].x + Robot[j].xmove() && Robot[i].y + Robot[i].ymove() == Robot[j].y + Robot[j].ymove()) // 注意此处使用全局的Robot
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
            weight[i] = abs(Robot[i].x - Robot[i].Tx) + abs(Robot[i].y - Robot[i].Ty) * w1 + Robot[i].goods * w2;
        }
        // 对冲突机器人列表进行排序
        std::sort(conflict.begin(), conflict.end(), [&](int a, int b)
                  { return weight[a] > weight[b]; });
    }
    //根据当前的试探指令探测是否已被占用或将被更高优先级占用
    bool isMoveOK(int move,int robotid){
        //首先根据move指令试探机器人的下一个位置
        int x = Robot[robotid].x + Robot[robotid].Move2x(move);
        int y = Robot[robotid].y + Robot[robotid].Move2y(move);
        //首先检测是否是障碍物或者超出地图边界
        if(Grid[x][y] == '#' || x < 0 || x >= 200 || y < 0 || y >= 200){
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
            if(Robot[conflict[i]].x+Robot[conflict[i]].xmove() == x && Robot[conflict[i]].y+Robot[conflict[i]].ymove() == y){
                return false;
            }
        }
        //然后考虑不在冲突列表中的机器人
        for(int i = 0;i<10;i++){
            if(mark[i] == 1) continue;
            if(Robot[i].x+Robot[i].xmove() == x && Robot[i].y+Robot[i].ymove() == y){
                return false;
            }
        }
        //之所以这里要这么迂回的处理是因为希望不考虑低优先级的机器人，给高优先一点权力选择
    }
    //根据当前的冲突解决进程返回一个机器人的移动命令
    int GetMove(int idx){
        //此时旧的MoveQueue应该还没删，去除那个方向的试探
        int move = Robot[idx].MoveQueue.front();
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
            if (Robot[i].MoveQueue.empty())
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
                    while(!Robot[conflict[i]].MoveQueue.empty()){
                        Robot[conflict[i]].MoveQueue.pop();
                    }
                    Robot[conflict[i]].MoveQueue.push(move);
                    //这里激进一点，由于免费A_star，所以这里可以直接用A_star来重新计算路径
                    //但是A_star接口每完善
                    //TODO::auto Path = A_star(Robot[conflict[i]].x,Robot[conflict[i]].y,Robot[conflict[i]].Tx,Robot[conflict[i]].Ty);
                    //robot[Path.id].Point2Move(Path)
                    //这样新的路径也写进去了
                    
                }
            }
        }
    }
};