#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "my_string.h"

enum { buf_size = 1432 };
volatile static sig_atomic_t pr = 0;
const char massege[] = "No responce\n";

void sig_alarm_handler(int s)
{
	int save_errno = errno;
	signal(SIGALRM, SIG_DFL);
	if(pr == 0) {
		write(1, massege, sizeof(massege)-1);
		exit(0);
	} 
	errno = save_errno;
}

void fill_msg_buf(char *buf, int cnt_characters, char ch)
{
	int i = 0;
	for( ; i < cnt_characters; i++)
		buf[i] = ch;
}

int main(int argc, char **argv)
{
	int sd, port, cnt, ok;
	struct sockaddr_in serv_addr;
	socklen_t serv_addr_len = sizeof(serv_addr);
	char msg_buf[buf_size] = { 0 };
	if(argc != 4) {
		fprintf(stderr, "Error.Ucorrect agrs. Need - ip port size_massege\n");
		return 1;
	}
	port = str_to_number(argv[2]);
	cnt = str_to_number(argv[3]);
	if(cnt >= 1432) {
		fprintf(stderr, "Error.Ucorrect size_massege( < 1432).\n");
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	ok = inet_aton(argv[1], &(serv_addr.sin_addr));
	if(!ok) {
		fprintf(stderr, "Error.Ucorrect ip.\n");
		return 1;
	}
	signal(SIGALRM, sig_alarm_handler);
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd == -1) {
		perror("socket()");
		return 2;
	}
	fill_msg_buf(msg_buf, cnt, '1');
	sendto(sd, msg_buf, cnt, 0, 
			(struct sockaddr*)&serv_addr, sizeof(serv_addr));
	alarm(10);
	pr = recvfrom(sd, msg_buf, buf_size, 0,
			(struct sockaddr*)&serv_addr, &serv_addr_len);
	if(pr != 0)
		printf("%s\n", msg_buf);
	else
		printf("%s\n", massege);
	return 0;
}
