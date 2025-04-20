#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <sstream>
#include <fstream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;


namespace MeshLibrary {
bool importMesh(const string& path, PolygonalMesh& mesh,double tol1D,double tol2D){
    if(!importCell0Ds(path + "./Cell0Ds.csv",mesh)){
        return false;
    }
    else{ //TEST INSERIMENTO MARKER
        for(auto& el : mesh.VerticiMarker){// per ogni elemento (chiave,valore) della mappa dei Marker dei vertici
            cout<<"key:" <<el.first<< endl; // .first() restituisce la chiave
            for(auto& id : el.second ){     // . second() restituisve il valore che in questo caso è una lista
                cout<<id<<" ";              // sul quale posso iterare
            }
            cout<<endl;
        }
    }

    if(!importCell1Ds(path + "./Cell1Ds.csv",mesh, tol1D)){
        return false;
    }
    else{//TEST INSERIMENTO MARKER
        for(auto& el : mesh.EdgesMarker){// per ogni elemento (chiave,valore) della mappa dei Marker dei vertici
            cout<<"key:" <<el.first<< endl; // .first() restituisce la chiave
            for(auto& id : el.second ){     // . second() restituisve il valore che in questo caso è una lista
                cout<<id<<" ";              // sul quale posso iterare
            }
            cout<<endl;
        }
    }


    if(!importCell2Ds(path + "./Cell2Ds.csv",mesh, tol2D)){
        return false;
    }
    else{
        for(unsigned int i = 0; i< mesh.NumeroCell2Ds;i++){
            cout <<"Id: "<< mesh.IdCell2Ds[i] <<" Numero vertici e lati: "<< (mesh.VerticiCell2Ds[i]).size() <<" "<< (mesh.EdgesCell2Ds[i]).size()<<endl;
        }
    }
    return true;
}


bool importCell0Ds(const string& fileName, PolygonalMesh& mesh){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close(); // Ho finito di leggere posso chiudere

    mesh.NumeroCell0Ds = lines.size(); // Il num di righe del file Cell0Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.CoordinateCell0Ds.reserve(mesh.NumeroCell0Ds);
    mesh.IdCell0Ds.reserve(mesh.NumeroCell0Ds);
    mesh.MarkerCell0Ds.reserve(mesh.NumeroCell0Ds); //Il vettore ha ancora dimensione nulla ma non la capacita
        // Viene riservato sufficiente spazio per poter inserire gli elementi
        // Non usare resize
    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line); // converte la linea
        char c;
        unsigned int id;
        unsigned int marker;
        Vector2d coordinate;
        convert >> id >>c>> marker >>c>> coordinate(0) >>c>> coordinate(1);

        // Qui ho dei vettori quindi aggiungo in coda i nuovi elementi che leggo
        mesh.IdCell0Ds.push_back(id);
        mesh.MarkerCell0Ds.push_back(marker); // così ho in posizione i il marker del nodo con id in posizione i
        mesh.CoordinateCell0Ds.push_back(coordinate);

