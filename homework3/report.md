# 41343114

作業三

## 解題說明
本題透過循環鏈結串列與表頭節點來實作一元多項式，並結合 available list 進行節點管理，使多項式在結構設計、運算與記憶體使用上皆具有效率且清楚的實作方式。

ex: 

輸入:    
3 4 5 -2 2 7 0

2 1 3 -1 0

輸出:

P1=3 4 5 -2 2 7 0

P2=2 1 3 -1 0

P1+P2=4 4 5 1 3 -2 2 6 0

P1-P2=4 4 5 -1 3 -2 2 8 0

P1*P2=5 4 8 -6 5 7 3 2 2 -7 0

P1(2)=127



### 解題策略

1. 循環鏈結串列（circular linked list with header node） 實作一元多項式，並提供多項式的輸入、輸出與各種運算功能。
2. 先建立穩定的資料結構，確保多項式能正確地以節點方式儲存與走訪。
3. 將多項式視為一個抽象資料型別（ADT），透過定義好的操作介面與多項式互動。
4. 善用 available list 管理記憶體，以提升節點配置與回收的效率。


## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

/* ===== Term ===== */
struct Term {
    int coef;
    int exp;
};

/* ===== ChainNode ===== */
template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;
    ChainNode(const T& d = T(), ChainNode<T>* l = nullptr)
        : data(d), link(l) {
    }
};

/* ===== Polynomial ===== */
class Polynomial {
private:
    ChainNode<Term>* head;                 // header node
    static ChainNode<Term>* avail;          // available list

    // get a node from avail or new
    ChainNode<Term>* GetNode(const Term& t) {
        if (avail) {
            ChainNode<Term>* x = avail;
            avail = avail->link;
            x->data = t;
            x->link = nullptr;
            return x;
        }
        return new ChainNode<Term>(t);
    }

    // return node to avail
    void RetNode(ChainNode<Term>* x) {
        x->link = avail;
        avail = x;
    }

public:
    /* constructor */
    Polynomial() {
        head = new ChainNode<Term>();
        head->link = head;
    }

    /* copy constructor */
    Polynomial(const Polynomial& a) {
        head = new ChainNode<Term>();
        head->link = head;
        *this = a;
    }

    /* destructor */
    ~Polynomial() {
        ChainNode<Term>* cur = head->link;
        while (cur != head) {
            ChainNode<Term>* temp = cur;
            cur = cur->link;
            RetNode(temp);
        }
        delete head;
    }

    /* assignment operator */
    Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;

        // clear current list
        ChainNode<Term>* cur = head->link;
        while (cur != head) {
            ChainNode<Term>* temp = cur;
            cur = cur->link;
            RetNode(temp);
        }
        head->link = head;

