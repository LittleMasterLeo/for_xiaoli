#include "rrulas.h"

using namespace std;

float emg_data_flt[432] = {0};
extern sem_t sem1;

/* Function prototypes */
//char * readReply(int socket);
char * readReply(int socket)
{
        int bytesAvail = 0;
        while (!bytesAvail && ioctl(socket, FIONREAD, &bytesAvail) >= 0)
        {
                usleep(100 * 1000); /* 100 ms */
                printf(".");
        }
        if (bytesAvail > 0)
        {
                char *tmp =(char*) malloc(bytesAvail);
                recv(socket, tmp, bytesAvail, 0);
                tmp[bytesAvail] = '\0';
                return tmp;
        }

        char *tmp = (char*) malloc(1);
        tmp[0] = '\0';
        return tmp;
}
//char * readReplyIfReady(int socket, int bytesExpected);
char * readReplyIfReady(int socket, int bytesExpected)
{
        int bytesAvail = 0;
        if (ioctl(socket, FIONREAD, &bytesAvail) >= 0 && bytesAvail < bytesExpected)
        {
                return NULL;
        }

        char *tmp = (char*) malloc(bytesAvail);
        recv(socket, tmp, bytesAvail, 0);
        tmp[bytesAvail] = '\0';
        return tmp;
}

