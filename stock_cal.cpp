#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include "stock_cal.h"
using namespace std;

vector <string> name;
vector <int> index;
vector <double> Y;

int times, stock_choose_number, divide_money;
double initial_money, risk, expected_return;
vector <int> stock_buy_number;
string stock_name[50], str;
vector <double> each_money, remain_money, money, d;

//計算趨勢值
double trend_ratio_cal(double expected_return, double risk)
{
	double trend_tmp = 0;
	if (expected_return > 0)
	{
		trend_tmp = expected_return / risk;
	}
	else
	{
		trend_tmp = expected_return * risk;
	}
	return trend_tmp;
}

//計算回歸趨勢線之值
void Y_cal(double expected_return, int times, double initial_money)
{
	for (int h = 1; h < times; h++)
	{
		Y.push_back(expected_return * h + initial_money);
	}
}

//每日風險
double risk_cal(double expected_return, int times, double initial_money, vector <double> money)
{
	double tmp = 0, risk_tmp = 0;

	Y_cal(expected_return, times, initial_money);
	for (int h = 0; h < times - 1; h++)
	{
		tmp = tmp + (money[h] - Y[h]) * (money[h] - Y[h]);
	}
	risk_tmp = sqrt(tmp / double((times - 1)));
	Y.clear();
	return risk_tmp;
}



//每日預期報酬
double expected_return_cal(vector <double> money, int times, double initial_money)
{
	double numerator = 0, denominator = 0, expected_return_tmp = 0;
	for (int h = 0, k = 1; h < times - 1; h++, k++)
	{
		numerator = numerator + (k * money[h] - k * initial_money);
		denominator = denominator + double((k * k));
	}
	expected_return_tmp = numerator / denominator;
	return expected_return_tmp;
}

//多檔的output
void mul_output(int stock_choose_number, vector <int> stock_buy_number, int divide_money, vector <double> remain_money, vector <double> each_money, vector <double> money, int times)
{
	ofstream output("train_2019_06(2019 Q1)_test.csv");

	output << "張數	,";
	for (int h = 0; h < stock_choose_number; h++)
	{
		output << stock_buy_number[h];
		if (h == stock_choose_number - 1)
		{
			output << endl;
		}
		else
		{
			output << ",";
		}
	}

	output << "分配資金	,";
	for (int h = 0; h < stock_choose_number; h++)
	{
		output << divide_money;
		if (h == stock_choose_number - 1)
		{
			output << endl;
		}
		else
		{
			output << ",";
		}
	}

	output << "剩餘資金	,";
	for (int h = 0; h < stock_choose_number; h++)
	{
		output << fixed << setprecision(15) << remain_money[h];
		if (h == stock_choose_number - 1)
		{
			output << endl;
		}
		else
		{
			output << ",";
		}
	}

	output << "FS(" << times << "),";
	for (int h = 0; h < stock_choose_number; h++)
	{
		output << fixed << each_money[h] << ",";
	}
	output << fixed << setprecision(15) << money[times-1] << endl;
	output.close();
}

//尋找指定股票的檔案位置
void find_stock_index(int stock_choose_number,string stock_name[], vector <int> &index)
{
	for (int h, p = 0; p < stock_choose_number; p++)
	{
		for (h = 0; h < name.size(); h++)
		{
			if (name[h] == stock_name[p])
			{
				index.push_back(h);
				break;
			}
		}
	}
}

//對讀檔進來的資料進行處理(處理逗號)
void data_processing(vector <string> &name, vector <double> &d, string str,int times)
{
	int start = 0;
	stringstream ss;
	string tmp;
	double number;

	for (int i = 0; i < str.length(); i++)
	{
		/*if (str[i] == ',' || i == str.length() - 2)
		{
			for (int j = start; j < i; j++)
			{
				tmp = tmp + str[j];
				
			}
			if (i == str.length() - 2)
			{
				tmp = tmp + str[i];
			}
			

			if (times == 0)
			{
				name.push_back(tmp);
			}
			else
			{
				ss << tmp;
				ss >> number;
				d.push_back(number);
				ss.str("");
				ss.clear();
			}
			tmp.clear();
			start = i + 1;
		}*/

		if (str[i] == ',')
		{
			for (int j = start; j < i; j++)
			{
				tmp = tmp + str[j];

			}
			if (times == 0)
			{
				name.push_back(tmp);
			}
			else
			{
				ss << tmp;
				ss >> number;
				d.push_back(number);
				ss.str("");
				ss.clear();
			}
			tmp.clear();
			start = i + 1;
		}
	}
	start = 0;
}




