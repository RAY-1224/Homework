//Problem1
#include <iostream>
using namespace std;

int AckermannNonRecursive(int m, int n) {
    const int MAX = 200000;  // 模擬堆疊上限
    int st[MAX];
    int top = -1;
    st[++top] = m;

    while (top >= 0) {
        m = st[top--];
        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            if (top + 1 >= MAX) return -1;  // 溢位保護
            st[++top] = m - 1;
            n = 1;
        } else {
            if (top + 2 >= MAX) return -1;
            st[++top] = m - 1;
            st[++top] = m;
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "Ackermann 非遞迴版\n請輸入 m n（建議 m<=3, n<=6）：";
    if (!(cin >> m >> n)) return 0;

    int ans = AckermannNonRecursive(m, n);
    if (ans >= 0)
        cout << "A(" << m << "," << n << ") = " << ans << endl;
    else
        cout << "堆疊溢位（輸入太大）！" << endl;

    return 0;
}

