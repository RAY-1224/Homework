# 41343114

作業一

## 解題說明

將兩個不同的多項式相加，同指數先加，多得項另外自己計算。

ex: 

     2x^2 + 5X^1
    
     3X^2 + 2x^1 + 5x^0
  
    =5x^2 + 7x^1 + 5x^0

### 解題策略

1. 加入friend讓多載的函式存取物件裡的屬性。
2. 用了一個 Term 結構去存一項的「係數跟指數，再用一個陣列 termArray 來放全部的項。(多項式的項數不一定，所以要用「動態陣列」)
3. 加法的概念是「同指數的項相加，指數不同的照順序放」。
4. 如果一邊比較大 → 把那一項直接放進結果裡，最後再把剩下的項補上。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend Polynomial;
    friend ostream& operator<<(ostream &output, const Polynomial &Poly);
private:
    int exp;  // 指數
    float coef;  // 系數
};

class Polynomial {
private:
    Term *termArray;
    int capacity;  // 容量
    int terms;  // 已經存儲的項目數量
public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];  // 初始化
    }
    
    ~Polynomial() {
        delete[] termArray;  // 釋放記憶體
    }
    
    Polynomial Add(const Polynomial &b);
    Polynomial Mult(const Polynomial &b);  
    float Eval(float x);  // 評估多項式

    void newTerm(const float newcoef, const int newexp);  // 新增項目
    
    friend istream& operator>>(istream &input, Polynomial &Poly);
    friend ostream& operator<<(ostream &output, const Polynomial &Poly);
};

// 輸入多項式
istream& operator>>(istream& is, Polynomial& poly) {
    float coef;
    int exp, n;
    is >> n;  // 讀取多項式的項數
    while (n--) {
        is >> coef >> exp;  // 讀取每一項的系數和指數
        poly.newTerm(coef, exp);
    }
    return is;
}

// 輸出多項式
ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        if (i > 0) os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

// 新增一個項到多項式中
void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {  // 容量不足時，擴展容量
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms++].exp = theExp;
}

// 多項式加法
Polynomial Polynomial::Add(const Polynomial& b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t) c.newTerm(t, termArray[aPos].exp);
            aPos++;
            bPos++;
        } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        } else {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    // 處理剩餘項
    for (; aPos < terms; aPos++) {
        c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
    }
    for (; bPos < b.terms; bPos++) {
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
    }
    return c;
}

int main() {
    Polynomial poly1, poly2, result
    cin >> poly1;
    cin >> poly2;

    result = poly1.Add(poly2);

    cout << result << endl;

    return 0;
}



}
```

## 效能分析

1. 時間複雜度：

    newTerm( )      ---  攤銷 𝑂(1) (擴充時𝑂(t))    

    Add( )          ---  𝑂(n+m)     

    Mult( )         ---  𝑂(nm) ~ 𝑂(nmlog(nm))

    Eval( )         --- 𝑂(t)

 
 2. 空間複雜度：
   
    newTerm( )      ---  𝑂(t)    

    Add( )          ---  𝑂(n+m)     

    Mult( )         ---  𝑂(nm) 

    Eval( )         --- 𝑂(1)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | 3   3 2   2 1   0 5     |         |         |
|           | 2   3 1   1 4|3X^3+1X^2+4X^1+5X^0|3X^3+1X^2+4X^1+5X^0|
| 測試二   |4  4 3  3 -2  2 1  0 6|         |         |
|     |3  3 5  2 -1  0 -3|3X^4+3X^3+0X^2+3X^0|3X^4+3X^3+0X^2+3X^0|


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

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用
