#ifndef ASATR_CPP
#define ASATR_CPP
#pragma GCC optimize(3, "Ofast", "inline")
#include "DataStructure.h" //引用全局数据结构头文件
#include <cstring>
#include <chrono>
extern char Map[200][200];     // 引用全局地图数组
extern Robot robot[robot_num]; // 引用全局机器人数组
extern unsigned short int berth_dist[10][200][200];

/*
 * @brief: 判断该单元格是否可拓展
 */
bool isValidPoint(int x, int y)
{
    return (x >= 0 && x < 200 && y >= 0 && y < 200 && Map[x][y] != '#' && Map[x][y] != '*');
}

/*
 * @brief: 判断该单元格是否是目标单元格
 */
bool isTargetPoint(int x, int y, Point *target)
{
    return (x == target->x && y == target->y);
}

/*
 * @brief: 重构解路径
 */
std::vector<Point *> reconstructPath(Point *current, int flag = 1)
{ // 重构解路径
    std::vector<Point *> path;
    while (current != nullptr)
    {
        path.push_back(current);
        current = current->parent;
    }
    if(flag)reverse(path.begin(), path.end());
    return path;
}
/*
 * @brief: A*算法函数主体
 */
struct ComparePoint : std::binary_function<Point *, Point *, bool>
{
    bool operator()(const Point *a, const Point *b) const
    {
        return a->f > b->f;
    }
};
constexpr int TIME_LIMIT_MS = 1000;
std::vector<Point *> aStar(Point *start, Point *target, int _id)
{
    auto start_time = std::chrono::steady_clock::now();
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    std::vector<Point *> open;
    std::make_heap(open.begin(), open.end(), ComparePoint());
    bool visited[200][200];
    std::memset(visited, false, sizeof(visited));
    start->calculateHeuristic(target);
    start->f = start->g + start->h;
    open.push_back(start);
    std::push_heap(open.begin(), open.end(), ComparePoint());
    visited[start->x][start->y] = true;

    // A* algorithm
    while (!open.empty())
    {
        auto end_time = std::chrono::steady_clock::now();
        auto duration = end_time - start_time;
        double microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        if (microseconds >= TIME_LIMIT_MS)
        {
            // 如果超过时间限制，返回空指针或其他指示
            return {};
        }
        std::pop_heap(open.begin(), open.end(), ComparePoint());
        Point *current = open.back();
        open.pop_back();

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
                open.push_back(neighbor);
                std::push_heap(open.begin(), open.end(), ComparePoint());
                visited[newx][newy] = true; // 忽略re-open问题
            }
        }
    }

    // No path found
    return {};
}
std::vector<Point *> aStar_berth(Point *start, Point *target, int robotid, int flag = 1)
{
    int berth_id = 0;
    for (int i = 0; i < 10; i++)
    {
        if (berth[i].x == robot[robotid].Tx && berth[i].y == robot[robotid].Ty&&flag == 1)
        {
            berth_id = i;
            break;
        }
        if(berth[i].x == robot[robotid].x && berth[i].y == robot[robotid].y &&flag == 0){
            berth_id = i;
            break;
        }
    }
    if(berth_dist[berth_id][start->x][start->y] == 40000){
        return{};
    }
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    start->f = berth_dist[berth_id][start->x][start->y];
    Point* current =nullptr,*neighbor = start;
    int cnt = 4000;
    while (cnt--)
    {
        current = neighbor;
        if (isTargetPoint(current->x, current->y, target))
        {
            return reconstructPath(current, flag);
        }
        int ntx = 0, nty = 0;
        int min = berth_dist[berth_id][current->x][current->y];
        for (int i = 0; i < 4; i++)
        {
            int newx = current->x + dr[i];
            int newy = current->y + dc[i];
            if (min >= berth_dist[berth_id][newx][newy] && isValidPoint(newx, newy))
            {
                min = berth_dist[berth_id][newx][newy];
                ntx = newx;
                nty = newy;
                break;
            }
        }
        neighbor = new Point(ntx, nty);
        neighbor->parent = current;
        neighbor->f = berth_dist[berth_id][ntx][nty];
    }
    // No path found
    return {};
}

#endif // ASATR_CPP