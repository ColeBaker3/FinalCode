// FinalProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Simulator.h"

Simulator hospital;

int main()
{
	hospital.enter_data();
	hospital.run_simulation();
	hospital.end_data();
	hospital.end_menu();
    return 0;
}

