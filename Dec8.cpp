#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


void print(const std::vector<std::vector<int>>& map){
    for (auto it=map.begin();it<map.end();++it){
        for (auto itt=(*it).begin();itt<(*it).end();++itt){
            std::cout<<*itt<<' ';
        }
        std::cout<<'\n';
    }
}

//PART ONE
bool visible_left(const std::vector<std::vector<int>>& map,int i,int j){
    //RETURNS TRUE IF ELEMENT I,J IS VISIBLE FROM THE LEFT
    int maxrow{};
    for (int jj=0;jj<j;++jj){
        maxrow = std::max(maxrow,map[i][jj]);
    }
    if (maxrow<map[i][j]) return true;
    return false;
}
bool visible_right(const std::vector<std::vector<int>>& map,int i,int j){
    //RETURNS TRUE IF ELEMENT I,J IS VISIBLE FROM THE RIGHT
    int maxrow{};
    for (int jj=j+1;jj<map[i].size();++jj){
        maxrow = std::max(maxrow,map[i][jj]);
    }
    if (maxrow<map[i][j]) return true;
    return false;
}
bool visible_top(const std::vector<std::vector<int>>& map,int i,int j){
    //RETURNS TRUE IF ELEMENT I,J IS VISIBLE FROM THE TOP
    int maxrow{};
    for (int ii=0;ii<i;++ii){
        maxrow = std::max(maxrow,map[ii][j]);
    }
    if (maxrow<map[i][j]) return true;
    return false;
}
bool visible_bottom(const std::vector<std::vector<int>>& map,int i,int j){
    //RETURNS TRUE IF ELEMENT I,J IS VISIBLE FROM THE BOTTOM
    int maxrow{};
    for (int ii=i+1;ii<map.size();++ii){
        maxrow = std::max(maxrow,map[ii][j]);
    }
    if (maxrow<map[i][j]) return true;
    return false;
}
int analyze(const std::vector<std::vector<int>>& map){
    int total{};
    for (int i=1;i<map.size()-1;++i){
        for (int j=1;j<map[0].size()-1;++j){
            if (visible_left(map,i,j)){
                ++total;
            }else
            if (visible_right(map,i,j)){
                ++total;
            }else
            if (visible_top(map,i,j)){
                ++total;
            }else
            if (visible_bottom(map,i,j)){
                ++total;
            }
        }
    }
    return total;
}

//PART TWO
int count_left(const std::vector<std::vector<int>>& map,int i,int j){
    //COUNTS THE TREES VISIBLE ON THE LEFT
    int count{};
    for (int jj=j-1;jj>=0;--jj){
        if (map[i][jj]<map[i][j]) ++count;
        else {
            ++count;
            break;
        }
    }
    return count;
}
int count_right(const std::vector<std::vector<int>>& map,int i,int j){
    //COUNTS THE TREES VISIBLE ON THE RIGHT
    int count{};
    for (int jj=j+1;jj<map[i].size();++jj){
        if (map[i][jj]<map[i][j]) ++count;
        else {
            ++count;
            break;
        }
    }
    return count;
}
int count_top(const std::vector<std::vector<int>>& map,int i,int j){
    //COUNTS THE TREES VISIBLE ON THE TOP
    int count{};
    for (int ii=i-1;ii>=0;--ii){
        if (map[ii][j]<map[i][j]) ++count;
        else {
            ++count;
            break;
        }
    }
    return count;
}
int count_bottom(const std::vector<std::vector<int>>& map,int i,int j){
    //COUNTS THE TREES VISIBLE ON THE BOTTOM
    int count{};
    for (int ii=i+1;ii<map.size();++ii){
        if (map[ii][j]<map[i][j]) ++count;
        else {
            ++count;
            break;
        }
    }
    return count;
}
int analyze2(const std::vector<std::vector<int>>& map){
    int best{};
    for (int i=1;i<map.size()-1;++i){
        for (int j=1;j<map[0].size()-1;++j){
            best = std::max(best,
            count_left(map,i,j)*count_right(map,i,j)*count_top(map,i,j)*count_bottom(map,i,j)
            );
        }
    }
    return best;
}



int main(){

    //READ DATA
    std::ifstream file("data8.txt");
    std::string str{};
    char height{};
    std::vector<std::vector<int>> map{};

    while(getline(file,str)){
        std::stringstream ss{str};
        std::vector<int> row{};

        while (ss.peek()!=EOF){
            ss>>height;
            row.push_back(height-'0');
        }
        map.push_back(row);
    }

    int visible_int{analyze(map)};
    std::cout<<visible_int+map.size()*2+(map[0].size()-2)*2<<'\n';


    //PART TWO
    std::cout<<analyze2(map)<<'\n';




    return 0;
}



