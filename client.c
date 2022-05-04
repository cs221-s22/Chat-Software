#include "project05.h"



int check_client(char *buffer, User *buff,int *buff_size){
	char cpybuf[BUFF_SIZE];
	char name_token[BUFF_SIZE];
	strcpy(cpybuf, buffer);
	strtok(cpybuf, " ");
	snprintf(name_token, BUFF_SIZE, "%s", strtok(NULL, " "));
	for (int i = 0; i < *buff_size; i++) {
		if(!strcmp(name_token, buff[i].name)){
			printf("\n");
			return i;
		}
	}
	return *buff_size;
}
int connect_client(char *buff,User *online, int online_size){
	char name[BUFF_SIZE];
	char cpy_buff[BUFF_SIZE];
	strcpy(cpy_buff, buff);
	char msg[BUFF_SIZE];
	char cpy_buff_to_send[BUFF_SIZE];
	int  i = 0;

	
	snprintf(name, BUFF_SIZE, "%s", strtok(cpy_buff, " "));
	int len = strlen(name);
	for (int  i = 0; i < len; i++) {
		name[i] = name[i+1];
	}
	char * space = " ";
	strcpy(cpy_buff_to_send, buff);
	int size  = strlen(cpy_buff_to_send);
	while(cpy_buff_to_send[0]!= ' '){
		while(i < size){
			cpy_buff_to_send[i] = cpy_buff_to_send[i+1];
			i++;
		}
		i = 0;
	}
	snprintf(msg, BUFF_SIZE, "@%s%s",online[0].name, cpy_buff_to_send);
	for (int i = 0; i < online_size; i++) {
		if(!strcmp(name, online[i].name)){
			msg_client(online[i].machine, online[i].host, msg);
			snprintf(online[0].port, BUFF_SIZE, "%s", online[i].host);
			return 1;
		}
	}
	
	return 0;
}


int msg_client (char *machine, char *port, char *buff) {
	struct addrinfo hints;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	struct addrinfo *res;
	int rc = getaddrinfo(machine, port, &hints, &res);
	if (rc != 0)
	     printf("%s\n", gai_strerror(rc));
	int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	if (connect(fd, res->ai_addr, res->ai_addrlen) != 0)
	     perror("connect");
	
	int len = strlen(buff) + 1;
	if (send(fd, buff, len, 0) == -1)
	    perror("send");
	
	if (close(fd) == -1)
	     perror("close");
	
	return 0;
}


int Tcp_socket_client(User *online){
	int sockfd;
	    struct sockaddr_in servaddr, cli;
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    
	    if (sockfd == -1) {
	        printf("socket creation failed...\n");
	        exit(0);
	    }

	    servaddr.sin_family = AF_INET;
	    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    servaddr.sin_port = htons(atoi(online[0].host));

	    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
	           printf("socket bind failed...\n");
	           exit(0);
	    }
	    if ((listen(sockfd, 5)) != 0) {
	           printf("Listen failed...\n");
	           exit(0);
	    } 
	    int enable = 1;
	    ioctl(sockfd, FIONBIO, &enable); 


	    return sockfd;       
}

int accept_client_msg(int sfd, User *online, int online_size){
	struct sockaddr_in cli;
	int connfd, len;
	char buff[BUFF_SIZE];
	char client_msg[BUFF_SIZE];
	char cpybuf[BUFF_SIZE];
	char name_token[BUFF_SIZE];

	int  i = 0;
		
	
	
	connfd = accept(sfd, (struct sockaddr*)&cli, &len);
	 if (connfd < 0) {
	      printf("server accept failed...\n");
	      exit(0);
	 }

	 read(connfd, buff, sizeof(buff));
	 printf("%s\n", buff);
	 return 0;

	  
}




