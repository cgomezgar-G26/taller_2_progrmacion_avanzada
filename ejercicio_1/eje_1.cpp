#include <iostream>
#include <cmath>   // Para usar sqrt()
#include <locale>   // librería para tildes y caracteres especiales
using namespace std;

// ==================================================
// Clase Vector3D: representa un vector en el espacio 3D
// ==================================================
class Vector3D {
private:
    double coordX, coordY, coordZ; // Componentes del vector

public:
    // Constructor: guarda las coordenadas del vector
    Vector3D(double xInicial, double yInicial, double zInicial)
        : coordX(xInicial), coordY(yInicial), coordZ(zInicial) {}

    // Devuelve la magnitud o tamaño del vector
    double magnitud() const {
        return sqrt(coordX * coordX + coordY * coordY + coordZ * coordZ);
    }

    // Normaliza el vector (lo convierte en uno unitario)
    void normalizar() {
        double magn = magnitud(); // Calcula la magnitud
        if (magn != 0) {          // Evita dividir por cero
            coordX /= magn;
            coordY /= magn;
            coordZ /= magn;
        } else {
            cout << "No se puede normalizar un vector nulo." << endl;
        }
    }

    // Suma dos vectores
    Vector3D operator+(const Vector3D& otroVector) const {
        return Vector3D(coordX + otroVector.coordX,
                        coordY + otroVector.coordY,
                        coordZ + otroVector.coordZ);
    }

    // Resta dos vectores
    Vector3D operator-(const Vector3D& otroVector) const {
        return Vector3D(coordX - otroVector.coordX,
                        coordY - otroVector.coordY,
                        coordZ - otroVector.coordZ);
    }

    // Producto escalar (devuelve un número)
    double operator*(const Vector3D& otroVector) const {
        return (coordX * otroVector.coordX +
                coordY * otroVector.coordY +
                coordZ * otroVector.coordZ);
    }

    // Permite imprimir el vector con cout << vector
    friend ostream& operator<<(ostream& salida, const Vector3D& vector) {
        salida << "(" << vector.coordX << ", " 
                << vector.coordY << ", " 
                << vector.coordZ << ")";
        return salida;
    }
};

// ==================================================
// Programa principal para probar la clase
// ==================================================
int main() {

    setlocale(LC_ALL, "Spanish");
    // Crear dos vectores
    Vector3D vectorA(3, 4, 0);
    Vector3D vectorB(1, 2, 3);

    // Mostrar los vectores
    cout << "Vector A: " << vectorA << endl;
    cout << "Vector B: " << vectorB << endl;

    // Hacer operaciones básicas
    Vector3D vectorSuma = vectorA + vectorB;
    Vector3D vectorResta = vectorA - vectorB;
    double productoEscalar = vectorA * vectorB;

    // Mostrar resultados
    cout << "\nSuma: " << vectorSuma << endl;
    cout << "Resta: " << vectorResta << endl;
    cout << "Producto escalar: " << productoEscalar << endl;

    // Mostrar magnitud y vector normalizado (sin línea en blanco previa)
    cout << "Magnitud de A: " << vectorA.magnitud() << endl;
    vectorA.normalizar();
    cout << "Vector A normalizado: " << vectorA << endl << endl; // ← línea en blanco final
    cout << endl;
    return 0;
}
