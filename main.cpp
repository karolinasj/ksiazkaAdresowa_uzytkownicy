#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

struct Uzytkownik
{
    int id;
    string login, haslo;
};
struct Kontakt
{
    string imie;
    string nazwisko;
    int idKontaktu, idUzytkownika;
    string adres, numerTelefonu, email;
};
void zmianaNazwy()
{
    char oldname[] = "kontakty_roboczy.txt";
    char newname[] = "kontakty.txt";

    if (rename(oldname, newname) != 0)
        perror("Error renaming file");
}
int wyszukiwanieKontaktuPoId(vector <Kontakt> &listaOsob, int sptawdzaneIdKontaktu)
{

    int szukanyIndeks=-1;

    for( std::vector<Kontakt>::iterator i = listaOsob.begin(); i < listaOsob.end(); i++)
    {

        if(i-> idKontaktu == sptawdzaneIdKontaktu )
        {
            szukanyIndeks=distance(listaOsob.begin(), i);
        }
    }
    return szukanyIndeks;

}
int policzLiczbeKontaktow()
{
    string linia;
    int licznik = 0;
    int liczbaKontaktow;
    fstream plik;
    plik.open("kontakty.txt", ios::in);

    if(!plik.good())
    {
        liczbaKontaktow = 0;
        return liczbaKontaktow;
    }

    while (!plik.eof())
    {
        getline(plik, linia);
        licznik++;
    }
    liczbaKontaktow = licznik;

    plik.close();
    return liczbaKontaktow;
}

bool sprawdzanieLini(string linia, vector <Kontakt> &listaOsob, bool czyPierwszaLinia, int idZalogowanegoUzytkownika, int indeksUsuwanegoKontaktu )
{
    string slowo = "";

    for (auto x : linia)
    {

        if (x == '|')
        {
            int idKontaktu = stoi(slowo);

            if (indeksUsuwanegoKontaktu >=0)
            {
                if (idKontaktu == listaOsob[indeksUsuwanegoKontaktu].idKontaktu)
                {
                    break;
                }
            }

            int indeksKontaktu = wyszukiwanieKontaktuPoId(listaOsob, idKontaktu);

            if (indeksKontaktu!= -1)
            {
                fstream plik;
                plik.open("kontakty_roboczy.txt", ios::out | ios::app);

                if(czyPierwszaLinia)
                {
                    plik << idKontaktu<<"|"<< idZalogowanegoUzytkownika<<"|"<<listaOsob[indeksKontaktu].imie<<"|"<<listaOsob[indeksKontaktu].nazwisko<<"|"<<listaOsob[indeksKontaktu].numerTelefonu<<"|"<<listaOsob[indeksKontaktu].email<<"|"<<listaOsob[indeksKontaktu].adres<<"|";
                    czyPierwszaLinia=false;
                }

                else
                {
                    plik << endl<< idKontaktu<<"|"<< idZalogowanegoUzytkownika<<"|"<<listaOsob[indeksKontaktu].imie<<"|"<<listaOsob[indeksKontaktu].nazwisko<<"|"<<listaOsob[indeksKontaktu].numerTelefonu<<"|"<<listaOsob[indeksKontaktu].email<<"|"<<listaOsob[indeksKontaktu].adres<<"|";;
                }
                plik.close();
                break;
            }
            else
            {
                fstream plik;
                plik.open("kontakty_roboczy.txt", ios::out | ios::app);
                if(czyPierwszaLinia)
                {
                    plik << linia ;
                    czyPierwszaLinia=false;
                }
                else
                {
                    plik <<endl<<linia;
                }

                plik.close();

                slowo = "";
                break;
            }
        }
        else
        {
            slowo = slowo + x;
        }
    }

    std::string str;

    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
    return czyPierwszaLinia;
}

void robieniePlikuRoboczego(vector <Kontakt> &listaOsob, int idZalogowanegoUzytkownika, int indeksUsuwanegoKontaktu)
{
    system( "cls" );
    string linia;
    int liczbaKontaktowWPliku = policzLiczbeKontaktow();
    bool czyPierwszaLinia=true;

    fstream plik;
    plik.open("kontakty.txt",ios::in);
    if (plik.good()==false)
    {
        return;
    }
    else
    {
        for(int i=0; i<liczbaKontaktowWPliku; i++)
        {
            while(getline(plik,linia))
            {
                czyPierwszaLinia = sprawdzanieLini(linia, listaOsob, czyPierwszaLinia, idZalogowanegoUzytkownika, indeksUsuwanegoKontaktu);
            }
        }
        plik.close();
    }
    remove( "kontakty.txt" );
    zmianaNazwy();
}

