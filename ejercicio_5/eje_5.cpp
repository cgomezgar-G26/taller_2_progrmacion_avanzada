#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// ===================================================
// Clase CTérmino → representa un elemento individual del polinomio (a*x^n)
// ===================================================
class CTermino {
private:
    double valorCoef;   // Guarda el coeficiente del término
    int gradoExp;       // Guarda el exponente asociado

public:
    // ------------------------------------------------
    // Constructor con valores por defecto (coef=0, exp=0)
    // ------------------------------------------------
    CTermino(double c = 0, int e = 0) {
        valorCoef = c;
        gradoExp = e;
    }

    // ------------------------------------------------
    // Retorna el coeficiente del término
    // ------------------------------------------------
    double leerCoef() const { return valorCoef; }

    // ------------------------------------------------
    // Retorna el exponente del término
    // ------------------------------------------------
    int leerExp() const { return gradoExp; }

    // ------------------------------------------------
    // Convierte el término en texto con formato algebraico (+4x^3)
    // ------------------------------------------------
    string aTexto() const {
        stringstream salidaTxt;

        if (valorCoef == 0)
            return "";

        // Determina el signo que se mostrará
        if (valorCoef > 0)
            salidaTxt << "+";
        else
            salidaTxt << "-";

        // Valor absoluto del coeficiente
        double valorAbs = abs(valorCoef);

        // Muestra el número si no es 1 o si el exponente es 0
        if (valorAbs != 1 || gradoExp == 0)
            salidaTxt << valorAbs;

        // Agrega la parte literal con x y su exponente
        if (gradoExp > 0) {
            salidaTxt << "x";
            if (gradoExp > 1)
                salidaTxt << "^" << gradoExp;
        }

        return salidaTxt.str();
    }

    // ------------------------------------------------
    // Comprueba si dos términos tienen el mismo exponente
    // ------------------------------------------------
    bool compararExp(const CTermino& otro) const {
        return gradoExp == otro.gradoExp;
    }

    // ------------------------------------------------
    // Suma con otro término del mismo grado
    // ------------------------------------------------
    void unir(const CTermino& otro) {
        if (compararExp(otro))
            valorCoef += otro.valorCoef;
    }

    // ------------------------------------------------
    // Multiplica el término actual con otro
    // ------------------------------------------------
    CTermino producto(const CTermino& otro) const {
        return CTermino(valorCoef * otro.valorCoef, gradoExp + otro.gradoExp);
    }
};

// ===================================================
// Clase CPolinomio → agrupa varios términos en un solo objeto
// ===================================================
class CPolinomio {
private:
    vector<CTermino> listaTerminos;  // Contenedor de los términos del polinomio

public:
    // ------------------------------------------------
    // Constructor sin parámetros (polinomio vacío)
    // ------------------------------------------------
    CPolinomio() {}

    // ------------------------------------------------
    // Devuelve cuántos términos tiene el polinomio actual
    // ------------------------------------------------
    int totalTerminos() const {
        return listaTerminos.size();
    }

    // ------------------------------------------------
    // Inserta un nuevo término, acomodándolo en orden creciente de exponente
    // Si el exponente ya existe, suma sus coeficientes
    // ------------------------------------------------
    void insertarTermino(const CTermino& nuevoTermino) {
        if (nuevoTermino.leerCoef() == 0)
            return;

        bool encontrado = false;
        for (auto& t : listaTerminos) {
            if (t.leerExp() == nuevoTermino.leerExp()) {
                t.unir(nuevoTermino);
                encontrado = true;
                break;
            }
        }

        // Si no existía, se agrega
        if (!encontrado)
            listaTerminos.push_back(nuevoTermino);

        organizar();
        depurarCeros();
    }

    // ------------------------------------------------
    // Suma dos polinomios y devuelve el resultado
    // ------------------------------------------------
    CPolinomio sumar(const CPolinomio& otroPoli) const {
        CPolinomio resultado = *this;
        for (const auto& t : otroPoli.listaTerminos)
            resultado.insertarTermino(t);
        return resultado;
    }

