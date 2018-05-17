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
	EmergencyRoom * emergencyQueue;
	queue<Patient *> doctor_queue;
	int treatment_time;
	int service_time;
	double doctor_wait_time = 0;
	double visit_time = 0;
	int patients_served;

public:

	void set_emergency_queue(EmergencyRoom *emergencyQueue) {
		this->emergencyQueue = emergencyQueue;
	}

	int get_num_of_patients_served() {
		return patients_served;
	}

	int get_treatment_time() {
		treatment_time = my_random.next_int(20) % 20 + 1;
		return treatment_time;
	}

	double get_visit_time() {
		return visit_time;
	}

	double get_doctor_wait_time() {
		return doctor_wait_time;
	}

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