#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>
#include <chrono>
#include "bfs.h"

using Nodes = std::vector<int>;
using Neighbours = std::vector<Nodes>;

class Blizzards{
    public:
    std::vector<std::vector<std::vector<int>>> positions{};
    std::vector<std::set<std::vector<int>>> occupied{};
    int width{};
    int height{};
    int period{};
    std::vector<int> goal{};
    
    std::vector<std::vector<int>> directions{{0,1},{1,0},{0,-1},{-1,0}};
    std::map<std::string,int> visited{};


    void print(int) const;
    void read_file(std::ifstream&);
    void generate_scenarios();

    Neighbours getNeighbours(const Nodes&) const;
    bool isGoal(const Nodes&) const;
    
};

std::string hash_scenario(const std::vector<std::vector<int>>& pos){
    std::string hash_str{};
    for (auto it=pos.begin();it!=pos.end();++it){
        hash_str.append(std::to_string(it->at(0)));
        hash_str.append("_");
        hash_str.append(std::to_string(it->at(1)));
        hash_str.append("_");
    }
    return hash_str;
}
std::vector<int> match_direction(int d){
    switch (d){
        case 1:
            return {0,1};
        case 2:
            return {1,0};
        case 3:
            return {0,-1};
        case 4:
            return {-1,0};
        default:
            std::cerr<<"Wrong direction\n";
            return {0,0};
    }
}

void Blizzards::read_file(std::ifstream& file){
    std::string str{};
    char c{};
    int row{-1};
    height = 0;
    
    std::vector<std::vector<int>> initial_pos{};
    while (getline(file,str)){
        std::stringstream ss{str};
        int col{-1};
        width = 0;

        while (ss.get(c)){
            if (c=='>') initial_pos.push_back({row,col,1});
            else if(c=='v') initial_pos.push_back({row,col,2});
            else if(c=='<') initial_pos.push_back({row,col,3});
            else if(c=='^') initial_pos.push_back({row,col,4});
            ++col;
            ++width;
        }
        ++row;
        ++height;
    }
    width -= 2;
    height -= 2;
    positions.push_back(initial_pos);

    std::set<std::vector<int>> cur_occ{};
    for (size_t i=0;i<positions[0].size();++i){
        cur_occ.insert({positions[0][i][0],positions[0][i][1]});
    }
    occupied.push_back(cur_occ);

}
void Blizzards::generate_scenarios(){

    std::vector<std::vector<int>> cur_pos = positions[0];
    std::set<std::string> visited{};
    visited.insert(hash_scenario(positions[0]));

    int scenario{};
    while(true){

        std::set<std::vector<int>> cur_occ{};
        for (size_t i=0;i<cur_pos.size();++i){
            std::vector<int> dir{match_direction(cur_pos[i][2])};

            cur_pos[i][0] = ((cur_pos[i][0] + dir[0]) % height + height) % height;
            cur_pos[i][1] = ((cur_pos[i][1] + dir[1]) % width + width) % width;

            cur_occ.insert({cur_pos[i][0],cur_pos[i][1]});

        }

        //HASH THE POSITION
        std::string hash_str{hash_scenario(cur_pos)};

        if (visited.find(hash_str)==visited.end()){
            positions.push_back(cur_pos);
            occupied.push_back(cur_occ);
            visited.insert(hash_str);
            ++scenario;
        }else{
            period = scenario+1;
            break;
        }

    }

}

Neighbours Blizzards::getNeighbours(const Nodes& node) const{
    int index = (node[0]+1)%period;
    Neighbours neighbours{};

    for (int i=0;i<4;++i){
        std::vector<int> new_pos{node[1]+directions[i][0],node[2]+directions[i][1]};

        if (new_pos[0]<0 || new_pos[0]>=height || new_pos[1]<0 || new_pos[1]>=width) continue;

        if (occupied[index].find(new_pos)==occupied[index].end()){
            neighbours.push_back({node[0]+1,new_pos[0],new_pos[1]});
        }

    }

    if (occupied[index].find({node[1],node[2]})==occupied[index].end()){
        neighbours.push_back({node[0]+1,node[1],node[2]});
    }

    return neighbours;
}
bool Blizzards::isGoal(const Nodes& node) const{
    return (node[1]==goal[0] && node[2]==goal[1]);
}


int main(){

    std::ifstream file("data24.txt");

    Blizzards blizzards{};

    blizzards.read_file(file);

    blizzards.generate_scenarios();
    std::cout<<"Scenarios done\n";

    Nodes root{};

    auto start_t = std::chrono::high_resolution_clock::now();


    blizzards.goal = {blizzards.height-1,blizzards.width-1};
    root = {0,-1,0};
    int trip1 = bfs(root,blizzards)[0]+1;
    std::cout<<"Trip 1 "<<trip1<<'\n';
    
    root = {trip1,blizzards.goal[0]+1,blizzards.goal[1]};
    blizzards.goal = {0,0};
    int trip2 = bfs(root,blizzards)[0]+1;
    std::cout<<"Trip 2 "<<trip2<<'\n';

    root = {trip2,-1,0};
    blizzards.goal = {blizzards.height-1,blizzards.width-1};
    int trip3 = bfs(root,blizzards)[0]+1;
    std::cout<<"Trip 3 "<<trip3<<'\n';


    auto end_t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_t-start_t;
    std::cout<<"Duration "<<duration.count()<<'\n';

    return 0;
}



