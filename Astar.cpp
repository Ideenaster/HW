#ifndef ASATR_CPP
#define ASATR_CPP

#pragma GCC optimize(3,"Ofast","inline")
#include "DataStructure.h" //引用全局数据结构头文件
#include <cstring>
extern char Map[200][200];     // 引用全局地图数组
extern Robot robot[robot_num]; // 引用全局机器人数组

/*
 * @brief: 判断该单元格是否可拓展
 */
bool isValidPoint(int row, int col)
{
    return (row >= 0 && row < 200 && col >= 0 && col < 200 && Map[row][col] != '#'&& Map[row][col] !='*');
}

/*
 * @brief: 判断该单元格是否是目标单元格
 */
bool isTargetPoint(int row, int col, Point *target)
{
    return (row == target->x && col == target->y);
}

/*
 * @brief: 重构解路径
 */
std::vector<Point *> reconstructPath(Point *current)
{ // 重构解路径
    std::vector<Point *> path;
    while (current != nullptr)
    {
        path.push_back(current);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

/*
 * @brief: 处理跳点逻辑
 */
bool JumpPoint(int newx,int newy,Point* current){
    //首先检测有没有父节点
    if(current->parent==nullptr){
        return true;
    }
    //拒绝返回到父节点
    if(current->parent->x==newx&&current->parent->y==newy){
        return false;
    }
    //判断强制邻居
    //首先计算偏移向量
    int dx=newx-current->x;
    int dy=newy-current->y;
    //计算障碍坐标
    int obsx=current->parent->x+dx;
    int obsy=current->parent->y+dy;
    if(obsx==current->x&&obsy==current->y){
        return true;//与父方向相同则通过
    }
    if(Map[obsx][obsy]!='#'&&Map[obsx][obsy]!='*'){
        return false;
    }
    return true;
}


/*
 * @brief: A*算法函数主体
 */
std::vector<Point*> aStar(Point *start, Point *target, int robotid)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    std::priority_queue<Point *, std::vector<Point *>, std::function<bool(Point *, Point *)>> open([](Point *a, Point *b)
                                                                                                   { return a->f > b->f; });
    bool visited[200][200];
    std::memset(visited, false, sizeof(visited));
    start->calculateHeuristic(target);
    start->f = start->g + start->h;
    open.push(start);
    visited[start->x][start->y] = true;
    // A* algorithm
    while (!open.empty())
    {
        Point *current = open.top();
        open.pop();
        if (isTargetPoint(current->x, current->y, target))
        {
            return reconstructPath(current);
        }
        visited[current->x][current->y] = true;
        for (int i = 0; i < 4; i++)
        {
            int newx = current->x + dr[i];
            int newy = current->y + dc[i];
            if (isValidPoint(newx, newy) && !visited[newx][newy])
            {
                Point *neighbor = new Point(newx, newy);
                neighbor->parent = current;
                neighbor->g = current->g + 1;
                neighbor->calculateHeuristic(target);
                neighbor->f = neighbor->g + neighbor->h;
                open.push(neighbor);
                visited[newx][newy] = true; // 忽略re-open问题
            }
        }
    }
    // No path found
    return {};
}

#endif // ASATR_CPP