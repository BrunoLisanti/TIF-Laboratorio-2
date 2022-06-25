#ifndef PRESTAMOS_H_INCLUDED
#define PRESTAMOS_H_INCLUDED

void agregarPrestamo();
void mostrarPrestamosEnCurso();
void finalizarPrestamo(int id);
void mostrarPrestamosDevueltos();
int prestamoUltimaID();
bool comprobacionFechaAtraso(int idSocio);
int buscarPrestamo(int id);
void menuPrestamosRealizados();

void subMenuPrestamosConsultas();
void subMenuPrestamosModificaciones();
void mostrarArchivoPrestamos();
void mostrarDemorados();
void buscarPorFecha();

void modificarSocioPrestamo(int id);
void modificarFechaPrestamo(int id);
void modificarLibroPrestamo(int id);

void buscarPrestamoPorFecha();

class Prestamo{
private:
	int idSocio;
    int idPrestamo;
	int librosPrestadosID;
	Fecha fechaPrestamo;
	Fecha fechaDevolucion;
	bool estado;
public:
    ///SETS:
	void setidSocio(int id){idSocio=id;}
	void setidPrestamo(int idPrest){idPrestamo=idPrest;}
	void setIdLibrosPrestados(int id){librosPrestadosID=id;}
	void setFechaPrestamo(Fecha f){fechaPrestamo=f;}
	void setFechaDevolucion(Fecha f){fechaDevolucion=f;}
	void setEstado(bool e){estado=e;}

    ///GETS:
    int getidSocio(){return idSocio;}
    int getIdPrestamo(){return idPrestamo;}
    int getIdLibrosPrestados(){return librosPrestadosID;}
    Fecha getFechaPrestamo(){return fechaPrestamo;}
    Fecha getFechaDevolucion(){return fechaDevolucion;}
    bool getEstado(){return estado;}

    ///FUNCIONES:
	bool cargar();
	void mostrar();

	bool grabarEnDisco();
	bool leerDeDisco(int pos);
	bool modificarEnDisco(int pos);
};

                                ///*******DESARROLLO DE FUNCIONES!!!*******
///CARGAR PRESTAMO:
bool Prestamo::cargar(){
    int aux;
    Libro auxLib;
    Fecha obj;
    idPrestamo=prestamoUltimaID()+1;
    cout<<"INGRESE EL ID DEL SOCIO: ";
    cin>>aux;

    if(buscarIdSocio(aux)==-1){
        cout<<"NO SE ENCONTRO UN SOCIO CON ESE ID";
        system("pause>nul");
        return false;
    }
    cout<<"NOMBRE DEL SOCIO: "<<retornarSocio(aux).getNombre()<<" "<<retornarSocio(aux).getApellido()<<endl<<endl;

    if(!comprobacionFechaAtraso(aux)){
        cout<<"EL SOCIO NO PUEDE PEDIR OTRO PRESTAMO DEBIDO A DEBER OTRO LIBRO ATRASADO."<<endl;
        system("pause>nul");
        return false;
    }

    if(!comprobacionCuotaAtraso(aux)){
        cout<<"NO SE LE PUEDE PRESTAR DEBIDO A NO PAGAR LA CUOTA."<<endl;
        system("pause>nul");
        return false;
    }
    cout<<"ID DEL PRESTAMO: "<<idPrestamo<<endl;
    idSocio=aux;
    cout<<"INGRESE ID DEL LIBRO A PRESTAR: ";
    cin>>aux;

    if(buscarLibroID(aux)==-1){
        cout<<"EL ID INGRESADO NO CORRESPONDE A NINGUN LIBRO EN EXISTENCIA, INGRESE OTRO."<<endl;
        system("pause");
        return false;
    }
    if(retornarLibro(aux).getCant()==0){
        cout<<"EL LIBRO QUE SE DESEA PEDIR NO ESTA DISPONIBLE";
        system("pause>nul");
        return false;
    }
    librosPrestadosID=aux;

    cout<<"NOMBRE DEL LIBRO: "<<retornarLibro(librosPrestadosID).getNombre()<<endl<<endl;

    cout<<"FECHA EN QUE SE INGRESO EL PRESTAMO: "<<endl;
    fechaPrestamo.cargar();
    cout<<"FECHA DE DEVOLUCION: "<<endl;
    fechaDevolucion=fechaPrestamo+30;
    fechaDevolucion.mostrar();
    system("pause>nul");
    estado=true;
    return true;
}


