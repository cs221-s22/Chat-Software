#include <stdio.h>
#include "project05.h"
#define BUF_SIZE 64
int main (int args, char *argv[]) {
	struct pollfd pdf [BUF_SIZE];
	User online [BUF_SIZE];
	char buff [BUFF_SIZE];
	int user_Size = 0;
	int ndfs = 0;
	int pollReturn;
	int store_size=0;
	int readFile;
	bool end_Loop = false;

	announce_Presence(args, argv, &online[user_Size]);
	user_Size++;
	
	pdf[0].fd = STDIN_FILENO;
	pdf[0].events = POLLIN;
	ndfs++;

	pdf[1].fd = Presence_socket();
	pdf[1].events = POLLIN;
	ndfs++;

	pdf[2].fd = Tcp_socket_client(online);
	pdf[2].events = POLLIN;
	ndfs++;

	while (!end_Loop) {
		pollReturn = poll(pdf, ndfs, 30000);
		if(pollReturn == 0){
			announce_Presence(args, argv, &online[0]);
		}

		//for (int  i = 0; i < pollReturn; i++) {
			if (pdf[0].revents & POLLIN) {
					memset((void*)buff, 0, BUF_SIZE);
					readFile = read(pdf[0].fd, (void*) buff, 50);
					if(readFile == 0) {
						end_Loop = true;
						break;
					}
					if (connect_client(buff, online, user_Size) == 0) {
						printf("The person is not online\n");
					}
					announce_Presence(args, argv, &online[0]);
			} 
			if (pdf[1].revents & POLLIN) {
				int client = capture_Presence(pdf[1].fd, online, &user_Size);
				printf("%s %s\n", online[client].status,online[client].name);
											
			}	

			if (pdf[2].revents & POLLIN) {
				accept_client_msg(pdf[2].fd, online, user_Size);
			}
		//}
	}
	broadcast_Offline(pdf[1].fd, &online[0]);

}
