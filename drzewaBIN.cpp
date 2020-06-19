//Kod stworzony przez: Mateusz Kulesza
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#define N 5000


using namespace std;

class wezel {

public:
     wezel *lewy;              //wskaznik na lewy wezel (mniejszy)
     wezel *prawy;            //wskaznik na prawy wezel (wiekszy)
     wezel *rodzic;          //wskaznik na rodzica
     int wartosc;           //wartosc
     int level;            // poziom drzewa
     friend class drzewo; //klasa zaprzyjzniona drzewo

     void Wyzeruj() { lewy=prawy=NULL;}                    // konstruktor drzewa
     ~wezel(){
        lewy = prawy = rodzic = NULL;
        level = 0;
        wartosc = NULL;
     }
     szukajBST(wezel *v,int key);                         // wyszukuje wezel o danym kulucz zaczynajac od v ktory powinnen byc korzeniem, bo wartosc moze byc wszedzie, wiec warto zaczac od korzenia
     void wypisz();                                      // wypisuje informacje o drzewie binarnym
     void DFS(wezel * v);                               // przechodzi przez drzewo binarne i uzupelnia informacje o nim, ktore wypisze void wypisz()
     void DFSRelease(wezel * v);                       // przechodzi drzewo i kasuje wezly
     void deleteBT();                                 // usuwa drzewo binarne
     void wczytaj();                                 // wczytujemy cale drzewo binarne, instrukcja nizej
     void printBT(string sp, string sn, wezel * v); // wypisuje drzewo binarne

};
//------------------------- Funkcje dotyczące Klasy wezel------------------------------------
string cr, cl, cp;                // stringi potrzebne do wypisywania drzew
wezel *korzen;                   // wskaznik na korzen
int maxpath = 0;                // dlugosc maksymalnej sciezki
int minpath = 2147483647;      // dlugosc minimalnej sciezki
int * levelcount;             // tablica liczników
int leafcount = 0;           // liczba lisci
int onechildcount = 0;      // liczba wezlow z jednym dzieckiem
int nodesum = 0;           // suma wartosci wezlow
int n;                    // liczba wezlow drzewa potrzebna do funkcji wczytaj

void wezel::DFS ( wezel * v ){
  if( v )
  {
         // przetwarzamy biezacy wezel

        int children = 0; // liczba dzieci, 0, 1 lub 2

        if( v->lewy )
        {
          children++;
          v->lewy->level = v->level + 1;
        }

        if( v->prawy )
        {
          children++;
          v->prawy->level = v->level + 1;
        }

        if( v->level > maxpath ) maxpath = v->level;  // sprawdzanie najluzszej sciezki
   
        if( !children )
        {
            if( v->level < minpath ) minpath = v->level;  //  sprawdzanie najkrotszej sciezki i liczenie lisci
            leafcount++;
        }


        levelcount [ v->level ] ++;     // liczenie wezlow na obecnym  poziomie

        if( children == 1 ) onechildcount++;  // liczenie wezlow z jednym dzieckiem

        nodesum += v->wartosc;  // sumowanie wartosci wezlow

        DFS ( v->lewy );  // przechodzimy lewe poddrzewo
        DFS ( v->prawy ); // przechodzimy prawe poddrzewo
  }
}

void wezel::wypisz(){  //funkcja wypisuje dane zebrane przez void DFS

  cout << endl
       << "Maksymalna sciezka       = " << maxpath << endl
       << "Minimalna  sciezka       = " << minpath << endl
       << endl;
  for( int i = 0; i <= maxpath; i++ )
    cout << "poziom " << i << ": liczba wezlow = " << levelcount [ i ] << endl;

  cout << endl
       << "liczba lisci                     = " << leafcount << endl
       << "liczba wezlow z jednym dzieckiem = " << onechildcount << endl
       << "suma wartosci wezlow             = " << nodesum << endl
       << endl;
}

void wezel::DFSRelease ( wezel * v ){

  if( v )
  {
    DFSRelease ( v->lewy );   // usuwamy lewe poddrzewo
    DFSRelease ( v->prawy );  // usuwamy prawe poddrzewo
    delete v;              // usuwamy sam wezel
  }
}

