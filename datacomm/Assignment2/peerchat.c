#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* ADD error checking! */




/*requried message contents

ALL need to have a field specifying what they are doing
* 	use an unsigned char

1/2=join message: userdata--fixed size
* 	1==give ip list, 2==dont give ip list

3=response to join: userdata, int#peers, ip's
* 4=no-ip's response

*/

typedef struct{
	char username[12];
	int age;
	int zip;
	int port;
	uint32_t addr;
}userData;

//struct for use with age, who, and zip. is a linkedlist node.
struct peerinfo_st{
	userData data;
	int thisFD;
	struct peerinfo_st *next;
};
typedef struct peerinfo_st peerinfo;

typedef struct{
	int numPeers;
	peerinfo *peers;
}peerRoot;

peerRoot *peerList;
char username[12];
int myzip;
int myage;
int myport=1100;
uint32_t myaddr;
fd_set readlistOrig;
int maxFd;


void addPeer(userData peer, int socketFD){
	peerinfo *temp=peerList->peers;
	peerinfo *tempParent=NULL;
	while(temp!=NULL){
		tempParent=temp;
		temp = temp->next;
	}
	temp = malloc(sizeof(peerinfo));
	temp->data = peer;
	temp->thisFD=socketFD;
	temp->next=NULL;
	if(tempParent==NULL){
		peerList->peers = temp;
	}else{
		tempParent->next = temp;
	}
	peerList->numPeers++;
}

void remPeer(int socketFD){
	peerinfo *temp=peerList->peers;
	peerinfo *tempParent=NULL;
	int depth=1;
	while(temp!=NULL){
		if(temp->thisFD==socketFD){//correct peer to remove
			struct in_addr remaddr;
			remaddr.s_addr = temp->data.addr;
			printf("<%s>@<%s> left chat\n", temp->data.username, inet_ntoa(remaddr));
			if(peerList->numPeers==1){//only peer
				peerList->numPeers--;
				free(peerList->peers);
				peerList->peers=NULL;
				return;
			}else{
				if(depth==1){
					peerList->peers=temp->next;
					peerList->numPeers--;
					free(temp);
					return;
				}else{
					tempParent->next=temp->next;
					peerList->numPeers--;
					free(temp);
					return;
				}
			}
			return;
		}else{//not the correct one
			tempParent=temp;
			temp=temp->next;
			depth++;
		}
	}
}

void commandErr(){
	printf("invalid command, commands are : join, leave, who, zip, age, exit\n");
}

