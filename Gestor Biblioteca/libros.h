#ifndef LIBROS_H_INCLUDED
#define LIBROS_H_INCLUDED

class Libro;

void agregarLibro();
void mostrarArchivoLibros();
int libroUltimaID();
int buscarLibroID(int id); ///Devuelve la pos del id ingresado
void eliminarLibro(int id); ///Modifica el estado a false
void listarPorGenero(int gen);
void listarPorAutor(const char *autor);
void listarLibroNombre(const char *n);

void modificarNombreLibro(int id);
void modificarGeneroLibro(int id);
void modificarAutorLibro(int id);
void modificarExistencias(int id);
void subMenuLibrosConsultas();
void subMenuLibrosModificaciones();

Libro retornarLibro(int id);

string generoNombre(int n); ///Convierte el numero del genero en el nombre

class Libro{
    private:
        char nombre[50];
        char autor[50];
        int idLibro;
        int cant;
        int genero;///del 1 al 10
        bool estado;
    public:
        bool cargar();
        void mostrar();

        bool leerDeDisco(int pos);
        bool grabarEnDisco();
        bool modificarEnDisco(int pos);

        void setNombre(const char *n){strcpy(nombre,n);}
        void setAutor(const char *a){strcpy(autor,a);}
        void setID(int id){idLibro=id;}
        void setCant(int e){cant=e;}
        void setGenero(int g){genero=g;}
        void setEstado(bool e){estado=e;}

        Libro(int c=0){cant=c;}
        const char *getNombre(){return nombre;}
        const char *getAutor(){return autor;}
        int getIDLibro(){return idLibro;}
        int getGenero(){return genero;}
        int getCant(){return cant;}
        bool getEstado(){return estado;}
};

bool Libro::cargar(){
    idLibro=libroUltimaID()+1;
    cout<<"Ingrese el nombre del libro: ";
    cargarCadena(nombre,50);
    cout<<endl;
    cout<<"Ingrese el nombre del autor: ";
    cargarCadena(autor,50);
    cout<<endl;
    cout<<"Ingrese el genero del libro(entre 1 y 10): ";
    cin>>genero;
    cout<<endl;
    if(genero<1 || genero>10){
        cout<<"EL GENERO DEBE ESTAR ENTRE 1 Y 10"<<endl;
        return false;
    }
    cout<<"Ingrese las existencias: ";
    cin>>cant;
    cout<<endl;
    estado=true;
    return true;
}

void Libro::mostrar(){
    if(estado){
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
    cout<<"ID: "<<idLibro<<endl;
    cout<<"Nombre: "<<nombre<<endl;
    cout<<"Autor: "<<autor<<endl;
    cout<<"Genero: "<<generoNombre(genero)<<endl;
    cout<<"Existencias: "<<cant<<endl;
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::";
    }
}

bool Libro::grabarEnDisco(){
    bool grabo;
    FILE *p;
    p=fopen("Libros.dat","ab");
    if(p==NULL){
        cout<<"No se pudo crear/abrir archivo"<<endl;
        return false;
    }
    grabo=fwrite(this,sizeof *this,1,p);
    fclose(p);
    return grabo;
}

bool Libro::leerDeDisco(int pos){
    bool leyo;
    FILE *p;
    p=fopen("Libros.dat","rb");
    if(p==NULL){
        cout<<"No se pudo abrir archivo"<<endl;
        return false;
    }
    fseek(p,pos * sizeof *this,0);
    leyo=fread(this,sizeof *this,1,p);
    fclose(p);
    return leyo;
}

bool Libro::modificarEnDisco(int pos){
    bool leyo;
    FILE *p;
    p=fopen("Libros.dat","rb+");
    if(p==NULL){
        cout<<"No se pudo abrir el archivo"<<endl;
        return false;
    }
    fseek(p,pos * sizeof *this,0);
    leyo=fwrite(this,sizeof *this,1,p);
    fclose(p);
    return leyo;
}

void mostrarArchivoLibros(){
    int pos=0;
    Libro reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado()){
            reg.mostrar();
            cout<<endl;
        }
    }
}

void agregarLibro(){
    Libro reg;
    if(reg.cargar()){
        if(reg.grabarEnDisco()){
        cout<<"Registro agregado"<<endl;
        }
    }
    else{
        cout<<"No se pudo agregar el registro"<<endl;
    }

}

