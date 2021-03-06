/*

一、理解题意（识破出题人的意图）
1.跑步者能被第i个观察员观察到的条件：经过i,且到达点i时，用时为w[i]，注意跑步者可以来自点i的上下两个方向
2.从什么时候开始计时？即起跑线在哪里？
3.本题的时间等价于距离，可以用距离来算

二、数据结构（如何高效的存储和查询各种数据）

本题与一般的树形差分不同，因为考虑两个方向过来的人，要将一条路径分成两条链，分别做差分
差分打标记的方法也很特别，因为要记录经过这条路径上的不同的人（即不同的深度）的次数，
即要统计的是很多种类的量的分别的累加和，而不是像一般差分那样一个量的累加，就不能直接加加或减减一个固定的值，
所以在打标记的点的地方通过vector把在此处具体要加或减的每个量记录下来，在dfs的过程中，用一个统计数组统计每个点的各种作用量
这个统计数组就保存了到目前搜索点i为止的满足d[i] + w[i]的各个深度（由差分数组的下标表示）的跑步者数量，或满足w[i] - d[i]的d[x]-2d[z]的跑步者数量

差分打标记的区间即跑的路径，显然跑这条路径的跑步人，能到这条路径上的所有点

*/

#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<vector>
#include<queue>
using namespace std;
const int SIZE = 300010;
int ver[SIZE * 2], Next[SIZE * 2], head[SIZE], tot;
int f[SIZE][20], d[SIZE], w[SIZE], v[SIZE];
int c1[SIZE * 2], c2[SIZE * 2], ans[SIZE];
int n, m, t;
queue<int> q;
vector<int> a1[SIZE], b1[SIZE], a2[SIZE], b2[SIZE];

void add(int x, int y) {
    ver[++tot] = y, Next[tot] = head[x], head[x] = tot;
}

void bfs() {
    t = log(n) / log(2);
    q.push(1); d[1] = 1;
    while (q.size()) {
        int x = q.front(); q.pop();
        for (int i = head[x]; i; i = Next[i]) {
            int y = ver[i];
            if (d[y]) continue;
            d[y] = d[x] + 1;
            f[y][0] = x;
            for (int j = 1; j <= t; j++)
                f[y][j] = f[f[y][j - 1]][j - 1];
            q.push(y);
        }
    }
}

int lca(int x, int y) {
    if (d[x] > d[y]) swap(x, y);
    for (int i = t; i >= 0; i--)
        if (d[f[y][i]] >= d[x]) y = f[y][i];
    if (x == y) return x;
    for (int i = t; i >= 0; i--)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}

void dfs(int x) {
    int val1 = c1[d[x] + w[x]], val2 = c2[w[x] - d[x] + n];
    v[x] = 1;
    for (int i = head[x]; i; i = Next[i]) {
        int y = ver[i];
        if (v[y]) continue;
        dfs(y);
    }
    for (int i = 0; i < a1[x].size(); i++) c1[a1[x][i]]++;
    for (int i = 0; i < b1[x].size(); i++) c1[b1[x][i]]--;
    for (int i = 0; i < a2[x].size(); i++) c2[a2[x][i] + n]++;
    for (int i = 0; i < b2[x].size(); i++) c2[b2[x][i] + n]--;
    ans[x] += c1[d[x] + w[x]] - val1 + c2[w[x] - d[x] + n] - val2;
    //减去val1是减去进入x点之前的统计的那些深度也等于d[x]+w[x]的点的数量，即减去不在x的子树内的点
}

int main() {
    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int x, y; scanf("%d%d", &x, &y);
        add(x, y), add(y, x);
    }
    for (int i = 1; i <= n; i++) scanf("%d", &w[i]);
    bfs();
    for (int i = 1; i <= m; i++) {
        int x, y; scanf("%d%d", &x, &y);
        int z = lca(x, y);
        a1[x].push_back(d[x]);//x到lca(x,y)的路径上的所有点都有个从下方来的d[x]的人
        b1[f[z][0]].push_back(d[x]);
        a2[y].push_back(d[x] - 2 * d[z]);//y到lca(x,y)的路径上的所有点都有个从上方来的d[x]-2d[z]的人
        b2[z].push_back(d[x] - 2 * d[z]);
    }
    dfs(1);
    for (int i = 1; i < n; i++) printf("%d ", ans[i]);
    printf("%d\n", ans[n]);
}
