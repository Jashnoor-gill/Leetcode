class Solution {
public:
    string s;
    int i;

    set<string> parseExpression() {
        set<string> result = parseTerm();

        while (i < s.size() && s[i] == ',') {
            i++;
            set<string> next = parseTerm();

            result.insert(next.begin(), next.end());
        }

        return result;
    }

    set<string> parseTerm() {
        set<string> result = {""};

        while (i < s.size() && s[i] != '}' && s[i] != ',') {
            set<string> cur;

            if (s[i] == '{') {
                i++;
                cur = parseExpression();
                i++; // skip '}'
            }
            else {
                cur.insert(string(1, s[i]));
                i++;
            }

            set<string> temp;

            for (string a : result) {
                for (string b : cur) {
                    temp.insert(a + b);
                }
            }

            result = temp;
        }

        return result;
    }

    vector<string> braceExpansionII(string expression) {
        s = expression;
        i = 0;

        set<string> ans = parseExpression();

        return vector<string>(ans.begin(), ans.end());
    }
};