int libroUltimaID(){
    Libro aux;
    int maximo=0;
    int pos=0;
    while(aux.leerDeDisco(pos++)){
        if(aux.getEstado() && aux.getIDLibro()>maximo){
            maximo=aux.getIDLibro();
        }
    }
    system("cls");
    return maximo;
}

int buscarLibroID(int id){
    Libro reg;
    int pos=0;
    while(reg.leerDeDisco(pos)){
        if (id==reg.getIDLibro() && reg.getEstado()){
            return pos;
        }
    pos++;
    }
    return -1;
}

void eliminarLibro(int id){
    Libro reg;
    int pos;
    char opc;
    pos=buscarLibroID(id);
    if(pos==-1){
        cout<<"No existe libro con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
            cout<<"Esta seguro que quiere eliminar el libro "<<reg.getNombre()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            reg.setEstado(false);
            if(reg.modificarEnDisco(pos)){
                cout<<"El libro ha sido eliminado."<<endl;
            }
            else{
                cout<<"No se pudo eliminar el libro."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void listarPorGenero(int gen){
    int pos=0;
    Libro reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado() && reg.getGenero()==gen){
            reg.mostrar();
            cout<<endl;
        }
    }
}

void listarPorAutor(const char *autor){
    int pos=0,c=0;
    Libro reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado() && strcmp(autor,reg.getAutor())==0){
            reg.mostrar();
            cout<<endl;
            c++;
        }
}
    if(c==0){
            cout<<"EL NOMBRE INGRESADO NO EXISTE"<<endl;
        }
}
void listarLibroNombre(const char *n){
    int pos=0,c=0;
    Libro reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado() && strcmp(n,reg.getNombre())==0){
            reg.mostrar();
            cout<<endl;
            c++;
        }
}
    if(c==0){
            cout<<"EL NOMBRE INGRESADO NO EXISTE"<<endl;
        }
}

int buscarIdLibro(int id){
int pos=0;
Libro obj;
while(obj.leerDeDisco(pos)){
    if(id==obj.getIDLibro() && obj.getEstado()){
            return pos;

    }
    pos++;
    }
    return -1;
}

Libro retornarLibro(int id){
    int pos;
    Libro obj;
    pos=buscarIdLibro(id);
    while(obj.leerDeDisco(pos++)){
        if(obj.getIDLibro()==id){
            return obj;
        }
    }
}


