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
	int num_of_doctors; //number of doctors in simulation
	int num_of_nurses; //number of nurses in simulation
	int total_time = 10080; //total duration of simulation (10080 minutes or 1 week)
	double average_visit_time; //average patient visit time to the emergency room
	double average_wait_time; // average patient wait time to the emergency room
	int total_patients_served = 0; //total number of patients treated
	int clock; //clock used from running the simulation

	Patient *patient; //pointer to the patient
	EmergencyRoom *emergencyRoom; //pointer to the emergency room
	vector < NurseQueue* > nurses; //vector of Nurses
	vector < DoctorQueue* > doctors; //vector of Doctors

public:
	Simulator() {
		emergencyRoom = new EmergencyRoom;
		patient = new Patient;
	}

	//reads out a prompt and then lets you enter a valid integer
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

	//takes in all the user input
	void enter_data() {
		cout << "Welcome to the 273ville Hospital!\n";
		cout << "Here you can test to find the most effiecent hospital.\n";
		cout << "-------------------------------------------------------\n\n";

		double rate = read_int("Please enter in the patient arrival rate per hour: ", 1, 59); //patient arrival rate
		double arrival_rate = (rate / 60);
		num_of_doctors = read_int("Please enter in the number of doctor(s): ", 1, INT_MAX); //number of doctors
		num_of_nurses = read_int("Please enter in the number of nurse(s): ", 1, INT_MAX); //number of nurses

		//creates new doctor queues for the number of doctors user inputed
		for (int i = 0; i < num_of_doctors; i++) {
			doctors.push_back(new DoctorQueue());
		}

		//creates new nurse queues for the number of nurses user inputed
		for (int i = 0; i < num_of_nurses; i++) {
			nurses.push_back(new NurseQueue());
		}

		patient->add_patients(); //adds the patients from the list
		emergencyRoom->set_arrival_rate(arrival_rate); //sets the arrival rate for emergency room
		emergencyRoom->set_num_of_doctors(num_of_doctors); //sets number of doctors for emergency room
		emergencyRoom->set_num_of_nurses(num_of_nurses); //sets number of nurses for emergency room
		emergencyRoom->set_patient(patient); //sets the patient pointer

		//sets the emergency queue to all the nurses
		for (int i = 0; i < num_of_nurses; i++) {
			nurses[i]->set_emergency_queue(emergencyRoom);
		}

		//sets the emergency queue to all the doctors
		for (int i = 0; i < num_of_doctors; i++) {
			doctors[i]->set_emergency_queue(emergencyRoom);
		}
	}

	//loops through the different queues over the course of 10080 clock ticks
	//representing a week of simulation
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

	//displays every patient that has been served and their illness level
	void show_list(){
		for (int i = 0; i < total_patients_served; i++) {
			cout << emergencyRoom->people_served.at(i)->first_name << " ";
			cout << emergencyRoom->people_served.at(i)->last_name << "; ";
			cout << "Illness level: " << emergencyRoom->people_served.at(i)->illness_level;
			cout << '\n';
		}
	}

	//searches for a patient that has been served by their first name
	void search(string name) {
		bool found = false;
		//loops through the vector
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

	//displays the average wait time and average visit time of the patients treated
	//shows how many patients entered the emergency room and were treated also
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

	//lets you choose whether you want to display all the patients, search for an indiviual one by
	//their first name or exit the program
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