void join(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	int port;
	char *addr;
	word=strtok(buff, " ");
	if(strcmp(word, "/join")!=0){
		commandErr();
	}else{
		if(peerList->numPeers!=0){
			printf("join failure - already in a chat session\n");
		}
		word=strtok(NULL, " ");
		if(word==NULL){
			printf("proper command usage is /join [-p <port>] <ip address>\n");
			return;
		}
		if(strcmp(word, "-p")==0){//port chosen
			word=strtok(NULL, " ");
			if(word==NULL){
				printf("proper command usage is /join [-p <port>] <ip address>\n");
				return;
			}
			port=atoi(word);
			word=strtok(NULL, " ");
			if(word==NULL){
				printf("proper command usage is /join [-p <port>] <ip address>\n");
				return;
			}
		}else{
			port=myport;
		}
		addr=word;
		//connect to target first
		struct sockaddr_in sa;
		struct in_addr targIP;//network form ip address
		if(0==inet_aton(addr, &targIP)){
			printf("failed to convert address to in_addr\n");
			return;
		}
		int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0){
			printf("failed to set sockopt reuseaddr\n");
			perror(NULL);
		}
		memset(&sa, 0, sizeof sa);
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port); 
		sa.sin_addr = targIP;
		int connected = connect(SocketFD,(struct sockaddr *)&sa, sizeof sa);
		if(connected==-1){
			printf("join failure-");
			perror(NULL);
			return;
		}
		//send ipreq join message
		unsigned char *sendBuf = malloc(sizeof(unsigned char)*1000);
		unsigned char *recBuf = malloc(sizeof(unsigned char)*1000);
		int *code = (int *)sendBuf;
		*code = 1;
		userData *selfdata=(userData *)&sendBuf[4];
		selfdata->age=myage;
		selfdata->zip=myzip;
		selfdata->port=myport;
		memcpy(selfdata->username, username, 12);
		////////////////////////////////////////////////own ip will be read at target
		int writelength = write(SocketFD, sendBuf, 4+sizeof(userData));
		if(writelength<0){
			printf("failed to write, error: ");
			perror(NULL);
			return;
		}
		int readlength = read(SocketFD, recBuf, 1000);
		if(readlength<0){
			printf("failed to read, error: ");
			perror(NULL);
			return;
		}
		int *readcode = (int *)recBuf;
		if(*readcode!=3){
			printf("something wrong with response from target..readcode=%i, readlength=%i\n", *readcode, readlength);
		}
		userData *readdata=(userData *)&recBuf[4];
		userData tempdata;
		tempdata.age=readdata->age, tempdata.zip=readdata->zip;
		memcpy(tempdata.username, readdata->username, 12);//copy ip address too
		tempdata.addr=targIP.s_addr;
		tempdata.port=readdata->port;
		int readpoint = 4+sizeof(userData);
		int *peercount = (int *)&recBuf[readpoint];
		int joinedpeers = *peercount;
		readpoint+=sizeof(int);
		//add target to peerlist
		addPeer(tempdata, SocketFD);
		/* keep track of highest descriptor for select! */
        if (SocketFD >= maxFd)
            maxFd = SocketFD + 1;
        /* add this socket to the list of descriptors watched by select */
        FD_SET(SocketFD, &readlistOrig);
		//send second join messages and add others to list
		unsigned char recbuf2[1000];
		*code=2;
		int peersadded=1;
		while(1){
			printf("					peersadded=%i\n", peersadded);
			if(*peercount<=peersadded){
				break;
			}
			unsigned char *rec2 = malloc(sizeof(unsigned char)*1000);
			uint32_t *nextaddrval = (uint32_t *)&recBuf[readpoint];
			int *nextport = (int *)&recBuf[readpoint+4];
			struct in_addr nextaddr;
			nextaddr.s_addr = *nextaddrval;
			int nextFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			if(setsockopt(nextFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0){
				printf("failed to set sockopt reuseaddr\n");
				perror(NULL);
			}
			struct sockaddr_in nextsa;
			memset(&sa, 0, sizeof nextsa);
			nextsa.sin_family = AF_INET;
			nextsa.sin_port = htons(*nextport); 
			nextsa.sin_addr = nextaddr;
			connected = connect(nextFD,(struct sockaddr *)&nextsa, sizeof nextsa);
			if(connected==-1){perror(NULL);return;}
			writelength = write(nextFD, sendBuf, 4+sizeof(userData));
			if(writelength<0){
				printf("failed to write, error: ");
				perror(NULL);
				return;
			}
			readlength = read(nextFD, rec2, 1000);
			if(readlength<0){
				printf("failed to read, error: ");
				perror(NULL);
				return;
			}
			userData *readdata2=(userData *)&rec2[4];
			userData tempdata2;
			tempdata2.age=readdata2->age, tempdata2.zip=readdata2->zip;
			memcpy(tempdata2.username, readdata2->username, 12);//copy ip address too
			tempdata2.addr=nextaddr.s_addr;
			tempdata2.port=readdata2->port;
			printf("%s added\n", tempdata2.username);
			//add target to peerlist
			addPeer(tempdata2, nextFD);
			/* keep track of highest descriptor for select! */
			if (nextFD >= maxFd)
				maxFd = nextFD + 1;
			/* add this socket to the list of descriptors watched by select */
			FD_SET(nextFD, &readlistOrig);
			
			readpoint+=8;
			peersadded++;
			free(rec2);
		}//break out when done
		printf("joined chat with <%i> members\n", joinedpeers);
		free(recBuf);
		free(sendBuf);
	}
	
}

void leave(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	word=strtok(buff, " ");
	if(strcmp(word, "/leave")!=0){
		commandErr();
	}else{
		if(peerList->numPeers<1){
			printf("not in any chat!\n");
			return;
		}
		printf("[left chat]\n");
		peerinfo *temp=peerList->peers;
		peerinfo *temp2=NULL;
		while(temp!=NULL){//clear peerlist
			temp2=temp->next;
			close(temp->thisFD);
			FD_CLR(temp->thisFD, &readlistOrig);
			free(temp);
			temp=temp2;
		}
		peerList->peers=NULL;
		peerList->numPeers=0;
	}
}

void who(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	word=strtok(buff, " ");
	if(strcmp(word, "/who")!=0){
		commandErr();
	}else{
		//print all peers..need struct holding them.
		peerinfo *temp=peerList->peers;
		while(temp!=NULL){
			printf("[%s %i %i]\n", temp->data.username, temp->data.age, temp->data.zip);
			temp=temp->next;
		}
	}
}

