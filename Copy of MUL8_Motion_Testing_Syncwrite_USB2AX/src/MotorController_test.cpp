/*
 *  MotorController_test.cpp
 *
 *  Created by: Kellen Carey
 *  Date: 11/14/2013
 */

#include "MotorController.h"
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>

using namespace std;

MotorController motorController;

string input;

void motionTesting();
void createNewMotion();
void createNewMotionQueue();

int main(int argc, char* argv[]) {

	motorController.setMotion("Stand");
	motorController.step(false);



	bool done = false;
	bool MotionTesting = false;
	bool executingQueue = false;

	do {
		cout << "S = Stand\nT= Motion Testing\nM= Play Motion Queue\nX = exit " << std::endl;
		std::cout << "Enter name of the motion you'd like to play:\t" << std::endl;
		std::cout << "T for editing motions and individual testing:\t";

		std::cin >> ws;
		std::getline(std::cin, input);


		if (input == "S" || input == "s") {
			motorController.setMotion("Stand");
			cout << "Standing" << endl;
		}

		else if (input == "X" || input == "x"){
			done = true;
		}
		else if (input == "T" || input == "t") {
			motionTesting();
			MotionTesting = true;
		}
		else if (input == "M" || input == "m") {

			std::cout << "Please enter name of motion queue: 	" << std::endl;
			std::string tempQueue;
			std::cin >> ws;
			std::getline(std::cin, tempQueue);

			while (!motorController.setMotionQueue(tempQueue))
			{
				std::cout << "Queue was not recognized, please try another" << std::endl;
				std::getline(std::cin, tempQueue);
				//MotionTesting = true;
			}

			if(!motorController.setMotion(motorController.getCurrentMotionName()))
			{
				std::cout << "This queue is empty." << std::endl;
				done= true;
			}

			int queueLength = motorController.getQueueLength();
			int queueCountdown = queueLength;


			executingQueue = true;
			//MotionTesting= true;


			while(executingQueue){
				if(queueCountdown==0){
					executingQueue = false;
				}
				else if(motorController.step(false)){	//if this motion is over, go to the next one
					motorController.incrementQueueIndex();
					motorController.setMotion(motorController.getCurrentMotionName());	//set it to the next motion in the queue
					clock_t startTime = clock();
					while ((((float)clock() - (float)startTime)/CLOCKS_PER_SEC) < motorController.getQueuePause());//index has already been incremented, hence the -1
					queueCountdown--;
				}
			}
		}

		else {
			while (!motorController.setMotion(input))
			{
				std::cout << "That motion was not recognized, please try another" << std::endl;
				std::getline(std::cin, input);
				//MotionTesting = true;
			}
		}
		clock_t startTime = clock();

		if (!MotionTesting) {
			while ((((float)clock() - (float)startTime)/CLOCKS_PER_SEC) < 7 && !done) {
				motorController.step(false);
			}
		}
		MotionTesting = false;

	} while (!done);

	return 0;
}

