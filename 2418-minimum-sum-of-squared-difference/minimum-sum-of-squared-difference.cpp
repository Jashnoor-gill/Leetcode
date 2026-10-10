class Solution {
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2,
                               int k1, int k2) {

        int n = nums1.size();
        long long k = (long long)k1 + k2;

        vector<long long> diff(n);
        long long total = 0;

        for(int i = 0; i < n; i++) {
            diff[i] = abs(nums1[i] - nums2[i]);
            total += diff[i];
        }

        if(k >= total) return 0;

        // Binary search the maximum difference we can reduce to
        long long low = 0, high = 100000;

        while(low < high) {
            long long mid = (low + high) / 2;
            long long need = 0;

            for(long long d : diff) {
                if(d > mid) {
                    need += d - mid;
                }
            }

            if(need <= k)
                high = mid;
            else
                low = mid + 1;
        }

        long long level = low;
        long long used = 0;

        // Reduce every difference greater than level
        for(long long &d : diff) {
            if(d > level) {
                used += d - level;
                d = level;
            }
        }

        // Distribute remaining operations by reducing level to level-1
        long long remaining = k - used;

        for(long long &d : diff) {
            if(remaining > 0 && d == level) {
                d--;
                remaining--;
            }
        }

        long long ans = 0;

        for(long long d : diff) {
            ans += d * d;
        }

        return ans;
    }
};