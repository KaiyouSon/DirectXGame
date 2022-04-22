#pragma once

class Easeing {
  public:
	double In(double timer, double totaltime, double start_x, double end_x, int pow_num);
	double Out(double timer, double totaltime, double start_x, double end_x, int pow_num);
	double InOut(double timer, double totaltime, double start_x, double end_x, int pow_num);
	double InBack(
	  double timer, double totaltime, double start_x, double end_x, int pow_num, double back);
};