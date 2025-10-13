//Problem1
#include <iostream>
using namespace std;

int AckermannRecursive(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return AckermannRecursive(m - 1, 1);
    return AckermannRecursive(m - 1, AckermannRecursive(m, n - 1));
}

int main() {
    int m, n;
    cout << "Ackermann 遞迴版\n請輸入 m n（建議 m<=3, n<=6）：";
    if (!(cin >> m >> n)) return 0;

    cout << "A(" << m << "," << n << ") = " << AckermannRecursive(m, n) << endl;
    cout << "⚠️ 注意：遞迴版成長極快，m=3,n>6 可能造成 stack overflow！\n";
    return 0;
}
