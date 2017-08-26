/*
 * Test application that data integraty of inter processor
 * communication from linux userspace to a remote software
 * context. The application sends chunks of data to the
 * remote processor. The remote side echoes the data back
 * to application which then validates the data returned.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

/* Shutdown message ID */
#define SHUTDOWN_MSG	0xEF56A55A
#define MAX_SIZE 20
struct _payload {
	unsigned long num;
	unsigned long size;
	char data[];
};

static int fd, err_cnt;

struct _payload *i_payload;
struct _payload *r_payload;

#define RPMSG_GET_KFIFO_SIZE 1
#define RPMSG_GET_AVAIL_DATA_SIZE 2
#define RPMSG_GET_FREE_SPACE 3

#define RPMSG_HEADER_LEN 16
#define MAX_RPMSG_BUFF_SIZE (512 - RPMSG_HEADER_LEN)
#define PAYLOAD_MIN_SIZE	1
#define PAYLOAD_MAX_SIZE	(MAX_RPMSG_BUFF_SIZE - 24)
#define NUM_PAYLOADS		(PAYLOAD_MAX_SIZE/PAYLOAD_MIN_SIZE)

typedef struct DATA_ARRAY{
	unsigned int size;
	unsigned int elements[MAX_SIZE];
}data_array;

static data_array send_data;
static data_array recv_data;

int main(int argc, char *argv[])
{
	int flag = 1;
	int shutdown_msg = SHUTDOWN_MSG;
	int cmd, ret, i, expect_rnum;
	int size, bytes_rcvd, bytes_sent;
	err_cnt = 0;
	int opt;
	char *rpmsg_dev="/dev/rpmsg0";

	while ((opt = getopt(argc, argv, "d:")) != -1) {
		switch (opt) {
		case 'd':
			rpmsg_dev = optarg;
			break;
		default:
			printf("getopt return unsupported option: -%c\n",opt);
			break;
		}
	}
	printf("\r\n VIVO TEST START \r\n");

	printf("\r\n Open rpmsg dev! \r\n");

	fd = open(rpmsg_dev, O_RDWR | O_NONBLOCK);

	if (fd < 0) {
		perror("Failed to open rpmsg file /dev/rpmsg0.");
		return -1;
	}

	printf("\r\n Query internal info .. \r\n");

	ioctl(fd, RPMSG_GET_KFIFO_SIZE, &size);

	printf(" rpmsg kernel fifo size = %u \r\n", size);

	ioctl(fd, RPMSG_GET_FREE_SPACE, &size);

	printf(" rpmsg kernel fifo free space = %u \r\n", size);

	i_payload = (struct _payload *)malloc(2 * sizeof(unsigned long) + PAYLOAD_MAX_SIZE);
	r_payload = (struct _payload *)malloc(2 * sizeof(unsigned long) + PAYLOAD_MAX_SIZE);

	if (i_payload == 0 || r_payload == 0) {
		printf("ERROR: Failed to allocate memory for payload.\n");
		return -1;
	}

	send_data.elements[0] = 0;
	send_data.elements[1] = 0;
	send_data.elements[2] = 0;
	send_data.elements[3] = 0;
	send_data.elements[4] = 0;


	while (flag == 1) {
		printf("\r\n **************************************** \r\n");
		printf(" Please enter command and press enter key\r\n");
		printf(" **************************************** \r\n");
		printf(" 1 - Send a command, and receive data from remote processor");
		printf(" 2 - Quit this application .. \r\n");
		printf(" CMD>");
		ret = scanf("%d", &cmd);
		if (!ret) {
			while (1) {
				if (getchar() == '\n')
					break;
			}

			printf("\r\n invalid command\r\n");
			continue;
		}

		send_data.size = 5;
		send_data.elements[0] += 1;
		send_data.elements[1] += 2;
		send_data.elements[2] += 3;
		send_data.elements[3] += 4;
		send_data.elements[4] += 5;

		if (cmd == 1) {
			expect_rnum = 0;

		int write_result = write(fd, &send_data,
						(4+4*send_data.size));

		usleep(20);

		while(1)
		{
			bytes_rcvd = read(fd, &recv_data,
						(4 * sizeof(unsigned long)));

            if(bytes_rcvd>0)
				{
				printf(" received payload size ");
				printf("is %d .\r\n", recv_data.size);
				for (i = 0; i < recv_data.size; i++)
					{
					printf("Sum is: %d\n",recv_data.elements[i]);
					}
				break;
				}
            else
            	usleep(20);
			//if(recv_data.elements[0]==1)
			//	break;
		}   //end while


		} else if (cmd == 2) {
			flag = 0;
			/* Send shutdown message to remote */
			write(fd, &shutdown_msg, sizeof(int));
			sleep(1);
			printf("\r\n Quitting application .. \r\n");
			printf(" Echo test end \r\n");
		} else {
			printf("\r\n invalid command! \r\n");
		}
	}

	free(i_payload);
	free(r_payload);

	close(fd);

	return 0;
}
