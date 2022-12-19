#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



int checkCycle(int cycle,int regX){
    if (
        cycle==20 ||
        cycle==60 ||
        cycle==100||
        cycle==140||
        cycle==180||
        cycle==220){
            return cycle*regX;
        }
        return 0;
}

void draw(const std::vector<int>& pixels){
    for (int i=0;i<240;++i){
        if (pixels[i]==0) std::cout<<'.';
        else std::cout<<'#';
        if (i%40==39) std::cout<<'\n';
    }
}

void checkPixel(int& cur_pixel,int regX,std::vector<int>& pixels){
    if (std::abs((cur_pixel%40)-regX)<=1){
        pixels[cur_pixel] = 1;
    }
    ++cur_pixel;
}


int main(){

    //READ DATA
    std::ifstream file("data10.txt");
    std::string str{};
    int regX{1};
    int cycle{};
    std::string instruction{};
    int amount{};
    int total{};

    std::vector<int> pixels(240,0);
    int cur_pixel{};

    while (getline(file,str)){
        std::stringstream ss{str};
        ss>>instruction;
        if (instruction=="noop") {
            ++cycle;
            total += checkCycle(cycle,regX);
            checkPixel(cur_pixel,regX,pixels);


        }else{
            ss>>amount;
            ++cycle;
            total += checkCycle(cycle,regX);
            checkPixel(cur_pixel,regX,pixels);
            ++cycle;
            total += checkCycle(cycle,regX);
            checkPixel(cur_pixel,regX,pixels);
            regX += amount;
        }

    }
    std::cout<<total<<'\n';
    draw(pixels);

    return 0;
}



