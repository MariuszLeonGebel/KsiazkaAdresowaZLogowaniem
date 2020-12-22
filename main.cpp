#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Adresat {
    int id, idUzytkownika;
    string imie, nazwisko, numerTelefonu, email, adres;
};

struct Uzytkownik {
    int id;
    string nazwa, haslo;
};

void zmianaNazwyPliku() {
    char staraNazwa[] ="Adresaci_temp.txt";
    char nowaNazwa[] ="KsiazkaAdresowa.txt";
    rename(staraNazwa, nowaNazwa);
}

void usunieciePliku() {
    char nazwaPliku[] = "KsiazkaAdresowa.txt";
    remove(nazwaPliku);
}

bool czySaOsoby(vector<Adresat> &adresaci) {
    bool CzyJestKtosWKsiazce=true;
    if(adresaci.size()==0) {
        cout<<"W twojej ksiazce adresowej nie ma zadnych zapisow!"<<endl;
        system("pause");
        CzyJestKtosWKsiazce = false;
    }
    return CzyJestKtosWKsiazce;
}

bool czyWlasciweID(vector<Adresat> &adresaci, int wybraneID) {
    bool czyJestWybraneDobreID=false;
    for(int i=0; i<adresaci.size(); i++) {
        if(adresaci[i].id == wybraneID) return czyJestWybraneDobreID = true;
    }
    return czyJestWybraneDobreID;
}

int wyborOsobyDoEdycji(vector<Adresat> &adresaci) {
    int idOsobyDoEdycji=0;

    cout<<"Podaj ID osoby, ktora chcesz edytowac: ";
    cin>>idOsobyDoEdycji;
    while(cin.good()==false) {
        cin.clear(); cin.sync();
        cout<<"Podaj ID osoby, ktora chcesz edytowac: ";
        cin>>idOsobyDoEdycji;
    }

    if(czyWlasciweID(adresaci, idOsobyDoEdycji)==false) {
        cout<<endl<<"Podano niewlasciwy numer ID!"<<endl;
        system("pause");
        return 0;
    }
    return idOsobyDoEdycji;
}

int wyborPozycjiDoEdycji(vector<Adresat> &adresaci) {
    int wybranaOpcja;
    char wybor;
    cout<<endl;
    cout <<"===========================EDYCJA=========================" << endl;
    cout << "1. Edytuj imie"<<endl;
    cout << "2. Edytuj nazwisko"<<endl;
    cout << "3. Edytuj numer telefonu"<<endl;
    cout << "4. Edytuj adres email"<<endl;
    cout << "5. Edytuj adres zamieszkania"<<endl;
    cout << "9. Powrot do menu glownego"<<endl;

BlednyWybor:
    cin>>wybor;

    switch(wybor) {
    case '1':
        wybranaOpcja = 1;
        break;
    case '2':
        wybranaOpcja = 2;
        break;
    case '3':
        wybranaOpcja = 3;
        break;
    case '4':
        wybranaOpcja = 4;
        break;
    case '5':
        wybranaOpcja = 5;
        break;
    case '9':
        system("cls");
        return 0;
    default:
        cout<<"Nie ma takiej opcji!"<<endl;
        system("pause");
        goto BlednyWybor;
    }
    return wybranaOpcja;
}
string intToStr(int liczba) {
    ostringstream ss;
    ss<<liczba;
    string str = ss.str();
    return str;
}

string wczytajLinie() {
    string wejscie = "";
    getline(cin, wejscie);
    return wejscie;
}

