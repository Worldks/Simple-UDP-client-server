#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "my_string.h"

enum { buf_size = 1432 };

void make_counters(char *buf, int size_cnt, int dgram_cnt)
{
	int str_len;
	char str[max_number_length];
	number_to_str(size_cnt, str);	
	str_len = str_length(str);
	str_copy(buf, "Recived size dgram: ");
	str_copy(buf+20, str);
	number_to_str(dgram_cnt, str);
	buf[str_len+20] = ' ';
	str_copy(buf+20+str_len+1, "Count of recived dgram: ");
	str_copy(buf+20+str_len+1+24, str);
}

int main(int argc, char **argv)
{
	int sd, pid, port, res;
	int recv_dgram_cnt = 0;
	int recv_data_size = 0;
	char msg_buf[buf_size] = { 0 };
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	if(argc != 2) {
		fprintf(stderr, "Error.Uncorrect args. Need server port.\n");
		return 1;
	}
	/* demonization */
	close(0);
	close(1);
	close(2);
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_WRONLY);
	open("/dev/null", O_WRONLY);
	chdir("/");
	pid = fork();
	if(pid > 0)
		exit(0);
	setsid();
	pid = fork();
	if(pid > 0)
		exit(0);
	/*--------------*/
	port = str_to_number(argv[1]);
	if(port <= 1024) {
		fprintf(stderr, "Error. Server port uncorrect ( < 1024).\n");
		return 1;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd == -1) {
		perror("socket()");
		return 2;
	}
	res = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
	if(res == -1) {
		perror("bind()");
		return 2;
	}
	for(;;) {
		recv_data_size += recvfrom(sd, msg_buf, sizeof(msg_buf),
									0, (struct sockaddr*)&client_addr, &client_addr_size);
		recv_dgram_cnt++;
		make_counters(msg_buf, recv_data_size, recv_dgram_cnt);
		sendto(sd, msg_buf, sizeof(msg_buf), 0, 
						(struct sockaddr*)&client_addr, client_addr_size);
	}
	return 0;
}
