#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

class Fecha;

Fecha hoy();

class Fecha{
    private:
        int dia,mes,anio;
    public:
        Fecha(int d=0, int m=0, int a=0){
            dia=d;
            mes=m;
            anio=a;
        }
        ///Funciones
        Fecha operator+(int dias);
        Fecha operator-(int dias);
        bool operator <(Fecha obj);
        bool operator >(Fecha obj);
        bool operator ==(Fecha obj);
        void cargar();
        void mostrar();
        ///Sets
        void setDia(int d){dia=d;}
        void setMes(int m){mes=m;}
        void setAnio(int a){anio=a;}
        ///Gets
        int getDia(){return dia;}
        int getMes(){return mes;}
        int getAnio(){return anio;}
};
bool validarFecha(int dia, int mes, int anio);

void Fecha::cargar(){
    int d,m,a;
    while(true){
        cout<<"INSERTE DIA: ";
        cin>>d;
        cout<<"INSERTE MES: ";
        cin>>m;
        cout<<"INSERTE ANIO: ";
        cin>>a;
        if(validarFecha(d,m,a)){
            setDia(d);
            setMes(m);
            setAnio(a);
            return;
        }
        else{
            cout<<"FECHA INVALIDA, INGRESE OTRA."<<endl;

        }
    }
    }

bool Fecha::operator <(Fecha obj){
        if(anio<obj.anio)return true;
        else if(anio>obj.anio)return false;
                ///ANALIZA EL MES
        else if(anio==obj.anio){
            if(mes<obj.mes){return true;}
            else if(mes>obj.mes){return false;}
                ///ANALIZA EL DÍA
            else if(mes==obj.mes){
                if(dia<obj.dia){return true;}
                else if(dia>obj.dia){return false;}
                else if(dia==obj.dia){return false;}
            }
        }
    }

bool Fecha::operator >(Fecha obj){
        if(anio>obj.anio)return true;
        else if(anio<obj.anio)return false;
                ///ANALIZA EL MES
        else if(anio==obj.anio){
            if(mes>obj.mes){return true;}
            else if(mes<obj.mes){return false;}
                ///ANALIZA EL DÍA
            else if(mes==obj.mes){
                if(dia>obj.dia){return true;}
                else if(dia<obj.dia){return false;}
                else if(dia==obj.dia){return false;}
            }
        }
    }

bool Fecha::operator ==(Fecha obj){
        if(anio==obj.anio && mes==obj.mes && dia==obj.dia)return true;
        else {return false;}
    }


Fecha Fecha::operator+(int dias){
    Fecha obj(dia,mes,anio);
    int diasPorMes[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(dia+dias>diasPorMes[mes-1]){
        obj.setDia(dia+dias-diasPorMes[mes-1]);

        if(mes==12){
            obj.setMes(1);
            obj.setAnio(anio+1);
            }
        else{obj.setMes(mes+1);}
    }
    else{
      obj.setDia(dia+dias);
    }

    return obj;
}


void Fecha::mostrar(){
    cout<<dia<<"/"<<mes<<"/"<<anio<<endl;
}

bool validarFecha(int dia,int mes,int anio){
    int diasPorMes[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(dia>0 && dia<diasPorMes[mes-1] && mes>0 && mes<=12 && anio>0 && anio<=hoy().getAnio()){
            return true;}
    else {return false;}
}

Fecha hoy(){
int anio,mes,dia;
Fecha hoy;
time_t tiempo;
struct tm *tmPtr;
tiempo = time(NULL);
tmPtr = localtime(&tiempo);
anio=tmPtr->tm_year+1900;
mes=tmPtr->tm_mon+1;
dia=tmPtr->tm_mday;

hoy.setAnio(anio);
hoy.setMes(mes);
hoy.setDia(dia);

return hoy;
}


#endif // FECHA_H_INCLUDED
