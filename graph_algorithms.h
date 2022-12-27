#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <map>
#include <utility>




//=========================================================================================
//  FUNCTION BFS
//=========================================================================================
template <typename NodeObject,typename GraphObject>
std::map<NodeObject,int> bfs(const NodeObject& root,GraphObject& graphObject){
    //============================================================
    // BREADTH FIRST SEARCH ALGORITHM
    // 
    // INPUT:
    // - root node
    // - graphObject: it must contain member functions
    //   - std::vector<NodeObject> getNeighbours(const NodeObject&) const
    //     (returning the neighbours of a certain node)
    //
    //  OUTPUT:
    //  map (node , distance)
    //
    //============================================================

    std::map<NodeObject,int> distances{};
    distances.insert(std::make_pair(root,0));

    std::queue<std::pair<NodeObject,int>> Q{};
    Q.push(std::make_pair(root,0));

    std::set<NodeObject> visited{};
    visited.insert(root);

    while (!Q.empty()){

        NodeObject node{Q.front().first};
        int cur_dist{Q.front().second};
        Q.pop();

        distances.insert(std::make_pair(node,cur_dist));

        const std::vector<NodeObject> neighbours = graphObject.getNeighbours(node);

        for (auto it=neighbours.begin();it!=neighbours.end();++it){

            auto resultInsert = visited.insert(*it);
            if (resultInsert.second){

                Q.push(std::make_pair(*it,cur_dist+1));

            }

        }

    }

    return distances;
}
//=========================================================================================
//  END OF FUNCTION
//=========================================================================================





//=========================================================================================
//  FUNCTION BFS_WITH_TARGET
//=========================================================================================
template <typename NodeObject,typename GraphObject>
std::pair<NodeObject,int> bfs_with_target(const NodeObject& root,GraphObject& graphObject){
    //============================================================
    // BREADTH FIRST SEARCH ALGORITHM WITH TARGET
    // 
    // INPUT:
    // - root node
    // - graphObject: it must contain member functions
    //   - std::vector<NodeObject> getNeighbours(const NodeObject&) const
    //     (returning the neighbours of a certain node)
    //   - bool isGoal(const NodeObject&) const
    //     (returning if a certain node is the goal)
    //
    //  OUTPUT:
    //  pair (target node , distance)
    //
    //============================================================

    std::queue<std::pair<NodeObject,int>> Q{};
    Q.push(std::make_pair(root,0));

    std::set<NodeObject> visited{};
    visited.insert(root);

    while (!Q.empty()){

        NodeObject node{Q.front().first};
        int cur_dist{Q.front().second};
        Q.pop();

        if (graphObject.isGoal(node)) return std::make_pair(node,cur_dist);

        const std::vector<NodeObject> neighbours = graphObject.getNeighbours(node);

        for (auto it=neighbours.begin();it!=neighbours.end();++it){

            auto resultInsert = visited.insert(*it);
            if (resultInsert.second){

                Q.push(std::make_pair(*it,cur_dist+1));

            }

        }

    }

    return {};
}
//=========================================================================================
//  END OF FUNCTION
//=========================================================================================




//=========================================================================================
//  FUNCTION DFS
//=========================================================================================
template <typename NodeObject,typename GraphObject>
void dfs(const NodeObject& root,const GraphObject& graphObject){
    //============================================================
    // DEPTH FIRST SEARCH ALGORITHM
    // 
    // INPUT:
    // - root node
    // - graphObject: it must contain member functions
    //   - std::vector<NodeObject> getNeighbours(const NodeObject&) const
    //     (returning the neighbours of a certain node)
    //
    //============================================================

    std::stack<NodeObject> S{};
    std::set<NodeObject> visited{};
    S.push(root);

    while (!S.empty()){

        NodeObject node{S.top()};
        S.pop();

        auto resultInsert = visited.insert(node);
        if (resultInsert.second){

            const std::vector<NodeObject> neighbours = graphObject.getNeighbours(node);
            for (auto it=neighbours.begin();it!=neighbours.end();++it){
                S.push(*it);
            }

        }

    }

}
//=========================================================================================
//  END OF FUNCTION
//=========================================================================================



//=========================================================================================
// FUNCTION DIJKSTRA
//=========================================================================================
template <typename NodeObject,typename GraphObject>
int dijkstra(const NodeObject& source,const NodeObject& target,GraphObject& graphObject){
    //============================================================
    // DIJKSTRA ALGORITHM 
    //
    // INPUT:
    // - source node
    // - target node
    // - graphObject: it must contain member function
    //  -std::vector<NodeObject> getNeighbours(const NodeObject&) const;
    //   (returns the neighbours of the node)
    //  -int getCost(const NodeObject&,const NodeObject&) const;
    //   (returns the cost of edge between two nodes)
    //  -void setPrev(const NodeObject&,const NodeObject&);
    //   (saves the second node as prev of the first node)
    //
    //============================================================

    std::set<NodeObject> visited{};
    std::map<NodeObject,int> hasDistance{};
    hasDistance.insert({source,0});

    std::priority_queue<std::pair<int,NodeObject>,std::vector<std::pair<int,NodeObject>>,std::greater<std::pair<int,NodeObject>>> prQ{};

    prQ.push({0,source});
    
    while (!prQ.empty()){

        NodeObject current = prQ.top().second;
        int curDist = prQ.top().first;

        prQ.pop();
        if (visited.find(current)!=visited.end()) continue;
        visited.insert(current);

        if (current == target){
            return curDist;
        }

        std::vector<NodeObject> neighbours{graphObject.getNeighbours(current)};

        for (auto it=neighbours.begin();it!=neighbours.end();++it){

            int temp = curDist + graphObject.getCost(current,*it);

            auto found = hasDistance.find(*it);
            if (found!=hasDistance.end()){

                if (temp<found->second){
                    found->second = temp;
                    graphObject.setPrev(*it,current);
                    prQ.push({temp,*it});
                }

            }else{

                prQ.push({temp,*it});
                hasDistance.insert({*it,temp});
                graphObject.setPrev(*it,current);

            }
            
        }
        
    }

    return INT_MAX;
}
//=========================================================================================
//  END OF FUNCTION
//=========================================================================================





#endif