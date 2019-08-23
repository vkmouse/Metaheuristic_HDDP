#include "__header__.h"
sa::sa(int neighbor, double max_temperature, double min_temperature, double decrease_rate)
{
    this->neighbor = neighbor;
    this->max_temperature = max_temperature;
    this->min_temperature = min_temperature;
    this->decrease_rate = decrease_rate;
}
void sa::run()
{
    init();
    for (int now_iter = 0; now_iter < num_iter && now_evaluation_time < max_evaluation_time; now_iter++)
    {
        create_neighbor(now_sol, now_objectvalue);
        determination(now_sol, now_objectvalue, now_temperature);
        if (now_temperature > min_temperature)
            now_temperature *= decrease_rate;
    }
}
void sa::init()
{
    if (init_sol_path == "")
        best_sol = now_sol = create_sol();
    else
        best_sol = now_sol = init_sol[now_run];
    best_objectvalue = now_objectvalue = evaluation(now_sol);
    determination(now_sol, now_objectvalue, 0.0);
}
void sa::create_neighbor(v1d<int> &sol, double &objectvalue)
{
    v1d<int> best_sol, tmp_sol;
    double best_objectvalue, tmp_objectvalue;

    best_sol = transition(sol);
    best_objectvalue = evaluation(best_sol);

    for (int i = 0; i < neighbor - 1; i++)
    {
        tmp_sol = transition(sol);
        tmp_objectvalue = evaluation(tmp_sol);

        if (tmp_objectvalue > best_objectvalue)
        {
            best_sol = tmp_sol;
            best_objectvalue = tmp_objectvalue;
        }
    }

    sol = best_sol;
    objectvalue = best_objectvalue;
}
bool sa::determination(v1d<int> &sol, double &objectvalue, double temperature)
{
    double r = 0.0 + rand() / static_cast<double>(RAND_MAX);
    double p = exp(double(objectvalue - best_objectvalue) / temperature);

    if (objectvalue > best_objectvalue)
    {
        best_sol = sol;
        best_objectvalue = objectvalue;
        return true;
    }
    else if (r >= p)
    {
        sol = best_sol;
        objectvalue = best_objectvalue;
        return false;
    }
    return false;
}