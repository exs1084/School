/*
 * Title: exe_3.5_4.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: finds the drag force on a car for a given set of varables.
 *
 */

#include <iostream>

using namespace std;

int main(){
	double DRAG_COEF = 0.26;
	double AIRDENSITY = .0077;
	int area = 20;
	int velocity = 88;
	double drag = 0;

	drag = DRAG_COEF * AIRDENSITY * area * velocity * velocity /2;

	cout << "The drag force on a vehicle with front area of "<< area << " ft^2";
	cout << " at a speed of " << velocity << " feet per second,\n with an air ";
	cout << "density of " << AIRDENSITY << " lb/ft^3 and a coefficient of drag ";
	cout << "equal to " << DRAG_COEF << " is " << drag << " lbs.";
}
