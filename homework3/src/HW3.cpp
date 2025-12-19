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
        : data(d), link(l) {}
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
            } else if (aPtr->data.exp < bPtr->data.exp) {
                t = bPtr->data;
                bPtr = bPtr->link;
            } else {
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
                t.exp  = aPtr->data.exp  + bPtr->data.exp;
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