char wczytajZnak() {
    string wejscie = "";
    char znak  = {0};

    while (true) {
        cin.clear(); cin.sync();
        getline(cin, wejscie);

        if (wejscie.length() == 1) {
            znak = wejscie[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }
    return znak;
}

int znajdowanieID(vector<int> &id) {
    int szukaneID=0;
    for(int i=1; i<=id.size(); i++) {
        for(int y=0; y<id.size(); y++) {
            if(id[y] >= i) szukaneID = id[y];
        }
    }
    return szukaneID+1;
}

void kasowanieDanychWPlikuUzytkownikow() {
    ofstream plik;
    plik.open("Uzytkownicy.txt", ofstream::out | ofstream::trunc);
    plik.close();
}

void zapisDoPlikuUzytkownicy(vector<Uzytkownik> &uzytkownicy) {
    string daneDoZapisu, daneUzytkownika;
    Uzytkownik temp;
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out);

    for(int i=0; i<uzytkownicy.size(); i++) {
        daneDoZapisu = intToStr(uzytkownicy[i].id);
        daneDoZapisu += "|"+uzytkownicy[i].nazwa+"|";
        daneDoZapisu += uzytkownicy[i].haslo;
        plik << daneDoZapisu <<endl;
    }
    plik.close();
}

void zapisPoUsunieciu(vector<Adresat> &adresaci, int idOsobyUsunietej) {
    string linia;
    Adresat temp;
    int idO;
    char znak = '|';
    fstream plik, plik_temp;
    plik.open("KsiazkaAdresowa.txt", ios::in);
    plik_temp.open("Adresaci_temp.txt", ios::out);

    if(plik_temp.good()==true) {
        while(getline(plik, linia)) {
            string daneDoZapisu="";
            int pozycjaZnaleziona=0;
            pozycjaZnaleziona = linia.find(znak, pozycjaZnaleziona);
            idO = atoi(linia.substr(0, pozycjaZnaleziona).c_str());
            if(idO!=idOsobyUsunietej) {
                daneDoZapisu = linia;
                plik_temp << daneDoZapisu <<endl;
            }
        }
    }
    plik.close();
    plik_temp.close();
    usunieciePliku();
    zmianaNazwyPliku();
}

void zapisPoEdycji(vector<Adresat> &adresaci, int idOsobyDoEdycji, int i) {
    string linia;
    Adresat temp;
    int idO;
    char znak = '|';
    fstream plik, plik_temp;
    plik.open("KsiazkaAdresowa.txt", ios::in);
    plik_temp.open("Adresaci_temp.txt", ios::out);

    if(plik_temp.good()==true) {
        while(getline(plik, linia)) {
            string daneDoZapisu="";
            int pozycjaZnaleziona=0;
            pozycjaZnaleziona = linia.find(znak, pozycjaZnaleziona);
            idO = atoi(linia.substr(0, pozycjaZnaleziona).c_str());
            if(idO==idOsobyDoEdycji) {
                daneDoZapisu = intToStr(adresaci[i].id);
                daneDoZapisu += "|"+intToStr(adresaci[i].idUzytkownika)+"|";
                daneDoZapisu += adresaci[i].imie+"|";
                daneDoZapisu += adresaci[i].nazwisko+"|";
                daneDoZapisu += adresaci[i].numerTelefonu+"|";
                daneDoZapisu += adresaci[i].email+"|";
                daneDoZapisu += adresaci[i].adres;
            } else {
                daneDoZapisu = linia;
            }
            plik_temp << daneDoZapisu <<endl;
        }
    }
    plik.close();
    plik_temp.close();
    usunieciePliku();
    zmianaNazwyPliku();
}

void dodajOsobe(vector<Adresat> &adresaci, vector<int> &id, int wolneID, int idZalogowanegoUzytkownika) {
    string daneDoZapisu, daneUzytkownika;
    Adresat temp;

    daneDoZapisu = intToStr(wolneID);
    temp.id = wolneID;
    id.push_back(wolneID);

    daneDoZapisu += "|"+intToStr(idZalogowanegoUzytkownika)+"|";
    temp.idUzytkownika = idZalogowanegoUzytkownika;

    cout << "Podaj imie: ";
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika+"|";
    temp.imie = daneUzytkownika;

    cout << "Podaj nazwisko: ";
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika+"|";
    temp.nazwisko = daneUzytkownika;

    cout << "Podaj numer telefonu: ";
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika+"|";
    temp.numerTelefonu = daneUzytkownika;

    cout << "Podaj email: ";
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika+"|";
    temp.email = daneUzytkownika;

    cout << "Podaj adres: ";
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika;
    temp.adres = daneUzytkownika;

    adresaci.push_back(temp);

    fstream plik;
    plik.open("KsiazkaAdresowa.txt",ios::out | ios::app);

    if (plik.good() == true) {
        plik << daneDoZapisu <<endl;
        plik.close();
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system("pause");
    }
    cout << endl << "Osoba zostala dodana" << endl;
    system("pause");
}

int wczytajOsobyZPliku(vector<Adresat> &adresaci, vector<int> &id, int idZalogowanegoUzytkownika) {
    string linia;
    char znak = '|';
    Adresat temp;

    fstream plik;
    plik.open("KsiazkaAdresowa.txt",ios::in);
    if (plik.good() == true) {
        while (getline(plik,linia)) {
            int pozycjaZnaleziona=0, pozycjaPoprzednia=0;

            pozycjaZnaleziona = linia.find(znak, pozycjaZnaleziona);
            temp.id = atoi(linia.substr(0, pozycjaZnaleziona).c_str());
            id.push_back(temp.id);
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.idUzytkownika = atoi(linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia).c_str());
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.imie = linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia);
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.nazwisko = linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia);
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.numerTelefonu = linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia);
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.email = linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia);
            pozycjaPoprzednia = pozycjaZnaleziona+1;
            temp.adres = linia.substr(pozycjaPoprzednia, linia.length());

            if(temp.idUzytkownika == idZalogowanegoUzytkownika) adresaci.push_back(temp);
        }
        plik.close();
    }
}

