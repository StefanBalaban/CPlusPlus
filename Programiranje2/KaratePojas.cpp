#include "pch.h"
#include <iostream>

using namespace std;

// Tri vrste aktivnosti
enum VrstaAktivnosti { Tehnike, Kata, Borba };
// Šest vrta pojasa
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };

// Struct-a datum je uredna, zašiteæna je od ispisa nullpointer-a i ima boo metodu NisamNull radi provjere stanja pointera
struct Datum {
	int* _dan, * _mjesec, * _godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	bool NisamNull() {
		if (_dan != nullptr && _mjesec != nullptr && _godina != nullptr)
			return true;
		return false;
	}
	void Ispis() { if(NisamNull()) cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; } 	
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	int GetDays() {
		if (NisamNull()) {
			return *_dan + (*_mjesec) * 30 + *(_godina) * 365;
		}
	}
	
};
// Struct-a Aktivnost je uredna, zaštiæena je od ispisa nullpointera i ima bool metodu NisamNull radi provjere stanja pointera
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char* _nazivOpis;
	int _ocjena; // 1 - 10 

	bool NisamNull()
	{
		if (_vrsta && _nazivOpis != nullptr && _datumIzvrsenja.NisamNull())
			return true;
	}
	void Unos(VrstaAktivnosti vrsta, Datum* datum, int ocjena, const char* nazivOpis) {
		if (datum == nullptr || !datum->NisamNull() || nazivOpis == nullptr || ocjena > 10 || ocjena < 1)
			return;

		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;

		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);

		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
	}


	void Ispis() {
		if (_vrsta)
			cout << "\n" << *_vrsta;
		
		if (_nazivOpis != nullptr)
			cout << " " << _nazivOpis;
		
		cout << _ocjena << " ";
		if (_datumIzvrsenja.NisamNull())
			_datumIzvrsenja.Ispis();

		cout << endl;
		
	}
	
	void Dealociraj() {

		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
		_vrsta.reset();
		_ocjena = 0;
	}
};

struct KaratePojas {
	Datum* _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost* _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;

	// Prvi korak
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Dealociraj();
		delete _datumPolaganja;  _datumPolaganja = nullptr;

		if (_listaIzvrsenihAktivnosti != nullptr)
			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti; _listaIzvrsenihAktivnosti = nullptr;

