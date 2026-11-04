
class Solution {
public:
    long long minOperations(vector<int>& nums, int x, int k) {
        int n = nums.size();

        multiset<int> low, high;
        long long sumLow = 0, sumHigh = 0;

        // Insert an element
        auto add = [&](int val) {
            if (low.empty() || val <= *low.rbegin()) {
                low.insert(val);
                sumLow += val;
            } else {
                high.insert(val);
                sumHigh += val;
            }
        };

        // Remove an element
        auto remove = [&](int val) {
            auto it = low.find(val);

            if (it != low.end()) {
                low.erase(it);
                sumLow -= val;
            } else {
                it = high.find(val);
                high.erase(it);
                sumHigh -= val;
            }
        };

        // Maintain the smaller half in low
        auto rebalance = [&]() {
            int target = (low.size() + high.size() + 1) / 2;

            while (low.size() > target) {
                int val = *low.rbegin();
                low.erase(prev(low.end()));
                sumLow -= val;

                high.insert(val);
                sumHigh += val;
            }

            while (low.size() < target) {
                int val = *high.begin();
                high.erase(high.begin());
                sumHigh -= val;

                low.insert(val);
                sumLow += val;
            }
        };

        // Calculate cost of current window
        auto getCost = [&]() -> long long {
            long long median = *low.rbegin();

            return median * low.size() - sumLow
                 + sumHigh - median * high.size();
        };

        // Calculate cost of every window of size x
        vector<long long> cost;

        for (int i = 0; i < n; i++) {
            add(nums[i]);

            if (i >= x) {
                remove(nums[i - x]);
            }

            rebalance();

            if (i >= x - 1) {
                cost.push_back(getCost());
            }
        }

        // DP: previous[j] = minimum cost using j windows
        vector<long long> prevDP(n + 1, 0);
        vector<long long> currDP(n + 1);

        const long long INF = LLONG_MAX / 4;

        for (int j = 1; j <= k; j++) {
            fill(currDP.begin(), currDP.end(), INF);

            for (int i = 1; i <= n; i++) {

                // Option 1: Skip current position
                currDP[i] = currDP[i - 1];

                // Option 2: Select a window ending at i-1
                if (i >= x) {
                    currDP[i] = min(
                        currDP[i],
                        prevDP[i - x] + cost[i - x]
                    );
                }
            }

            prevDP.swap(currDP);
        }

        return prevDP[n];
    }
};
