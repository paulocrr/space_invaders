#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <regex>


//g++ -std=c++0x -o client.exe client.cpp
#include <unistd.h>
#include <termios.h>

using namespace std;
bool termino = false;

char  p[100][100];
int c,f,x=0;

int playerx=8;
int playery=12;
char temp;
char temp1;
char ga='x';
int cambio=0;
map< string,pair<int,int> > users;
string fillWithZeros(int stringLength,int zerosQuantity){
	string msgSize = to_string(stringLength);
	int numZeros =zerosQuantity-msgSize.size();

	for(int i = 0; i<numZeros;i++){
		msgSize="0"+msgSize;
	}
	
	return msgSize;
}

void read_thread(int socketCliente) 
{
  int n;
  char buffer[1025];
  string str;
  string str2;
  int nam;
  do{
    n = read(socketCliente,buffer,1024);
	if(n>0){
		str.clear();
		cout<<n<<endl;
		buffer[n]='\0';
		cout << "(Client Command: )" << buffer << endl;
		str2 = buffer;
		int px = stoi(str.substr(0,2));
		int py = stoi(str.substr(2,2));
				
		map<string,pair<int,int>>::iterator it;
		//it=lista.begin();
		nam=str2.find(' ');
		str=str2.substr(0,nam);
		str2.erase(0,nam+1);
		it = users.find(str);
		if (it == users.end()){
			users[str]=make_pair(10,0);
		}
		it->second.first=px;
		it->second.second=py;
	}

    if (str.compare("END")==0)
      termino=true;
  }while(!termino);
}

char getch() {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
	perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
	perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
	perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
	perror ("tcsetattr ~ICANON");
	return (buf);
}

void write_thread(int socketCliente) 
{
  int n;
  char buffer[1025];
  char str;
  string str1;
  string str2;
  string spx;
  string spy;
  do{
	if(cambio==0){//login
		cout << "MSG: ";		
		getline(cin,str2); 
		n = write(socketCliente,str2.c_str(),str2.size());
		cambio=1;
	}
	else{
		str1.clear();
		str=getch();
		cout<<"com: "<<str<<endl;
		if(str=='w'){
			playerx--;
		}
		else if(str=='s'){
			playerx++;
		}
		else if(str=='a'){
			playery--;
		}
		else if(str=='d'){
			playery++;
		}
		temp1=p[playerx][playery];
		spx=to_string(playerx);
		spy=to_string(playery);
		str1=str1+' ';
		if(spx.size()<2)
			str1=str1+'0'+spx;
		else
			str1=str1+spx;
		if(spy.size()<2)
			str1=str1+'0'+spy;
		else
			str1=str1+spy;
		
		cout<<str1<<endl;
		n = write(socketCliente,str1.c_str(),str1.size());
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
	temp1=p[playerx][playery];
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
			map<string,pair<int,int>>::iterator it;
			it=users.begin();
			for(;it!=users.end();++it){
				string pa=it->first;
				p[it->second.first][it->second.second]=pa[0];
			}
		}
		for(f=0;f<=24;f++){
			printf("%s\n",p[f]);
		}
		
		sleep(200000);
	}
}




int main(void)
{
  struct sockaddr_in stSockAddr;
  int Res;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  int n;

  if (-1 == SocketFD)
  {
    perror("cannot create socket");
    exit(EXIT_FAILURE);
  }

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(40002);
  Res = inet_pton(AF_INET, "167.0.0.0", &stSockAddr.sin_addr);

  if (0 > Res)
  {
    perror("error: first parameter is not a valid address family");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }
  else if (0 == Res)
  {
    perror("char string (second parameter does not contain valid ipaddress");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
  {
    perror("connect failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

	
	playerx=8;
	playery=2;
	std::thread (printMap).detach();
	std::thread (read_thread,SocketFD).detach();
	
	std::thread (write_thread,SocketFD).detach();

	while(!termino){};

	shutdown(SocketFD, SHUT_RDWR);

	close(SocketFD);
	return 0;
}
