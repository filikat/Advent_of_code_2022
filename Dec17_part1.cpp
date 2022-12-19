#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <deque>
#include <chrono>


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
    bool moveDown(mapset&,std::vector<long long>&);

    bool canGoLeft(const mapset&) const;
    bool canGoRight(const mapset&) const;
    bool canMoveDown(const mapset&) const;

    void addPositions(mapset&,std::vector<long long>&) const;

    long long max_y_pos() const;

    void updateOcc(mapset&,std::vector<long long>&) const;

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
bool Rock::moveDown(mapset& occupied,std::vector<long long>& max_per_col){
    //RETURNS TRUE IF ROCK MOVED DOWN
    //        FALSE IF ROCK STOPPED
    if (canMoveDown(occupied)){
        for (auto it=positions.begin();it<positions.end();++it){
            --(it->second);
        }
        return true;
    }
    else{
        addPositions(occupied,max_per_col);
        updateOcc(occupied,max_per_col);
        return false;
    }
}

//CHECK IF ROCK CAN GO RIGHT/LEFT/DOWN
bool Rock::canGoLeft(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->first)==0) return false;
        if (checkPosition((it->first)-1,(it->second),occupied)) return false;
    }    

    return true;

}
bool Rock::canGoRight(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->first)==max_x_pos) return false;
        if (checkPosition((it->first)+1,(it->second),occupied)) return false;
    }    

    return true;

}
bool Rock::canMoveDown(const mapset& occupied) const {

    for (auto it=positions.begin();it<positions.end();++it){
        if ((it->second)==1) return false;
        if (checkPosition((it->first),(it->second)-1,occupied)) return false;
    }    

    return true;

}

//SAVE CURRENT POSITION AS OCCUPIED
void Rock::addPositions(mapset& occupied,std::vector<long long>& max_per_col) const{

    for (auto it=positions.begin();it<positions.end();++it){
        auto found = occupied.find(it->second);
        if (found!=occupied.end()){
            (found->second).insert(it->first);
        }else{
            std::pair<int,std::set<int>> cur_pos{it->second,{it->first}};
            occupied.insert(cur_pos);
        }
        max_per_col[it->first] = std::max(max_per_col[it->first],it->second);
    }    

}

void Rock::updateOcc(mapset& occupied,std::vector<long long>& max_per_col) const{
    
    long long min_max_y = *std::min_element(max_per_col.begin(),max_per_col.end());

    std::vector<int> toErase{};

    for (auto it = occupied.begin();it!=occupied.end();++it){
        if (it->first < min_max_y) toErase.push_back(it->first);
    }

    for (auto it = toErase.begin();it<toErase.end();++it){
        occupied.erase(*it);
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

//INITIALIZE ROCK BASED ON TYPE
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
    std::deque<int> types{1,2,3,4,5};

    mapset occupied{};
    std::vector<long long> max_per_col(7,0);

    Rock rock{};
    long long highest_rock{};
    long long rock_count{};

    auto t_start = std::chrono::high_resolution_clock::now();

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

            bool cont = rock.moveDown(occupied,max_per_col);
            if (!cont) break;
        }
        ++rock_count;
        highest_rock = std::max(highest_rock,rock.max_y_pos());
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = t_end-t_start;

    std::cout<<"Highest rock: "<<highest_rock<<'\n';
    std::cout<<duration.count()<<'\n';

    return 0;
}



