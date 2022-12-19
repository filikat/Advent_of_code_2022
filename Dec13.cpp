#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>



bool processLines(std::string::iterator& it1,std::string::iterator& it2){
    //TRUE IF RIGHT ORDER
    //FALSE IF WRONG ORDER

    //IF COMMA, SKIP CHARACHTER
    if (*it1==','){
        ++it1;
        return processLines(it1,it2);
    }
    if (*it2==','){
        ++it2;
        return processLines(it1,it2);
    }


    
    //CHECK IF NUMBER
    int num1 = *it1-'0';
    int num2 = *it2-'0';
    bool isnum1{false};
    bool isnum2{false};

    if (num1>=0 && num1<=9){
        isnum1 = true;
        if (num1==1 && (*(it1+1))=='0'){
            num1 = 10;
            ++it1;
        }else if (num1==0 && (*(it1-1)=='1')){
            num1 = 10;
        }
    }
    if (num2>=0 && num2<=9){
        isnum2 = true;
        if (num2==1 && (*(it2+1))=='0'){
            num2 = 10;
            ++it2;
        }else if (num2==0 && (*(it2-1)=='1')){
            num2 = 10;
        }
    }

    //CHECK IF [
    bool isopen1{false};
    bool isopen2{false};
    if (*it1=='[') isopen1 = true;
    if (*it2=='[') isopen2 = true;

    //CHECK IF ]
    bool isclose1{false};
    bool isclose2{false};
    if (*it1==']') isclose1 = true;
    if (*it2==']') isclose2 = true;


    //BOTH NUMBERS
    if (isnum1 && isnum2){
        if (num1<num2) return true;
        else if (num1>num2) return false;
        else {
            ++it1;
            ++it2;
            return processLines(it1,it2);
        }
    } 

    if(isnum1 && isopen2){
        *(it1+1)=']';
        return processLines(it1,++it2);
    }
    if (isopen1 && isnum2){
        *(it2+1)=']';
        return processLines(++it1,it2);
    }

    //NUMBER AND ]
    if (isnum1 && isclose2){
        return false;
    }
    if (isclose1 && isnum2){
        return true;
    }
    //[ AND [
    if (isopen1 && isopen2){
        return processLines(++it1,++it2);
    }
    //] AND ]
    if (isclose1 && isclose2){
        return processLines(++it1,++it2);
    }
    //[ AND ]
    if (isopen1 && isclose2){
        return false;
    }
    //] AND [
    if (isclose1 and isopen2){
        return true;
    }

    
    return false;

}


int main(){

    std::ifstream file("data13.txt");
    std::string str1{};
    std::string str2{};
    std::string dummyline{};

    int pairIndex{1};
    int total{};
    std::vector<std::string> allstrings{};

    //PART ONE
    while(file.peek()!=EOF){
        getline(file,str1);
        allstrings.push_back(str1);
        getline(file,str2);
        allstrings.push_back(str2);
        getline(file,dummyline);

        auto it1 = str1.begin();
        auto it2 = str2.begin();

        bool result = processLines(it1,it2);

        if (result){
            total += pairIndex;
        }
        ++pairIndex;
    }
    std::cout<<total<<'\n';
    
    //PART TWO
    allstrings.push_back("[[2]]");
    allstrings.push_back("[[6]]");

    std::sort(allstrings.begin(),allstrings.end(),[](std::string a,std::string b){
        auto ita=a.begin();
        auto itb=b.begin();
        return processLines(ita,itb);
        });


    int result2{1};
    for (auto it=allstrings.begin();it<allstrings.end();++it){

        if (*it=="[[2]]"){
            result2 *= (it-allstrings.begin()+1);
        }
        if (*it=="[[6]]"){
            result2 *= (it-allstrings.begin()+1);
        }
    }
    std::cout<<result2<<'\n';


    
    return 0;
}



