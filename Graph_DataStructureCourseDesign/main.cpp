#include <iostream>

#include <vector>
#include <memory>
#include <unordered_map>

using std::vector;
using std::shared_ptr;
using std::make_shared;

template<typename Value>
struct Graph {
    // `Graph<Value>::Vertex`，定义在下方。
    struct Vertex;
    vector<Vertex> vertices;
    // 每个顶点值到其所在下标的映射。
    std::unordered_map<Value, int> valueToIndex;

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
        vertices[startIndex].adjointList->push_back(endIndex);
    }

    int vertexIndexWithValue(const Value &value) const { return valueToIndex.at(value); }

    const Vertex &vertexAt(int index) const { return vertices[index]; }

    int verticesCount() const { return vertices.size(); }
};

template<typename Value>
struct Graph<Value>::Vertex {
    Value value;
    // 邻接表，不会产生循环引用，用智能指针自动销毁，
    // `vector<int>`代表顶点在图中所处的下标而非值
    shared_ptr<vector<int>> adjointList;

    // 初始化时指针始终指向空数组，保证访问安全。
    explicit Vertex(const Value &value) :
            value(value), adjointList(make_shared<vector<int>>()) {}

    const Value &getValue() const { return value; }

    const vector<int> &getAdjList() const { return *adjointList; }
};

// 根据下标进行广度优先搜索。
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

// 根据值进行广度优先搜索
template<typename Value>
vector<Value> deepFirstSearch_recursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_recursive(graph, graph.vertexIndexWithValue(value));
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
    auto f = deepFirstSearch_recursive(g, std::string("zero"));
    return 0;
}
