class Solution {
public:

    unordered_set<string> st;

    void dfs(string &s, int i,
             int left, int right,
             int remLeft, int remRight,
             string curr) {

        // End of string
        if(i == s.size()) {

            if(remLeft == 0 &&
               remRight == 0 &&
               left == right) {

                st.insert(curr);
            }

            return;
        }

        // Remove current character
        if(s[i] == '(' && remLeft > 0) {

            dfs(s, i + 1,
                left, right,
                remLeft - 1, remRight,
                curr);
        }

        if(s[i] == ')' && remRight > 0) {

            dfs(s, i + 1,
                left, right,
                remLeft, remRight - 1,
                curr);
        }

        // Keep current character
        if(s[i] == '(') {

            dfs(s, i + 1,
                left + 1, right,
                remLeft, remRight,
                curr + '(');
        }

        else if(s[i] == ')') {

            if(left > right) {

                dfs(s, i + 1,
                    left, right + 1,
                    remLeft, remRight,
                    curr + ')');
            }
        }

        else {

            dfs(s, i + 1,
                left, right,
                remLeft, remRight,
                curr + s[i]);
        }
    }

    vector<string> removeInvalidParentheses(string s) {

        int remLeft = 0;
        int remRight = 0;

        // Find minimum removals
        for(char ch : s) {

            if(ch == '(') {
                remLeft++;
            }

            else if(ch == ')') {

                if(remLeft > 0) {
                    remLeft--;
                }
                else {
                    remRight++;
                }
            }
        }

        dfs(s, 0, 0, 0,
            remLeft, remRight, "");

        return vector<string>(st.begin(), st.end());
    }
};