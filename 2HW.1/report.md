# 41343114

作業一

## 解題說明

Q1:本題先定義抽象類別 MinPQ，其中包含最小優先佇列的基本操作：IsEmpty()、Top()、Push() 與 Pop()。
接著實作 MinHeap 類別並繼承 MinPQ，使用陣列表示完全二元樹來建立最小堆積

### 解題策略

抽象類別 MinPQ 定義操作介面

類別 MinHeap 繼承並實作

採用:Binary Heap
1.可用陣列實作
2.操作效率佳

Heap 性質:父節點 ≤ 子節點


## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
#include <stdexcept>
using namespace std;

// 抽象資料型態：最小優先佇列（Min Priority Queue）
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}  // 虛擬解構子

    // 判斷是否為空
    virtual bool IsEmpty() const = 0;

    // 取得最小元素（回傳參考）
    virtual const T& Top() const = 0;

    // 插入元素
    virtual void Push(const T& x) = 0;

    // 刪除最小元素
    virtual void Pop() = 0;
};

// MinHeap 類別，繼承 MinPQ
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;          // 儲存 heap 的陣列
    int capacity;     // 最大容量
    int heapSize;     // 目前元素數量

    // 當容量不足時，擴充陣列大小（倍增）
    void Resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity + 1];

        // 複製原本資料到新陣列
        for (int i = 1; i <= heapSize; i++) {
            newHeap[i] = heap[i];
        }

        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