///MOSTRAR PRESTAMO:
void Prestamo::mostrar(){
    Libro reg;
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
    cout<<"ID DEL SOCIO: "<<idSocio<<endl;
    cout<<"NOMBRE: "<<retornarSocio(idSocio).getNombre()<<" "<<retornarSocio(idSocio).getApellido()<<endl;
    cout<<"ID DEL PRESTAMO: "<<idPrestamo<<endl;
    cout<<"LIBRO: "<<retornarLibro(librosPrestadosID).getNombre()<<endl;
    cout<<"FECHA: ";
    fechaPrestamo.mostrar();
    cout<<"DEVOLUCION: ";
    fechaDevolucion.mostrar();

    cout<<"ESTADO: ";
    if(!estado){
        cout<<"DEVUELTO";
    }
    else if(fechaDevolucion>hoy()){
        cout<<"EN PRESTAMO";
    }
    else{
        cout<<"EN PRESTAMO-DEMORADO";
    }
    cout<<endl;
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
}



///GRABAR EN DISCO:
bool Prestamo::grabarEnDisco(){
FILE *p;
p=fopen("Prestamos.dat","ab");
    if(p==NULL){
        cout<<"NO SE PUDO ABRIR EL ARCHIVO";
        cout<<endl;
        return false;
        }
bool escribio=fwrite(this,sizeof(Prestamo),1,p);
fclose(p);
return escribio;
}



///LEER DE DISCO:
bool Prestamo::leerDeDisco(int pos){
FILE *p;
p=fopen("Prestamos.dat","rb");
    if(p==NULL){return false;}
fseek(p,sizeof(Prestamo)*pos,0);
bool leyo=fread(this,sizeof(Prestamo),1,p);
fclose(p);
return leyo;
    }



///AGREGAR PRESTAMO:
void agregarPrestamo(){
Prestamo reg;
Libro obj;
int pos;
    if(reg.cargar()){
        pos=buscarLibroID(reg.getIdLibrosPrestados());
        obj.leerDeDisco(pos);
        obj.setCant(obj.getCant()-1);
        obj.modificarEnDisco(pos);
        if(reg.grabarEnDisco()){

            cout<<endl<<endl;
            cout<<"REGISTRO AGREGADO";
            cout<<endl;
        }
    }
    else{
        cout<<endl<<endl;
        cout<<"NO SE PUDO AGREGAR EL REGISTRO";}
        cout<<endl;
}

///MOSTRAR TODOS LOS PRESTAMOS CUYA FECHA DE DEVOLUCION YA PASO Y NO HAN SIDO DEVUELTOS:
void mostrarDemorados(){
Prestamo reg;

int pos=0;

while(reg.leerDeDisco(pos++)){
    if(reg.getEstado() && reg.getFechaDevolucion()<hoy()){
        reg.mostrar();
    }
}
}


///MOSTRAR TODOS LOS PRESTAMOS EN CURSO:
void mostrarPrestamosEnCurso(){
    Prestamo reg;

    int pos=0;

    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado() && reg.getFechaDevolucion()>hoy()){
            reg.mostrar();
        }
    }
}

void mostrarPrestamosDevueltos(){
    Prestamo reg;
    int pos=0;
    while(reg.leerDeDisco(pos++)){
        if(!reg.getEstado()){
            reg.mostrar();
        }
    }
}

///MODIFICAR EN DISCO:
bool Prestamo::modificarEnDisco(int pos){
    FILE *p;
    p=fopen("Prestamos.dat","rb+");
    if(p==NULL){
        cout<<"NO SE PUDO ABRIR EL ARCHIVO"<<endl;
        return false;
        }
        fseek(p,sizeof(Prestamo)*pos,0);
        bool leyo=fwrite(this,sizeof(Prestamo),1,p);
        fclose(p);
        return leyo;
}

///BUSCAR PRESTAMO:
int buscarPrestamo(int id){
Prestamo reg;
int pos=0;
while(reg.leerDeDisco(pos)){
        if(reg.getIdPrestamo()==id){
        return pos;
    }
    pos++;
}
    return -1;
}

Prestamo retornarPrestamo(int id){
    int pos;
    Prestamo obj;
    pos=buscarPrestamo(id);
    while(obj.leerDeDisco(pos++)){
        if(obj.getIdPrestamo()==id){
            return obj;
        }
    }
}


