cnfgen randkcnf 200 1200 30000 > test.cnf
g++ -std=c++17 -O3 -o runsat dpll.cpp
./runsat