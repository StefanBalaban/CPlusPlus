// Globalno	
// PronadiNajskupljiArtikal(Racun racun) - vraæa pointer na najskuplju stavku raèuna Racun.Max(r =>r.Artikal(kolicina*cijena))

// Dodao
	// Artikal
	// 1 Unos(const char* brojRacuna, Datum datumKreiranja) - kreira raèun i dodijeli mu broj
		// Koristi funkciju GenerisiSljedeciBrojRacuna()
	// 2 Dealociraj() - dealocira _artikli i _kolicine
	// 3 DodajArtikal(Artikal artikal, int kolicina) - Ako se unese artikal koji je veæ prisutan onda sabrat mu kolièinu sa artiklom koji je veæ na raèunu
		// Unos samo jednog artikla
	// 4 Ispis() - ispisuje raèun u nekom obliku i spasi ga u tekstualnom, broj raèuna je ime datoteke
	// 5 BinarnaPretraga(const char* nazivArtikala) koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca 
		//pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.

	// GenerisiSljedeciBrojRacuna() koja æe pratiti int varijablu u kojoj æe biti posljednji dodijeljeni broj raèuna
		// broj raèuna æe biti u formatu sa nulama sa lijeve strane 0000000001	



#include "pch.h"
#include <iostream>
#include <fstream>
#include <cctype>
/*
using namespace std;
const char* crt = "\n----------------------------------------------------\n";
// Ova struct-a ima int pointere za datumske jedinice sa funkcijama unosa, ispisa i dealociranja
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
// Ova structa ima char pointer za naziv artikla i float vrijednost za cijenu
// Inicijalizira se sa unos metodom kojoj proslijeðujemo string i cijenu
// Takoðer ima metode dealociraj i metodu ispis artikla
struct Artikal {
	char* _naziv;
	float _cijena;
	void Unos(const char* naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
// Ova struct-a ima char array za broj raèuna od 10 karaktera
// Datum polje
// Pointer za Artikal struct-u
// Int pointer za kolicinu
// Int vrijednost za broj artikala
struct Racun {

	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal* _artikli = nullptr;
	// Kolièina za svaki artikal na raèunu
	int* _kolicine = nullptr;
	int _brojArtikala = 0;
	// Ova metoda inicijalizira _brojRacuna i _datumKreiranja
	void Unos(char* brojRacuna, Datum datumKreiranja) { strcpy_s(_brojRacuna, brojRacuna); _datumKreiranja = datumKreiranja; }
	// Ova metoda dealocira pointere _artikli i _kolicine	
	void Dealociraj() { _artikli = nullptr; _kolicine = nullptr; delete[] _artikli; delete[] _kolicine; }
	// Unos artikla na raèun
	void DodajArtikal(Artikal artikal, int kolicina)
	{
		// Bazni sluèaj
		if (_brojArtikala == 0)
		{
			_artikli = new Artikal[++_brojArtikala];
			_kolicine = new int[_brojArtikala];
			_artikli[0] = artikal;
			_kolicine[0] = kolicina;
			return;
		}
		// Provjera da li je artikal veæ prisutan na raèunu i ako jeste saberu se kolièine
		for (int i = 0; i < _brojArtikala; i++)
		{
			if (_artikli[i]._naziv == artikal._naziv && _artikli[i]._cijena == artikal._cijena)
			{
				_kolicine[i] += kolicina;
				return;
			}
		}
		// Da nemamo problema sa warningom koristimo ovo
		// Kada imamo posla sa novim arrayom koristimo ovaj integer
		// Kada imamo posla sa starim arrayom koristimo _brojArtikala
		int brojArtikalaNovi = _brojArtikala + 1;
		// Kreiranje Buffer arrayova i instanciranje arrayova sa starim vrijednostima plus sa novom
		// Buffer je velièine kao stari array
		Artikal* artikliBuffer = new Artikal[_brojArtikala];
		for (int i = 0; i < _brojArtikala; i++)
			artikliBuffer[i] = _artikli[i];

		delete[] _artikli;
		_artikli = nullptr;
		_artikli = new Artikal[brojArtikalaNovi];
		for (int i = 0; i < (brojArtikalaNovi - 1); i++)
			_artikli[i] = artikliBuffer[i];
		_artikli[brojArtikalaNovi - 1] = artikal;
		delete[] artikliBuffer;
		artikliBuffer = nullptr;


		int* kolicineBuffer = new int[_brojArtikala];
		for (int i = 0; i < _brojArtikala; i++)
			kolicineBuffer[i] = _kolicine[i];

		delete[] _kolicine;
		_kolicine = nullptr;
		_kolicine = new int[brojArtikalaNovi];
		for (int i = 0; i < (brojArtikalaNovi - 1); i++)
			_kolicine[i] = kolicineBuffer[i];
		_kolicine[brojArtikalaNovi - 1] = kolicina;
		delete[] kolicineBuffer;
		kolicineBuffer = nullptr;

		_brojArtikala++;
	}
	void Ispis()
	{
		if (_brojArtikala == 0)
			return;
		int total = 0;
		char* naziv = new char[16];
		strcpy_s(naziv, strlen(_brojRacuna) + 1, _brojRacuna);
		strcat_s(naziv, 16, ".txt");
		ofstream racun(naziv);

		racun << "\nBroj: " << _brojRacuna;
		racun << "\nDatum: " << *_datumKreiranja._dan << "." << *_datumKreiranja._mjesec << "." << *_datumKreiranja._godina << ".";
		racun << "\n------------------------------";
		racun << "\nArtikal \t Cijena \t Kolicina";
		for (int i = 0; i < _brojArtikala; i++)
		{
			racun << "\n" << _artikli[i]._naziv << " \t " << _artikli[i]._cijena << " \t " << _kolicine[i];
			total += _artikli[i]._cijena * _kolicine[i];
		}

		racun << "\n------------------------------";
		racun << "\nUkupan iznos: " << total;
		racun.close();

		cout << "\nBroj: " << _brojRacuna;
		cout << "\nDatum: " << *_datumKreiranja._dan << "." << *_datumKreiranja._mjesec << "." << *_datumKreiranja._godina << ".";
		cout << "\n------------------------------";
		cout << "\nArtikal \t Cijena \t Kolicina";
		for (int i = 0; i < _brojArtikala; i++)
			cout << "\n" << _artikli[i]._naziv << " \t " << _artikli[i]._cijena << " \t " << _kolicine[i];
		cout << "\n------------------------------";
		cout << "\nUkupan iznos: " << total;
	}
	Artikal* BinarnaPretraga(const char* naziv) {
		bool prolaz = false;
		// Sortiranje
		do {
			prolaz = false;
			for (size_t i = 0; i < _brojArtikala - 1; i++)
			{
				// Ovo mjenja mjesta artikala iz arraya, odnosno sortira ih
				// Ako je artikal indeksa i veæi po broju karaktera od artikla indeksa + 1 onda se mjenjaju za mjesta
				if (strcmp(_artikli[i]._naziv, _artikli[i + 1]._naziv) > 0) {
					Artikal temp = _artikli[i];
					_artikli[i] = _artikli[i + 1];
					_artikli[i + 1] = temp;
					prolaz = true;
				}
			}
			// While petlja služi da for petlja se izvrti dovoljno puta da svi artikli budu sortirani kako treba
		} while (prolaz);

		// Binarna pretraga
		int prvi = 0;
		int zadnji = _brojArtikala;
		int sredina = 0;
		while (prvi <= zadnji) {
			sredina = (prvi + zadnji) / 2;
			if (strcmp(_artikli[sredina]._naziv, naziv) == 0)
				return &_artikli[sredina];
			else if (strcmp(_artikli[sredina]._naziv, naziv) < 0)
				prvi = sredina + 1;
			else
				zadnji = sredina - 1;
		}
		return nullptr;
	}
};

long int brojRacuna = 1;
char* GenerisiSljedeciBrojRacuna()
{
	// Zaštita od nereguralnog brojRacuna
	if (brojRacuna >= 99999999 || brojRacuna < 1)
		brojRacuna = 1;
	char broj[10];
	// Parametri: 1. broj koji konvertujemo u char, 2. array gdje ga konvertujemo, 3. dužina array-a, 4. baza broja
	_itoa_s(brojRacuna, broj, 10, 10);
	// Array gdje smještamo nule prefixe
	const char* nule = nullptr;

	if (brojRacuna < 10)
		nule = "00000000";
	if (brojRacuna >= 10)
		nule = "0000000";
	if (brojRacuna >= 100)
		nule = "000000";
	if (brojRacuna >= 1000)
		nule = "00000";
	if (brojRacuna >= 10000)
		nule = "0000";
	if (brojRacuna >= 100000)
		nule = "000";
	if (brojRacuna >= 1000000)
		nule = "00";
	if (brojRacuna >= 10000000)
		nule = "0";

	// Keira se char*
	char* racun = new char[11];
	// Kopiraju se nule na racun, dužina je dužina nula + 1
	// Ako je cat_s onda dobijemo error
	strcpy_s(racun, strlen(nule) + 1, nule);
	// Konkatira se broj na racun
	strcat_s(racun, 11, broj);
	brojRacuna++;
	return racun;
}
Artikal * PronadjiNajskupljiArtikal(Racun racun)
{
	int indeksNajskupljeg;
	float cijenaNajskupljeg = 0;
	for (int i = 0; i < racun._brojArtikala; i++)
	{
		if (racun._artikli[i]._cijena * racun._kolicine[i] > cijenaNajskupljeg)
		{
			indeksNajskupljeg = i;
			cijenaNajskupljeg = racun._artikli[i]._cijena * racun._kolicine[i];
		}
	}
	return &racun._artikli[indeksNajskupljeg];
}
*/