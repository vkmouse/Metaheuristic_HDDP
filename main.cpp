#include "__header__.h"

int main(int argc, char **argv)
{
    string algorithm = argv[1];
    search_algorithm *search_alg;

    if (algorithm == "hc")
        search_alg = new hc();

    search_alg->init(
        argv[2],       //dataset_path
        atoi(argv[3]), // num_iter
        atoi(argv[4]), // max_evaluation_time
        atoi(argv[5]), // per_evaluation_time
        atoi(argv[6]), // num_run
        argv[7]);      // init_sol_path

    search_alg->run_all();
}
