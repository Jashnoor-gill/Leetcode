
class Solution {
public:
    int shortestPath(int n, vector<vector<int>>& edges,
                     string labels, int k) {

        vector<vector<pair<int, int>>> adj(n);
        for (auto &e : edges) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
        }
        const long long INF = LLONG_MAX;
        vector<vector<long long>> dist(
            n, vector<long long>(k + 1, INF)
        );
        using State = tuple<long long, int, int>;
        priority_queue<
            State,
            vector<State>,
            greater<State>
        > pq;

        // Starting node itself contributes one character
        dist[0][1] = 0;
        pq.push({0, 0, 1});

        while (!pq.empty()) {
            auto [d, u, cnt] = pq.top();
            pq.pop();

            if (d != dist[u][cnt])
                continue;

            // First valid destination popped is optimal
            if (u == n - 1)
                return (int)d;

            for (auto &[v, w] : adj[u]) {

                int newCnt;

                if (labels[u] == labels[v])
                    newCnt = cnt + 1;
                else
                    newCnt = 1;

                if (newCnt > k)
                    continue;

                long long newDist = d + w;

                if (newDist < dist[v][newCnt]) {
                    dist[v][newCnt] = newDist;
                    pq.push({newDist, v, newCnt});
                }
            }
        }

        return -1;
    }
};
