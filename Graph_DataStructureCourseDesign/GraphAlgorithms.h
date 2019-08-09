//
// Created by 魏宇翔 on 2019-08-06.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H


#include "Graph.h"
#include <deque>

struct SearchResult {
    // 记录了某种遍历时的下标顺序。
    std::vector<int> indexOrder;
    // 记录了栈、队列的实时信息。
    std::vector<std::deque<int>> containerCondition;
};

// 根据下标进行深度优先搜索。
template<typename Value>
SearchResult deepFirstSearch_recursive_withIndex(const Graph<Value> &graph, int startIndex) {
    // 记录每个顶点是否被访问过
    std::vector<bool> visited(graph.verticesCount(), false);
    SearchResult result;
    struct AnonymousDfsActor {
        SearchResult &result;
        std::vector<bool> &visited;
        const Graph<Value> &graph;

        AnonymousDfsActor(const Graph<Value> &graph, SearchResult &result, std::vector<bool> &visited) :
                result(result), visited(visited), graph(graph) {}

        void dfs_recursive(int startIndex) {
            result.indexOrder.push_back(startIndex);
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
SearchResult deepFirstSearch_recursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_recursive_withIndex(graph, graph.vertexIndexWithValue(value));
}

// 根据下标进行广度优先搜索
template<typename Value>
SearchResult breadthFirstSearch_withIndex(const Graph<Value> &graph, int startIndex) {
    SearchResult result;
    std::vector<bool> visited(graph.verticesCount(), false);
    std::deque<int> openList; // 存储活节点的下标即可
    openList.push_back(startIndex);
    while (!openList.empty()) {
        // 更新队列情况
        result.containerCondition.push_back(openList);
        // 出队
        auto current = openList.front();
        openList.pop_front();
        // 标记为已访问
        visited[current] = true;
        // 访问
        result.indexOrder.push_back(current);

        for (const auto &index : graph.vertexAt(current).getAdjList()) {
            if (!visited[index]) {
                openList.push_back(index);
                visited[index] = true; // !!不增加这个会导致进队多次
            }
        }
    }
    return result;
}

// 根据值进行广度优先搜索
template<typename Value>
SearchResult breadthFirstSearch(const Graph<Value> &graph, const Value &value) {
    return breadthFirstSearch_withIndex(graph, graph.vertexIndexWithValue(value));
}

// 根据下标非递归深度优先搜索
template<typename Value>
SearchResult deepFirstSearch_nonRecursive_withIndex(const Graph<Value> &graph, int startIndex) {
    SearchResult result;
    std::vector<bool> visited(graph.verticesCount(), false);
    // 栈，存放待扩展节点
    std::deque<int> vertices;
    vertices.push_back(startIndex);
    while (!vertices.empty()) {
        // 队列情况更新
        result.containerCondition.push_back(vertices);
        // 出栈
        auto current = vertices.back();
        vertices.pop_back();
        // 存结果
        result.indexOrder.push_back(current);
        // 标记为已访问
        visited[current] = true;
        for (const int &index : graph.vertexAt(current).getAdjList()) {
            if (!visited[index]) {
                visited[index] = true;
                vertices.push_back(index);
            }
        }
    }
    return result;
}

// 根据值进行非递归深度优先搜索
template<typename Value>
SearchResult deepFirstSearch_nonRecursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_nonRecursive_withIndex(graph, graph.vertexIndexWithValue(value));
}


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H
