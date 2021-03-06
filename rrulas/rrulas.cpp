/*
This code is written by Mads Johan Laastad as part of a master thesis in Cybernetics and Robotics at the Norwegian University of Science and Technology (NTNU) during the spring of 2017. The author can not guarrantee for the safety of anyone that desides to use this code in their own projects. This module is named 'rrulas' after the title of the master thesis it is part of, 'rrulas' is an acronyme for "Robotic Rehabilitation of Upper-Limb After Stroke". Feel free to contact the autor at laastad.m@gmail.com if you have any questions.

Thank you to Thomas Timm Andersson for guidence and for sharing his work on a new driver for Universal Robots, ur_modern_driver, with the robotic community. Thank you to Hakan Yildrim for sharing the code from another master thesis titled "Robot Control using Vision and Force Sensors" written by Hakan Yildrim written during the spring of 2016. Parts of the code in this master thesis is based on their preliminary work.

NETWORK DETAILS:
The IP address can be found in the PolyScope interface (tablet) of the robot.
SETUP Robot -> Setup NETWORK (requires password: "ngr12") -> IP address
ur3_IP = "10.42.0.63"
ur3_HOSTNAME = 'ur-2012208983' #requires dns.
F/T_SENSOR_IP = "10.42.0.8"
*/

#include "rrulas.h"

extern int get_force_mode;
extern int set_k;

//std::condition_variable rt_msg_cond_;
//std::condition_variable msg_cond_;
//UrDriver ur3(rt_msg_cond_, msg_cond_,"10.193.0.45",0);
//std::mutex msg_lock;
//std::unique_lock<std::mutex> locker(msg_lock);
//double q_start[6] = {-0.9100, -1.9109, -2.5350, -0.2569, 1.5920, 6.2692};//start position for 111111

void getq(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double q[6])
{
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
	rt_msg_cond_->wait(locker);
	}
	std::vector<double> q_vector = ur3->rt_interface_->robot_state_->getQActual();
	std::copy(q_vector.begin(), q_vector.end(), q);
	ur3->rt_interface_->robot_state_->setDataPublished();
}

void RobotWait(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double pose_target[6])
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	double startTime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	double goal_tol = 0.0075;  
	while(1)
	{
	gettimeofday(&tp, NULL);
	double timeStamp = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	double elapsTime = (timeStamp-startTime)/1000;
	  
		std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
		rt_msg_cond_->wait(locker);
	}
		ur3->rt_interface_->robot_state_->setDataPublished();
		std::vector<double> q_actual = ur3->rt_interface_->robot_state_->getQActual();
	if(((fabs(q_actual[0] - pose_target[0])) < goal_tol) &&
	   ((fabs(q_actual[1] - pose_target[1])) < goal_tol) &&
	   ((fabs(q_actual[2] - pose_target[2])) < goal_tol) &&
	   ((fabs(q_actual[3] - pose_target[3])) < goal_tol) &&
	   ((fabs(q_actual[4] - pose_target[4])) < goal_tol) &&
	   ((fabs(q_actual[5] - pose_target[5])) < goal_tol)) break;
	   
	if(elapsTime > 15)
	{
	  std::cout << "RobotWait is stuck - breaking out!" << std::endl;
	  break;
	}
	}
	usleep(1000000);
}

void moveSimpleJoint(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], int angle_offset, double v, double a)
{
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
	rt_msg_cond_->wait(locker);
	}
	ur3->rt_interface_->robot_state_->setDataPublished();
	
	double q[6];  
	getq(ur3, rt_msg_cond_, q);
	double qGoal[6];
	getnearest(T06, q, qGoal);
	qGoal[5] = qGoal[5]-((angle_offset+TCP_OFFSET)*M_PI/180);
	char TargetString[200];
	sprintf(TargetString, "movej([%.4f, %.4f, %.4f, %.4f, %.4f, %.4f], %.4f, %.4f)\n",
		qGoal[0], qGoal[1], qGoal[2], qGoal[3], qGoal[4], qGoal[5], v, a);
	ur3->rt_interface_->addCommandToQueue(TargetString);  
	RobotWait(ur3, rt_msg_cond_, qGoal);
}

void moveSimpleJointDirect(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double qGoal[6], double v, double a)
{
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
		rt_msg_cond_->wait(locker);
	}
	ur3->rt_interface_->robot_state_->setDataPublished();
	
	char TargetString[200];
	sprintf(TargetString, "movej([%.4f, %.4f, %.4f, %.4f, %.4f, %.4f], %.4f, %.4f)\n",
		qGoal[0], qGoal[1], qGoal[2], qGoal[3], qGoal[4], qGoal[5], v, a);
	ur3->rt_interface_->addCommandToQueue(TargetString);  
	RobotWait(ur3, rt_msg_cond_, qGoal);
}