    // ------------------------------------------------
    // Multiplica dos polinomios y devuelve el resultado
    // ------------------------------------------------
    CPolinomio multiplicar(const CPolinomio& otroPoli) const {
        CPolinomio resultadoFinal;

        for (const auto& a : listaTerminos) {
            for (const auto& b : otroPoli.listaTerminos) {
                CTermino producto = a.producto(b);
                resultadoFinal.insertarTermino(producto);
            }
        }

        resultadoFinal.organizar();
        resultadoFinal.depurarCeros();
        return resultadoFinal;
    }

    // ------------------------------------------------
    // Convierte el polinomio en texto legible
    // ------------------------------------------------
    string aTexto() const {
        if (listaTerminos.empty())
            return "0";

        stringstream salidaTxt;
        for (const auto& t : listaTerminos)
            salidaTxt << t.aTexto();

        string texto = salidaTxt.str();
        if (texto[0] == '+')
            texto.erase(0, 1); // Elimina el '+' inicial

        // Reemplaza signos para mejorar la lectura visual
        string resultado;
        for (size_t i = 0; i < texto.size(); ++i) {
            if (texto[i] == '+' || texto[i] == '-')
                resultado += " " + string(1, texto[i]) + " ";
            else
                resultado += texto[i];
        }

        return resultado;
    }

    // ------------------------------------------------
    // Ordena los términos del polinomio de menor a mayor exponente
    // ------------------------------------------------
    void organizar() {
        sort(listaTerminos.begin(), listaTerminos.end(),
             [](const CTermino& a, const CTermino& b) {
                 return a.leerExp() < b.leerExp();
             });
    }

    // ------------------------------------------------
    // Borra los términos cuyo coeficiente sea cero
    // ------------------------------------------------
    void depurarCeros() {
        listaTerminos.erase(
            remove_if(listaTerminos.begin(), listaTerminos.end(),
                      [](const CTermino& t) { return t.leerCoef() == 0; }),
            listaTerminos.end());
    }
};

// ===================================================
// Bloque principal de ejecución del programa
// ===================================================
int main() {
    cout << fixed << setprecision(2);

    // ------------------------------------------------
    // Se crean los polinomios del ejemplo inicial
    // ------------------------------------------------
    cout << "=== CREACIÓN DE POLINOMIOS ===" << endl;
    CPolinomio poliUno;
    poliUno.insertarTermino(CTermino(4, 3));
    poliUno.insertarTermino(CTermino(3, 2));
    poliUno.insertarTermino(CTermino(-2, 1));
    poliUno.insertarTermino(CTermino(7, 0));

    CPolinomio poliDos;
    poliDos.insertarTermino(CTermino(5, 2));
    poliDos.insertarTermino(CTermino(-6, 1));
    poliDos.insertarTermino(CTermino(10, 0));

    cout << "Polinomio P(x) = " << poliUno.aTexto() << endl;
    cout << "Polinomio Q(x) = " << poliDos.aTexto() << endl << endl;

    // ------------------------------------------------
    // Se calcula la suma entre P(x) y Q(x)
    // ------------------------------------------------
    cout << "=== SUMA DE POLINOMIOS ===" << endl;
    cout << "Calculando R(x) = P(x) + Q(x)" << endl;
    CPolinomio resultadoSuma = poliUno.sumar(poliDos);
    cout << "Resultado: R(x) = " << resultadoSuma.aTexto() << endl << endl;

    // ------------------------------------------------
    // Se calcula la multiplicación entre P(x) y Q(x)
    // ------------------------------------------------
    cout << "=== MULTIPLICACIÓN DE POLINOMIOS ===" << endl;
    cout << "Calculando M(x) = P(x) * Q(x)" << endl;
    CPolinomio resultadoMult = poliUno.multiplicar(poliDos);
    cout << "Resultado: M(x) = " << resultadoMult.aTexto() << endl << endl;

    // ------------------------------------------------
    // Mensaje de finalización del programa
    // ------------------------------------------------
    cout << "=== FIN DEL PROGRAMA ===" << endl;
    cout << endl; // línea vacía al final

    return 0;
}

