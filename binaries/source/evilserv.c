#define _WIN32_WINNT 0x501

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define VERSION "0.0.1"
#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "666"

void Function1(char *Input);
DWORD WINAPI ConnectionHandler(LPVOID CSocket);

int main( int argc, char *argv[] ) {
	char PortNumber[6];
	const char Usage[94] = "Usage: %s [port_number]\n\nIf no port number is provided, the default port of %s will be used.\n";
	if ( argc > 2) {
		printf(Usage, argv[0], DEFAULT_PORT);
		return 1;
	} else if ( argc == 2 ) {
		if ( (atoi(argv[1]) > 0) && (atoi(argv[1]) < 65536) && (strlen(argv[1]) < 7) ) {
			strncpy(PortNumber, argv[1], 6);
		} else {
			printf(Usage, argv[0], DEFAULT_PORT);
			return 1;
		}
	} else {		
		strncpy(PortNumber, DEFAULT_PORT, 6);
	}
	printf("Starting evilserver version %s\n", VERSION);
	BasicFunc1(); // Call function from external dll
	printf("\nThis is vulnerable software!\nDo not allow access from untrusted systems or networks!\n\n");
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET,
	ClientSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, hints;
	int Result;
	struct sockaddr_in ClientAddress;
	int ClientAddressL = sizeof(ClientAddress);

	Result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (Result != 0) {
		printf("WSAStartup failed with error: %d\n", Result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	Result = getaddrinfo(NULL, PortNumber, &hints, &result);
	if ( Result != 0 ) {
		printf("Getaddrinfo failed with error: %d\n", Result);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	Result = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (Result == SOCKET_ERROR) {
		printf("Bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	Result = listen(ListenSocket, SOMAXCONN);
	if (Result == SOCKET_ERROR) {
		printf("Listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}		

	while(ListenSocket) {	
		printf("Waiting for client connections...\n");

		ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientAddress, &ClientAddressL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("Accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		printf("Received a client connection from %s:%u\n", inet_ntoa(ClientAddress.sin_addr), htons(ClientAddress.sin_port));
		CreateThread(0,0,ConnectionHandler, (LPVOID)ClientSocket , 0,0);
		
	}

	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}

void Function1(char *Input) {
	char Buffer2S[2000];	
	strcpy(Buffer2S, Input);
}


DWORD WINAPI ConnectionHandler(LPVOID CSocket) {
	int RecvBufLen = DEFAULT_BUFLEN;
	char *RecvBuf = malloc(DEFAULT_BUFLEN);
	char BigEmpty[1000];
	int Result, SendResult, i, k;
	memset(BigEmpty, 0, 1000);
	memset(RecvBuf, 0, DEFAULT_BUFLEN);
	SOCKET Client = (SOCKET)CSocket; 
	SendResult = send( Client, "Welcome to Evil Server! Enter HELP for help.\n", 51, 0 );
	if (SendResult == SOCKET_ERROR) {
		printf("Send failed with error: %d\n", WSAGetLastError());
		closesocket(Client);
		return 1;
	}
	while (CSocket) {
		Result = recv(Client, RecvBuf, RecvBufLen, 0);
		if (Result > 0) {
			if (strncmp(RecvBuf, "HELP ", 5) == 0) {
				const char NotImplemented[47] = "Command specific help has not been implemented\n";
				SendResult = send( Client, NotImplemented, sizeof(NotImplemented), 0 );
			} else if (strncmp(RecvBuf, "HELP", 4) == 0) {
				const char ValidCommands[251] = "Valid Commands:\nHELP\nSUPERSECUREFUNC [ssf_value]\nSUPERSECUREFUNC2 [ssf2_value]\nEXIT\n";
				SendResult = send( Client, ValidCommands, sizeof(ValidCommands), 0 );
			} else if (strncmp(RecvBuf, "SUPERSECUREFUNC ", 5) == 0) {
				char *TrunBuf = malloc(3000);
				memset(TrunBuf, 0, 3000);
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '.') {
						strncpy(TrunBuf, RecvBuf, 3000);				
						Function1(TrunBuf);
						break;
					}
				}
				memset(TrunBuf, 0, 3000);				
				SendResult = send( Client, "COMMAND COMPLETED\n", 18, 0 );
			} else if (strncmp(RecvBuf, "SUPERSECUREFUNC2 ", 5) == 0) {
				char GmonStatus[25] = "SUPERSECUREFUNC2 STARTED\n";
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '/') {
						if (strlen(RecvBuf) > 3950) {
							Function1(RecvBuf);
						}
						break;
					}
				}				
				SendResult = send( Client, GmonStatus, sizeof(GmonStatus), 0 );
			} else if (strncmp(RecvBuf, "EXIT", 4) == 0) {
				SendResult = send( Client, "GOODBYE\n", 8, 0 );
				printf("Connection closing...\n");
				closesocket(Client);
				return 0;
			} else {
				SendResult = send( Client, "UNKNOWN COMMAND\n", 16, 0 );
			}
		} else if (Result == 0) {
			printf("Connection closing...\n");
			closesocket(Client);
			return 0;			
		} else  {
			printf("Recv failed with error: %d\n", WSAGetLastError());
			closesocket(Client);
			return 1;
		}
	}
}
