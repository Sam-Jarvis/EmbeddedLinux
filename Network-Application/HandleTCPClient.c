#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <libusb-1.0/libusb.h>
#include <string.h>

#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int controller_map(int to_map)
{
	switch (to_map)
	{
	case 1:
		return 1;
		break;

	case 2:
		return 2;
		break;
	
	case 4:
		return 3;
		break;

	case 8:
		return 4;
		break;

	case 16:
		return 5;
		break;

	case 32:
		return 6;
		break;

	case 64:
		return 7;
		break;
	
	case -128:
		return 8;
		break;

	default:
		return 0;
		break;
	}
}

int open_controller(libusb_device_handle * h)
{
    int success = 0;

    if (h == NULL) {
	    perror("Failed to open device\n");
        success += 1;
	}

	if(libusb_detach_kernel_driver(h, 0) != 0){
		perror("Device detach failed\n");
        success += 1;
	}

	if(libusb_claim_interface(h, 0) != 0){
		perror("Device claim failed\n");
        success += 1;
	}
    else
    {
        printf("Claim successful\n");
    }
    

    return success;
}

int close_controller(libusb_device_handle * h)
{
    int success = 0;

    if(libusb_release_interface(h, 0) != 0){
        perror("Device release failed\n");
        success += 1;
    }
    else
    {
        printf("%s\n", "successfully released");
    }

    libusb_close(h);

    printf("%s\n", "sucessfully closed");
    
    // if(libusb_attach_kernel_driver(h, 0) != 0){
    //     perror("Device attach failed\n");
    //     success += 1;
    // }

    return success;
}

void HandleTCPClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */
    libusb_device_handle *h;

	unsigned char data_led[3];
    unsigned char data_rumble[8];

    int error;
    int transferred;

    int received;
	char read_data[512];
    int endpoint = 0x01;
    int timeout = 2000;

    //pthread_mutex_lock(&mutex);
    libusb_init(NULL);
	h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);

    open_controller(h);

    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed\n");
    }
    info_d ("Recv", recvMsgSize);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        // TODO: add code to print the received string; use printf()
        //printf("%s", "supposed to print message received.");
        //printf(echoBuffer);

        // TODO: add code to convert the upper/lower chars of the received string
        // toupper(echoBuffer);

        // delaying ();
        
        if(strcmp("f", echoBuffer) == 0)
        {   
            data_rumble[0] = 0;
			data_rumble[1] = 8;
			data_rumble[2] = 0;
			data_rumble[3] = 64;
			data_rumble[4] = 0;
			data_rumble[5] = 0;
			data_rumble[6] = 0;
			data_rumble[7] = 0;

            if ((error = libusb_interrupt_transfer(h, 0x01, data_rumble, sizeof(data_rumble), &transferred, 2000)) != 0) {
                perror("trans failed");
            }

            sleep(1);

            data_rumble[3] = 0;
            if ((error = libusb_interrupt_transfer(h, 0x01, data_rumble, sizeof(data_rumble), &transferred, 2000)) != 0) {
                perror("trans failed");
            }

            close_controller(h);
            pthread_mutex_unlock(&mutex);

            recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
            if (recvMsgSize < 0)      
            {
                DieWithError ("recv() failed");
            }
            info_d ("recv", recvMsgSize);
        }

        if(strcmp("l", echoBuffer) == 0)
        {
            data_led[0] = 1;
            data_led[1] = 3;
            data_led[2] = 1;

            printf("Transfer LED\n");
            if ((error = libusb_interrupt_transfer(h, 0x01, data_led, sizeof(data_led), &transferred, 2000)) != 0) {
                perror("trans failed");
            }

            close_controller(h);
            pthread_mutex_unlock(&mutex);

            recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
            if (recvMsgSize < 0)
            {
                DieWithError ("recv() failed");
            }
            info_d ("recv", recvMsgSize);
        }

        if(strcmp("r", echoBuffer) == 0)
        {
            memset(read_data, 0, sizeof(data_led));
			endpoint = 0x81;

            printf("%s\n", "Reading controller state ...");

            libusb_interrupt_transfer(h, endpoint, read_data, sizeof(read_data), &transferred, timeout);

            char *button_text[8];
            button_text[0] = "0";
            button_text[1] = "dpad-up";
            button_text[2] = "dpad-down";
            button_text[3] = "dpad-left";
            button_text[4] = "dpad-right";
            button_text[5] = "start";
            button_text[6] = "back";
            button_text[7] = "L-press";
            button_text[8] = "R-press";


            char *trigger_text[8];
            trigger_text[0] = "0";
            trigger_text[1] = "LB";
            trigger_text[2] = "RB";
            trigger_text[3] = "xbox";
            trigger_text[4] = "LB-RB";
            trigger_text[5] = "A";
            trigger_text[6] = "B";
            trigger_text[7] = "X";
            trigger_text[8] = "Y";

            printf("\n");
            printf("Message Type: %x\n", read_data[0]);
            printf("Message Length: %x\n", read_data[1]);
            printf("\n");
            printf("-------------------------------------------\n");
            printf("\n");
            printf("Pad: %s\n", button_text[controller_map(read_data[2])]);
            printf("Buttons: %s\n", trigger_text[controller_map(read_data[3])]);
            printf("LT: %x\n", read_data[4]);
            printf("RT: %x\n", read_data[5]);
            printf("Left X: %x %x\n", read_data[6], read_data[7]);
            printf("Left Y: %x %x\n", read_data[8], read_data[9]);
            printf("Right X: %x %x\n", read_data[10], read_data[11]);
            printf("Right Y: %x %x\n", read_data[12], read_data[13]);
            printf("\n");
        }

        close_controller(h);
        pthread_mutex_unlock(&mutex);
        /* Echo message back to client 
        if (send (clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        {
            DieWithError ("send() failed");
        }
        */
        // TODO: add code to display the transmitted string in verbose mode; use info_s()
        // argv_verbose = true;
        //info_s()

        // receive next string
        recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
        info_d ("recv", recvMsgSize);
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}
