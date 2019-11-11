/*
 * Title: exe_5.5_8.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: calculate birthday gifts of a child
 *
 */

#include <iostream>
#include <cmath>

using namespace std;

int main(){
	int age = 12;
	int currGift = 10;
	int totalGift = 10;
	while(totalGift <=1000){
		currGift *=2;
		totalGift += currGift;
		age++;
	}
	cout << "at age " << age << " the child has been given $" << totalGift;
}
