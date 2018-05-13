#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*requried message contents

					NEW
message code field
* 1=initial join info share--just [1][struct]
* 2=info share response to new member///will get forwarded  [2][struct][numpeers]
* 3=chat message [3][message]
* 4=forwarded [4][uint32_t src addr][msg]
* 5=leave [5]
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
	char *address;
	//int thisFD;    //no longer needed
	struct peerinfo_st *next;
};
typedef struct peerinfo_st peerinfo;

typedef struct{
	int numPeers;
	peerinfo *peers;
}peerRoot;

peerRoot *peerList;
struct sockaddr_in forwardList[4];
int forwardSock = -1;//for sending
int listenSock=-1;//for recieving
int numForwards=0;
char username[12];
int myzip;
int myage;
int myport=10000;
uint32_t myaddr;
fd_set readlistOrig;
int maxFd;
int multiFd = -1;
struct sockaddr_in multiAddr;
struct sockaddr_in listenAddr;


void addPeer(userData peer, char *address){
	peerinfo *temp=peerList->peers;
	peerinfo *tempParent=NULL;
	while(temp!=NULL){
		if(strcmp(address, temp->address)==0){
			return;//already in
		}
		tempParent=temp;
		temp = temp->next;
	}
	temp = malloc(sizeof(peerinfo));
	char *tempaddr = malloc(sizeof(address));
	//printf("adding %s\n", address);
	memcpy(tempaddr, address, sizeof(address));
	temp->data = peer;
	//printf("addpeer addr=%i\n", (int)peer.addr);
	temp->address = tempaddr;
	temp->next=NULL;
	if(tempParent==NULL){
		peerList->peers = temp;
	}else{
		tempParent->next = temp;
	}
	peerList->numPeers++;
}

void remPeer(uint32_t addr){
	//printf("rempeer addr = %i\n", (int)addr);
	peerinfo *temp=peerList->peers;
	peerinfo *tempParent=NULL;
	int depth=1;
	while(temp!=NULL){
		if(temp->data.addr==addr){//correct peer to remove
		//printf("tempdata addr=%i, match\n", (int)temp->data.addr);
			printf("<%s>@<%s> left chat\n", temp->data.username, temp->address);
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
		//printf("tempdata addr=%i, ignoring\n", (int)temp->data.addr);
			tempParent=temp;
			temp=temp->next;
			depth++;
		}
	}
}

void commandErr(){
	printf("invalid command, commands are : join, leave, who, zip, age, exit\n");
}

void forward(char *msg, int size, uint32_t src){
	char buff[size+8];
	int *code = (int *)buff;
	uint32_t *addr = (uint32_t *)(buff+4);
	*code = 4;
	*addr = src;
	memcpy(buff+8, msg, size);
	for(int i=0;i<numForwards;i++){
		//send to this forward target
		int writelength = sendto(forwardSock, buff, size+8, 0, (struct sockaddr *) &forwardList[i], sizeof(forwardList[i]));
		if(writelength<0){
			printf("failed to write, error: ");
			perror(NULL);
			return;
		}
	}
}

