#include <stdio.h>
#include <string.h>

using namespace std;

#define MAXN 10005
typedef long long int ll;

ll weight[MAXN][MAXN];
ll M[MAXN][MAXN];
ll u[MAXN];
ll v[MAXN];
bool match_x[MAXN];
bool visit_y[MAXN];
bool vertex_cover_x[MAXN];
bool vertex_cover_y[MAXN];
int prev_y[MAXN];
int max_nm;

bool dfs(int x)
{
    for (int y = 0; y < max_nm; ++y) {
        if (M[x][y] != 0 || visit_y[y])
            continue;
        visit_y[y] = true;
        if (prev_y[y] == -1 || dfs(prev_y[y])) {
            prev_y[y] = x;
            return true;
        }
    }
    return false;
}

void dfs_mark_alternating_path(int x)
{
    vertex_cover_x[x] = false;
    for (int y = 0; y < max_nm; ++y) {
        if (M[x][y] != 0 || vertex_cover_y[y])
            continue;
        vertex_cover_y[y] = true;
        if (prev_y[y] != -1)
            dfs_mark_alternating_path(prev_y[y]);
    }
}

inline void update_matrix_M()
{
    for (int x = 0; x < max_nm; ++x)
        for (int y = 0; y < max_nm; ++y)
            M[x][y] = u[x] + v[y] - weight[x][y];
}

inline void update_uv()
{
    ll eps = 0x3fff3fff3fff3fffLL;
    for (int x = 0; x < max_nm; ++x)
        for (int y = 0; y < max_nm; ++y)
            if (!vertex_cover_x[x] && !vertex_cover_y[y])
                eps = M[x][y] < eps ? M[x][y] : eps;

    for (int i = 0; i < max_nm; ++i) {
        if (!vertex_cover_x[i])
            u[i] -= eps;
        if (vertex_cover_y[i])
            v[i] += eps;
    }
}

inline int max_matching()
{
    int num_match = 0;
    memset(match_x, 0, sizeof(match_x));
    memset(prev_y, -1, sizeof(prev_y));

    for (int x = 0; x < max_nm; ++x) {
        memset(visit_y, 0, sizeof(visit_y));
        match_x[x] = dfs(x);
        num_match += match_x[x];
    }
    return num_match;
}

inline void min_vertex_cover()
{
    memset(vertex_cover_x, true, sizeof(vertex_cover_x));
    memset(vertex_cover_y, false, sizeof(vertex_cover_y));
    for (int x = 0; x < max_nm; ++x)
        if (match_x[x] == false)
            dfs_mark_alternating_path(x);
}

int main()
{
    // Read size
    int n;
    scanf("%d", &n);
    int m = n;
    if (getchar() == ' ')
        scanf("%d", &m);
    max_nm = n > m ? n : m;

    // Read input
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < m; ++y) {
            scanf("%lld", &weight[x][y]);
            u[x] = weight[x][y] > u[x] ? weight[x][y] : u[x];
        }
    }

    // Hungarian algorithm
    do {
        update_matrix_M();

        if (max_nm == max_matching())
            break;

        min_vertex_cover();

        update_uv();
    } while (true);

    // Calculate ans
    ll sum = 0;
    for (int y = 0; y < max_nm; ++y) {
        sum += weight[prev_y[y]][y];
    }
    printf("%lld\n", sum);

    return 0;
}
