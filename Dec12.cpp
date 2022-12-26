#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include "../graph_algorithms.h"

using Nodes = std::pair<int,int>;

class Graph{
    public:

    std::vector<std::vector<int>> map{};
    Nodes start{};
    Nodes target{};
    std::vector<std::pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

    bool part_one{};

    void readFile(std::ifstream&);
    void print() const;

    bool isGoal(const Nodes&) const;
    std::vector<Nodes> getNeighbours(const Nodes&) const;

};

void Graph::readFile(std::ifstream& file){
    int row{};
    int col{};
    std::string str{};
    while(getline(file,str)){
        std::stringstream ss{str};
        char c{};
        std::vector<int> line{};
        col=0;
        while (ss.get(c)){
            if (c=='S'){
                line.push_back(0);
                start = std::make_pair(row,col);
            }else if (c=='E'){
                line.push_back('z'-'a'+1);
                target = std::make_pair(row,col);
            }else{
                line.push_back(c-'a');
            }
            ++col;
        }
        map.push_back(line);
        ++row;
    }
}

void Graph::print() const{
    for (auto it=map.begin();it<map.end();++it){
        for (auto itt=(*it).begin();itt<(*it).end();++itt){
            std::cout<<*itt<<' ';
        }
        std::cout<<'\n';
    }
}

bool Graph::isGoal(const Nodes& node) const{
    if (part_one){
        return (node == target);
    }else{
        return (map[node.first][node.second]==0);
    }
}

std::vector<Nodes> Graph::getNeighbours(const Nodes& node) const{
    std::vector<Nodes> neighbours{};

    for (int i=0;i<4;++i){
        Nodes cur{node};
        cur.first += directions[i].first;
        cur.second += directions[i].second;

        if (cur.first<0 || cur.first>map.size()-1 || cur.second<0 || cur.second>map[0].size()-1) continue;

        if (part_one){

            if (map[cur.first][cur.second]<=map[node.first][node.second]+1){
                neighbours.push_back(cur);
            }

        }else{

            if (map[cur.first][cur.second]>=map[node.first][node.second]-1){
                neighbours.push_back(cur);
            }

        }

    }

    return neighbours;
}


int main(){

    std::ifstream file("data12.txt");

    Graph graph{};
    graph.readFile(file);

    //graph.print();

    graph.part_one = true;
    std::pair<Nodes,int> result = bfs(graph.start,graph);
    std::cout<<"Part one: "<<result.second<<'\n';
    
    graph.part_one = false;
    result = bfs(graph.target,graph);
    std::cout<<"Part two: "<<result.second<<'\n';

    return 0;
}



