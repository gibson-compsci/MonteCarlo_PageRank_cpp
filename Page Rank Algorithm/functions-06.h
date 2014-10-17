#ifndef PAGE_RANK
#define PAGE_RANK

#include<map>
using std::map; 
#include<vector>
using std::vector;
#include<string>
using std::string; 
#include<random>
using std::default_random_engine; 
#include<iostream>
using std::ostream;

bool read_index(map<long, string> &index_map, string file_name);
bool read_arc(map<long, vector<long>> &arc_map, string file_name);
void init_pr_map(map<long,double> &pr_map, size_t sz);
vector<long> incoming (map<long, vector<long>> &arc_map, long id);
long outgoing (map<long, vector<long>> &arc_map, long id);
void page_rank(
    map<long, vector<long>> &arc_map, 
    map<long,double> & pr_map,
    long iters, double damping,
    default_random_engine &dre);
void print_page_ranks(map<long, double> &pr_map, map<long, string> &index_map, ostream &out);


#endif