void modificarNombreLibro(int id){
    Libro reg;
    int pos;
    char opc;
    char n[30];
    pos=buscarLibroID(id);
    if(pos==-1){
        cout<<"No existe libro con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el libro "<<reg.getNombre()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO NOMBRE: ";
            cargarCadena(n,30);
            cout<<endl;
            reg.setNombre(n);
            if(reg.modificarEnDisco(pos)){
                cout<<"El libro ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el libro."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void modificarGeneroLibro(int id){
    Libro reg;
    int pos,gen;
    char opc;
    pos=buscarLibroID(id);
    if(pos==-1){
        cout<<"No existe libro con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el libro "<<reg.getNombre()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO GENERO: ";
            cin>>gen;
            while(gen<1 || gen>10){
                cout<<"DEBE INGRESAR UN NUMERO ENTRE 1 Y 10"<<endl;
                cout<<"INGRESE EL NUEVO GENERO: ";
                cin>>gen;
            }
            reg.setGenero(gen);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El libro ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el libro."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void modificarAutorLibro(int id){
    Libro reg;
    int pos;
    char opc;
    char n[30];
    pos=buscarLibroID(id);
    if(pos==-1){
        cout<<"No existe libro con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el libro "<<reg.getNombre()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO AUTOR: ";
            cargarCadena(n,30);
            reg.setAutor(n);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El libro ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el libro."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void modificarExistencias(int id){
    Libro reg;
    int pos,existencias;
    char opc;
    pos=buscarLibroID(id);
    if(pos==-1){
        cout<<"No existe libro con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el libro "<<reg.getNombre()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE NUEVAS EXISTENCIAS: ";
            cin>>existencias;
            while(existencias<0){
                cout<<"VALOR INVALIDO"<<endl;
                cout<<"INGRESE NUEVAS EXISTENCIAS: ";
                cin>>existencias;
            }
            reg.setCant(existencias);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"Las existencias han sido modificadas con exito."<<endl;
            }
            else{
                cout<<"No fue posible modificar las existencias."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}


void subMenuLibros(){
    int id,opc;
    while(true){
    cout<<"MENU LIBROS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. ALTA"<<endl;
    cout<<"2. BAJA"<<endl;
    cout<<"3. CONSULTAS"<<endl;
    cout<<"4. MODIFICACIONES"<<endl;
	cout<<"<--------------------------------->"<<endl;
	cout<<"0. VOLVER"<<endl<<endl;

	cout<<"SELECCIONE UNA OPCION: ";
	cin>>opc;
	system("cls");
	switch(opc){
        case 1:
            agregarLibro();
            system("pause>nul");
                break;
        case 2:
            cout<<"ID de libro a dar de baja: ";
            cin>>id;
            cout<<endl;
            eliminarLibro(id);
            system("pause>nul");
                break;
        case 3:
            subMenuLibrosConsultas();
            break;
        case 4:
            subMenuLibrosModificaciones();
            break;
        case 0: return;
            break;
	}
	system("cls");
	}
}


void subMenuLibrosConsultas(){
    int opc,id;
    char n[30]={};
    while(true){
    cout<<"MENU CONSULTAS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MOSTRAR TODO"<<endl;
    cout<<"2. BUSCAR LIBRO"<<endl;
    cout<<"3. LISTAR POR GENERO"<<endl;
    cout<<"4. LISTAR POR AUTOR"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1: mostrarArchivoLibros();
            break;
        case 2:
            cout<<"INGRESE NOMBRE DEL LIBRO: ";
            cargarCadena(n,50);
            cout<<endl;
            listarLibroNombre(n);
            break;
        case 3:
            cout<<"INGRESE EL GENERO: ";
            cin>>id;
            cout<<endl;
            listarPorGenero(id);
            break;
        case 4:
            cout<<"INGRESE EL NOMBRE DEL AUTOR: ";
            cargarCadena(n,30);
            cout<<endl;
            listarPorAutor(n);
            break;
        default: return;
    }
    system("pause>nul");
	system("cls");
    }
}

void subMenuLibrosModificaciones(){
    int opc,id;
    while(true){
    cout<<"MENU MODIFICACIONES"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MODIFICAR NOMBRE"<<endl;
    cout<<"2. MODIFICAR GENERO"<<endl;
    cout<<"3. MODIFICAR AUTOR"<<endl;
    cout<<"4. MODIFICAR EXISTENCIAS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            cout<<"INGRESE ID DEL LIBRO A MODIFICAR EL NOMBRE: ";
            cin>>id;
            cout<<endl;
            modificarNombreLibro(id);
            break;
        case 2:
            cout<<"INGRESE ID DEL LIBRO A MODIFICAR EL GENERO: ";
            cin>>id;
            cout<<endl;
            modificarGeneroLibro(id);
            break;
        case 3:
            cout<<"INGRESE ID DEL LIBRO A MODIFICAR EL AUTOR: ";
            cin>>id;
            cout<<endl;
            modificarAutorLibro(id);
            break;
        case 4:
            cout<<"INGRESE ID DEL LIBRO A MODIFICAR LAS EXISTENCIAS: ";
            cin>>id;
            cout<<endl;
            modificarExistencias(id);
            break;
        case 0: return;
            break;
    }
    system("pause>nul");
    system("cls");
    }
}

string generoNombre(int n){
     string gen1="Fantasia",gen2="Terror",gen3="Ciencia ficcion",gen4="Romance",gen5="Historico";
     string gen6="Religion",gen7="Manual",gen8="Policial",gen9="Comic",gen10="Filosofia",def="Nada";
    switch(n){
        case 1: return gen1;
            break;
        case 2: return gen2;
            break;
        case 3: return gen3;
            break;
        case 4: return gen4;
            break;
        case 5: return gen5;
            break;
        case 6: return gen6;
            break;
        case 7: return gen7;
            break;
        case 8: return gen8;
            break;
        case 9: return gen9;
            break;
        case 10: return gen10;
            break;
        default: return def;
    }
}

#endif // LIBROS_H_INCLUDED

