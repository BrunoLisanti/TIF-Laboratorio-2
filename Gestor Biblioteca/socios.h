#ifndef SOCIO_H_INCLUDED
#define SOCIOS_H_INCLUDED

bool comprobacionDni(int d);
int socioUltimaID();
void agregarSocio();
void darDeBajaSocio(int id);
int buscarIdSocio(int id);
void subMenuSociosConsultas();
void subMenuSociosModificaciones();

void modificarSocioNombre(int id);
void modificarSocioDomicilio(int id);
void modificarSocioEdad(int id);
void modificarSocioDni(int id);

class Socio{
private:
    char nombre[30];
    char apellido[30];
    int idSocio;
    int edad;
    int dni;
    char domicilio[30];
    bool estado;
public:

    void setNombre(const char*n){strcpy(nombre,n);}
    void setApellido(const char *a){strcpy(apellido,a);}
    void setIdSocio(int n){idSocio=n;}
    void setEdad(int e){edad=e;}
    void setDni(int d){dni=d;}
    void setDomicilio(const char *a){strcpy(apellido,a);}
    void setEstado(bool est){estado=est;}

    const char *getNombre(){return nombre;}
    const char *getApellido(){return apellido;}
    int getIdSocio(){return idSocio;}
    int getEdad(){return edad;}
    int getDni(){return dni;}
    const char *getDomicilio(){return domicilio;}
    bool getEstado(){return estado;}

    bool cargar();
    void mostrar();

    bool leerDeDisco(int pos);
    bool grabarEnDisco();
    bool modificarEnDisco(int pos);
};

int socioUltimaID(){
    Socio aux;
    int maximo=0;
    int pos=0;
    while(aux.leerDeDisco(pos++)){
        if(aux.getEstado() && aux.getIdSocio()>maximo){
            maximo=aux.getIdSocio();
        }
    }
    system("cls");
    return maximo;
}

  bool Socio::cargar(){
      int aux;
        idSocio=socioUltimaID()+1;
        cout<<"INGRESE NOMBRE: ";
        cargarCadena(nombre,30);
        cout<<endl<<"INGRESE APELLIDO: ";
        cargarCadena(apellido,30);
        cout<<endl;
        cout<<"INGRESE DOMICILIO: ";
        cargarCadena(domicilio,30);
        cout<<endl;
        cout<<"INGRESE EDAD: ";
        cin>>edad;
        cout<<endl;
        cout<<"INGRESE DNI: ";
        cin>>aux;
        cout<<endl;
        if(!comprobacionDni(aux)){
            return false;}
        dni=aux;
        estado=true;
        }

void Socio::mostrar(){
    if(estado){
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
        cout<<nombre<<" "<<apellido<<endl;
        cout<<"ID: ";
        cout<<idSocio<<endl;
        cout<<"EDAD: ";
        cout<<edad<<endl;
        cout<<"DNI: ";
        cout<<dni<<endl;
        cout<<"DOMICILIO: ";
        cout<<domicilio<<endl;
        cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::";
    }
}
bool Socio::leerDeDisco(int pos){
        FILE *p;
        p=fopen("Socios.dat","rb");
        if(p==NULL){
            return false;
        }
        fseek(p,sizeof(Socio)*pos,0);
        bool leyo=fread(this,sizeof(Socio),1,p);
        fclose(p);
        return leyo;
    }


bool Socio::grabarEnDisco(){
        FILE *p;
        p=fopen("Socios.dat","ab");
        if(p==NULL){
            cout<<"NO SE PUDO CREAR/ABRIR EL ARCHIVO";
            cout<<endl;
            return false;
        }
        bool escribio=fwrite(this,sizeof(Socio),1,p);
        fclose(p);
        return escribio;
    }

bool Socio::modificarEnDisco(int pos){
    bool leyo;
    FILE *p;
    p=fopen("Socios.dat","rb+");
    if(p==NULL){
        cout<<"NO SE PUDO ABRIR EL ARCHIVO"<<endl;
        return false;
    }
    fseek(p,pos * sizeof *this,0);
    leyo=fwrite(this,sizeof *this,1,p);
    fclose(p);
    return leyo;
}

