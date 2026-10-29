
class Solution {
    vector<vector<int>> adj;
    vector<int> parent, depth, sz, heavy, head, pos;
    vector<int> seg;
    int timer = 0;

    void build(int n, string& s) {
        // Iterative DFS: establish parents and depths.
        vector<int> order;
        order.reserve(n);
        order.push_back(0);
        parent[0] = -1;

        for (int i = 0; i < (int)order.size(); ++i) {
            int u = order[i];

            for (int v : adj[u]) {
                if (v == parent[u]) continue;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                order.push_back(v);
            }
        }

        // Compute subtree sizes and heavy children bottom-up.
        for (int i = n - 1; i >= 0; --i) {
            int u = order[i];
            sz[u] = 1;
            int bestSize = 0;

            for (int v : adj[u]) {
                if (parent[v] != u) continue;

                sz[u] += sz[v];
                if (sz[v] > bestSize) {
                    bestSize = sz[v];
                    heavy[u] = v;
                }
            }
        }

        // Decompose heavy chains iteratively.
        vector<pair<int, int>> st;
        st.push_back({0, 0});

        while (!st.empty()) {
            auto [u, h] = st.back();
            st.pop_back();

            for (int v = u; v != -1; v = heavy[v]) {
                head[v] = h;
                pos[v] = timer++;

                for (int w : adj[v]) {
                    if (parent[w] == v && w != heavy[v]) {
                        st.push_back({w, w});
                    }
                }
            }
        }

        // Build a segment tree storing character bitmasks.
        seg.assign(2 * n, 0);

        for (int u = 0; u < n; ++u) {
            seg[n + pos[u]] = 1 << (s[u] - 'a');
        }

        for (int i = n - 1; i > 0; --i) {
            seg[i] = seg[i * 2] ^ seg[i * 2 + 1];
        }
    }

    void update(int p, int value, int n) {
        p += n;
        seg[p] = value;

        for (p >>= 1; p > 0; p >>= 1) {
            seg[p] = seg[p * 2] ^ seg[p * 2 + 1];
        }
    }

    int rangeQuery(int l, int r, int n) {
        int res = 0;
        l += n;
        r += n;

        while (l <= r) {
            if (l & 1) res ^= seg[l++];
            if (!(r & 1)) res ^= seg[r--];

            l >>= 1;
            r >>= 1;
        }

        return res;
    }

    int pathQuery(int u, int v, int n) {
        int mask = 0;

        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }

            mask ^= rangeQuery(pos[head[u]], pos[u], n);
            u = parent[head[u]];
        }

        if (depth[u] > depth[v]) swap(u, v);

        mask ^= rangeQuery(pos[u], pos[v], n);
        return mask;
    }

public:
    vector<bool> palindromePath(
        int n,
        vector<vector<int>>& edges,
        string s,
        vector<string>& queries
    ) {
        adj.assign(n, {});
        parent.assign(n, -1);
        depth.assign(n, 0);
        sz.assign(n, 0);
        heavy.assign(n, -1);
        head.assign(n, 0);
        pos.assign(n, 0);
        timer = 0;

        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        build(n, s);

        vector<bool> ans;

        for (const string& q : queries) {
            stringstream ss(q);
            string type;
            int u;
            ss >> type >> u;

            if (type == "update") {
                char c;
                ss >> c;
                s[u] = c;
                update(pos[u], 1 << (c - 'a'), n);
            } else {
                int v;
                ss >> v;

                int mask = pathQuery(u, v, n);
                ans.push_back((mask & (mask - 1)) == 0);
            }
        }

        return ans;
    }
};
