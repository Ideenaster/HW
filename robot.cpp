#include"robot.h"
#define avoidant
#define priority
//bool not_wall(int x,int y);
/*
    vector<Point> current_position;
    vector<Point> future_position;
    */
void update_cur_fu_position(){
    //更新未来位置
    for(int i=0;i<10;i++){
        current_position[i].x=Robot[i].x;
        current_position[i].y=Robot[i].y;
        future_positon[i].x=Robot[i].x+Robot[i].xmove();
        future_position[i].y=Robot[i].y+Robot[i].ymove();
    }
}
bool in_set(int target,std::vector<int> set)
{   
    bool flag=false;
    for(int i=0;i<set.size();i++)
        if(set[i]==target)
        flag=true;
    return flag;
}
void generate_avoid_robots_vector_index(Robot Robot[10]) //产生避障冲突机器
{   
    //参数robot是当前判定的机器人，机器人总数组在Robot[10];中
    for(int i=0;i<10;i++){       
    //判定cur_robot的下一时刻的位置和robot下一时刻的位置是否冲突
/*  0 表示右移一格 
    1 表示左移一格 
    2 表示上移一格 
    3 表示下移一格 
*/
    for(int j=i+1;j<10;j++){
        if(Robot[i].x+Robot[i].xmove()==Robot[j].x+Robot[j].xmove()&&Robot[i].y+Robot[i].ymove()==Robot[j].y+Robot[j].move())
        {
            //如果机器人i的下一时刻的位置和机器人j的下一时刻的位置相同,i和j不会重复，那么直接把这两个压入躲避判断序列
            if(!in_set(i,Robot::avoid_robots_index)) //如果不在集合里，我就添加
            avoid_robots_index.push_back(i);
            if(!in_set(j,Robot::avoid_robots_index))
            avoid_robots_index.push_back(j);     
        }
    }         
    }
}
//两个位置 当前位置 未来位置
//开始遍历冲突机器人索引，这里应该有一个最高优先级，优先级最高的执行原计划，优先级较弱的
//执行避让处理（临时避让)
void  update_status()
{
    //更新优先级
}
void avoid_conflict()
{   
    char command;
    bool down=true;
    bool up=true;
    bool right=true;
    bool left=true;
    for(int index : Robot::avoid_robots_index){
        //遍历索引，判断机器人是否是避让状态
        if(Robot[index].status==priority) continue;
        if(Robot[index].status==avoidant){
            //避让前面的未来位置，不要到后面的当前位置，穷举四个方向，遍历前方机器人和后方机器人
            //同时还要避让优先哥的位置
            for(int i=0;i<index;i++)
            {   //往下走一格 down
                if(Robot[index].x+1==future_position[i].x&&Robot[index].y==future_position[i].y|!not_wall(Robot[index].x+1,Robot[index].y)|Robot[index].x+1==future_position[priority_index].x&&Robot[index].y==future_position[priority_index].y)
                down=false; //往下，和前面的未来位置冲突
                if(Robot[index].x-1==future_position[i].x&&Robot[index].y==future_postion[i].y|!not_wall(Robot[index].x-1,Robot[index].y)|Robot[index].x-1==future_position[priority_index].x&&Robot[index].y==future_postion[priority_index].y)
                up=false; //往上也不行
                if(Robot[index].x==future_position[i].x&&Robot[index].y+1==future_positon[i].y|!not_wall(Robot[index].x,Robot[index].y+1)|Robot[index].x==future_position[priority_index].x&&Robot[index].y+1==future_positon[priority_index].y)
                right=false;
                if(Robot[index].x==future_position[i].x&&Robot[index].y-1==future_positon[i].y|!not_wall(Robot[index].x,Robot[index].y-1)|Robot[index].x==future_position[priority_index].x&&Robot[index].y-1==future_positon[priority_index].y)
                left=false;
            }
            //比如6现在优先级最高，现在保证了6以前的避让机器人不会到6的未来位置，6之后的避让机器人也不会到6的未来位置
            //6之前的避让机器人也不会到6的当前位置
            //现在这样貌似是没问题的，然后当避让机器人遍历到status为优先的时候直接执行操作就行，也就是future和current不变，其他避让的必然会
            //改变future，所以在每次避让操作末尾改变future位置，这个逻辑应该是没有问题的。
            for(int i=index+1;i<10;i++)
            {
                //遍历后面的机器人，不能和当前位置重合
                if(Robot[index].x+1==current_position[i].x&&Robot[index].y==current_position[i].y|!not_wall(Robot[index].x+1,Robot[index].y))
                down=false; //往下，和前面的未来位置冲突
                if(Robot[index].x-1==current_position[i].x&&Robot[index].y==current_postion[i].y|!not_wall(Robot[index].x-1,Robot[index].y))
                up=false; //往上也不行
                if(Robot[index].x==current_position[i].x&&Robot[index].y+1==current_positon[i].y|!not_wall(Robot[index].x,Robot[index].y+1))
                right=false;
                if(Robot[index].x==current_position[i].x&&Robot[index].y-1==current_positon[i].y|!not_wall(Robot[index].x,Robot[index].y-1))
                left=false;
            }
            if(left==true) command='l',future_position[index].y=Robot[index].y-1;
            if(right==true) command='r',future_positon[index].y=Robot[index].y+1;
            if(up==true) command='u',future_position[index].x=Robot[index].x-1;
            if(down==true) command='d',future_position[index].x=Robot[index].x+1;
            //如果由多种尝试，直接算最后一种，他会覆盖前面的判断，这里赋值操作最终就是更新future
            //这个避免逻辑就是下一步操作我绝对不会去和别人撞，至于去哪先不管，然后如果不用躲避就可以
            //在当前位置跑寻路执行梯度下降方向
            //有一个最高优先级所以应该不会死锁。
            //更新path
            return command;
            //不知道返回什么，这里做出决策

        }
    }
}