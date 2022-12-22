#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using WrapRules = std::map< std::pair<std::pair<int,int>,std::pair<int,int>> , std::pair<std::pair<int,int>,std::pair<int,int>> >;

void print(const std::vector<std::vector<char>>& map){

    std::cout<<"  ";
    for (int i=0;i<map[0].size();++i){
        std::cout<<i%10;
    }

    int i{};
    for (auto it=map.begin();it!=map.end();++it){
        std::cout<<(i++)%10<<' ';
        for (auto itt=it->begin();itt!=it->end();++itt){
            std::cout<<*itt;
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void read_file(std::ifstream& file,std::vector<std::vector<char>>& map,std::string& operations){
    std::string str{};

    int max_row_size{};


    while (getline(file,str)){

        if (str.front()==' ' || str.front()=='.' || str.front()=='#'){

            std::vector<char> row{};
            row.push_back(' ');
            for (auto it=str.begin();it!=str.end();++it){
                    row.push_back(*it);
            }
            row.push_back(' ');
            max_row_size = std::max(max_row_size,static_cast<int>(row.size()));
            map.push_back(row);

        }else{
            operations = str;
        }

    }

    for (auto it=map.begin();it!=map.end();++it){
        if (it->size()<max_row_size){
            size_t itsize = it->size();
            for (int i=0;i<max_row_size-itsize;++i){
                it->push_back(' ');
            }
        }
    }

    map.push_back(std::vector<char> (max_row_size,' '));
    map.insert(map.begin(),std::vector<char> (max_row_size,' '));

}

void starting_point(const std::vector<char>& row,std::pair<int,int>& point){
    point.first = 1;
    for (auto it=row.begin();it!=row.end();++it){
        if (*it=='.'){
            point.second = it-row.begin();
            break;
        }
    }
}

void move(const std::vector<std::vector<char>>& map,std::pair<int,int>& point,std::pair<int,int>& direction,int value,
            const WrapRules& wrapRules){

    std::pair<int,int> next_point{};

    for (int i=0;i<value;++i){


        char next = map[point.first+direction.first][point.second+direction.second];

        if (next=='.'){
            point.first += direction.first;
            point.second += direction.second;
        }else if(next=='#'){
            break;
        }else{
            //IF NEXT POINT IS EMPTY, LOOK AT WRAPRULES TO FIND NEXT POINT AND DIRECTION
            auto found = wrapRules.find({point,direction});
            next_point = found->second.first;
            char next_char = map[next_point.first][next_point.second];
            if (next_char=='#'){
                break;
            }else if(next_char=='.'){
                point = next_point;
                direction = found->second.second;
            }
        }
    }
}

void rotation(std::pair<int,int>& direction,char rotate){
    std::vector<std::pair<int,int>> directions {{0,1},{1,0},{0,-1},{-1,0}};

    int index{};
    for (int i=0;i<4;++i){
        if (direction==directions[i]){
            index = i;
            break;
        }
    }

    if (rotate=='R'){
        ++index;
        if (index==4) index = 0;
    }else{
        --index;
        if (index==-1) index = 3;
    }

    direction = directions[index];
}

int direction_score(const std::pair<int,int>& direction){
    if (direction==std::pair<int,int> {0,1})  return 0;
    if (direction==std::pair<int,int> {1,0})  return 1;
    if (direction==std::pair<int,int> {0,-1}) return 2;
    return 3;
}

void print(const std::pair<int,int>& p){
    std::cout<<'('<<p.first<<','<<p.second<<")\n";
}

void wrappingMap(WrapRules& wrapRules){
    //WRAPRULES ASSOCIATES EACH POINT ON THE BORDER TO THE POINT AND DIRECTION IT WRAPS AROUND TO

    std::pair<int,int> cur_point{};
    std::pair<int,int> arrive{};
    std::pair<int,int> cur_direction{};
    std::pair<int,int> direction{};


    //UP SIDE OF 1
    cur_direction = {-1,0};
    for (int i=0;i<50;++i){
        cur_point = {1,101+i};
        arrive = {200,1+i};
        direction = {-1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});

    }

    //RIGHT SIDE OF 1
    cur_direction = {0,1};
    for (int i=0;i<50;++i){
        cur_point = {1+i,150};
        arrive = {150-i,100};
        direction = {0,-1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //DOWN SIDE OF 1
    cur_direction = {1,0};
    for (int i=0;i<50;++i){
        cur_point = {50,150-i};
        arrive = {100-i,100};
        direction = {0,-1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //RIGHT SIDE OF 3
    cur_direction = {0,1};
    for (int i=0;i<50;++i){
        cur_point = {51+i,100};
        arrive = {50,101+i};
        direction = {-1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //RIGHT SIDE OF 4
    cur_direction = {0,1};
    for (int i=0;i<50;++i){
        cur_point = {101+i,100};
        arrive = {50-i,150};
        direction = {0,-1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //DOWN SIDE OF 4
    cur_direction = {1,0};
    for (int i=0;i<50;++i){
        cur_point = {150,100-i};
        arrive = {200-i,50};
        direction = {0,-1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //RIGHT SIDE OF 6
    cur_direction = {0,1};
    for (int i=0;i<50;++i){
        cur_point = {151+i,50};
        arrive = {150,51+i};
        direction = {-1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //DOWN SIDE OF 6
    cur_direction = {1,0};
    for (int i=0;i<50;++i){
        cur_point = {200,50-i};
        arrive = {1,150-i};
        direction = {1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //LEFT SIDE OF 6
    cur_direction = {0,-1};
    for (int i=0;i<50;++i){
        cur_point = {200-i,1};
        arrive = {1,100-i};
        direction = {1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //LEFT SIDE OF 5
    cur_direction = {0,-1};
    for (int i=0;i<50;++i){
        cur_point = {150-i,1};
        arrive = {1+i,51};
        direction = {0,1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //TOP SIDE OF 5
    cur_direction = {-1,0};
    for (int i=0;i<50;++i){
        cur_point = {101,1+i};
        arrive = {51+i,51};
        direction = {0,1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }    

    //LEFT SIDE OF 3
    cur_direction = {0,-1};
    for (int i=0;i<50;++i){
        cur_point = {100-i,51};
        arrive = {101,50-i};
        direction = {1,0};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //LEFT SIDE OF 2
    cur_direction = {0,-1};
    for (int i=0;i<50;++i){
        cur_point = {50-i,51};
        arrive = {101+i,1};
        direction = {0,1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }

    //TOP SIDE OF 2
    cur_direction = {-1,0};
    for (int i=0;i<50;++i){
        cur_point = {1,51+i};
        arrive = {151+i,1};
        direction = {0,1};
        wrapRules.insert({{cur_point,cur_direction},{arrive,direction}});
    }


}

int main(){

    std::ifstream file("data22.txt");

    std::vector<std::vector<char>> map{};
    std::string operations{};
    read_file(file,map,operations);
    
    //print(map);

    std::pair<int,int> point{};
    starting_point(map[1],point);

    std::stringstream opstream{operations};
    std::pair<int,int> direction{0,1};

    int value{};
    char rotate{};

    WrapRules wrapRules{};
    wrappingMap(wrapRules);

    while (true){

        opstream>>value;
        move(map,point,direction,value,wrapRules);
        //std::cout<<point.first<<' '<<point.second<<'\n';

        opstream>>rotate;
        if (rotate=='X') break;
        else{
            rotation(direction,rotate);
        }

    }

    std::cout<<"Score "<<1000*point.first+4*point.second+direction_score(direction)<<'\n';




    return 0;
}



