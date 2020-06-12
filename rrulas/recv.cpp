#include "rrulas.h"

extern int set_k;
extern int get_force_mode;

using namespace std;
// command 
char cmd[128] = {0};
char cnt_rob[30] = "connect_robot";
char dnt_rob[30] = "disconnect_robot";
char cnt_msc_epm[30] = "connect_muscle_equipment";
char dnt_msc_epm[30] = "disconnect_muscle_equipment";
char force_mode_zero[30] = "zero";
char force_mode_ea[30] = "EA";
char c_s_k[30] = "change_set_k";

char buff_epm[60] = {0};
char buff_sk[12] = {0};

cs_t setting_change;

// judge sign
int CR, CME=0;
int robot_control_sign = 0, muscle_control_sign = 0;

pthread_t tid_rrulas,tid_get_muscle_data;

void start_robot(int sfd)
{
	
	if(robot_control_sign == 0){
		if (strcmp(cmd,force_mode_zero) == 0){
			get_force_mode = 4;
			pthread_create(&tid_rrulas, NULL, rrulas, NULL);
			cout<<"robot start successfully!"<<endl;
			robot_control_sign = 1;
			return;
		}else if(strcmp(cmd,force_mode_ea) == 0){
			get_force_mode = 3;
			send(sfd,"input k setting",15,0);
			pthread_create(&tid_rrulas, NULL, rrulas, NULL);
			cout<<"robot start successfully!"<<endl;
			robot_control_sign = 1;
			return;
		}
		
	}else if(strcmp(cmd,c_s_k) == 0){
			cout<<"receive change set_k!"<<endl;
			bzero(buff_sk,sizeof(buff_sk));
			int ret = recv(sfd,buff_sk,sizeof(buff_sk),MSG_WAITALL);
			if (ret <= 0){
				printf("can not recive set_k\n");
				return;
			}
			memset(&setting_change, 0, sizeof(setting_change));
			memcpy(&setting_change, buff_sk, sizeof(setting_change));
			if (setting_change.flag == 1)
				set_k = setting_change.value;
			cout<<"receive change set_k successful!"<<endl;
			return;
	}else{
			cout<<"robot has already started!"<<endl;
	}		

	
}

void stop_robot()
{
	if (robot_control_sign == 1){
		pthread_cancel(tid_rrulas);
		cout<<"robot stop successfully"<<endl;
		robot_control_sign = 0;
	}else{
		cout<<"robot dose not start or can not stop!"<<endl;
	}
}

void start_muscle_equipment(int sfd){
	if (muscle_control_sign == 0){
		menv_t muscle_setting;
		bzero(buff_epm,sizeof(buff_epm));
		int ret = recv(sfd,buff_epm,sizeof(buff_epm),0);
		if (ret <= 0){
			printf("can not recive muscle data setting\n");
			return;
		}
		memset(&muscle_setting, 0, sizeof(muscle_setting));
		memcpy(&muscle_setting, buff_epm, sizeof(buff_epm));
		pthread_create(&tid_get_muscle_data, NULL, get_muscle_data, &muscle_setting);//muscle_setting
		cout<<"muscle equipment start successfully!"<<endl;
		muscle_control_sign = 1;
	}else{
		cout<<"muscle equipment has already started!"<<endl;
	}
}

void stop_muscle_equipment()
{
	if(muscle_control_sign == 1){
		pthread_cancel(tid_get_muscle_data);
		cout<<"muscle equipment stop successfully"<<endl;
		muscle_control_sign = 0;
	}else{
		cout<<"muscle equipment dose not start or can not stop!"<<endl;
	}
}


void mode_choice_control(int sfd)
{
	if (strcmp(cmd,cnt_rob) == 0 || CR == 1){
		printf("receive connect robot!\n");
		CR = 1;
		start_robot(sfd);
		if(strcmp(cmd, dnt_rob) == 0){
			cout<<"receive disconnect robot!"<<endl;
			stop_robot();
			CR = 0;
		}
	}
	if(strcmp(cmd,cnt_msc_epm) == 0 || CME == 1){
		cout<<"receive connect muscle equipment!"<<endl;
		CME = 1;
		start_muscle_equipment(sfd);
		if(strcmp(cmd, dnt_msc_epm) == 0){
			cout<<"receive disconnect muscle equipment!"<<endl;
			stop_muscle_equipment();
			CME = 0;
		}
	}
	return;
}

void *recv_info_up(void *p)
{
	
	int sfd = *((int *)p);
	int ret;
	
	while(1)
	{
		bzero(cmd,128);
		ret = recv(sfd, cmd, 128, 0);
		if(ret <= 0)
			break;
		printf("%s\n",cmd);
		mode_choice_control(sfd);
	}
	printf("client end\n");
	exit(0);
}
