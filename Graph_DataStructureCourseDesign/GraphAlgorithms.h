//
// Created by 魏宇翔 on 2019-08-06.
//

#ifndef GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H
#define GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H


#include "Graph.h"
#include <queue>
#include <stack>

// 根据下标进行深度优先搜索。
template<typename Value>
std::vector<Value> deepFirstSearch_recursive_withIndex(const Graph<Value> &graph, int startIndex) {
    // 记录每个顶点是否被访问过
    std::vector<bool> visited(graph.verticesCount(), false);
    std::vector<Value> result;
    struct AnonymousDfsActor {
        std::vector<Value> &result;
        std::vector<bool> &visited;
        const Graph<Value> &graph;

        AnonymousDfsActor(const Graph<Value> &graph, std::vector<Value> &result, std::vector<bool> &visited) :
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
std::vector<Value> deepFirstSearch_recursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_recursive_withIndex(graph, graph.vertexIndexWithValue(value));
}

// 根据下标进行广度优先搜索
template<typename Value>
std::vector<Value> breadthFirstSearch_withIndex(const Graph<Value> &graph, int startIndex) {
    std::vector<Value> result;
    std::vector<bool> visited(graph.verticesCount(), false);
    std::queue<int> openList; // 存储活节点的下标即可
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

// 根据值进行广度优先搜索
template<typename Value>
std::vector<Value> breadthFirstSearch(const Graph<Value> &graph, const Value &value) {
    return breadthFirstSearch_withIndex(graph, graph.vertexIndexWithValue(value));
}

// 根据下标非递归深度优先搜索
template<typename Value>
std::vector<Value> deepFirstSearch_nonRecursive_withIndex(const Graph<Value> &graph, int startIndex) {
    std::vector<Value> result;
    std::vector<bool> visited(graph.verticesCount(), false);
    // 栈，存放待扩展节点
    std::stack<int> vertices;
    vertices.push(startIndex);
    while (!vertices.empty()) {
        // 出栈
        auto current = vertices.top();
        vertices.pop();
        // 存结果
        result.push_back(graph.vertexAt(current).getValue());
        // 标记为已访问
        visited[current] = true;
        for (const int &index : graph.vertexAt(current).getAdjList()) {
            if (!visited[index]) {
                visited[index] = true;
                vertices.push(index);
            }
        }
    }
    return result;
}

// 根据值进行非递归深度优先搜索
template<typename Value>
std::vector<Value> deepFirstSearch_nonRecursive(const Graph<Value> &graph, const Value &value) {
    return deepFirstSearch_nonRecursive_withIndex(graph, graph.vertexIndexWithValue(value));
}


#endif //GRAPH_DATASTRUCTURECOURSEDEIGN_GRAPHALGORITHMS_H
