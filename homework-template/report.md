# 41343114

作業一

## 解題說明

Problem1:本題要求實作阿克曼函數分別為遞迴跟非遞迴。

Problem2:
### 解題策略

Problem1:

遞迴版:

1.每次呼叫 A(m, n) 會再呼叫自己：
若 m = 0 → 直接回傳 n + 1

若 n = 0 → 呼叫 A(m – 1, 1)

其他情況 → 呼叫 A(m – 1, A(m, n – 1))

重點:內層A(m, n – 1)要先算，再代回外層計算。

2. 終止條件:
當 m = 0 時停止。

非遞迴版:

1.用一個 stack <int> 來代替系統的呼叫堆疊。

2.每次遞迴時，把 m「暫存」進堆疊裡。

3.每次返回時，再把它拿出來（pop）

-----------------------------------
Problem2:

## 程式實作

以下為主要程式碼：
Problem1:
```cpp
#include <iostream>
#include <stack>   // 非遞迴版會用到 stack
using namespace std;

/*------------------------------------------------------------
  函式定義：Ackermann’s function A(m, n)
  
  數學定義：
      A(m, n) =
        n + 1                 , if m = 0
        A(m - 1, 1)           , if m > 0 and n = 0
        A(m - 1, A(m, n - 1)) , if m > 0 and n > 0
------------------------------------------------------------*/

// 🔹 遞迴版 (Recursive Version)
int AckermannRecursive(int m, int n) {
    if (m == 0)
        return n + 1;

    else if (n == 0)
        return AckermannRecursive(m - 1, 1);

    else
        return AckermannRecursive(m - 1, AckermannRecursive(m, n - 1));
}

----------------------------------------------------------------
// 🔹 非遞迴版 (Non-recursive Version)
int AckermannNonRecursive(int m, int n) {
    stack<int> st;        // 用來記錄每次呼叫的 m 值
    st.push(m);           // 先把初始 m 放進 stack

    while (!st.empty()) {
        m = st.top();     // 取出最上層的 m
        st.pop();         // 彈出代表「要處理這層」

        if (m == 0) {
            n = n + 1;
        } 
        else if (n == 0) {
            st.push(m - 1);  // 把下一層要算的 m 放入 stack
            n = 1;           // 更新 n 值
        } 
        else {
            // 因為要先算 A(m, n - 1)，再算外層 A(m - 1, ...)
            st.push(m - 1);  // 外層呼叫 A(m - 1, ...)
            st.push(m);      // 內層呼叫 A(m, n - 1)
            n = n - 1;       // 先讓 n - 1，等內層算完再回來
        }
    }

    // 當 stack 清空時，n 即為最終結果
    return n;
}



// 🔹 主程式 (Main)
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

```
Problem2:
```cpp


```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(\log n)$。
2. 空間複雜度：空間複雜度為 $O(100\times \log n + \pi)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |

|----------|--------------|----------|----------|
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
