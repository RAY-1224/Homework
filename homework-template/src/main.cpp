//Problem1
#include <iostream>
#include <stack>
using namespace std;

int AckermannRecursive(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return AckermannRecursive(m - 1, 1);
    else
        return AckermannRecursive(m - 1, AckermannRecursive(m, n - 1));
}

int AckermannNonRecursive(int m, int n) {
    stack<int> st;
    st.push(m);
    while (!st.empty()) {
        m = st.top();
        st.pop();
        if (m == 0) {
            n = n + 1;
        } 
        else if (n == 0) {
            st.push(m - 1);
            n = 1;
        } 
        else {
            st.push(m - 1);
            st.push(m);
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m, n;
    cout << "Ackermann's Function A(m, n)\n";
    cout << "請輸入 m 與 n（建議 m <= 3, n <= 5）: ";
    cin >> m >> n;

    cout << "\n=== 遞迴版 Recursive ===\n";
    cout << "A(" << m << "," << n << ") = " << AckermannRecursive(m, n) << endl;

    cout << "\n=== 非遞迴版 Non-recursive ===\n";
    cout << "A(" << m << "," << n << ") = " << AckermannNonRecursive(m, n) << endl;

    cout << "\n⚠️ 注意：Ackermann 函數成長極快，請勿輸入太大值！\n";
    return 0;
}
