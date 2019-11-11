/*
 * Title: exe_5.2_5.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: list distances in feet and meters from 3-30 feet
 *
 */

#include <iostream>

using namespace std;

int main(){
	const float conversion = 1/3.28;

	for(int i=3;i<30;i+=3){
		cout << i << "ft. = " << i*conversion << "m\n";
	}
}
