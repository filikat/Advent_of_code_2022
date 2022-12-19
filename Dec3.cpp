#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int searchString(const std::string::iterator& start,const std::string::iterator& middle,const std::string::iterator& finish){
    for (auto it=start;it<middle;++it){
        auto found = std::find(middle,finish,*it);
        if (found!=finish){
            if (*found<'a'){
                return (*found-'A')+27;
            }else{
                return (*found-'a')+1;
            }
        }
    }
    return 0;
}

int search3String(
    const std::string::iterator& start1,const std::string::iterator& finish1,
    const std::string::iterator& start2,const std::string::iterator& finish2,
    const std::string::iterator& start3,const std::string::iterator& finish3
    ){
    
    for (auto it=start1;it<finish1;++it){
        auto found = std::find(start2,finish2,*it);
        if (found!=finish2){
            auto found2 = std::find(start3,finish3,*it);
            if (found2!=finish3){
                if (*it<'a'){
                    return (*it-'A')+27;
                }else{
                    return (*it-'a')+1;
                }
            }
        }
    }
    return 0;
}

int main(){

    bool part_one{false};

    std::ifstream file("data3.txt");
    std::string str{};
    std::string str1{};
    std::string str2{};
    std::string str3{};
    int score{};

    if (part_one){

        //PART ONE
        while (getline(file,str)){
            int n = str.size()/2;
            score += searchString(str.begin(),str.begin()+n,str.end());
        }
    
    }else{

        //PART TWO
        while (file.peek()!=EOF){
            file>>str1;
            file>>str2;
            file>>str3;

            score += search3String(str1.begin(),str1.end(),str2.begin(),str2.end(),str3.begin(),str3.end());

        }

    }
    std::cout<<score<<'\n';


    return 0;
}



