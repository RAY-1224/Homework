#include <iostream>
#include <string>
using namespace std;

void PowerSetRecursive(string S[], int n, int index, bool chosen[]) {
    if (index == n) {
        cout << "{";
        bool first = true;
        for (int i = 0; i < n; ++i) {
            if (chosen[i]) {
                if (!first) cout << ", ";
                cout << S[i];
                first = false;
            }
        }
        cout << "}\n";
        return;
    }
    // 不選 S[index]
    chosen[index] = false;
    PowerSetRecursive(S, n, index + 1, chosen);
    // 選 S[index]
    chosen[index] = true;
    PowerSetRecursive(S, n, index + 1, chosen);
}

int main() {
    int n;
    cout << "Powerset：請輸入元素個數 n：";
    if (!(cin >> n) || n <= 0 || n > 30) return 0;

    string S[30];
    bool chosen[30];
    for (int i = 0; i < n; ++i) chosen[i] = false;

    cout << "請輸入 " << n << " 個元素（空白分隔）：";
    for (int i = 0; i < n; ++i) cin >> S[i];

    cout << "\n所有子集合（共 2^" << n << " 個）：\n";
    PowerSetRecursive(S, n, 0, chosen);
    return 0;
}


