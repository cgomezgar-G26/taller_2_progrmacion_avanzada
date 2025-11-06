#include <iostream>   // Para manejo de entrada/salida en consola
#include <cmath>      // Para la función pow() que eleva a potencia
#include <iomanip>    // Para mostrar números con formato fijo
using namespace std;

// =====================================================
// Clase Polinomio: representa un polinomio de grado n
// =====================================================
class Polinomio {
private:
    double* terminos;   // Arreglo dinámico para almacenar los coeficientes
    int gradoMax;       // Grado máximo del polinomio

public:
    // -------------------------------------------------
    // Constructor: inicializa el grado y los coeficientes
    // -------------------------------------------------
    Polinomio(int grado, const double* coeficientes) {
        gradoMax = grado;                  // Guarda el grado
        terminos = new double[gradoMax + 1]; // Reserva memoria dinámica para los coeficientes

        // Copia los coeficientes al arreglo interno
        for (int i = 0; i <= gradoMax; i++) {
            terminos[i] = coeficientes[i];
        }
    }

    // -------------------------------------------------
    // Método para evaluar el polinomio en un valor de x
    // -------------------------------------------------
    double evaluar(double valorX) const {
        double resultado = 0;  // Acumula el valor del polinomio
        for (int i = 0; i <= gradoMax; i++) {
            // Cada término: coef * (x^i)
            resultado += terminos[i] * pow(valorX, i);
        }
        return resultado;  // Devuelve el resultado final
    }

    // -------------------------------------------------
    // Sobrecarga del operador + : suma de polinomios
    // -------------------------------------------------
    Polinomio operator+(const Polinomio& otro) const {
        // El nuevo grado es el mayor entre ambos polinomios
        int nuevoGrado = max(gradoMax, otro.gradoMax);
        double* nuevosTerminos = new double[nuevoGrado + 1];

        // Suma término a término
        for (int i = 0; i <= nuevoGrado; i++) {
            double a = (i <= gradoMax) ? terminos[i] : 0;
            double b = (i <= otro.gradoMax) ? otro.terminos[i] : 0;
            nuevosTerminos[i] = a + b;
        }

        // Crea un nuevo polinomio con la suma
        Polinomio resultado(nuevoGrado, nuevosTerminos);
        delete[] nuevosTerminos; // Libera el arreglo temporal
        return resultado;
    }

    // -------------------------------------------------
    // Sobrecarga del operador * : multiplicación de polinomios
    // -------------------------------------------------
    Polinomio operator*(const Polinomio& otro) const {
        // El grado del producto es la suma de los grados
        int nuevoGrado = gradoMax + otro.gradoMax;
        double* nuevosTerminos = new double[nuevoGrado + 1];

        // Inicializa los nuevos coeficientes en 0
        for (int i = 0; i <= nuevoGrado; i++) {
            nuevosTerminos[i] = 0;
        }

        // Multiplica término a término
        for (int i = 0; i <= gradoMax; i++) {
            for (int j = 0; j <= otro.gradoMax; j++) {
                nuevosTerminos[i + j] += terminos[i] * otro.terminos[j];
            }
        }

        // Devuelve el nuevo polinomio multiplicado
        Polinomio producto(nuevoGrado, nuevosTerminos);
        delete[] nuevosTerminos; // Libera memoria temporal
        return producto;
    }

    // -------------------------------------------------
    // Método para derivar el polinomio
    // Devuelve un nuevo objeto Polinomio con la derivada
    // -------------------------------------------------
    Polinomio derivar() const {
        // Si el polinomio es constante, su derivada es 0
        if (gradoMax == 0) {
            double c[1] = {0};
            return Polinomio(0, c);
        }

        // Crea arreglo para los nuevos coeficientes
        double* nuevosTerminos = new double[gradoMax];
        for (int i = 1; i <= gradoMax; i++) {
            nuevosTerminos[i - 1] = terminos[i] * i; // Derivada: coef * exponente
        }

        // Crea un nuevo polinomio derivado
        Polinomio derivado(gradoMax - 1, nuevosTerminos);
        delete[] nuevosTerminos; // Libera memoria temporal
        return derivado;
    }

    // -------------------------------------------------
    // Muestra el polinomio en formato legible
    // -------------------------------------------------
    void mostrar() const {
        for (int i = gradoMax; i >= 0; i--) {
            cout << fixed << setprecision(2) << terminos[i]; // Muestra coeficiente con 2 decimales
            if (i > 0) cout << "x^" << i << " ";             // Añade potencia si corresponde
            if (i > 0 && terminos[i - 1] >= 0) cout << "+ "; // Añade signo positivo si el siguiente es positivo
        }
        cout << endl;
    }

    // -------------------------------------------------
    // Destructor: libera la memoria dinámica usada
    // -------------------------------------------------
    ~Polinomio() {
        delete[] terminos; // Evita fugas de memoria
    }
};

// =====================================================
// Programa principal con mensajes explicativos en consola
// =====================================================
int main() {
    // -------------------------------------------------
    // Se crean dos polinomios de ejemplo
    // A(x) = 1 + 2x + 3x²
    // B(x) = 2 + 1x
    // -------------------------------------------------
    double coefA[] = {1, 2, 3};
    Polinomio A(2, coefA);

    double coefB[] = {2, 1};
    Polinomio B(1, coefB);

    // -------------------------------------------------
    // Se muestran los polinomios originales
    // -------------------------------------------------
    cout << "→ Polinomio A(x): ";
    A.mostrar();

    cout << "→ Polinomio B(x): ";
    B.mostrar();

    // -------------------------------------------------
    // Operación: suma de polinomios
    // -------------------------------------------------
    cout << "\n🧮 Operación: SUMA de A(x) + B(x)" << endl;
    Polinomio suma = A + B;
    cout << "Resultado de A(x) + B(x): ";
    suma.mostrar();

    // -------------------------------------------------
    // Operación: multiplicación de polinomios
    // -------------------------------------------------
    cout << "\n🧮 Operación: MULTIPLICACIÓN de A(x) * B(x)" << endl;
    Polinomio mult = A * B;
    cout << "Resultado de A(x) * B(x): ";
    mult.mostrar();

    // -------------------------------------------------
    // Evaluar el polinomio A(x) en un valor de x dado
    // -------------------------------------------------
    double x = 2.0;  // Valor donde se evaluará el polinomio
    cout << "\n🔢 Evaluando A(x) en x = " << x << " ..." << endl;
    cout << "A(" << x << ") = " << A.evaluar(x) << endl;

    // -------------------------------------------------
    // Calcular la derivada del polinomio A(x)
    // -------------------------------------------------
    cout << "\n📈 Calculando la derivada de A(x) ..." << endl;
    Polinomio deriv = A.derivar();
    cout << "Derivada de A(x): ";
    deriv.mostrar();

    // -------------------------------------------------
    // Fin del programa
    // -------------------------------------------------
    cout << "\n✅ Fin del programa. Memoria liberada correctamente.\n";

    return 0;
}
