#include <iostream>  // Para usar cout, endl, etc.
#include <cstdlib>   // Para rand() y srand()
#include <ctime>     // Para time()
#include <locale>    // Para mostrar tildes y ñ correctamente
using namespace std;

// ==================================================
// Clase Matriz: representa una matriz cuadrada de tamaño n x n
// ==================================================
class Matriz {
private:
    int** elementos;  // Puntero doble para los valores de la matriz
    int n;            // Tamaño de la matriz

public:
    // ----------------------------------------------
    // Constructor: inicializa con valores aleatorios
    // ----------------------------------------------
    Matriz(int tam) {
        n = tam;
        elementos = new int*[n];  // Reserva memoria para las filas
        for (int i = 0; i < n; i++) {
            elementos[i] = new int[n];  // Reserva memoria para las columnas
        }

        srand(static_cast<unsigned>(time(nullptr)));  // Semilla aleatoria

        // Llena la matriz con números aleatorios del 0 al 9
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                elementos[i][j] = rand() % 10;
            }
        }
    }

    // ----------------------------------------------
    // Calcula la transpuesta de la matriz
    // ----------------------------------------------
    Matriz transpuesta() const {
        Matriz t(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                t.elementos[i][j] = elementos[j][i];
            }
        }
        return t;
    }

    // ----------------------------------------------
    // Multiplica la matriz actual por otra del mismo tamaño
    // ----------------------------------------------
    Matriz multiplicar(const Matriz& otra) const {
        Matriz resultado(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int suma = 0;
                for (int k = 0; k < n; k++) {
                    suma += elementos[i][k] * otra.elementos[k][j];
                }
                resultado.elementos[i][j] = suma;
            }
        }
        return resultado;
    }

    // ----------------------------------------------
    // Sobrecarga del operador () para acceder a elementos
    // ----------------------------------------------
    int& operator()(int fila, int columna) {
        return elementos[fila][columna];
    }

    const int& operator()(int fila, int columna) const {
        return elementos[fila][columna];
    }

    // ----------------------------------------------
    // Destructor: libera la memoria usada
    // ----------------------------------------------
    ~Matriz() {
        for (int i = 0; i < n; i++) {
            delete[] elementos[i];
        }
        delete[] elementos;
    }

    // ----------------------------------------------
    // Devuelve el tamaño de la matriz (para recorrerla fuera de la clase)
    // ----------------------------------------------
    int getTamanio() const { return n; }

    // ----------------------------------------------
    // Devuelve un puntero constante a los elementos (solo lectura)
    // ----------------------------------------------
    const int* operator[](int fila) const { return elementos[fila]; }
};

// ==================================================
// Programa principal
// ==================================================
int main() {
    system("chcp 65001 > nul");       // Configura UTF-8 en la consola
    setlocale(LC_ALL, "es_ES.UTF-8"); // Permite tildes y ñ

    int tam = 3;  // Tamaño de la matriz (3x3)
    Matriz A(tam);
    Matriz B(tam);

    // -------------------------------
    // Imprimir Matriz A
    // -------------------------------
    cout << "Matriz A:\n";
    for (int i = 0; i < A.getTamanio(); i++) {
        for (int j = 0; j < A.getTamanio(); j++) {
            cout << A(i, j) << " ";
        }
        cout << "\n";
    }
    cout << endl;

    // -------------------------------
    // Imprimir Matriz B
    // -------------------------------
    cout << "Matriz B:\n";
    for (int i = 0; i < B.getTamanio(); i++) {
        for (int j = 0; j < B.getTamanio(); j++) {
            cout << B(i, j) << " ";
        }
        cout << "\n";
    }
    cout << endl;

    // -------------------------------
    // Imprimir transpuesta de A
    // -------------------------------
    Matriz T = A.transpuesta();
    cout << "Transpuesta de A:\n";
    for (int i = 0; i < T.getTamanio(); i++) {
        for (int j = 0; j < T.getTamanio(); j++) {
            cout << T(i, j) << " ";
        }
        cout << "\n";
    }
    cout << endl;

    // -------------------------------
    // Imprimir A * B
    // -------------------------------
    Matriz C = A.multiplicar(B);
    cout << "A * B:\n";
    for (int i = 0; i < C.getTamanio(); i++) {
        for (int j = 0; j < C.getTamanio(); j++) {
            cout << C(i, j) << " ";
        }
        cout << "\n";
    }
    cout << endl;

    // -------------------------------
    // Demostración del operador ()
    // -------------------------------
    cout << "Elemento A(1,1) antes: " << A(1, 1) << "\n";
    A(1, 1) = 99;
    cout << "Elemento A(1,1) después: " << A(1, 1) << "\n";
    cout << endl;
    return 0;
}