void wezel:: deleteBT( ) // funkcja sprzata pamiec
{
  delete [ ] levelcount;
  DFSRelease (korzen );   // wykorzystujemy DFS do usuniêcia drzewa
}

void wezel::printBT ( string sp, string sn, wezel * v ) {
  string s;

  if( v )
  {
    s = sp;
    if( sn == cr ) s [ s.length( ) - 2 ] = ' ';
    printBT ( s + cp, cr, v->prawy );

    s = s.substr ( 0, sp.length( ) - 2 );
    cout << s << sn << v->wartosc << endl;

    s = sp;
    if( sn == cl ) s [ s.length( ) - 2 ] = ' ';
    printBT ( s + cp, cl, v->lewy );
  }
}
void wezel::wczytaj(){

  wezel ** vp;              // tablica wskaznikow
  int i, d, l, r;

  cin >> n;                  // odczytujemy liczbê wezlow drzewa binarnego

  vp = new wezel * [ n ];   // tworzymy tablice dynamiczna wskaznikow

  for( i = 0; i < n; i++ )
    vp [ i ] = new wezel;   // tworzymy dynamicznie wezly

  for( i = 0; i < n; i++ )
  {
    cin >> d >> l >> r;      // odczytujemy trzy liczby, d -wartosc wezla, l -numer lewego dziecka, r - numer prawego dziecka

    vp [ i ] ->level = 0;    // zerujemy poziom wezla

    vp [ i ] ->wartosc = d;     // wprowadzamy do wezla dane

    vp [ i ] ->lewy  = l ? vp [ l ] : NULL;  // ustawiamy lewego syna, jesli istnieje

    vp [ i ] ->prawy = r ? vp [ r ] : NULL;  // ustawiamy prawego syna, jesli istnieje
  }

  korzen = vp [ 0 ];           // zapamietujemy korzen

  delete [ ] vp;             // usuwamy tablice wskazinkow

  levelcount = new int [ n ];        // tworzymy tablice liczników elementów na poziomach
  for( i = 0; i < n; i++ ) levelcount [ i ] = 0;
}
wezel* szukajBST(wezel *start, int wartosc) // funkcja wyszukuje wezel o zadanej wartosci, zaczynajac od start(zazwyczaj korzen) i zwraca wskaznik na ten wezel
{    

    if (start->wartosc == wartosc) return start;

    else if (wartosc < start->wartosc && start->lewy != NULL) return szukajBST(start->lewy, wartosc);
    else if (wartosc > start->wartosc && start->prawy != NULL) return szukajBST(start->prawy, wartosc);
    return NULL;
}


wezel* naj_lewo(wezel *start) //wyszukuje najmniejszy element drzewa BST i zwraca wskanzik na niego
{  

    if(start->lewy != NULL)
        return naj_lewo(start->lewy);
    else
        return start;
}
wezel* naj_prawo(wezel *start) //wyszukuje najwiekszy element drzewa BST i zwraca wskanzik na niego
{ 
    if(start->prawy != NULL) return naj_prawo(start->prawy);
    else return start;
}


class drzewo{

public:

    int licznik;                                         //licznik elementow drzewa
    void Wyzeruj();                                     // czysci drzewo
    ~drzewo(){
        licznik = 0;
    }
    int  DodajBST(const int a);                        // dodaje element a do drzewa
    void printBST(string sp, string sn, wezel * v);   // drukuje drzewo
};
//------------------------------ Funkcje związane z klasa drzewo---------------------------