void agregarSocio(){
    Socio reg;
    if(reg.cargar()){
            if(reg.grabarEnDisco()){
            cout<<"REGISTRO AGREGADO"<<endl;
        }
        else{
            cout<<"NO SE PUDO AGREGAR EL REGISTRO"<<endl;
        }
    }

}

int buscarId(int id){
int pos=0;
Socio obj;
while(obj.leerDeDisco(pos)){
        if(id==obj.getIdSocio()){
            return pos;
        }
        pos++;
    }
    return -1;
}

Socio retornarSocio(int id){
    int pos;
    Socio obj;
    pos=buscarIdSocio(id);
    while(obj.leerDeDisco(pos++)){
        if(obj.getIdSocio()==id){
            return obj;
        }
    }
}

void darDeBajaSocio(int id){
    int pos;
    Socio obj;
    char opc;
    pos=buscarId(id);
    if(pos==-1){
            cout<<"NO EXISTE UN SOCIO CON EL ID INGRESADO"<<endl;
            return;
        }
        obj.leerDeDisco(pos);
        do{
            cout<<"Esta seguro que quiere eliminar el socio "<<obj.getNombre()<<" "<<obj.getApellido()<<"?"<<" S/N: ";
            cin>>opc;
            if(opc=='s' || opc=='S'){
                obj.setEstado(false);
                if(obj.modificarEnDisco(pos)){
                    cout<<"El socio ha sido eliminado."<<endl;
                }
                else{
                    cout<<"No se pudo eliminar el socio."<<endl;
                }
            }
            else if(opc=='n' || opc=='N'){return;}
            else{cout<<"Opcion invalida"<<endl;}
        }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}
void mostrarSocioDni(int dni){
    int pos=0,registro=-1;
    Socio obj;
    while(obj.leerDeDisco(pos)){
        if(dni==obj.getDni() && obj.getEstado()){
            registro=pos;
        }
        pos++;
    }
    if(registro==-1){
        cout<<"NO EXISTE SOCIO CON ESE DNI"<<endl;
    }
    else{
        obj.leerDeDisco(registro);
        obj.mostrar();
    }
}

void mostrarSocioNombre(char *n, char *a){
    int pos=0,registro=-1;
    Socio obj;
    while(obj.leerDeDisco(pos++)){
        if(obj.getEstado() && strcmp(n,obj.getNombre())==0 && strcmp(a,obj.getApellido())==0){
            registro=pos;
        }
        else{
            cout<<"EL NOMBRE NO EXISTE"<<endl;
            return;
        }
    }
    if(registro==-1){
        cout<<"NO EXISTE SOCIO CON TAL NOMBRE"<<endl;
    }
    else{
        obj.leerDeDisco(registro);
        obj.mostrar();
    }
}

void mostrarArchivoSocios(){
int pos=0;
Socio obj;
while(obj.leerDeDisco(pos)){
        if(obj.getEstado()){
        obj.mostrar();
        cout<<endl;
    }
    pos++;
}
return;
}

int buscarIdSocio(int id){
    Socio reg;
    int pos=0;
    while(reg.leerDeDisco(pos)){
            if(reg.getEstado() && reg.getIdSocio()==id){
            return pos;
        }
        pos++;
    }
        return -1;
}


bool comprobacionDni(int d){
Socio reg;
int pos=0;
while(reg.leerDeDisco(pos++)){
    if(reg.getEstado() && reg.getDni()==d){
        cout<<"YA HAY UN SOCIO CON ESE DNI";
        return false;}
}
return true;
}

