#include "__header__.h"

int main(int argc, char **argv)
{
    string algorithm = argv[1];
    search_algorithm *search_alg;

    if (algorithm == "hc")
        search_alg = new hc();
    else if (algorithm == "sa")
        search_alg = new sa(
            atoi(argv[8]),  // neighbor
            atof(argv[9]),  // max_temperature
            atof(argv[10]), // min_temperature
            atof(argv[11])  // decrease_rate
        );
    else if (algorithm == "ga")
        search_alg = new ga(
            atoi(argv[8]), // num_cms
            atof(argv[9]), // Pc
            atof(argv[10]) // Pm
        );
    else if (algorithm == "se")
        search_alg = new se(
            atoi(argv[8]),  // num_searcher
            atoi(argv[9]),  // num_region
            atoi(argv[10]), // num_sample
            atoi(argv[11])  // num_player
        );

    search_alg->init(
        argv[2],       //dataset_path
        atoi(argv[3]), // num_iter
        atoi(argv[4]), // max_evaluation_time
        atoi(argv[5]), // per_evaluation_time
        atoi(argv[6]), // num_run
        argv[7]);      // init_sol_path

    search_alg->run_all();
}