void motionTesting() {
	bool FinishMotionTest= false;
	bool motionTestingDone = false;
	while(!FinishMotionTest) {

		motorController.lockHead();
		motionTestingDone = false;

		std::cout << "\n####  MOTION TESTING  ####" << std::endl;
		std::cout << "\n1-Enter motion name OR choose from the options below:" << std::endl;
		cout << "\nL= New Motion\nMQ= New Motion Queue\nMM= Mirror Motion\nX = exit " << std::endl;


		std::cin >> ws;
		getline(std::cin, input);

		if (motorController.setMotion(input)){
			// We don't have to do anything, since the motion is already set
		}

		else if (input == "S" || input == "s")
		{
			motorController.setMotion("Stand");
		}

		else if (input == "z") {
			std::cout << "Which Motion would you like to print? ";
			std::string motion;
			std::cin >> ws;
			std::getline(std::cin, motion);
			while(!motorController.setMotion(motion)) {
				std::cout << "Could not find motion with that name. Please try again: ";
				std::cin >> ws;
				std::getline(std::cin, motion);
			}
			motorController.printMotion(1);
		}
		else if (input == "X" || input == "x"){
			return;
		}
		else if (input == "L") {
			createNewMotion();
			motionTestingDone = true;
		}
		else if (input == "MQ"){
			createNewMotionQueue();
			motionTestingDone = true;
		}
		else if (input == "MM"){
			std::cout<<"Which motion would you like to mirror?"<<std::endl;
			std::string motion;
			std::cin >> ws;
			std::getline(std::cin, motion);

			while(!motorController.setMotion(motion)) {
				std::cout << "Could not find motion with that name. Please try again: ";
				std::cin >> ws;
				std::getline(std::cin, motion);
			}

			motorController.mirrorCurrentMotion();
		}

		else{
			cout << "Please enter a valid input" << endl;
			motionTestingDone = true;
		}


		while (!motionTestingDone) {
			std::cout << "\nn ------> Next step\tf ------> Finish the current motion"<<std::endl;
			std::cout << "i ------> Insert new step after current step"<<std::endl;
			std::cout << "l ------> Last step  "<<std::endl;
			std::cout << "aj ------> Add Joints  "<<std::endl;
			std::cout << "pid ------> Change P gain  "<<std::endl;
			std::cout << "cmp ------> Set compliancy for limbs"<<std::endl;
			//std::cout << "d -> Delete current step"<<std::endl;
			std::cout << "pm ------> Passive Mode (robot will not move)  "<<std::endl;
			std::cout << "am ------> Active Mode (robot is active) "<<std::endl;

			std::cout << "\n";
			std::cout << "r ------> Release *SINGLE* Motor   \trll -----> Release Left Leg\tell -----> Enable Left Leg " << std::endl;
			std::cout << "ra -----> Release *ALL* Motors     \trrl -----> Release Right Leg\terl -----> Enable Right Leg " << std::endl;
			std::cout << "e ------> Engage *SINGLE* Motor    \trla -----> Release Left Arm\tela -----> Enable Left Arm " << std::endl;
			std::cout << "ea -----> Engage *ALL* Motors      \trra -----> Release Right Arm\tera -----> Enable Right Arm " << std::endl;
			std::cout << "rs -----> Release Sway Motors (hip shifting motors)" << std::endl;
			std::cout << "\n";

			std::cout << "rp -----> Read Position " << std::endl;
			std::cout << "gm -----> Get Mirrored Pose (Must do this FIRST!!)" << std::endl;
			std::cout << "sm -----> Set Mirrored Pose (always do gm FIRST!!) " << std::endl;
			std::cout << "shift+t -----> Edit Step time (speed) " << std::endl;
			std::cout << "shift+s -----> Save Positions step " << std::endl;
			std::cout << "shift+p -----> Print current Motion " << std::endl;

			motorController.displayMotionStatus();
			cin >> input;



			if (input == "n" || input == "N") {

				/**
				 *  If the incrementStep function returns false, then the while loop continues, and waits for user input.
				 *  If it returns true, that means the motion has no more steps and the while loop exits
				 */
				//motorController.incrementIndex();
				motionTestingDone= motorController.incrementStep();
				//motorController.decrementIndex();
				if(motionTestingDone==true) {
					bool validInput = false;
					while (!validInput) {
						std::cout<<"Motion is over. Test another motion?  (Y/N)"<<std::endl;
						motorController.enableMotionExecution();	//this way if you exit motion testing the robot will be active by defualt
						cin >> input;
						if(input == "Y" || input == "y") {
							FinishMotionTest= false;
							validInput = true;
						}
						else if(input == "N" || input == "n") {
							FinishMotionTest= true;
							validInput = true;
						}
						else {
							std::cout << "Please enter a valid input" << std::endl;
						}
					}
				}
			}

			else if (input == "l" || input == "L") {

				motorController.executePrevious();
			}
			else if (input == "i" || input == "I") {

				motorController.insertStep();
				motorController.incrementIndex();

			}
			//			else if (input == "d" || input == "D") {
			//				motorController.decrementIndex();
			//				motorController.deleteCurrentStep();
			//				motorController.incrementIndex();
			//				motorController.recalculateCurrentMotionSpeeds();
			//
			//			}
			else if (input == "pm" || input == "PM") {
				motorController.disableMotionExecution();
			}
			else if (input == "am" || input == "AM") {
				motorController.enableMotionExecution();
			}
			else if (input == "pid" || input == "PID") {
				//release single motor
				std::cout <<"Enter P gain value 0-32:  ";
				std::string tempinput;
				cin >> tempinput;
				int pid = 0;
				std::stringstream(tempinput) >> pid;
				motorController.changePID(pid);
			}
			else if (input == "cmp" || input == "CMP") {

				std::string tempinput;
				std::cout <<"Enter STEP:  ";
				cin >> tempinput;
				int step = 0;
				std::stringstream(tempinput) >> step;

				std::cout <<"Enter MOTOR NUMBER:  ";
				cin >> tempinput;
				int motorID = 0;
				std::stringstream(tempinput) >> motorID;

				std::cout <<"Enter compliance (0= no resist   1023= max resist):  ";
				cin >> tempinput;
				int compliancy = 1023;
				std::stringstream(tempinput) >> compliancy;
				motorController.chooseCompliantLimb(step, motorID, compliancy);

			}
			else if (input == "r" || input == "R") {
				//release single motor
				std::cout <<"Which motor would you like to release? ";
				std::string tempinput;
				cin >> tempinput;
				int motor = 0;
				std::stringstream(tempinput) >> motor;
				motorController.disableMotor(motor);
			}
			else if (input == "aj" || input == "AJ") {
				//release single motor
				std::cout <<"1) Add Arms\t2)Add Head ";
				std::string tempinput;
				cin >> tempinput;
				int joint = 0;
				std::stringstream(tempinput) >> joint;
				switch(joint){
				case 1:
					motorController.setLimbSelection(9);//corresponds to case for adding arms
					break;
				case 2:
					motorController.setLimbSelection(10);//corresponds to case for adding head
					break;
				}

			}
			else if (input == "ra" || input == "RA") {
				//release all motors
				motorController.disableAllMotors();
			}
			else if (input == "e" || input == "E") {
				//engage single motor
				std::cout <<"Which motor would you like to engage? ";
				std::string tempinput;
				cin >> tempinput;
				int motor;
				std::stringstream(tempinput) >> motor;
				motorController.enableMotor(motor);
			}
			else if (input == "ea" || input == "EA") {
				//engage all motors
				motorController.enableAllMotors();

			}
			else if (input == "f" || input == "F") {
				clock_t startTime = clock();
				while ((((float)clock() - (float)startTime)/CLOCKS_PER_SEC) < 5) {
					motorController.step(false);
				}
				motionTestingDone = true;
				motorController.enableMotionExecution();	//put robot back in active mode after motion by default
			}
			else if (input == "erl" || input == "ERL") {
				motorController.enableRightLeg();
			}
			else if (input == "ell" || input == "ELL") {
				motorController.enableLeftLeg();
			}
			else if (input == "rrl" || input == "RRL") {
				motorController.disableRightLeg();
			}
			else if (input == "rll" || input == "RLL") {
				motorController.disableLeftLeg();
			}
			else if (input == "rs" || input == "RS") {
				motorController.disableSwayMotors();
			}


			else if (input == "era" || input == "ERA") {
				motorController.enableRightArm();
			}
			else if (input == "ela" || input == "ELA") {
				motorController.enableLeftArm();
			}
			else if (input == "rra" || input == "RRA") {
				motorController.disableRightArm();
			}
			else if (input == "rla" || input == "RLA") {
				motorController.disableLeftArm();
			}


			else if (input == "rp" || input == "RP") {
				std::string tempinput;
				int motor;
				std::cout << "Which motor would you like to read? ";
				std::cin >> tempinput;
				std::stringstream(tempinput)>>motor;
				std::cout << motorController.readMotorPosition(motor) << std::endl;
			}
			else if (input == "gm" || input == "GM") {
				motorController.getMirroredPose();
			}
			else if (input == "sm" || input == "SM") {
				motorController.setMirroredPose();
			}
			else if (input == "T") {
				std::string getTime;
				double time;
				std::cout << "How long should this step take?  ";
				std::cin >> getTime;
				std::stringstream(getTime)>>time;
				motorController.changeTime(time);

			}
			else if (input == "S") {
				std::string getTime;
				double time;
				std::cout << "How long should this step take?  ";
				std::cin >> getTime;
				std::stringstream(getTime)>>time;
				motorController.setTime(time);
				//motorController.recalculateCurrentMotionSpeeds();

			}

			else if (input == "P") {
				std::string printOption;
				int printDecision;
				std::cout << "Print to file or console? (f/c) "<<std::endl;
				std::cin >> printOption;

				if(printOption=="c" ||printOption=="C"){
					printDecision= 1;
				}
				if(printOption=="f" ||printOption=="F"){
					printDecision= 2;
				}


				motorController.printMotion(printDecision);
			}

			else {
				std::cout << "Please enter a valid input" << std::endl;
			}
		}
	}
}

