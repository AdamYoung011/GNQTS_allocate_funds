#include <stdio.h>
using namespace std;

double trend_ratio_cal(double, double);
void Y_cal(double expected_return, int times, double initial_money);
double risk_cal(double expected_return, int times, double initial_money, vector <double> money);
double expected_return_cal(vector <double> money, int times, double initial_money);
void mul_output(int stock_choose_number, vector <int> stock_buy_number, int divide_money, vector <double> remain_money, vector <double> each_money, vector <double> money, int time);
void find_stock_index(int stock_choose_number, string stock_name[], vector <int>& index);
void data_processing(vector <string>& name, vector <double>& d, string str, int times);