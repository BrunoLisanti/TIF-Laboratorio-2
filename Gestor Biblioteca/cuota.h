#ifndef CUOTA_H_INCLUDED
#define CUOTA_H_INCLUDED

class Cuota;

void mostrarArchivoCuotas();
void agregarCuota();
void darDeBajaCuota(int id);
int cuotaUltimaID();
int buscarIDCuota(int id);
bool comprobacionCuotaAtraso(int idSocio);

void buscarCuotaPorFecha();
void mostrarCuotasPorSocio(int id);
Cuota retornarCuota(int id);

void modificarSocioCuota(int id);
void modificarFechaCuota(int id);
void modificarMontoCuota(int id);

void menuCuotas();
void subMenuCuotasConsultas();
void submenuCuotasModificaciones();

class Cuota{
private:
	int idSocio;
	int idCuota;
	Fecha fechaCuota;
	int importe;
	bool estado;
public:
    ///gets
    int getIdSocio(){return idSocio;}
    int getIdCuota(){return idCuota;}
    Fecha getFechacuota(){return fechaCuota;}
    int getImporte(){return importe;}
    bool getEstado(){return estado;}
    ///sets
    void setIdSocio(int id){idSocio=id;}
    void setIdCuota(int id){idCuota=id;}
    void setFechaCuota(Fecha f){fechaCuota=f;}
    void setImporte(int imp){importe=imp;}
    void setEstado(bool e){estado=e;}
    ///metodos
    bool cargar();
    void mostrar();

    bool grabarEnDisco();
    bool leerDeDisco(int pos);
    bool modificarEnDisco(int pos);
};

bool Cuota::cargar(){
    int aux;
    cout<<"INGRESE EL ID DEL SOCIO: ";
    cin>>aux;
    if(buscarIdSocio(aux)==-1){
        cout<<"EL ID DEL SOCIO NO EXISTE, INGRESE OTRO";
        return false;
    }
    cout<<"NOMBRE: "<<retornarSocio(aux).getNombre()<<" "<<retornarSocio(aux).getApellido()<<endl;
    idSocio=aux;
    idCuota=cuotaUltimaID()+1;
    cout<<"ID CUOTA: "<<idCuota<<endl;
    cout<<"INGRESE LA FECHA: "<<endl;
    fechaCuota.cargar();
    cout<<"INGRESE EL IMPORTE: ";
    cin>>importe;
    estado=true;
    return true;

}

void Cuota::mostrar(){
    if(estado){
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
    cout<<"ID CUOTA: "<<idCuota<<endl;
    cout<<"ID SOCIO: "<<idSocio<<endl;
    cout<<"NOMBRE DEL SOCIO: "<<retornarSocio(idSocio).getNombre()<<" "<<retornarSocio(idSocio).getApellido()<<endl;
    cout<<"FECHA: ";
    fechaCuota.mostrar();
    cout<<"IMPORTE: "<<importe<<endl;
    cout<<":::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
    }
}

bool Cuota::grabarEnDisco(){
    bool grabo;
    FILE *p;
    p=fopen("Cuotas.dat","ab");
    if(p==NULL){
        cout<<"No se pudo crear/abrir archivo"<<endl;
        return false;
    }
    grabo=fwrite(this,sizeof *this,1,p);
    fclose(p);
    return grabo;
}

bool Cuota::leerDeDisco(int pos){
    bool leyo;
    FILE *p;
    p=fopen("Cuotas.dat","rb");
    if(p==NULL){
        cout<<"No se pudo abrir archivo"<<endl;
        return false;
    }
    fseek(p,pos * sizeof *this,0);
    leyo=fread(this,sizeof *this,1,p);
    fclose(p);
    return leyo;
}

bool Cuota::modificarEnDisco(int pos){
    bool leyo;
    FILE *p;
    p=fopen("Cuotas.dat","rb+");
    if(p==NULL){
        cout<<"No se pudo abrir archivo"<<endl;
        return false;
    }
    fseek(p,pos * sizeof *this,0);
    leyo=fwrite(this,sizeof *this,1,p);
    fclose(p);
    return leyo;
}

void mostrarArchivoCuotas(){
    int pos=0;
    Cuota reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado())reg.mostrar();
    }
}

