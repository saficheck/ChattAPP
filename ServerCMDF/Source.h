#define _WINSOCK_DEPRCATED_NO_WARNINGS // Odignoruje warnings, diky kterym nelze provest kompilaci (Nejaka zastarala API warnings podle google)
#define _CRT_SECURE_NO_WARNINGS // Odignoruje warnings, diky kterym nelze provest kompilaci (Nejaka zastarala API warnings podle google)
#pragma comment (lib, "ws2_32.lib") // Link knihovny Winsock2


#define PORT 50000 // Definice portu

// Potrebne importy

#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>
#include <string>
#include <fstream> 

using namespace std;

class server {
private:
	WSAData wsaData; // Struktura obsajujici informace ohledne implementaci windows socketu
	SOCKET listening; // Socket funkce vytvori socket (Ipv4, oboustranne spojeni, protokol(0 je asi nejaka default hodnota))
	SOCKADDR_IN sin; // Svazani IP adresy a portu k socketu
	SOCKET cSocket; // Funkce prijme prichozi pokus o spojeni na socket
	sockaddr_in client; // Svazani IP adresy a portu k socketu
	int* polesocketu = new int[100];  // Pole socketu
	int pocKlientu = 0;  // Pocet klientu
	int pocKlientuV = 0;  // Pocet klientu kvuli vypisu
	int tr = 0; // Pocet threadu
	thread** pole = new thread * [20]; // Pole threadu
	//vector<thread*> pole(20);

public:

	server(); // Server
	~server(); // Server desktruktor
	void ulozCsockety(SOCKET cSocket); //  Ulozeni socketu + klient
	void th_client(SOCKET cSocket, sockaddr_in client, int cSize, PCHAR host, PCHAR service, int* polesocketu); // Client 
	void posliVsem(SOCKET cSocket, char buffer[4096], int Brec, int flag); // Zaslat echo zpravy vsem pripojenym klientum 
	void vypisCsockety();  // Vypis klienta + socket
	void posli(SOCKET cSocket, char buffer[4096], int Brec, int flag); // Zaslat echo zpravy konkretnimu klientu
	void naslouchej(); // Pripojeni + thready


};
