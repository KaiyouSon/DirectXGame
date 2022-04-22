#include "Easeing.h"

#include <math.h>

const double PI = 3.1415926;

double Easeing::In(double timer, double totaltime, double start_x, double end_x, int pow_num) {
	timer /= totaltime;
	end_x -= start_x;
	return end_x * pow(timer, pow_num) + start_x;
}

double Easeing::Out(double timer, double totaltime, double start_x, double end_x, int pow_num) {
	timer /= totaltime;
	timer = timer - 1;
	end_x -= start_x;

	if (pow_num % 2 == 1)
		return end_x * (pow(timer, pow_num) + 1) + start_x;
	if (pow_num % 2 == 0)
		return -end_x * (pow(timer, pow_num) - 1) + start_x;
}

double Easeing::InOut(double timer, double totaltime, double start_x, double end_x, int pow_num) {
	timer /= totaltime / 2.0;
	end_x -= start_x;
	if (timer < 1)
		return end_x / 2.0 * pow(timer, pow_num) + start_x;
	timer = timer - 2;

	if (pow_num % 2 == 1)
		return end_x / 2.0 * (pow(timer, pow_num) + 2) + start_x;
	if (pow_num % 2 == 0)
		return -end_x / 2.0 * (pow(timer, pow_num) - 2) + start_x;
}

double Easeing::InBack(
  double timer, double totaltime, double start_x, double end_x, int pow_num, double back) {
	timer /= totaltime;
	end_x -= start_x;
	return end_x * pow(timer, pow_num) * ((back + 1.0) * timer - back) + start_x;
}