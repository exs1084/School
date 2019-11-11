/*
 * Title: exe_5.4_7.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description:convert km/hr to mi/hr for 60-110 km/hr
 *
 */

#include <iostream>
#include <cmath>

using namespace std;

int main(){
	const float conversion = 0.6241;//km->mi
	for(int i=60;i<=110;i+=5){
		float miles = i*conversion;
		cout << i << "km/hr = " << miles << "mi/hr\n";
	}
}