void wyswietlWszystkieOsoby(vector<Adresat> &adresaci) {
    if(czySaOsoby(adresaci)==false) return;

    for(int i=0; i<adresaci.size(); ++i) {
        cout<<"ID: "<<adresaci[i].id<<endl;
        cout<<"ID uzytkownika: "<<adresaci[i].idUzytkownika<<endl;
        cout << adresaci[i].imie<< " " << adresaci[i].nazwisko << endl;
        cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
        cout << "Email: " << adresaci[i].email << endl;
        cout << "Adres: " << adresaci[i].adres << endl;
        cout << endl;
    }
    system("pause");
}

void wyszukajPoImieniu(vector<Adresat> &adresaci) {
    if(czySaOsoby(adresaci)==false) return;

    string imie;
    cout << "Podaj imie osoby jaka chcesz wyszukac: ";
    imie = wczytajLinie();
    for (int i = 0; i < adresaci.size(); i++) {
        if (adresaci[i].imie == imie) {
            cout << endl;
            cout << "ID: " << adresaci[i].id << endl;
            cout<<"ID uzytkownika: "<<adresaci[i].idUzytkownika<<endl;
            cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
            cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
            cout << "Email: " << adresaci[i].email << endl;
            cout << "Adres: " << adresaci[i].adres << endl;
            cout << endl;
        }
    }
    system("pause");
}

void wyszukajPoNazwisku(vector<Adresat> &adresaci) {
    if(czySaOsoby(adresaci)==false) return;

    string nazwisko;
    cout << "Podaj nazwisko osoby jaka chcesz wyszukac: ";
    nazwisko = wczytajLinie();
    for (int i = 0; i < adresaci.size(); i++) {
        if (adresaci[i].nazwisko == nazwisko) {
            cout << endl;
            cout << "ID: " << adresaci[i].id << endl;
            cout<<"ID uzytkownika: "<<adresaci[i].idUzytkownika<<endl;
            cout << adresaci[i].imie << " " << adresaci[i].nazwisko << endl;
            cout << "Telefon: " << adresaci[i].numerTelefonu << endl;
            cout << "Email: " << adresaci[i].email << endl;
            cout << "Adres: " << adresaci[i].adres << endl;
            cout << endl;
        }
    }
    system("pause");
}

void usunEdytujWszyscy(vector<Adresat> &adresaci) {
    if(czySaOsoby(adresaci)==false) return;

    for(int i=0; i<adresaci.size(); ++i) {
        cout<<"ID: "<<adresaci[i].id<<"\t|"<<adresaci[i].idUzytkownika<<"|";
        cout <<"\t"<< adresaci[i].nazwisko<< " " << adresaci[i].imie<<"        ";
        cout << "\t\ttel: " << adresaci[i].numerTelefonu ;
        cout << "; e-mail: " << adresaci[i].email;
        cout << "; adres: " << adresaci[i].adres << endl;
    }
}

void UsunOsobe(vector<Adresat> &adresaci) {
    int idOsobyDoUsuniecia=0;
    char znak;

    cout<<"Podaj ID osoby, ktora chcesz usunac: ";
    cin>>idOsobyDoUsuniecia;
    while(cin.good()==false) {
        cin.clear(); cin.sync();
        cout<<"Podaj ID osoby, ktora chcesz usunac: ";
        cin>>idOsobyDoUsuniecia;
    }

    if(czyWlasciweID(adresaci, idOsobyDoUsuniecia)==false) {
        cout<<endl<<"Podano niewlasciwy numer ID!"<<endl;
        system("pause");
        return;
    }

    cout<<"Czy na pewno chcesz usunac osobe o ID: "<<idOsobyDoUsuniecia<<"? <t> ";
    cin>>znak;
    if(znak=='t' || znak=='T') {
        for(int i=0; i<adresaci.size(); i++) {
            if(adresaci[i].id ==idOsobyDoUsuniecia) {
                adresaci.erase(adresaci.begin()+i);
                zapisPoUsunieciu(adresaci, idOsobyDoUsuniecia);
                cout<<endl<<"Osoba zostala usunieta!"<<endl;
                system("pause");
                return;
            }
        }
    } else {
        cout<<endl<<"Osoba NIE zostala usunieta!"<<endl;
        system("pause");
        return;
    }
}

