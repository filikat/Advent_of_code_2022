#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <queue>
#include <map>
#include <cmath>
#include <chrono>


using Node = std::vector<int>; //STATE OF THE SYSTEM:   node[0] = minute
                               //                       node[1] = ore
                               //                       node[2] = clay
                               //                       node[3] = obsidian
                               //                       node[4] = geodes
                               //                       node[5] = ore robots
                               //                       node[6] = clay robots
                               //                       node[7] = obsidian robots
                               //                       node[8] = geode robots
                               

void print(const Node& node){
    for (int i: node){
        std::cout<<i<<' ';
    }
    std::cout<<'\n';
}



class Graph{
    public:
    int ore4ore{};
    int ore4clay{};
    int ore4obs{};
    int clay4obs{};
    int ore4geode{};
    int obs4geode{};
    int maxore{};

    int maxMinutes{};
    int best_score{};

    void dfs(const Node&);
    std::vector<Node> getNeighbours(const Node&) const;
    void checkNode(const Node&);

    void compute_maxore(){
        std::vector ores{ore4ore,ore4clay,ore4obs,ore4geode};
        maxore = *std::max_element(ores.begin(),ores.end());
    }
};


void Graph::dfs(const Node& root){
    //DEPTH FIRST SEARCH

    std::stack<Node> S{};
    std::set<Node> visited{};
    S.push(root);

    while (!S.empty()){

        Node node{S.top()};
        S.pop();

        checkNode(node);

        auto resultInsert = visited.insert(node);
        if (resultInsert.second){

            const std::vector<Node> neighbours = getNeighbours(node);
            for (auto n: neighbours){
                S.push(n);
            }
        }
    }
}
void Graph::checkNode(const Node& node){
    //IF REACHED MAX MINUTE, SAVE BEST RESULT
    if (node[0]==maxMinutes){
        best_score = std::max(best_score,node[4]);
    }
}
std::vector<Node> Graph::getNeighbours(const Node& node) const{

    std::vector<Node> neighbours{};

    if (node[0]<maxMinutes){

        //BRANCH ON NEXT ROBOT TO BUILD

        //GEODE ROBOT
        if (node[7]>0 && (node[1]<ore4geode || node[3]<obs4geode)){
            //IF POSSIBLE TO BUILD IN THE FUTURE AND NOT POSSIBLE TO BUILD NOW

            int minute4geode = 1 + std::max(std::ceil(static_cast<double>(ore4geode-node[1])/node[5]),std::ceil(static_cast<double>(obs4geode-node[3])/node[7]));
            if (node[0]+minute4geode<=maxMinutes){
                Node cur_neigh{node};
                cur_neigh[0] += minute4geode;
                for (int i=1;i<=4;++i){
                    cur_neigh[i] += cur_neigh [i+4]*minute4geode;
                }
                cur_neigh[1] -= ore4geode;
                cur_neigh[3] -= obs4geode;
                ++cur_neigh[8];
                neighbours.push_back(cur_neigh);
            }
        }else if(node[1]>=ore4geode && node[3]>=obs4geode){
            //IF POSSIBLE TO BUILD NOW

            Node cur_neigh{node};
            ++cur_neigh[0];
            for (int i=1;i<=4;++i){
                cur_neigh[i] += cur_neigh [i+4];
            }
            cur_neigh[1] -= ore4geode;
            cur_neigh[3] -= obs4geode;
            ++cur_neigh[8];
            neighbours.push_back(cur_neigh);
            return neighbours;      //IF CAN BUILD GEODE, DO ONLY THAT
        }


        //ore
        if (node[5]<maxore){
            //IF DON'T HAVE ENOUGH ROBOTS TO SATISFY DEMAND

            if (node[1]<ore4ore){
                //IF NOT POSSIBLE TO BUILD NOW
                int minute4ore = 1 + std::ceil(static_cast<double>(ore4ore-node[1])/node[5]);
                if (node[0]+minute4ore<=maxMinutes){
                    Node cur_neigh{node};
                    cur_neigh[0] += minute4ore;
                    for (int i=1;i<=4;++i){
                        cur_neigh[i] += cur_neigh [i+4]*minute4ore;
                    }
                    cur_neigh[1] -= ore4ore;
                    ++cur_neigh[5];
                    neighbours.push_back(cur_neigh);
                }
            }else{
                //IF POSSIBLE TO BUILD NOW

                Node cur_neigh{node};
                ++cur_neigh[0];
                for (int i=1;i<=4;++i){
                    cur_neigh[i] += cur_neigh [i+4];
                }
                cur_neigh[1] -= ore4ore;
                ++cur_neigh[5];
                neighbours.push_back(cur_neigh);
            }
        }

        //clay
        if (node[6]<clay4obs){
            //IF DON'T HAVE ENOUGH ROBOTS TO SATISFY DEMAND

            if (node[1]<ore4clay){
                //IF NOT POSSIBLE TO BUILD NOW

                int minute4clay = 1 + std::ceil(static_cast<double>(ore4clay-node[1])/node[5]);
                if (node[0]+minute4clay<=maxMinutes){
                    Node cur_neigh{node};
                    cur_neigh[0] += minute4clay;
                    for (int i=1;i<=4;++i){
                        cur_neigh[i] += cur_neigh [i+4]*minute4clay;
                    }
                    cur_neigh[1] -= ore4clay;
                    ++cur_neigh[6];
                    neighbours.push_back(cur_neigh);
                }
            }else{
                //IF POSSIBLE TO BUILD NOW

                Node cur_neigh{node};
                ++cur_neigh[0];
                for (int i=1;i<=4;++i){
                    cur_neigh[i] += cur_neigh [i+4];
                }
                cur_neigh[1] -= ore4clay;
                ++cur_neigh[6];
                neighbours.push_back(cur_neigh);
            }
        }

        //obsidian
        if (node[7]<obs4geode){
            //IF DON'T HAVE ENOUGH ROBOTS TO SATISFY DEMAND
            
            if (node[6]>0 && (node[1]<ore4obs || node[2]<clay4obs)){
                //IF POSSIBLE TO BUILD IN THE FUTURE AND NOT POSSIBLE TO BUILD NOW

                int minute4obs = 1 + std::max(std::ceil(static_cast<double>(ore4obs-node[1])/node[5]),std::ceil(static_cast<double>(clay4obs-node[2])/node[6]));
                if (node[0]+minute4obs<=maxMinutes){
                    Node cur_neigh{node};
                    cur_neigh[0] += minute4obs;
                    for (int i=1;i<=4;++i){
                        cur_neigh[i] += cur_neigh [i+4]*minute4obs;
                    }
                    cur_neigh[1] -= ore4obs;
                    cur_neigh[2] -= clay4obs;
                    ++cur_neigh[7];
                    neighbours.push_back(cur_neigh);
                }
            }else if (node[1]>=ore4obs && node[2]>=clay4obs){
                //IF POSSIBLE TO BUILD NOW

                Node cur_neigh{node};
                ++cur_neigh[0];
                for (int i=1;i<=4;++i){
                    cur_neigh[i] += cur_neigh [i+4];
                }
                cur_neigh[1] -= ore4obs;
                cur_neigh[2] -= clay4obs;
                ++cur_neigh[7];
                neighbours.push_back(cur_neigh);
            }
        }

    }

    return neighbours;
}


void read_line(std::string& str,Graph& g){
    std::stringstream ss{str};
    std::string dummy{};

    ss>>dummy>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>g.ore4ore;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>g.ore4clay;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>g.ore4obs;

    ss>>dummy>>dummy;
    ss>>g.clay4obs;

    ss>>dummy>>dummy>>dummy>>dummy>>dummy;
    ss>>g.ore4geode;

    ss>>dummy>>dummy;
    ss>>g.obs4geode;

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
        if (!part_one && index>3) break;    //READ ONLY THREE LINES FOR PART 2
        
        Graph g{};
        read_line(str,g);
        g.compute_maxore();
        g.maxMinutes = maxMinutes;

        Node start = {0,0,0,0,0,1,0,0,0};
        g.dfs(start);
        int result = g.best_score;
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



