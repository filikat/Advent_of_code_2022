#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <set>
#include <queue>
#include <cmath>
#include <map>
#include <deque>
#include <unordered_set>


/*
REFERENCE POSITIONS
ROCK 1: LEFT BIT
ROCK 2: CENTER
ROCK 3: CENTER
ROCK 4: TOP BIT
ROCK 5: TOP LEFT
*/


constexpr int max_x_pos = 6;
using mapset = std::map<long long,std::set<int>>;

void print(mapset& occupied){
    std::cout<<"OCCUPIED\n";
    for (auto it=occupied.begin();it!=occupied.end();++it){
        std::cout<<"y: "<<(it->first)<<"  x: ";
        for (auto itt=(it->second).begin();itt!=(it->second).end();++itt){
            std::cout<<(*itt)<<' ';
        }
        std::cout<<'\n';
    }
}

bool checkPosition(int x,long long y,const mapset& occupied){
    //RETURNS TRUE IF POSITION (X,Y) IS OCCUPIED
    auto found = occupied.find(y);
    if (found!=occupied.end()){
        std::set<int> y_occupied = (found->second);
        if (y_occupied.find(x)!=y_occupied.end()){
            return true;
        }
    }
    return false;
}

class Rock{
    public:
    int type{};
    std::vector<std::pair<int,long long>> positions{};

    void initialize(int t,int x,long long y);

    void printRefPos() const;

    void applyOperation(char,const mapset&);
    bool moveDown(mapset&);

    bool canGoLeft(const mapset&) const;
    bool canGoRight(const mapset&) const;
    bool canMoveDown(const mapset&) const;

    void addPositions(mapset&) const;

    long long max_y_pos() const;

};

//INITIALIZE ROCK
void Rock::initialize(int t,int x,long long y){
    type = t;
    positions.clear();

    switch (type){
        case 1:
            positions = {{x,y},{x+1,y},{x+2,y},{x+3,y}};
            break;
        case 2:
            positions = {{x,y},{x+1,y},{x-1,y},{x,y+1},{x,y-1}};
            break;
        case 3:
            positions = {{x,y},{x-1,y},{x-2,y},{x,y+1},{x,y+2}};
            break;
        case 4:
            positions = {{x,y},{x,y-1},{x,y-2},{x,y-3}};
            break;
        case 5:
            positions = {{x,y},{x+1,y},{x,y-1},{x+1,y-1}};
            break;
    }

}

//PRINT REFERENCE POSITION
void Rock::printRefPos() const{
    std::cout<<positions.front().first<<' '<<positions.front().second<<'\n';
}

//GO RIGHT/LEFT/DOWN
void Rock::applyOperation(char c,const mapset& occupied){
    switch (c){
        case '<':
            if (canGoLeft(occupied)){
                for (auto it=positions.begin();it<positions.end();++it){
                    --(it->first);
                }
            }
            break;
        case '>':
            if (canGoRight(occupied)){
                for (auto it=positions.begin();it<positions.end();++it){
                    ++(it->first);
                }
            }
            break;
        default:
            std::cerr<<"Wrong direction\n";
            break;
    }
}
bool Rock::moveDown(mapset& occupied){
    //RETURNS TRUE IF ROCK MOVED DOWN
    //        FALSE IF ROCK STOPPED
    if (canMoveDown(occupied)){
        for (auto it=positions.begin();it<positions.end();++it){
            --(it->second);
        }
        return true;
    }
    else{
        addPositions(occupied);
        return false;
    }
}

//CHECK IF ROCK CAN GO RIGHT/LEFT/DOWN
bool Rock::canGoLeft(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->first)==0) return false;
        if (checkPosition((it->first)-1,(it->second),occupied)) return false;
    }    

    //std::cout<<"Safe to go left\n";
    return true;

}
bool Rock::canGoRight(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->first)==max_x_pos) return false;
        if (checkPosition((it->first)+1,(it->second),occupied)) return false;
    }    

    //std::cout<<"Safe to go right\n";
    return true;

}
bool Rock::canMoveDown(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->second)==1) return false;
        if (checkPosition((it->first),(it->second)-1,occupied)) return false;
    }    

    //std::cout<<"Safe to go down\n";
    return true;

}

