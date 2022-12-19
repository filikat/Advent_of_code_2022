#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <queue>


using vec      = std::vector<int>;
using Pointset = std::set<vec>;

void print(const Pointset& wallPoints){
    for (auto it=wallPoints.begin();it!=wallPoints.end();++it){
        std::cout<<it->at(0)<<' '<<it->at(1)<<' '<<it->at(2)<<'\n';
    }
}

vec sum_vec(const vec& v1,const vec& v2){
    vec v3{v1};
    for (int i=0;i<v1.size();++i){
        v3[i] += v2[i];
    }
    return v3;
}


int main(){

    std::ifstream file("data18.txt");
    std::string str{};
    Pointset wallPoints{};

    int coord_x{};
    int coord_y{};
    int coord_z{};
    char dummy{};

    int max_coord{};

    while(getline(file,str)){
        std::stringstream ss{str};
        ss>>coord_x>>dummy;
        ss>>coord_y>>dummy;
        ss>>coord_z;

        max_coord = std::max(max_coord,coord_x);
        max_coord = std::max(max_coord,coord_y);
        max_coord = std::max(max_coord,coord_z);

        wallPoints.insert({coord_x+1,coord_y+1,coord_z+1});
    }

    //print(wallPoints);

    std::queue<vec> toVisit{};
    toVisit.push({0,0,0});
    std::vector<vec> directions = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
    std::set<vec> visited{};

    int free_faces{};
    while (!toVisit.empty()){
        vec point = toVisit.front();
        toVisit.pop();
        if (visited.find(point)!=visited.end()) continue;

        visited.insert(point);

        for (vec dir: directions){
            vec neighbour{sum_vec(point,dir)};
            if (neighbour[0]<0 || neighbour[0]>max_coord+2 ||
                neighbour[1]<0 || neighbour[1]>max_coord+2 ||
                neighbour[2]<0 || neighbour[2]>max_coord+2
                ) continue;

            if (wallPoints.find(neighbour)!=wallPoints.end()) ++free_faces;
            else toVisit.push(neighbour);
        }
    }

    std::cout<<free_faces<<'\n';

    return 0;
}



