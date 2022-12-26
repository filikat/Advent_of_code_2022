#include <vector>
#include <set>
#include <queue>

#ifndef BFS_H
#define BFS_H


template <typename NodeObject,typename GraphObject>
NodeObject bfs(const NodeObject& root,const GraphObject& graphObject){
    //============================================================
    // BREADTH FIRST SEARCH ALGORITHM
    // 
    // INPUT:
    // - root node
    // - graphObject: it must contain member functions
    //   - std::vector<NodeObject> getNeighbours(const NodeObject&) const
    //     (returning the neighbours of a certain node)
    //   - bool isGoal(const NodeObject&) const
    //     (returning if a certain node is the goal)
    //
    //============================================================

    std::queue<NodeObject> Q{};
    std::set<NodeObject> visited{};
    Q.push(root);
    visited.insert(root);

    while (!Q.empty()){

        NodeObject node{Q.front()};
        Q.pop();

        if (graphObject.isGoal(node)) return node;

        const std::vector<NodeObject> neighbours = graphObject.getNeighbours(node);

        for (auto it=neighbours.begin();it!=neighbours.end();++it){

            auto resultInsert = visited.insert(*it);
            if (resultInsert.second){
                Q.push(*it);
            }

        }

    }

    return {};
}



#endif