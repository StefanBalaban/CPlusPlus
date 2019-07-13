#include "pch.h"
#include <iostream>

using namespace std;

// Struct-a datum

// Struct osoba

// Dinamièki niz

// Dinamièki niz pokazivaèa

// Statièki niz

struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int dan, int mjesec, int godina) {
		_dan = &dan; _mjesec = &mjesec; _godina = &godina;
	}
	void Ispis() {
		if (_dan == nullptr || _mjesec == nullptr || _godina == nullptr)
			return;
		cout << "\n" << *_dan << "." << *_mjesec << "." << *_godina << ".";
	}
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = nullptr; _mjesec = nullptr; _godina = nullptr;
	}
	bool NijeNull() {
		if (_dan != nullptr && _mjesec != nullptr && _godina != nullptr)
			return true;
		return false;
	}
};
struct Osoba {
	char* _imePrezime;
	Datum* _godiste;
	int _visina;

	void Unos(const char* imePrezime, Datum* godiste, int visina) { // Const char* mora biti tip
		if (imePrezime == nullptr || godiste == nullptr) // Ipak šti ove unose što primaju pointere
			return;
		int duzina = strlen(imePrezime) + 1;
		_imePrezime = new char[duzina];
		strcpy_s(_imePrezime, duzina, imePrezime);

		_godiste = new Datum;
		_godiste->Unos(*godiste->_dan, *godiste->_mjesec, *godiste->_godina); // Dereferenciraj pointere
		_visina = visina;
	}
	void Ispis() {
		cout << "\n" << "Ime i prezime: " << _imePrezime << " ";
		_godiste->Ispis();
		cout << " " << ", visine: " << _visina << "\n";
	}
	void Dealokacija() {
		delete _imePrezime; _imePrezime = nullptr; // Kada uradimo delete kažemo memorijskoj lokaciji da je lokacija slobodna da se prepiše, ali dok ne uradimo nullptr pointer i dalje pokazuje na tu memoriju
		_godiste->Dealociraj();
		_visina = 0;
	}
	bool NijeNull() {
		if (_imePrezime != nullptr && _godiste != nullptr && _godiste->NijeNull())
			return true;
		return false;
	}

};

struct StatickiNiz {
	Osoba _osoba[10];
	int _velicina;
	char* _naziv;

	// Prvi korak
	void Unos(const char* naziv) {
		int duzina = strlen(naziv) + 1;
		_naziv = new char[duzina];
		strcpy_s(_naziv, duzina, naziv);
		_velicina = 0;
	}
	bool Dodaj(Osoba* osoba) {
		if (osoba == nullptr || !osoba->NijeNull())
			return false;
		// Drugi korak
		if (_velicina > 9)
			return false;
		// Èetvrti korak
		_osoba[_velicina].Unos(osoba->_imePrezime, osoba->_godiste, osoba->_visina);
		// Peti korak
		_velicina++;

		return true;
	}
	void Ispis() {
		if (_naziv != nullptr)
			cout << "\n" << _naziv;

		for (int i = 0; i < _velicina; i++) {
			if (_osoba[i].NijeNull())
				_osoba->Ispis();
		}
	}
	void Dealokacija() {
		for (int i = 0; i < _velicina; i++) {
			_osoba[i].Dealokacija();
		}
		delete _naziv; _naziv = nullptr;
		_velicina = 0;
	}
	
};
struct DinamickiNiz {
	Osoba* _osoba;
	int _velicina;
	char* _naziv;

	// Prvi korak
	void Unos(const char* naziv) {
		_osoba = nullptr;
		_velicina = 0;

		int duzina = strlen(naziv) + 1;
		_naziv = new char[duzina];
		strcpy_s(_naziv, duzina, naziv);
	}
	bool DodajOsobnu(Osoba* osoba) {
		// Drugi korak
		Osoba* pom = new Osoba[_velicina];
		// Treæi korak
		for (int i = 0; i < _velicina; i++) {
			pom[i].Unos(_osoba[i]._imePrezime, _osoba[i]._godiste, _osoba[i]._visina);
			_osoba[i].Dealokacija();
		}
		// Èetvrti korak
		delete[] _osoba; _osoba = nullptr;

		// Peti korak
		_osoba = new Osoba[_velicina + 1];

		// Šesti korak

		for (int i = 0; i < _velicina; i++) {
			_osoba[i].Unos(pom[i]._imePrezime, pom[i]._godiste, pom[i]._visina);
			pom[i].Dealokacija();
		}

		// Sedmi korak
		delete[] pom; pom = nullptr;

		// Osmi korak
		_osoba[_velicina].Unos(osoba->_imePrezime, osoba->_godiste, osoba->_visina);

		// Deveti korak
		_velicina++;

		return true;

	}
	void Ispis() {
		for (int i = 0; i < _velicina; i++) {
			cout << "\n";
			_osoba[i].Ispis();
		}

		cout << "\n";		
		cout << "Velicina: " << _velicina;
		if (_naziv != nullptr)
			cout<< " , naziv: " << _naziv;
	}
};
struct DinamickiNizPokazivaca {
	Osoba** _osoba;
	int _velicina;
	char* _naziv;

