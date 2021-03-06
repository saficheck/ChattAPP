// Potrebne importy
#ifndef KlientBuilderH
#define KlientBuilderH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <string>
#include <thread>

using namespace std;

class TForm1 : public TForm    // Ma gui form dedi z puvodni form c++ builderu, tvori IDE
{
__published:	// IDE komponenty
	TEdit *NICK_TXT;
	TLabel *NICK_L;
	TButton *CONNECT;
	TLabel *IP_L;
	TEdit *IP_TXT;
	TPanel *TPanel;
	TEdit *Edit1;
	TButton *Button1;
	TListBox *ListBox;
	void __fastcall CONNECTClick(TObject *Sender); // Tlacitko pripojeni
	void __fastcall Button1Click(TObject *Sender); // Tlacitko odeslani
	void __fastcall Edit1KeyPress(TObject *Sender, WORD &Key, TShiftState Shift); // Tlacitko enter

private:	// Deklarace uzivatele
void prijmi(int sock);      // Deklarace funkce prijmi
void posli(int sock, string str);   // Deklarace funkce posli
public:		// Deklarace uzivatele
     thread * recvThread = NULL;

	__fastcall TForm1(TComponent* Owner);  // Vytvorene automaticky IDE
};

extern PACKAGE TForm1 *Form1;

#endif
