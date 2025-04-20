#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace std;
using namespace MeshLibrary;
int main()
{
    PolygonalMesh mesh;
    double tol1D = setTol1D();
    double tol2D = setTol2D(tol1D);
    string path = "./PolygonalMesh";
    if(!importMesh(path, mesh,tol1D,tol2D)){
        return 1;
    }
    testLunghezzaEdges(mesh,tol1D);
    testAreaPoligono(mesh,tol2D);
    return 0;
}
