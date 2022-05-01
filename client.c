#include "project05.h"



int check_client(char *buffer, User *buff,int *buff_size){
	char cpybuf[BUFF_SIZE];
	char name_token[BUFF_SIZE];
	strcpy(cpybuf, buffer);
	strtok(cpybuf, " ");
	snprintf(name_token, BUFF_SIZE, "%s", strtok(NULL, " "));
	for(int i = 0; i < *buff_size; i++) {
		if(!strcmp(name_token, buff[i].name)){
			printf("\n");
			return i;
		}
	}
	return *buff_size;
}
int connect_client(char *buff,User *online){
	char name[BUFF_SIZE];
	char cpy_buff[BUFF_SIZE];
	strcpy(cpy_buff, buff);
	snprintf(name, BUFF_SIZE, "%s", strtok(cpy_buff, " "));
	strcpy(name, &name[1]);


	

	printf("%s\n", name);
	return 1;
}


int msg (int argc, char **argv) {
	struct addrinfo hints;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	struct addrinfo *res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (rc != 0)
	     printf("%s\n", gai_strerror(rc));
	int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	if (connect(fd, res->ai_addr, res->ai_addrlen) != 0)
	     perror("connect");
	
	char *msg = "test";
	int len = strlen(msg) + 1;
	if (send(fd, msg, len, 0) == -1)
	    perror("send");
	
	if (close(fd) == -1)
	     perror("close");
	
	return 0;
}
