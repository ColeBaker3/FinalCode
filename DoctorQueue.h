#pragma once

#include <iostream>
#include <string>
#include <queue>
#include "Random.h"
#include "Patient.h"
#include "EmergencyRoomQueue.h"

class DoctorQueue
{
private:
	EmergencyRoom * emergencyQueue; //Pointer to the emergency room queue
	queue<Patient *> doctor_queue; //A queue of Patient pointers
	int treatment_time; //Integer that finds out how long a doctor will take to treat someone (1 - 20) min
	int service_time; //The time it takes to treat someone
	double doctor_wait_time = 0; //the wait time to see a doctor
	double visit_time = 0; //the total visit time a patient is at the emergency room that sees a doctor
	int patients_served; //number of patients seen by this doctor

public:
	//sets the pointer to the emergency room
	void set_emergency_queue(EmergencyRoom *emergencyQueue) {
		this->emergencyQueue = emergencyQueue;
	}

	//returns the number of patients served
	int get_num_of_patients_served() {
		return patients_served;
	}

	//dertermines how long treatment will take and returns it
	int get_treatment_time() {
		treatment_time = my_random.next_int(20) % 20 + 1;
		return treatment_time;
	}

	//gets how long a patient visited the emergency room for
	double get_visit_time() {
		return visit_time;
	}

	//gets the wait time for a patient to see the doctor
	double get_doctor_wait_time() {
		return doctor_wait_time;
	}

	//Checks to see if there is any patients in the emergency room that need to see a doctor
	//and adds them to the doctor queue. If none then checks to see if there is any that just
	//need to see a nurse and puts them in the doctor queue. Also checks to see if the patient with
	//doctor has been treated if there is one.
	void update(int clock)
	{
		if (doctor_queue.empty()) {

			if (!emergencyQueue->emergency_doctor_queue.empty()) {
				Patient * patient = emergencyQueue->emergency_doctor_queue.top();
				emergencyQueue->emergency_doctor_queue.pop();

				doctor_queue.push(patient);
				patient->start_treatment_time = clock;
				service_time = get_treatment_time();
			}
		}

		if (doctor_queue.empty()) {

			if (!emergencyQueue->emergency_nurse_queue.empty()) {
				Patient * patient = emergencyQueue->emergency_nurse_queue.top();
				emergencyQueue->emergency_nurse_queue.pop();

				doctor_queue.push(patient);
				patient->start_treatment_time = clock;
			}
		}

		if (!doctor_queue.empty()) {

			Patient * patient = doctor_queue.front();

			if ((clock - patient->start_treatment_time) > service_time) {

				emergencyQueue->people_served.push_back(doctor_queue.front());

				doctor_queue.pop();
				doctor_wait_time = doctor_wait_time + (patient->start_treatment_time - patient->arrival_time);
				visit_time = visit_time + (clock - patient->arrival_time);
				patients_served++;
			}
		}
	}

	friend class EmergencyRoom;
};