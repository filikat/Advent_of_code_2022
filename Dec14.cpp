#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

char convert(int i){
    switch (i){
        case 0: return '.';
        case 1: return '#';
        case 2: return 'o';
        default: return '?';
    }
}

class SandMap{
    const int height{};
    const int width{};
    const int startCol{};
    const int dropCol{};
    std::vector<std::vector<int>> map{};    //MAP: 0-AIR  1-WALL  2-SAND

    public:
    SandMap(int h,int w,int sC,int dC):
        height{h},width{w},startCol{sC},dropCol{dC},
        map{std::vector<std::vector<int>> (height,std::vector<int>(width,0))}
        {
        }
    void print() const;
    void updateMap(int col0,int row0,int col1,int row1);
    bool dropSand_part1();
    bool dropSand_part2();

};

void SandMap::print() const {
    for (auto it=map.begin();it<map.end();++it){
        for (auto itt=(*it).begin();itt<(*it).end();++itt){
            std::cout<<convert(*itt)<<' ';
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void SandMap::updateMap(int col0,int row0,int col1,int row1){
    if (col0==col1){
        int colIndex{col0-startCol};
        for (int row=std::min(row0,row1);row<=std::max(row0,row1);++row){
            map[row][colIndex] = 1;
        }
    }else{
        for (int col=std::min(col0,col1);col<=std::max(col0,col1);++col){
            int colIndex = col-startCol;
            map[row0][colIndex] = 1;
        }
    }
}

bool SandMap::dropSand_part1(){
    //TRUE IF SAND DIDN'T FALL OFF THE EDGE

    int sandCol{dropCol};
    int sandRow{0};
    while(true){

        if (sandRow<map.size()-1){

            if (map[sandRow+1][sandCol]==0) ++sandRow;
            else if(sandCol>0 && map[sandRow+1][sandCol-1]==0){
                ++sandRow;
                --sandCol;
            }else if(sandCol<map[0].size()-1 && map[sandRow+1][sandCol+1]==0){
                ++sandRow;
                ++sandCol;
            }else{
                map[sandRow][sandCol] = 2;
                return true;
            }

        }else{
            return false;
        }

    }
    return true;
}

bool SandMap::dropSand_part2(){
    //TRUE IF SAND DIDN'T STOP AT THE FIRST STEP

    int sandCol{dropCol};
    int sandRow{0};
    while(true){

        if (map[sandRow+1][sandCol]==0) ++sandRow;
        else if(sandCol>0 && map[sandRow+1][sandCol-1]==0){
            ++sandRow;
            --sandCol;
        }else if(sandCol<map[0].size()-1 && map[sandRow+1][sandCol+1]==0){
            ++sandRow;
            ++sandCol;
        }else if(sandRow==0){
            map[sandRow][sandCol] = 2;
            return false;
        }else{
            map[sandRow][sandCol] = 2;
            return true;
        }

    }
    return true;
}

int main(){

    std::ifstream file("data14.txt");
    std::string str{};
    char dummy{};
    int row0{};
    int col0{};
    int row1{};
    int col1{};

    constexpr bool part2{false}; 

    //FIND SIZE OF THE PROBLEM
    int minCol{INT_MAX};
    int maxCol{0};
    int maxRow{0};
    while (getline(file,str)){
        std::stringstream ss{str};
        ss>>col0>>dummy>>row0;
        minCol = std::min(minCol,col0);
        maxCol = std::max(maxCol,col0);
        maxRow = std::max(maxRow,row0);

        while (ss.peek()!=EOF){
            ss>>dummy>>dummy;
            ss>>col0>>dummy>>row0;
            minCol = std::min(minCol,col0);
            maxCol = std::max(maxCol,col0);
            maxRow = std::max(maxRow,row0);
        }
    }

    //RESET FILE TO BEGINNING
    file.clear();
    file.seekg(0);

    //CREATE SANDMAP
    int height{};
    int width{};
    int startCol{};
    int dropCol{};

    height = maxRow+3;
    width = height*2+1;
    startCol = 500-height;
    dropCol = 500-startCol;

    SandMap sand{height,width,startCol,dropCol};

    while (getline(file,str)){
        std::stringstream ss{str};
        ss>>col0>>dummy>>row0;
        
        while (ss.peek()!=EOF){
            ss>>dummy>>dummy;
            ss>>col1>>dummy>>row1;
            sand.updateMap(col0,row0,col1,row1);
            col0 = col1;
            row0 = row1;
        }
    }
    if (part2){
        col0=startCol;
        col1=startCol+width;
        row0=height-1;
        row1=height-1;
        sand.updateMap(col0,row0,col1,row1);
    }

    int iter{0};
    if (part2){  
        do {
            ++iter;
        } while(sand.dropSand_part2());
    }else{
        while(sand.dropSand_part1()){
            ++iter;
        };  
    }
    std::cout<<iter<<'\n';
    //sand.print();

    return 0;
}



