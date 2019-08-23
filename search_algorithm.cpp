#include "__header__.h"
void search_algorithm::run_all()
{
    for (now_run = 0; now_run < num_run; now_run++)
    {
        now_evaluation_time = 0;
        run();
    }
}
void search_algorithm::init(string dataset_path, int num_iter, int max_evaluation_time, int per_evaluation_time, int num_run, string init_sol_path)
{
    this->dataset_path = dataset_path;
    this->num_iter = num_iter;
    this->max_evaluation_time = max_evaluation_time;
    this->per_evaluation_time = per_evaluation_time;
    this->num_run = num_run;
    this->init_sol_path = init_sol_path;

    read_dataset();
    read_init_sol();
    max_cost = cell_cost[macrocell] * num_candicate_point;
    max_interference = evaluation_interference(v1d<int>(num_candicate_point, macrocell));
}
v1d<int> search_algorithm::create_sol()
{
    v1d<int> sol;
    for (int i = 0; i < num_candicate_point; i++)
        sol.push_back((int)(rand() % 5) + 1); //產生1~5
    for (int i = 0; i < num_original_macrocell; i++)
        sol[original_macrocell[i]] = 1; //原設定的macrocell
    return sol;
}
v1d<int> search_algorithm::transition(v1d<int> sol)
{
    int tmp, cell;

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

    tmp = rand() % num_candicate_point;
    sol[tmp] = cell;

    return sol;
}
double search_algorithm::evaluation(v1d<int> &sol)
{
    double objectvalue;
    double coverage;
    double cost;
    double interference;

    coverage = evaluation_coverage(sol);
    cost = evaluation_cost(sol);
    interference = evaluation_interference(sol);
    objectvalue = coverage / num_user + (max_cost - cost) / max_cost + (max_interference - interference) / max_interference;

    now_evaluation_time++;
    if (now_evaluation_time % per_evaluation_time == 0 && now_evaluation_time != 0)
    {
        cout << endl;
        for (int i = 0; i < num_candicate_point; i++)
            cout << best_sol[i];
        cout << now_evaluation_time << ",";
        cout << best_objectvalue << endl;
        //		cout<<best_sol_coverage<<"\t";
        //		cout<<best_sol_cost<<"\t";
        //		cout<<best_sol_interference<<"\t";
        //		cout<<best_sol_coverage/num_user<<"\t";
        //		cout<<(max_cost-best_sol_cost)/max_cost<<"\t";
        //		cout<<(max_interference-best_sol_interference)/max_interference<<endl;
    }
    else if (now_evaluation_time == 1)
    {
        cout << now_evaluation_time << ",";
        cout << objectvalue << endl;
        //		cout<<coverage<<"\t";
        //		cout<<cost<<"\t";
        //		cout<<interference<<"\t";
        //		cout<<coverage/num_user<<"\t";
        //		cout<<(max_cost-cost)/max_cost<<"\t";
        //		cout<<(max_interference-interference)/max_interference<<endl;
    }
    //	cout<<now_evaluation_time<<",";
    //	cout<<objectvalue<<endl;

    return objectvalue;
}
bool search_algorithm::determination(v1d<int> sol, double objectvalue)
{
    if (objectvalue > best_objectvalue)
    {
        best_sol = sol;
        best_objectvalue = objectvalue;
        best_sol_cost = evaluation_cost(best_sol);
        best_sol_interference = evaluation_interference(best_sol);
        best_sol_coverage = evaluation_coverage(best_sol);
        return true;
    }
    return false;
}
void search_algorithm::read_dataset()
{
    ifstream file;
    file.open(dataset_path.c_str());
    string line;
    stringstream ss;
    double dtmp;

    while (getline(file, line))
    {
        if (line == "Start the candicate point")
            while (getline(file, line) && line != "End the candicate point")
            {
                v1d<double> d1dtmp;
                ss.clear();
                ss << line;
                ss >> dtmp; //點的號碼，不需要的資訊
                while (!ss.eof())
                {
                    ss >> dtmp;
                    d1dtmp.push_back(dtmp);
                }
                candicate_point.push_back(d1dtmp);
            }
        else if (line == "Start the user")
            while (getline(file, line) && line != "End the user")
            {
                v1d<double> d1dtmp;
                ss.clear();
                ss << line;
                ss >> dtmp; //點的號碼，不需要的資訊
                while (!ss.eof())
                {
                    ss >> dtmp;
                    d1dtmp.push_back(dtmp);
                }
                user.push_back(d1dtmp);
            }
        else if (line == "Start the original macrocell")
            while (getline(file, line) && line != "End the original macrocell")
            {
                ss.clear();
                ss << line;
                ss >> dtmp; //點的號碼，不需要的資訊
                ss >> dtmp;
                original_macrocell.push_back(dtmp);
            }
        else
            for (int k = 0; k < 30; k++)
                if (line.substr(0, k) == "region_length = ")
                {
                    if (isInt(line.substr(k, line.size())))
                    {
                        ss.clear();
                        ss << line.substr(k, line.size());
                        ss >> region_length;
                    }
                }
                else if (line.substr(0, k) == "region_width = ")
                {
                    if (isInt(line.substr(k, line.size())))
                    {
                        ss.clear();
                        ss << line.substr(k, line.size());
                        ss >> region_width;
                    }
                }
                else if (line.substr(0, k) == "num_candicate_point = ")
                {
                    if (isInt(line.substr(k, line.size())))
                    {
                        ss.clear();
                        ss << line.substr(k, line.size());
                        ss >> num_candicate_point;
                    }
                }
                else if (line.substr(0, k) == "num_user = ")
                {
                    if (isInt(line.substr(k, line.size())))
                    {
                        ss.clear();
                        ss << line.substr(k, line.size());
                        ss >> num_user;
                    }
                }
                else if (line.substr(0, k) == "num_original_macrocell = ")
                {
                    if (isInt(line.substr(k, line.size())))
                    {
                        ss.clear();
                        ss << line.substr(k, line.size());
                        ss >> num_original_macrocell;
                    }
                }
    }
}
void search_algorithm::read_init_sol()
{
    v2d<int> i2dtmp;
    ifstream fin;
    fin.open(init_sol_path.c_str());
    string line;
    for (int i = 0; i < num_run; i++)
    {
        getline(fin, line);
        v1d<string> s1dtmp = split(line, ",");
        i2dtmp.push_back(v1d<int>(0, 0));
        for (int j = 0; j < s1dtmp.size(); j++)
            i2dtmp[i].push_back(atoi(s1dtmp.at(j).c_str()));
    }
    init_sol = i2dtmp;
}
bool search_algorithm::isInt(string str)
{
    const char *c = str.c_str();
    for (int i = 0; i < str.size(); i++)
        if ((int)c[i] < (int)'0' || (int)c[i] > (int)'9')
            return false;
    return true;
}
bool search_algorithm::is_covered(int cell, double *x1, double *y1, double *x2, double *y2)
{
    double dist = pow(&x1 - &x2, 2) + pow(&y1 - &y2, 2);
    if (dist < pow(cell_range[cell], 2))
        return true;
    return false;
}
v1d<string> search_algorithm::split(string str, string pattern)
{
    int pos;
    v1d<string> result;
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
double search_algorithm::evaluation_cost(v1d<int> sol)
{
    double objectvalue = 0;
    double max_cost = 0;
    for (int i = 0; i < num_candicate_point; i++)
        if (sol[i] == macrocell)
            objectvalue += cell_cost[macrocell];
        else if (sol[i] == microcell)
            objectvalue += cell_cost[microcell];
        else if (sol[i] == picocell)
            objectvalue += cell_cost[picocell];
        else if (sol[i] == femtocell)
            objectvalue += cell_cost[femtocell];

    for (int i = 0; i < num_original_macrocell; i++)
        if (sol[original_macrocell[i]] == macrocell)
            objectvalue -= cell_cost[macrocell];

    return objectvalue;
}
double search_algorithm::evaluation_coverage(v1d<int> &sol)
{
    //macro and null不需要被覆蓋, 若cell未被較大cell覆蓋,設定為 macro or null
    double objectvalue = 0;
    v1d<bool> candicate_point_covered(num_candicate_point, false);
    int num_user_covered = 0;

    for (int i = 0; i < num_candicate_point; i++)
    {
        if (sol[i] == macrocell || sol[i] == nullcell)
        {
            candicate_point_covered[i] = true;
        }
        else if (sol[i] == microcell)
        {
            for (int j = 0; j < num_candicate_point; j++)
                if (sol[j] == macrocell)
                {
                    candicate_point_covered[i] = is_covered(sol[j], &candicate_point[i][0], &candicate_point[i][1], &candicate_point[j][0], &candicate_point[j][1]);
                    if (candicate_point_covered[i])
                        break;
                }
        }
        else if (sol[i] == picocell)
        {
            for (int j = 0; j < num_candicate_point; j++)
                if (sol[j] == macrocell || sol[j] == microcell)
                {
                    candicate_point_covered[i] = is_covered(sol[j], &candicate_point[i][0], &candicate_point[i][1], &candicate_point[j][0], &candicate_point[j][1]);
                    if (candicate_point_covered[i])
                        break;
                }
        }
        else if (sol[i] == femtocell)
        {
            for (int j = 0; j < num_candicate_point; j++)
                if (sol[j] == macrocell || sol[j] == microcell || sol[j] == picocell)
                {
                    candicate_point_covered[i] = is_covered(sol[j], &candicate_point[i][0], &candicate_point[i][1], &candicate_point[j][0], &candicate_point[j][1]);
                    if (candicate_point_covered[i])
                        break;
                }
        }
        if (candicate_point_covered[i] == false)
        {
            sol[i] = nullcell;
            candicate_point_covered[i] = true;
        }
    }

    for (int i = 0; i < num_user; i++)
        for (int j = 0; j < num_candicate_point; j++)
        {
            if (sol[j] != nullcell && is_covered(sol[j], &user[i][0], &user[i][1], &candicate_point[j][0], &candicate_point[j][1]))
            {
                double dist = pow(user[i][0] - candicate_point[j][0], 2) + pow(user[i][1] - candicate_point[j][1], 2);
                if (dist < pow(cell_range[sol[j]], 2))
                {
                    num_user_covered++;
                    break;
                }
            }
        }

    return num_user_covered;
}
double search_algorithm::evaluation_interference(v1d<int> sol)
{
    double objectvalue = 0;
    for (int i = 0; i < num_user; i++)
    {
        double total = 0;
        double max_signal = 0;
        int index;
        for (int j = 0; j < num_candicate_point; j++)
        {
            if (sol[j] != nullcell)
            {
                double dist = pow(user[i][0] - candicate_point[j][0], 2) + pow(user[i][1] - candicate_point[j][1], 2);
                if (dist < pow(cell_range[sol[j]], 2))
                {
                    double signal = cell_power[sol[j]] / dist;
                    total += signal;
                    if (signal > max_signal)
                    {
                        max_signal = signal;
                        index = j;
                    }
                }
            }
        }
        objectvalue += total - max_signal;
    }
    return objectvalue;
}
