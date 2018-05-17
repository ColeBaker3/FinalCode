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
	Patient * patient; //pointer to the patient
	double arrival_rate; //arrival rate per hour of patients to the emergency room
	int num_of_doctors; //number of doctors working during the simulation
	int num_of_nurses; //number of nurses working during the simulation
	int num_of_patients = 0; //total number of patients that enter the emergency room



public:

	priority_queue<Patient *> emergency_nurse_queue; //priority queue for patients with illness levels under 11
	priority_queue<Patient *> emergency_doctor_queue; //priority queue for patients with illness levels above 10

	vector<Patient *> people_served; //vector to put the patients treated into

	//sets the patient pointer the the current patient
	void set_patient(Patient * patient) {
		this->patient = patient;
	}

	//sets the arrival rate of patients
	void set_arrival_rate(double arrival_rate) {
		this->arrival_rate = arrival_rate;
	}

	//returns the number of patients that entered the emergency room
	double get_num_of_patients() {
		return num_of_patients;
	}

	//sets the number of doctors working
	void set_num_of_doctors(int num_of_doctors) {
		this->num_of_doctors = num_of_doctors;
	}

	//sets the number of nurses working
	void set_num_of_nurses(int num_of_nurses) {
		this->num_of_nurses = num_of_nurses;
	}

	//generates a random level of illness for incoming patients
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

	//checks if a randomly generated number is less than the arrival rate and then
	//generates a illness level and adds a patient to one of the priority queues depending
	//on their illness level.
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