void mai44n()
{
	string str, stock_name[50];
	fstream file;
	ofstream output("train_2019_06(2019 Q1)_INTC+MCD+NKE_trend.csv");
	output << fixed << setprecision(15);
	int k = 0, times = 0;
	vector <double>  stock_price, remain_money, each_money, money;
	double divide_remain_money, initial_money = 10000000, expected_return = 0, risk = 0, trend = 0;
	vector <int> stock_buy_number;
	int stock_choose_number, divide_money;

	//money >> 資金水位, remain_money >> 買後剩餘資金
	//stock_buy_number >> 可買張數, stock_price >> 每張買入價格
	//divide_remain_money >> 分配剩餘資金, stock_choose_number >> 選擇幾檔股票

	ifstream in("train_2019_06(2019 Q1).csv");

	cin >> stock_choose_number;
	output << "投資組合	," << stock_choose_number << endl;
	output << "Stock#,";
	for (int p = 0; p < stock_choose_number; p++)
	{
		cin >> stock_name[p];
		output << stock_name[p];
		if (p == stock_choose_number - 1)
		{
			output << endl;
		}
		else
		{
			output << ",";
		}
	}
	divide_money = (int)initial_money / stock_choose_number;
	
	while (in >> str)
	{
		data_processing(name, d, str,times);

		//output << "第" << times << "天資金水位 : ";

		//如果讀進來是股票名，先搜尋指定股票的位置
		if (times == 0)
		{
			find_stock_index(stock_choose_number, stock_name, index);
		}
		else
		{
			if (times == 1)    //第一次買股票
			{
				
				for (int h = 0; h < stock_choose_number; h++)
				{
					stock_price.push_back(d[index[h]]);
					stock_buy_number.push_back(divide_money / stock_price[h]);
					remain_money.push_back(divide_money - stock_price[h] * stock_buy_number[h]);
					if (stock_choose_number > 1)
					{
						each_money.push_back(divide_money);
					}
				}
				divide_remain_money = initial_money - (double)divide_money * stock_choose_number;

				money.push_back(initial_money);
				if (stock_choose_number > 1)
				{
					
					mul_output(stock_choose_number, stock_buy_number, divide_money, remain_money, each_money, money, times);
				}
			}
			else
			{
				double tmp = 0;
				for (int h = 0; h < stock_choose_number; h++)
				{
					if (stock_choose_number > 1)
					{
						each_money[h] = d[index[h] + name.size() * (times - 1)] * stock_buy_number[h] + remain_money[h];
						tmp = tmp + each_money[h];
					}
					else
					{
						tmp = tmp + d[index[h] + name.size() * (times - 1)] * stock_buy_number[h] + remain_money[h];
					}
				}
				money.push_back(tmp);
				tmp = 0;
				
				if (stock_choose_number > 1)
				{
					money[times-1] = money[times-1] + divide_remain_money;

					output << "FS(" << times  << "),";
					for (int h = 0; h < stock_choose_number; h++)
					{
						output << fixed << setprecision(15) << each_money[h] << ",";
					}
					output << fixed << setprecision(15) << money[times-1] << endl;
				}
			}
			if (stock_choose_number == 1)    //一檔的輸出
			{
				output << fixed << setprecision(15) << money[times-1] << endl;
			}
		}
		times++;
	}
	
	expected_return = expected_return_cal(money, times, initial_money);
	risk = risk_cal(expected_return, times, initial_money, money);
	trend = trend_ratio_cal(expected_return, risk);
	output << "預期報酬," << fixed << setprecision(15) << expected_return << endl;
	output << "風險," << fixed << setprecision(15) << risk << endl;
	output << "趨勢值," << fixed << setprecision(15) << trend << endl;
	output.close();
}
