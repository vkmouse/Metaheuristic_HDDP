#include "__header__.h"
void search_algorithm::init(string dataset_path, int num_iter, int max_evaluation_time, int per_evaluation_time, int now_evaluation_time, int num_run, string init_sol_path)
{
    this->dataset_path = dataset_path;
    this->num_iter = num_iter;
    this->max_evaluation_time = max_evaluation_time;
    this->per_evaluation_time = per_evaluation_time;
    this->now_evaluation_time = now_evaluation_time;
    this->num_run = num_run;
    this->init_sol_path = init_sol_path;
}