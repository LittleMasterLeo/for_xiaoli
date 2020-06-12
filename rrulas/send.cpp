#include "rrulas.h"

using namespace std;

extern sem_t sem1;

void *send_info_up(void *p)
{
	int type,ret;
	int sfd = *((int *)p);
	
	for(;;){
		send(sfd, "hello",5,0);
		char msg[1024] = "world!";
		send(sfd, msg,sizeof(msg),0);
		sleep(2);
	}
	printf("client is end\n");
	pthread_exit(0);
}