void createNewMotion() {
	std::cout << "What would you like this motion to be called?\n   ";
	std::string tempAnimation;
	std::cin >> ws;
	std::getline(std::cin, tempAnimation);

	bool done = false;
	while (!done) {
		done = motorController.addMotion(tempAnimation);
		if (done == false) {
			std::cout << "That name is already taken, please choose another\n   ";
			std::getline(std::cin, tempAnimation);
		}
	}

	std::cout << "\nSelect limbs: \n";
	std::cout << "1) Head and Arms [1-18]"<<std::endl;
	std::cout << "2) Legs [1-12]"<<std::endl;
	std::cout << "3) Right Leg [1-12 odd]    4) Left Leg [1-12 even]"<<std::endl;
	std::cout << "5) Arms [13-18]"<<std::endl;
	std::cout << "6) Right Arm [13-18 odd]   7) Left Arm [13-18 even]"<<std::endl;
	std::cout << "8) Head [23-24]" <<std::endl;
	std::string tempinput;
	int limb_select = 0;
	bool validSelection= false;
	while(!validSelection){
		cin >> tempinput;
		std::stringstream(tempinput) >> limb_select;
		if(limb_select>0 && limb_select<9){
			validSelection= true;
		}
		else{
			std::cout << "\nPlease enter a valid input: \n";
			std::cout << "1) Head and Arms [1-18]"<<std::endl;
			std::cout << "2) Legs [1-12]"<<std::endl;
			std::cout << "3) Right Leg [1-12 odd]    4) Left Leg [1-12 even]"<<std::endl;
			std::cout << "5) Arms [13-18]"<<std::endl;
			std::cout << "6) Right Arm [13-18 odd]   7) Left Arm [13-18 even]"<<std::endl;
			std::cout << "8) Head [23-24]" <<std::endl;
		}
	}
	motorController.setLimbSelection(limb_select);


	int stepTracker=0;
	while (true) {
		if(stepTracker==0){
			std::cout <<"\nYOU ARE NOW CREATING A NEW MOTION!"<<std::endl;
			std::cout <<"\n 1. Move the robot into a pose... "<<std::endl;
			std::cout <<"\n 2. engage the motors using 'ea' "<<std::endl;
			std::cout <<"\n 3. save the step using 'shift+s' "<<std::endl;
			std::cout <<"\n 4. move on to the next step using 'n' "<<std::endl;
			std::cout <<"\n 5. save the whole motion using 'shift+p' and then 'f' "<<std::endl;

			std::cout <<"\n Current Step = 0 \n"<<std::endl;

			std::cout << "r ------> Release *SINGLE* Motor  "<<std::endl;
			std::cout << "ra -----> Release *ALL* Motors  "<<std::endl;
			std::cout << "e ------> Engage *SINGLE* Motor  "<<std::endl;
			std::cout << "ea -----> Engage *ALL* Motors  "<<std::endl;
			std::cout << "rl -----> Release Left Leg " << std::endl;
			std::cout << "rr -----> Release Right Leg " << std::endl;
			std::cout << "rs -----> Release Sway Motors (hip shifting motors)" << std::endl;
			std::cout << "er -----> Enable Right Leg " << std::endl;
			std::cout << "el -----> Enable Left Leg " << std::endl;
			std::cout << "rp -----> Read Position " << std::endl;
			std::cout << "gm -----> Get Mirrored Pose (Must do this FIRST!!)" << std::endl;
			std::cout << "sm -----> Set Mirrored Pose (always do gm FIRST!!) " << std::endl;
			std::cout << "shift+s -----> Save Positions step " << std::endl;
			std::cout << "shift+p -----> Print current Motion " << std::endl;
			std::cout << "d -----> Done editing motion" << std::endl;
			std::cout << "n ------> Add new step (after step 0 has been saved)"<<std::endl;
		}
		else{
			std::cout <<"\n Current Step = "<<stepTracker<< "\n"<<std::endl;

			std::cout << "n ------> Create a new step "<<std::endl;
			std::cout << "r ------> Release *SINGLE* Motor  "<<std::endl;
			std::cout << "ra -----> Release *ALL* Motors  "<<std::endl;
			std::cout << "e ------> Engage *SINGLE* Motor  "<<std::endl;
			std::cout << "ea -----> Engage *ALL* Motors  "<<std::endl;
			std::cout << "rl -----> Release Left Leg " << std::endl;
			std::cout << "rr -----> Release Right Leg " << std::endl;
			std::cout << "rs -----> Release Sway Motors (hip shifting motors)" << std::endl;
			std::cout << "er -----> Enable Right Leg " << std::endl;
			std::cout << "el -----> Enable Left Leg " << std::endl;
			std::cout << "rp -----> Read Position " << std::endl;
			std::cout << "gm -----> Get Mirrored Pose (Must do this FIRST!!)" << std::endl;
			std::cout << "sm -----> Set Mirrored Pose (always do gm FIRST!!) " << std::endl;
			std::cout << "shift+s -----> Save Positions step " << std::endl;
			std::cout << "shift+p -----> Print current Motion " << std::endl;
			std::cout << "d -----> Done editing motion" << std::endl;
		}

		cin >> input;


		if (input == "n" || input == "N") {
			// Create a new step
			motorController.addMotionStep();
			stepTracker++;

		}

		else if (input == "r" || input == "R") {
			//release single motor
			std::cout <<"Which motor would you like to release? ";
			std::string tempinput;
			cin >> tempinput;
			int motor = 0;
			std::stringstream(tempinput) >> motor;
			motorController.disableMotor(motor);
		}
		else if (input == "ra" || input == "RA") {
			//release all motors
			motorController.disableAllMotors();
		}
		else if (input == "e" || input == "E") {
			//engage single motor
			std::cout <<"Which motor would you like to engage? ";
			std::string tempinput;
			cin >> tempinput;
			int motor;
			std::stringstream(tempinput) >> motor;
			motorController.enableMotor(motor);
		}
		else if (input == "ea" || input == "EA") {
			//engage all motors
			motorController.enableAllMotors();

		}
		else if (input == "er" || input == "ER") {
			motorController.enableRightLeg();
		}
		else if (input == "el" || input == "EL") {
			motorController.enableLeftLeg();
		}
		else if (input == "rr" || input == "RR") {
			motorController.disableRightLeg();
		}
		else if (input == "rl" || input == "RL") {
			motorController.disableLeftLeg();
		}
		else if (input == "rs" || input == "RS") {
			motorController.disableSwayMotors();
		}
		else if (input == "rp" || input == "RP") {
			std::string tempinput;
			int motor;
			std::cout << "Which motor would you like to read? ";
			std::cin >> tempinput;
			std::stringstream(tempinput)>>motor;
			std::cout << motorController.readMotorPosition(motor) << std::endl;
		}
		else if (input == "gm" || input == "GM") {
			motorController.getMirroredPose();
		}
		else if (input == "sm" || input == "SM") {
			motorController.setMirroredPose();
		}
		else if (input == "S") {
			std::string getTime;
			double time;
			std::cout << "How long should this step take?  ";
			std::cin >> getTime;
			std::stringstream(getTime)>>time;
			motorController.incrementIndex();
			motorController.setTime(time);
			motorController.decrementIndex();



		}
		else if (input == "P") {
			std::string printOption;
			int printDecision;
			std::cout << "Print to file or console? (f/c) "<<std::endl;
			std::cin >> printOption;

			if(printOption=="c" ||printOption=="C"){
				printDecision= 1;
			}
			if(printOption=="f" ||printOption=="F"){
				printDecision= 2;
			}


			motorController.printMotion(printDecision);
		}
		else if (input == "d" || input == "D") {
			//motorController.printMotion(2);
			return;
		}

		else {
			std::cout << "Please enter a valid input\t" << std::endl;
		}
	}
}

