#pragma GCC optimize(3, "Ofast", "inline")
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
using namespace std;
char grid[200][200];
// extern char grid[200][200];
struct Cell
{
    int row, col;
    int f, g, h;
    Cell *parent;

    Cell(int r, int c) : row(r), col(c), f(0), g(0), h(0), parent(nullptr) {}

    void calculateHeuristic(Cell *target)
    {
        h = abs(target->row - row) + abs(target->col - col); // 计算 曼哈顿距离
    }
};

// 函数用于检查一个单元格是否有效，有效指的是在地图范围内且不是障碍物
bool isValidCell(int row, int col, char grid[200][200])
{
    return (row >= 0 && row < 200 && col >= 0 && col < 200 && grid[row][col] != '#');
}

// 函数用于检查一个单元格是否是目标单元格
bool isTargetCell(int row, int col, Cell *target)
{
    return (row == target->row && col == target->col);
}
vector<Cell *> reconstructPath(Cell *current)
{ // 重构解路径
    vector<Cell *> path;
    while (current != nullptr)
    {
        path.push_back(current);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

// A* algorithm to find the shortest path from start to target
vector<Cell *> aStar(char grid[200][200], Cell *start, Cell *target)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    priority_queue<Cell *, vector<Cell *>, function<bool(Cell *, Cell *)>> open([](Cell *a, Cell *b)
                                                                                { return a->f > b->f; });
    bool visited[200][200] = {false};
    bool closed[200][200] = {false};
    start->calculateHeuristic(target);
    start->f = start->g + start->h;
    open.push(start);
    visited[start->row][start->col] = true;
    // A* algorithm
    while (!open.empty())
    {
        Cell *current = open.top();
        open.pop();
        if (isTargetCell(current->row, current->col, target))
        {
            return reconstructPath(current);
        }
        visited[current->row][current->col] = true;
        for (int i = 0; i < 4; i++)
        {
            int newRow = current->row + dr[i];
            int newCol = current->col + dc[i];
            // 如果新生成的邻居节点n'已经存在于OPEN集合中,我们不应该直接忽略它,而是比较这两条路径到达n'的代价g值,取其中较小的那条路径。

            Cell *neighbor = new Cell(newRow, newCol);
            neighbor->parent = current;
            // Calculate the g and h values for the neighbor
            neighbor->g = current->g + 1;
            neighbor->calculateHeuristic(target);
            neighbor->f = neighbor->g + neighbor->h;
            open.push(neighbor);
            visited[newRow][newCol] = true;
        }
    }

    // No path found
    return {};
}
// #include <ctime>
// int main()
// {
//     // 记录程序时间
//     //  Define the grid
//     // char grid[200][200];
//     // 从./map/map1.txt中读取地图
//     freopen("./map/map1.txt", "r", stdin);
//     for (int i = 0; i < 200; i++)
//         scanf("%s", grid[i]);
//     fclose(stdin);
//     // Find the start and target cells
//     Cell *start = nullptr;
//     Cell *target = nullptr;
//     for (int i = 0; i < 200; i++)
//     {
//         for (int j = 0; j < 200; j++)
//         {
//             if (grid[i][j] == 'S')
//             {
//                 start = new Cell(i, j);
//             }
//             else if (grid[i][j] == 'D')
//             {
//                 target = new Cell(i, j);
//             }
//         }
//     }

//     // Run the A* algorithm
//     clock_t sta, end;
//     sta = clock();
//     vector<Cell *> path = aStar(grid, start, target);
//     end = clock();
//     // Print the path
//     if (!path.empty())
//     {
//         cout << "Path found:" << endl;
//         for (Cell *cell : path)
//         {
//             cout << "(" << cell->row << ", " << cell->col << ")" << endl;
//         }
//         // 可视化输出路径，将路径中的'.'替换为'@'
//         for (Cell *cell : path)
//         {
//             grid[cell->row][cell->col] = '@';
//         }
//         // 输出到文件solution.txt
//         std::ofstream outputFile("./solution.txt");

//         // Write the grid to the output file
//         for (int i = 0; i < 200; i++)
//         {
//             for (int j = 0; j < 200; j++)
//             {
//                 outputFile << grid[i][j];
//             }
//             outputFile << endl;
//         }
//         // Close the output file
//         outputFile.close();
//     }
//     else
//     {
//         cout << "No path found." << endl;
//     }
//     // 重置stdout为输出到终端
//     // 输出程序运行时间,单位为ms
//     cout << "Time: " << (double)(end - sta) / CLOCKS_PER_SEC * 1000.0 << "ms" << endl;
//     // Clean up memory
//     delete start;
//     delete target;
//     for (Cell *cell : path)
//     {
//         delete cell;
//     }

//     return 0;
// }