int policzLiczbeUzytkownikow()
{
    string linia;
    int licznik = 0;
    int liczbaUzytkownikow;
    fstream plik;
    plik.open("uzytkownicy.txt", ios::in);

    if(!plik.good())
    {
        liczbaUzytkownikow = 0;
        return liczbaUzytkownikow;
    }

    while (!plik.eof())
    {
        getline(plik, linia);
        licznik++;
    }
    liczbaUzytkownikow = licznik;

    plik.close();
    return liczbaUzytkownikow;
}

void dodanieDoWektoraZPliku(string linia, vector <Uzytkownik> &nazwaWekora )
{
    Uzytkownik roboczyDoDodania;
    string polaKsiazkiAdresowej[3];
    string slowo = "";
    int j=0;

    for (auto x : linia)
    {
        if (x == '|')
        {
            polaKsiazkiAdresowej[j] = slowo;
            j++;
            slowo = "";
        }
        else
        {
            slowo = slowo + x;
        }
    }
    int idUzytkownika = stoi(polaKsiazkiAdresowej[0]);
    roboczyDoDodania.id = idUzytkownika;
    roboczyDoDodania.login = polaKsiazkiAdresowej[1];
    roboczyDoDodania.haslo = polaKsiazkiAdresowej[2];

    nazwaWekora.push_back(roboczyDoDodania);

}

void wczytywanieUzytkownikowZPliku(vector <Uzytkownik> &nazwaWektora)
{
    system( "cls" );

    string linia;
    int liczbaUzytkownikowWPliku = policzLiczbeUzytkownikow();
    fstream plik;
    plik.open("uzytkownicy.txt",ios::in);
    if (plik.good()==false)
    {
        cout<<"Nie udalo sie otworzyc pliku!";
    }
    else
    {
        for(int i=0; i<liczbaUzytkownikowWPliku; i++)
        {
            while(getline(plik,linia))
            {
                dodanieDoWektoraZPliku(linia, nazwaWektora);
            }
        }
        plik.close();
    }
}

void rejestracja(vector <Uzytkownik> &uzytkownicy)
{
    Uzytkownik roboczy;
    string nazwa, haslo;
    cout << "Podaj nazwe uzytkownika: ";
    cin >> nazwa;

    int i=0;
    while (i < uzytkownicy.size())
    {
        if (uzytkownicy[i].login == nazwa)
        {
            cout << "Taki uzytkownik juz istnienie. Wybierz inna nazwe: ";
            cin >> nazwa;
            i=0;
        }
        else
        {
            i++;
        }
    }
    cout << "Podaj haslo: ";
    cin >> haslo;
    roboczy.login = nazwa;
    roboczy.haslo = haslo;
    roboczy.id = uzytkownicy.size() +1;
    uzytkownicy.push_back(roboczy);

    cout << "Uzytkownik dodany";
    Sleep(1000);
}

int logowanie(vector <Uzytkownik> &uzytkownicy)
{
    string login, haslo;
    cout << "Podaj login: ";
    cin >> login;

    int i=0;
    while (i < uzytkownicy.size())
    {
        if (uzytkownicy[i].login == login)
        {
            for (int j=0; j<3; j++)
            {
                cout << "Zostalo "<< 3-j << " prob. Podaj haslo: ";
                cin >> haslo;
                if (uzytkownicy[i].haslo == haslo)
                {
                    cout << endl<< "Logowanie poprawne";
                    Sleep(1000);
                    return (uzytkownicy[i].id);
                }
            }
            cout << "Podano 3 razy bledne haslo. Blokada logowania";
            Sleep(3000);
            return 0;
        }

        else
        {
            i++;
        }
    }
    cout << "Nie ma takiego uzytkownika ";
    Sleep(1500);
    return (0);
}

void zmianaHasla(vector <Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika)
{
    string haslo;

    cout << "Podaj nowe haslo: ";
    cin >> haslo;
    for (int i=0; i<uzytkownicy.size(); i++)
    {
        if (uzytkownicy[i].id == idZalogowanegoUzytkownika)
        {
            uzytkownicy[i].haslo = haslo;
            cout << "Haslo zostalo zmienione"<<endl;
            Sleep(1500);
            break;
        }
    }
}

