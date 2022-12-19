#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>


void print(std::vector<std::stack<char>> stacks){
    for (int i=0;i<stacks.size();++i){
        while (!stacks[i].empty()){
            std::cout<<stacks[i].top()<<' ';
            stacks[i].pop();
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

int main(){

    bool part_one{false};

    std::ifstream file("data5.txt");
    std::vector<std::stack<char>> stacks(9,std::stack<char>{});

    stacks[0] = std::stack<char> ({'S','Z','P','D','L','B','F','C'});
    stacks[1] = std::stack<char> ({'N','V','G','P','H','W','B'});
    stacks[2] = std::stack<char> ({'F','W','B','J','G'});
    stacks[3] = std::stack<char> ({'G','J','N','F','L','W','C','S'});
    stacks[4] = std::stack<char> ({'W','J','L','T','P','M','S','H'});
    stacks[5] = std::stack<char> ({'B','C','W','G','F','S'});
    stacks[6] = std::stack<char> ({'H','T','P','M','Q','B','W'});
    stacks[7] = std::stack<char> ({'F','S','W','T'});
    stacks[8] = std::stack<char> ({'N','C','R'});

    std::string str{};
    std::string dummy{};
    int amount{};
    int from{};
    int to{};
    while (getline(file,str)){
        std::stringstream ss{str};
        ss>>dummy>>amount;
        ss>>dummy>>from;
        ss>>dummy>>to;

        if (part_one){
            //PART ONE
            for (int i=0;i<amount;++i){
                int element{stacks[from-1].top()};
                stacks[from-1].pop();
                stacks[to-1].push(element);
            }
        }else{
            //PART TWO
            std::vector<char> temp{};
            for (int i=0;i<amount;++i){
                int element{stacks[from-1].top()};
                stacks[from-1].pop();
                temp.push_back(element);
            }

            for (int i=temp.size()-1;i>=0;--i){
                stacks[to-1].push(temp[i]);
            }
        }

    }
    print(stacks);


    return 0;
}