void kasowanie(wezel *start)  //kasuje dany wezel z drzewa i odpowienio zamienia wartosci jezeli to konieczne
{          
                                                //jezeli wezel nie ma dzieci
  if(start->lewy==NULL && start->prawy==NULL){
                                                //jezeli wezel jest korzeniem
       if(start->rodzic==NULL){
        korzen=NULL;
       }
                                                //jezeli wezel jest po lewej stronie rodzica,
       else if(start->rodzic->lewy==start){
                                                //usun wezel z lewej strony wezla rodzica
        start->rodzic->lewy=NULL;
       }
       else{
                                                //usun wezel z prawej strony wezla rodzica
        start->rodzic->prawy=NULL;
       }
       delete start;
  }
                                                    //jezeli wezel ma tylko jedno dziecko
  else if(start->lewy==NULL || start->prawy==NULL){
                                                        //jezeli po lewej stronie nie ma dziecka
       if(start->lewy==NULL){
                                                //jezeli wezel jest korzeniem
            if(start->rodzic==NULL){
             korzen=start->prawy;
            }
                                                    //jezeli wezel jest po lewej stronie rodzica
            else if(start->rodzic->lewy==start){
                                                    //przyczep z lewej strony rodzica wezel bedacy po prawej stronie usuwanego wezla
             start->rodzic->lewy=start->prawy;
            }
            else{
                                                    //przyczep z prawej strony rodzica wezel bedacy po prawej stronie usuwanego wezla
             start->rodzic->prawy=start->prawy;
            }
       }
       else{
                                                    //jezeli wezel jest korzeniem
            if(start->rodzic==NULL){
             korzen=start->lewy;
            }
                                                        //jezeli wezel jest po lewej stronie rodzica
            else if(start->rodzic->lewy==start){
                                                        //przyczep z lewej strony rodzica wezel bedacy po lewej stronie usuwanego wezla
             start->rodzic->lewy=start->lewy;
            }
            else{
                                                        //przyczep z prawej strony rodzica wezel bedacy po prawej stronie usuwanego wezla
             start->rodzic->prawy=start->lewy;
            }
       }
       delete start;
  }
  else
  {
                                                    //wstaw w miejsce usuwanego elementu - najmniejsza wartosc z prawego poddrzewa
       wezel *temp;
       temp=naj_lewo(start->prawy);
       start->wartosc = temp->wartosc;
       kasowanie(temp);
  }
 }



void drzewo::Wyzeruj() //konstuktor drzewa
{ 

    korzen = NULL;
    licznik = 0;
}

void drzewo::printBST(string sp, string sn, wezel * v )
{
  string s;

  if( v )
  {
    s = sp;
    if( sn == cr ) s [ s.length( ) - 2 ] = ' ';
    printBST ( s + cp, cr, v->prawy );

    s = s.substr ( 0, sp.length( ) - 2 );
    cout << s << sn << v->wartosc << endl;

    s = sp;
    if( sn == cl ) s [ s.length( ) - 2 ] = ' ';
    printBST ( s + cp, cl, v->lewy );
  }
}

int drzewo::DodajBST(const int a)
{
    wezel *nowy;
    nowy = new wezel();
    nowy -> wartosc = a;
    nowy -> lewy = NULL;
    nowy -> prawy = NULL;
    nowy -> rodzic = NULL;

    if (licznik==0){
       korzen = nowy;
       licznik++;
       return 0;
    }
    else
    {
        wezel *miejsce = korzen;      //wskazik miejsce bedzie przebiegal sciezke w dol drzewa, na poczatku wskazuje na korzen
        wezel *rodzic_miejsca;       //rodzic miejsce wskazuje a rodzica miejsca, czyli poczatkowo na NULL*/

        //petla wykonuje sie dopoki miejsce nie jest wskaznikiem na zero
        while(miejsce != NULL){

              rodzic_miejsca = miejsce;
              if(a <  miejsce->wartosc){
                   miejsce = miejsce ->lewy;
              } else
              if (a > miejsce->wartosc)
              {
                    miejsce = miejsce ->prawy;
              }
                else
                                       break;
        }
        if ( a < rodzic_miejsca->wartosc){

             rodzic_miejsca->lewy = nowy;
             nowy -> rodzic = rodzic_miejsca;
             licznik++;
             return 0;
        }
        if ( a > rodzic_miejsca->wartosc){
             rodzic_miejsca->prawy = nowy;
             nowy -> rodzic = rodzic_miejsca;
             licznik++;
             return 0;
        }
    }


}

class kopiec
{
public:

    int rozmiar;             //rozmiar kopca
    int tab[N];             // tablica kopca wielkosci N= 5000 zdefiniowane wyzej

