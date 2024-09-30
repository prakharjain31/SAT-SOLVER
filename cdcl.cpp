#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
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

void unit_prop(vector<vector<int> > clauses, vector<int> &model)
{
    for(auto clause : clauses)
    {
        int count1 = 0;
        int count2 = 0;
        int unit_prop_literal = 0;
        for(auto literal : clause)
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
        if(count2 == clause.size() - 1 and count1 == 1)
        {
            if(unit_prop_literal > 0)
            {
                model[unit_prop_literal] = 1;
                return unit_prop(clauses , model);
            }
            else
            {
                model[abs(unit_prop_literal)] = -1;
                return unit_prop(clauses , model);
            }
        }
    }
}

vector<int> analyse_conflict(vector<vector<int> > clauses, vector<int> model, int &decision_level, vector<vector<int> > decision_stack)
{
    if(decision_level == 1)
    {
        decision_level--;
        vector<int> m1 = decision_stack[decision_level];
        vector<int> m2 = decision_stack[decision_level + 1];
        vector<int> conflict_clause;
        int next_literal = 0;
        for(int i = 1; i < m1.size(); i++)
        {
            if(m1[i] != m2[i])
            {
                next_literal = -1*m2[i]*i;
                break;
            }
        }
        conflict_clause.push_back(next_literal);
        return conflict_clause;
    }
    else {
        decision_level -= 2;
        vector<int> m1 = decision_stack[decision_level];
        vector<int> m2 = decision_stack[decision_level + 1];
        vector<int> m3 = decision_stack[decision_level + 2];
        vector<int> conflict_clause;
        int next_literal = 0;
        for(int i = 1; i < m1.size(); i++)
        {
            if(m1[i] != m2[i])
            {
                next_literal = -1*m2[i]*i;
                break;
            }
        }
        conflict_clause.push_back(next_literal);
        for(int i = 1; i < m2.size(); i++)
        {
            if(m2[i] != m3[i])
            {
                next_literal = -1*m3[i]*i;
                break;
            }
        }
        conflict_clause.push_back(next_literal);
        return conflict_clause;

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

bool cdcl(vector<vector<int> > clauses, vector<int>& model)
{
   int decision_level = 0;
   vector<vector<int> > decision_stack(100, vector<int>(model.size(), 0));
   unit_prop(clauses, model);

   do
   {
        while(check(clauses, model) == -1)
        {
            if(decision_level == 0) return false;
            vector<int> temp = analyse_conflict(clauses, model, decision_level, decision_stack);
            model = decision_stack[decision_level];
            clauses.push_back(temp);
            unit_prop(clauses, model);
        }

        if(check(clauses, model) == 0)
        {
            decision_stack[decision_level] = model;
            decision_level++;
            for(int i = 1; i < model.size(); i++)
            {
                if(model[i] == 0)
                {
                    model[i] = 1;
                    break;
                }
            }
            unit_prop(clauses, model);

        }



   } while (check(clauses, model) != 1);
   return true;
}

int main()
{
   string filename = "test.cnf";
    pair<vector<vector<int> > , int > p  = read_dimacs_cnf(filename);
    vector<vector<int> > clauses = p.first;
    vector<int> model(p.second + 1 , 0);
    cout << cdcl(clauses, model) << endl;
    for(int i = 1; i < model.size(); i++)
    {
        cout << i << " : " << (model[i] ? "TRUE" : "FALSE") << "\n";
    }
    return 0;
}