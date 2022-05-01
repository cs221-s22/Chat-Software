#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <poll.h>
#include <string.h>
#include <stdbool.h>
#define BUFF_SIZE 64

 struct user {
     char status[BUFF_SIZE]; /* online or offline? */
     char name[BUFF_SIZE];   /* user name */
     char port[BUFF_SIZE];   /* TCP listener port */
     char host[NI_MAXHOST]; /* host port number */
     char machine [BUFF_SIZE];
 };
 typedef struct user User;

// presence.c UDP
int Presence_socket();
int announce_Presence(int args, char * argv [],  User *buff);
int capture_Presence(int sfd, User *buff, int *buf_size);
int broadcast_Offline(int sdf, User *buff);


//client.c TCP
int check_client(char *buffer, User *buff,int *buff_size);
int connect_client(char *buff,User *online, int online_size);
int msg_client (char *machine, char *port);
int Tcp_socket_client(User * online);
