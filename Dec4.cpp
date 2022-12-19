#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



int main(){

    std::ifstream file("data4.txt");
    std::string str{};
    int lower1{};
    int upper1{};
    int lower2{};
    int upper2{};
    char dummy{};
    int count_part1{};
    int count_part2{};

    while (getline(file,str)){
        std::stringstream ss{str};

        //EXTRACT UPPER AND LOWER BOUNDS
        ss>>lower1>>dummy>>upper1;
        ss>>dummy>>lower2>>dummy>>upper2;

        //PART ONE
             if (lower2>=lower1 && upper2<=upper1) ++count_part1;
        else if (lower1>=lower2 && upper1<=upper2) ++count_part1;

        //PART TWO
             if (upper1>=lower2 && lower2>=lower1) ++count_part2;
        else if (upper2>=lower1 && upper2<=upper1) ++count_part2;
        else if (upper2>=lower1 && lower1>=lower2) ++count_part2;
        else if (upper1>=lower2 && upper1<=upper2) ++count_part2;

    }
    std::cout<<count_part1<<'\n';
    std::cout<<count_part2<<'\n';


    return 0;
}