void createNewMotionQueue() {

	bool done = false;
	while (!done) {
		std::cout << "What would you like this motion queue to be called?\n   ";
		std::string tempQueue;
		std::cin >> ws;
		std::getline(std::cin, tempQueue);
		std::cout << "reaf the name \n  ";
		done = motorController.addMotionQueue(tempQueue);
		std::cout << "added the queue\n ";
		if (done == false) {
			std::cout << "That name is already taken, please choose another\n   ";
			std::getline(std::cin, tempQueue);
		}
	}

	int stepTracker=0;
	while (true) {
		if(stepTracker==0){
			std::cout <<"\nYOU ARE NOW CREATING A NEW MOTION QUEUE!"<<std::endl;
			std::cout <<"\n 1. Enter the name of the first motion "<<std::endl;
			std::cout <<"\n 2. Enter the pause time "<<std::endl;
			std::cout <<"\n 3. Enter number of repeats "<<std::endl;
			std::cout <<"\n 4. move on to the next step using 'n' "<<std::endl;
			std::cout <<"\n 5. save the whole motion using 'shift+p' and then 'f' "<<std::endl;

			std::cout <<"\n Current Motion = 0 \n"<<std::endl;

			std::cout << "Enter n to begin:"<<std::endl;
		}
		else{
			std::cout <<"\n Current Motion = "<<stepTracker-1<< "\n"<<std::endl;

			std::cout << "n ------> Add next Motion "<<std::endl;

			std::cout << "shift+p -----> Print current Motion " << std::endl;
			std::cout << "d -----> Done editing motion" << std::endl;
		}

		cin >> input;

		if (input == "n" || input == "N") {
			// Add new Motion
			std::cout << "Which motion would you like to add?  ";
			std::string temp;
			std::cin >> temp;
			motorController.addMotionToQueue(temp); //this function increments the
			motorController.incrementQueueIndex();

			std::string getTime;
			double time;
			std::cout << "Enter the post-motion pause time:  ";
			std::cin >> getTime;
			std::stringstream(getTime)>>time;
			std::cout<< "There will be a pause of "<<time<<" seconds after this motion"<<std::endl;
			motorController.incrementQueueIndex();
			motorController.setPauseTime(time);
			motorController.decrementQueueIndex();
			stepTracker++;
		}

		else if (input == "P") {
			motorController.printMotionQueue();
		}
		else if (input == "d" || input == "D") {

			return;
		}

		else {
			std::cout << "Please enter a valid input\t" << std::endl;
		}
	}
}
