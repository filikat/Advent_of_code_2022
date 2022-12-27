#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <cassert>
#include "../graph_algorithms.h"


using NodeTimeGraph = std::vector<int>;     //vector containing: (node,minute,state of valves)
                                            //state of valves: 0 if not open, min if open at minute min
                                            //For part 2: (node,minute,elephant,minute_elephant,state of valves)


//CLASS FOR PARSING THE INPUT FILE
class InputNode{
    public:
    std::string id{};
    int flow{};
    std::vector<std::string> neighbours{};
};


void print(const NodeTimeGraph& node){
    for (int i: node){
        std::cout<<i<<' ';
    }
    std::cout<<'\n';
}



//INPUT GRAPH WITH ALL NODES
class Graph{
    public:
    std::map<std::string,int> node_id{};
    std::vector<int> node_flows{};
    std::vector<std::vector<int>> neighbours{};
    int goal{};

    void addEdge(std::string,std::string);
    void addNodes(const std::vector<std::string>&);
    void computeDistances();
    std::vector<int> getNeighbours(int) const;
    bool isGoal(int) const;
};
void Graph::addNodes(const std::vector<std::string>& node_names){
    int id{};
    for (std::string s: node_names){
        node_id.insert({s,id});
        ++id;
    }
    neighbours.resize(id);
}
void Graph::addEdge(std::string a,std::string b){
    int id_a = node_id.find(a)->second;
    int id_b = node_id.find(b)->second;

    neighbours[id_a].push_back(id_b);
    neighbours[id_b].push_back(id_a);
}
std::vector<int> Graph::getNeighbours(int node) const{
    std::vector<int> nodeNeigh{};
    for (auto& neigh: neighbours[node]){
        nodeNeigh.push_back(neigh);
    }
    return nodeNeigh;
}
bool Graph::isGoal(int a) const{
    return a==goal;
}

class TimeGraph{
    public:
    int nnodes{};
    std::vector<std::vector<int>> distances{};
    std::vector<int> flows{};
    int best_score{};
    NodeTimeGraph best_node{};
    bool part_one{};

    std::vector<NodeTimeGraph> getNeighbours(const NodeTimeGraph&) const;
    void dfs(const NodeTimeGraph&);
    void checkNode(const NodeTimeGraph&);
};
std::vector<NodeTimeGraph> TimeGraph::getNeighbours(const NodeTimeGraph& timenode) const{

    int node = timenode[0];
    int minute = timenode[1];
    std::vector<NodeTimeGraph> neighbours{};

    if (part_one){
        if (node==0){

            for (int i=1;i<nnodes;++i){
                NodeTimeGraph cur_neigh = timenode;
                cur_neigh[0] = i;
                cur_neigh[1] = distances[0][i]+1;
                cur_neigh[i+1] = cur_neigh[1];
                neighbours.push_back(cur_neigh);
            }

        }else{
            assert(timenode[node+1]!=0 && "Visited valve must be open");

            for (int i=1;i<nnodes;++i){
                if (timenode[i+1]!=0) continue;     //skip if valve is already open

                NodeTimeGraph cur_neigh = timenode;
                cur_neigh[0] = i;
                cur_neigh[1] = minute + distances[node][i] + 1;
                cur_neigh[i+1] = cur_neigh[1];

                if (cur_neigh[1]>30) continue;  //skip if arrival time is over 30

                neighbours.push_back(cur_neigh);
            }

        }
    }else{

        int minute_e = timenode[3];

        if (node==0){

            for (int i=1;i<nnodes;++i){
                NodeTimeGraph cur_neigh = timenode;
                cur_neigh[0] = i;
                cur_neigh[1] = distances[0][i]+1;
                cur_neigh[i+3] = cur_neigh[1];
                neighbours.push_back(cur_neigh);
            }

        }else{
            
            int index{};
            if (minute>minute_e) index = 2;

            for (int i=1;i<nnodes;++i){
                if (timenode[i+3]!=0) continue;     //skip if valve is already open

                NodeTimeGraph cur_neigh = timenode;
                cur_neigh[index] = i;
                cur_neigh[index+1] = timenode[index+1] + distances[timenode[index]][i] + 1;
                cur_neigh[i+3] = cur_neigh[index+1];

                if (cur_neigh[1]>26 || cur_neigh[3]>26) continue;  //skip if arrival time is over 26
                
                neighbours.push_back(cur_neigh);
            }

        }

    }

    return neighbours;
}
void TimeGraph::checkNode(const NodeTimeGraph& timenode){
    if (part_one){
        int cur_score{};
        for (int i=1;i<nnodes;++i){
            if (timenode[i+1]!=0){
                cur_score += (30-timenode[i+1])*flows[i];
            }
        }
        best_score = std::max(best_score,cur_score);
    }else{
        int cur_score{};
        for (int i=1;i<nnodes;++i){
            if (timenode[i+3]!=0){
                cur_score += (26-timenode[i+3])*flows[i];
            }
        }
        if (cur_score>best_score){
            best_score = cur_score;
            best_node = timenode;
            std::cout<<best_score<<'\n';
        }
    }
}
void TimeGraph::dfs(const NodeTimeGraph& root){

    std::stack<NodeTimeGraph> S{};
    std::set<NodeTimeGraph> visited{};
    S.push(root);

    while (!S.empty()){

        NodeTimeGraph node{S.top()};
        S.pop();

        checkNode(node);

        auto resultInsert = visited.insert(node);
        if (resultInsert.second){

            const std::vector<NodeTimeGraph> neighbours = getNeighbours(node);
            for (auto it=neighbours.begin();it!=neighbours.end();++it){
                S.push(*it);
            }

        }
    }

}