//SAVE CURRENT POSITION AS OCCUPIED
void Rock::addPositions(mapset& occupied) const{

    for (auto it=positions.begin();it<positions.end();++it){
        auto found = occupied.find(it->second);
        if (found!=occupied.end()){
            (found->second).insert(it->first);
        }else{
            std::pair<int,std::set<int>> cur_pos{it->second,{it->first}};
            occupied.insert(cur_pos);
        }
    }    

}

//RETURN MAX Y POSITION
long long Rock::max_y_pos() const{
    long long max_y{};
    for (auto it=positions.begin();it<positions.end();++it){
        max_y = std::max(max_y,it->second);
    }
    return max_y;
}

//HASH FUNCTION FOR TYPE_INDEX, OP_INDEX AND TOP 20 ROWS
std::string top20(int highest,const mapset& occupied,int type_index,int op_index){
    int height = 20;
    std::string str{};
    
    for (long long y=highest-height+1;y<=highest;++y){
        for (int x: occupied.find(y)->second){
            str.push_back(x+'0');
        }
        str.push_back('_');
    }
    str.push_back(type_index+'0');
    str.push_back('_');
    str.append(std::to_string(op_index));

    return str;
}

//READ OPERATIONS
std::deque<char> read_operations(std::ifstream& file){
    std::string str{};
    file>>str;
    std::stringstream ss{str};
    char c{};
    std::deque<char> operations{};
    while (ss.get(c)) operations.push_back(c);
    return operations;
}
std::deque<int> types{1,2,3,4,5};

void initialize_rock(Rock& rock,long long highest_rock,int type){
    switch (type){
        case 1:
            rock.initialize(1,2,highest_rock+4);
            break;
        case 2:
            rock.initialize(2,3,highest_rock+5);
            break;
        case 3:
            rock.initialize(3,4,highest_rock+4);
            break;
        case 4:
            rock.initialize(4,2,highest_rock+7);
            break;
        case 5:
            rock.initialize(5,2,highest_rock+5);
            break;
    }
}



int main(){

    std::ifstream file("data17.txt");
    auto operations{read_operations(file)};

    mapset occupied{};

    Rock rock{};
    long long highest_rock{};

    long long rock_count{};
    int type_index{};
    int op_index{};

    std::map<std::string,int> visited{};
    bool cont_hash{true};
    int start_repetition{};
    int end_repetition{};
    int period{};
    std::vector<long long> heights{};
    
    while (rock_count<2022){
        int cur_type = types.front();
        types.pop_front();
        types.push_back(cur_type);
        initialize_rock(rock,highest_rock,cur_type);


        while (true){
            char op = operations.front();
            operations.pop_front();
            operations.push_back(op);

            rock.applyOperation(op,occupied);
            op_index = (op_index+1)%operations.size();

            bool cont = rock.moveDown(occupied);
            if (!cont) break;
        }

        type_index = (type_index+1)%types.size();

        ++rock_count;
        highest_rock = std::max(highest_rock,rock.max_y_pos());
        //std::cout<<"Done "<<rock_count<<" size "<<occupied.size()<<'\n';


        heights.push_back(highest_rock);

        //CHECK FOR REPETITIONS
        if (highest_rock>20 && cont_hash){

            std::string cur_top_20 = top20(highest_rock,occupied,type_index,op_index);
            if (visited.find(cur_top_20)!=visited.end()){
                start_repetition = visited.find(cur_top_20)->second;
                end_repetition = rock_count;
                period = end_repetition-start_repetition;
                cont_hash = false;
            }else{
                visited.insert({cur_top_20,rock_count});
            }

        }
    }


    std::cout<<"Part 1: "<<highest_rock<<"\n\n";

    long long number{1000000000000};
    long long times = (number-start_repetition+1)/period;
    long long buffer = number-start_repetition+1-times*period;
    long long hperiod = heights[end_repetition-1] - heights[start_repetition-1];


    std::cout<<number<<" = "<<start_repetition-1<<" + "<<times<<" * "<<period<<" + "<<buffer<<"\n\n";
    std::cout<<"Part 2: "<<hperiod*times+heights[start_repetition+buffer-2]<<'\n';

    return 0;
}



