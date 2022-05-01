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
int connect_client(char *buff,User *online, int online_size){
	char name[BUFF_SIZE];
	char cpy_buff[BUFF_SIZE];
	strcpy(cpy_buff, buff);
	snprintf(name, BUFF_SIZE, "%s", strtok(cpy_buff, " "));
	strcpy(name, &name[1]);
	for(int i = 0; i < online_size; i++) {
		if(!strcmp(name, online[i].name)){
			msg_client(online[i].machine, online[i].host);
			snprintf(online[0].port, BUFF_SIZE, "%s", online[i].host);
		}
	}
	
	return 0;
}


int msg_client (char *machine, char *port) {
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
	
	char *msg = "test";
	int len = strlen(msg) + 1;
	if (send(fd, msg, len, 0) == -1)
	    perror("send");
	
	if (close(fd) == -1)
	     perror("close");
	
	return 0;
}


int Tcp_socket_client(User *online){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	struct sockaddr_storage peer_addr;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // Datagram socket
	hints.ai_flags = AI_PASSIVE;    // Any IP address (DHCP)
	hints.ai_protocol = 0;          // Any protocol
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	char port [BUFF_SIZE];
	strcpy(port, online[0].host);
	
	    s = getaddrinfo(NULL, port, &hints, &result);
	    if (s != 0) {
	        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	        exit(EXIT_FAILURE);
	    }
	
	    for (rp = result; rp != NULL; rp = rp->ai_next) {
	        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	
			int enable_broadcast = 1;
	      	int rv = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable_broadcast, sizeof(int));
			if(rv == -1) {
				perror("setsocketopt");
			}
	
	        if (sfd == -1)
	            continue;
	
	        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
	            break;  // Success
	
	        close(sfd);
	    }
	
	    freeaddrinfo(result);  // No longer needed
	
	    if (rp == NULL) {  // No address succeeded
	        fprintf(stderr, "Could not bind\n");
	        exit(EXIT_FAILURE);
	    }
	return sfd;
}




