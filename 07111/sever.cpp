#include "rrulas.h"

using namespace std;
//sem_t sem1;
//sem_t sem1,sem2;


int main()
{
	pthread_t tid_send_up, tid_recv_up;
    int sockfd,newfd;
    int ret;
	pid_t pid;
	struct sockaddr_in server_addr;
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0){
		perror("sockfd()");
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9999);
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.158");
	
	ret = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(ret < 0){
		perror("bind");
		return -1;
	}
	if(listen(sockfd, 5) < 0){
		perror("listen()");
		return -1;
	}

	printf("waiting client connect!\n");
	for(;;){
		newfd = accept(sockfd, NULL, NULL);
		printf("after accept\n");
		if(newfd < 0)
			break;
		if((pid = fork()) < 0){
			perror("fork()");
			exit(-1);
		}else if(fork() == 0){
			printf("connected\n");
			//sem_init(&sem1, 0, 0);
			//sem_init(&sem2, 0, 0);
			pthread_create(&tid_send_up, NULL, send_info_up, &newfd);
			pthread_create(&tid_recv_up, NULL, recv_info_up, &newfd);
			
			
			
			//pthread_create(&tid_get_muscle_data, NULL, get_muscle_data, NULL);
			//pthread_create(&tid_rrulas, NULL, rrulas, NULL);
			
			//pthread_join(tid_send_up, NULL);
			//pthread_join(tid_recv_up, NULL);
		}
	}

    return 0;
}