void zip(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	int zip;
	word=strtok(buff, " ");
	if(strcmp(word, "/zip")!=0){
		commandErr();
	}else{
		word=strtok(NULL, " ");
		if(word==NULL){
			printf("proper command usage is /zip <zipcode>\n");
			return;
		}
		zip=atoi(word);
		peerinfo *temp=peerList->peers;
		while(temp!=NULL){
			if(temp->data.zip==zip){
				printf("[%s %i %i]\n", temp->data.username, temp->data.age, temp->data.zip);
			}
			temp=temp->next;
		}
	}
}

void age(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	int age;
	word=strtok(buff, " ");
	if(strcmp(word, "/age")!=0){
		commandErr();
	}else{
		word=strtok(NULL, " ");
		if(word==NULL){
			printf("proper command usage is /age <age>\n");
			return;
		}
		age=atoi(word);
		peerinfo *temp=peerList->peers;
		while(temp!=NULL){
			if(temp->data.age==age){
				printf("[%s %i %i]\n", temp->data.username, temp->data.age, temp->data.zip);
			}
			temp=temp->next;
		}
		//print peers with specified age
	}
}

void exitComm(char *buff, int size){
	buff[size-1]=0;//avoiding having the newline cause problems
	char *word;
	int port;
	char *addr;
	word=strtok(buff, " \0\n\r");
	if(strcmp(word, "/exit")!=0){
		commandErr();
	}else{
		//send message to peers about leaving chat
		peerinfo *temp=peerList->peers;
		peerinfo *temp2;
		while(temp!=NULL){
			temp2=temp->next;
			//send message to peer
			free(temp);
			temp=temp2;
		}
		exit(EXIT_SUCCESS);
	}
}

void command(char *buff, int size){
	switch(buff[1]){
		case 'j' : join(buff, size);
			break;
		case 'l' : leave(buff, size);
			break;
		case 'w' : who(buff, size);
			break;
		case 'z' : zip(buff, size);
			break;
		case 'a' : age(buff, size);
			break;
		case 'e' : exitComm(buff, size);
			break;
		default : commandErr();
	}
}

