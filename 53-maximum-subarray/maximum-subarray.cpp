class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n=nums.size();
        int f=nums[0];
        int g=nums[0];
        for(int i=1;i<n;i++){
            g=max(nums[i],g+nums[i]);
            f=max(g,f);
        }
        return f;
    }
};