void mostrarCuotasPorSocio(int id){
    int pos=0;
    Cuota reg;
    while(reg.leerDeDisco(pos++)){
        if(reg.getEstado() && reg.getIdSocio()==id){
            reg.mostrar();
        }
    }
}



int cuotaUltimaID(){
    Cuota aux;
    int maximo=0;
    int pos=0;
    while(aux.leerDeDisco(pos++)){
        if(aux.getEstado() && aux.getIdCuota()>maximo){
            maximo=aux.getIdCuota();
        }
    }
    return maximo;
}

void agregarCuota(){
    Cuota reg;
    if(reg.cargar()){
        cout<<endl;
        if(reg.grabarEnDisco()){
            cout<<"Registro agregado"<<endl;
        }
        else{
            cout<<"No se pudo agregar el registro"<<endl;
        }
    }
}

void darDeBajaCuota(int id){
    int pos;
    Cuota obj;
    char opc;
    pos=buscarIDCuota(id);
    if(pos==-1){
            cout<<"NO EXISTE CUOTA CON EL ID INGRESADO"<<endl;
            return;
        }
        obj.leerDeDisco(pos);
        do{
            obj.mostrar();
            cout<<endl;
            cout<<"ESTA SEGURO QUE QUIERE BORRAR LA CUOTA? S/N: ";
            cin>>opc;
            if(opc=='s' || opc=='S'){
                obj.setEstado(false);
                if(obj.modificarEnDisco(pos)){
                    cout<<"LA CUOTA HA SIDO ELIMINADA."<<endl;
                }
                else{
                    cout<<"NO SE PUDO ELIMINAR LA CUOTA."<<endl;
                }
            }
            else if(opc=='n' || opc=='N'){return;}
            else{cout<<"OPCION INVALIDA"<<endl;}
        }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

int buscarIDCuota(int id){
Cuota reg;
int pos=0;
while(reg.leerDeDisco(pos)){
        if(reg.getIdCuota()==id && reg.getEstado()){
        return pos;
    }
    pos++;
}
    return -1;
}

Cuota retornarCuota(int id){
    int pos;
    Cuota obj;
    pos=buscarIDCuota(id);
    while(obj.leerDeDisco(pos++)){
        if(obj.getIdCuota()==id){
            return obj;
        }
    }
}

///True=No esta atrasado False=Esta atrasado
bool comprobacionCuotaAtraso(int id){
    Cuota aux;
    int pos=0;
    while(aux.leerDeDisco(pos++)){
        if(aux.getIdSocio()==id){
            if(aux.getFechacuota().getMes()==hoy().getMes()){
                return true;
            }
            if((aux.getFechacuota().getMes()==hoy().getMes()-1) || (hoy().getMes()==1 && aux.getFechacuota().getMes()==12)){
                if(hoy().getDia()<=10){
                    return true;
                }
            }
        }
    }
    return false;
}

void buscarCuotaPorFecha(){
    Cuota obj;
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
                    if(dt1==obj.getFechacuota()){
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
                    if(obj.getFechacuota()>dt1 && obj.getFechacuota()<dt2){
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

void modificarSocioCuota(int id){
    Cuota reg;
    char opc;
    int pos=0,registro=-1,aux;
    while(reg.leerDeDisco(pos)){
        if(id==reg.getIdCuota()){
            registro=pos;
        }
        pos++;
    }
    if(registro==-1){
        cout<<"No existe cuota con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(registro);
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar la cuota? S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<"INGRESE EL NUEVO SOCIO: ";
            cin>>aux;
            if(buscarIdSocio(aux)==-1){
                cout<<"NO EXISTE EL SOCIO"<<endl;
                return;
            }
            reg.setIdSocio(aux);
            if(reg.modificarEnDisco(registro)){
                cout<<"LA CUOTA HA SIDO MODIFICADA."<<endl;
            }
            else{
                cout<<"NO SE PUDO MODIFICAR LA CUOTA."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void modificarFechaCuota(int id){
    Cuota reg;
    char opc;
    int pos=0,registro=-1;
    Fecha aux;
    while(reg.leerDeDisco(pos)){
        if(id==reg.getIdCuota()){
            registro=pos;
        }
        pos++;
    }
    if(registro==-1){
        cout<<"No existe cuota con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(registro);
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar la cuota? S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<"INGRESE LA NUEVA FECHA: "<<endl;
            aux.cargar();
            reg.setFechaCuota(aux);
            if(reg.modificarEnDisco(registro)){
                cout<<"LA CUOTA HA SIDO MODIFICADA."<<endl;
            }
            else{
                cout<<"NO SE PUDO MODIFICAR LA CUOTA."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void modificarMontoCuota(int id){
    Cuota reg;
    char opc;
    int pos=0,registro=-1,aux;
    while(reg.leerDeDisco(pos)){
        if(id==reg.getIdCuota()){
            registro=pos;
        }
        pos++;
    }
    if(registro==-1){
        cout<<"No existe cuota con el id ingresado"<<endl;
        return;
    }
    reg.leerDeDisco(registro);
    do{
        reg.mostrar();
        cout<<endl;
        cout<<"Esta seguro que quiere modificar la cuota? S/N: ";
        cin>>opc;
        if(opc=='s' || opc=='S'){
            cout<<"INGRESE EL NUEVO MONTO: ";
            cin>>aux;
            reg.setImporte(aux);
            if(reg.modificarEnDisco(registro)){
                cout<<"LA CUOTA HA SIDO MODIFICADA."<<endl;
            }
            else{
                cout<<"NO SE PUDO MODIFICAR LA CUOTA."<<endl;
            }
        }
        else if(opc=='n' || opc=='N'){return;}
        else{cout<<"Opcion invalida"<<endl;}
    }while(opc!='s' && opc!='n' && opc!='S' && opc!='N');
    return;
}

void menuCuotas(){
    int opc,id;
    while(true){
        cout<<"MENU CUOTAS"<<endl;
        cout<<"----------------------"<<endl;
        cout<<"1. ALTA"<<endl;
        cout<<"2. BAJA"<<endl;
        cout<<"3. CONSULTAS"<<endl;
        cout<<"4. MODIFICACIONES"<<endl;
        cout<<"----------------------"<<endl<<endl;

        cout<<"0. VOLVER"<<endl<<endl;

        cout<<"SELECCIONE UNA OPCION: ";
        cin>>opc;
        system("cls");
        switch(opc){
            case 1:
                agregarCuota();
                system("pause>nul");
                break;
            case 2:
                cout<<"INGRESE LA ID DE LA CUOTA A BORRAR: ";
                cin>>id;
                darDeBajaCuota(id);
                system("pause>nul");
                break;
            case 3: subMenuCuotasConsultas();
                break;
            case 4: submenuCuotasModificaciones();
                break;
            case 0: return;
                break;
        }
        system("cls");
    }
}

void subMenuCuotasConsultas(){
    int opc,id;
    while(true){
    cout<<"MENU CONSULTAS"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MOSTRAR TODO"<<endl;
    cout<<"2. LISTAR POR SOCIO"<<endl;
    cout<<"3. BUSCAR POR FECHA"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            mostrarArchivoCuotas();
            system("pause>nul");
            break;
        case 2:
            cout<<"INGRESE ID DEL SOCIO A LISTAR CUOTAS: ";
            cin>>id;
            mostrarCuotasPorSocio(id);
            system("pause>nul");
            break;
        case 3:
            buscarCuotaPorFecha();
            break;
        case 0: return;
    }
	system("cls");
    }
}

void submenuCuotasModificaciones(){
    int opc,id;
    while(true){
    cout<<"MENU MODIFICACIONES"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"1. MODIFICAR SOCIO"<<endl;
    cout<<"2. MODIFICAR FECHA"<<endl;
    cout<<"3. MODIFICAR MONTO"<<endl;
    cout<<"<--------------------------------->"<<endl;
    cout<<"0. VOLVER"<<endl<<endl;

    cout<<"SELECCIONE UNA OPCION: ";
    cin>>opc;
    system("cls");
    switch(opc){
        case 1:
            cout<<"INGRESE ID DE LA COUTA A MODIFICAR SOCIO: ";
            cin>>id;
            modificarSocioCuota(id);
            break;
        case 2:
            cout<<"INGRESE ID DE LA CUOTA A MODIFICAR FECHA: ";
            cin>>id;
            modificarFechaCuota(id);
            break;
        case 3:
            cout<<"INGRESE ID DE LA CUOTA A MODIFICAR MONTO: ";
            cin>>id;
            modificarMontoCuota(id);
            break;
        case 0: return;
            break;
    }
    system("cls");
    }
}


#endif // CUOTA_H_INCLUDED