void edycjaZapisu(vector<Adresat> &adresaci, int idOsobyDoEdycji, int co) {
    string staryZapis, nowyZapis, coZmieniono, odmianaZmienione_y;

    for(int i=0; i<adresaci.size(); i++) {
        if(adresaci[i].id ==idOsobyDoEdycji) {
            switch(co) {
            case 1:
                staryZapis = adresaci[i].imie;
                coZmieniono = "Dotychczasowe imie";
                odmianaZmienione_y = "zostalo zmienione";
                break;
            case 2:
                staryZapis = adresaci[i].nazwisko;
                coZmieniono = "Dotychczasowe nazwisko";
                odmianaZmienione_y = "zostalo zmienione";
                break;
            case 3:
                staryZapis = adresaci[i].numerTelefonu;
                coZmieniono = "Dotychczasowy numer telefonu";
                odmianaZmienione_y = "zostal zmieniony";
                break;
            case 4:
                staryZapis = adresaci[i].email;
                coZmieniono = "Dotychczasowy adres e-mail";
                odmianaZmienione_y = "zostal zmieniony";
                break;
            case 5:
                staryZapis = adresaci[i].adres;
                coZmieniono = "Dotychczasowy adres zamieszkania";
                odmianaZmienione_y = "zostal zmieniony";
                break;
            }
            cout<<coZmieniono<<": "<<endl;
            cout<<staryZapis<<endl;
            cout<<"Podaj nowy: "<<endl;
            cin.clear(); cin.sync();
            getline(cin, nowyZapis);
            if(co==1) adresaci[i].imie = nowyZapis;
            if(co==2) adresaci[i].nazwisko = nowyZapis;
            if(co==3) adresaci[i].numerTelefonu = nowyZapis;
            if(co==4) adresaci[i].email = nowyZapis;
            if(co==5) adresaci[i].adres = nowyZapis;

            cout<<coZmieniono<<": "<<staryZapis<<" "<<odmianaZmienione_y<<" na: "<<nowyZapis<<endl;

            zapisPoEdycji(adresaci, idOsobyDoEdycji, i);
            cout<<endl;
        }
    }
    system("pause");
}

void zmianaHasla(vector<Uzytkownik> &uzytkownicy, int idOsobyDoEdycji) {
    string staryZapis, nowyZapis;

    for(int i=0; i<uzytkownicy.size(); i++) {
        if(uzytkownicy[i].id ==idOsobyDoEdycji) {
            staryZapis = uzytkownicy[i].haslo;
            cout<<"Podaj nowe haslo: "<<endl;
            cin.clear(); cin.sync();
            getline(cin, nowyZapis);
            uzytkownicy[i].haslo = nowyZapis;
            cout<<"Haslo zostalo zmienione!"<<endl;

            kasowanieDanychWPlikuUzytkownikow();
            zapisDoPlikuUzytkownicy(uzytkownicy);
            cout<<endl;
        }
    }
    system("pause");
}

void wczytajOsobyZPlikuUzytkownicy(vector<Uzytkownik> &uzytkownicy) {
    string linia;
    char znak = '|';
    Uzytkownik temp;

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::in);
    if (plik.good() == true) {
        while (getline(plik,linia)) {
            int pozycjaZnaleziona=0, pozycjaPoprzednia=0;

            pozycjaZnaleziona = linia.find(znak, pozycjaZnaleziona);
            temp.id = atoi(linia.substr(0, pozycjaZnaleziona).c_str());
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaZnaleziona = linia.find(znak, pozycjaPoprzednia);
            temp.nazwa = linia.substr(pozycjaPoprzednia, pozycjaZnaleziona - pozycjaPoprzednia);
            pozycjaPoprzednia = pozycjaZnaleziona+1;

            pozycjaPoprzednia = pozycjaZnaleziona+1;
            temp.haslo = linia.substr(pozycjaPoprzednia, linia.length());

            uzytkownicy.push_back(temp);
        }
        plik.close();
    }
}