void moveSimpleCart(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], int angle_offset, double v, double a)
{
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
	rt_msg_cond_->wait(locker);
	}
	ur3->rt_interface_->robot_state_->setDataPublished();
	
	double q[6];
	getq(ur3, rt_msg_cond_, q);
	double qGoal[6];
	getnearest(T06, q, qGoal);
	qGoal[5] = qGoal[5]-((angle_offset+TCP_OFFSET)*M_PI/180);
	char TargetString[200];
	sprintf(TargetString, "movel([%.4f, %.4f, %.4f, %.4f, %.4f, %.4f], %.4f, %.4f)\n",
		qGoal[0], qGoal[1], qGoal[2], qGoal[3], qGoal[4], qGoal[5], v, a);
	ur3->rt_interface_->addCommandToQueue(TargetString); 
	RobotWait(ur3, rt_msg_cond_, qGoal);
}

void moveSimpleCartDirect(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], double v, double a)
{
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	while (!ur3->rt_interface_->robot_state_->getDataPublished())
	{
	rt_msg_cond_->wait(locker);
	}
	ur3->rt_interface_->robot_state_->setDataPublished();
	
	double q[6];
	getq(ur3, rt_msg_cond_, q);
	double qGoal[6];
	getnearest(T06, q, qGoal);	
	char TargetString[200];
	sprintf(TargetString, "movel([%.4f, %.4f, %.4f, %.4f, %.4f, %.4f], %.4f, %.4f)\n",
		qGoal[0], qGoal[1], qGoal[2], qGoal[3], qGoal[4], qGoal[5], v, a);
	ur3->rt_interface_->addCommandToQueue(TargetString); 
	RobotWait(ur3, rt_msg_cond_, qGoal);
} 

void introductionProcedure(UrDriver *ur3, std::condition_variable *rt_msg_cond_, int *force_mode, double *user_parameters, double *force_threshold, double *torque_threshold)
{	
	//DEFAULT VALUES
	for (int j=0; j<6; j++)
	{
		user_parameters[j] = 0; //Fx, Fy, Fz, Tx, Ty, Tz
	}
	char user_status;
	
	while(user_status != 'y' || user_status != 'Y')
	{
		std::cout << std::endl;
		std::cout << "========== ROBOTIC REHABILITATION OF UPPER-LIMB AFTER STROKE - 'rrulaf' ==========" << std::endl;
		std::cout << std::endl;
		std::cout << "Currently avaliable force modes are: " << std::endl;
		std::cout << "1. Compliance mode" << std::endl;
		std::cout << "2. Buoyancy mode" << std::endl;
		std::cout << "3. Random mode" << std::endl;  //EA
		std::cout << "4. Resistance mode" << std::endl; //ZERO
		std::cout << "5. 2-plane mode" << std::endl;
		std::cout << std::endl;
		std::cout << "Please enter the desired force mode. : ";
		//std::cin >> *force_mode; //changed by leo 200605
		//*force_mode = 4;
		//
		if(*force_mode != 1 || *force_mode != 2 || *force_mode != 3 || *force_mode != 4 || *force_mode != 5)
		{
			std::cout << "ERROR: Invalid input --> please enter a valid mode..." << std::endl;
			std::cin.clear();
		}
		
		if (*force_mode == 2) //Buoyancy mode
		{
			std::cout << "========== BUOYANCY MODE ==========" << std::endl;
			std::cout << "Please enter a desired buoyancy [N] between 0-25 Newtons. : ";
			std::cin >> user_parameters[1];
			if (user_parameters[1] < 0 || user_parameters[1] > 25)
			{
				std::cout << std::endl;
				std::cout << "ERROR: Invalid input --> value(s) defaults to 0 ..." << std::endl;
				std::cout << std::endl;
				user_parameters[1] = 0;
			}
		}
		if (*force_mode == 3) //Random mode, random torque disabled due to safety conserns
		{
			std::cout << "========== RANDOM MODE ==========" << std::endl;
			std::cout << "Please enter a desired disturbance scale between 0-100[%] for all axis." << std::endl;
			std::cout << "Fx [space] Fy [space] Fz [enter] : ";// Tx [space] Ty [space] Tz [enter] : ";
			
			//std::cin >> user_parameters[0] >> user_parameters[1] >> user_parameters[2]; //user_parameters[3] >> user_parameters[4] >> user_parameters[5];
			
			user_parameters[0] = 70;
			user_parameters[1] = 70;
			user_parameters[2] = 70;
			
			for (int j=0; j<6; j++)
			{
				if (user_parameters[j] < 0 || user_parameters[j] > 100)
				{
					std::cout << std::endl;
					std::cout << "ERROR: Invalid input --> value(s) defaults to 0 ..." << std::endl;
					std::cout << std::endl;
					user_parameters[j] = 0;
				}
				else
				{
					user_parameters[j] /= 100;
				}
			}
		}
		if (*force_mode == 4) //Resistance mode
		{
			std::cout << "========== RESISTANCE MODE ==========" << std::endl;
			std::cout << "Please enter a desired resistance scale between 0-100[%] for all axis." << std::endl;
			std::cout << "Fx [space] Fy [space] Fz [space] Tx [space] Ty [space] Tz [enter] : ";
			//changed by leo 200605
			//std::cin >> user_parameters[0] >> user_parameters[1] >> user_parameters[2] >> user_parameters[3] >> user_parameters[4] >> user_parameters[5];
			user_parameters[0] = 70;
			user_parameters[1] = 70;
			user_parameters[2] = 70;
			//
			for (int j=0; j<6; j++)
			{
				if (user_parameters[j] < 0 || user_parameters[j] > 100)
				{
					std::cout << std::endl;
					std::cout << "ERROR: Invalid input --> value(s) defaults to 0 ..." << std::endl;
					std::cout << std::endl;
					user_parameters[j] = 0;
				}
				else
				{
					user_parameters[j] /= 100;
				}
			}
		}
		if (*force_mode == 5) //2-plane mode
		{
			std::cout << "========== 2-PLANE MODE ==========" << std::endl;
			std::cout << "Move the end-effector to the desired staring position. The robot will be compliant for the next 10 seconds. " << std::endl;
			forceControl(ur3, rt_msg_cond_, 10, 1, user_parameters, 0, 0);
			std::cout << "Hopefully, the robot is now in the correct position." << std::endl;
		}
		
		std::cout << std::endl;
		std::cout << "Would you like to keep the current setup [y/n]? : ";
		
		//std::cin >> user_status;// change by leo 200608
		
		user_status = 'y';
		if (user_status == 'y' || user_status == 'Y')
		{
			break;
		}
	}
}





