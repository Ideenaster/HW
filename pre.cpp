#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <cstring>
#include <sstream>
#include <math.h>
#include <queue>
using namespace std;
const int MAXLEN = 200;
const int MAXNODE = MAXLEN * MAXLEN;
const unsigned short UNREACH = 255;
const int BERTH_NUM = 10;
vector<unsigned char> mindist[MAXNODE];
// 各个点之间的最短距离,前面两个维度是x0,y0,后面两个是x1,y1;
char grid[MAXLEN][MAXLEN]; // 地图数据,注意边界
// 泊位之间的转移是500帧

struct Berth
{
    int x;
    int y;              // 泊位的左上角坐标
    int transport_time; // 船泊位到虚拟点的时间,同时也是虚拟点到泊位的时间
    int loading_speed;  // 装货时间,单位个/帧
} berth[10];
int boat_capacity; // 船的容积,单位个

bool st[MAXLEN][MAXLEN];
int dx[4] = {0, 0, 1, -1}, dy[4] = {1, -1, 0, 0};
// 表现形式上就是bfs
inline void dijkstra(int num) {
    int y = num % MAXLEN, x = num / MAXLEN;
    if (grid[x][y] == '#') return;
    memset(st, 0, sizeof st);
    queue<pair<unsigned short, pair<int, int>>> heap;//小根堆
    heap.push({ 0,{x,y} });
    st[x][y] = true;
    while (heap.size()) {
        auto t = heap.front();
        heap.pop(); 
        int x0 = t.second.first, y0 = t.second.second;
        int num1 = x0 * MAXLEN + y0 - num;
        unsigned short distance = t.first;
        if (num1 >= 0) {
            mindist[num][num1] = distance;
        }
        for (int i = 0; i < 4; i++) {
            int x = x0 + dx[i], y = y0 + dy[i];
            if (x<0 || x>=MAXLEN || y<0 || y>=MAXLEN || st[x][y] || grid[x][y] == '#') continue;
            unsigned short nex = distance + 1;
            if (!st[x][y]) {
                heap.push({ nex,{x,y} });
                st[x][y] = true;
            }
        }
    }
}

// 调用最短距离的函数
inline unsigned short getdis(int x0, int y0, int x1, int y1)
{
    int num0 = x0 * MAXLEN + y0, num1 = x1 * MAXLEN + y1;
    if (num0 > num1)
        swap(num0, num1);
    return mindist[num0][num1 - num0];
}
#include <ctime>
inline void Init()
{
    for (int i = 0; i < MAXNODE; i++)
    {
        mindist[i].resize(MAXNODE - i, UNREACH); // 之后认为距离大于等于UNREACH就不可到达
    }
    for (int x0 = 0; x0 < MAXLEN; x0++)
    {
        for (int y0 = 0; y0 < MAXLEN; y0++)
        {
            int num = x0 * MAXLEN + y0;
            dijkstra(num);
            if (num % 10000 == 0)
            {
                printf("%d\n", num);
            }
        }
    }
    printf("OK\n");
    fflush(stdout);
}