void aktualizujPlikUzytkownicy(vector <Uzytkownik> &listaUzytkownikow)
{
    int wielkoscWektora = listaUzytkownikow.size();

    if(wielkoscWektora == 0)
    {
        return;
    }
    else
    {
        fstream plik;
        plik.open("uzytkownicy.txt", ios::out);

        plik <<listaUzytkownikow[0].id<<"|"<<listaUzytkownikow[0].login <<"|"<<listaUzytkownikow[0].haslo<<"|";

        for(int i=1; i<wielkoscWektora; i++)
        {
            plik <<endl<< listaUzytkownikow[i].id<<"|"<<listaUzytkownikow[i].login <<"|"<<listaUzytkownikow[i].haslo<<"|";
        }
        plik.close();
    }
}

int dodajNowy(vector <Kontakt> &listaKontaktow, int ostatnieIdWPliku, int idZalogowanegoUzytkownika)
{
    int noweOstatnieId=0;
    system( "cls" );
    cout << "Dodawanie nowego kontaktu..."<<endl;

    string imie, nazwisko, email, adres, numerTelefonu;

    Kontakt Roboczy;

    cout << "Podaj imie: ";
    cin.clear();
    cin.sync();
    getline( cin, imie );

    cout << "Podaj nazwisko: ";
    cin.clear();
    cin.sync();
    getline( cin, nazwisko );

    cout << "Podaj adres email: ";
    cin.clear();
    cin.sync();
    getline( cin, email );

    cout << "Podaj adres: ";
    cin.clear();
    cin.sync();
    getline( cin, adres );

    cout << "Podaj numer telefonu: ";
    cin.clear();
    cin.sync();
    getline( cin, numerTelefonu );

    Roboczy.imie = imie;
    Roboczy.nazwisko = nazwisko;
    Roboczy.email = email;
    Roboczy.adres = adres;
    Roboczy.numerTelefonu = numerTelefonu;

    noweOstatnieId = ostatnieIdWPliku +1;

    Roboczy.idKontaktu=noweOstatnieId;
    listaKontaktow.push_back(Roboczy);

    fstream plik;
    plik.open("kontakty.txt", ios::out | ios::app);
    if (noweOstatnieId ==1)
    {
        plik <<noweOstatnieId<<"|"<<  idZalogowanegoUzytkownika<<"|"<<imie<<"|"<<nazwisko<<"|"<<numerTelefonu<<"|"<<email<<"|"<<adres<<"|";
    }
    else
    {
        plik <<endl<<noweOstatnieId<<"|"<<  idZalogowanegoUzytkownika<<"|"<<imie<<"|"<<nazwisko<<"|"<<numerTelefonu<<"|"<<email<<"|"<<adres<<"|";
    }
    plik.close();

    return noweOstatnieId;
}

int pobranieLiniZPlikuIDodanieDoWektora(string linia, vector <Kontakt> &listaOsob, int idZalogowanegoUzytkownika )
{
    Kontakt roboczyDoDodania;
    string polaKsiazkiAdresowej[7];
    string slowo = "";
    int j=0;

    for (auto x : linia)
    {
        if (x == '|')
        {
            polaKsiazkiAdresowej[j] = slowo;
            j++;
            slowo = "";
        }
        else
        {
            slowo = slowo + x;
        }
    }
    int ostatnieIdWPliku = 0;
    int idKontaktu = stoi(polaKsiazkiAdresowej[0]);
    int idUzytkownika = stoi(polaKsiazkiAdresowej[1]);

    ostatnieIdWPliku = idKontaktu;

    if (idUzytkownika == idZalogowanegoUzytkownika)
    {
        roboczyDoDodania.idUzytkownika = idUzytkownika;
        roboczyDoDodania.idKontaktu = idKontaktu;
        roboczyDoDodania.imie = polaKsiazkiAdresowej[2];
        roboczyDoDodania.nazwisko = polaKsiazkiAdresowej[3];
        roboczyDoDodania.numerTelefonu = polaKsiazkiAdresowej[4];
        roboczyDoDodania.email = polaKsiazkiAdresowej[5];
        roboczyDoDodania.adres = polaKsiazkiAdresowej[6];

        listaOsob.push_back(roboczyDoDodania);
    }
    return ostatnieIdWPliku;
}

