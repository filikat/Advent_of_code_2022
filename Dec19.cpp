#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <queue>
#include <map>
#include <chrono>

class Cost{
    public:
    int ore4ore{};
    int ore4clay{};
    int ore4obs{};
    int clay4obs{};
    int ore4geode{};
    int obs4geode{};
    int maxore{};

    void compute_maxore(){
        std::vector ores{ore4ore,ore4clay,ore4obs,ore4geode};
        maxore = *std::max_element(ores.begin(),ores.end());
    }
};

int recursion(int ore,int clay,int obs,int geode,int r_ore,int r_clay,int r_obs,int r_geode,const Cost& cost,int minute,std::map<std::vector<int>,int>& computed,int maxMinutes){

    //CHECK IF SCENARIO ALREADY HAPPENED
    std::vector<int> todo{ore,clay,obs,geode,r_ore,r_clay,r_obs,r_geode,minute};
    auto found = computed.find(todo);
    if (found!=computed.end()){
        return found->second;
    }

    //BASE CASE
    if (minute==maxMinutes) return geode;

    //NEW AMOUNTS
    int n_ore = ore + r_ore;
    int n_clay = clay + r_clay;
    int n_obs = obs + r_obs;
    int n_geode = geode + r_geode;

    //IF CAN BUILD GEODE, DO IT
    if (ore>=cost.ore4geode && obs>=cost.obs4geode){
        return recursion(n_ore-cost.ore4geode,n_clay,n_obs-cost.obs4geode,n_geode,r_ore,r_clay,r_obs,r_geode+1,cost,minute+1,computed,maxMinutes);
    }    

    std::vector<int> results{};

    //BRANCH ON DIFFERENT POSSIBILITIES:
    //(DO IT ONLY IF A CERTAIN POSSIBILITY MAKES SENSE, GIVEN HOW MUCH MATERIAL IS NEEDED BY THE ROBOTS)

    //DO NOTHING
    results.push_back(recursion(n_ore,n_clay,n_obs,n_geode,r_ore,r_clay,r_obs,r_geode,cost,minute+1,computed,maxMinutes));

    //BUILD ORE
    if (r_ore<cost.maxore && r_ore*(maxMinutes-minute)+ore<cost.maxore*(maxMinutes-minute)){
        if (ore>=cost.ore4ore){
            results.push_back(recursion(n_ore-cost.ore4ore,n_clay,n_obs,n_geode,r_ore+1,r_clay,r_obs,r_geode,cost,minute+1,computed,maxMinutes));
        }
    }

    //BUILD CLAY
    if (r_clay<cost.clay4obs && r_clay*(maxMinutes-minute)+clay<cost.clay4obs*(maxMinutes-minute)){
        if (ore>=cost.ore4clay){
            results.push_back(recursion(n_ore-cost.ore4clay,n_clay,n_obs,n_geode,r_ore,r_clay+1,r_obs,r_geode,cost,minute+1,computed,maxMinutes));
        }
    }

    //BUILD OBS
    if (r_obs<cost.obs4geode && r_obs*(maxMinutes-minute)+obs<cost.obs4geode*(maxMinutes-minute)){
        if (ore>=cost.ore4obs && clay>=cost.clay4obs){
            results.push_back(recursion(n_ore-cost.ore4obs,n_clay-cost.clay4obs,n_obs,n_geode,r_ore,r_clay,r_obs+1,r_geode,cost,minute+1,computed,maxMinutes));
        }
    }

    int result = *std::max_element(results.begin(),results.end());
    computed.insert({todo,result});

    return(result);
}

Cost read_line(std::string& str){
    std::stringstream ss{str};
    std::string dummy{};

    Cost cost{};

    ss>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>cost.ore4ore;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>cost.ore4clay;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>cost.ore4obs;

    ss>>dummy>>dummy;
    ss>>cost.clay4obs;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>cost.ore4geode;

    ss>>dummy>>dummy;
    ss>>cost.obs4geode;

    cost.compute_maxore();

    return cost;
}



int main(){

    bool part_one{false};

    int maxMinutes{};
    if (part_one) maxMinutes = 24;
    else maxMinutes = 32;

    std::ifstream file("data19.txt");
    std::string str{};

    int index{1};
    int total{};
    if (!part_one) total = 1;

    auto t_start = std::chrono::high_resolution_clock::now();

    while(getline(file,str)){
        if (!part_one && index>3) break;
        
        Cost cost{read_line(str)};

        std::map<std::vector<int>,int> computed{};

        int result{recursion(0,0,0,0,1,0,0,0,cost,0,computed,maxMinutes)};
        std::cout<<index<<' '<<result<<'\n';

        if (part_one) total += index*result;
        else total *= result;

        ++index;
    }
    
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = t_end-t_start;

    std::cout<<total<<'\n';
    std::cout<<duration.count()<<'\n';

    return 0;
}



