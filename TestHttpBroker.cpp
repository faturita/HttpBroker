#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <winsock.h>

#define GET			1
#define LOCATION	2
#define HTTPREF		3
#define ANYTHING	4

char HTDOCS[256]="Q:\\JAVA\\VISA\\TERMINALESCAPTURA\\CLIENT\\HTDOCS\\";

void getParameter(char *parameters, char *data, int inx)
{
	int auxInx=0;
	while (data[inx]!=' ' && data[inx]!='\r') {
		parameters[auxInx++]=data[inx++];
	}

	parameters[auxInx]='\0';

}

int getToken(char *data, char *parameters, int inx, int status) 
{
	// GET LOCATION HTTPREF CTRL (ANYTHING CRLF)*

	int auxInx=0;
	char auxData[256];

	while (data[inx]!=' ' && data[inx]!='\r') {
		auxData[auxInx++]=data[inx++];
	}

	auxData[auxInx]='\0';inx++;
	
	if (strcmpi(auxData,"GET")==0) {
		getParameter(parameters, data, inx);
		return GET;
	}
	else return ANYTHING;
}


char * getExtension(char *sExtension, const char *fileName)
{
	int inx=0;
	int inxAux=0;

	char *sAux;
	sAux = strstr(fileName, ".");
	strcpy(sExtension, sAux+1);
	return sExtension;
}

void fileHeader(char *fileName, int socket)
{
	char sExtension[256];
	char sContentType[256];
	char sContentTypeToSend[256];

	getExtension(sExtension, fileName);

	if (strcmpi(sExtension, "GIF")==0)
		strcpy(sContentType, "image/gif");
	if (strcmpi(sExtension, "JPG")==0)
		strcpy(sContentType, "image/jpg");
	if (strcmpi(sExtension, "TXT")==0)
		strcpy(sContentType, "text/html");
	if (strcmpi(sExtension, "HTM")==0)
		strcpy(sContentType, "text/html");
	if (strcmpi(sExtension, "HTML")==0)
		strcpy(sContentType, "text/html");
	else 
		strcpy(sContentType, "text/x-text");

	strcpy(sContentTypeToSend, "Content-Type: ");
	strcat(sContentTypeToSend, sContentType);
	strcat(sContentTypeToSend, "\n\r\n\r");

	send(socket, sContentTypeToSend,strlen(sContentTypeToSend),0);
	
}


void fileToHtml(FILE *pf, int socket)
{
	char data[256];

	fgets(data, 256, pf);
	
	while (!feof(pf)) {
		send(socket, data, strlen(data), 0);
		fgets(data, 256, pf);
	}
}

void fileNotFound(int socket)
{
	char data[2048];

	send(socket, "Content-type: text/html\n\r\n\r",27,0);

	strcpy(data,"<html><head><title>404 - NotFound</title></head><body><font face=\"Verdana\" size=\"20\"><b>Not Found</b></font></body></html>");
	send(socket, data, strlen(data),0);

}

void parseHTTPHeader(char *data, int socket)
{
	char parameters[256];
	int status=GET;
	int inx=0;
	
	if (getToken(data, parameters, inx, status)==GET)
	{
		char file[256];
		strcpy(file,"");
		strcat(file, HTDOCS);
		strcat(file, parameters+1);
		FILE *pf = fopen(file,"r");
		//MessageBox(0,file,"Error",MB_OK);
		if (pf == NULL) {
			fileNotFound(socket);
			return;
		}
		
		fileHeader(file, socket);
		fileToHtml(pf, socket);

		fclose(pf);
	}

	
}

void standAsServer() 
{
	char data[2048];
	int port=80;

	// Inicialización de las estructuras para habilitar la conexion.
	int sd=socket(AF_INET,SOCK_STREAM,0);
	if (sd == -1) {
		MessageBox( 0, "Error al conectarse.", "Errores",MB_OK);
		return ;
	}

	struct sockaddr_in addr;
	addr.sin_port=htons(port);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addr.sin_addr.s_addr = addr_list[0];
		
	if (bind(sd, (sockaddr*)&addr, sizeof(addr))==-1) {
		MessageBox (0, "Cannot bind socket sctructure.","Error",MB_OK);
		return ;
	}


	if (listen(sd, 6)==-1) {
		MessageBox(0,"Error when establishing listenning queue.","Error",MB_OK);
		return;
	}


	while (true) {
		struct sockaddr_in raddr;
		raddr.sin_port=htons(port);
		raddr.sin_family=AF_INET;
		raddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		int addrlen=sizeof(raddr);
		int cnSocket;

		if ( (cnSocket = accept(sd, (sockaddr *)&raddr, &addrlen))==-1) {
			MessageBox (0,"Error when accepting connection.","ERROR",MB_OK);
			return;
		}
	

		//MessageBox (0, "Connection complete", "ok",MB_OK);


		int len = recv(cnSocket,data,2048,0);
		data[len]='\0';

		MessageBox(0,data,"HTTP HEADER",MB_OK);
	
		send(cnSocket, "HTTP/1.0\n\r",12,0);

		parseHTTPHeader(data, cnSocket);

		//strcpy(data, "<html><head><title>Error</title></head><body><p size=\"20\">Forbidden</p></body></html>");
		//send(cnSocket, data, strlen(data), 0);
		
		closesocket(cnSocket);

	}

	closesocket(sd);

	//MessageBox (0, data, "Error", MB_OK);
}

void standAsClient() 
{
	char data[2048];
	int port=80;


	// Inicialización de las estructuras para habilitar la conexion.
	int sd=socket(AF_INET,SOCK_STREAM,0);
	if (sd == -1) {
		MessageBox( 0, "Error al conectarse.", "Errores",MB_OK);
		return ;
	}

	struct sockaddr_in addr;
	addr.sin_port=htons(port);
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = inet_addr("172.16.0.20");
	//addr.sin_addr.s_addr = addr_list[0];
		
	if (connect(sd,(sockaddr*)&addr,sizeof(addr))==-1) {
		//MessageBox (0, "No puede establecerse conexión con el servidor.","Error", MB_OK);
		return ;
	}

	//send(sd, "GET / HTTP/1.0\n\rAccept: application/msword\n\rAccept-language: es-ar\n\rUser-Agent: Mozilla/4.0\r\nHost: NTS_VSIJACAC\r\nConnection: Keep-Alive\n\r\n\r",155,0);
	send(sd, "GET http://www.microsoft.com/ HTTP/1.0\n\rAccept: application/msword\n\rAccept-language: es-ar\n\rUser-Agent: Mozilla/4.0\r\nHost: www.microsoft.com\r\nProxy-Connection: Keep-Alive\n\r\n\r",186,0);

	int len = recv(sd,data,2048,0);
	data[len]='\0';
	
	//MessageBox (0, data, "Error", MB_OK);
}



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
			
	int status;

	WSADATA WSAData;       
	
	// WinSock initialization.
	if ((status = WSAStartup(MAKEWORD(1,1), &WSAData)) == 0) {
		MessageBox( 0, WSAData.szDescription, WSAData.szSystemStatus, MB_OK);       
	}       
	
	hostent *ht=gethostbyname("172.16.0.122");
	
	// Query a client and get the result.
	//standAsClient();

	// Query a server and get the result.
	standAsServer();

	
	WSACleanup();
	return 0;
}