void wyszukiwanieKontaktuPoImieniu(vector <Kontakt> &listaOsob)
{
    string imie;
    system( "cls" );
    cout << "Wyszukiwanie kontaktu po imieniu..."<<endl;
    cout << "Podaj imie: ";
    cin >> imie;
    int liczbaZnalezionychKontaktow=0;
    for( std::vector<Kontakt>::iterator i = listaOsob.begin(); i < listaOsob.end(); i++)
    {
        if(i->imie == imie)
        {
            cout <<listaOsob[distance(listaOsob.begin(), i)].idKontaktu <<" " <<listaOsob[distance(listaOsob.begin(), i)].imie <<" "<<listaOsob[distance(listaOsob.begin(), i)].nazwisko<<endl;
            cout << "email: "<<listaOsob[distance(listaOsob.begin(), i)].email <<endl<<"adres: "<<listaOsob[distance(listaOsob.begin(), i)].adres <<endl<<"numer tel: "<<listaOsob[distance(listaOsob.begin(), i)].numerTelefonu<< endl ;

            liczbaZnalezionychKontaktow++;
        }
    }
    cout << "Znaleziono "<<liczbaZnalezionychKontaktow <<" pasujacych kontaktow"<<endl;
    system("pause");

}
void wyszukiwanieKontaktuPoNazwisku(vector <Kontakt> &listaOsob)
{
    string nazwisko;
    system( "cls" );
    cout << "Wyszukiwanie kontaktu po nazwisku..."<<endl;
    cout << "Podaj nazwisko: ";
    cin >> nazwisko;
    int liczbaZnalezionychKontaktow=0;

    for( std::vector<Kontakt>::iterator i = listaOsob.begin(); i < listaOsob.end(); i++)
    {
        if(i->nazwisko == nazwisko)
        {
            cout << listaOsob[distance(listaOsob.begin(), i)].idKontaktu <<" "<<listaOsob[distance(listaOsob.begin(), i)].imie <<" "<<listaOsob[distance(listaOsob.begin(), i)].nazwisko<<endl;
            cout << "email: "<< listaOsob[distance(listaOsob.begin(), i)].email <<endl<<"adres: "<<listaOsob[distance(listaOsob.begin(), i)].adres <<endl<<"numer tel: "<<listaOsob[distance(listaOsob.begin(), i)].numerTelefonu<< endl ;
            liczbaZnalezionychKontaktow++;
        }
    }
    cout << "Znaleziono "<<liczbaZnalezionychKontaktow <<" pasujacych kontaktow"<<endl;
    system("pause");

}
void modyfikowanieKontaktu(vector <Kontakt> &listaOsob, int idZalogowanegoUzytkownika)
{
    char wybor;
    int indeksEdytowanegoKontaktu;
    int idSzukanegoKontaktu;
    cout<< "Podaj ID kontaktu, ktory chcesz edytowac: ";
    cin >> idSzukanegoKontaktu;

    if(cin.fail())
    {
        cout << "Bledny znak, ID moze byc tylko liczba";
        Sleep(1000);
        cin.clear();
        return;
    }

    indeksEdytowanegoKontaktu = wyszukiwanieKontaktuPoId(listaOsob,idSzukanegoKontaktu);

    if (indeksEdytowanegoKontaktu==-1)
    {
        cout << "Nie ma takiego kontaktu"<<endl;
        Sleep(600);
    }
    else
    {
        while(true)
        {
            system("cls");
            cout << "Co chesz edytowac? "<<endl;
            cout << "1. Imie"<<endl;
            cout << "2. Nazwisko"<<endl;
            cout << "3. Numer telefonu"<<endl;
            cout << "4. email"<<endl;
            cout << "5. Adres"<<endl;
            cout << "9. powrot do menu"<<endl;
            cin >> wybor;

            switch(wybor)
            {
            case '1':
            {
                string noweImie="";
                cout << "Podaj nowe dane: ";
                cin >> noweImie;
                listaOsob[indeksEdytowanegoKontaktu].imie = noweImie;
                cout << "Kontakt zostal zmodyfikowany";
                Sleep(1000);
                robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika, -2);
                break;
            }
            case '2':
            {
                string noweNazwisko="";
                cout << "Podaj nowe dane: ";
                cin >> noweNazwisko;
                listaOsob[indeksEdytowanegoKontaktu].nazwisko = noweNazwisko;
                cout << "Kontakt zostal zmodyfikowany";
                Sleep(1000);
                robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika, -2);
                break;
            }
            case '3':
            {
                string nowyNumerTelefonu="";
                cout << "Podaj nowe dane: ";
                cin >> nowyNumerTelefonu;
                listaOsob[indeksEdytowanegoKontaktu].numerTelefonu = nowyNumerTelefonu;
                cout << "Kontakt zostal zmodyfikowany";
                Sleep(1000);
                robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika,-2);

                break;
            }
            case '4':
            {
                string nowyEmail="";
                cout << "Podaj nowe dane: ";
                cin >> nowyEmail;
                listaOsob[indeksEdytowanegoKontaktu].email = nowyEmail;
                cout << "Kontakt zostal zmodyfikowany";
                Sleep(1000);
                robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika, -2);

                break;
            }
            case '5':
            {
                string nowyAdres="";
                cout << "Podaj nowe dane: ";

                cin.clear();
                cin.sync();
                getline( cin, nowyAdres );
                listaOsob[indeksEdytowanegoKontaktu].adres = nowyAdres;
                cout << "Kontakt zostal zmodyfikowany";
                Sleep(1000);
                robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika, -2);

                break;
            }
            case '9':
            {
                return;
            }
            }
        }
    }
}
int wczytywanieWszystkichKontaktowZPliku(vector <Kontakt> &listaOsob, int idZalogowanegoUzytkownika)
{
    system( "cls" );
    int nrLinii=1;
    string linia;
    int idOstatniegoKontaktu;
    int liczbaKontaktow = policzLiczbeKontaktow();
    fstream plik;
    plik.open("kontakty.txt",ios::in);

    for(int i=0; i<liczbaKontaktow; i++)
    {
        while(getline(plik,linia))
        {
            idOstatniegoKontaktu = pobranieLiniZPlikuIDodanieDoWektora(linia, listaOsob, idZalogowanegoUzytkownika);
            nrLinii++;
        }
    }
    plik.close();

    return idOstatniegoKontaktu;
}
void wyswietlanieWszystkichKontaktow(vector <Kontakt> &listaOsob)
{
    system( "cls" );
    cout << "Liczba kontaktow: "<< listaOsob.size()<<endl;
    for(int i=0; i<listaOsob.size(); i++)
    {
        cout << listaOsob[i].idKontaktu;
        cout << " "<<listaOsob[i].imie<<" "<<listaOsob[i].nazwisko<<endl;
        cout << "Adres: "<<listaOsob[i].adres<<endl;
        cout << "E-mail: "<<listaOsob[i].email<<endl;
        cout << "Numer telefonu: " << listaOsob[i].numerTelefonu<<endl<<endl;
    }
    system("pause");
}

