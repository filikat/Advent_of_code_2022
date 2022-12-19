#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



int createstack(std::ifstream& file,std::vector<int>& dir_size){
    std::string str{};
    int file_size{};
    long long stack_size{};
    while (getline(file,str)){
        std::stringstream ss{str};
        char dummy{};
        std::string name{};

        if (str[0]=='$'){

            if (str[2]=='c'){

                ss>>dummy>>dummy>>dummy;
                ss>>name;
                if (name[0]!='.'){
                    stack_size += createstack(file,dir_size);
                }
                else{
                    dir_size.push_back(stack_size);
                    return stack_size;
                    }

            }else continue;

        }else{
            if (str[0]=='d') continue;
            else{
                ss>>file_size;
                stack_size += file_size;
            }
        }
    }

    dir_size.push_back(stack_size);
    return stack_size;
}



int main(){

    std::ifstream file("data7.txt");
    std::string str{};
    std::vector<int> dir_size{};

    while (getline(file,str)){

        std::stringstream ss{str};
        char dummy{};
        std::string name{};
        if (str[0]=='$'){
            if (str[2]=='c'){
                ss>>dummy>>dummy>>dummy;
                ss>>name;
                createstack(file,dir_size);
            }
        }
    }
    
    int total{};
    for (auto it=dir_size.begin();it<dir_size.end();++it){
        if (*it>100000) continue;
        else total += *it;
    }
    std::cout<<total<<'\n';


    //PART TWO
    constexpr int totalspace{70000000};
    constexpr int minspace{30000000};
    const int usedspace{*std::max_element(dir_size.begin(),dir_size.end())};
    const int unusedspace{totalspace-usedspace};
    const int requiredspace{minspace-unusedspace};

    int currentchoice{INT_MAX};
    for (auto it=dir_size.begin();it<dir_size.end();++it){
        if (*it>=requiredspace){
            currentchoice = std::min(currentchoice,*it);
        }
    }
    std::cout<<"Eliminate "<<currentchoice<<'\n';

    
    return 0;
}



