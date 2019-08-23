#include "__header__.h"
se::se(int num_searcher, int num_region, int num_sample, int num_player)
{
    this->num_searcher = num_searcher;
    this->num_region = num_region;
    this->num_sample = num_sample;
    this->num_player = num_player;
}
void se::run()
{
    init();
    for (int now_iter = 0; now_iter < num_iter && now_evaluation_time < max_evaluation_time; now_iter++)
    {
        transition_se(searcher_sol, sample_sol, sampleV_sol, sample_sol_fitness);
        compute_expected_value(sample_sol, sample_sol_best, sampleV_sol,
                               sample_sol_fitness, sample_sol_best_fitness, sampleV_sol_fitness,
                               region_it, region_nit, expected_value);
        vision_selection(searcher_sol, sample_sol, sample_sol_best, searcher_sol_fitness, sample_sol_fitness, sample_sol_best_fitness,
                         region_it, region_nit, expected_value);
        for (int j = 0; j < num_region; j++)
            determination(sample_sol_best[j], sample_sol_best_fitness[j]);
    }
}
void se::init()
{
    // init
    region_it.assign(num_region, 1.0);
    region_nit.assign(num_region, 1.0);

    expected_value.assign(num_searcher, v1d<double>(num_region, 0.0));

    //  resource_arrangement
    searcher_sol.assign(num_searcher, v1d<int>(num_candicate_point, 0));
    sample_sol.assign(num_region, v2d<int>(num_sample, v1d<int>(num_candicate_point, 0)));
    sample_sol_best.assign(num_region, v1d<int>(num_candicate_point, 0));
    sampleV_sol.assign(num_searcher, v3d<int>(num_region, v2d<int>(num_sample * 2, v1d<int>(num_candicate_point, 0))));

    searcher_sol_fitness.assign(num_searcher, 0);
    sample_sol_fitness.assign(num_region, v1d<double>(num_sample, 0));
    sample_sol_best_fitness.assign(num_region, 0);
    sampleV_sol_fitness.assign(num_searcher, v2d<double>(num_region, v1d<double>(num_sample * 2, 0)));
    // searcher
    if (init_sol_path == "")
        best_sol = searcher_sol[0] = create_sol();
    else
        best_sol = searcher_sol[0] = init_sol[now_run];
    match_region(searcher_sol[0], num_region, 0);
    searcher_sol_fitness[0] = evaluation(searcher_sol[0]);
    determination(searcher_sol[0], searcher_sol_fitness[0]);

    for (int i = 1; i < num_searcher; i++)
    {
        searcher_sol[i] = create_sol();
        match_region(searcher_sol[i], num_region, i % num_region);
        searcher_sol_fitness[i] = evaluation(searcher_sol[i]);
        determination(searcher_sol[i], searcher_sol_fitness[i]);
    }
    // sample and sample best
    for (int j = 0; j < num_region; j++)
        for (int k = 0; k < num_sample; k++)
        {
            sample_sol[j][k] = create_sol();
            match_region(sample_sol[j][k], num_region, j);
            sample_sol_fitness[j][k] = evaluation(sample_sol[j][k]);
            determination(sample_sol[j][k], sample_sol_fitness[j][k]);
            if (sample_sol_fitness[j][k] > sample_sol_best_fitness[j])
            {
                sample_sol_best[j] = sample_sol[j][k];
                sample_sol_best_fitness[j] = sample_sol_fitness[j][k];
            }
        }
}
void se::match_region(v1d<int> &sol, int num_region, int region)
{
    int sum = 0;
    for (int i = 0; i < num_candicate_point; i++)
        sum += sol[i];
    while (sum % num_region != region)
    {
        sol = transition_in_region(sol, num_region, region);
        sum = 0;
        for (int i = 0; i < num_candicate_point; i++)
            sum += sol[i];
    }
}
v1d<int> se::transition_in_region(v1d<int> sol, int num_region, int region)
{
    int tmp, cell;
    int num_gene = sol.size();

    tmp = rand() % 5;
    if (tmp == 1)
        cell = macrocell;
    else if (tmp == 2)
        cell = microcell;
    else if (tmp == 3)
        cell = picocell;
    else if (tmp == 4)
        cell = femtocell;
    else
        cell = nullcell;

    tmp = rand() % (num_gene / num_region);
    tmp += num_gene / num_region * region;
    if (tmp >= num_gene)
        tmp = num_gene - 1;
    sol[tmp] = cell;

    return sol;
}
void se::transition_se(v2d<int> searcher_sol, v3d<int> sample_sol, v4d<int> &sampleV_sol, v2d<double> sample_sol_fitness)
{
    string method = "two point";
    // crossover
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
            for (int k = 0; k < num_sample; k++)
            {
                int m = k * 2;
                sampleV_sol[i][j][m] = searcher_sol[i];
                sampleV_sol[i][j][m + 1] = sample_sol[j][k];
                crossover(sampleV_sol[i][j][m], sampleV_sol[i][j][m + 1], method);
            }
    // transition and match region
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
            for (int k = 0; k < 2 * num_sample; k++)
            {
                sampleV_sol[i][j][k] = transition(sampleV_sol[i][j][k]);
                match_region(sampleV_sol[i][j][k], num_region, j);
            }
}
void se::crossover(v1d<int> &sol1, v1d<int> &sol2, string method)
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
void se::compute_expected_value(v3d<int> &sample_sol, v2d<int> &sample_sol_best, v4d<int> &sampleV_sol,
                                v2d<double> &sample_sol_fitness, v1d<double> &sample_sol_best_fitness, v3d<double> &sampleV_sol_fitness,
                                v1d<double> region_it, v1d<double> region_nit, v2d<double> &expected_value)
{
    v1d<double> T_j(num_region, 0.0);
    v2d<double> V_ij(num_searcher, v1d<double>(num_region, 0.0));
    v1d<double> M_j(num_region, 0.0);

    // fitness value of sampleV, update sampleV to sample, V_ij
    for (int i = 0; i < num_searcher; i++)
        for (int j = 0; j < num_region; j++)
        {
            V_ij[i][j] = 0.0;
            for (int k = 0; k < num_sample; k++)
            {
                int m = k * 2;

                // fitness value of sampleV
                sampleV_sol_fitness[i][j][m] = evaluation(sampleV_sol[i][j][m]);
                sampleV_sol_fitness[i][j][m + 1] = evaluation(sampleV_sol[i][j][m + 1]);

                // update sampleV to sample
                if (sampleV_sol_fitness[i][j][m] > sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m];
                }
                if (sampleV_sol_fitness[i][j][m + 1] > sample_sol_fitness[j][k])
                {
                    sample_sol[j][k] = sampleV_sol[i][j][m + 1];
                    sample_sol_fitness[j][k] = sampleV_sol_fitness[i][j][m + 1];
                }

                // V_ij
                V_ij[i][j] += sampleV_sol_fitness[i][j][m] + sampleV_sol_fitness[i][j][m + 1];
            }
            V_ij[i][j] /= 2 * num_sample;
        }
    // update sample best fitness, compute all sample fitness
    double all_sample_fitness = 0.0;
    for (int j = 0; j < num_region; j++)
    {
        double rbj = sample_sol_best_fitness[j];
        int index = -1;

        for (int k = 0; k < num_sample; k++)
        {
            all_sample_fitness += sample_sol_fitness[j][k];
            // update fbj
            if (sample_sol_fitness[j][k] > rbj)
            {
                index = k;
                rbj = sample_sol_fitness[j][k];
            }
        }

        if (index >= 0)
        {
            sample_sol_best_fitness[j] = rbj;
            sample_sol_best[j] = sample_sol[j][index];
        }
    }

    // M_j
    for (int j = 0; j < num_region; j++)
        M_j[j] = sample_sol_best_fitness[j] / all_sample_fitness;

    // T_j
    for (int j = 0; j < num_region; j++)
        T_j[j] = region_nit[j] / region_it[j];

    // normalization
    M_j = normalization(M_j);
    T_j = normalization(T_j);
    for (int i = 0; i < num_searcher; i++)
        V_ij[i] = normalization(V_ij[i]);

    // compute the expected_value
    for (int i = 0; i < num_searcher; i++)
    {
        for (int j = 0; j < num_region; j++)
        {
            expected_value[i][j] = T_j[j] * V_ij[i][j] * M_j[j];
        }
    }
}
void se::vision_selection(v2d<int> &searcher_sol, v3d<int> &sample_sol, v2d<int> sample_sol_best, v1d<double> &searcher_sol_fitness, v2d<double> &sample_sol_fitness, v1d<double> sample_sol_best_fitness,
                          v1d<double> &region_it, v1d<double> &region_nit, v2d<double> expected_value)
{
    for (int j = 0; j < num_region; j++)
        region_nit[j]++;

    for (int i = 0; i < num_searcher; i++)
    {
        int player_index = rand() % num_region;
        double player_ev = expected_value[i][player_index];

        for (int j = 0; j < num_player - 1; j++)
        {
            int player1_index = rand() % num_region;
            if (expected_value[i][player1_index] > player_ev)
            {
                player_index = player1_index;
                player_ev = expected_value[i][player1_index];
            }
        }

        // update searcher_sol
        if (sample_sol_best_fitness[player_index] > searcher_sol_fitness[i])
        {
            searcher_sol[i] = sample_sol_best[player_index];
            searcher_sol_fitness[i] = sample_sol_best_fitness[player_index];
        }

        // update region_it[i] and region_hl[i];
        region_it[player_index]++;
        region_nit[player_index] = 1;
        //        cout<<"searcher = "<<i<<", "<<"choose region "<<player_index<<endl;
    }
}
v1d<double> se::normalization(v1d<double> array)
{
    double min = array[0], max = array[0];
    for (int i = 1; i < array.size(); i++)
    {
        if (min > array[i])
            min = array[i];
        if (array[i] > max)
            max = array[i];
    }
    for (int i = 0; i < array.size(); i++)
        array[i] = (array[i] - min) / (max - min);
    return array;
}