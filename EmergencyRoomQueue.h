#pragma once

#include <iostream>
#include <string>
#include <queue>
#include "Random.h"
#include "Patient.h"

extern Random my_random;

class EmergencyRoom
{
private:
	Patient * patient;
	double arrival_rate;
	int num_of_doctors;
	int num_of_nurses;
	int num_of_patients = 0;



public:

	priority_queue<Patient *> emergency_nurse_queue;
	priority_queue<Patient *> emergency_doctor_queue;

	vector<Patient *> people_served;

	void set_patient(Patient * patient) {
		this->patient = patient;
	}

	void set_arrival_rate(double arrival_rate) {
		this->arrival_rate = arrival_rate;
	}

	double get_num_of_patients() {
		return num_of_patients;
	}

	void set_num_of_doctors(int num_of_doctors) {
		this->num_of_doctors = num_of_doctors;
	}

	void set_num_of_nurses(int num_of_nurses) {
		this->num_of_nurses = num_of_nurses;
	}

	int level() {
		double val = my_random.next_double();

		int random;
		if (val <= 0.70)       //  70%
			random = (my_random.next_int(10) % 10) + 1;
		else if (val > 0.70 && val <= 0.90)  //  20%
			random = (my_random.next_int(5) % 5) + 11;
		else
			random = (my_random.next_int(5) % 5) + 16;
		return random;
	}


	void update(int clock) {
		if (my_random.next_double() < arrival_rate) {
			num_of_patients++;
			int illness = level();

			Patient * person = new Patient(patient->get_first_name(), patient->get_last_name(), illness, clock);

			if (illness < 11) {
				emergency_nurse_queue.push(person);
			}
			else {
				emergency_doctor_queue.push(person);
			}
		}
	}

};