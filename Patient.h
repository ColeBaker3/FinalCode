#pragma once
#include <iostream>
#include <string>
using namespace std;

extern Random my_random;

struct Patient
{
	Patient(string fname, string lname, int level_of_illness, int clock) : 
		first_name(fname), last_name(lname), illness_level(level_of_illness), start_treatment_time(0), arrival_time(clock) {}

	Patient() {}

	int arrival_time;
	int illness_level;
	int start_treatment_time;
	string first_name;
	string last_name;
	string fnames[2000];
	string lnames[1000];

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

	string get_first_name() {
		return fnames[my_random.next_int(2000)];
	}

	string get_last_name() {
		return lnames[my_random.next_int(1000)];
	}
};

