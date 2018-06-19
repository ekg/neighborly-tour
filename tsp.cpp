#include <map>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    string filename = argv[1];
    ifstream infile(filename.c_str(), ifstream::in);
    vector<tuple<string, string, int64_t> > graph;
    string a, b; double c;
    unordered_map<string, int64_t> ids;
    map<int64_t, string> names;
    while (infile >> a >> b >> c) {
        if (ids.find(a) == ids.end()) {
            int64_t id = ids.size();
            ids[a] = id;
            names[id] = a;
        }
        if (ids.find(b) == ids.end()) {
            int64_t id = ids.size();
            ids[b] = id;
            names[id] = b;
        }
        graph.push_back(make_tuple(a, b, c));
    }
    int64_t nc = ids.size();
    unordered_map<int64_t, map<double, vector<int64_t> > > edges_and_weights;
    int top_n = 0; // try 16
    auto rebuild_weights = [&](void){
        edges_and_weights.clear();
        for (auto& e : graph) {
            edges_and_weights[ids[get<0>(e)]][get<2>(e)].push_back(ids[get<1>(e)]);
        }
        if (top_n) {
            for (auto& w : edges_and_weights) {
                while (w.second.size() > top_n) {
                    w.second.erase(w.second.begin());
                }
            }
        }
    };
    vector<pair<double, vector<int64_t> > > tours(ids.size());
    unordered_set<int64_t> seen;
    auto clean_seen = [&](map<double, vector<int64_t> >& e_and_w) {
        vector<double> to_remove;
        for (auto& p : e_and_w) {
            p.second.erase(
                std::remove_if(
                    p.second.begin(),
                    p.second.end(),
                    [&seen](int64_t id) {
                        return seen.count(id);
                    }), p.second.end());
            if (p.second.empty()) to_remove.push_back(p.first);
        }
        for (auto& r : to_remove) {
            e_and_w.erase(r);
        }
    };
    for (auto& i : names) {
        int64_t id = i.first;
        cerr << "on " << id << " " << names[id] << "(" << id << "/" << ids.size() << ")" << endl;
        seen.clear();
        rebuild_weights();
        auto& weight = tours[id].first;
        auto& tour = tours[id].second;
        tour.push_back(id);
        // follow the best edge (assuming high weights are better)
        while (true) {
            seen.insert(id);
            auto& e = edges_and_weights[id];
            clean_seen(e);
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
    std::sort(tours.begin(), tours.end(), [&](const pair<double, vector<int64_t> >& a,
                                              const pair<double, vector<int64_t> >& b) {
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
