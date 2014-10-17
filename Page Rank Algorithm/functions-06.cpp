/* 
 * CSE 232   Project #6
 * File:   functions-06.cpp
 * Author: Cameron Gibson
 *      MONTE CARLO PAGE RANK ALGORITHM:
 *          This file contains seven functions.
 *          This file is made to work with files of a certain format. (such as
 *              the formats found in simple_nodes.txt and simple_arcs.txt)
 *          Please read the comments before each function for more information.
 * Created on October 14, 2014, 12:04 PM
 */

#include<map> 
#include<vector>
#include<string>
#include<random> 
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>

using std::map;
using std::pair;
using std::vector;
using std::default_random_engine; using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::string;
using std::cout; using std::endl;
using std::ostream;
using std::ifstream; using std::ofstream;
using std::istringstream; using std::ostringstream;
using std::find;

/*
 * This function reads the file_name as an index file and updates index_map.
 * RETURNS: boolean (false if file cannot be opened, true otherwise)
 */
bool read_index(map<long, string> &index_map, string file_name){
    //initialize variables
    ifstream input_file;
    string line, web_name;
    long web_id;
    bool return_val = false;
    //try to open file_name
    input_file.open(file_name);
    if(input_file.is_open()){
        return_val = true;
        //place the contents of each line into a pair, then insert to index_map
        while (getline(input_file, line)){
            istringstream istr(line);
            istr>>web_id>>web_name;
            /*
             * FOR example_index.txt:
             * istr>>web_name>>web_id;
             */ 
            pair<long, string> website = {web_id, web_name};
            index_map.insert(website);
        }
        //close
        input_file.close();
    }
    return return_val;
}

/*
 * This function reads the file_name as an arc file and updates arc_map.
 * RETURNS: boolean (false if file cannot be opened, true otherwise)
 */
bool read_arc(map<long, vector<long>> &arc_map, string file_name){
    //initialize variables
    vector<long> arcs;
    string line;
    long start_id, end_id;
    bool return_val=false;
    ifstream input_file;
    //try to open file_name
    input_file.open(file_name);
    if (input_file.is_open()){
        return_val=true;
        //use contents of each line to update the vector<long> for each id
        //in arc_map (if the key doesn't exist then create it)
        while (getline(input_file, line)){
            istringstream istr(line);
            istr>>start_id>>end_id;
            arc_map[end_id].push_back(start_id);
        }
        //close
        input_file.close();
    }
    return return_val;
}

/*
 * This function initializes the Page Rank map (pr_map), which assigns each
 * id a value of 1/n (n is the total # of nodes)
 * RETURN: void
 */
void init_pr_map(map<long,double> &pr_map, size_t sz){
    double rank = 0.0;
    //for each node (website) set the rank equal to 1/n
    for (int i=0;i<sz;++i){
        rank = (1.0/sz);
        pr_map.insert({i,rank});
    }
}

/*
 * This function take in an arc_map and a node id
 * RETURNS: a VECTOR of node ids, which have incoming arcs to the given node
 */
vector<long> incoming (map<long, vector<long>> &arc_map, long id){
    vector<long> output={};
    //output the vector in arc_map for key=id (if id is in arc_map)
    for(auto element:arc_map){
        if(element.first==id)
            output = element.second;
    }
    return output;
}

/*
 * This function takes in arc_map and a node id
 * RETURNS: the number of outgoing arcs from that node (of type LONG)
 */
long outgoing (map<long, vector<long>> &arc_map, long id){
    long count=0;
    //for each node (website) in arc_map
    for(auto element:arc_map){
        //find if the element is in any of the vectors for any other node id
        auto found = find(element.second.begin(),element.second.end(),id);
        if(found!=element.second.end()){
            //if it is, then count it has having an outgoing arc
            ++count;
        }
    }
    return count;
}

/*
 * This function does "iters" number of iterations and updates a random node
 * with each iteration. (Monte Carlo Page Rank Algorithm)
 * RETURN: void
 */
void page_rank(map<long, vector<long>> &arc_map, 
        map<long,double> & pr_map,
        long iters, double damping,
        default_random_engine &dre){
    //initialize variables
    vector<long> in_vec={};
    double page_rank;
    double sum =0;
    //create distribution from 0-n, where n is the number of nodes (websites)
    auto sz_map = pr_map.size();
    uniform_int_distribution<long> dist(0,sz_map);
    //for the number of iterations given
    while(iters>0){
        sum=0;
        //update a random id (use the engine given and the created distribution)
        long id = dist(dre);
        //find the incoming vectors to this random id
        in_vec = incoming(arc_map,id);
        //check if the node is not a source or a sink
        if(!in_vec.empty() && outgoing(arc_map,id)){
            //for each node in the vector add up the sum for the algorithm
            for(auto node:in_vec){
                sum+=(pr_map.at(node)/outgoing(arc_map, node));
            }
            //finish the calculation of the algorithm
            sum*=damping;
            page_rank = sum+((1.0-damping)/sz_map);
            //update the page rank of this random id
            pr_map.at(id)=page_rank;
        }
        //subtract one from the number of iterations
        --iters;
    }
}

/*
 * This function creates a outstringstream which can be used to display all of
 * the page ranks for each node (website)
 * RETURN: void
 */
void print_page_ranks(map<long, double> &pr_map, 
        map<long, string> &index_map,
        ostream &out){
    //for each element in the pr_map print the web_name from index_map and the
    //page rank from the pr_map
    for(auto element:pr_map){
        out<<"NAME: "<<index_map.at(element.first)<<" \tRANK:"<<element.second<<endl;
    }
}
