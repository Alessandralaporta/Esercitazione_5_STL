#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"
#include <Eigen>

using namespace std;

namespace MeshLibrary {
bool importCell0Ds(const string& fileName, PolygonalMesh& mesh);

bool importCell1Ds(const string& fileName, PolygonalMesh& mesh, double tol1D);

bool importCell2Ds(const string& fileName, PolygonalMesh& mesh, double tol2D);

bool importMesh(const string& path, PolygonalMesh& mesh, double tol1D, double tol2D);

void testLunghezzaEdges(PolygonalMesh& mesh,  double tol1D);
void testAreaPoligono(PolygonalMesh& mesh, double tol2D);
}

double crossProduct(const Vector2d& v1, const Vector2d& v2 );
double setTol1D();
double setTol2D(const double tol1D);
