#include <iostream>
#include <math.h>
#include <random>



double rastrigin_Function(double x){
	return 10 + x * x - 10 * cos(2 * M_PI * x);
}

double rastrigin_diffential(double x){
	return 2 * x  + 20 * M_PI * sin(2 * M_PI * x);
}

double rastrigin_second_diffential(double x){
	return 2 + 40 * M_PI * M_PI * cos(2 * M_PI * x);
}

double bisection_method(double (*func)(double), 
						double left, double right,
						double minDV, double minDX, int max_steps) {
	double x = (left + right) / 2;
	double value = func(x);
	double dv;
	size_t step = 0;
	double dx;
	do {
		if (value > 0) right = x;
		else left = x;

		x = (left + right) / 2;
		double new_value = func(x);
		dv = abs(new_value - value);
		value = new_value;
		++step;
		dx = abs(left - right);
	} while ((dv > minDV || dx > minDX) && step > max_steps);

	return x;
}

double Niuton_method(double (*func)(double), double (*df)(double), double (*ddf)(double), 
						double x0,
						double minDV, double minDX, int max_steps) {
	double x = x0;
	double value = func(x);
	size_t step = 0;
	double arg_df =  10 * minDX;
	while ((abs(value) > minDV || abs(arg_df) > minDX) && step < max_steps){
		arg_df = (ddf(x)) ? df(x)/ddf(x) : 0;
		x -= arg_df;
		value = func(x);
	}
	return x;
}


int main(){
	std:: cout << "Метод биссектрис для первой производной: " << bisection_method(rastrigin_diffential, -1, 0.1, 1e-10, 1e-10, 1000) << std:: endl;

    std:: cout << "Метод ньютона: " << Niuton_method(rastrigin_Function, rastrigin_diffential, rastrigin_second_diffential, 0.1, 1e-10, 1e-10, 400) << std:: endl;
	return 0;
}