public:
    // 建構子
    MinHeap(int theCapacity = 10) {
        capacity = theCapacity;
        heapSize = 0;
        heap = new T[capacity + 1];  // index 從 1 開始使用
    }

    // 解構子
    ~MinHeap() {
        delete[] heap;
    }

    // 判斷 heap 是否為空
    bool IsEmpty() const override {
        return heapSize == 0;
    }

    // 回傳最小值（位於根節點）
    const T& Top() const override {
        if (IsEmpty()) {
            throw runtime_error("Heap 是空的");
        }
        return heap[1];
    }

    // 插入元素（維持 Min Heap 性質）
    void Push(const T& x) override {
        // 若容量不足，進行擴充
        if (heapSize == capacity) {
            Resize();
        }

        int i = ++heapSize;

        // 向上調整（Percolate Up）
        // 若新元素比父節點小，就往上移動
        while (i != 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }

        heap[i] = x;
    }

    // 刪除最小元素（根節點）
    void Pop() override {
        if (IsEmpty()) {
            throw runtime_error("Heap 是空的");
        }

        // 取出最後一個元素
        T lastElement = heap[heapSize--];

        int parent = 1;
        int child = 2;

        // 向下調整（Percolate Down）
        while (child <= heapSize) {
            // 找出較小的子節點
            if (child < heapSize && heap[child + 1] < heap[child]) {
                child++;
            }

            // 若已符合 heap 性質，停止
            if (lastElement <= heap[child]) {
                break;
            }

            // 將較小子節點往上移
            heap[parent] = heap[child];
            parent = child;
            child *= 2;
        }

        // 放入最後元素
        heap[parent] = lastElement;
    }

    // 印出 heap 內容（測試用）
    void PrintHeap() const {
        for (int i = 1; i <= heapSize; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};

// 測試主程式
int main() {
    MinHeap<int> h;

    // 插入元素
    h.Push(20);
    h.Push(15);
    h.Push(30);
    h.Push(5);
    h.Push(10);
    h.Push(12);

    cout << "Heap 內容: ";
    h.PrintHeap();

    cout << "最小值 = " << h.Top() << endl;

    // 刪除最小值
    h.Pop();
    cout << "刪除後: ";
    h.PrintHeap();

    cout << "新的最小值 = " << h.Top() << endl;

    return 0;
}
```

-------------------------------

## 效能分析
Q1:
| 函式 | 時間複雜度 | 空間複雜度 |
|----------|--------------|----------|
| IsEmpty() | O(1)     | O(1)       |
| Top()     | O(1)     | O(1)       |
| Push()    | O(log n) | O(1)       |
| Pop()     | O(log n) | O(1)       |
| Resize()  | O(n)     | O(n)       |

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數  | 輸出(Heap內容) | 說明 |
|----------|--------------|----------|----------|
|測試一     | Push(20)     | 20               | 第一個元素         |
|          | Push(15)     | 15 20             | 15 < 20 → 上浮     |
|          | Push(30)     | 15 20 30          | 符合 heap          |
|          | Push(5)      | 5 15 30 20        | 5 一路上浮到 root   |
|          | Push(10)     | 5 10 30 20 15     | 10 上浮          |
|          | Push(12)     | 5 10 12 30 20 15  | 調整完成      |
|          | Top()        | 5                 | 最小值 |
|          | Pop()        | 10 15 12 20 30    | 10 上浮 |
|          | Top()        | 10 | 新最小值 |


### 編譯與執行指令

```shell

```

### 結論

1.可應用於需要快速取得最小值的情境，如優先佇列與排程問題

2.本程式利用 MinHeap 有效實作最小優先佇列，在維持效率的同時，能快速取得最小元素並正確處理插入與刪除操作。

## 申論及開發報告

### 選擇Heap的原因

是一種完全二元樹結構，可用陣列實作，節省記憶體空間，能在不進行完整排序的情況下，維持部分排序（parent ≤ child）

1. **可快速取得最小值**  
   根節點即為最小元素
   
   Top() 操作時間複雜度為 O(1)

3. **插入與刪除效率高**  
   Push() 與 Pop() 僅需調整樹的高度
   
   時間複雜度為 O(log n)

5. **相較其他資料結構的優勢**
   
   若使用陣列排序：插入需重新排序 → O(n log n) 或 O(n)
   
   若使用鏈結串列：找最小值需遍歷 → O(n)
   
   Heap 可同時兼顧「快速存取 + 動態更新」
   
---------------------------------------------------------------
## 解題說明

Q2:

### 解題策略

Problem2:

1.對第 index 個元素做二擇一：不選 / 選；用遞迴展開到尾（index==n）就輸出。

2.用固定陣列裝元素與選取狀態。

## 程式實作

以下為主要程式碼：

Problem2:
```cpp
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


```
## 效能分析

Problem2:  
1. 時間複雜度：T(n)=O(n×2^n)。
2. 空間複雜度：空間複雜度為S(n)=O(n)。

## 測試與驗證

### 測試案例

| 測試案例 | 元素數 n  | 輸入內容    | 實際數量輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 1$      |    a    |    2     |
| 測試二   | $n = 2$      |     AＢ   |    4     |
| 測試三   | $n = 3$      |      １　２　３   |    8     |
| 測試四   | $n = 5$      |    Ａ B c d e   |    32    |
| 測試五   | $n = 25$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
輸入元素個數:1
輸入1個元素(空白分隔)a

所有子集合(2^1):
{}
{a}
```

### 結論

1. 程式在 n≤5 時執行迅速且輸出完整，但當 n 過大時（例如 n≥20），輸出量成指數成長，導致運算時間與輸出資料量過大而無法實際執行。
2. 冪集演算法屬於 指數級時間複雜度 (O(2ⁿ)) 的問題，顯示在處理組合爆炸問題時需控制輸入規模。
3. 以 遞迴法 產生集合的所有子集合（冪集），驗證了當集合有 n 個元素時，總共有 2ⁿ 個子集合 的理論結果。

## 申論及開發報告

### 選擇遞迴的原因

Problem 2 — Powerset

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **遞迴結構最符合問題本質**  
   每個元素只有「取或不取」兩種狀態，遞迴能自然地分支出所有可能組合，並於遞迴終止條件（index == n）時輸出當前子集合。

2. **程式結構簡單且易於理解**  
   使用遞迴不需額外的巢狀迴圈，只需一個控制 index 的參數與布林陣列 chosen[]，即可有系統地生成所有子集合。
3. **回溯法 (Backtracking) 的應用示範**  
   每次遞迴呼叫都代表一個決策分支，返回時自動撤銷上一步的選擇。

