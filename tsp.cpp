#include <map>
#include <vector>
#include <map>
#include <unordered_map>
#include <tuple>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    string filename = argv[1];
    ifstream infile(filename.c_str(), ifstream::in);
    vector<tuple<string, string, int> > graph;
    string a, b; double c;
    unordered_map<string, int> ids;
    map<int, string> names;
    while (infile >> a >> b >> c) {
        if (ids.find(a) == ids.end()) {
            int id = ids.size();
            ids[a] = id;
            names[id] = a;
        }
        if (ids.find(b) == ids.end()) {
            int id = ids.size();
            ids[b] = id;
            names[id] = b;
        }
        graph.push_back(make_tuple(a, b, c));
    }
    int nc = ids.size();
    unordered_map<int, map<double, vector<int> > > edges_and_weights;
    auto rebuild_weights = [&](void){
        edges_and_weights.clear();
        for (auto& e : graph) {
            edges_and_weights[ids[get<0>(e)]][get<2>(e)].push_back(ids[get<1>(e)]);
        }
    };
    vector<pair<double, vector<int> > > tours(ids.size());
    for (auto& i : names) {
        int id = i.first;
        cerr << "on " << id << " " << names[id] << "(" << id << "/" << ids.size() << ")" << endl;
        rebuild_weights();
        auto& weight = tours[id].first;
        auto& tour = tours[id].second;
        tour.push_back(id);
        // follow the best edge (assuming high weights are better)
        while (true) {
            auto& e = edges_and_weights[id];
            if (e.empty()) break;
            auto nexts = e.rbegin();
            id = nexts->second.back();
            tour.push_back(id);
            weight += nexts->first;
            nexts->second.pop_back();
            if (nexts->second.empty()) {
                e.erase(nexts->first);
            }
        }
    }
    std::sort(tours.begin(), tours.end(), [&](const pair<double, vector<int> >& a,
                                              const pair<double, vector<int> >& b) {
                  return a.first > b.first;
              });
    for (auto& tour : tours) {
        cout << tour.first << "\t";
        for (auto& id : tour.second) {
            cout << names[id] << ",";
        }
        cout << endl;
    }
    return 0;
}