	// Prvi korak
	void Unos(const char* naziv) {
		_osoba = nullptr;
		_velicina = 0;
		int duzina = strlen(naziv) + 1;
		_naziv = new char[duzina];
		strcpy_s(_naziv, duzina, naziv);
	}
	bool Dodaj(Osoba* osoba) {
		if (osoba == nullptr || osoba->_godiste == nullptr || osoba->_godiste->_dan == nullptr || osoba->_imePrezime == nullptr)
			return false;
		// Drugi korak
		Osoba** pom = new Osoba* [_velicina];
		// Treæi korak
		for (int i = 0; i < _velicina; i++)
		{
			pom[i] = new Osoba;
			pom[i]->Unos(_osoba[i]->_imePrezime, _osoba[i]->_godiste, _osoba[i]->_visina);
			_osoba[i]->Dealokacija();
			delete _osoba[i]; _osoba[i] = nullptr;
		}
		// Èetvrti korak
		delete[] _osoba; _osoba = nullptr;

		// Peti korak
		_osoba = new Osoba * [_velicina + 1];

		// Šesti korak
		for (int i = 0; i < _velicina; i++)
		{
			_osoba[i] = new Osoba;
			_osoba[i]->Unos(pom[i]->_imePrezime, pom[i]->_godiste, pom[i]->_visina);
			pom[i]->Dealokacija();
			delete pom[i]; pom[i] = nullptr;
		}
		// Sedmi korak
		delete[] pom; pom = nullptr;
		// Osmi korak
		_osoba[_velicina] = new Osoba;
		_osoba[_velicina]->Unos(osoba->_imePrezime, osoba->_godiste, osoba->_visina);

		// Deveti korak
		_velicina++;

		return true;
	}
	void Ispis() {
		if (_osoba == nullptr)
			return;
		if (_naziv != nullptr)
			cout << "\n" << _naziv;
		for (int i = 0; i < _velicina; i++) {
			if (_osoba[i] != nullptr && _osoba[i]->_godiste != nullptr && _osoba[i]->_godiste->_dan != nullptr && _osoba[i]->_imePrezime != nullptr)
				cout << "\n";
				_osoba[i]->Ispis();
		}

	}
};
struct StatickiNizPokazivaca {
	Osoba** _osoba;
	int _velicina;
	char* _naziv;

	// Pvi korak
	void Unos(const char* naziv) {
		_osoba = new Osoba * [10];
		_velicina = 0;
		int duzina = strlen(naziv) + 1;
		_naziv = new char[duzina];
		strcpy_s(_naziv, duzina, naziv);
	}
	bool Dodaj(Osoba* osoba) {
		if (_osoba == nullptr || osoba == nullptr || !osoba->NijeNull())
			return false;
		// Drugi korak
		if (_velicina > 9)
			return false;
		// Treæi korak
		_osoba[_velicina] = new Osoba;
		_osoba[_velicina]->Unos(osoba->_imePrezime, osoba->_godiste, osoba->_visina);

		// Èetvrti korak
		_velicina++;
		return true;
	}
	void Ispis()
	{
		if (_naziv != nullptr)
			cout << "\n" << _naziv;

		for (int i = 0; i < _velicina; i++) {
			if (_osoba[i] != nullptr && _osoba[i]->NijeNull())
				_osoba[i]->Ispis();
		}
	}
	void Dealociraj() {
		delete _naziv; _naziv = nullptr;
		for (int i = 0; i < _velicina; i++) {
			if (_osoba[i] != nullptr)
				_osoba[i]->Dealokacija();
			delete _osoba[i]; _osoba[i] = nullptr;
		}
		delete[] _osoba; _osoba = nullptr;
		_velicina = 0;
	}
};

//int main() {
//	Datum datum;
//	datum.Unos(1, 1, 2000);
//	datum.Ispis();
//	Osoba stefan;
//	stefan.Unos("Stefan Balaban", &datum, 180);
//	stefan.Ispis();

	



	
//	system("pause");
//	return 0;
//}