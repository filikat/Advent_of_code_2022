#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>


void updateHead(std::pair<int,int>& headPos,char dir){
    switch (dir){
        case 'R': ++headPos.first;break;
        case 'L': --headPos.first;break;
        case 'U': ++headPos.second;break;
        case 'D': --headPos.second;break;
        default: 
            std::cout<<"Wrong direction";
            break;
        }
}

void updateTail(std::pair<int,int>& tailPos,std::pair<int,int>& headPos){

    int xdif{headPos.first-tailPos.first};
    int ydif{headPos.second-tailPos.second};
    if (std::abs(xdif)>1 || std::abs(ydif)>1){
        tailPos.first += xdif/std::abs(xdif);
        tailPos.second += ydif/std::abs(ydif);
    }

}

int main(){

    bool part_one{false};

    //READ DATA
    std::ifstream file("data9.txt");
    char dir{};
    int amount{};

    std::pair<int,int> headPos(0,0);
    std::vector<std::pair<int,int>> tailPos;
    if (part_one){
        tailPos.push_back({0,0});
    }else{
        for (int i=0;i<9;++i){
            tailPos.push_back({0,0});
        }
    }

    std::set<std::pair<int,int>> Positions{};

    while(file.peek()!=EOF){
        file>>dir>>amount;

        for (int i=0;i<amount;++i){
            updateHead(headPos,dir);
            updateTail(tailPos[0],headPos);

            if(!part_one){
                for (int i=1;i<9;++i){
                    updateTail(tailPos[i],tailPos[i-1]);
                }
            }

            Positions.insert(tailPos.back());
            
        }
    }
    
    std::cout<<Positions.size()<<'\n';

    return 0;
}



