#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int u = 2010, w = 3000010;
int ver[w], Next[w], head[u], dfn[u], low[u], c[u], s[u], ins[u];
int val[u], deg[u], opp[u], S[u], T[u], D[u];
int n, m, tot, num, t, p;

// 原图加边
void add(int x, int y) {
	ver[++tot] = y, Next[tot] = head[x], head[x] = tot;
}

void tarjan(int x) {
	dfn[x] = low[x] = ++num;
	s[++p] = x, ins[x] = 1;
	for (int i = head[x]; i; i = Next[i])
		if (!dfn[ver[i]]) {
			tarjan(ver[i]);
			low[x] = min(low[x], low[ver[i]]);
		}
		else if (ins[ver[i]])
			low[x] = min(low[x], low[ver[i]]);
	if (dfn[x] == low[x]) {//极大强连通分量
		t++; int y;
		do { y = s[p--], ins[y] = 0; c[y] = t; } while (x != y);
	}
}

bool overlap(int a, int b, int c, int d) {
	if (a >= c&&a<d || b>c&&b <= d || a <= c&&b >= d)
        return 1;
	return 0;
}

int main() {
	cin >> n;
	for (int i = 1; i <= n; i++) 	{
        int sh, sm, th, tm;
		scanf("%d:%d %d:%d %d", &sh, &sm, &th, &tm, &D[i]);
		S[i] = sh * 60 + sm; T[i] = th * 60 + tm;
	}
	for (int i = 1; i < n; i++)
		for (int j = i + 1; j <= n; j++) {
            if (overlap(S[i], S[i] + D[i], S[j], S[j] + D[j]))//i开始时举行，j开始时举行有冲突
				add(i, n + j), add(j, n + i);//i开始时与j结束前，j开始时与i结束前
            if (overlap(S[i], S[i] + D[i], T[j] - D[j], T[j]))//i开始时，j结束前有冲突
				add(i, j), add(n + j, n + i);
            if (overlap(T[i] - D[i], T[i], S[j], S[j] + D[j]))//i结束前，j开始时有冲突
				add(n + i, n + j), add(j, i);
            if (overlap(T[i] - D[i], T[i], T[j] - D[j], T[j]))//i结束前，j结束前有冲突
				add(n + i, j), add(n + j, i);
        }
	for (int i = 1; i <= 2 * n; i++)
		if (!dfn[i]) tarjan(i);
	for (int i = 1; i <= n; i++) 	{
		if (c[i] == c[n + i]) { //矛盾，因为同一个变量只能取一个值
                puts("NO"); return 0;
        }
		opp[i] = n + i, opp[n + i] = i;//这是一对，互为对称点
	}
    puts("YES");
    // 构造方案
    for (int i = 1; i <= 2 * n; i++)//循环到2*n结束
    	val[i] = c[i] > c[opp[i]]; // i和opp[i]不在同一个强连通分量，所在分量编号小的赋值为1

    // 输出最终结果
	for (int i = 1; i <= n; i++) //循环到n结束
		if (val[i] == 0) printf("%02d:%02d %02d:%02d\n",
			S[i] / 60, S[i] % 60,
			(S[i] + D[i]) / 60, (S[i] + D[i]) % 60);
		else printf("%02d:%02d %02d:%02d\n",
			(T[i] - D[i]) / 60, (T[i] - D[i]) % 60,
			T[i] / 60, T[i] % 60);
	return 0;
}
