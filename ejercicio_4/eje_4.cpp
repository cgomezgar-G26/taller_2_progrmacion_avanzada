#include <iostream>
#include <cmath>
#include <initializer_list> // Para la función acumula
using namespace std;

// ==================================================
// Clase para números complejos en forma binomial (a + ib)
// ==================================================
class Complejo {
private:
    double parteReal;  // Parte real
    double parteImag;  // Parte imaginaria

public:
    // -------------------------------
    // Constructor flexible
    // -------------------------------
    Complejo(double real = 0, double imag = 0) {
        parteReal = real;
        parteImag = imag;
    }

    // -------------------------------
    // Mostrar el número complejo
    // -------------------------------
    void mostrar() const {
        cout << parteReal << " + " << parteImag << "i";
    }

    // -------------------------------
    // Obtener el conjugado
    // -------------------------------
    Complejo conjugar() const {
        return Complejo(parteReal, -parteImag);
    }

    // -------------------------------
    // Suma de dos complejos
    // -------------------------------
    Complejo operator+(const Complejo& otro) const {
        return Complejo(parteReal + otro.parteReal, parteImag + otro.parteImag);
    }

    // -------------------------------
    // Resta de dos complejos
    // -------------------------------
    Complejo operator-(const Complejo& otro) const {
        return Complejo(parteReal - otro.parteReal, parteImag - otro.parteImag);
    }

    // -------------------------------
    // Multiplicación de dos complejos
    // -------------------------------
    Complejo operator*(const Complejo& otro) const {
        double real = parteReal * otro.parteReal - parteImag * otro.parteImag;
        double imag = parteReal * otro.parteImag + parteImag * otro.parteReal;
        return Complejo(real, imag);
    }

    // -------------------------------
    // División de dos complejos
    // -------------------------------
    Complejo operator/(const Complejo& otro) const {
        double denominador = otro.parteReal * otro.parteReal + otro.parteImag * otro.parteImag;
        double real = (parteReal * otro.parteReal + parteImag * otro.parteImag) / denominador;
        double imag = (parteImag * otro.parteReal - parteReal * otro.parteImag) / denominador;
        return Complejo(real, imag);
    }

    // -------------------------------
    // Funciones para conversión a forma polar
    // -------------------------------
    double getMagnitud() const {
        return sqrt(parteReal * parteReal + parteImag * parteImag);
    }

    double getAngulo() const {
        return atan2(parteImag, parteReal);
    }

    // Accesores
    double getReal() const { return parteReal; }
    double getImag() const { return parteImag; }
};

// ==================================================
// Clase para números complejos en forma polar (r·e^{iθ})
// ==================================================
class Polar {
private:
    double magnitud;  // Módulo del número
    double angulo;    // Argumento (en radianes)

public:
    // -------------------------------
    // Constructor flexible
    // -------------------------------
    Polar(double r = 0, double theta = 0) {
        magnitud = r;
        angulo = theta;
    }

    // -------------------------------
    // Mostrar el número polar
    // -------------------------------
    void mostrar() const {
        cout << magnitud << "e^(" << angulo << "i)";
    }

    // -------------------------------
    // Convertir a forma binomial
    // -------------------------------
    Complejo convertirABinomial() const {
        return Complejo(magnitud * cos(angulo), magnitud * sin(angulo));
    }

    // -------------------------------
    // Crear un número polar desde un binomial
    // -------------------------------
    static Polar desdeBinomial(const Complejo& c) {
        return Polar(c.getMagnitud(), c.getAngulo());
    }

    // -------------------------------
    // Operaciones básicas en forma polar
    // -------------------------------
    Polar operator*(const Polar& otro) const {
        return Polar(magnitud * otro.magnitud, angulo + otro.angulo);
    }

    Polar operator/(const Polar& otro) const {
        return Polar(magnitud / otro.magnitud, angulo - otro.angulo);
    }

    // Conjugado (inversión del ángulo)
    Polar conjugar() const {
        return Polar(magnitud, -angulo);
    }

    // Accesores
    double getMagnitud() const { return magnitud; }
    double getAngulo() const { return angulo; }
};

// ==================================================
// Función para acumular varios números (complejos o polares)
// ==================================================
Complejo acumular(std::initializer_list<Complejo> lista) {
    Complejo sumaTotal(0, 0);
    for (auto& elem : lista) {
        sumaTotal = sumaTotal + elem;
    }
    return sumaTotal;
}

// ==================================================
// Función principal
// ==================================================
int main() {
    system("chcp 65001 > nul");  // Activa UTF-8 en consola (Windows)

    // Creación de números complejos
    Complejo comp1(3, 4);
    Complejo comp2(1, -2);

    // Mostrar los valores iniciales
    cout << "Número complejo 1: ";
    comp1.mostrar();
    cout << endl;

    cout << "Número complejo 2: ";
    comp2.mostrar();
    cout << endl << endl;

    // Suma
    cout << "→ Suma de los dos complejos: ";
    (comp1 + comp2).mostrar();
    cout << endl;

    // Resta
    cout << "→ Resta de los dos complejos: ";
    (comp1 - comp2).mostrar();
    cout << endl;

    // Multiplicación
    cout << "→ Producto de los dos complejos: ";
    (comp1 * comp2).mostrar();
    cout << endl;

    // División
    cout << "→ División de los dos complejos: ";
    (comp1 / comp2).mostrar();
    cout << endl;

    // Conjugado
    cout << "→ Conjugado del primer complejo: ";
    comp1.conjugar().mostrar();
    cout << endl << endl;

    // Conversión a forma polar
    Polar pol1 = Polar::desdeBinomial(comp1);
    cout << "Forma polar del primer complejo: ";
    pol1.mostrar();
    cout << endl;

    // Conversión de vuelta a binomial
    cout << "Forma binomial convertida de la polar: ";
    pol1.convertirABinomial().mostrar();
    cout << endl << endl;

    // Ejemplo de acumulación
    cout << "→ Suma acumulada de varios complejos: ";
    Complejo total = acumular({comp1, comp2, Complejo(2, 1), Complejo(-1, 3)});
    total.mostrar();
    cout << endl;
    cout << endl;
    return 0;
}