        // copy from a
        ChainNode<Term>* last = head;
        for (ChainNode<Term>* p = a.head->link; p != a.head; p = p->link) {
            ChainNode<Term>* x = GetNode(p->data);
            last->link = x;
            last = x;
        }
        last->link = head;
        return *this;
    }

    /* input */
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n;

        // clear existing
        ChainNode<Term>* cur = x.head->link;
        while (cur != x.head) {
            ChainNode<Term>* temp = cur;
            cur = cur->link;
            x.RetNode(temp);
        }
        x.head->link = x.head;

        ChainNode<Term>* last = x.head;
        for (int i = 0; i < n; i++) {
            Term t;
            is >> t.coef >> t.exp;
            ChainNode<Term>* node = x.GetNode(t);
            last->link = node;
            last = node;
        }
        last->link = x.head;
        return is;
    }

    /* output */
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        int count = 0;
        for (ChainNode<Term>* p = x.head->link; p != x.head; p = p->link)
            count++;

        os << count;
        for (ChainNode<Term>* p = x.head->link; p != x.head; p = p->link)
            os << " " << p->data.coef << " " << p->data.exp;
        return os;
    }

    /* addition */
    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        ChainNode<Term>* aPtr = head->link;
        ChainNode<Term>* bPtr = b.head->link;
        ChainNode<Term>* cLast = c.head;

        while (aPtr != head && bPtr != b.head) {
            Term t;
            if (aPtr->data.exp > bPtr->data.exp) {
                t = aPtr->data;
                aPtr = aPtr->link;
            }
            else if (aPtr->data.exp < bPtr->data.exp) {
                t = bPtr->data;
                bPtr = bPtr->link;
            }
            else {
                t.exp = aPtr->data.exp;
                t.coef = aPtr->data.coef + bPtr->data.coef;
                aPtr = aPtr->link;
                bPtr = bPtr->link;
                if (t.coef == 0) continue;
            }
            cLast->link = c.GetNode(t);
            cLast = cLast->link;
        }

        while (aPtr != head) {
            cLast->link = c.GetNode(aPtr->data);
            cLast = cLast->link;
            aPtr = aPtr->link;
        }

        while (bPtr != b.head) {
            cLast->link = c.GetNode(bPtr->data);
            cLast = cLast->link;
            bPtr = bPtr->link;
        }

        cLast->link = c.head;
        return c;
    }

    /* subtraction */
    Polynomial operator-(const Polynomial& b) const {
        Polynomial negB = b;
        for (ChainNode<Term>* p = negB.head->link; p != negB.head; p = p->link)
            p->data.coef = -p->data.coef;
        return (*this + negB);
    }

    /* multiplication */
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;

        for (ChainNode<Term>* aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
            Polynomial temp;
            ChainNode<Term>* tLast = temp.head;

            for (ChainNode<Term>* bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
                Term t;
                t.coef = aPtr->data.coef * bPtr->data.coef;
                t.exp = aPtr->data.exp + bPtr->data.exp;
                tLast->link = temp.GetNode(t);
                tLast = tLast->link;
            }
            tLast->link = temp.head;
            result = result + temp;
        }
        return result;
    }

    /* evaluate */
    float Evaluate(float x) const {
        float sum = 0;
        for (ChainNode<Term>* p = head->link; p != head; p = p->link)
            sum += p->data.coef * pow(x, p->data.exp);
        return sum;
    }
};

/* static member */
ChainNode<Term>* Polynomial::avail = nullptr;

/* ===== main ===== */
int main() {
    Polynomial p1, p2;

    cout << "Input polynomial 1: ";
    cin >> p1;
    cout << "Input polynomial 2: ";
    cin >> p2;

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;
    cout << "P1 + P2 = " << (p1 + p2) << endl;
    cout << "P1 - P2 = " << (p1 - p2) << endl;
    cout << "P1 * P2 = " << (p1 * p2) << endl;
    cout << "P1(2) = " << p1.Evaluate(2) << endl;

    return 0;
}
```

## 效能分析

|  | 時間複雜度 | 空間複雜度 | 
|----------|--------------|----------|
|operator>>（輸入）|O(n)|O(n)|
|operator<<（輸出）|O(n)|O(1)|
|Copy Constructor|O(n)|O(n)|
|Assignment Operator|O(n)|O(n)|
|Destructor|O(n)|O(1)|
|operator+（加法）|O(m+n)|O(m+n)|
|operator-（減法）|O(m-n)|O(m+n)|
|operator*（乘法）|O(m*n)|O(m*n)|
|Evaluate(x)|O(n)|O(1)|
   
    

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | P1 | P2 |P1+P2|P1*P2|
|---------|--------------|----|----|-----|-----|
| 測試一  |3 4 5 -2 2 7 0
| |2 1 3 -1 0|3 4 5 -2 2 7 0|2 1 3 -1 0|4 4 5 1 3 -2 2 6 0|4 4 5 -1 3 -2 2 8 0|5 4 8 -6 5 7 3 2 2 -7 0|
| 測試二   |2 3 2 4 1
|     |2 5 2 -4 1|2 3 2 4 1|2 5 2 -4 1|1 8 2|2 -2 1|3 15 4 8 3 -16 2|


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



