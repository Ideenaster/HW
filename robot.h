ifndef ROBOT_H
#define ROBOT_H
#include <vector>
#include <string>
#include <queue>
#include <ctime>
#include <unordered_map>
#include <map>
using namespace std;
class Robot{
    bool is_alive=true;
    Point lastPosition; //记录每个机器人50帧以前的位置
    Point lastPosition1; //记录每个机器人100帧以前的位置
    bool near_face=false; //是否被附近机器人贴脸
    bool near_wall=false;//是否三面靠墙
    bool is_avoiding=false; //是否正在躲避状态
    bool is_backing=false;//是否在回溯状态
    static int money; //器人共享总资金
    int id;//机器人编号
    bool is_near_bowei; //是否在泊位附近
    int target_time; //拿到目标时间估算
    int left_time; //目标消失时间
    bool is_loading; //是否负载装货

    //避障处理
    static vector<int> avoid_robots_index;
    static unordered_map<Robot*,Robot*> avoided_robots;
}Robot[10];
    map<Robot*,Point> conflict_point; //机器人和会冲突点的映射
    vector<Point> avoid_path; //躲避路径 我去找一个空旷地带 左右迁移
    vector<Point> back_path; //回溯路径，躲避路径的逆路径
    vector<Point> true_path; //当前正在走的路径，理想路径加避障路径/回溯路径
    vector<Point> re_astar_path; //实在不行了避开不回去重新寻路
    vector<Point> current_position;
    vector<Point> future_position;


