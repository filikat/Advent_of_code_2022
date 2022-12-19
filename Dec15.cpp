#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>


class Sensor{
    
    const int x_pos{};
    const int y_pos{};
    const int x_bea{};
    const int y_bea{};
    const int dist{};

    public:
    Sensor(int x,int y,int x_s,int y_s):
        x_pos{x},y_pos{y},
        x_bea{x_s},y_bea{y_s},
        dist{std::abs(x_pos-x_bea)+std::abs(y_pos-y_bea)}
    {
    }

    int computeDist(int x,int y){
        return std::abs(x_pos-x)+std::abs(y_pos-y);
    }

    friend class MapSensor;

};

class MapSensor{
    public:
    std::vector<Sensor> sensors{};
    int xmin{};
    int xmax{};
    int ymin{};
    int ymax{};

    int partOne();
    long long partTwo();
    bool checkPoint(int x,int y);
    
};

int MapSensor::partOne(){
    std::set<int> impossible_x{};
    for (int x=10*xmin;x<=10*xmax;++x){
    for (Sensor s: sensors){
        if (s.computeDist(x,ymin)<=s.dist && (x!=s.x_bea || ymin!=s.y_bea)){
                impossible_x.insert(x);
                break;
            }
        }
    }
    return impossible_x.size();
}

long long MapSensor::partTwo(){
    std::vector<std::vector<int>> direction {{1,-1},{-1,-1},{1,1},{-1,1}};

    int xp{};
    int yp{};

    for (Sensor s: sensors){
        
        int yUp = s.y_pos + s.dist + 1;
        int yDown = s.y_pos - s.dist - 1;

        int xp_start{s.x_pos};
        std::vector<int> yp_start{yUp,yUp,yDown,yDown};

        bool found{false};

        for (int j=0;j<4;++j){

            if (!found){
                xp = xp_start;
                yp = yp_start[j];

                while (yp>=ymin && yp<=ymax && xp>=xmin && xp<=xmax){
                    bool res = checkPoint(xp,yp);
                    if (res){
                        found = true;
                        break;
                    }
                    xp += direction[j][0];
                    yp += direction[j][1];
                }
            }
        }

        if (found) break;
    }
    return (4000000*static_cast<long long>(xp)+yp);
}

bool MapSensor::checkPoint(int x,int y){
    //RETURNS TRUE IF POINT CAN CONTAIN BEACON

    if (y<ymin || y>ymax || x<xmin || x>xmax) return false;

    for (Sensor s: sensors){
        if (s.computeDist(x,y)<=s.dist) return false;
    }
    return true;

}

Sensor readLine(std::string str,int& x_min,int& x_max){
    std::string dummys{};
    char dummyc{};
    std::stringstream ss{str};
    ss>>dummys>>dummys;
    ss>>dummyc>>dummyc;

    int x_pos{};
    ss>>x_pos;
    x_min = std::min(x_min,x_pos);
    x_max = std::max(x_max,x_pos);
    ss>>dummyc>>dummyc>>dummyc;
    int y_pos{};
    ss>>y_pos;

    ss>>dummyc>>dummys>>dummys>>dummys>>dummys;
    ss>>dummyc>>dummyc;

    int x_bea{};
    ss>>x_bea;
    x_min = std::min(x_min,x_bea);
    x_max = std::max(x_max,x_bea);
    ss>>dummyc>>dummyc>>dummyc;
    int y_bea{};
    ss>>y_bea;

    return Sensor(x_pos,y_pos,x_bea,y_bea);
}


int main(){

    bool part_one{false};
    std::ifstream file("data15.txt");
    std::string str{};

    MapSensor map{};
    int x_min{};
    int x_max{};

    while (getline(file,str)){
        map.sensors.push_back(readLine(str,x_min,x_max));
    }    

    if (part_one){
        //PART ONE
        constexpr int y_index{2000000};
        map.ymin = y_index;
        map.ymax = y_index;
        map.xmin = x_min;
        map.xmax = x_max;
        std::cout<<"Part 1 "<<map.partOne()<<'\n';
    }else{
        //PART TWO
        map.xmin = 0;
        map.ymin = 0;
        map.xmax = 4000000;
        map.ymax = 4000000;
        std::cout<<"Part 2 "<<map.partTwo()<<'\n';
    }

    
    return 0;
}



