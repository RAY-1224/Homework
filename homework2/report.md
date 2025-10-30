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

// Term class 用來儲存一項的係數與指數
class Term {
    friend class Polynomial;  
    friend ostream& operator<<(ostream& output, const Polynomial& Poly);
private:
    int exp;    // 指數
    float coef; // 係數
};

// Polynomial 類別：用動態陣列表示多項式
class Polynomial {
private:
    Term* termArray; // 指向 Term 陣列
    int capacity;    // 陣列容量
    int terms;       // 目前多項式的項數
public:
    Polynomial() : capacity(2), terms(0) { termArray = new Term[capacity]; }
    ~Polynomial() { delete[] termArray; }

    // 拷貝建構子
    Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }

    // 指派運算子
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        Term* newArr = new Term[other.capacity];
        for (int i = 0; i < other.terms; ++i) newArr[i] = other.termArray[i];
        delete[] termArray;
        termArray = newArr;
        capacity = other.capacity;
        terms = other.terms;
        return *this;
    }

    void newTerm(const float newcoef, const int newexp);
    Polynomial Add(const Polynomial& b) const;

    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
};

// operator>>：輸入多項式
istream& operator>>(istream& is, Polynomial& poly) {
    float coef; int exp, n;
    is >> n;
    poly.terms = 0;                 // ★ 清空舊內容
    while (n--) {
        is >> coef >> exp;
        poly.newTerm(coef, exp);    // 請給降冪輸入
    }
    return is;
}

// operator<<：輸出多項式
ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        if (i > 0) os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

// Add()：兩個多項式相加（假設兩邊皆降冪）
Polynomial Polynomial::Add(const Polynomial& b) const {
    Polynomial r;
    int i = 0, j = 0;
    while (i < terms && j < b.terms) {
        if (termArray[i].exp == b.termArray[j].exp) {
            float s = termArray[i].coef + b.termArray[j].coef;
            if (s) r.newTerm(s, termArray[i].exp);
            ++i; ++j;
        }
        else if (termArray[i].exp < b.termArray[j].exp) {
            r.newTerm(b.termArray[j].coef, b.termArray[j].exp);
            ++j;
        }
        else {
            r.newTerm(termArray[i].coef, termArray[i].exp);
            ++i;
        }
    }
    while (i < terms) { r.newTerm(termArray[i].coef, termArray[i].exp); ++i; }
    while (j < b.terms) { r.newTerm(b.termArray[j].coef, b.termArray[j].exp); ++j; }
    return r;
}

// newTerm()：append（不排序）
void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; ++i) temp[i] = termArray[i];
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    ++terms;
}

int main() {
    Polynomial a, b, c;
    cin >> a >> b;
    c = a.Add(b);
    cout << c << endl;
    return 0;
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
C:\Users\user\source\repos\Polynomial\x64\Debug\Polynomial.exe
```

### 結論

1. 程式能正確計算多項式的加總。  
2. 更加了解friend的用途，且提高效率。  
3. 測試案例的指數有不同但也能成功，驗證程式的正確性。

## 申論及開發報告

### 選擇friend的原因


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

1. 使用運算子多載 >> 與 <<。
2. 每次新增一項時檢查容量是否足夠，不足就自動倍增。
3. 若係數為 0，直接忽略，不浪費儲存空間。
4. 加法的概念是「同指數的項相加，指數不同的照順序放」。


## 程式實作

以下為主要程式碼：

```cpp

istream& operator>>(istream& is, Polynomial& poly) {
    float coef; int exp, n;
    is >> n;
    poly.terms = 0;
    while (n--) {
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}

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
C:\Users\user\source\repos\Polynomial\x64\Debug\Polynomial.exe
```

### 結論

1. 只需輸入項數與各項數值即可建立多項式物件。  
2. 使用運算子多載，讓多項式以標準數學形式呈現。  
3. 測試值必須是降冪排列才不會出問題。

## 申論及開發報告

### 選擇operator >> <<的原因


1. **讓物件輸入更直覺、像基本型別一樣用**  
   a.C++ 內建型別（int、float）可以 cin >> x。

     多載之後，我們的 Polynomial 也能 cin >> poly;

   b.使用者不必知道內部細節（例如 termArray），只要照格式輸入即可。。

2. **封裝輸入格式的細節**  
   在 operator>> 內部，我們決定如何解析輸入（例如先讀項數 n，再讀每項 coef exp）。
   這樣外部程式只要知道怎麼輸入就好，不需要知道內部陣列結構。


