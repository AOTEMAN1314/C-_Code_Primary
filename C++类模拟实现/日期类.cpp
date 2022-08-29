#include "iostream"
using namespace std;

class Date {
	friend ostream& operator<<(ostream& _cout, Date& d);
private:
	int _year;
	int _month;
	int _day;
public:
	//构造函数
	Date(int year = 2020,int month = 12,int day = 12) 
		:_year(year)
		,_month(month)
		,_day(day)
	{
		//当输入的日期不符合实际 JudgeDay返回false，通过取反进入选择语句
		if (!JudgeDay(year,month,day)) {
			_year = 2021;
			_month = 1;
			_day = 2;
		}
	}

	//判断构造函数中输入的Day变量是否符合实际
	bool JudgeDay(int year,int month,int day) {
		if (year < 0)
			return false;

		if (month > 12 || month < 1)
			return false;

		int Days[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

		//如果是闰年，二月天数要加一
		if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
			Days[2] += 1;

		if (Days[month] >= day && day > 0)
			return true;
		else
			return false;
	}

	//得到对应天数
	int GetDay(int year,int month) {
		int Days[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
		if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
			Days[2] += 1;
		return Days[month]; 
	}

	//拷贝构造函数
	Date(const Date& d) 
		:_year(d._year)
		,_month(d._month)
		,_day(d._day)
	{}

	//赋值运算符重载
	Date& operator=(const Date& d) {
		//判断是否是自己给自己赋值
		if (this != &d) {
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}

	//+运算符重载，计算x天后的日期
	Date operator+(int day) {
		if (day < 0) {
			return *this - (0 - day);
		}

		Date temp(*this);
		temp._day += day;
		while (temp._day > GetDay(temp._year, temp._month)) {
			temp._day -= GetDay(temp._year, temp._month);
			temp._month += 1;
			if (temp._month > 12) {
				temp._month %= 12;
				temp._year += 1;
			}
		}
		return temp;
	}

	//-运算符重载，计算x天前的日期
	Date operator-(int day) {
		//复用重载后的 + 运算符
		if (day < 0) {
			return *this + (0 - day);
		}

		Date temp(*this);
		temp._day -= day;
		while (temp._day <= 0) {
			temp._month -= 1;

			if (temp._month <= 0) {
				temp._month = 12;
				temp._year -= 1;
			}

			temp._day += GetDay(temp._year, temp._month);
		}
		return temp;
	}

	//-运算符重载，计算两个日期对象之间差多少天
	int operator-(const Date& d) {
		Date minDate(*this);
		Date maxDate(d);
		if (minDate > maxDate) {
			minDate = d;
			maxDate = *this;
		}
		int count = 0;
		while (minDate != maxDate) {
			count++;
			minDate++;
		}
		return count;
	}

	// >重载
	//this大，返回ture
	bool operator>(const Date& d) {
		if ((_year > d._year) || (_year == d._year && _month > d._month) || (_year == d._year && _month == d._month && _day > d._day)) {
			return true;
		}
		else {
			return false;
		}
	}

	// <重载
	//复用>
	bool operator<(const Date& d) {
		return !(*this > d || *this == d);
	}
	bool operator==(const Date& d) {
		return (_year == d._year) && (_month == d._month) && (_day == d._day);
	}
	
	// != 重载
	//复用==
	bool operator!=(const Date& d) {
		return !(*this == d);
	}

	//前置++重载
	Date& operator++() {
		*this = *this + 1;
		return *this;
	}

	//后置++重载
	Date operator++(int x) {
		Date temp(*this);
		*this = *this + 1;
		return temp;
	}

	//前置--重载
	Date& operator--() {
		*this = *this - 1;
		return *this;
	}

	//后置--重载
	Date operator--(int x) {
		Date temp(*this);
		*this = *this - 1;
		return temp;
	}
};
//<<重载为全局函数
ostream& operator<<(ostream & _cout,Date& d) {
	cout << d._year << "-" << d._month << "-" << d._day;
	return _cout;
}

int main() {
	Date d1(2022, 1, 1);
	cout << "d1:" << d1 << endl;//输出 d1:2022-1-1
	Date d2 = d1;
	cout << "d2:" << d2 << endl;//输出 d2:2022-1-1
	Date d3 = d1 - 100;
	cout << "d3:" << d3 << endl;//输出 d3:2021-9-23
	cout << "d3 - d1:" << d3 - d1 << endl;//输出 d3 - d1:100
}