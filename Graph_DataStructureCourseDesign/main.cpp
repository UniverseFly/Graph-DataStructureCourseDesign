#include <iostream>

#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>
#include <unordered_set>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::unordered_map;
using std::unordered_set;
using std::queue;

/// Value should be hashable
template<typename Value>
struct Graph {
    // `Graph<Value>::Vertex`，定义在下方。
    struct Vertex;
    vector<Vertex> vertices;
    // 每个顶点值到其所在下标的映射。
    unordered_map<Value, int> valueToIndex;

    void appendVertex(const Value &value) {
        vertices.push_back(Vertex(value));
        // `vertices.size() - 1`代表刚刚加入的顶点的下标。
        valueToIndex.insert({value, vertices.size() - 1});
    }

    // 直接通过值添加边。
    void addArc(const Value &start, const Value &end) {
        addArc(valueToIndex.at(start), valueToIndex.at(end));
    }

    void addArc(int startIndex, int endIndex) {
        vertices[startIndex].adjointList->insert(endIndex);
    }

    // 通过下标删除顶点W(n^2)，复杂度很高。
    void deleteVertex(int index) {
        vertices.erase(vertices.begin() + index);
        // 更新map同时更新被删除的边
        valueToIndex = {};
        for (unsigned long i = 0; i < vertices.size(); ++i) {
            valueToIndex.insert({vertices[i].getValue(), i});
            // 删除邻接边
            vertices[i].adjointList->erase(index);
        }
        // 更新映射下标（因为删除了index，index后的全部被移上来了）
        for (unsigned long i = 0; i < vertices.size(); ++i) {
            auto newSetForVertexI = make_shared<unordered_set<int>>();
            for (const int &x : vertices[i].getAdjList()) {
                if (x > index) { newSetForVertexI->insert(x - 1); }
                else { newSetForVertexI->insert(x); }
            }
            vertices[i].adjointList = newSetForVertexI;
        }
    };

    // 通过值删除顶点 W(n^2)复杂度
    void deleteVertex(const Value &value) {
        deleteVertex(valueToIndex.at(value));
    }

    // 通过值删除边。
    void deleteArc(const Value &start, const Value &end) {
        deleteArc(valueToIndex.at(start), valueToIndex.at(end));
    }

    // 通过下标删除边，O(log n)!!!
    void deleteArc(int startIndex, int endIndex) {
        vertices[startIndex].adjointList->erase(endIndex);
    }

    // 通过值知道下标
    int vertexIndexWithValue(const Value &value) const { return valueToIndex.at(value); }

    // 通过下标知道顶点
    const Vertex &vertexAt(int index) const { return vertices[index]; }

    int verticesCount() const { return vertices.size(); }
};

template<typename Value>
struct Graph<Value>::Vertex {
    Value value;
    // 邻接表，不会产生循环引用，用智能指针自动销毁，
    // `vector<int>`代表顶点在图中所处的下标而非值
    shared_ptr<unordered_set<int>> adjointList;

    // 初始化时指针始终指向空数组，保证访问安全。
    explicit Vertex(const Value &value) :
            value(value), adjointList(make_shared<unordered_set<int>>()) {}

    const Value &getValue() const { return value; }

    const unordered_set<int> &getAdjList() const { return *adjointList; }
};

// 根据下标进行深度优先搜索。
template<typename Value>
vector<Value> deepFirstSearch_recursive(const Graph<Value> &graph, int startIndex) {
    // 记录每个顶点是否被访问过
    vector<bool> visited(graph.verticesCount(), false);
    vector<Value> result;
    struct AnonymousDfsActor {
        vector<Value> &result;
        vector<bool> &visited;
        const Graph<Value> &graph;

        AnonymousDfsActor(const Graph<Value> &graph, vector<Value> &result, vector<bool> &visited) :
                result(result), visited(visited), graph(graph) {}

        void dfs_recursive(int startIndex) {
            result.push_back(graph.vertexAt(startIndex).getValue());
            visited[startIndex] = true;
            // 与startIndex这个顶点所邻接的顶点下标集。
            for (const auto &index : graph.vertexAt(startIndex).getAdjList()) {
                if (!visited[index]) { dfs_recursive(index); }
            }
        }
    };

    AnonymousDfsActor(graph, result, visited).dfs_recursive(startIndex);
    return result;
}

// 根据值进行深度优先搜索
template<typename Value>
vector<Value> deepFirstSearch_recursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_recursive(graph, graph.vertexIndexWithValue(value));
}

// 根据下标进行广度优先搜索
template<typename Value>
vector<Value> breadthFirstSearch(const Graph<Value> &graph, int startIndex) {
    vector<Value> result;
    vector<bool> visited(graph.verticesCount(), false);
    queue<int> openList; // 存储活节点的下标即可
    openList.push(startIndex);
    while (!openList.empty()) {
        // 出队
        auto current = openList.front();
        openList.pop();
        // 标记为已访问
        visited[current] = true;
        // 访问
        result.push_back(graph.vertexAt(current).getValue());

        for (const auto &index : graph.vertexAt(current).getAdjList()) {
            if (!visited[index]) {
                openList.push(index);
                visited[index] = true; // !!不增加这个会导致进队多次
            }
        }
    }
    return result;
}

int main() {
    Graph<std::string> g;
    g.appendVertex("zero");
    g.appendVertex("one");
    g.appendVertex("two");
    g.appendVertex("three");
    g.addArc("zero", "one");
    g.addArc("one", "two");
    g.addArc("zero", "three");
    g.appendVertex("four");
    g.appendVertex("five");
    g.addArc("three", "five");
    g.addArc(3, 4);
    g.addArc("one", "five");
    // g.deleteArc("one", "five");
    g.deleteArc("three", "four");
    g.addArc("three", "four");
    g.deleteVertex("zero");
    auto r1 = deepFirstSearch_recursive(g, std::string("one"));
    auto r2 = breadthFirstSearch(g, 0);
    return 0;
}
