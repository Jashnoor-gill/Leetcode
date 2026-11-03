
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int mx = *max_element(nums.begin(), nums.end());

        int sum = 0;

        for (int x : nums) {
            sum += x;
        }

        return mx * nums.size() - sum;
    }
};