/* Read and return a reply, if sufficient bytes are present
 * (otherwise, return NULL).
 * Memory allocated by this function must be free by the caller.
 */
 

void *rrulas(void *p)
{

	// ROBOT CONNECTION
	std::condition_variable rt_msg_cond_;
	std::condition_variable msg_cond_;
	UrDriver ur3(rt_msg_cond_, msg_cond_,"10.193.0.45",0);
	ur3.start();
	std::cout << "Connecting to robot ..." << std::endl;
	std::mutex msg_lock;
	std::unique_lock<std::mutex> locker(msg_lock);
	double q_start[6] = {-0.9100, -1.9109, -2.5350, -0.2569, 1.5920, 6.2692};//start position for 111111
	std::cout << "Waiting for data ..." << std::endl;
	
	while (!ur3.rt_interface_->robot_state_->getDataPublished())
	{
		rt_msg_cond_.wait(locker);
	}	
	std::cout << "Data received!" << std::endl;


//200609
//	ur3.start();
//	std::cout << "Connecting to robot ..." << std::endl;
//
//	std::cout << "Waiting for data ..." << std::endl;
//	
//	while (!ur3.rt_interface_->robot_state_->getDataPublished())
//	{
//		rt_msg_cond_.wait(locker);
//	}	
//	std::cout << "Data received!" << std::endl;
//

	//GUI
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "======================== Welcome to the robotic rehabiliation system! ========================" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;


//auto changed by leo
//	char user_ready;

/*	200609
	while(user_ready != 'y' || user_ready != 'Y')
	{
		std::cout << "======================== WARNING! ========================" << std::endl;
		std::cout << "The robot will now move to a starting position. Make sure it is safe for the robot to operate inside the workspace. The external controller is not active. \n Is the area clear [y/n]? : ";
		//std::cin >> user_ready;    //changed by leo
		user_ready = 'y';
		//
		if (user_ready == 'y' || user_ready == 'Y')
		{
			// MOVE TO STARING POINT
			std::cout << "======================== POSITION CONTROL ACTIVE ========================" << std::endl;
			std::cout << "Moving to staring location... ";
			moveSimpleJointDirect(&ur3, &rt_msg_cond_, q_start, 1, 1);
			break;
		}
	}
*/
	
//leo
	std::cout << "======================== POSITION CONTROL ACTIVE ========================" << std::endl;
	std::cout << "Moving to staring location... ";
	moveSimpleJointDirect(&ur3, &rt_msg_cond_, q_start, 1, 1);
//

	// FORCE CONTROL
	int force_mode = get_force_mode;
	double force_threshold = 0;
	double torque_threshold = 0;
	double user_parameters[6] = {0,0,0,0,0,0}; //Fx, Fy, Fz, Tx, Ty, Tz
	//double safety_timeout = 3.5; //[s]
	double safety_timeout = 200; //[s]
	
	introductionProcedure(&ur3, &rt_msg_cond_, &force_mode, user_parameters, &force_threshold, &torque_threshold);

	std::cout << "Initializing force control... \n" << std::endl;
	pthread_t forceID;
	forceControl(&ur3, &rt_msg_cond_, safety_timeout, force_mode, user_parameters, force_threshold, torque_threshold);
	//forceTune(&ur3, &rt_msg_cond_, safety_timeout, force_threshold, torque_threshold);
	usleep(1000);
	std::cout << "Shutting down force control. \n";
	ur3.halt();
	stopFT(&forceID);
	
	std::cout << "Disconnected!\n";
	return 0;
}
