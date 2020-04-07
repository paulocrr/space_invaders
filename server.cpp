#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iterator> 
#include <string.h>
#include <thread> 
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//g++ -std=c++0x -o server.exe server.cpp
using namespace std;
struct sockaddr_in stSockAddr;
struct sockaddr_in cli_addr;
bool termino = false;
map<string,int> users;

char  p[100][100];
int c,f,x=0;

int playerx=8;
int playery=12;
char temp;
char temp1;
char ga='y';
int writeInSocket(int connectFd,string text){
	int n;
	n = write(connectFd,text.c_str(),text.size());
	return n;
}


void writeToSocket(int ConnectFD,char buffer[1025]){
	int n;
	char c;
	string str;
	do{
		//cout << "MSG: ";
		temp1=p[playerx][playery];
		if(str.size()>0){
			str = buffer;
			n = write(ConnectFD,str.c_str(),str.size());
			if (str.compare("END")==0)
				termino=true;
		}
	}while(!termino);
}



void readFromSocket(int ConnectFD){
	int n;
	char buffer[5];
	string str;
	string str2;
	do{
		n = read(ConnectFD,buffer,1);
		if( buffer[0] == '['){		//"[1 04 manu]"
				read(ConnectFD,buffer,1);
				if ( buffer[0] == '1' ){
					n= read(ConnectFD,buffer,3); // read size 
					buffer[n]='\0';
					int tamano = atoi(buffer);
					tamano++;
					n= read(ConnectFD,buffer,tamano); // read nickname 
					buffer[n]='\0';
					map<string,int>::iterator it;
					it = users.find(buffer);
					if (it != users.end()){
						break;
					}
					users[buffer]=ConnectFD;
					
					
					
				}
		}
		else{	//" 0215" hay un espacio para diferenciar el login
			n = read(ConnectFD,buffer,1024);
			if(n>0){
				str.clear();
				cout<<n<<endl;
				buffer[n]='\0';
				cout << "(Client Command: )" << buffer << endl;
				str2 = buffer;
				//playerx = stoi(str.substr(0,2));
				//playery = stoi(str.substr(2,2));
				
				map<string,int>::iterator it;
				it=users.begin();
				for(;it!=users.end();++it){
					if(it->second!=ConnectFD){
						str=str+it->first+' ';
						str=str+str2;
						write(it->second,str.c_str(),str.size());
					}
				}
			}
		}	
	}while(!termino);
}

void printMap(){
	strcpy(p[x++],"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||");
	strcpy(p[x++],"||||||||||||||||||||||||||||||||  |||||||||        |||||||||");
	strcpy(p[x++],"|||||| ||||||||||||||||||||||||    ||||||||        |||||||||");
	strcpy(p[x++],"|||||   ||||||||||||||||||||||      ||||||||        ||||||||");
	strcpy(p[x++],"||||     ||||||||||||||| ||||        |||||||||       ||  | |");
	strcpy(p[x++]," ||      ||||||||| ||||   |||         |||||||            |  ");
	strcpy(p[x++]," |        ||||||    ||    |||         ||||||                ");
	strcpy(p[x++],"          |||||            |          ||||||                ");
	strcpy(p[x++],"           |||                         ||||                 ");
	strcpy(p[x++],"           ||                          ||||                 ");
	strcpy(p[x++],"                                        ||                  ");
	strcpy(p[x++],"                                        ||                  ");
	strcpy(p[x++],"                                                            ");
	strcpy(p[x++],"                                --                          ");
	strcpy(p[x++],"      |                        |##|                    |    ");
	strcpy(p[x++],"     |#|                      |####|            |      |    ");
	strcpy(p[x++],"    |###|               |    |######|          |#|____|#|   ");
	strcpy(p[x++],"   |####|         |    |#|   |#######|       |###########|  ");
	strcpy(p[x++],"  |######|      |##|  |##|   |########|      |############| ");
	strcpy(p[x++],"|########|     |####-|####| |########|      |##############|");
	strcpy(p[x++],"##########|   |###########||###########|    |###############");
	strcpy(p[x++],"##########|  |#########################|    |###############");
	strcpy(p[x++],"###########-|###########################|  |################");
	strcpy(p[x++],"########################################|  |################");
	temp=' ';
	temp1 = p[playerx][playery];
	while(1){
		for(f=0;f<=24;f++){
        	p[f][60]=p[f][0];
		}
		if (0==playery){
			p[playerx][60]=temp1;
			temp1=temp;
		}
		for(f=0;f<=24;f++){
			for(c=0;c<=60;c++){
				if(f==playerx && c+1==playery)
					p[f][c]=temp1;
				else
					p[f][c]=p[f][c+1];
			}
		}
		if(playery>0){
			p[playerx][playery]=temp1;
			p[playerx][playery--]=ga;
			
			temp1=p[playerx][playery];
		}
		else{
			p[playerx][playery]=ga;
			temp=p[playerx][playery+1];
		}
		for(f=0;f<=24;f++){
			printf("%s\n",p[f]);
		}
		
		sleep(200000);
	}
}

int main(void){
	
	socklen_t client;
	int SocketFD;

	if ((SocketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			exit(1);    
	}
	
	if (setsockopt(SocketFD,SOL_SOCKET,SO_REUSEADDR,"1",sizeof(int)) == -1) {
			perror("Setsockopt");
			exit(1);
	}

	memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
			
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(40002);
	stSockAddr.sin_addr.s_addr = INADDR_ANY;


	if (bind(SocketFD, (struct sockaddr *)&stSockAddr, sizeof(struct sockaddr))  == -1) {
			perror("Unable to bind");
			exit(1);
	}

	if (listen(SocketFD, 5) == -1) {
			perror("Listen");
			exit(1);
	}
	cout<<"Servidor Iniciado v2"<<endl;
   
	for(;;)
	{
		 
		
		int ConnectClient = accept(SocketFD, NULL, NULL);
		//std::thread(printMap).detach();
		std::thread (readFromSocket,ConnectClient).detach();
		//std::thread (writeToSocket,ConnectClient).detach(); 
	}

	close(SocketFD);
	return 0;
}