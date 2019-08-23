#ifndef _HEADER_H_
#define _HEADER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <sstream>
#include <cmath>

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;
using std::stringstream;
using std::vector;

#define macrocell 1
#define microcell 2
#define picocell 3
#define femtocell 4
#define nullcell 5

template <typename T>
using v1d = vector<T>;
template <typename T>
using v2d = vector<vector<T>>;
template <typename T>
using v3d = vector<vector<vector<T>>>;
template <typename T>
using v4d = vector<vector<vector<vector<T>>>>;

class search_algorithm
{
public:
    // Parameter for every algorithm
    string dataset_path;
    int num_iter = INT_MAX;
    int max_evaluation_time = INT_MAX;
    int per_evaluation_time = 1000;
    int now_evaluation_time = 0;
    int num_run = 1;
    int now_run;
    string init_sol_path;

    v1d<int> best_sol;
    double best_objectvalue;
    double best_sol_cost;
    double best_sol_interference;
    double best_sol_coverage;

    v1d<double> cell_cost{0, 175, 25, 5, 1, 0};
    double max_cost;
    v1d<double> cell_range{0, 25, 10, 0.5, 0.05, 0};
    double max_range;
    v1d<double> cell_power{0, 50, 10, 1, 0.25, 0};
    double max_interference;

    // Dataset Infomation
    double region_length;
    double region_width;
    int num_candicate_point;
    int num_user;
    int num_original_macrocell;

    // Dataset
    v2d<double> candicate_point; // [num_candicate_point][2]
    v2d<double> user;            // [num_user][2]
    v1d<int> original_macrocell;
    v2d<int> init_sol;

    // method
    virtual void run() = 0;
    void run_all();
    void init(string dataset_path, int num_iter, int max_evaluation_time, int per_evaluation_time, int num_run, string init_sol_path);
    v1d<int> create_sol();
    v1d<int> transition(v1d<int> sol);
    double evaluation(v1d<int> &sol);
    bool determination(v1d<int> sol, double objectvalue);

private:
    void read_dataset();
    void read_init_sol();
    bool isInt(string str);
    bool is_covered(int cell, double *x1, double *y1, double *x2, double *y2);
    v1d<string> split(string str, string pattern);
    double evaluation_cost(v1d<int> sol);
    double evaluation_coverage(v1d<int> &sol);
    double evaluation_interference(v1d<int> sol);
    // v1d<double> normalization(v1d<double> array);
};
class hc : public search_algorithm
{
public:
    virtual void run();

private:
    v1d<int> now_sol;
    double now_objectvalue;
    void init();
};
class sa : public search_algorithm
{
public:
    sa(int neighbor, double max_temperature, double min_temperature, double decrease_rate);
    virtual void run();

private:
    v1d<int> now_sol;
    double now_objectvalue;
    double now_temperature;
    int neighbor;
    double max_temperature;
    double min_temperature;
    double decrease_rate;
    void init();
    void create_neighbor(v1d<int> &sol, double &objectvalue);
    bool determination(v1d<int> &sol, double &objectvalue, double temperature);
};
class ga : public search_algorithm
{
public:
    ga(int num_cms, double Pc, double Pm);
    virtual void run();

private:
    int num_cms;
    double Pc;
    double Pm;
    v2d<int> cms_sol;
    v1d<double> cms_objectvalue;
    void init();
    void selection(v2d<int> &cms_sol, v1d<double> &cms_objectvalue);
    void crossover(v2d<int> &cms_sol);
    void crossover(v1d<int> &sol1, v1d<int> &sol2, string method);
    void mutation(v2d<int> &cms_sol);
    void fitness(v2d<int> cms_sol, v1d<double> &cms_objectvalue);
    template <class T>
    T takeout(vector<T> &vec, int num);
};
class se : public search_algorithm
{
public:
    se(int num_searcher, int num_region, int num_sample, int num_player);
    virtual void run();

private:
    int num_searcher;
    int num_region;
    int num_sample;
    int num_player;
    v2d<int> searcher_sol; // [searcher, num_bit_sol]
    v1d<double> searcher_sol_fitness;
    v3d<int> sample_sol;      // [region, sample, num_bit_sol]
    v2d<int> sample_sol_best; // [region, num_bit_sol]
    v4d<int> sampleV_sol;     // [searcher, region, sample, num_bit_sol]
    v2d<double> sample_sol_fitness;
    v1d<double> sample_sol_best_fitness;
    v3d<double> sampleV_sol_fitness;
    v1d<double> region_it;  // region invest time, 區域總共被調查的次數
    v1d<double> region_nit; // region not invest time, 目前區域未調查的連續次數
    v2d<double> expected_value;
    void init();
    void match_region(v1d<int> &sol, int num_region, int region);
    v1d<int> transition_in_region(v1d<int> sol, int num_region, int region);
    void transition_se(v2d<int> searcher_sol, v3d<int> sample_sol, v4d<int> &sampleV_sol, v2d<double> sample_sol_fitness);
    void crossover(v1d<int> &sol1, v1d<int> &sol2, string method);
    void compute_expected_value(v3d<int> &sample_sol, v2d<int> &sample_sol_best, v4d<int> &sampleV_sol,
                                v2d<double> &sample_sol_fitness, v1d<double> &sample_sol_best_fitness, v3d<double> &sampleV_sol_fitness,
                                v1d<double> region_it, v1d<double> region_nit, v2d<double> &expected_value);
    void vision_selection(v2d<int> &searcher_sol, v3d<int> &sample_sol, v2d<int> sample_sol_best, v1d<double> &searcher_sol_fitness, v2d<double> &sample_sol_fitness, v1d<double> sample_sol_best_fitness,
                          v1d<double> &region_it, v1d<double> &region_nit, v2d<double> expected_value);
    v1d<double> normalization(v1d<double> array);
};
#endif