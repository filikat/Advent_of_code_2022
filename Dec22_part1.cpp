#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

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

void move_row(const std::vector<std::vector<char>>& map,std::pair<int,int>& point,int direction,int value){
    //MOVE IN ROW FROM POINT ALONG DIRECTION FOR AN AMOUNT VALUE
    
    for (int i=0;i<value;++i){
        char next = map[point.first][point.second+direction];

        if (next=='.'){
            point.second += direction;
        }else if(next=='#'){
            break;
        }else{
            //IF NEXT POINT IS EMPTY, LOOK BACK FOR BEGINNING/END OF ROW

            int next_coord{point.second};
            while (map[point.first][next_coord]!=' '){
                next_coord -= direction;
            }
            next_coord += direction;

            char new_next = map[point.first][next_coord];
            if (new_next=='.'){
                point.second = next_coord;
            }else if (new_next=='#'){
                break;
            }

        }
    }

}

void move_col(const std::vector<std::vector<char>>& map,std::pair<int,int>& point,int direction,int value){
    //MOVE IN COL FROM POINT ALONG DIRECTION FOR AN AMOUNT VALUE
    
    for (int i=0;i<value;++i){
        char next = map[point.first+direction][point.second];

        if (next=='.'){
            point.first += direction;
        }else if(next=='#'){
            break;
        }else{
            //IF NEXT POINT IS EMPTY, LOOK BACK FOR BEGINNING/END OF COL

            int next_coord{point.first};
            while (map[next_coord][point.second]!=' '){
                next_coord -= direction;
            }
            next_coord += direction;

            char new_next = map[next_coord][point.second];
            if (new_next=='.'){
                point.first = next_coord;
            }else if (new_next=='#'){
                break;
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




int main(){


    std::ifstream file("data22.txt");

    std::vector<std::vector<char>> map{};
    std::string operations{};
    read_file(file,map,operations);
    
    print(map);

    std::pair<int,int> point{};
    starting_point(map[1],point);

    std::stringstream opstream{operations};
    std::pair<int,int> direction{0,1};

    int value{};
    char rotate{};

    while (opstream.peek()!=EOF){

        opstream>>value;
        if (direction.first==0){
            move_row(map,point,direction.second,value);
        }else{
            move_col(map,point,direction.first,value);
        }

        opstream>>rotate;
        if (rotate=='X') break;
        else{
            rotation(direction,rotate);
        }

        //std::cout<<point.first<<' '<<point.second<<'\n';

    }

    std::cout<<"Score "<<1000*point.first+4*point.second+direction_score(direction)<<'\n';




    return 0;
}



