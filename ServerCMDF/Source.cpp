#define _WINSOCK_DEPRCATED_NO_WARNINGS  // Odignoruje warnings, diky kterym nelze provest kompilaci (Nejaka zastarala API warnings podle google)
#define _CRT_SECURE_NO_WARNINGS         // Odignoruje warnings, diky kterym nelze provest kompilaci (Nejaka zastarala API warnings podle google)
#pragma comment (lib, "ws2_32.lib")     // Link knihovny Winsock2

#define PORT 50000 // Definice portu

// Potrebne importy

#include "Source.h"
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

server::server()
{

	// Inicializace WINSOCK
	WORD DllVersion = MAKEWORD(2, 2); // Potreba pro WSAStartup, nejsem si jisty konkretne jaky je ucel (jakesi verzovani)
	if (WSAStartup(DllVersion, &wsaData) != 0) { // Inicializacni funkce, (verze, struktura socketu)
		cout << "Nemuzu inicializovat WinSock2";
		ExitProcess(EXIT_FAILURE);
	};

	// Vytvoreni Socketu
	listening = socket(AF_INET, SOCK_STREAM, 0); // Socket funkce vytvori socket (Ipv4, oboustranne spojeni, protokol(0 je asi nejaka default hodnota))
	if (listening == INVALID_SOCKET) { // Osetreni chyby
		cout << "Nemuzu vytvorit socket";
		ExitProcess(EXIT_FAILURE);
	};


	sin.sin_family = AF_INET; // "Adress family transportni adresy", pry podle internetu vzdy na hodnotu AF_INET
	sin.sin_port = htons(PORT); // Cislo portu, htons vraci hodnotu v tcp/ip bytovem poradi
	sin.sin_addr.S_un.S_addr = INADDR_ANY; // Umozni se pripojit ze vsech IP adres stejne site, respektive nespecifikuji speicifickou IP

	naslouchej(); // Server zacne naslouchat a cekat na spojeni
}

server::~server() {


	closesocket(listening); // Ukoncit socket

	WSACleanup(); // Ukoncit WinSock2

}

void server::vypisCsockety()  // Vypis klienta + socket

{
	for (int i = 0; i < pocKlientu; i++) {
		cout << "Klient cislo: " << i << " s cislem socketu: " << polesocketu[i] << endl << endl;
	}
}

void server::ulozCsockety(SOCKET cSocket) { // Ulozeni socketu + klient
	polesocketu[pocKlientu] = cSocket;
	pocKlientu++;
	pocKlientuV++;
}

void server::posliVsem(SOCKET cSocket, char buffer[4096], int Brec, int flag) { // Zaslat echo zpravy vsem pripojenym klientum
	for (int i = 0; i < 100; i++)

	{
		int a = cSocket;
		if (polesocketu[i] != a) {
			send(polesocketu[i], buffer, Brec + 1, 0);
		}
	}
}

void server::posli(SOCKET cSocket, char buffer[4096], int Brec, int flag)  // Zaslat echo zpravy konkretnimu klientu (pro muj debugg)

{
	send(cSocket, buffer, Brec + 1, 0);
}

void server::th_client(SOCKET cSocket, sockaddr_in client, int cSize, PCHAR host, PCHAR service, int* polesocketu) // Funkce kde budeme pracovat s klientem
{

	ulozCsockety(cSocket); //  Ulozeni socketu + klient



	if (getnameinfo((sockaddr*)&client, cSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) // Nezavisle na protokolu
		cout << host << " Pripojen na portu: " << service << " Pripojen na socketu: " << cSocket << endl;


	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // IPV4 nebo IPV6
		cout << host << " Pripojen na portu: " << service << " Pripojen na socketu: " << cSocket << endl;
	}

	// Cyklus, naslouchat a odesilat echo zpravy ke klientovy
	char buffer[4096]; // Promenna pro zpravu

	while (true) {
		ZeroMemory(buffer, 4096); // Nastavi obsah pameti promenne na nuly

		int Brec = recv(cSocket, buffer, 4096, 0); // Funkce dokaze ziskat  data z pripojeneho socketu (socket, buffer, velikost, flag = upravuje chovani funkce, 0 je asi default)
		if (Brec == INVALID_SOCKET) {
			cout << "Klient " << cSocket << " odpojen" << endl;

			closesocket(cSocket);
			// Odpojeni socketu
			break;
		}

		if (Brec == 0) {
			cout << "Klient " << cSocket << " odpojen" << endl;

			closesocket(cSocket);
			// Odpojeni socketu
			break;
		}


		posliVsem(cSocket, buffer, Brec + 1, 0); // Echo vsem klientum

		cout << cSocket << ": " << buffer << endl; // Vypis buffer promene na server do konzole

	}

	// Ukoncit socket a tim padem i odebrat jednotku z pole klientu
	closesocket(cSocket);
	pocKlientu--;
}



void server::naslouchej() { // Pripojeni + thready



	bind(listening, (sockaddr*)&sin, sizeof(sin)); // Asociace lokální ip se socketem (pointer na strukturu sockadrr = lokalni adresa se socketem, delka nazvu v bytech)

	listen(listening, SOMAXCONN); // Funkce ulozi do stavu naslouchajici pro prichozi spojeni (socket, maximalni fronta cekajicich spojeni)

	while (true)
	{

		// Cekej na pripojeni
		sockaddr_in client;
		int cSize = sizeof(client);

		cSocket = accept(listening, (sockaddr*)&client, &cSize); // Funkce prijme prichozi pokus o spojeni na socket

		char host[NI_MAXHOST]; // Jmeno klienta
		char service[NI_MAXHOST]; // Port, na kterem je klient pripojen


		//ZeroMemory(host, NI_MAXHOST);
		//ZeroMemory(service, NI_MAXHOST);

		cout << "klient: " << pocKlientu << endl;


		try { // Pokusim se pro klienta vytvorit nove vlakno pro klienta a priradit do pole vlaken

			pole[tr] = new thread(&server::th_client, this, cSocket, client, cSize, host, service, polesocketu);

			tr++;


		}
		catch (string* caught) {
			cout << caught;
		}


	}
	for (int i = 0; i < tr; i++) {
		pole[i]->join();
	}
	//pole[tr]->join();
	// Ceka se na dokonceni vsech operaci v konkretnim threadu

	WSACleanup(); // Ukonceni funkcnosti Winsock

}




int main(const int argc, const char* argv[])
{
	cout << " <------------> " << endl;
	cout << "| Chatt server |" << endl;
	cout << " <------------> " << endl;
	server();


}