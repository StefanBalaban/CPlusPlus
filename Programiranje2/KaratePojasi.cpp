#include "pch.h";
#include <iostream>


using namespace std;
// Tri vrste aktivnosti
enum VrstaAktivnosti { Tehnike, Kata, Borba }; 
// Šest vrsti pojasa
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };

// Structa za datum je uredna, ima metodu Dealociraj koja delaocira sve.
struct Datum {
	int* _dan = nullptr, * _mjesec = nullptr, * _godina = nullptr;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { 
		if (_dan != nullptr)			
		cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; 
	}
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; _dan = nullptr; _mjesec = nullptr; _godina = nullptr; }
	int GetDatys() { return *_dan + *(_mjesec) * 30 + *(_godina) * 365; }
};
// Aktivnost sadrži pametni pointer na vrstu i char pointerž
// Dealocira datum objekat koji se sadrži u njemu
// Ured je
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta = nullptr;
	Datum _datumIzvrsenja;
	char* _nazivOpis = nullptr;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum* datum, int ocjena, const char* nazivOpis) {
		if (datum->_dan == nullptr)
			return;
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
	}
	void Ispis() {
		if (_vrsta == nullptr || _nazivOpis == nullptr)
			return;
		cout << *_vrsta << " " << _ocjena << " " << _nazivOpis;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};
// Sadrži pointer na Datum i niz Aktivnost
// Dealociranje ukine Datum i niz Aktivnost ( i datume sa aktivnosti)

struct KaratePojas{
	Datum* _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost* _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Pojas pojas) {
		_trenutnoIzvrsenihAktivnosti = 0;
		_listaIzvrsenihAktivnosti = nullptr;		
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		_datumPolaganja->Dealociraj(); _datumPolaganja = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr) // Ipak se štitimo od nullptr ali na ovaj naèin
			_datumPolaganja->Ispis();
		cout << _pojas << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
	bool DodajIzvrsenuAktivnost(Aktivnost* novaAktivnost) {
		if (_datumPolaganja != nullptr) return false;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) 
		{ 
			if (
				_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDatys() == novaAktivnost->_datumIzvrsenja.GetDatys() &&
				_listaIzvrsenihAktivnosti[i]._vrsta == novaAktivnost->_vrsta && 
				_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				return false;
			if (*novaAktivnost->_vrsta == *_listaIzvrsenihAktivnosti[i]._vrsta && (novaAktivnost->_datumIzvrsenja.GetDatys() - _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDatys()) < 15)
				return false;
		}

		// 1
		Aktivnost* pom = new Aktivnost[_trenutnoIzvrsenihAktivnosti];
		// 2
		
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			pom[i].Unos(*(_listaIzvrsenihAktivnosti[i]._vrsta), &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}	
		
		//3
		delete[] _listaIzvrsenihAktivnosti;		
		_listaIzvrsenihAktivnosti = nullptr;
	
		// 4
		_listaIzvrsenihAktivnosti = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		// 5
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			_listaIzvrsenihAktivnosti[i].Unos(*(pom[i]._vrsta), &pom[i]._datumIzvrsenja, pom[i]._ocjena, pom[i]._nazivOpis);
			pom[i].Dealociraj();
		}
		// 6
		delete[] pom;
		pom = nullptr;
		// 7 
		_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti].Unos(*novaAktivnost->_vrsta, &novaAktivnost->_datumIzvrsenja, novaAktivnost->_ocjena, novaAktivnost->_nazivOpis);
		// 8
		_trenutnoIzvrsenihAktivnosti++;
	
		return true;
	}
	float PretragaRekrzivno(int brojAktivnosti, int brojPolozenih, float zbirPolozenih) {
		if (brojAktivnosti < 0)
			return zbirPolozenih / brojPolozenih;
		if (_listaIzvrsenihAktivnosti[brojAktivnosti]._ocjena > 5)
			return PretragaRekrzivno(brojAktivnosti - 1, brojPolozenih + 1, zbirPolozenih += _listaIzvrsenihAktivnosti[brojAktivnosti]._ocjena);
		return PretragaRekrzivno(brojAktivnosti - 1, brojPolozenih, zbirPolozenih);
	}
};

struct Kandidat {
	char* _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	int _brojPojasa;
	void Unos(const char* imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
		_brojPojasa = 0;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
		_brojPojasa = 0;
	}
	void Ispis() {		
		cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
	/*bool DodajPojas(KaratePojas noviPojas) {
		switch (noviPojas._pojas)
		{
		case Zuti:
			if (_brojPojasa != 0)
				break;
			_pojasevi[_brojPojasa]->Unos(noviPojas._pojas);
			for (int i = 0; i < noviPojas._trenutnoIzvrsenihAktivnosti)
		default:
			break;
		}
	};*/
};

void mainO() {
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");
	
	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	// Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	//	pojasZuti.Sortiraj();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno(pojasZuti._trenutnoIzvrsenihAktivnosti, 0, 0) << endl;
	
	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno dodani koristiti lambda funkciju.
	*/
	//if (jasmin.DodajPojas(pojasZuti))
	//	cout << "Pojas uspjesno dodan!" << endl;
	//if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
	//	cout << "Pojas uspjesno dodan!" << endl;
	//if (jasmin.DodajPojas(pojasNarandzasti))
	//	cout << "Pojas uspjesno dodan!" << endl;

	
}