#include <iostream>
#include <algorithm>
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



