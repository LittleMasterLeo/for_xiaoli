#include "rrulas.h"

using namespace std;

extern float emg_data_flt[];
extern sem_t sem1;

void *send_info_up(void *p)
{
//	int type,ret;
	int sfd = *((int *)p);
	
	for(;;){
		//send(sfd, "hello",5,0);
		//char msg[1024] = "world!";
		sem_wait(&sem1);
		send(sfd, emg_data_flt, SZ_DATA_EMG,0);
		sleep(1);
	}
	printf("client is end\n");
	pthread_exit(0);
}
