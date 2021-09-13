#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    auto x1 = 0, y1 = 0, r1 = 0, x2 = 0, y2 = 0, r2 = 0, T = 0, ans = 0;
    auto between = 0, rSum = 0;
    cin >> T;
    for (int i = 0; i < T; i++)
    {
        cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
        between = pow((x2 - x1), 2) + pow((y2 - y1), 2);
        rSum = pow((r1 + r2), 2);
        if (between > rSum)
        {
            cout << 0 << endl;
        }
        else if (between < rSum)
        {
            cout << 2 << endl;
        }
        else
        {
            cout << 1 << endl;
        }
    }

    return 0;
}