void usuwanieKontaktu(vector <Kontakt> &listaOsob, int idZalogowanegoUzytkownika)
{
    int idUsuwanegoKontaktu;
    cout << "Podaj ID kontaktu, ktory chcesz usunac: ";
    cin >> idUsuwanegoKontaktu;
    int indeksUsuwanegoKontaktu = wyszukiwanieKontaktuPoId(listaOsob, idUsuwanegoKontaktu);
    int wielkoscWektora = listaOsob.size();
    char znakBezpieczenstwa;

    if(indeksUsuwanegoKontaktu==-1)
    {
        cout << "Nie znaleziono kontaktu o podanym ID";
        Sleep(1000);
        return;
    }
    cout << "Czy na pewno chcesz usunac kontakt? t/n ";
    cin >> znakBezpieczenstwa;
    if(znakBezpieczenstwa=='t')
    {

        for(int i=indeksUsuwanegoKontaktu; i<wielkoscWektora-1; i++)
        {
            listaOsob[i].imie=listaOsob[i+1].imie;
            listaOsob[i].nazwisko=listaOsob[i+1].nazwisko;
            listaOsob[i].email=listaOsob[i+1].email;
            listaOsob[i].adres=listaOsob[i+1].adres;
            listaOsob[i].numerTelefonu=listaOsob[i+1].numerTelefonu;
        }
        listaOsob.pop_back();
        wielkoscWektora = listaOsob.size();
        robieniePlikuRoboczego(listaOsob, idZalogowanegoUzytkownika, indeksUsuwanegoKontaktu);
        cout<< "Kontakt zostal usuniety"<<endl;
        Sleep(1000);
        return;
    }
    else if (znakBezpieczenstwa =='n')
    {
        cout << "Kontakt nie zostal usuniety";
        Sleep(1000);
        return;
    }
    else
    {
        cout << "Nierozpoznany znak, prosze sie zdecydowac";
        Sleep(1000);
        return;
    }

}

