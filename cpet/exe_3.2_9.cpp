/*
 * Title: exe_3.2_9.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: calculate the resistance of 3 resistors in parallel
 *
 */

#include <iostream>
#include <iomanip>

using namespace std;

int main(){
	double res1 = 1000;
	double res2 = 1000;//set resistances of resistors
	double res3 = 1000;

	double result=0;
	result = 1/(1/res1 + 1/res2 + 1/res3);//calculate result

	cout << "The combined resistance is "<< fixed << setprecision(2);
	cout << result << " ohms";//print output

	return 0;
}
