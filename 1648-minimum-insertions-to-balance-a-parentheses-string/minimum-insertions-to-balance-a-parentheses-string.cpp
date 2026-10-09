class Solution {
public:
    int minInsertions(string s) {

        int open = 0;
        int ans = 0;

        for(int i = 0; i < s.size(); i++) {

            if(s[i] == '(') {
                open++;
            }
            else {

                // If next character is also ')',
                // consume both as a pair
                if(i + 1 < s.size() && s[i + 1] == ')') {
                    i++;
                }
                else {
                    // Insert one ')' to complete the pair
                    ans++;
                }

                // If no '(' is available, insert one
                if(open > 0) {
                    open--;
                }
                else {
                    ans++;
                }
            }
        }

        // Every unmatched '(' needs two ')'
        ans += 2 * open;

        return ans;
    }
};