void modificarNombreSocio(int id){
    Socio reg;
    char opc;
    char n[30],a[30];
    int pos;
    pos=buscarIdSocio(id);
    if(pos==-1){
        cout<<"No existe socio con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el socio "<<reg.getNombre()<<" "<<reg.getApellido()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO NOMBRE: ";
            cargarCadena(n,30);
            cout<<"INGRESE EL NUEVO APELLIDO: ";
            cargarCadena(a,30);
            reg.setNombre(n);
            reg.setApellido(a);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El socio ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el socio."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void modificarDomicilioSocio(int id){
    Socio reg;
    char opc;
    char n[30];
    int pos;
    pos=buscarIdSocio(id);
    if(pos==-1){
        cout<<"No existe socio con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el socio "<<reg.getNombre()<<" "<<reg.getApellido()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO NOMBRE: ";
            cargarCadena(n,30);
            reg.setDomicilio(n);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El socio ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el socio."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void modificarEdadSocio(int id){
    Socio reg;
    int pos,e;
    char opc;
    pos=buscarIdSocio(id);
    if(pos==-1){
        cout<<"No existe socio con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el socio "<<reg.getNombre()<<" "<<reg.getApellido()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE LA NUEVA EDAD: ";
            cin>>e;
            reg.setEdad(e);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El socio ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el socio."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void modificarDniSocio(int id){
    Socio reg;
    char opc;
    int pos,dni;
    pos=buscarIdSocio(id);
    if(pos==-1){
        cout<<"No existe socio con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    do{
        cout<<"Esta seguro que quiere modificar el socio "<<reg.getNombre()<<" "<<reg.getApellido()<<"?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO DNI: ";
            cin>>dni;
            reg.setDni(dni);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El socio ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el socio."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}



void menuSocios(){

    while(true){
    system("cls");
    int id,opc;
    cout<<"MENU SOCIOS"<<endl;
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
            agregarSocio();
            system("pause");
                break;
        case 2:
            cout<<"INGRESE EL ID DEL SOCIO A ELMINAR: ";
            cin>>id;
            cout<<endl;
            darDeBajaSocio(id);
                break;
        case 3:
            subMenuSociosConsultas();
            break;
        case 4:
            subMenuSociosModificaciones();
            break;
        case 0:
            return;
	}
	system("cls");
	}
}

void subMenuSociosConsultas(){
    int opc,id;
    char n[30],a[30];
    while(true){
    cout<<"MENU CONSULTAS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MOSTRAR TODO"<<endl;
    cout<<"2. BUSCAR POR DNI"<<endl;
    cout<<"3. BUSCAR POR NOMBRE"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1: mostrarArchivoSocios();
            break;
        case 2:
            cout<<"INGRESE EL DNI: ";
            cin>>id;
            cout<<endl;
            mostrarSocioDni(id);
            break;
        case 3:
            cout<<"INGRESE EL NOMBRE: ";
            cargarCadena(n,30);
            cout<<endl;
            cout<<"INGRESE EL APELLIDO: ";
            cargarCadena(a,30);
            cout<<endl;
            mostrarSocioNombre(n,a);
            break;
        default: return;
    }
    system("pause>nul");
	system("cls");
    }
}


void subMenuSociosModificaciones(){
    int opc,id;
    while(true){
    cout<<"MENU MODIFICACIONES"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MODIFICAR NOMBRE"<<endl;
    cout<<"2. MODIFICAR DOMICILIO"<<endl;
    cout<<"3. MODIFICAR EDAD"<<endl;
    cout<<"4. MODIFICAR DNI"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            cout<<"INGRESE ID DEL SOCIO A MODIFICAR EL NOMBRE: ";
            cin>>id;
            cout<<endl;
            modificarNombreSocio(id);
            break;
        case 2:
            cout<<"INGRESE ID DEL SOCIO A MODIFICAR EL DOMICILIO: ";
            cin>>id;
            cout<<endl;
            modificarDomicilioSocio(id);
            break;
        case 3:
            cout<<"INGRESE ID DEL SOCIO A MODIFICAR EDAD: ";
            cin>>id;
            cout<<endl;
            modificarEdadSocio(id);
            break;
        case 4:
            cout<<"INGRESE ID DEL SOCIO A MODIFICAR DNI: ";
            cin>>id;
            cout<<endl;
            modificarDniSocio(id);
            break;
        case 0: return;
            break;
    }
    system("pause>nul");
    system("cls");
    }
}


#endif // SOCIOS_H_INCLUDED
