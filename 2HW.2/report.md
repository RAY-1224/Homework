# 41343108 / 41343114

資料結構(二)作業二

## 解題說明

本題主要實作 Graph 圖形結構，並使用 Adjacency List（鄰接串列） 作為圖的表示方法。

Graph 是由 Vertex（頂點） 與 Edge（邊） 組成的資料結構，可以用來表示節點之間的關係。例如地圖路線、網路連線、社群關係、課程先修關係等，都可以使用 Graph 來表示。

-------------
加權圖與無加權圖說明:

無加權圖：

每一條邊只表示兩個頂點之間有連接關係，沒有額外的成本或距離。
```
0 -- 1
0 -- 2
```
加權圖：

每一條邊除了表示連接關係外，還會附加一個權重，例如距離、時間、費用或成本。
```
0 --5-- 1
0 --8-- 2
```
0 到 1 的成本是 5

0 到 2 的成本是 8

### 解題策略

本題採用：Adjacency List

1.可用 vector<vector<int>> 實作

2.比 Adjacency Matrix 更節省空間

3.適合邊數較少的圖

4.方便 DFS 與 BFS 走訪相鄰節點

Graph 性質：
```
Graph = Vertex + Edge
```
Adjacency List 表示方式：
```
每一個頂點後面，記錄與它相鄰的頂點
```


## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Graph 類別
class Graph {
private:
    int vertices;                  // 頂點數量
    vector<vector<int>> adjList;    // 鄰接串列

public:
    // 建構子
    Graph(int v) {
        vertices = v;
        adjList.resize(v);
    }

    // 新增邊，這裡使用無向圖
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // DFS 遞迴函式
    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        // 拜訪所有相鄰節點
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                DFSUtil(neighbor, visited);
            }
        }
    }

    // DFS 深度優先搜尋
    void DFS(int start) {
        vector<bool> visited(vertices, false);

        cout << "DFS Result: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // BFS 廣度優先搜尋
    void BFS(int start) {
        vector<bool> visited(vertices, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Result: ";

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << current << " ";

            // 將尚未拜訪的相鄰節點加入 queue
            for (int neighbor : adjList[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        cout << endl;
    }

    // 印出 Adjacency List
    void PrintGraph() {
        cout << "Adjacency List:" << endl;

        for (int i = 0; i < vertices; i++) {
            cout << i << ": ";

            for (int neighbor : adjList[i]) {
                cout << neighbor << " ";
            }

            cout << endl;
        }
    }
};

// 測試主程式
int main() {
    Graph g(6);

    // 建立無向圖
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // 印出圖形
    g.PrintGraph();

    cout << endl;

    // 從頂點 0 開始 DFS
    g.DFS(0);

    // 從頂點 0 開始 BFS
    g.BFS(0);

    return 0;
}
```

-------------------------------

## 效能分析

```
V = 頂點數量
E = 邊的數量
deg(v) = 某個頂點 v 的相鄰邊數量
```
| 函式 / 項目              | 時間複雜度     | 空間複雜度    |
| -------------------- | --------- | -------- |
| 建立 Adjacency List    | O(V)      | O(V)     |
| addEdge()            | O(1)      | O(1)     |
| 查詢某頂點所有相鄰點           | O(deg(v)) | O(1)     |
| 查詢兩點是否相鄰             | O(deg(v)) | O(1)     |
| PrintGraph()         | O(V + E)  | O(1)     |
| DFS()                | O(V + E)  | O(V)     |
| BFS()                | O(V + E)  | O(V)     |
| 整體 Adjacency List 儲存 | —         | O(V + E) |


## 測試與驗證

### 測試案例

本題建立一個有 6 個頂點的無向圖。
```
頂點：0, 1, 2, 3, 4, 5
```

| 邊     | 說明       |
| ----- | -------- |
| 0 - 1 | 0 與 1 相連 |
| 0 - 2 | 0 與 2 相連 |
| 1 - 3 | 1 與 3 相連 |
| 1 - 4 | 1 與 4 相連 |
| 2 - 5 | 2 與 5 相連 |

圖型結構如下:
```
        0
      /   \
     1     2
   /  \     \
  3    4     5
```

輸出結果
| 測試案例 | 操作           | 輸出結果        | 說明         |
| ---- | ------------ | ----------- | ---------- |
| 測試一  | PrintGraph() | 0: 1 2      | 顯示 0 的相鄰節點 |
|      |              | 1: 0 3 4    | 顯示 1 的相鄰節點 |
|      |              | 2: 0 5      | 顯示 2 的相鄰節點 |
|      |              | 3: 1        | 顯示 3 的相鄰節點 |
|      |              | 4: 1        | 顯示 4 的相鄰節點 |
|      |              | 5: 2        | 顯示 5 的相鄰節點 |
| 測試二  | DFS(0)       | 0 1 3 4 2 5 | 深度優先搜尋     |
| 測試三  | BFS(0)       | 0 1 2 3 4 5 | 廣度優先搜尋     |


### 編譯與執行指令

```shell
C:\Users\user\source\repos\123\x64\Debug\123.exe
```

### 結論

1.本題利用 Adjacency List 成功建立 Graph 圖形結構。

2.DFS 可以正確完成深度優先搜尋。

3.BFS 可以正確完成廣度優先搜尋。

4.測試結果符合預期的走訪順序。

5.使用 Adjacency List 可以有效節省空間，適合邊數較少的圖。

6.DFS 與 BFS 的時間複雜度皆為 O(V + E)。


## 申論及開發報告

### 選擇Adjacency List的原因

本題選擇使用 Adjacency List（鄰接串列） 作為 Graph 的表示方法。

主要優點是節省空間，只需要記錄實際存在的邊，因此比較適合邊數較少的圖。

1. **節省記憶體空間**  
    Adjacency List 的空間複雜度為：O(V + E)
   
    比 Adjacency Matrix 的 O(V²) 更適合稀疏圖。

2. **方便走訪相鄰節點**  
    在 DFS 和 BFS 中，程式只需要拜訪某個頂點的相鄰節點。

    使用 Adjacency List 可以直接取得相鄰節點，不需要掃描整個矩陣。

3. **適合 DFS 與 BFS 實作**
   
    DFS 和 BFS 都需要不斷檢查相鄰節點。
   
    使用 Adjacency List 可以讓程式更簡潔，也能維持良好的效率。

DFS 與 BFS 比較
| 項目     | DFS               | BFS       |
| ------ | ----------------- | --------- |
| 中文名稱   | 深度優先搜尋            | 廣度優先搜尋    |
| 使用資料結構 | Recursion / Stack | Queue     |
| 搜尋方式   | 先往深處走             | 一層一層搜尋    |
| 適合用途   | 連通性、路徑搜尋、生成樹      | 最短步數、層級搜尋 |
| 時間複雜度  | O(V + E)          | O(V + E)  |

   