        // Ora aggiorno la mappa (marker,lista_dei_vertici_con_quel_marker)
        // Devo controllare se la chiave esiste già
        // Se esiste, aggiungo un elemento alla lista, altrimenti deve creare una chiave
        if(marker != 0){//Non ho interesse a memorizzare i vertici interni
            auto ret = mesh.VerticiMarker.insert({marker,{id}}); // {id} crea una lista che contiene id
            // restituisce una coppia iteratore,booleano
            // dove iteratore punta alla chiave e booleano mi dice se l'ha trovata o meno
            // se ritorna true, vuol dire che la chiave non esisteva ed è stata inserita correttamente
            // se ritorna false vuol dire che non l'ha inserito perchè già esisteva
            if(!ret.second){// se non l'ha inserito lo devo inserire io ad uno esistente
                mesh.VerticiMarker[marker].push_back(id);
                // accede all'elemento della mappa con chiave [marker] e aggiunge alla lista corrispondente un elemento
            }
        }
    }
    return true;
}

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh,double tol1D){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close();
    mesh.NumeroCell1Ds = lines.size(); // Il num di righe del file Cell1Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.IdCell1Ds.reserve(mesh.NumeroCell1Ds);
    mesh.VerticiCell1Ds.reserve(mesh.NumeroCell1Ds);
    mesh.MarkerCell1Ds.reserve(mesh.NumeroCell1Ds);

    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line);
        char c;
        unsigned int id;
        unsigned int marker;
        array<unsigned int,2> vertici ;
        convert >> id >>c>> marker >> c>>vertici[0] >>c>> vertici[1];

        // Qui ho dei vettori quindi aggiungo in coda i nuovi elementi che leggo
        mesh.IdCell1Ds.push_back(id);
        mesh.MarkerCell1Ds.push_back(marker); // così ho in posizione i il marker del nodo con id in posizione i
        mesh.VerticiCell1Ds.push_back(vertici);
        // Ora aggiorno la mappa (marker,lista_dei_vertici_con_quel_marker)
        // Devo controllare se la chiave esiste già
        // Se esiste, aggiungo un elemento alla lista, altrimenti deve creare una chiave
        if(marker != 0){//Non ho interesse a memorizzare i vertici interni
            auto ret = mesh.EdgesMarker.insert({marker,{id}}); // {id} crea una lista che contiene id
            // restituisce una coppia iteratore,booleano
            // dove iteratore punta alla chiave e booleano mi dice se l'ha trovata o meno
            // se ritorna true, vuol dire che la chiave non esisteva ed è stata inserita correttamente
            // se ritorna false vuol dire che non l'ha inserito perchè già esisteva
            if(!ret.second){// se non l'ha inserito lo devo inserire io ad uno esistente
                mesh.EdgesMarker[marker].push_back(id);
                // accede all'elemento della mappa con chiave [marker] e aggiunge alla lista corrispondente un elemento
            }
        }
    }

    return true;
}

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh,double tol2D){
    ifstream file(fileName);
    if(file.fail()){
        return false;
    }

    string header; // Prima riga
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file,line)){ // si ferma quando ha letto tutto il file e non riesce a fare getline
        lines.push_back(line); // inserisco con push_back alla fine della lista
            // posso dimenticarmi del file e lavorare sulla lista
    }
    file.close();
    mesh.NumeroCell2Ds = lines.size(); // Il num di righe del file Cell1Ds corrisponde al numero di vertici che ho perchè su ogni riga ho un vertice
    mesh.IdCell2Ds.reserve(mesh.NumeroCell2Ds);
    mesh.VerticiCell2Ds.reserve(mesh.NumeroCell2Ds);
    mesh.EdgesCell2Ds.reserve(mesh.NumeroCell2Ds);

    for(const string& line : lines){//per ogni linea nella lista delle linee
        istringstream convert(line);
        char c;
        unsigned int id;
        unsigned int marker;
        unsigned int numVertici;
        unsigned int numEdges;
        vector<unsigned int> vertici ;
        vector<unsigned int> edges;

        convert >> id >>c>> marker >>c>> numVertici >> c;

        mesh.IdCell2Ds.push_back(id);
        vertici.reserve(numVertici);
        for(unsigned int i = 0; i < numVertici; i ++){
            unsigned int idVertice;
            convert >> idVertice >> c;
            vertici.push_back(idVertice);
        }

        convert >> numEdges;
        edges.reserve(numEdges);
        for(unsigned int i = 0; i < numVertici; i ++){
            unsigned idEdge;
            convert >>c>> idEdge ;
            edges.push_back(idEdge);
        }


        mesh.VerticiCell2Ds.push_back(vertici);
        mesh.EdgesCell2Ds.push_back(edges);

    }
    return true;
}


void testLunghezzaEdges(PolygonalMesh& mesh, double tol1D){
    for(size_t i=0; i< mesh.IdCell1Ds.size(); ++i){//sto accedendo direttamente all'oggetto
        unsigned int idBegin = mesh.VerticiCell1Ds[i][0];
        unsigned int idEnd = mesh.VerticiCell1Ds[i][1];
        if((mesh.CoordinateCell0Ds[idBegin]-mesh.CoordinateCell0Ds[idEnd]).norm() < tol1D){
            cout << "ERRORE : l' edge " << mesh.IdCell1Ds[i] << " ha lunghezza zero"<<endl;

        }
    }


}



void testAreaPoligono(PolygonalMesh& mesh, const double tol2D){
    for(unsigned int id =0; id <mesh.NumeroCell2Ds; id++){
        const vector<unsigned int> idVertici = mesh.VerticiCell2Ds[id];
        // CALCOLO AREA
        double Area = 0.0;
        unsigned int n = idVertici.size();
        for(unsigned int i = 0; i<n;i++){
            const Vector2d& p1 = mesh.CoordinateCell0Ds[idVertici[i]];
            const Vector2d& p2= mesh.CoordinateCell0Ds[idVertici[(i+1)%n]];
            Area += (p1[0]*p2[1]- p2[0]*p1[1]);
        }
        Area = abs(Area) /2;
        if(Area < tol2D){
            cout << "ERRORE : il poligono con ID " << id << " ha area zero."<<endl;

        }
    }

}

}


double crossProduct(Vector2d& v1, Vector2d& v2 ){
    return abs(v1[0]*v2[1] - v1[1]*v2[0]);
}
double setTol1D(){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 1D";
    cin >> tolInput;
    return max(tolDefault,tolInput);
}

double setTol2D(const double tol1D){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 2D";
    cin >> tolInput;
    double tol2D = pow(tol1D,2)*sqrt(3)/4;
    double tol = max(tol2D,tolDefault);
    return max(tol,tolInput);
}
