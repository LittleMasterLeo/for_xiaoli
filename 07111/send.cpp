#include "rrulas.h"

using namespace std;

data_t load_data;

extern float emg_data_flt[];
extern double force_data[];
extern double vector_data[];
extern int robot_data_ready;
extern int emg_data_ready;
//extern sem_t sem1;

void *send_info_up(void *p)
{
//	int type,ret;
	int sfd = *((int *)p);
	
	for(;;){
		//send(sfd, "hello",5,0);
		//char msg[1024] = "world!";
		//sem_wait(&sem1);
		memset(&load_data, 0, sizeof(load_data));
		while(emg_data_ready == 1 || robot_data_ready == 1){
			if(emg_data_ready == 1){
				memcpy(load_data.emg_data, emg_data_flt, SZ_DATA_EMG);
				load_data.emg_data_ready = 1;
				emg_data_ready = 0;
			}
			if(robot_data_ready == 1){
				memcpy(load_data.force_data, force_data, sizeof(load_data.force_data));
				memcpy(load_data.vector_data, vector_data, sizeof(load_data.vector_data));
				load_data.robot_data_ready = 1;
				robot_data_ready = 0;
			}
		}
		send(sfd, load_data, sizeof(load_data), 0);
		//send(sfd, emg_data_flt, SZ_DATA_EMG,0);
		sleep(1);
	}
	printf("send is end\n");
	pthread_exit(0);
}
