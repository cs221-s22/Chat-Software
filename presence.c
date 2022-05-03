#include <arpa/inet.h>
#include "project05.h"

#define BUF_SIZE 64


int announce_Presence(int args, char * argv[],  User *buff) {
	if (args < 2) {
		return -1;
	}
	struct sockaddr_in serv_addr;
	char active[BUFF_SIZE];
	
	snprintf(buff->status, BUFF_SIZE, "online");
	snprintf(active, BUFF_SIZE, "%s %s %s",buff->status, argv[1], argv[2]);
	snprintf(buff->name, BUFF_SIZE, "%s", argv[1]);
	snprintf(buff->host, BUFF_SIZE, "%s", argv[2]);

	
	int sdf = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int enable_broadcast = 1;	
	int len, s;
	int rv = setsockopt(sdf, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, sizeof(int));
	if (rv == -1) {
	 	perror("setsockopt");
	 	exit(EXIT_FAILURE);
	}
	serv_addr.sin_family = PF_INET;
	int ip = inet_pton(PF_INET, "10.10.13.255", &serv_addr.sin_addr);
	if (ip != 1) {
	 	perror("inet_pton");
	 	exit(EXIT_FAILURE);
	}
	serv_addr.sin_port = htons(8221);
	len =  strlen(active);
	s = sendto(sdf, active, len +1, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if ( len + 1 != s)
	    printf("%s\n", "Socket connection failed!");
	
}

int Presence_socket() {
	struct addrinfo hints;
	    struct addrinfo *result, *rp;
	    int sfd, s;
	    struct sockaddr_storage peer_addr;
	
	    memset(&hints, 0, sizeof(hints));
	    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
	    hints.ai_socktype = SOCK_DGRAM; // Datagram socket
	    hints.ai_flags = AI_PASSIVE;    // Any IP address (DHCP)
	    hints.ai_protocol = 0;          // Any protocol
	    hints.ai_canonname = NULL;
	    hints.ai_addr = NULL;
	    hints.ai_next = NULL;
	    char *port = "8221";
	
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
	      	rv = setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, sizeof(int));
	      	
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

int capture_Presence(int sfd, User  *buff, int *buf_size){
    int s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

        peer_addr_len = sizeof(peer_addr);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
        (struct sockaddr *) &peer_addr, &peer_addr_len);


        if (nread == 0) {
        	return -2;
        }

        if (nread == -1)
            return -1;  // Ignore failed request 

        char host[NI_MAXHOST], service[NI_MAXSERV];

        s = getnameinfo((struct sockaddr *) &peer_addr,
                peer_addr_len, host, NI_MAXHOST,
                service, NI_MAXSERV, NI_NUMERICSERV);
        if (s != 0)
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

        if (sendto(sfd, buf, nread, 0,(struct sockaddr *) &peer_addr, peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");


	  		int index = check_client(buf, buff, buf_size); 
	  		if (index == *buf_size){
    			strcpy(buff[index].status, strtok(buf, " "));
    			snprintf(buff[index].name, BUFF_SIZE, "%s", strtok(NULL, " "));
    			snprintf(buff[index].host, BUFF_SIZE, "%s", strtok(NULL, " "));
    			snprintf(buff[index].machine, BUFF_SIZE, "%s", host);
    			*(buf_size)+= 1;
    		}

    		if (strcmp(buff[index].status, strtok(buf, " ")))
    			strcpy(buff[index].status, strtok(buf, " "));
    		

    return index;
}

int broadcast_Offline(int sdf, User *buff){
	char inactive[BUFF_SIZE];
	struct sockaddr_in serv_addr;



	snprintf(buff->status, BUFF_SIZE, "offline");
	snprintf(inactive, BUFF_SIZE, "%s %s %s", buff->status, buff->name, buff->host);
	int len =  strlen(inactive);
	int enable_broadcast = 1;
	int rv = setsockopt(sdf, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, sizeof(int));
	if (rv == -1) {
		 perror("setsockopt");
		 exit(EXIT_FAILURE);
	}
	serv_addr.sin_family = PF_INET;
	int ip = inet_pton(PF_INET, "10.10.13.255", &serv_addr.sin_addr);
	if (ip != 1) {
		 perror("inet_pton");
		 exit(EXIT_FAILURE);
	}
	serv_addr.sin_port = htons(8221);
	int s = sendto(sdf, inactive, len +1, 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if ( len + 1 != s)
		 printf("%s\n", "Socket connection failed!");
	return 0;
}
