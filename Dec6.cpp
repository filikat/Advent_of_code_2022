#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool check_str(const char c1,const char c2,const char c3,const char c4){
//RETURNS TRUE IF ALL FOUR CHARACHTERS ARE DIFFERENT
    if (
        c1==c2 ||
        c1==c3 ||
        c1==c4 ||
        c2==c3 ||
        c2==c4 ||
        c3==c4
    ) return false;
    return true;
}

bool check_str_14(std::string str){
    std::sort(str.begin(),str.end());
    auto it = std::unique(str.begin(),str.end());
    if (it-str.begin()<14) return false;
    return true;
}

int main(){

    std::ifstream file("data6.txt");
    std::string str{};
    file>>str;

    //FIRST PART
    for (int i=0;i<str.size()-3;++i){
        if (check_str(str[i],str[i+1],str[i+2],str[i+3])){
            std::cout<<i+4<<'\n';
            break;
        }
    }   

    //SECOND PART
    for (int i=0;i<=str.size()-14;++i){
        if (check_str_14(str.substr(i,14))){
            std::cout<<i+14<<'\n';
            break;
        }
    }

    
    return 0;
}