int logowanie(vector<Uzytkownik> &uzytkownicy) {
    string nazwa, haslo;
    cout<<"Podaj login: ";
    cin>>nazwa;

    int i=0;
    while(i<uzytkownicy.size()) {
        if(uzytkownicy[i].nazwa==nazwa) {
            for(int proby=0; proby<3; proby++) {
                if(proby==2)  cout<<"Podaj haslo! Pozostala ostatnia proba: ";
                else cout<<"Podaj haslo: ";
                cin>>haslo;
                if(uzytkownicy[i].haslo == haslo) {
                    cout<<"Witaj "<<nazwa<<" :)"<<endl<<endl;
                    system("pause");
                    return uzytkownicy[i].id;
                }
            }
            cout<<"Podales 3 razy bledne haslo! Program konczy dzialanie!"<<endl;
            exit(0);
        }
        i++;
    }
    cout<<"Nie ma uzytkownika o takim loginie!"<<endl<<endl;
    system("pause");
    return 0;
}

void rejestracjaLogowanie();

void poZalogowaniu(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika) {
    vector<Adresat>adresaci;
    vector<int>id;
    int wolneID;
    char wybor;

    wczytajOsobyZPliku(adresaci, id, idZalogowanegoUzytkownika);

    while (true) {
        system("cls");
        cout << "1. Dodaj osobe" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkie osoby" << endl;
        cout << "5. Usun" << endl;
        cout << "6. Edytuj" << endl;
        cout << "7. Zmien haslo" << endl;
        cout << "8. Wyloguj sie" << endl;
        cout << "9. Zakoncz program" << endl;
        wybor = wczytajZnak();

        if (wybor == '1') {
            wolneID = znajdowanieID(id);
            dodajOsobe(adresaci, id, wolneID, idZalogowanegoUzytkownika);
        } else if (wybor == '2') {
            wyszukajPoImieniu(adresaci);
        } else if (wybor == '3') {
            wyszukajPoNazwisku(adresaci);
        } else if (wybor == '4') {
            wyswietlWszystkieOsoby(adresaci);
        } else if (wybor == '5') {
            usunEdytujWszyscy(adresaci);
            if(adresaci.size()>0) UsunOsobe(adresaci);
        } else if (wybor == '6') {
            int wyborOsoby, wyborOpcji;
            usunEdytujWszyscy(adresaci);
            if(adresaci.size()>0) {
                wyborOsoby = wyborOsobyDoEdycji(adresaci);
                if (wyborOsoby>0) {
                    wyborOpcji = wyborPozycjiDoEdycji(adresaci);
                    if (wyborOpcji != 0) edycjaZapisu(adresaci, wyborOsoby, wyborOpcji);
                }
            }
        } else if (wybor == '7') {
            zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
        } else if (wybor == '8') {
            rejestracjaLogowanie();
        } else if (wybor == '9') {
            exit(0);
        }
    }
}

void dodajUzytkownika(vector<Uzytkownik> &uzytkownicy) {
    string daneDoZapisu, daneUzytkownika;
    int iloscUzytkownikow = uzytkownicy.size()+1;
    Uzytkownik temp;

    daneDoZapisu = intToStr(iloscUzytkownikow);
    temp.id = iloscUzytkownikow;
    cout << "Podaj nazwe uzytkownika: ";
    cin.clear(); cin.sync();
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += "|"+daneUzytkownika+"|";
    temp.nazwa = daneUzytkownika;

    cout << "Podaj podaj haslo: ";
    cin.clear(); cin.sync();
    daneUzytkownika = wczytajLinie();
    daneDoZapisu += daneUzytkownika;
    temp.haslo = daneUzytkownika;

    uzytkownicy.push_back(temp);

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::out | ios::app);

    if (plik.good() == true) {
        plik << daneDoZapisu <<endl;
        plik.close();
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system("pause");
    }
    cout << endl << "Uzytkownik: "<<temp.nazwa<<" zostal dodany!" << endl;
    system("pause");
}

void rejestracjaLogowanie() {
    vector<Uzytkownik>uzytkownicy;
    int idZalogowanegoUzytkownika=0;
    char wybor;

    wczytajOsobyZPlikuUzytkownicy(uzytkownicy);

    while(true) {
        system("cls");
        cout<<"1. Logowanie"<<endl;
        cout<<"2. Rejestracja"<<endl;
        cout<<"9. Zamknij program"<<endl;
        cin>>wybor;

        if(wybor=='1') {
            idZalogowanegoUzytkownika = logowanie(uzytkownicy);
            if (idZalogowanegoUzytkownika !=0) poZalogowaniu(uzytkownicy, idZalogowanegoUzytkownika);
        } else if(wybor=='2') {
            dodajUzytkownika(uzytkownicy);
        } else if(wybor=='9') {
            exit(0);
        }
    }
}

int main() {
    rejestracjaLogowanie();
    return 0;
}

