#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h>

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

int main(int argc, char *argv[])
{

	libusb_device_handle *h;
	unsigned char data_led[3];
	unsigned char data_rumble[8];

	int error, transferred, received;
	char read_data[512];


	libusb_init(NULL);
	h = libusb_open_device_with_vid_pid(NULL, 0x045e, 0x028e);

	if (h == NULL) {
		perror("failed to open device ");
		return (1);
	}

	int endpoint = 0x01;
	int timeout = 2000;

	int dt_crm = libusb_detach_kernel_driver(h, 0);
	if(dt_crm != 0){
		perror("detach failed ");
	}

	int cl_crm = libusb_claim_interface(h, 0);
	if(cl_crm != 0){
		perror("claim failed ");
	}

	int opt;
	while((opt = getopt(argc, argv, ":if:rls")) != -1)
	{
		switch(opt)
		{
			case 'r':
			memset(read_data, 0, sizeof(data_led));
			endpoint = 0x81;
			
			printf("%s\n", "Reading controller state ...");
			

			    while(1) {
                libusb_interrupt_transfer(h, endpoint, read_data, sizeof(read_data), &transferred, timeout);
                
            	// for(int x=0; x < transferred; x++) {
            	// 	printf("%02x ", read_data[x]);
				// }
                // printf("\n");

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


			break;

			case 'l':
			data_led[0] = 0x01;
			data_led[1] = 0x03;
			data_led[2] = 0x01;

			if ((error = libusb_interrupt_transfer(h, endpoint, data_led, sizeof(data_led), &transferred, 2000)) != 0) {
				perror("transfer failed ");
				return (1);
			}
			break;

			case 's':
			data_rumble[0] = 0x00;
			data_rumble[1] = 0x08;
			data_rumble[2] = 0x00;
			data_rumble[3] = 0xFF;
			data_rumble[4] = 0xFF;
			data_rumble[5] = 0x00;
			data_rumble[6] = 0x00;
			data_rumble[7] = 0x00;

				if ((error = libusb_interrupt_transfer(h, endpoint, data_rumble, sizeof(data_rumble), &transferred, 2000)) != 0) {
					perror("transfer failed ");
					return (1);
				}
			break;
		}
	}
	
	//printf("%s\n", "opening device: passed");

	return (0);
}

//check
//detach
//claim
