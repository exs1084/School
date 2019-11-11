/*
 * Title: project1.cpp
 * Course: CPET-121
 * Developer: Erikson Sodergren
 * Date: Spring 2019
 * Description: calculates final grade from many inputs
 *
 */

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cctype>

using namespace std;

int main(){
	const int weightHW = 15;
	const int weightLab = 20;
	const int weightExam = 25;
	const int weightPaper = 10;
	const int weightPart = 5;

	float hw1, hw2, hw3, hw4, hw5, hw6, hw7, hw8, avgHw;
	float lab1, lab2, lab3, lab4, avgLab, midterm, final;
	char paper;
	float paperGrade;
	bool partic;
	float totalGrade=0;

	cout << "please enter your 8 numerical HW grades separated by spaces\n>";
	cin >> hw1 >> hw2 >> hw3 >> hw4 >> hw5 >> hw6 >> hw7 >> hw8;
	cout << "please enter your 4 numerical lab grades separated by spaces\n>";
	cin >> lab1 >> lab2 >> lab3 >> lab4;
	cout << "please enter your numerical midterm and final grades\n>";
	cin >> midterm >> final;
	cout << "please enter your paper letter grade\n>";
	cin >> paper;
	paper = toupper(paper);
	cout << "please enter '1' for good participation or '0' for bad\n>";
	cin >> partic;

	//average scores and also convert into percentage
	avgHw = (hw1 + hw2 + hw3 + hw4 + hw5 + hw6 + hw7 + hw8)/.8;
	avgLab = (lab1 + lab2 + lab3 + lab4)/2;

	switch(paper){
		case('A'):
			paperGrade = 95;
			break;
		case('B'):
			paperGrade = 85;
			break;
		case('C'):
			paperGrade = 75;
			break;
		case('D'):
			paperGrade = 65;
			break;
		case('F'):
			paperGrade = 55;
			break;
		default:
			paperGrade = 0;
	}
	//add everything into the total grade with weighting
	totalGrade += avgHw * weightHW;
	totalGrade += avgLab * weightLab;
	totalGrade += midterm * weightExam;
	totalGrade += final * weightExam;
	totalGrade += paperGrade * weightPaper;
	totalGrade +=partic * weightPart * 100;
	totalGrade /= 100;

	cout << "Homework Average...... : "<< setprecision(2) << fixed << avgHw << "\n";
	cout << "Laboratory Average.... : "<< setprecision(2) << fixed << avgLab << "\n";
	cout << "Midterm Exam.......... : "<< setprecision(0) << midterm << "\n";
	cout << "Final Exam............ : "<< final << "\n";
	cout << "Term Paper............ : "<< paperGrade << "[" << paper << "]\n";
	cout << "Class Participation... : "<< partic << "00\n\n";
	cout << "Final Average......... : "<< setprecision(2) << fixed << totalGrade;

	string finalLetter;
	string msg = "";

	if(totalGrade > 93){
		finalLetter = "A";
	}else if(totalGrade >= 90){
		finalLetter = "A-";
	}else if(totalGrade >= 87){
		finalLetter = "B+";
	}else if(totalGrade >= 83){
		finalLetter = "B";
	}else if(totalGrade >= 80){
		finalLetter = "B-";
	}else if(totalGrade >= 77){
		finalLetter = "C+";
	}else if(totalGrade >= 73){
		finalLetter = "C";
	}else if(totalGrade >= 70){
		finalLetter = "C-";
		msg = "Please see your academic advisor.";
	}else if(totalGrade >= 60){
		finalLetter = "D";
		msg = "Please see your academic advisor.";
	}else{
		finalLetter = "F";
		msg = "Sorry, you will have to repeat this course.";
	}

	cout << "\nCourse Letter Grade... : "<< finalLetter << "\n";
	cout << msg;




}
