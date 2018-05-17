#pragma once

#include <iostream>
#include <string>
#include <queue>
#include "Random.h"
#include "Patient.h"
#include "EmergencyRoomQueue.h"



class NurseQueue
{
private:
	EmergencyRoom* emergencyQueue; //Pointer to the emergency room queue
	queue<Patient* > nurse_queue; //A queue of Patient pointers
	int treatment_time; //Integer that finds out how long a nurse will take to treat someone (1 - 10) min
	int service_time; //The time it takes to treat someone
	double visit_time = 0; //the wait time to see a doctor
	double nurse_wait_time = 0; //the total visit time a patient is at the emergency room that sees a nurse
	int patients_served; //number of patients seen by this nurse

public:
	NurseQueue() {}

	//sets pointer to the emergency room
	void set_emergency_queue(EmergencyRoom *emergencyQueue) {
		this->emergencyQueue = emergencyQueue;
	}

	//returns the number of patients served by the nurse
	int get_num_of_patients_served() {
		return patients_served;
	}

	//generates a treatment time for that patient
	int get_treatment_time() {
		treatment_time = my_random.next_int(10) % 10 + 1;
		return treatment_time;
	}

	//returns the visit time for the patient
	double get_visit_time() {
		return visit_time;
	}

	//returns the wait time to see that nurse
	double get_nurse_wait_time() {
		return nurse_wait_time;
	}

	//Checks to see if there is any patients in the emergency room that need to see a nurse
	//and adds them to the nurse queue. If none it checks to see if there is a patient with
	//the nurse that has been treated and can leave.
	void update(int clock)
	{
		if (nurse_queue.empty()) {
			if (!emergencyQueue->emergency_nurse_queue.empty()) {
				Patient * patient = emergencyQueue->emergency_nurse_queue.top();
				emergencyQueue->emergency_nurse_queue.pop();

				nurse_queue.push(patient);
				patient->start_treatment_time = clock;
				service_time = get_treatment_time();
			}
		}

		if (!nurse_queue.empty()) {

			Patient * patient = nurse_queue.front();

			if ((clock - patient->start_treatment_time) > service_time) {

				emergencyQueue->people_served.push_back(nurse_queue.front()); //adds to the people that have been treated vector

				nurse_queue.pop(); //removes from the nurses queue

				nurse_wait_time = nurse_wait_time + (patient->start_treatment_time - patient->arrival_time); //the wait time to be seen by a nurse
				visit_time = visit_time + (clock - patient->arrival_time); //adds up for total visit time
				patients_served++; //number of patients served by nurses
			}
		}
		

	}

	friend class EmergencyRoom;
};