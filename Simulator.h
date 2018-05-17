#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "EmergencyRoomQueue.h"
#include "DoctorQueue.h"
#include "NurseQueue.h"
#include "Random.h"

Random my_random;

class Simulator 
{
private:
	int num_of_doctors;
	int num_of_nurses;
	int total_time = 10080;
	double average_visit_time;
	double average_wait_time;
	int total_patients_served = 0;
	int clock;

	Patient *patient;
	EmergencyRoom *emergencyRoom;
	vector < NurseQueue* > nurses;
	vector < DoctorQueue* > doctors;

	int read_int(const std::string &prompt, int low, int high)
	{
		if (low >= high) // invalid range
			throw std::invalid_argument("invalid range specified");

		std::cin.exceptions(std::ios_base::failbit);
		int num = 0;
		while (true) {
			try {
				while (true) {
					std::cout << prompt;
					std::cin >> num;
					if (num >= low && num <= high) { // within the specified range
						std::cout << std::endl;
						return num;
					}
				}
			}
			catch (std::ios_base::failure) {
				std::cout << "Bad numeric string -- try again\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			}
		}
	}

public:
	Simulator() {
		emergencyRoom = new EmergencyRoom;
		patient = new Patient;
	}

	void enter_data() {
		cout << "Welcome to the 273ville Hospital!\n";
		cout << "Here you can test to find the most effiecent hospital.\n";
		cout << "-------------------------------------------------------\n\n";

		double rate = read_int("Please enter in the patient arrival rate per hour: ", 1, 59);
		double arrival_rate = (rate / 60);
		num_of_doctors = read_int("Please enter in the number of doctor(s): ", 1, INT_MAX);
		num_of_nurses = read_int("Please enter in the number of nurse(s): ", 1, INT_MAX);

		for (int i = 0; i < num_of_doctors; i++) {
			doctors.push_back(new DoctorQueue());
		}

		for (int i = 0; i < num_of_nurses; i++) {
			nurses.push_back(new NurseQueue());
		}

		patient->add_patients();
		emergencyRoom->set_arrival_rate(arrival_rate);
		emergencyRoom->set_num_of_doctors(num_of_doctors);
		emergencyRoom->set_num_of_nurses(num_of_nurses);
		emergencyRoom->set_patient(patient);

		for (int i = 0; i < num_of_nurses; i++) {
			nurses[i]->set_emergency_queue(emergencyRoom);
		}

		for (int i = 0; i < num_of_doctors; i++) {
			doctors[i]->set_emergency_queue(emergencyRoom);
		}
	}

	void run_simulation() {
		for (clock = 0; clock < total_time; clock++)
		{
			emergencyRoom->update(clock);
			for (int i = 0; i < num_of_nurses; i++) {
				nurses[i]->update(clock);
			}
			for (int i = 0; i < num_of_doctors; i++) {
				doctors[i]->update(clock);
			}
		}
	}

	void show_list(){
		for (int i = 0; i < total_patients_served; i++) {
			cout << emergencyRoom->people_served.at(i)->first_name << " ";
			cout << emergencyRoom->people_served.at(i)->last_name << "; ";
			cout << "Illness level: " << emergencyRoom->people_served.at(i)->illness_level;
			cout << '\n';
		}
	}

	void search(string name) {
		bool found = false;

		for (int i = 0; i < total_patients_served; i++) {
			if (emergencyRoom->people_served.at(i)->first_name == name) {
				cout << emergencyRoom->people_served.at(i)->first_name << " ";
				cout << emergencyRoom->people_served.at(i)->last_name << "; Illness level of ";
				cout << emergencyRoom->people_served.at(i)->illness_level << endl;
				found = true;
			}
		}

		if (found == false) {
			cout << "Nobody with the name " << name << endl;
		}
		cout << endl;
	}

	void end_data() {
		double total_nurse_wait_time = 0;
		double total_doctor_wait_time = 0;
		double total_visit_time = 0;

		cout << "Patients Entered Emergency Room: " << emergencyRoom->get_num_of_patients() << endl;

		for (int i = 0; i < num_of_nurses; i++) {
			total_visit_time = total_visit_time + nurses[i]->get_visit_time();
			total_nurse_wait_time = total_nurse_wait_time + nurses[i]->get_nurse_wait_time();
			total_patients_served = total_patients_served + nurses[i]->get_num_of_patients_served();
		}
		for (int i = 0; i < num_of_doctors; i++) {
			total_visit_time = total_visit_time + doctors[i]->get_visit_time();
			total_doctor_wait_time = total_doctor_wait_time + doctors[i]->get_doctor_wait_time();
			total_patients_served = total_patients_served + doctors[i]->get_num_of_patients_served();
		}
		average_wait_time = (total_nurse_wait_time + total_doctor_wait_time) / total_patients_served;
		average_visit_time = (total_visit_time / total_patients_served);

		cout << "The average wait time was: " << average_wait_time << endl;
		cout << "The average visit time was: " << average_visit_time << endl;
		cout << "The total number of patients served was: " << total_patients_served << endl << endl;
	}

	void end_menu() {
		int num = 0;
		string search_name;

		cout << "Hospital Records:\n-------------------" << endl;
		while (num != 3) {
			cout << "What would you like to do?\n---------------------\n";
			cout << "View treated patients - 1\n";
			cout << "Search for a patient - 2\n";
			cout << "Exit - 3\n";
			num = read_int("Please Enter a Choice: ", 1, 3);
			cout << endl;

			switch (num) {
			case 1:
				show_list();
				break;
			case 2:
				cout << "Please enter a name to search: ";
				cin >> search_name;
				cout << "\nWe Found:\n----------\n";
				search(search_name);
				break;
			case 3:
				cout << "Thanks for using the simulator\n";
				break;
			default:
				cout << "invalid choice\n\n";
				num = 0;
				break;
			}
		}
	}
};