		_trenutnoIzvrsenihAktivnosti = 0;
	}	
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << _pojas << endl;

		if (_listaIzvrsenihAktivnosti != nullptr)
			for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Ispis();
	}
	bool DodajIzvrsenuAktivnost(Aktivnost* aktivnost) {
		if (_datumPolaganja != nullptr)
			return false;
		if ( aktivnost == nullptr || !aktivnost->NisamNull())
			return false;
		if (!IspravnaAktivnost(aktivnost))
			return false;
		
		// Drugi korak
		Aktivnost* pom = new Aktivnost[_trenutnoIzvrsenihAktivnosti];
		
		// Treiæi korak
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			pom[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		
		// Èetvrti korak
		delete[] _listaIzvrsenihAktivnosti; _listaIzvrsenihAktivnosti = nullptr;
		
		// Peti korak
		_listaIzvrsenihAktivnosti = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		
		// Šetsti korak 
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			_listaIzvrsenihAktivnosti[i].Unos(*pom[i]._vrsta, &pom[i]._datumIzvrsenja, pom[i]._ocjena, pom[i]._nazivOpis);
			pom[i].Dealociraj();
		}
		
		// Sedmi korak
		delete[] pom; pom = nullptr;
		

		// Osmi korak
		_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti].Unos(*aktivnost->_vrsta, &aktivnost->_datumIzvrsenja, aktivnost->_ocjena, aktivnost->_nazivOpis);

		// Deveti korak
		_trenutnoIzvrsenihAktivnosti++;
		return true;
		

	}
	// Nova aktivnost se ne smije dodati ako postoji prethodna sa istim datumom, istom vrstom i ocjenom manjom od 6
		// GetDays za Datum
	// Nova aktivnost iste vrste se može dodati samo ako je prošlo 15 dana od prošle uspješne		
	// Ne smije se dodavat aktivnost na položenom pojasu
	bool IspravnaAktivnost(Aktivnost* aktivnost) {
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays() == aktivnost->_datumIzvrsenja.GetDays() &&
 				*_listaIzvrsenihAktivnosti[i]._vrsta == *aktivnost->_vrsta &&
				_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				return false;	

			if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.GetDays() + 15 > aktivnost->_datumIzvrsenja.GetDays() &&
				*_listaIzvrsenihAktivnosti[i]._vrsta == *aktivnost->_vrsta &&
				_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				return false;

		}
		return true;
	}
	

	// Pojas da bude položen zahjteva broj svake aktivnosti jednak (enum vrijednost + 1)
		// Datum polaganja se postavlja na datum posljednje uspješno realizovano aktivnosti
	// Jedan pojas da bude položen ne smije imati tri aktivnosti iste vrste ocjene manje od 5
	bool DaLiJePolozen() {
		int brojPolozenihAktivnosti[3] = { 0 };
		int brojNeuspjelihAktivnosti[3] = { 0 };
		Datum d;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._ocjena >= 6)
			{
				brojPolozenihAktivnosti[*_listaIzvrsenihAktivnosti[i]._vrsta] ++;
				d.Unos(*_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[i]._datumIzvrsenja._godina);
			}
			else
				brojNeuspjelihAktivnosti[*_listaIzvrsenihAktivnosti[i]._vrsta] ++;
			
		}

		for (int i = 0; i < 3; i++)
		{
			if (brojPolozenihAktivnosti[i] < _pojas + 1 || brojNeuspjelihAktivnosti[i] > 2)
				return false;
		}

		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*d._dan, *d._mjesec, *d._godina);
		return true;
	}

	// Prosjeèna ocjena aktivnosti ocjene iznad 5 koje sadrže proslijeðeni char u svom _nazivOpis-u	
	float PretragaRekurzivno(const char* str, int i = 0, int brojacIzvrsenih = 0, float prosjek = 0) {
		if (i == _trenutnoIzvrsenihAktivnosti)
		{
			if (brojacIzvrsenih == 0) return 0;
			return prosjek / brojacIzvrsenih;
		}

		if (_listaIzvrsenihAktivnosti[i]._ocjena > 5 && strstr(_listaIzvrsenihAktivnosti[i]._nazivOpis, str))
			return PretragaRekurzivno(str, i + 1, brojacIzvrsenih + 1, prosjek += _listaIzvrsenihAktivnosti[i]._ocjena);
		return PretragaRekurzivno(str, i + 1, brojacIzvrsenih, prosjek);
	}

	void Kopiraj(KaratePojas pojas) {
		Unos(pojas._pojas);

		for (int i = 0; i < pojas._trenutnoIzvrsenihAktivnosti; i++)
			pojas.DodajIzvrsenuAktivnost(&pojas._listaIzvrsenihAktivnosti[i]);
		if (pojas._datumPolaganja != nullptr &&  pojas._datumPolaganja->NisamNull())
		{
			_datumPolaganja = new Datum;
			_datumPolaganja->Unos(*pojas._datumPolaganja->_dan, *pojas._datumPolaganja->_mjesec, *pojas._datumPolaganja->_godina);
		}
		
	}


};
struct Kandidat {
	char* _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];

	void Unos(const char* imePrezime) {		
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;

	}	
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
	}	
	void Ispis() {
		if (_imePrezime != nullptr)
			cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}

	bool DodajPojas(KaratePojas pojas) {		
		auto GetLokacija = [this]() {
			for (int i = 0; i < 6; i++)
				if (_pojasevi[i] == nullptr)
					return i;
		};
		int lokacija = GetLokacija();
		if (lokacija != (int)pojas._pojas)
			return false;
		
		_pojasevi[lokacija] = make_shared<KaratePojas>();
		
		_pojasevi[lokacija]->Kopiraj(pojas);
		
	}
};
int main() {

	// BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
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
	pojasNarandzasti.Unos(Narandzasti);
	pojasZeleni.Unos(Zeleni);

	
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


	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	//pojasZuti.Sortiraj();

	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...

	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno
	ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao
	sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	
	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili 
	opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekurzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno dodani koristiti lambda funkciju.
	*/
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;



	system("pause");
	return 0;
}