void read_file(std::ifstream& file,std::vector<InputNode>& input){
    std::string line{};
    while (getline(file,line)){
        InputNode node{};

        //READ ID
        node.id.push_back(line[6]);
        node.id.push_back(line[7]);

        //READ FLOW
        size_t pos = line.find('=');
        int index = 1;
        char c = line[pos+index];
        while (c!=';'){
            node.flow = node.flow*10 + (c-'0');

            ++index;
            c = line[pos+index];
        }

        //READ NEIGHBOURS
        pos = line.find("valves");
        if (pos!=std::string::npos){

            std::string neigh{};
            neigh.push_back(line[pos+7]);
            neigh.push_back(line[pos+8]);
            node.neighbours.push_back(neigh);

            pos = line.find(',',pos);
            while (pos!=std::string::npos){
                neigh.clear();
                neigh.push_back(line[pos+2]);
                neigh.push_back(line[pos+3]);
                node.neighbours.push_back(neigh);
                pos = line.find(',',pos+1);
            }

        }else{

            pos = line.find("valve");
            std::string neigh{};
            neigh.push_back(line[pos+6]);
            neigh.push_back(line[pos+7]);
            node.neighbours.push_back(neigh);

        }

        input.push_back(node);
    }
}


int main(){

    bool part_one{false};

    std::ifstream file("data16.txt");
    std::vector<InputNode> input{};

    read_file(file,input);

    //PREPARE INPUT GRAPH WITH ALL NODES
    Graph inputGraph{};
    std::vector<std::string> node_names{};
    std::vector<int> nzflows{};

    for (InputNode node: input){
        node_names.push_back(node.id);
        inputGraph.node_flows.push_back(node.flow);

        if (node.flow!=0 || node.id=="AA"){
            nzflows.push_back(node.flow);
        }
    }
    //CREATE NODES
    inputGraph.addNodes(node_names);
    //CREATE EDGES
    for (InputNode node: input){
        for (std::string s: node.neighbours){
            inputGraph.addEdge(node.id,s);
        }
    }



    //PREPARE SIMULATION GRAPH, ONLY WITH NODE AA AND POSITIVE FLOWS
    TimeGraph t{};
    t.nnodes = nzflows.size();
    t.flows = nzflows;
    t.part_one = part_one;

    //PRE-COMPUTE DISTANCES USING BFS
    t.distances = std::vector<std::vector<int>> (t.nnodes,std::vector<int> (t.nnodes,0));
    int row{};
    for (int i=0;i<inputGraph.node_id.size();++i){
        int col{row+1};

        if (inputGraph.node_flows[i]!=0 || inputGraph.node_id.find("AA")->second==i){
            std::map<int,int> distances_bfs = bfs(i,inputGraph);

            for (int j=i+1;j<inputGraph.node_id.size();++j){

                if (inputGraph.node_flows[j]!=0 || inputGraph.node_id.find("AA")->second==j){
                    t.distances[row][col] = distances_bfs[j];
                    t.distances[col][row] = t.distances[row][col];
                    ++col;
                }
            }
            ++row;
        }
    }

    //RUN DFS WITH NODE-CHECK
    if (part_one){

        NodeTimeGraph start(nzflows.size()+1,0);
        t.dfs(start);
        std::cout<<t.best_score<<'\n';

    }else{

        t.best_score={};
        NodeTimeGraph start(nzflows.size()+3,0);
        t.dfs(start);
        std::cout<<t.best_score<<'\n';
        print(t.best_node);
        
    }

    return 0;
}