int main(int argc, char *argv[])
{
	
	if(argc!=4&&argc!=6){
		printf("incorrect arguments, correct format is peerchat [-p port] username zipcode age\n");
		return 0;
	}else{
		if(argc==6&&strcmp(argv[1], "-p")!=0){
			printf("incorrect arguments, correct format is peerchat [-p port] username zipcode age\n");
			return 0;
		}
	}
	
	if(argc==4){//no port specified
		if(strlen(argv[1])>11){//restricting usernames to 10 characters
			printf("please keep usernames to 11 or fewer characters\n");
			return 0;
		}else{
			strcpy(username, argv[1]);
			myzip=atoi(argv[2]);
			myage=atoi(argv[3]);
		}
	}else{//port specified
		if(strlen(argv[3])>11){//restricting usernames to 10 characters
			printf("please keep usernames to 11 or fewer characters\n");
			return 0;
		}else{
			myport=atoi(argv[2]);
			strcpy(username, argv[3]);
			myzip=atoi(argv[4]);
			myage=atoi(argv[5]);
		}
	}
	printf("user %s, port %i, zip %i, age %i\n", username, myport, myzip, myage);
    struct sockaddr_in sa;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(setsockopt(SocketFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0){
		printf("failed to set sockopt reuseaddr\n");
		perror(NULL);
	}
    memset(&sa, 0, sizeof sa);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(myport); /* make port dynamic */
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(SocketFD,(struct sockaddr *)&sa, sizeof sa);

    listen(SocketFD, 10);

    
    maxFd = SocketFD + 1;
    FD_ZERO(&readlistOrig);

    /* monitor both console (0) & main connection socket */
    FD_SET(0, &readlistOrig);
    FD_SET(SocketFD, &readlistOrig);
    
    //create root node of peerlist
	peerList = malloc(sizeof(peerRoot));
	peerList->numPeers=0;
	peerList->peers=NULL;
	
    while (1)
    {
        int num;
        char buff[1000];
        fd_set readlist;

        /* make a copy, since select alters the list passed to it */
        readlist = readlistOrig;

        /* wait for something to happen */
        num = select(maxFd, &readlist, 0, 0, 0);
        
        /* 
         * select modified readlist to set only bits with activity,
         * and told us how many had activity
         */


        /* was it input from the console? */
        if (FD_ISSET(0, &readlist))
        {
            num--;

            /* read the input */
            int num;
            int numread = read(0, buff, sizeof(buff));
            /* process text - command or data to peers? */
            if(buff[0]=='/'){
				command(buff, numread);
			}else{
				//send to all peers///////////////////////////////////////////////basic linkedlist loop, sending to that socketFD
				peerinfo *sendpeer = peerList->peers;
				peerinfo *nextsend=NULL;
				while(sendpeer!=NULL){
					nextsend=sendpeer->next;
					char text[numread];
					buff[numread-1]=0;
					strcpy(text, buff);
					int writelength = write(sendpeer->thisFD, text, strlen(text));
					if(writelength<0){
						printf("failed to write, error: ");
						perror(NULL);
					}
					sendpeer=nextsend;
				}
			}
        }

        /* was it a new connection? */
        if (FD_ISSET(SocketFD, &readlist))
        {
            num--;
            /* extract connection from queue, get new socket */
            int ConnectFD;
            struct sockaddr_in accsa;
            socklen_t addrlen = sizeof(accsa);
            ConnectFD = accept(SocketFD,(struct sockaddr *)&accsa, &addrlen);
            if(setsockopt(ConnectFD, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0){
				printf("failed to set sockopt reuseaddr\n");
				perror(NULL);
			}
            /* keep track of highest descriptor for select! */
            if (ConnectFD >= maxFd)
                maxFd = ConnectFD + 1;

            /* add this socket to the list of descriptors watched by select */
            FD_SET(ConnectFD, &readlistOrig);

            /* do what is necessary when new peer joins */
            //add new peer info to linkedlist
            userData newdata;
            newdata.addr = accsa.sin_addr.s_addr;
            unsigned char *sendBuf = malloc(sizeof(unsigned char)*1000);
			unsigned char *recBuf = malloc(sizeof(unsigned char)*1000);
            int readlength = read(ConnectFD, recBuf, 1000);
            if(readlength<0){
				printf("failed to write, error: ");
				perror(NULL);
			}
            userData *recdata = (userData *)&recBuf[4];
            memcpy(newdata.username, recdata->username, 12);
            newdata.age=recdata->age;
            newdata.zip=recdata->zip;
            newdata.port=recdata->port;
            addPeer(newdata, ConnectFD);
            char *newip = inet_ntoa(accsa.sin_addr);
            printf("member joined: <%s>@<%s> <%i> <%i>\n", newdata.username, newip, newdata.zip, newdata.age);
            ////////////////////////////////////////////////////////////////////////////////
            
            userData *senddata = (userData *)&sendBuf[4];
            memcpy(senddata->username, username, 12);
            senddata->age=myage;
            senddata->zip=myzip;
            senddata->port=myport;
            
            int readpoint=4+sizeof(userData);
            int *code = (int *)sendBuf;
			int *readcode = (int *)recBuf;
			int totalwrote=4+sizeof(userData);
            if(*readcode==1){//needs iplist
				*code=3;
				//add ip's
				int *sendcount = (int *)&sendBuf[readpoint];
				totalwrote+=4;
				readpoint+=sizeof(int);
				*sendcount = peerList->numPeers;
				peerinfo *tempinfo = peerList->peers;
				for(int i=0;i<*sendcount;i++){
					if(tempinfo==NULL){
						break;
					}else{
						uint32_t *sendaddr = (uint32_t *)&sendBuf[readpoint];
						int *sendport = (int *)&sendBuf[readpoint+4];
						*sendaddr = tempinfo->data.addr;
						*sendport = tempinfo->data.port;
						readpoint+=sizeof(uint32_t)+4;
						tempinfo = tempinfo->next;
						totalwrote+=8;
					}
				}
				
			}else{
				*code=4;
			}
            int writelength = write(ConnectFD, sendBuf, totalwrote);
            if(writelength<0){
				printf("failed to write, error: ");
				perror(NULL);
			}
            free(recBuf);
            free(sendBuf);
            
            //send required data for new peer to build their own list
        }

        /* walk through list of descriptors, checking for activity */
        for (int i=SocketFD+1; num && i<maxFd; i++)
        {
            /* was there activity on this descriptor? */
            if (FD_ISSET(i, &readlist))
            {
                int count;
                count = read(i, buff, sizeof(buff));

                /* if read got nothing, connection went away */
                if (count <= 0)
                {
                    close(i);

                    /* remove this descriptor from list*/
                    FD_CLR(i, &readlistOrig);

                    /* could/should reduce value maxFd */
                    if(i==maxFd){
						maxFd--;
					}

                    /* handle peer leaving */
                    //remove data from peerlist
                    remPeer(i);
                }
                else
                {
                    /* handle data from peer */
                    peerinfo *temp=peerList->peers;
                    buff[count]=0;
					while(temp!=NULL){
						if(temp->thisFD==i){
							printf("<%s> ", temp->data.username);
						}
						temp=temp->next;
					}
                    printf("%s\n", buff);//will have messages sent with null terminators
                }
            }
        }
    }

    close(SocketFD);
    return 0;  
}
