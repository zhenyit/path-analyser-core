#include "stdafx.h"
void tommorrow(int day, int date, int month, int year)
{
	int next_day = day;
	int next_date = date;
	int next_month = month;
	int next_year = year;
	if (day == 7)
		next_day = 1;
	else
		next_day++;
	if (date == 31 && month == 12)
	{
		next_date = 1;
		next_month = 1;
		next_year++;
	}
	else if (date == 28 && month == 2)
	{
		if (year % 4 == 0 || year & 400 == 0)
		{
			next_date = 29;
		}
		else
		{
			next_date = 1;
			next_month = 3;
		}
	}
	else if (date == 31 || (date == 29 && month == 2) ||
		(date == 30 && (month == 4 || month == 6 || month == 9 || month == 11)))
	{
		next_date = 1;
		next_month++;
	}
	else
	{
		next_date++;
	}
}