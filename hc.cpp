#include "__header__.h"
void hc::run()
{
    init();
    for (int now_iter = 0; now_iter < num_iter && now_evaluation_time < max_evaluation_time; now_iter++)
    {
        now_sol = transition(best_sol);
        now_objectvalue = evaluation(now_sol);
        determination(now_sol, now_objectvalue);
    }
}
void hc::init()
{
    if (init_sol_path == "")
        best_sol = now_sol = create_sol();
    else
        best_sol = now_sol = init_sol[now_run];
    best_objectvalue = now_objectvalue = evaluation(now_sol);
    determination(now_sol, now_objectvalue);
}
