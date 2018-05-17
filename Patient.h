#pragma once
#include <iostream>
#include <string>
using namespace std;

extern Random my_random;

struct Patient
{
	//Patient constructor
	Patient(string fname, string lname, int level_of_illness, int clock) : 
		first_name(fname), last_name(lname), illness_level(level_of_illness), start_treatment_time(0), arrival_time(clock) {}
	//Default constructor
	Patient() {}

	int arrival_time; //when they arrived at the emergency room
	int illness_level; //what their illness level is
	int start_treatment_time; //when they see the doctor/nurse
	string first_name; //their first name
	string last_name; //their last name
	string fnames[2000]; //array that holds all the first names from the town
	string lnames[1000]; //array that holds all the last names from the town

	//takes the patients from the list and adds them to the array
	void add_patients() {
		ifstream firstNameFile, lastNameFile;
		firstNameFile.open("residents_of_273ville.txt");
		lastNameFile.open("surnames_of_273ville.txt");

		for (int i = 0; i < 2000; i++) {
			string fname;
			getline(firstNameFile, fname);
			fnames[i] = fname;
		}

		for (int i = 0; i < 1000; i++) {
			string lname;
			getline(lastNameFile, lname);
			lnames[i] = lname;
		}
	}

	//gets a random first name from the array
	string get_first_name() {
		return fnames[my_random.next_int(2000)];
	}

	//gets a random last name from the array
	string get_last_name() {
		return lnames[my_random.next_int(1000)];
	}
};

