//
// Created by 魏宇翔 on 2019-08-06.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPH_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPH_H


#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

/// Value should be hashable
template<typename Value>
struct Graph {
private:
    // `Graph<Value>::Vertex`，定义在下方。
    struct Vertex;
    std::vector<Vertex> vertices;
    // 每个顶点值到其所在下标的映射。
    std::unordered_map<Value, int> valueToIndex;

public:
    void appendVertex(const Value &value) {
        vertices.push_back(Vertex(value));
        // `vertices.size() - 1`代表刚刚加入的顶点的下标。
        valueToIndex.insert({value, vertices.size() - 1});
    }

    // 直接通过值添加边。
    void addArc(const Value &start, const Value &end) {
        addArc_withIndex(valueToIndex.at(start), valueToIndex.at(end));
    }

    void addArc_withIndex(int startIndex, int endIndex) {
        vertices[startIndex].adjointList->insert(endIndex);
    }

    // 通过下标删除顶点W(n^2)，复杂度很高。
    void deleteVertex_withIndex(int index) {
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
            auto newSetForVertexI = std::make_shared<std::unordered_set<int>>();
            for (const int &x : vertices[i].getAdjList()) {
                if (x > index) { newSetForVertexI->insert(x - 1); }
                else { newSetForVertexI->insert(x); }
            }
            vertices[i].adjointList = newSetForVertexI;
        }
    };

    // 通过值删除顶点 W(n^2)复杂度
    void deleteVertex(const Value &value) {
        deleteVertex_withIndex(valueToIndex.at(value));
    }

    // 通过值删除边。
    void deleteArc(const Value &start, const Value &end) {
        deleteArc_withIndex(valueToIndex.at(start), valueToIndex.at(end));
    }

    // 通过下标删除边，O(log n)!!!
    void deleteArc_withIndex(int startIndex, int endIndex) {
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
private:
    friend struct Graph<Value>;

    Value value;
    // 邻接表，不会产生循环引用，用智能指针自动销毁，
    // `vector<int>`代表顶点在图中所处的下标而非值
    std::shared_ptr<std::unordered_set<int>> adjointList;

public:
    // 初始化时指针始终指向空数组，保证访问安全。
    explicit Vertex(const Value &value) :
            value(value), adjointList(std::make_shared<std::unordered_set<int >>()) {}

    const Value &getValue() const { return value; }

    const std::unordered_set<int> &getAdjList() const { return *adjointList; }
};


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPH_H
