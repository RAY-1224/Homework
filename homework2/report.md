# 41343114

作業二
--------------------------------------------------
Problem1
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

1. 程式能正確計算多項式的加總。  
2. 更加了解friend的用途，且提高效率。  
3. 測試案例的指數有不同但也能成功，驗證程式的正確性。

## 申論及開發報告

### 選擇friend的原因

在本程式中，使用friend來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   寫法清楚、邏輯直接，特別是像 Add()、Mult() 這種需要大量讀取內部資料的函式。

2. **方便緊密合作的類別設計**  
   a.Term 不會被外部亂改，只開放給 Polynomial 使用。
   
   b.Polynomial 管理 Term 的生命週期與操作權限，資料結構更完整。

3. **提高效率**  
   a.不需要為每個成員寫 getter / setter 函式。

   b.直接存取私有變數，少了一層呼叫，執行效率更好。
--------------------------


Problem2
## 解題說明

將兩個不同的多項式相加，同指數先加，多得項另外自己計算。

ex: 

     2x^2 + 5X^1
    
     3X^2 + 2x^1 + 5x^0
  
    =5x^2 + 7x^1 + 5x^0

### 解題策略

1. 
2.
3. 加法的概念是「同指數的項相加，指數不同的照順序放」。
4. 

## 程式實作

以下為主要程式碼：

```cpp

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

```

## 效能分析

| 函式 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|
| operator>>   | O(n)      | O(n)        |
| operator<<   | O(t)      | O(1)       |


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

1. 程式能正確計算多項式的加總。  
2. 更加了解friend的用途，且提高效率。  
3. 測試案例的指數有不同但也能成功，驗證程式的正確性。

## 申論及開發報告

### 選擇friend的原因

在本程式中，使用friend來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   寫法清楚、邏輯直接，特別是像 Add()、Mult() 這種需要大量讀取內部資料的函式。

2. **方便緊密合作的類別設計**  
   a.Term 不會被外部亂改，只開放給 Polynomial 使用。
   
   b.Polynomial 管理 Term 的生命週期與操作權限，資料結構更完整。

3. **提高效率**  
   a.不需要為每個成員寫 getter / setter 函式。

   b.直接存取私有變數，少了一層呼叫，執行效率更好。

