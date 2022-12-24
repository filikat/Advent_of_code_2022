#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>


// DIRECTIONS
// 1: >
// 2: v
// 3: <
// 4: ^


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


class Blizzards{
    public:
    std::vector<std::vector<std::vector<int>>> positions{};
    std::vector<std::set<std::vector<int>>> occupied{};
    int width{};
    int height{};
    int period{};
    int cur_best{INT_MAX/2};
    std::map<std::string,int> scenario_done{};

    std::vector<int> goal{};
    
    std::vector<std::vector<int>> directions{{0,1},{1,0},{0,-1},{-1,0}};
    std::map<std::string,int> visited{};


    void print(int) const;
    void read_file(std::ifstream&);
    void generate_scenarios();
    std::pair<int,bool> move(int,int,int);
    
};

void Blizzards::print(int i) const{
    for (auto it=positions[i].begin();it!=positions[i].end();++it){
        char c{};
        switch (it->at(2)){
            case 1: c='>';break;
            case 2: c='v';break;
            case 3: c='<';break;
            case 4: c='^';break;
        }
        std::cout<<'('<<it->at(0)<<','<<it->at(1)<<") -> "<<c<<'\n';
    }
    std::cout<<'\n';
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

std::string hash(const std::vector<std::vector<int>>& pos){
    std::string hash_str{};
    for (auto it=pos.begin();it!=pos.end();++it){
        hash_str.append(std::to_string(it->at(0)));
        hash_str.append("_");
        hash_str.append(std::to_string(it->at(1)));
        hash_str.append("_");
    }
    return hash_str;
}

void Blizzards::generate_scenarios(){

    std::vector<std::vector<int>> cur_pos = positions[0];
    std::set<std::string> visited{};
    visited.insert(hash(positions[0]));

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
        std::string hash_str{hash(cur_pos)};

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

std::pair<int,bool> Blizzards::move(int exp_x,int exp_y,int minute){

    if (minute>=cur_best || minute>=2000) return {INT_MAX/2,false};

    int index = (minute+1)%period;
    std::string scenario_str{};
    scenario_str.append(std::to_string(index));
    scenario_str.append("_");
    scenario_str.append(std::to_string(exp_x));
    scenario_str.append("_");
    scenario_str.append(std::to_string(exp_y));
    auto found = scenario_done.find(scenario_str);

    if (found!=scenario_done.end()){
        return {(found->second)+minute,true};

    }else{

        std::vector<std::pair<int,bool>> times{};
        bool moved{false};

        if (exp_x==goal[0] && exp_y==goal[1]){
            cur_best = std::min(cur_best,minute);
            return {minute,true};
        }

        //TRY THE FOUR DIRECTIONS
        for (int i=0;i<4;++i){
            std::vector<int> new_pos{exp_x+directions[i][0],exp_y+directions[i][1]};

            if (new_pos[0]<0 || new_pos[0]>=height || new_pos[1]<0 || new_pos[1]>=width) continue;

            if (occupied[index].find(new_pos)==occupied[index].end()){
                std::pair<int,bool> cur_res = move(new_pos[0],new_pos[1],minute+1);
                times.push_back(cur_res);
                moved = true;
            }

        }

        //TRY STAYING STILL
        if (occupied[index].find({exp_x,exp_y})==occupied[index].end()){
            std::pair<int,bool> cur_res = move(exp_x,exp_y,minute+1);
                times.push_back(cur_res);
                moved = true;
        }

        //IF NONE IS POSSIBLE, IT TAKES INFINITE MINUTES
        if (!moved){
            return {INT_MAX/4,true};
        }

        //OTHERWISE RETURN THE MINIMUM TIME
        std::pair<int,bool> result = *std::min_element(times.begin(),times.end(),[](auto& a,auto& b){return a.first<b.first;});
        if (result.second){
            scenario_done.insert({scenario_str,result.first-minute});
        }
        return result;
    }
    

}

int main(){

    std::ifstream file("data24.txt");

    Blizzards blizzards{};

    blizzards.read_file(file);

    blizzards.generate_scenarios();

    std::vector<int> expedition{};

    expedition = {-1,0};
    blizzards.goal = {blizzards.height-1,blizzards.width-1};
    int trip1{blizzards.move(expedition[0],expedition[1],0).first+1};
    std::cout<<"First trip "<<trip1<<"\n\n";

    expedition = {blizzards.goal[0]+1,blizzards.goal[1]};
    blizzards.goal = {0,0};
    blizzards.scenario_done.clear();
    blizzards.cur_best = INT_MAX/2;
    int trip2{blizzards.move(expedition[0],expedition[1],trip1).first+1};
    std::cout<<"Second trip "<<trip2<<"\n\n";

    expedition = {-1,0};
    blizzards.goal = {blizzards.height-1,blizzards.width-1};
    blizzards.scenario_done.clear();
    blizzards.cur_best = INT_MAX/2;
    int trip3{blizzards.move(expedition[0],expedition[1],trip2).first+1};
    std::cout<<"Third trip "<<trip3<<'\n';

    return 0;
}



