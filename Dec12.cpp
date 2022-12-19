#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>


void print(const std::vector<std::vector<int>>& map){
    for (auto it=map.begin();it<map.end();++it){
        for (auto itt=(*it).begin();itt<(*it).end();++itt){
            std::cout<<*itt<<' ';
        }
        std::cout<<'\n';
    }
}

int main(){

    std::ifstream file("data12.txt");
    std::string str{};
    std::vector<std::vector<int>> map{};
    std::queue<std::pair<int,int>> start{};
    std::pair<int,int> finish{};
    int row{};
    int col{};

    //READ DATA
    while(getline(file,str)){
        std::stringstream ss{str};
        char c{};
        std::vector<int> line{};
        col=0;
        while (ss.get(c)){
            if (c=='S' || c=='a'){
                line.push_back(0);
                start.push(std::pair<int,int> (row,col));
            }else if (c=='E'){
                line.push_back('z'-'a');
                finish.first = row;
                finish.second = col;
            }else{
                line.push_back(c-'a');
            }
            ++col;
        }
        map.push_back(line);
        ++row;
    }

    //print(map);

    int best_score{INT_MAX};

    while (!start.empty()){
        std::vector<std::vector<int>> cost(map.size(),std::vector<int> (map[0].size(),0));
        cost[start.front().first][start.front().second] = 1;
        
        std::queue<std::pair<int,int>> neighbours{};
        neighbours.push(start.front());

        while (!neighbours.empty()){
            
            std::pair<int,int> cur{neighbours.front()};
            if (cur.first>0){
                if (cost[cur.first-1][cur.second]==0 && map[cur.first-1][cur.second]<=map[cur.first][cur.second]+1){
                    neighbours.push(std::pair<int,int> (cur.first-1,cur.second));
                    cost[cur.first-1][cur.second] = cost[cur.first][cur.second]+1;
                }
            }
            if (cur.first<map.size()-1){
                if (cost[cur.first+1][cur.second]==0 && map[cur.first+1][cur.second]<=map[cur.first][cur.second]+1){
                    neighbours.push(std::pair<int,int> (cur.first+1,cur.second));
                    cost[cur.first+1][cur.second] = cost[cur.first][cur.second]+1;
                }
            }
            if (cur.second>0){
                if (cost[cur.first][cur.second-1]==0 && map[cur.first][cur.second-1]<=map[cur.first][cur.second]+1){
                    neighbours.push(std::pair<int,int> (cur.first,cur.second-1));
                    cost[cur.first][cur.second-1] = cost[cur.first][cur.second]+1;
                }
            }
            if (cur.second<map[0].size()){
                if (cost[cur.first][cur.second+1]==0 && map[cur.first][cur.second+1]<=map[cur.first][cur.second]+1){
                    neighbours.push(std::pair<int,int> (cur.first,cur.second+1));
                    cost[cur.first][cur.second+1] = cost[cur.first][cur.second]+1;
                }
            }
            neighbours.pop();
            }

        if (cost[finish.first][finish.second]!=0){
            best_score = std::min(best_score,cost[finish.first][finish.second]-1);
        }
        start.pop();
    }

    std::cout<<best_score<<'\n';


    
    return 0;
}



