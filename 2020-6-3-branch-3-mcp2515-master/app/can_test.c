#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <sys/socket.h>

#include <linux/can.h>

#include <signal.h>


	
#define  CAN_Set_Own_ID 	0x4501     //set own id
#define  CAN_Set_Broadcast_ID  0x4502    //set target id    default    0xff
#define  CAN_Set_Bandrate	0x4503    //set can bandrate    default   10Kbps
#define	 BUF_LEN			8

typedef enum{	
		BandRate_10kbps=10,
		BandRate_20kbps=20,
		BandRate_40kbps=40,
		BandRate_80kbps=80,
		BandRate_125kbps=125,
		BandRate_250kbps=250,
		BandRate_500kbps=500,		
		BandRate_1Mbps=1000
}CanBandRate; 		
			

static void useage()
{
	printf("CAN test for 2416--------------------\n");
	printf("brate:10K\n");
	printf("if '-s', the target_id=0x23, own_id=0x22 broad_id=0xff\n");
	printf("if '-r', the target_id=0xff,  own_id=0x23 broad_id=0xff\n");
	printf("example:\n");
	printf("CANtest send--------:  #CANtest  -s  \n");
	printf("CANtest receive------:  #CANtest  -r  \n");
}


static int can_set_own_id(int fd, canid_t id)
{
	if(ioctl(fd, CAN_Set_Own_ID, id) != 0)  //set own id
	{
		perror("Can Test Set ID: ");
		return -1;
	}

	return 0;
}


static int can_set_broadcast_id(int fd, canid_t id)
{
	if(ioctl(fd, CAN_Set_Broadcast_ID, id) != 0)  //set own id
	{
		perror("Can Test Set Broadcast: ");
		return -1;
	}

	return 0;
}

static int can_set_bandrate(int fd)
{
	int input;
	CanBandRate bandrate;

	printf("Press Number to Choose BandRate: \n"
		"\t10:  BandRate_10kbps\n"
		"\t20:  BandRate_20kbps\n"
		"\t40:  BandRate_40kbps\n"
		"\t80:  BandRate_80kbps\n"
		"\t125: BandRate_125kbps\n"
		"\t250: BandRate_250kbps\n"
		"\t500: BandRate_500kbps\n"
		"Your Choose is: ");

	scanf("%d", &input);

	switch(input) {
	case BandRate_500kbps:
	case BandRate_250kbps:
	case BandRate_125kbps:
	case BandRate_80kbps:
	case BandRate_40kbps:
	case BandRate_20kbps:
	case BandRate_10kbps:
		bandrate = input;
		break;
	default:
		return -1;
	}

	if(ioctl(fd, CAN_Set_Bandrate, bandrate) != 0)  //set own id
	{
		perror("Can Test Set BandRate: ");
		return -1;
	}

	printf("Can Test: Set BandRate %dKbps.\n", bandrate);
	return 0;
}



//void handle_sig(int num)
//{
//    printf( "%s\n", __func__ );
//    exit(1);
//}



int main(int argc, char **argv)
{
	struct can_frame frame;	
	canid_t own_id, broadcast_id;
	int nbytes;
	char buf[BUF_LEN];

	int rpm[8]={2,1,12,0,0,0,0,0};	
	int n;	
	int rpmval;

	int i;
	int len;
	int fd;

	FILE * fp;
	//fp = fopen("1.txt","w+");
	//signal(SIGINT, handle_sig);

	if (argc != 2)
	{
		useage();
		return -1;
	}

	if((fd = open("/dev/mcp2515", O_RDWR)) == -1)
	{
		perror("Can Test Open Error: ");
		return -1;
	}

	printf("Can Test: Open Can Device OK!\n");

	

	if (can_set_bandrate(fd) < 0)
		printf("Can Test: Set BandRate Failed, BandRate is default value 125kbps.\n");

/****************  read data  ********************/

	if(argv[1][1] == 'r')
	{
		own_id = 0x111111;
		broadcast_id = 0xFF;

		can_set_own_id(fd, own_id);
		can_set_broadcast_id(fd, broadcast_id);

		printf("\nWaiting Data ...\n");

		while (1)
		{
			memset(buf, 0, BUF_LEN);
			memset(&frame, 0, sizeof(struct can_frame));

			nbytes = read(fd, &frame, sizeof(struct can_frame));	

			memcpy(buf, frame.data, frame.can_dlc);
			
			fp = fopen("1.txt","w+");

			printf("%x", buf[0]);
			fprintf(fp,"%x", buf[0]);

			printf("%x", buf[1]);
			fprintf(fp,"%x", buf[1]);

			printf("%x", buf[2]);
			fprintf(fp,"%x", buf[2]);

			printf("%x", buf[3]);
			fprintf(fp,"%x", buf[3]);

			printf("%x", buf[4]);
			fprintf(fp,"%x", buf[4]);

			printf("%x", buf[5]);
			fprintf(fp,"%x", buf[5]);

			printf("%x", buf[6]);
			fprintf(fp,"%x", buf[6]);

			printf("%x\n\n", buf[7]);
			fprintf(fp,"%x", buf[7]);

			fclose(fp);			
			

		}
	}

/****************  read data  ********************/

	if(argv[1][1] == 's')
	{	
		own_id = 0x1;
		broadcast_id = 0xFF;

		can_set_own_id(fd, own_id);
		can_set_broadcast_id(fd, broadcast_id);
		frame.can_id = 0x7df;

		while (1)
		{	
			printf("record\n");
			i = 0;

			memset(buf, 0, BUF_LEN);
			memset(&frame, 0, sizeof(struct can_frame));

			

			//printf("\nPress Message(Less than 8 bytes): ");

			//while ((buf[i++] = getchar()) != '\n' && i < BUF_LEN);
			//buf[i-1] = '\0';
			
			for(n=0;n<8;n++)
			{
				buf[n] = rpm[n];
			}

			len = strlen(buf);
			frame.can_dlc = (len > 8) ? 8 : len;

			memcpy(frame.data, buf, frame.can_dlc);

			nbytes = write(fd, &frame, sizeof(struct can_frame));	
			
	

			
			//memset(buf, 0, BUF_LEN);
			//memset(&frame, 0, sizeof(struct can_frame));

			nbytes = read(fd, &frame, sizeof(struct can_frame));	
			
			usleep(1);

			memcpy(buf, frame.data, frame.can_dlc);
			
			

			//printf("%02x", buf[0]);
			//fprintf(fp,"%02x", buf[0]);

			//printf("%02x", buf[1]);
			//fprintf(fp,"%02x", buf[1]);

			//printf("%02x", buf[2]);
			//fprintf(fp,"%02x", buf[2]);

			//printf("%02x", buf[3]);
			//fprintf(fp,"%04x", buf[3]);

			//printf("%02x", buf[4]);
			//rpmval = *(frame.data+3)<<2 + *(frame.data+4);
			rpmval = ((buf[3]<<8) + (buf[4]));
			
			//rpmval = buf[4];
			printf("%d",rpmval/4);

			fp = fopen("/mnt/1.txt","w+");
			fprintf(fp,"%d", rpmval/4);
		
			
			//printf("%02x", buf[5]);
			//fprintf(fp,"%02x", buf[5]);

			//printf("%02x", buf[6]);
			//fprintf(fp,"%02x", buf[6]);

			//printf("%02x\n\n", buf[7]);
			//fprintf(fp,"%02x", buf[7]);

			fclose(fp);	




			//usleep(10000);
		}
	}

	close(fd);

   	return 0;
}
//Lee Sheen 2012-12-05
