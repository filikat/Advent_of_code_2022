#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int main(){

    std::ifstream file("data1.txt");

    std::string str{};
    std::vector<int> calories{};
    int cur_cal{};

    //GO THROUGH THE LINES OF THE DATA
    while (getline(file,str)){

        //IF THE LINE IS NOT EMPTY, SUM THE VALUE TO THE CURRENT TOTAL
        if (str.size()>0){
            cur_cal += std::stoi(str);
        }

        //IF THE LINE IS EMPTY, SAVE THE CURRENT TOTAL AND SET IT TO ZERO FOR NEXT ITERATION
        else{
            calories.push_back(cur_cal);
            cur_cal = 0;
        }

    }

    //SORT ALL THE CALORIES IN DECREASING ORDER
    std::sort(calories.begin(),calories.end(),std::greater<int>());

    //SUM THE THREE LARGEST VALUES
    std::cout<<calories[0]+calories[1]+calories[2]<<'\n';
    

    return 0;
}



