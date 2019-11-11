/*
 * Title: exe_3.3_7.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description:finds the maximum height of a ball thrown at a given speed and angle
 *
 */

#include <iostream>
#include <cmath>

#define PI 3.14159265

using namespace std;

int main(){
	double mph = 7;
	double mps = mph * 1609 / 360;//convert miles per hour to meters per second
	double angle = 45;
	double radians = angle * PI / 180;//convert degrees to radians for sin

	double height = (0.5 * pow(mps, 2) * pow(sin(radians), 2))/9.8;//formula

	cout << "the max height of a ball thrown at " << mph << " miles per hour ";
	cout << "and at a " << angle << " degree angle is " << height << " meters";

}