    kopiec(){
        rozmiar = 0;        //konstruktor drzewa
    }
    int  dodaj(int v);                            //dodaje element do kopca
    void kasuj_korzen();                         //usuwa korzen
    void printK(string sp, string sn, int v);   // drukuje kopiec
    void sort();                               // wypisuje posortowane elementy kopca, wypisuje wartosc korzenia i go usuwa
};
//-------------------------- Funkcje do klasy kopiec---------------

void kopiec::kasuj_korzen() {   //funcja kasuje korzen kopca, nastpenym korzeniem staje sie najwiekszy element w kopcu
   int i, j, v;

  if( rozmiar-- )
  {
    v = tab [ rozmiar ];

    i = 0;
    j = 1;

    while( j < rozmiar )
    {
      if( j + 1 < rozmiar && tab [ j + 1 ] > tab [ j ] ) j++;
      if( v >= tab [ j ] ) break;
      tab [ i ] = tab [ j ];
      i = j;
      j = 2 * j + 1;
    }

    tab [ i ] = v;
  }
}

void kopiec::sort()  //wypisuje zawartosc kopca w porządku malejącym bo kopiec jest typu max
{   
    while( rozmiar ){

        cout <<tab [ 0 ]<<" ";
        kasuj_korzen( );
    }
}

int kopiec::dodaj(int v) //dodawanie elementu do kopca
{   
     int i, j;

      i = rozmiar++;
      j = ( i - 1 ) / 2;

      while( i > 0 && tab [ j ] < v ){

            tab [ i ] = tab [ j ];
            i = j;
            j = ( i - 1 ) / 2;
      }

  tab [ i ] = v;

}
void kopiec::printK(string sp, string sn, int v )
{

  string s;

  if( v < rozmiar )
  {
    s = sp;
    if( sn == cr ) s [ s.length( ) - 2 ] = ' ';
    printK ( s + cp, cr, 2 * v + 2 );

    s = s.substr ( 0, sp.length( ) - 2 );

    cout << s << sn << tab [ v ] << endl;

    s = sp;
    if( sn == cl ) s [ s.length( ) - 2 ] = ' ';
    printK ( s + cp, cl, 2 * v + 1 );
  }
}





