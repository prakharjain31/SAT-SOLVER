#include <bits/stdc++.h>

using namespace std;


int sgn(int x)
{
    if(x > 0)
    {
        return 1;
    }
    else if(x < 0)
    {
        return -1;
    }
    return 0;
}


int check(vector<vector<int> > clauses, vector<int> model)
{
    int count = 0;
    for(auto clause : clauses)
    {
        for(auto literal : clause)
        {
            if(literal > 0)
            {
                if(model[literal] == 1)
                {
                    count++;
                    break;
                }
            }
            else
            {
                if(model[abs(literal)] == -1)
                {
                    count++;
                    break;
                }
            }
        }
    }
    if(count == clauses.size())
    {
        return 1;
    }
    for(auto clause : clauses)
    {
        int count = 0;
        for(auto literal : clause)
        {
            if(literal > 0)
            {
                if(model[literal] == -1)
                {
                    count++;
                }
            }
            else
            {
                if(model[abs(literal)] == 1)
                {
                    count++;
                }
            }
        }
        if(count == clause.size())
        {
            return -1;
        }
    }
    return 0;
}



bool dpll(vector<vector<int> > clauses, vector<int> &model)
{
    if(check(clauses, model) == 1)
    {
        return true;
    }
    if(check(clauses, model) == -1)
    {
        return false;
    }
    for(int i = 0; i < clauses.size(); i++)
    {
        int count1 = 0;
        int count2 = 0;
        int unit_prop_literal = 0;
        for(auto literal : clauses[i])
        {
            if(model[abs(literal)] == 0)
            {
                count1++;
                unit_prop_literal = literal;
            }
            else if (model[abs(literal)] == -1*sgn(literal))
            {
                count2++;
            }
        }
        if(count2 == clauses[i].size()-1 and count1 == 1)
        {
            if(unit_prop_literal > 0)
            {
                model[unit_prop_literal] = 1;
                return dpll(clauses , model);
            }
            else
            {
                model[abs(unit_prop_literal)] = -1;
                return dpll(clauses , model);
            }
        }
    }
    int next_literal;
    for(int literal = 1; literal < model.size(); literal++)
    {
        if(model[literal] == 0)
        {
            next_literal = literal;
            break;
        }
    }
    model[next_literal] = 1;
    if(dpll(clauses, model) == true)
    {
        return true;
    }
    else 
    {
        model[next_literal] = -1;
        return dpll(clauses, model);
    }
}

pair<vector<vector<int>> , int> read_dimacs_cnf(const string& filename) {
    ifstream file(filename);
    string line;
    vector<vector<int>> clauses;
    int num_vars = 0;

    while (getline(file, line)) {
        if (line[0] == 'c' || line.empty()) {
            continue; // Skip comments and empty lines
        } else if (line[0] == 'p') {
            istringstream iss(line);
            string p;
            iss >> p >> p >> num_vars >> p;

            // Parse the problem line (e.g., "p cnf 5 3")
            // We don't need to use the variable and clause counts 
            // in this example, but you could extract them if needed
            continue;
        } else {
            vector<int> clause;
            istringstream iss(line);
            int literal;

            while (iss >> literal) {
                if (literal == 0) {
                    break; // End of clause
                }
                clause.push_back(literal);
            }
            clauses.push_back(clause);
        }
    }

    return make_pair(clauses, num_vars);
}


int main()
{
    string filename = "test.cnf";
    pair<vector<vector<int> > , int > p  = read_dimacs_cnf(filename);
    vector<vector<int> > clauses = p.first;
    vector<int> model(p.second + 1 , 0);
    // for(auto clause : clauses)
    // {
    //     cout << "{";
    //     for(auto literal : clause)
    //     {
    //         cout << literal << ", ";
    //     }
    //     cout << "}, ";
    // }
    cout << (dpll(clauses, model) ? "SAT" : "UNSAT") << "\n";
    for(int i = 1; i < model.size(); i++)
    {
        cout << i << ":" << (model[i] == 1 ? "TRUE" : "FALSE" ) << "\n";
    }
    cout << endl;
    cout << check(clauses, model) << endl;
 
}