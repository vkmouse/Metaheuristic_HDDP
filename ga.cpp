#include "__header__.h"
ga::ga(int num_cms, double Pc, double Pm)
{
    this->num_cms = num_cms;
    this->Pc = Pc;
    this->Pm = Pm;
}
void ga::run()
{
    init();
    for (int now_iter = 0; now_iter < num_iter && now_evaluation_time < max_evaluation_time; now_iter++)
    {
        selection(cms_sol, cms_objectvalue);
        crossover(cms_sol);
        mutation(cms_sol);
        fitness(cms_sol, cms_objectvalue);
    }
}
void ga::init()
{
    cms_sol.resize(num_cms);
    cms_objectvalue.resize(num_cms);

    if (init_sol_path == "")
        best_sol = cms_sol[0] = create_sol();
    else
        best_sol = cms_sol[0] = init_sol[now_run];
    best_objectvalue = cms_objectvalue[0] = evaluation(cms_sol[0]);
    determination(cms_sol[0], cms_objectvalue[0]);

    for (int i = 1; i < num_cms; i++)
    {
        cms_sol[i] = create_sol();
        cms_objectvalue[i] = evaluation(cms_sol[i]);
        determination(cms_sol[i], cms_objectvalue[i]);
    }
}
void ga::selection(v2d<int> &cms_sol, v1d<double> &cms_objectvalue)
{
    v2d<int> copy_cms_sol = cms_sol;
    double total = 0;
    v1d<double> RouletteWheel;

    for (int i = 0; i < cms_objectvalue.size(); i++)
        total += cms_objectvalue[i];

    RouletteWheel.push_back(cms_objectvalue[0] / total);
    for (int i = 1; i < cms_objectvalue.size(); i++)
        RouletteWheel.push_back(cms_objectvalue[i] / total + RouletteWheel.back());

    for (int i = 0; i < cms_sol.size(); i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < RouletteWheel[0])
            cms_sol[i] = copy_cms_sol[0];
        else if (f == 1)
            cms_sol[i] = copy_cms_sol.back();
        else
        {
            for (int j = 1; j < RouletteWheel.size(); j++)
                if (f >= RouletteWheel[j - 1] && f < RouletteWheel[j])
                {
                    cms_sol[i] = copy_cms_sol[j];
                    break;
                }
        }
    }

    // take the best solution into the population
    cms_sol[0] = best_sol;
    cms_objectvalue[0] = best_objectvalue;
}
void ga::crossover(v2d<int> &cms_sol)
{
    v2d<int> copy_cms_sol = cms_sol;
    v1d<int> tmp1_sol, tmp2_sol;
    string method = "two point";

    for (int i = 0; i < cms_sol.size() / 2; i++)
    {
        tmp1_sol = takeout(copy_cms_sol, rand() % copy_cms_sol.size());
        tmp2_sol = takeout(copy_cms_sol, rand() % copy_cms_sol.size());

        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < Pc)
            crossover(tmp1_sol, tmp2_sol, method);
        cms_sol[i * 2] = tmp1_sol;
        cms_sol[i * 2 + 1] = tmp2_sol;
    }
}
void ga::crossover(v1d<int> &sol1, v1d<int> &sol2, string method)
{
    v1d<int> mask(sol1.size());
    v1d<int> tmp_sol = sol1;
    int num_gene = sol1.size();

    if (method == "two point")
    {
        int point1, point2;

        point1 = rand() % num_gene;
        point2 = point1 + rand() % (num_gene / 10);
        if (point2 >= num_gene - 1)
            point2 = num_gene - 1;

        if (point1 > point2)
        {
            int tmp = point1;
            point1 = point2;
            point2 = tmp;
        }

        for (int i = 0; i < mask.size(); i++)
            if (point1 <= i && i < point2)
                mask[i] = 1;
            else
                mask[i] = 0;
    }

    for (int i = 0; i < mask.size(); i++)
        if (mask[i])
        {
            if (sol1[i] < sol2[i])
                sol1[i] = (sol1[i] + 1) % 5;
            else if (sol1[i] > sol2[i])
                sol1[i] = (sol1[i] - 1) % 5;
            if (sol2[i] < tmp_sol[i])
                sol2[i] = (sol2[i] + 1) % 5;
            else if (sol2[i] > tmp_sol[i])
                sol2[i] = (sol2[i] - 1) % 5;
        }
}
void ga::mutation(v2d<int> &cms_sol)
{
    for (int i = 0; i < num_cms; i++)
    {
        double f = static_cast<double>(rand()) / RAND_MAX;
        if (f < Pm)
            cms_sol[i] = transition(cms_sol[i]);
    }
}
void ga::fitness(v2d<int> cms_sol, v1d<double> &cms_objectvalue)
{
    for (int i = 0; i < cms_sol.size(); i++)
    {
        cms_objectvalue[i] = evaluation(cms_sol[i]);
        determination(cms_sol[i], cms_objectvalue[i]);
    }
}
template <class T>
T ga::takeout(vector<T> &vec, int num)
{
    T t = vec[num];
    vec.erase(vec.begin() + num);
    return t;
}