void bounce(char *msg, int size, uint32_t src, uint32_t origSrc){//carry forward from 5->1->3 etc
	
	for(int i=0;i<numForwards;i++){
		//send to this forward target
		uint32_t temp = (uint32_t)forwardList[i].sin_addr.s_addr;
		if(origSrc!=temp && src!=temp){
			int writelength = sendto(forwardSock, msg, size, 0, (struct sockaddr *) &forwardList[i], sizeof(forwardList[i]));
			if(writelength<0){
				printf("failed to write, error: ");
				perror(NULL);
				return;
			}
		}
	}
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
		if(multiFd!=-1){
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
			port=myport+1;
		}
		addr=word;
		//create multicast socket and address
		struct in_addr targIP;//network form ip address
		if(0==inet_aton(addr, &targIP)){
			printf("failed to convert address to in_addr\n");
			return;
		}
		multiFd = socket(AF_INET, SOCK_DGRAM, 0);
		if(multiFd<0){
			perror("error creating multi socket");
		}
		memset(&multiAddr, 0, sizeof multiAddr);
		multiAddr.sin_family = AF_INET;
		multiAddr.sin_port = htons(port); 
		multiAddr.sin_addr = targIP;
		//setup multicast participation
		struct ip_mreq mreq;
		if (bind(multiFd, (struct sockaddr *) &multiAddr, sizeof(multiAddr)) < 0) {
			perror("bind");
			exit(1);
		}    
		mreq.imr_multiaddr.s_addr = inet_addr(addr);      
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
		if (setsockopt(multiFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
			perror("setsockopt mreq");
			exit(1);
		}
		
		char loopch = 0;
	    if(setsockopt(multiFd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0)
	    {
	       perror("Setting IP_MULTICAST_LOOP error");
	       close(multiFd);
	       exit(1);
	    }
	   else{
	      //printf("Disabling the loopback...OK.\n");
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////
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
		int writelength = sendto(multiFd, sendBuf, 4+sizeof(userData), 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
		if(writelength<0){
			printf("failed to write, error: ");
			perror(NULL);
			return;
		}
		forward(sendBuf, writelength, -1);
		//new stuff from here on needed
		int newpeers=0;
		int joinedpeers=0;
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		//if (setsockopt(multiFd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
			//perror("Error setting timeout");
		//}//so i can stop waiting for responses after a certain point in case its the first peerchat
		//if (setsockopt(forwardSock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
			//perror("Error setting timeout");
		//}
		/* keep track of highest descriptor for select! */
        if (multiFd >= maxFd)
            maxFd = multiFd + 1;
        /* add this socket to the list of descriptors watched by select */
        FD_SET(multiFd, &readlistOrig);
        int numTimedout = 0;
		while(1){
			//printf("select loop, newpeers=%i, joinedpeers=%i, numpeers=%i\n", newpeers, joinedpeers, peerList->numPeers);
			int num;
	        fd_set readlist;
	        /* make a copy, since select alters the list passed to it */
	        readlist = readlistOrig;
	        /* wait for something to happen */
	        num = select(maxFd, &readlist, 0, 0, &tv);
	        if(num<1){
				numTimedout++;
			}else{
				numTimedout=0;
			}
	        struct sockaddr_in newAddr;
	        uint32_t *readAddr = malloc(sizeof(uint32_t));
	        *readAddr = -1;
			int adderlen = sizeof(newAddr);
			int readpoint = 0;
	        if (FD_ISSET(multiFd, &readlist)){
				//printf("join response on multicast\n");
				int readlength = recvfrom(multiFd, recBuf, 16+sizeof(userData), 0, (struct sockaddr *) &newAddr, &adderlen);
				if(readlength<0){
					perror("failed read multifd:   ");
					break;//break condition if timed out
				}
				int *readcode = (int *)(recBuf+readpoint);
				if(*readcode==4){
					//printf("stripping forwarded join response, addr=%i\n", (int)*readAddr);
					readAddr = (uint32_t *)(recBuf+4);
					readpoint+=8;
					readcode = (int *)(recBuf+readpoint);
				}else{
					forward(recBuf, readlength, newAddr.sin_addr.s_addr);
				}
				if(*readcode!=2){
					printf("something wrong with response from target(code not 2)..readcode=%i, readlength=%i\n", *readcode, readlength);
				}
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				if(*readAddr==-1){
					//printf("extracting address...\n");
					tempdata.addr=(uint32_t)newAddr.sin_addr.s_addr;
					char *tempAddress = inet_ntoa(newAddr.sin_addr);
					addPeer(tempdata, tempAddress);
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = (in_addr_t)*readAddr;
					tempdata.addr=*readAddr;
					char *tempAddress = inet_ntoa(tempaddr);
					addPeer(tempdata, tempAddress);
				}
				
				readpoint +=sizeof(userData)+4;
				int *peercount = (int *)&recBuf[readpoint];
				joinedpeers = *peercount;
				//printf("recieved peercount=%i\n", joinedpeers);
				if(joinedpeers>=newpeers){
					newpeers=joinedpeers;
				}
			}
			readpoint=0;
			if (FD_ISSET(listenSock, &readlist)){
				//printf("join response on listensock\n");
				int readlength = recvfrom(listenSock, recBuf, 16+sizeof(userData), 0, (struct sockaddr *) &newAddr, &adderlen);
				if(readlength<0){
					perror("failed read listenSock:  ");
					break;//break condition if timed out
				}
				int *readcode = (int *)(recBuf+readpoint);
				if(*readcode!=4){
					printf("something wrong with response from target(code not 4)..readcode=%i, readlength=%i\n", *readcode, readlength);
				}
				uint32_t *readAddr=malloc(sizeof(uint32_t));
				*readAddr=-1;
				readAddr = (uint32_t *)(recBuf+readpoint+4);
				readpoint+=8;
				
				readcode = (int *)(recBuf+readpoint);
				if(*readcode!=2){
					printf("something wrong with response from target(code not 2)..readcode=%i, readlength=%i\n", *readcode, readlength);
				}
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				if(*readAddr==-1){
					//printf("extracting address...\n");
					tempdata.addr=(uint32_t)newAddr.sin_addr.s_addr;
					char *tempAddress = inet_ntoa(newAddr.sin_addr);
					addPeer(tempdata, tempAddress);
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = (in_addr_t)*readAddr;
					tempdata.addr=*readAddr;
					char *tempAddress = inet_ntoa(tempaddr);
					addPeer(tempdata, tempAddress);
				}
				bounce(recBuf, readlength, (uint32_t )newAddr.sin_addr.s_addr, *readAddr);
				int writelength = sendto(multiFd, recBuf, readlength, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
				readpoint +=sizeof(userData)+4;
				int *peercount = (int *)&recBuf[readpoint];
				int joinedpeers = *peercount;
				if(joinedpeers>=newpeers){
					newpeers=joinedpeers;
				}
			}
			/////////////////////////printf("reading loop\n");
			
			if(peerList->numPeers>=newpeers){
				//printf("done adding %i peers, newpeers=%i\n", peerList->numPeers, newpeers);
				break;
			}
			if(numTimedout>3){
				printf("not recieving expected responses\n");
				break;
			}
		}
		
		
		
		printf("joined chat with <%i> members\n", peerList->numPeers);
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
		if(multiFd==-1){
			printf("not in any chat!\n");
			return;
		}
		printf("[left chat]\n");
		char leavebuff[4];
		uint32_t *code = (uint32_t *)leavebuff;
		*code=5;
		int writelength = sendto(multiFd, leavebuff, 4, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
		if(writelength<0){
			printf("failed to write, error: ");
			perror(NULL);
			return;
		}
		forward(leavebuff, 4, -1);
		peerinfo *temp=peerList->peers;
		peerinfo *temp2=NULL;
		while(temp!=NULL){//clear peerlist
			temp2=temp->next;
			FD_CLR(multiFd, &readlistOrig);
			free(temp);
			temp=temp2;
		}
		peerList->peers=NULL;
		peerList->numPeers=0;
		close(multiFd);
		multiFd=-1;
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
			printf("[%s@%s %i %i]\n", temp->data.username, temp->address, temp->data.age, temp->data.zip);
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
				printf("[%s@%s %i %i]\n", temp->data.username, temp->address, temp->data.age, temp->data.zip);
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
				printf("[%s@%s %i %i]\n", temp->data.username, temp->address, temp->data.age, temp->data.zip);
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
		leave(buff, size);
		exit(EXIT_SUCCESS);
	}
}

void atExitFunc(){
	if(multiFd==-1){
		return;
	}
	char leavebuff[4];
	uint32_t *code = (uint32_t *)leavebuff;
	*code=5;
	int writelength = sendto(multiFd, leavebuff, 4, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
	if(writelength<0){
		return;
	}
	forward(leavebuff, 4, -1);
	peerinfo *temp=peerList->peers;
	peerinfo *temp2=NULL;
	while(temp!=NULL){//clear peerlist
		temp2=temp->next;
		FD_CLR(multiFd, &readlistOrig);
		free(temp);
		temp=temp2;
	}
	peerList->peers=NULL;
	peerList->numPeers=0;
	close(multiFd);
	multiFd=-1;
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
	//read input
	atexit(atExitFunc);
	if(argc%2!=0 || argc<4 || argc>14){
		printf("incorrect arguments\n");
		return 0;
	}
	int currArg=1;
	while(currArg<argc-3){
		if(strcmp(argv[currArg], "-p")==0){//listen port specified
			myport=atoi(argv[currArg+1]);
			
		}
		if(strcmp(argv[currArg], "-f")==0){//forward to here
			//printf("forwarding stuff\n");
			char *colon = strchr(argv[currArg+1], ':');
			char *forwardAddr;
			int forwardPort;
			if(colon!=NULL){//port specified
				*colon = 0;
				forwardAddr = argv[currArg+1];
				forwardPort = atoi(colon+1);
			}else{
				forwardAddr = argv[currArg+1];
				forwardPort = 10000;
			}
			//add to forwarding list
			memset(&forwardList[numForwards], 0, sizeof forwardList[numForwards]);
			forwardList[numForwards].sin_family = AF_INET;
			forwardList[numForwards].sin_port = htons(forwardPort); 
			struct in_addr targIP;//network form ip address
			if(0==inet_aton(forwardAddr, &targIP)){
				printf("failed to convert address to in_addr\n");
				return 0;
			}
			forwardList[numForwards].sin_addr = targIP;
			
			numForwards++;
		}
		currArg+=2;
	}
	FD_ZERO(&readlistOrig);

    /* monitor both console (0) & main connection socket */
    FD_SET(0, &readlistOrig);
    maxFd = 1;
	if(numForwards>0){
		listenSock = socket(AF_INET, SOCK_DGRAM, 0);
		listenAddr.sin_family = AF_INET;
	    listenAddr.sin_port = htons(myport); /* make port dynamic */
	    listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(listenSock, (struct sockaddr *) &listenAddr, sizeof(listenAddr)) < 0) {
			perror("bind");
			exit(1);
		} 
		/* keep track of highest descriptor for select! */
        if (listenSock >= maxFd)
            maxFd = listenSock + 1;
        /* add this socket to the list of descriptors watched by select */
        FD_SET(listenSock, &readlistOrig);
	}
	forwardSock = socket(AF_INET, SOCK_DGRAM, 0);
	//last 3 args
	strcpy(username, argv[currArg]);
	myzip=atoi(argv[currArg+1]);
	myage=atoi(argv[currArg+2]);
	
	printf("user %s, port %i, zip %i, age %i\n", username, myport, myzip, myage);
	
    
    //FD_SET(SocketFD, &readlistOrig);
    
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
        {//printf("activity on console\n");
            num--;

            /* read the input */
            int num;
            int numread = read(0, buff, sizeof(buff));
            /* process text - command or data to peers? */
            if(buff[0]=='/'){
				command(buff, numread);
			}else{
				if(peerList->numPeers!=0){
					char text[numread+4];
					buff[numread-1]=0;
					strcpy(&text[4], buff);
					int *code=(int *)text;
					*code = 3;
					int writelength = sendto(multiFd, text, numread+4, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
					if(writelength<0){
						printf("failed to write, error: ");
						perror(NULL);
					}
					forward(text, numread+4, -1);
					//printf("sent length = %i, strlentxt = %i\n", writelength, (int)strlen(text));
				}
			}
        }

        
        if (FD_ISSET(multiFd, &readlist))
        {/////////////////////////////////////////////////////////////////////////////////////////////////////////check if forwarded first and strip startign part
			//printf("activity on multicast socket\n");
			num--;
			struct sockaddr_in newAddr;
			unsigned char *sendBuf = malloc(sizeof(unsigned char)*1000);
			unsigned char *recBuf = malloc(sizeof(unsigned char)*1000);
			//process response
			int adderlen = sizeof(newAddr);
			int readlength = recvfrom(multiFd, recBuf, sizeof(buff), 0, (struct sockaddr *) &newAddr, &adderlen);
			if(readlength<0){
				printf("multifd=%i     ", multiFd);
				perror("error recieving a new connection~~");
				break;//break condition if timed out
			}
			uint32_t *forwardaddr = malloc(sizeof(uint32_t));
			*forwardaddr=-1;
			int *readcode = (int *)recBuf;
			int readpoint = 0;
			if(*readcode==4){
				//forwarded, strip that and set the address based on it
				forwardaddr = (uint32_t *)(recBuf+4);
				readpoint+=8;
				readcode = (int *)(recBuf+readpoint);
				//printf("stripped forward, addr=%i\n", (int)*forwardaddr);
			}else{
				forward(recBuf, readlength, newAddr.sin_addr.s_addr);
			}
			if(*readcode==2){
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				if(*forwardaddr==-1){
					//printf("extracting address...\n");
					tempdata.addr=(uint32_t)newAddr.sin_addr.s_addr;
					char *tempAddress = inet_ntoa(newAddr.sin_addr);
					addPeer(tempdata, tempAddress);
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = (in_addr_t)*forwardaddr;
					tempdata.addr=*forwardaddr;
					char *tempAddress = inet_ntoa(tempaddr);
					addPeer(tempdata, tempAddress);
				}
			}
			if(*readcode==5){
				printf("opcode 5:");
				if(*forwardaddr==-1){
					*forwardaddr = newAddr.sin_addr.  s_addr;
					//printf("replaced addr with %i", *forwardaddr);
				}
				printf("\n");
				remPeer(*forwardaddr);
			}
			if(*readcode==1){//new peer
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				char *tempAddress;
				if(*forwardaddr==-1){
					tempdata.addr=(uint32_t )newAddr.sin_addr.s_addr;
					tempAddress = inet_ntoa(newAddr.sin_addr);
					//printf("replacing addr of %i with %i\n", (int)*forwardaddr, (int)tempdata.addr);
					*forwardaddr = tempdata.addr;
				}else{
					tempdata.addr=*forwardaddr;
					struct in_addr tempaddr;
					tempaddr.s_addr = *forwardaddr;
					tempAddress = inet_ntoa(tempaddr);
				}
				addPeer(tempdata, tempAddress);
				//respond
				int *code = (int *)sendBuf;
				*code = 2;
				userData *selfdata=(userData *)&sendBuf[4];
				selfdata->age=myage;
				selfdata->zip=myzip;
				selfdata->port=myport;
				memcpy(selfdata->username, username, 12);
				int *sendCount = (int *)&sendBuf[4+sizeof(userData)];
				*sendCount = peerList->numPeers;
				////////////////////////////////////////////////own ip will be read at target
				int writelength = sendto(multiFd, sendBuf, 8+sizeof(userData), 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
				if(writelength<0){
					printf("failed to write, error: ");
					perror(NULL);
					return 0;
				}
				printf("member joined: <%s>@<%s> <%i> <%i>\n", tempdata.username, tempAddress, tempdata.zip, tempdata.age);
			}
			if(*readcode==3){//process as message
				char *tempAddress;
				if(*forwardaddr==-1){
					tempAddress = inet_ntoa(newAddr.sin_addr);
					//printf("replacing addr of %i with %i\n", (int)*forwardaddr, (int)newAddr.sin_addr.s_addr);
					*forwardaddr = newAddr.sin_addr.s_addr;
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = *forwardaddr;
					tempAddress = inet_ntoa(tempaddr);
				}
				peerinfo *temp=peerList->peers;
				while(temp!=NULL){
					if(strcmp(temp->address, tempAddress)==0){
						printf("<%s>", temp->data.username);
					}
					temp=temp->next;
				}
				//printf("recieving message\n");
				//recBuf[readlength]=0;
				char *msg = &recBuf[readpoint+4];
				printf("%s\n", msg);
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////
        }
        
        if (FD_ISSET(listenSock, &readlist))
        {
			//printf("activity on listening socket\n");
			num--;
			struct sockaddr_in newAddr;
			unsigned char *sendBuf = malloc(sizeof(unsigned char)*1000);
			unsigned char *recBuf = malloc(sizeof(unsigned char)*1000);
			//process response
			int adderlen = sizeof(newAddr);
			int readlength = recvfrom(listenSock, recBuf, sizeof(buff), 0, (struct sockaddr *) &newAddr, &adderlen);
			if(readlength<0){
				printf("listenSock=%i     ", listenSock);
				perror("error recieving a new connection~~");
				break;//break condition if timed out
			}
			uint32_t *forwardaddr = malloc(sizeof(uint32_t));
			*forwardaddr=-1;
			int *readcode = (int *)recBuf;
			int readpoint = 0;
			if(*readcode==4){
				//forwarded, strip that and set the address based on it
				forwardaddr = (uint32_t *)(recBuf+4);
				//printf("stripped forward, addr=%i\n", (int)*forwardaddr);
				readpoint+=8;
				readcode = (int *)(recBuf+readpoint);
			}else{
				printf("incoreect code recieved on listening socket\n");
				exit(0);
			}
			if(*readcode==2){
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				if(*forwardaddr==-1){
					//printf("extracting address...\n");
					tempdata.addr=(uint32_t)newAddr.sin_addr.s_addr;
					char *tempAddress = inet_ntoa(newAddr.sin_addr);
					addPeer(tempdata, tempAddress);
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = (in_addr_t)*forwardaddr;
					tempdata.addr=*forwardaddr;
					char *tempAddress = inet_ntoa(tempaddr);
					addPeer(tempdata, tempAddress);
				}
			}
			if(*readcode==5){
				if(*forwardaddr==-1){
					*forwardaddr = newAddr.sin_addr.s_addr;
				}
				remPeer(*forwardaddr);
				bounce(recBuf, readlength, (uint32_t )newAddr.sin_addr.s_addr, *forwardaddr);
				int writelength = sendto(multiFd, recBuf, readlength, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
			}
			if(*readcode==2&&multiFd!=-1){//just keep it going
				if(*forwardaddr==-1){
					//printf("replacing addr of %i with %i\n", (int)*forwardaddr, (int)newAddr.sin_addr.s_addr);
					*forwardaddr = (uint32_t )newAddr.sin_addr.s_addr;
				}
				bounce(recBuf, readlength, (uint32_t )newAddr.sin_addr.s_addr, *forwardaddr);
				int writelength = sendto(multiFd, recBuf, readlength, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
			}
			if(*readcode==1&&multiFd!=-1){//new peer
				userData *readdata=(userData *)&recBuf[readpoint+4];
				userData tempdata;
				tempdata.age=readdata->age, tempdata.zip=readdata->zip;
				memcpy(tempdata.username, readdata->username, 12);//copy ip address too
				tempdata.port=readdata->port;
				char *tempAddress;
				if(*forwardaddr==-1){
					tempdata.addr=(uint32_t )newAddr.sin_addr.s_addr;
					//printf("replacing addr of %i with %i\n", (int)*forwardaddr, (int)tempdata.addr);
					tempAddress = inet_ntoa(newAddr.sin_addr);
					*forwardaddr = tempdata.addr;
				}else{
					tempdata.addr=*forwardaddr;
					struct in_addr tempaddr;
					tempaddr.s_addr = *forwardaddr;
					tempAddress = inet_ntoa(tempaddr);
				}
				bounce(recBuf, readlength, (uint32_t )newAddr.sin_addr.s_addr, *forwardaddr);
				int writelength = sendto(multiFd, recBuf, readlength, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
				if(writelength<0){
					printf("failed to write, error: ");
					perror(NULL);
					return 0;
				}
				addPeer(tempdata, tempAddress);
				//respond
				int *code = (int *)sendBuf;
				*code = 2;
				userData *selfdata=(userData *)&sendBuf[4];
				selfdata->age=myage;
				selfdata->zip=myzip;
				selfdata->port=myport;
				memcpy(selfdata->username, username, 12);
				int *sendCount = (int *)&sendBuf[4+sizeof(userData)];
				*sendCount = peerList->numPeers;
				////////////////////////////////////////////////own ip will be read at target
				forward(sendBuf, 8+sizeof(userData), -1);
				printf("member joined: <%s>@<%s> <%i> <%i>\n", tempdata.username, tempAddress, tempdata.zip, tempdata.age);
			}
			if(*readcode==3&&multiFd!=-1){//process as message
				char *tempAddress;
				if(*forwardaddr==-1){
					tempAddress = inet_ntoa(newAddr.sin_addr);
					//printf("replacing addr of %i with %i\n", (int)*forwardaddr, (int)newAddr.sin_addr.s_addr);
					*forwardaddr = newAddr.sin_addr.s_addr;
				}else{
					struct in_addr tempaddr;
					tempaddr.s_addr = *forwardaddr;
					tempAddress = inet_ntoa(tempaddr);
				}
				bounce(recBuf, readlength, (uint32_t )newAddr.sin_addr.s_addr, *forwardaddr);
				int writelength = sendto(multiFd, recBuf, readlength, 0, (struct sockaddr *) &multiAddr, sizeof(multiAddr));
				if(writelength<0){
					printf("failed to write, error: ");
					perror(NULL);
					return 0;
				}
				peerinfo *temp=peerList->peers;
				while(temp!=NULL){
					if(strcmp(temp->address, tempAddress)==0){
						printf("<%s>", temp->data.username);
					}
					temp=temp->next;
				}
				//printf("recieving message\n");
				recBuf[readlength]=0;
				char *msg = &recBuf[12];
				printf("%s\n", msg);
			}
			
			//multicast it
			//process it
		}
    }

    return 0;  
}
