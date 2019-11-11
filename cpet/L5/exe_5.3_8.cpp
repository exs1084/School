/*
 * Title: exe_5.3_8.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: calculate force required to keep a boat still at a dock
 *
 */

#include <iostream>
#include <cmath>

using namespace std;

int main(){
	const float PI = 3.14159;
	float euler = 2.718;
	float alpha = PI/2;
	float friction = 0.4;

	int load = 500;

	for(int i=1;i<=8;i++){
		float finalLoad = load * pow(euler, -1*friction*alpha);
		cout << "force required to hold the boat with alpha = " << alpha << " ";
		cout << "is " << finalLoad << " newtons\n";
		alpha += PI/2;
	}

}
