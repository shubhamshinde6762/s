#include <bits/stdc++.h>
using namespace std;
class Solution
{
public:
    unordered_map<int, int> memo;
    int removeDigits(int n)
    {
        if (n == 0)
            return 0;
        if (memo.count(n))
            return memo[n];
        int steps = INT_MAX;
        int temp = n;
        while (temp > 0)
        {
            int digit = temp % 10;
            temp /= 10;
            if (digit != 0)
                steps = min(steps, 1 + removeDigits(n - digit));
        }
        return memo[n] = steps;
    }
};
int main()
{
    int n = 27;
    Solution obj;
    cout << obj.removeDigits(n) << endl;

    for (auto &[key, value] : obj.memo)
    {
        cout << "n: " << key << ", steps: " << value << endl;
    }
    return 0;
}