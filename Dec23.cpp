#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <set>

using Positions = std::set<std::pair<int,int>>;
using MapPos = std::map<std::pair<int,int>,std::pair<int,int>>;


class ElfMap{

    Positions positions{};
    MapPos proposed{};  //associates a proposed move to the initial position
    MapPos moves{};     //associates the initial position to the final move

    std::set<std::pair<int,int>> allDirections{{1,0},{0,1},{-1,0},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    std::vector<std::pair<int,int>> north{{-1,0},{-1,-1},{-1,1}};
    std::vector<std::pair<int,int>> south{{1,0},{1,-1},{1,1}};
    std::vector<std::pair<int,int>> west{{0,-1},{-1,-1},{1,-1}};
    std::vector<std::pair<int,int>> east{{0,1},{-1,1},{1,1}};
    std::vector<std::vector<std::pair<int,int>>> directions{};

    bool checkAllDirections(const std::pair<int,int>& pos) const;
    void addProposed(const std::pair<int,int>&,const std::pair<int,int>&);

    public:

    bool anyElfMoves{false};
    ElfMap(){
        directions.push_back(north);
        directions.push_back(south);
        directions.push_back(west);
        directions.push_back(east);
    }
    void print(int,int) const;
    void readMap(std::ifstream&);
    void playRound();
    int getRectangle() const;


};

void ElfMap::print(int mincoord,int maxcoord) const{
    for (int x=mincoord;x<=maxcoord;++x){
        for (int y=mincoord;y<=maxcoord;++y){
            if (positions.find({x,y})!=positions.end()){
                std::cout<<"# ";
            }else std::cout<<". ";
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void ElfMap::readMap(std::ifstream& file){
    std::string str{};
    char c{};

    int row{};
    while (getline(file,str)){
        std::stringstream ss{str};
        int col{};
        while (ss.get(c)){
            if (c=='#') positions.insert({row,col});
            ++col;
        }
        ++row;
    }
}

bool ElfMap::checkAllDirections(const std::pair<int,int>& pos) const{
    //RETURNS TRUE IF ALL DIRECTIONS ARE CLEAR
    for (auto it=allDirections.begin();it!=allDirections.end();++it){
        std::pair<int,int> posToCheck{pos};
        posToCheck.first += it->first;
        posToCheck.second += it->second;
        if (positions.find(posToCheck)!=positions.end()){
            return false;
        }
    }
    return true;
}

void ElfMap::addProposed(const std::pair<int,int>& prop,const std::pair<int,int>& orig){

    auto found = proposed.find(prop);

    if (found==proposed.end()){
        proposed.insert({prop,orig});
        moves.insert({orig,prop});
    }else{
        moves.insert({orig,orig});
        moves.find(found->second)->second = found->second;
    }

}

void ElfMap::playRound(){

    proposed.clear();
    moves.clear();
    anyElfMoves = false;

    for (auto it=positions.begin();it!=positions.end();++it){

        auto cur_pos = *it;
        if (checkAllDirections(cur_pos)){
            moves.insert({cur_pos,cur_pos});
        }else{

            for (int i=0;i<4;++i){
                auto dirset = directions[i];
                bool cont{true};

                for (auto dir: dirset){
                    std::pair<int,int> posToCheck{cur_pos};
                    posToCheck.first += dir.first;
                    posToCheck.second += dir.second;

                    if (positions.find(posToCheck)!=positions.end()){
                        cont = false;
                    }

                    if (!cont) break;
                }

                if (cont){
                    //if reach here, all 3 directions are clear
                    std::pair<int,int> propMove{cur_pos};
                    propMove.first += dirset[0].first;
                    propMove.second += dirset[0].second;
                    addProposed(propMove,cur_pos);
                    break;
                }
            }

        }

        //add positions that cannot move in any of the 4 directions
        if (moves.find(cur_pos)==moves.end()){
            moves.insert({cur_pos,cur_pos});
        }

    }

    auto top_dir = directions.front();
    directions.erase(directions.begin());
    directions.push_back(top_dir);        

    positions.clear();

    for (auto move: moves){
        positions.insert(move.second);
        if (move.first!=move.second) anyElfMoves = true;
    }

}

int ElfMap::getRectangle() const{
    int minx{INT_MAX};
    int maxx{INT_MIN};
    int miny{INT_MAX};
    int maxy{INT_MIN};

    for (auto pos: positions){
        minx = std::min(minx,pos.first);
        maxx = std::max(maxx,pos.first);
        miny = std::min(miny,pos.second);
        maxy = std::max(maxy,pos.second);
    }

    return (maxx-minx+1)*(maxy-miny+1)-positions.size();
}

int main(){

    bool part_one{false};

    std::ifstream file("data23.txt");

    ElfMap elfmap{};
    elfmap.readMap(file);

    if (part_one){

        for (int round=1;round<=10;++round){
            elfmap.playRound();
        }
        std::cout<<elfmap.getRectangle()<<'\n';

    }else{

        int round{1};
        while (true){
            elfmap.playRound();
            if (!elfmap.anyElfMoves){
                std::cout<<"Stop at round "<<round<<'\n';
                break;
            }
            ++round;
        }

    }

    //elfmap.print(0,50);

    return 0;
}



