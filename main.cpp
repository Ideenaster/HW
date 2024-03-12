#include "DataStructure.h"
#include "Astar.cpp"
#include "BOAT_TO_BARTH.cpp"
#include "Robots_choose.cpp"
#include "ctime"

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
    scanf("%d", &capacity);
    char okk[5];
    scanf("%s", okk);
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
        cur_frame.hws.push_back(hw(x,y,val,id));
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

clock_t sta;
int main()
{
    // clock_t sta;
    Init();
    pre_berth(berth);
    while(true){
        int frameid = Input();//读入完成
        // clock_t end = clock();
        // if(frameid != (end - sta)/CLOCKS_PER_SEC*50)fflush(stdin);
        cur_frame.frame_id = frameid;
        work(robot,berth,cur_frame);
       for(int i = 0; i<10; i++){
            if(robot[i].status == 23){//判定当前机器人需要寻路
                Point* start = new Point(robot[i].x,robot[i].y);
                Point* target = new Point(robot[i].Tx,robot[i].Ty);
                auto NewPath = aStar(start,target,i);
                robot[NewPath.id].Point2Move(NewPath);
            }
       }
        avoid.check_conflict();
        if(!avoid.conflict.empty()){
            avoid.rewriteMoveQueue();
        }
        for(int i = 0; i<10;i++){
            if(!robot[i].MoveQueue.empty()){
                Cmd.PushCommand(0,i,robot[i].MovePop());
            }//机器人的Move写入完毕
        }
        for(int i = 0; i < 10 ;i++){
            if(robot[i].status == 1){
                if(robot[i].x+robot[i].xmove() == robot[i].Tx && robot[i].y+robot[i].ymove() == robot[i].Ty){
                Cmd.PushCommand(1,i);
                robot[i].status = 3;//表示已到达货物目标位置
                }
            }
            else if(robot[i].status == 4){
                if(robot[i].x+robot[i].xmove() == robot[i].Tx && robot[i].y+robot[i].ymove() == robot[i].Ty){
                    Cmd.PushCommand(2,i);
                    for(int j = 0;j<10;j++){
                        if(robot[i].Tx == berth[j].x&&robot[i].Ty == berth[j].y){
                            berth[j].num++;
                            berth[j].value+=robot[i].goods_val;
                            berth[j].goods.push(robot[i].goods_val);
                            break;
                        }
                    }
                    robot[i].status = 5;//表示已到达泊位
                }
            }
        }
        //下面开始泊位指令
        boat_to_berth();
        Cmd.PopCommand();

    }

    return 0;
}