/* Application entrypoint */
void *get_muscle_data(void *p)
{
        /* User input */
        
        
        char szIpAddress[29];
        char szStartTrigger;
        char szStopTrigger;
        char szCollectDuration[29];
        char szTmp[SZ_INPUT];
        char szTmp1[SZ_INPUT];
		
		
        /* Network communication and program state */
        in_addr_t ipAddress;
        struct sockaddr_in sinRemote;
        bool bStartTrigger, bStopTrigger, bFirstSampleAcquired;
        int iCollectDuration;
        int commSock, emgSock, accSock, imemgSock, auxSock;
        char *reply;
        int i, j;
        time_t startTime;
        time_t lastTime;
		

        /* Recieved data */
        char emgData[SZ_DATA_EMG];
        char accData[SZ_DATA_ACC];
        char imemgData[SZ_DATA_IM_EMG];
        char auxData[SZ_DATA_IM_AUX];
        char sensorType[16];
        FILE *emgDataFile;
        FILE *accDataFile;
        FILE *imemgDataFile;
        FILE *auxDataFile;
        
        /* recieved struct data */
        menv_t *muscle_setting = ((menv_t *)p);//0608

		/* Time data (create by leo) */
		char sTemp[30] = {0};

        /* Read user input */
        printf("Delsys Digital SDK Linux Demo Application\r\n");
        //printf("Enter the server IP address: ");
        //cin>>szIpAddress;
        //fgets(szIpAddress, SZ_INPUT, stdin);
        
        //printf("Start trigger (Y/N): ");
        //fgets(szStartTrigger, SZ_INPUT, stdin);
        //cin>>szStartTrigger;
       // printf("Stop trigger (Y/N): ");
        //fgets(szStopTrigger, SZ_INPUT, stdin);
        //cin>>szStopTrigger;
        //printf("Data collection duration (sec): ");
        //fgets(szCollectDuration, SZ_INPUT, stdin);
		//cin>>szCollectDuration;
      	
        /* Disable buffering */
        setbuf(stdout, NULL);

        printf("\r\nPlease wait...setting up connection.");

        /* Convert input to appropriate data types */
        //ipAddress = inet_addr(szIpAddress);
        ipAddress = inet_addr(muscle_setting->szIpAddress);
        //bStartTrigger = (szStartTrigger[0] == 'Y' || szStartTrigger[0] == 'y');
        bStartTrigger = (muscle_setting->szStartTrigger == 'Y' || muscle_setting->szStartTrigger == 'y');
        //bStopTrigger = (szStopTrigger[0] == 'Y' || szStopTrigger[0] == 'y');
        bStopTrigger = (muscle_setting->szStopTrigger == 'Y' || muscle_setting->szStopTrigger == 'y');
        //iCollectDuration = atoi(szCollectDuration);
        iCollectDuration = atoi(muscle_setting->szCollectDuration);

        /* Connect to the server */
        commSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sinRemote.sin_family = AF_INET;
        sinRemote.sin_addr.s_addr = ipAddress;
        sinRemote.sin_port = htons(50040);
        if (connect(commSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
        {
                printf("\r\nProblem connecting to server.\r\n");
                return 0;
        }

        reply = readReply(commSock);
        printf("\r\nConnected to %s", reply);
        free(reply);

        emgSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sinRemote.sin_port = htons(50041);
        if (connect(emgSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
        {
                printf("\r\nProblem connecting to server.\r\n");
                return 0;
        }

        accSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sinRemote.sin_port = htons(50042);
        if (connect(accSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
        {
                printf("\r\nProblem connecting to server.\r\n");
                return 0;
        }
        /*
		imemgSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sinRemote.sin_port = htons(50043);
        if (connect(imemgSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
        {
                printf("\r\nProblem connecting to server.\r\n");
                return 0;
        }
        */
	auxSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sinRemote.sin_port = htons(50044);
        if (connect(auxSock, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr_in)) == -1)
        {
                printf("\r\nProblem connecting to server.\r\n");
                return 0;
        }

        /* Configure the server */
        printf("Requesting data.");

        //turn off the upsampling
        sprintf(szTmp, "UPSAMPLE [ON]\r\n\r\n");

        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem starting Delsys data stream.\r\n");
                return 0;
        }

        reply = readReply(commSock);
        free(reply);

    	//set EMG+IMU mode 
	//sprintf(szTmp, "SENSOR 1 SETMODE 0\r\n\r\n");

      // if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        //{
                //printf("\r\nProblem starting Delsys data stream.\r\n");
               //return 0;
        //}
       	//reply = readReply(commSock);
        //free(reply);

	//sprintf(szTmp, "SENSOR 2 SETMODE 0\r\n\r\n");

        //if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        //{
                //printf("\r\nProblem starting Delsys data stream.\r\n");
                //return 0;
        //}

        //reply = readReply(commSock);
        //free(reply);
        //set the endian to little for Linux socket 
        //start data collection
        sprintf(szTmp, "ENDIAN LITTLE\r\n\r\n");

        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem starting Delsys data stream.\r\n");
                return 0;
        }

        reply = readReply(commSock);
        free(reply);
        sprintf(szTmp, "ENDIANNESS?\r\n\r\n");

        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem starting Delsys data stream.\r\n");
                return 0;
        }

        reply = readReply(commSock);
        free(reply);

        sprintf(szTmp, "START\r\n\r\n");

        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem starting Delsys data stream.\r\n");
                return 0;
        }

        reply = readReply(commSock);
        free(reply);

        printf("\r\nCollecting data for %d sec.", iCollectDuration);
        printf("\r\n");


		printf("getting sensors' type ");
        /* get the type of sensors*/
        for(i=0; i<16; i++)
        {
                sprintf(szTmp1, "SENSOR %d TYPE?\r\n\r\n", i+1);
                if (send(commSock, szTmp1, strlen(szTmp1), 0) == -1)
                {
                        printf("\r\nProblem starting Delsys data stream.\r\n");
                        return 0;
                }

                reply=readReply(commSock);
                sensorType[i]=*reply;
                free(reply);

        }
        

		printf("\n sensors'types completed,ready to get time\n");
		
        /* Open files to store the acquired data */
        /* files for standard sensor*/
        emgDataFile = fopen("emgData.csv", "w+");
		fprintf(emgDataFile,"Time,");    
		for (i = 0; i < 16; i++)
		{
			if (i < 15)
				fprintf(emgDataFile, "EMG%d, ", i + 1);
			else
				fprintf(emgDataFile, "EMG%d\r\n", i + 1);
		}

        accDataFile = fopen("accData.csv", "w+");
		fprintf(accDataFile,"Time,");
        for (i = 0; i < 48; i++)
        {
                char axis = ' ';
                switch (i % 3)
                {
                        case 0:
                                axis = 'X';
                                break;
                        case 1:
                                axis = 'Y';
                                break;
                        case 2:
                                axis = 'Z';
                                break;
                }
                if (i < 47)
                        fprintf(accDataFile, "ACC%c%d, ", axis, i / 3 + 1);
                else
                        fprintf(accDataFile, "ACC%c%d\r\n", axis, i / 3 + 1);
        }
        /*files for IM sensor*/
        imemgDataFile = fopen("IM_emgData.csv", "w+");
		fprintf(imemgDataFile,"Time,");    
        for (i = 0; i < 16; i++)
        {
				if (i < 15)
                        fprintf(imemgDataFile, "EMG%d, ", i + 1);
                else
                        fprintf(imemgDataFile, "EMG%d\r\n", i + 1);
        }

        auxDataFile = fopen("IM_auxData.csv", "w+");
		fprintf(auxDataFile,"Time,");    
        for (i = 0; i < 144; i++)
        {
                char axis1 = ' ';
                switch (i % 3)
                {
                        case 0:
                                axis1 = 'X';
                                break;
                        case 1:
                                axis1 = 'Y';
                                break;
                        case 2:
                                axis1 = 'Z';
                                break;
                }
                if (i < 143)
                {
                        switch(i%9)
                        {
                                case 0:
                                        fprintf(auxDataFile, "ACC%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 1:
                                        fprintf(auxDataFile, "ACC%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 2:
                                        fprintf(auxDataFile, "ACC%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 3:
                                        fprintf(auxDataFile, "GYRO%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 4:
                                        fprintf(auxDataFile, "GYRO%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 5:
                                        fprintf(auxDataFile, "GYRO%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 6:
                                        fprintf(auxDataFile, "MAG%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 7:
                                        fprintf(auxDataFile, "MAG%c%d, ", axis1, i / 9 + 1);
                                        break;
                                case 8:
                                        fprintf(auxDataFile, "MAG%c%d, ", axis1, i / 9 + 1);
                                        break;
                        }
                }
                        
                else
                        fprintf(auxDataFile, "MAG%c%d\r\n", axis1, i / 9 + 1);
        }


         /* Data collection loop */
		printf("ready to write data\n");
        time(&startTime);
        time(&lastTime);
        bFirstSampleAcquired = false;
        while (time(0) - startTime < iCollectDuration || !bFirstSampleAcquired)
        {
                /* Read EMG data from the network and write to file */
                if (recv(emgSock, emgData, SZ_DATA_EMG, MSG_PEEK) >= SZ_DATA_EMG)
                {
                        /* When the first sample is acquired, set the start of the recording */
                        if (!bFirstSampleAcquired)
                        {
                                time(&startTime);
                                bFirstSampleAcquired = true;
                        }
                        recv(emgSock, emgData, SZ_DATA_EMG, 0);
                        unsigned int samples = SZ_DATA_EMG / sizeof(float) / 16;
                        float emgDataFlt[SZ_DATA_EMG / sizeof(float)];
                        memcpy(emgDataFlt, emgData, SZ_DATA_EMG);
                        //0705
                        
                        memcpy(emg_data_flt, emgData, SZ_DATA_EMG);
                        sem_post(&sem1);
                        
                        //
						for (i = 0; i < samples; i++)
                        {
								/* Write time */
								get_Sys_Time(sTemp);
								fprintf(emgDataFile,"%s,",sTemp);
								/* write data */
                                for (j = 0; j < 16; j++)
                                {
                                        
                                                if (j < 15)
                                                {
                                                        if(sensorType[j]=='O')
                                                                fprintf(emgDataFile, "%f, ", emgDataFlt[i * 16 + j]);
                                                        else
                                                                fprintf(emgDataFile, ", ");
                                                }        
                                                        
                                                else
                                                {
                                                        if(sensorType[j]=='O')
                                                                fprintf(emgDataFile, "%f", emgDataFlt[i * 16 + j]);
                                                        else
                                                                fprintf(emgDataFile, ",");
                                                }
                                                        
                                        
                                        
                                }
                                fprintf(emgDataFile, "\r\n");
                        }
                }

                /*Read ACC data from the network and write to file*/ 
                if (recv(accSock, accData, SZ_DATA_ACC, MSG_PEEK) >= SZ_DATA_ACC)
                {
                        if (!bFirstSampleAcquired)
                        {
                                time(&startTime);
                                bFirstSampleAcquired = true;
                        }
                        recv(accSock, accData, SZ_DATA_ACC, 0);
                        unsigned int samples = SZ_DATA_ACC / sizeof(float) / 48;
                        //unsigned int samples = 1;
                        float accDataFlt[SZ_DATA_ACC / sizeof(float)];
                        memcpy(accDataFlt, accData, SZ_DATA_ACC);
                        for (i = 0; i < samples; i++)
                        {
								/* Write time */
								get_Sys_Time(sTemp);
								fprintf(accDataFile,"%s,",sTemp);
								/* write data */
                                for (j = 0; j < 48; j++)
                                {       

                                        if (j < 47)
                                                {
                                                        if(sensorType[j/3]=='O')
                                                                fprintf(accDataFile, "%f, ", accDataFlt[i * 48 + j]);
                                                        else
                                                                fprintf(accDataFile, ", ");
                                                }        
                                                        
                                                else
                                                {
                                                        if(sensorType[j/3]=='O')
                                                                fprintf(accDataFile, "%f", accDataFlt[i * 48 + j]); 
                                                        else
                                                                fprintf(accDataFile, ","); 
                                                } 
                                        
                                }
                                fprintf(accDataFile, "\r\n");
                        }
                }
                /* Read IM EMG data from the network and write to file */
               if (recv(imemgSock, imemgData, SZ_DATA_IM_EMG, MSG_PEEK) >= SZ_DATA_IM_EMG)
                {
                        /* When the first sample is acquired, set the start of the recording */
                        if (!bFirstSampleAcquired)
                        {
                                time(&startTime);
                                bFirstSampleAcquired = true;
                        }
                        recv(imemgSock, imemgData, SZ_DATA_IM_EMG, 0);
                        unsigned int samples = SZ_DATA_IM_EMG / sizeof(float) / 16;
                        float imemgDataFlt[SZ_DATA_IM_EMG / sizeof(float)];
                        memcpy(imemgDataFlt, imemgData, SZ_DATA_IM_EMG);
                        for (i = 0; i < samples; i++)
                        {
								/* Write time */
								get_Sys_Time(sTemp);
								fprintf(imemgDataFile,"%s,",sTemp);
								/* write data */
                                for (j = 0; j < 16; j++)
                                {
                                        

                                                if (j < 15)
                                                { 
                                                        if(sensorType[j]=='O')
                                                            fprintf(imemgDataFile, "%f, ", imemgDataFlt[i * 16 + j]);
                                                        else
                                                            fprintf(imemgDataFile, ", ");
                                                }
                                                else
                                                {
                                                        if(sensorType[j]=='O')
                                                            fprintf(imemgDataFile, "%f ", imemgDataFlt[i * 16 + j]);
                                                        else
                                                            fprintf(imemgDataFile, ",");  
                                                }
                                                          
                                        
                                        
                                }
                                fprintf(imemgDataFile, "\r\n");
                        }
                }
                /*Read ACC,GYRO,MAG data from the network and write to file*/ 
                if (recv(auxSock, auxData, SZ_DATA_IM_AUX, MSG_PEEK) >= SZ_DATA_IM_AUX)
                {
                        recv(auxSock, auxData, SZ_DATA_IM_AUX, 0);
                        unsigned int samples = SZ_DATA_IM_AUX / sizeof(float) / 144;
                        float auxDataFlt[SZ_DATA_IM_AUX / sizeof(float)];
                        memcpy(auxDataFlt, auxData, SZ_DATA_IM_AUX);
                        for (i = 0; i < samples; i++)
                        {
								/* Write time */
								get_Sys_Time(sTemp);
								fprintf(auxDataFile,"%s,",sTemp);
								/* write data */
                                for (j = 0; j < 144; j++)
                                {       
                                     
                                        if (j < 143)
                                                { 
                                                        if(sensorType[j/9]=='O')
                                                            fprintf(auxDataFile, "%f, ", auxDataFlt[i * 144 + j]);
                                                        else
                                                            fprintf(auxDataFile, ", ");
                                                }
                                        else
                                                {
                                                        if(sensorType[j/9]=='O')
                                                            fprintf(auxDataFile, "%f", auxDataFlt[i * 144 + j]);
                                                        else
                                                            fprintf(auxDataFile, ",");  
                                                }
                                               
                                        
                                }
                            fprintf(auxDataFile, "\r\n");       
                        }
                        
                }
                
                /* Check for stop trigger input */
                reply = readReplyIfReady(commSock, 9);
                if (reply != NULL)
                {
                        printf("Recieved %s\r\n", reply);
                        if (strstr(reply, "STOPPED\r\n") != NULL)
                        {
                                printf("stopped via trigger.");
                                break;
                        }
                        free(reply);
                }

                /* Indicate progress */
                if (time(0) - lastTime > 0)
                {
                        //printf(".");
                        time(&lastTime);
                }
        }

        /* Close the EMG and ACC data storage files */
        fclose(emgDataFile);
        fclose(accDataFile);
        fclose(imemgDataFile);
        fclose(auxDataFile);

        /* Close the connection to the server */
        printf("\r\nClosing connections.");

        sprintf(szTmp, "STOP\r\n\r\n");
        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem stopping Delsys data stream.\r\n");
                return 0;
        }
        reply = readReply(commSock);
        free(reply);

        sprintf(szTmp, "QUIT\r\n\r\n");
        if (send(commSock, szTmp, strlen(szTmp), 0) == -1)
        {
                printf("\r\nProblem closing connection.\r\n");
                return 0;
        }

        close(emgSock);
        close(accSock);
        close(imemgSock);
        close(auxSock);

        /* Done */
        printf("muscle data collect Done\r\n");

        pthread_exit(0);
}

