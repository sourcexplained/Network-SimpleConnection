#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

int main(int argc, char* argv[]){
	// Windows dependencies
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); // initiates the use of the Windows Sockets DLL

	// Server information socket + addresss
	SOCKET srvSocket; // server socket to receive incomming calls
	sockaddr_in srvAddr; // server address
	SOCKET cltSocket; // socket to handle communication to a specific client
	sockaddr_in cltAddr; // client address
	int cltAddrSize = sizeof(cltAddr); // size of cltAddr
	int msgSize;
	// buffer definition for incoming data
#define BUFFER_SIZE 128
	char buffer[BUFFER_SIZE];

	// create a TCP socket information
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	// Define the local address
	srvAddr.sin_family = AF_INET; // Indicated Internet Protocol (IP)
	srvAddr.sin_addr.s_addr = INADDR_ANY; // Server side address wildcard
	srvAddr.sin_port = htons(8233); // port definition

	// htons stands for "host to network short"
	// The function htons converts the short int into a network safe/neutral architecture
	// from the native byte structure architecture from the host to a standard network
	//
	// OS may organize bytes in different orders
	// the most used are BIG ENDIAN (Unix based) and LITTLE ENDIAN (Windows)
	// Check working class slides

	// Associate the socket with the address we just defined
	bind(srvSocket, (sockaddr*)&srvAddr, sizeof(srvAddr));

	// active the socket (open the port), this will allow clients to connect to it
	listen(srvSocket, 8); // 8 defines the max size of waiting queue for incoming calls

	// Accept a new incomming connection
	// ATTN: accept is a blocking function, it will block this thread
	// until a new connection is received
	cltSocket = accept(srvSocket, (sockaddr*)&cltAddr, &cltAddrSize);
	// Accept will return a local socket which is responsible to communicate with the
	// incoming client
	// Note after this srvSocket will be available to receive new incoming calls

	// now translate it back to understandable IP Address and print it
	inet_ntop(AF_INET, &(cltAddr.sin_addr), buffer, INET_ADDRSTRLEN);
	printf("New connection received from %s:%d!\n", buffer, ntohs(cltAddr.sin_port));

	// now translate it back to understandable IP Address and print it
	inet_ntop(AF_INET, &(cltAddr.sin_addr), buffer, INET_ADDRSTRLEN);
	printf("New connection received from %s:%d!\n", buffer, ntohs(cltAddr.sin_port));


	// receive incomming String message (null terminated char sequence) from this client 
	msgSize = 0;
	do{
		msgSize += recv(cltSocket, &(buffer[msgSize]), BUFFER_SIZE - msgSize, 0);
	} while (buffer[msgSize - 1] != '\0');

	printf("Message received:%s\n", buffer);

	// send a welcome message
	strcpy_s(buffer, "Welcome");
	send(cltSocket, buffer, strlen(buffer) + 1, 0);

	// close sockets
	closesocket(cltSocket);
	closesocket(srvSocket);

	// shutdown Windows dependencies
	WSACleanup();
	return 0;
}