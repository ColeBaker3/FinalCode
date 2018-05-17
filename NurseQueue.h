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
	EmergencyRoom* emergencyQueue;
	queue<Patient* > nurse_queue;
	int treatment_time;
	int service_time;
	double visit_time = 0;
	double nurse_wait_time = 0;
	int patients_served;

public:
	NurseQueue() {}

	void set_emergency_queue(EmergencyRoom *emergencyQueue) {
		this->emergencyQueue = emergencyQueue;
	}

	int get_num_of_patients_served() {
		return patients_served;
	}

	int get_treatment_time() {
		treatment_time = my_random.next_int(10) % 10 + 1;
		return treatment_time;
	}

	double get_visit_time() {
		return visit_time;
	}

	double get_nurse_wait_time() {
		return nurse_wait_time;
	}

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

				emergencyQueue->people_served.push_back(nurse_queue.front());

				nurse_queue.pop();
				nurse_wait_time = nurse_wait_time + (patient->start_treatment_time - patient->arrival_time); //the wait time to be seen by a nurse
				visit_time = visit_time + (clock - patient->arrival_time); //adds up for total visit time
				patients_served++; //number of patients served by nurses
			}
		}
		

	}

	friend class EmergencyRoom;
};