///FINALIZAR PRESTAMO:
void finalizarPrestamo(int id){
    Prestamo pObj;
    Libro lObj;
    char opc;
    int pos,pos2;
    pos=buscarPrestamo(id);

    if(pos==-1){
        cout<<"NO SE ENCONTRO UN PRESTAMO CON ESE ID";
        cout<<endl;
        return;
    }
    pos2=buscarLibroID(pObj.getIdLibrosPrestados());
    pObj.leerDeDisco(pos);
    if(!pObj.getEstado()){
        cout<<"ESE PRESTAMO NO ESTA EN CURSO"<<endl;
        return;
    }
    lObj.leerDeDisco(pos2);
    do{
        pObj.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere dar de baja el prestamo? S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            pObj.setEstado(false);
            if(pObj.modificarEnDisco(pos)){
                cout<<"El prestamo ha sido dado de baja."<<endl;
            }
            else{
                cout<<"No se pudo dar de baja."<<endl;
            }
            lObj.setCant(lObj.getCant()+1);
            lObj.modificarEnDisco(pos2);
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
}


///AUTONUMÉRICO
int prestamoUltimaID(){
    Prestamo aux;
    int maximo=0;
    int pos=0;
    while(aux.leerDeDisco(pos++)){
        if(aux.getIdPrestamo()>maximo){
            maximo=aux.getIdPrestamo();
        }
    }
    return maximo;
}

///COMPROBAR SI LA FECHA DE DEVOLUCION ESTÁ ATRASADO SEGUN EL ID DEL SOCIO:

bool comprobacionFechaAtraso(int idSocio){
Prestamo prestamoAux;
int pos=0;
while(prestamoAux.leerDeDisco(pos++)){
    if(prestamoAux.getidSocio()==idSocio){
        if(prestamoAux.getEstado() && prestamoAux.getFechaDevolucion()<hoy()){
            return false;
        }
    }
}

return true;
}
///MUESTRA TODOS LOS PRESTAMOS
void mostrarArchivoPrestamos(){
    Prestamo reg;
    int pos=0;
    while(reg.leerDeDisco(pos++)){
            reg.mostrar();
    }
}

void modificarSocioPrestamo(int id){
    Prestamo reg;
    char opc;
    int pos=0,aux;

    pos=buscarPrestamo(id);
    if(pos==-1){
        cout<<"No existe prestamo con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    if(!reg.getEstado()){
        cout<<"NO SE PUEDEN MODIFICAR AQUELLOS PRESTAMOS DEVUELTOS"<<endl;
        return;
    }
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar el prestamo?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO ID SOCIO: ";
            cin>>aux;
            reg.setidSocio(aux);
            if(!comprobacionCuotaAtraso(aux) || !comprobacionFechaAtraso(aux)){
                cout<<"ESTE SOCIO NO CUMPLE CON LAS CONDICIONES PARA SOLICITAR UN PRESTAMO"<<endl;
                return;
            }
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El prestamo ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el prestamo."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void modificarFechaPrestamo(int id){
    Prestamo reg;
    Fecha aux;
    char opc;
    int pos=0;

    pos=buscarPrestamo(id);
    if(pos==-1){
        cout<<"No existe prestamo con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    if(!reg.getEstado()){
        cout<<"NO SE PUEDEN MODIFICAR AQUELLOS PRESTAMOS DEVUELTOS"<<endl;
        return;
    }
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar el prestamo?"<<" S/N: ";
        cin>>opc;
        cout<<endl;
        if(opc=='s' || opc=='S'){
            cout<<"INGRESE LA NUEVA FECHA: ";
            aux.cargar();
            reg.setFechaPrestamo(aux);
            reg.setFechaDevolucion(aux+30);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El prestamo ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el prestamo."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}

void modificarLibroPrestamo(int id){
    Libro obj1;
    Prestamo reg;
    char opc;
    int pos=0,pos2=0,aux;

    pos=buscarPrestamo(id);
    if(pos==-1){
        cout<<"No existe prestamo con la ID ingresada"<<endl;
        return;
    }
    reg.leerDeDisco(pos);
    if(!reg.getEstado()){
        cout<<"NO SE PUEDEN MODIFICAR AQUELLOS PRESTAMOS DEVUELTOS"<<endl;
        return;
    }
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar el prestamo?"<<" S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<endl;
            cout<<"INGRESE EL NUEVO ID LIBRO: ";
            cin>>aux;
            cout<<endl;
            if(retornarLibro(aux).getCant()<0 || buscarIdLibro(aux)==-1){
                cout<<"EL LIBRO NO EXISTE O NO HAY EXISTENCIAS"<<endl;
                return;
            }
            pos2=buscarIdLibro(reg.getIdLibrosPrestados());
            obj1.leerDeDisco(pos2);
            obj1.setCant(obj1.getCant()+1);
            obj1.modificarEnDisco(pos2);

            reg.setIdLibrosPrestados(aux);
            cout<<endl;
            if(reg.modificarEnDisco(pos)){
                cout<<"El prestamo ha sido modificado."<<endl;
            }
            else{
                cout<<"No se pudo modificar el prestamo."<<endl;
            }
            pos2=buscarIdLibro(aux);
            obj1.leerDeDisco(pos2);
            obj1.setCant(obj1.getCant()-1);
            obj1.modificarEnDisco(pos2);
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');

    return;
}


void buscarPrestamoPorFecha(){
    Prestamo obj;
    Fecha dt2,dt1;
    int opc,pos=0;
    while(true){
        cout<<"MOSTRAR POR FECHA"<<endl;
        cout<<"--------------------------"<<endl;
        cout<<"1. BUSCAR FECHA"<<endl;
        cout<<"2. BUSCAR ENTRE DOS FECHAS"<<endl;
        cout<<"--------------------------"<<endl;
        cout<<"0. VOLVER"<<endl<<endl;

        cout<<"SELECCIONE UNA OPCION: "<<endl;
        cin>>opc;
        system("cls");
        switch(opc){
            case 1:
                cout<<"INGRESE LA FECHA QUE DESEA BUSCAR"<<endl;
                dt1.cargar();
                system("cls");
                while(obj.leerDeDisco(pos++)){
                    if(dt1==obj.getFechaPrestamo()){
                        obj.mostrar();
                    }
                }
                break;
            case 2:
                cout<<"INGRESE LA PRIMER FECHA"<<endl;
                dt1.cargar();
                cout<<"INGRESE LA SEGUNDA FECHA"<<endl;
                dt2.cargar();
                system("cls");
                while(obj.leerDeDisco(pos++)){
                    if(obj.getFechaPrestamo()>dt1 && obj.getFechaPrestamo()<dt2){
                        obj.mostrar();
                    }
                }
                break;
            case 0: return;
        }
        system("pause>nul");
        system("cls");
    }
}
void menuPrestamos(){
    int opc,id;
    Prestamo aux;
    while(true){
        system("cls");
        cout<<"MENU PRESTAMOS"<<endl;
        cout<<"<--------------------------------->"<<endl;
        cout<<"1. ALTA"<<endl;
        cout<<"2. FINALIZAR PRESTAMO"<<endl;
        cout<<"3. CONSULTAS"<<endl;
        cout<<"4. MODIFICACIONES"<<endl;
        cout<<"<--------------------------------->"<<endl;
        cout<<"0. VOLVER"<<endl;
        cout<<"SELECCIONE UNA OPCION: ";
        cin>>opc;
        system("cls");
        switch(opc){
            case 1:
                agregarPrestamo();
                system("pause>nul");
                break;
            case 2:
                cout<<"INGRESE LA ID DEL PRESTAMO A FINALIZAR: ";
                cin>>id;
                cout<<endl;
                finalizarPrestamo(id);
                system("pause>nul");
                break;
            case 3:
                subMenuPrestamosConsultas();
                break;
            case 4:
                subMenuPrestamosModificaciones();
                break;
            case 0: return;
                break;
            default:cout<<"OPCION INVALIDA";
                break;
        }
        system("cls");
    }
}

void subMenuPrestamosConsultas(){
    int opc;
    while(true){
    cout<<"MENU CONSULTAS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MOSTRAR HISTORIAL"<<endl;
    cout<<"2. MOSTRAR EN CURSO"<<endl;
    cout<<"3. MOSTRAR DEMORADOS"<<endl;
    cout<<"4. MOSTRAR DEVUELTOS"<<endl;
    cout<<"5. BUSCAR POR FECHA"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            mostrarArchivoPrestamos();
            system("pause>nul");
            break;
        case 2:
            mostrarPrestamosEnCurso();
            system("pause>nul");
            break;
        case 3:
            mostrarDemorados();
            system("pause>nul");
            break;
        case 4:
            mostrarPrestamosDevueltos();
            system("pause>nul");
            break;
        case 5:
            buscarPrestamoPorFecha();
            break;
        case 0: return;
    }
	system("cls");
    }
}

void subMenuPrestamosModificaciones(){
    int opc,id;
    while(true){
    cout<<"MENU MODIFICACIONES"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MODIFICAR SOCIO"<<endl;
    cout<<"2. MODIFICAR FECHA DE DEVOLUCION"<<endl;
    cout<<"3. MODIFICAR ID LIBRO"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"NOTA: SOLO SE PUEDEN MODIFICAR AQUELLOS PRESTAMOS QUE NO HAYAN FINALIZADO."<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            cout<<"INGRESE ID DEL PRESTAMO A MODIFICAR SOCIO: ";
            cin>>id;
            cout<<endl;
            modificarSocioPrestamo(id);
            break;
        case 2:
            cout<<"INGRESE ID DEL PRESTAMO A MODIFICAR FECHA DE DEVOLUCION: ";
            cin>>id;
            cout<<endl;
            modificarFechaPrestamo(id);
            break;
        case 3:
            cout<<"INGRESE ID DEL PRESTAMO A MODIFICAR EL LIBRO: ";
            cin>>id;
            cout<<endl;
            modificarLibroPrestamo(id);
            break;
        case 0: return;
            break;
    }
    system("pause>nul");
    system("cls");
    }
}

#endif // PRESTAMOS_H_INCLUDED
