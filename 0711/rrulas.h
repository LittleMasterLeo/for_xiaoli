#include <iostream>
#include <string.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <fstream>

#include "../ur_modern_driver-master/include/ur_driver.h"
#include "../kinematics/ur_kin.h"
#include "../force/force.h"

#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <cstdio>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include<pthread.h>
#include <semaphore.h>

#define SZ_INPUT        (256)
#define SZ_DATA_EMG     (64 * 27)
#define SZ_DATA_ACC     (192 * 2)
#define SZ_DATA_IM_EMG  (64 * 15)
#define SZ_DATA_IM_AUX  (576 * 2)

#define IP htonl(INADDR_ANY)
//#define PORT htons(9999)

typedef struct muscle_env{
	char szIpAddress[29];
    char szCollectDuration[29];
    char szStartTrigger;
    char szStopTrigger;
}menv_t;

typedef struct change_setting{
	int flag;
	int mode;
	int value;
}cs_t;

// MOUNTING ANGLE OFFSET BETWEEN F/T SENSOR AND PROTOTYPE 1.1 (PHYSICAL INTERFACE BETWEEN END-EFFECTOR AND PATIENT) 
#define TCP_OFFSET 0


void getq(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double q[6]);
void RobotWait(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double pose_target[6]);
void moveSimpleJoint(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], int angle_offset, double v, double a);
void moveSimpleJointDirect(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double qGoal[6], double v, double a);
void moveSimpleCart(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], int angle_offset, double v, double a);
void moveSimpleCartDirect(UrDriver *ur3, std::condition_variable *rt_msg_cond_, double T06[16], double v, double a);
void moveTaskSpace(double q[6], double X_CHANGE, double Y_CHANGE, double Z_CHANGE, double T06_NEW[16]);
void gluInvertMatrix(const double m[16], double invOut[16]);
void rgbControl(UrDriver *ur3, std::condition_variable *rt_msg_cond_, int status);

char * readReply(int socket);
char * readReplyIfReady(int socket, int bytesExpected);
void *get_muscle_data(void *p);
void *send_info_up(void *p);
void *recv_info_up(void *p);
void *rrulas(void *p);