int main()
{
    int idZalogowanegoUzytkownika = 0;

    char wybor;
    vector <Uzytkownik> uzytkownicy;

    wczytywanieUzytkownikowZPliku(uzytkownicy);

    while(1)
    {
        if (idZalogowanegoUzytkownika == 0)
        {
            system("cls");
            cout << "1. Rejestracja"<<endl;
            cout << "2. Logowanie"<<endl;
            cout << "9. Zakoncz program"<<endl;

            cin >>wybor;

            if (wybor == '1')
            {
                rejestracja(uzytkownicy);
                aktualizujPlikUzytkownicy(uzytkownicy);
            }
            if (wybor == '2')
            {
                idZalogowanegoUzytkownika = logowanie(uzytkownicy);
            }
            if (wybor == '9')
            {
                exit(0);
            }
        }

        else
        {
            vector <Kontakt> listaKontaktowZalogowanegoUzytkownika;
            int idOstatniegoKontaktu = 0;

            idOstatniegoKontaktu = wczytywanieWszystkichKontaktowZPliku(listaKontaktowZalogowanegoUzytkownika,idZalogowanegoUzytkownika);

            system("cls");
            cout << "Wybierz opcje: "<<endl;
            cout << "1. Dodaj nowy kontakt"<<endl;
            cout << "2. Wyszukaj kontaktu po imieniu"<<endl;
            cout << "3. Wyszukaj kontaktu po naziwsku"<<endl;
            cout << "4. Wyswietl wszytkie kontakty"<<endl;
            cout << "5. Modyfikacja"<<endl;
            cout << "6. Usuwanie kontaktu"<<endl;
            cout << "7. Zmiana hasla"<<endl;
            cout << "8. Wylogowanie"<<endl;
            cout << "9. Zakoncz dzialanie programu"<<endl;
            cin >> wybor;

            switch(wybor)
            {
            case '1':
            {
                idOstatniegoKontaktu = dodajNowy(listaKontaktowZalogowanegoUzytkownika,idOstatniegoKontaktu,idZalogowanegoUzytkownika);
                cout << "Kontakt zostal dodany";
                Sleep(500);
                break;
            }
            case '2':
            {
                wyszukiwanieKontaktuPoImieniu(listaKontaktowZalogowanegoUzytkownika);
                break;
            }
            case '3':
            {
                wyszukiwanieKontaktuPoNazwisku(listaKontaktowZalogowanegoUzytkownika);
                break;
            }
            case '4':
            {
                wyswietlanieWszystkichKontaktow(listaKontaktowZalogowanegoUzytkownika);
                break;
            }
            case '5':
            {
                modyfikowanieKontaktu(listaKontaktowZalogowanegoUzytkownika, idZalogowanegoUzytkownika);
                break;
            }
            case '6':
            {
                usuwanieKontaktu(listaKontaktowZalogowanegoUzytkownika, idZalogowanegoUzytkownika);
                break;
            }
            case '7':
            {
                zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
                aktualizujPlikUzytkownicy(uzytkownicy);
                break;
            }
            case '8':
            {
                robieniePlikuRoboczego(listaKontaktowZalogowanegoUzytkownika, idZalogowanegoUzytkownika, -2);
                idZalogowanegoUzytkownika = 0;
                break;
            }
            case '9':
            {
                robieniePlikuRoboczego(listaKontaktowZalogowanegoUzytkownika, idZalogowanegoUzytkownika, -2);
                exit(0);
            }
            case 'x':
            {
                int id;
                cin >>id;
                cout << wyszukiwanieKontaktuPoId(listaKontaktowZalogowanegoUzytkownika,id);
                system("pause");
            }
            }
        }
    }
}