int main()
{

    cr = cl = cp = "  ";
    cr [ 0 ] = 218; cr [ 1 ] = 196;
    cl [ 0 ] = 192; cl [ 1 ] = 196; // odczytanie poszczegolnych znakow potrzebnych do wypisywania drzew itp...
    cp [ 0 ] = 179;
     drzewo Drzewo;
     kopiec Kopiec;
     wezel Wezel;

        int k,l,m;
menuG: //obsluga menu glownego
        cout<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cout<<"1 :: Utworz drzewo BST ::"<<endl;
        cout<<"2 :: Utworz kopiec Binarny ::"<<endl;
        cout<<"3 :: Utworz zwykle drzewo binarne ::"<<endl;
        cout<<"4 :: wyjscie ::"<<endl<<endl;
        cout<<"  :: Co chcesz zrobic? ::"<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cin>>k;
        Drzewo.Wyzeruj();
menu:
switch(k){

case 1:{    // Drzewo BST


        cout<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cout<<"1 :: dodaj element do drzewa BST ::"<<endl;
        cout<<"2 :: usun element z drzewa BST ::"<<endl;
        cout<<"3 :: wyswietl utworzone drzewo BST ::"<<endl;
        cout<<"4 :: szukaj elementu w drzewie ::"<<endl;
        cout<<"5 :: wyczysc drzewo BST ::"<<endl;
        cout<<"6 :: Wypisz najmniejszy element utworzonego drzewa BST ::"<<endl;
        cout<<"7 :: Wypisz najwiekszy element utworzonego drzewa BST::"<<endl;
        cout<<"8 :: wyjscie do glownego menu ::"<<endl;
        cout<<"9 :: wyjscie z programu ::"<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;

        cin>>m;

        switch(m){
            case 1:{
                int wartosc;

                cout<<endl;
                cout<<"Prosze o podanie liczby do dodania, pierwsza podana wartosc bedzie korzeniem"<<endl;
                cout<<"wpisz 'q' aby przerwac wpisywanie "<<endl;
                while(getchar() != 'q'){
                    if(scanf("%d", &wartosc) == 1){
                        Drzewo.DodajBST(wartosc);
                        cout<<"------Drzewo BST po dodaniu: "<<wartosc<<"------"<<endl<<endl;
                        Drzewo.printBST("","",korzen);
                    }
                }
             goto menu;
            break;
            }

            case 2:
                int wartosc2;
                cout<<endl;
                cout<<"Prosze o podanie liczby do usuniecia"<<endl;
                cout<<"wpisz 'q' aby przerwac wpisywanie "<<endl;

                while(getchar() != 'q'){

                   if(scanf("%d", &wartosc2) == 1){
                        kasowanie(szukajBST(korzen, wartosc2));
                        cout<<"------Drzewo BST po usunieciu: "<<wartosc2<<"------"<<endl<<endl;
                        Drzewo.printBST("","",korzen);
                   }
                }
             goto menu;

            break;


            case 3:
                cout<<":: Wyswietlam drzewo ::"<<endl<<endl;
                Drzewo.printBST("","",korzen);
             goto menu;

            break;

            case 4:
                int wartosc3;
                cout<<endl;
                cout<<"Prosze o podanie liczby ktorej szukasz"<<endl;
                cout<<"wpisz 'q' aby przerwac wpisywanie "<<endl;

                while(getchar() != 'q'){
                    if(scanf("%d", &wartosc3) == 1){
                        if(szukajBST(korzen,wartosc3) != NULL) cout<<endl<<" "<<wartosc3<<" znajduje sie w drzewie"<<endl;
                        else cout<<endl<<" "<<wartosc3<<" nie znajduje sie w drzewie"<<endl;
                    }
                }
             goto menu;
            break;

            case 5:
                 Drzewo.Wyzeruj();

             goto menu;

            break;
              case 6:
                    wezel *p;
                    p = naj_lewo(korzen);
                    cout<<":: Wyswietlam najmniejszy element ::"<<endl<<endl;
                    cout<<"element: "<<p->wartosc<<" jest najmniejszy w podanym drzewie"<<endl;

             goto menu;

            break;
              case 7:
                    wezel *k;
                    k = naj_prawo(korzen);
                    cout<<":: Wyswietlam najwiekszy element ::"<<endl<<endl;
                    cout<<"element: "<<k->wartosc<<" jest najwiekszy w podanym drzewie"<<endl;


             goto menu;

            break;

            case 8:
             goto menuG;
            break;

            case 9:
                Drzewo.Wyzeruj();
                delete &Drzewo;
                Wezel.deleteBT();           // przy wyjsciu czysciymy co sie da
                delete &Wezel;
                while( Kopiec.rozmiar ){
                    Kopiec.kasuj_korzen( );
                }
                delete &Kopiec;
                return 0;
            break;


        } // zakonczenie wewnetrzengo menu


break;  // break do  case 1 z zew menu
}
case 2:{ // case 2 obsluga kopca
        int l;
        cout<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cout<<"1 :: dodaj element do kopca ::"<<endl;
        cout<<"2 :: usun korzen ::"<<endl;
        cout<<"3 :: wyswietl utworzony kopiec ::"<<endl;
        cout<<"4 :: sortuj kopiec ::"<<endl;
        cout<<"5 :: wyjscie do glownego menu ::"<<endl;
        cout<<"6 :: wyjscie z programu ::"<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;

        cin>>l;

        switch(l){
            case 1:{
                int wartosc;
                cout<<endl;
                cout<<"Prosze o podanie liczby ktora chcesz dodac do kopca"<<endl;
                cout<<"wpisz 'q' aby przerwac wpisywanie "<<endl;

                while(getchar() != 'q'){

                    if(scanf("%d", &wartosc) == 1){
                        Kopiec.dodaj(wartosc);
                        cout<<"------Kopiec po dodaniu: "<<wartosc<<"------"<<endl<<endl;
                        Kopiec.printK("","",0);
                    }
                }
             goto menu;
            break;
            }

            case 2:
                int wartosc2;
                    Kopiec.kasuj_korzen();
                    cout<<"------Kopiec po usunieciu korzenia: "<<endl<<endl;
                    Kopiec.printK("","",0);

             goto menu;
            break;

            case 3:
                cout<<":: Wyswietlam kopiec ::"<<endl<<endl;
                Kopiec.printK("","",0);
                goto menu;

            break;

            case 4:
                cout<<":: Wyswietlam posortowane elementy kopca ::"<<endl<<endl;
                Kopiec.sort();
                cout<<endl;

                goto menu;
            break;

            case 5:
               goto menuG;
            break;

            case 6:
                Drzewo.Wyzeruj();
                delete &Drzewo;
                Wezel.deleteBT();           // przy wyjsciu czysciymy co sie da
                delete &Wezel;
                while( Kopiec.rozmiar ){
                    Kopiec.kasuj_korzen( );
                }
                delete &Kopiec;
                return 0;
            break;


        } // zakonczenie wew menu


break; // break do case 2 zew menu
}

case 3:{ // case 3 zew menu, obsluga drzewa binarnego
        int l;
        cout<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cout<<"1 :: wczytaj drzewo binarne ::"<<endl;
        cout<<"2 :: Zbadaj drzewo binarne ::"<<endl;
        cout<<"3 :: wyswietl utworzone drzewo binarne ::"<<endl;
        cout<<"4 :: wyjscie do glownego menu ::"<<endl;
        cout<<"5 :: wyjscie z programu ::"<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;

        cin>>l;

        switch(l){
            case 1:{
                cout<<"Drzewo wczytujemy w calosci poniewaz od razu musimy okreslic ktory element jest dzieckiem i czyim"<<endl<<endl;
                cout<<"Podajemy: liczbe wezlow, wartosc wezla, numery jego dzieci(zera jezeli ich nie posiada)"<<endl<<endl;
                cout<<"Jezeli wezel posiada dzieci w nastepnej linijce wpisujemy jego wartosc i czy ma dzieci "<<endl<<endl;
                cout<<"przyklad: "<<endl;
                cout<<" 4      (liczba wezlow)"<<endl;
                cout<<" 5  1 2 (kolejno: wartosc_wezla                numer_dziecka1 numer_dziecka2)"<<endl;
                cout<<" 11 0 0 (kolejno: wartosc_wezla(dziecko nr: 1) brak_dziecka   brak_dziecka)"<<endl;
                cout<<" 7  3 0 (kolejno: wartosc_wezla(dziecko nr: 2) numer_dziecka3 brak_dziecka)"<<endl;
                cout<<" 8  0 0 (kolejno: wartosc_wezla(dziecko nr: 3) brak_dziecka   brak_dziecka)"<<endl;
                Wezel.wczytaj();
                cout<<"------ Powstale drzewo binarne -------"<<endl;
                Wezel.printBT("", "", korzen);
                /* Inny przykład drzewa, bardziej skomplikowane
                    12
                    5 1 2
                    11 3 4
                    7 5 6
                    32 7 8
                    -16 0 9
                    21 10 11
                    -2 0 0
                    3 0 0
                    17 0 0
                    -5 0 0
                    4 0 0
                    1 0 0
                */

             goto menu;
            break;
            }

            case 2:
                Wezel.DFS(korzen);
                Wezel.wypisz();

             goto menu;
            break;

            case 3:
                cout<<":: Wyswietlam drzewo Binarne ::"<<endl<<endl;
                Wezel.printBT("", "", korzen);
                goto menu;

            break;
            case 4:
               goto menuG;
            break;

            case 5:
                Drzewo.Wyzeruj();
                delete &Drzewo;
                Wezel.deleteBT();           // przy wyjsciu czysciymy co sie da
                delete &Wezel;
                while( Kopiec.rozmiar ){
                    Kopiec.kasuj_korzen( );
                }
                delete &Kopiec;
                return 0;
            break;




        } // zakonczenie wew menu

break;// zakonczenie case 3 zew menu
}

case 4:
    Drzewo.Wyzeruj();
    delete &Drzewo;
    Wezel.deleteBT();           // przy wyjsciu czysciymy co sie da
    delete &Wezel;
    while( Kopiec.rozmiar ){
        Kopiec.kasuj_korzen( );
    }
    delete &Kopiec;
    return 0;
break;
}

 return 0;
}
