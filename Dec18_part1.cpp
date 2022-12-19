#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>



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

    while(getline(file,str)){
        std::stringstream ss{str};
        ss>>coord_x>>dummy;
        ss>>coord_y>>dummy;
        ss>>coord_z;

        wallPoints.insert({coord_x,coord_y,coord_z});
    }

    std::vector<vec> directions = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
    int free_faces{};

    for (vec cur: wallPoints){
        for (vec dir: directions){
            vec neighbour{sum_vec(cur,dir)};
            if (wallPoints.find(neighbour)==wallPoints.end()) ++free_faces;
        }
    }
    std::cout<<free_faces<<'\n';

    return 0;
}



