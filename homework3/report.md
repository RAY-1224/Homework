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

1. 循環鏈結串列（circular linked list）搭配表頭節點 實作一元多項式，避免了 NULL 指標的處理，使多項式的插入、刪除與走訪流程更為單純且穩定。
2.  透過 operator overloading（>>、<<、+、-、*），多項式的操作方式與基本型別一致，使程式介面直覺易讀，同時也清楚展現抽象資料型別（ADT）的設計概念。

3.利用指數已排序的特性，採用類似 merge 的方式進行運算，使時間複雜度維持在 O(m + n)，提升運算效率。


## 申論及開發報告

### 選擇循環鏈結串列（Circular Linked List）的原因

1. **多項式項數具有「動態變化」的特性**  
   加減法時:
   
   a.同次方項可能合併為一項。
   
   b.係數相加為 0 時，該項需被刪除。
   
   乘法時:
   
   a.每一項相乘會產生新的項目。
   
   b.項數可能大幅增加。

   鏈結串列可透過指標動態新增或移除節點，使多項式在項數變動時能夠更彈性地處理。

2. **插入與刪除操作的效率考量**  
   a.已知插入或刪除位置時，只需調整指標。
   
   b.時間複雜度為 O(1)。

3. **與陣列結構的比較**


| 比較項目 | 陣列(Array) |循環鏈結陣列(Circular Linked List) |
|---------|--------------|----|
|項目變動|不彈性|彈性高|
|插入/刪除|O(n)|O(1)|
|記憶體使用|連續|非連續|
|邊界處理|需處理index|不需NULL|
|適合多項式|❌|✅|
