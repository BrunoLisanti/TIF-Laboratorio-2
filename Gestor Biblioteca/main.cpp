#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>



using namespace std;

void cargarCadena(char *pal, int tam){
    int i;
    fflush(stdin);
    for(i=0;i<tam;i++){
        pal[i]=cin.get();
        if(pal[i]=='\n') break;
        }
    pal[i]='\0';
    fflush(stdin);
}
#include "fecha.h"
#include "socios.h"
#include "libros.h"
#include "cuota.h"
#include "prestamos.h"


int main()
{
    Fecha obj;
    int opc;
    while(true){

    cout<<"MENU PRINCIPAL"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MENU LIBROS"<<endl;
    cout<<"2. MENU SOCIOS"<<endl;
    cout<<"3. MENU PRESTAMOS"<<endl;
    cout<<"4. MENU CUOTAS"<<endl;
    cout<<"<--------------------------------->"<<endl;

    cout<<"0. SALIR"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1: subMenuLibros();
            break;
        case 2: menuSocios();
            break;
        case 3: menuPrestamos();
            break;
        case 4: menuCuotas();
            break;
        case 0: return 0;
            break;
    }



    system("cls");
    }
}
