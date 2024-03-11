#ifndef ASATR_CPP
#define ASATR_CPP

#include "DataStructure.h"    //引用全局数据结构头文件
extern char Map[200][200];    //引用全局地图数组
extern Robot robot[robot_num];//引用全局机器人数组


/*
 * @brief: 判断该单元格是否可拓展
 */
bool isValidPoint(int row, int col)
{
    return (row >= 0 && row < 200 && col >= 0 && col < 200 && Map[row][col] != '#');
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
 * @brief: A*算法函数主体
 */
Path aStar(Point *start, Point *target, int robotid)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    std::priority_queue<Point *, std::vector<Point *>, std::function<bool(Point *, Point *)>> open([](Point *a, Point *b)
                                                                                { return a->f > b->f; });
    bool visited[200][200] = {false};
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
            std::vector<Point*>&&path = reconstructPath(current);
            return Path{path, robotid};
        }
        visited[current->x][current->y] = true;
        for (int i = 0; i < 4; i++)
        {
            int newx = current->x + dr[i];
            int newy = current->y + dc[i];
            // 如果新生成的邻居节点n'已经存在于OPEN集合中,我们不应该直接忽略它,而是比较这两条路径到达n'的代价g值,取其中较小的那条路径。
            Point *neighbor = new Point(newx, newy);
            neighbor->parent = current;
            neighbor->g = current->g + 1;
            neighbor->calculateHeuristic(target);
            neighbor->f = neighbor->g + neighbor->h;
            open.push(neighbor);
            visited[newx][newy] = true;//忽略re-open问题
        }
    }
    // No path found
    return {};
}


#endif // ASATR_CPP