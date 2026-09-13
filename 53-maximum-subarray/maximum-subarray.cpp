class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n=nums.size();
        vector<int>f(n);
        vector<int>g(n);
        f[0]=nums[0];
        g[0]=nums[0];
        for(int i=1;i<n;i++){
            g[i]=max(nums[i],g[i-1]+nums[i]);
            f[i]=max(g[i],f[i-1]);
        }
        return f[n-1];
    }
};