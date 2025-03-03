#include <sys/socket.h>
#include <cstdio>


int main() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(5002);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	return 0;
}