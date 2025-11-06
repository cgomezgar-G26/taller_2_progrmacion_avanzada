// Archivo: eje_6.cpp
// Compilar: g++ -std=c++17 eje_6.cpp -o juego
// Ejecutar: ./juego (Windows: juego.exe)
// Implementación orientada a objetos del juego de dominó (2-4 jugadores humanos)

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <limits>
#include <map>

// === HABILITAR TILDES Y CARACTERES ESPECIALES EN CONSOLA (Windows + MinGW) ===
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    static struct ConfigurarUTF8 {
        ConfigurarUTF8() {
            SetConsoleOutputCP(CP_UTF8);  // Salida: UTF-8
            SetConsoleCP(CP_UTF8);        // Entrada: UTF-8
        }
    } configurar_utf8;  // Se ejecuta automáticamente al iniciar
#endif

using namespace std;

// =========================
// Clase PiezaDomino
// Representa una pieza de dominó con dos valores (ladoIzquierdo, ladoDerecho).
// =========================
class PiezaDomino {
private:
    int ladoIzquierdo;  // Valor del lado izquierdo de la pieza
    int ladoDerecho;    // Valor del lado derecho de la pieza
public:
    // Constructor: inicializa ambos lados (por defecto [0|0])
    PiezaDomino(int izq = 0, int der = 0) : ladoIzquierdo(izq), ladoDerecho(der) {}

    // Accesores: devuelven los valores de cada lado
    int obtenerIzquierdo() const { return ladoIzquierdo; }
    int obtenerDerecho() const { return ladoDerecho; }

    // Calcula la suma de puntos (pips) de la pieza
    int totalPuntos() const { return ladoIzquierdo + ladoDerecho; }

    // Gira la pieza intercambiando ambos lados
    void voltear() { swap(ladoIzquierdo, ladoDerecho); }

    // Representación en texto: [izq|der]
    string comoTexto() const {
        return "[" + to_string(ladoIzquierdo) + "|" + to_string(ladoDerecho) + "]";
    }

    // Compara dos piezas sin importar su orientación
    bool esIgualA(const PiezaDomino& otra) const {
        return (ladoIzquierdo == otra.ladoIzquierdo && ladoDerecho == otra.ladoDerecho) ||
               (ladoIzquierdo == otra.ladoDerecho && ladoDerecho == otra.ladoIzquierdo);
    }
};

// =========================
// Clase Participante (base)
// Contiene nombre y la colección de piezas en mano.
// =========================
class Participante {
protected:
    string alias;                   // Nombre del participante
    vector<PiezaDomino> piezasMano; // Piezas que tiene en su mano
public:
    // Constructor: recibe el nombre del participante
    Participante(const string& nombre_) : alias(nombre_) {}

    // Destructor virtual para permitir herencia
    virtual ~Participante() = default;

    // Devuelve el nombre del participante
    string obtenerNombre() const { return alias; }

    // Agrega una pieza a la mano
    void tomarPieza(const PiezaDomino& p) { piezasMano.push_back(p); }

    // Quita y devuelve la pieza en el índice indicado
    PiezaDomino sacarPieza(int posicion) {
        PiezaDomino p = piezasMano[posicion];
        piezasMano.erase(piezasMano.begin() + posicion);
        return p;
    }

    // Indica cuántas piezas tiene en mano
    int cantidadPiezas() const { return (int)piezasMano.size(); }

    // Acceso de solo lectura a una pieza específica
    const PiezaDomino& verPieza(int posicion) const { return piezasMano[posicion]; }

    // Muestra todas las piezas de la mano con sus índices
    void mostrarMano() const {
        cout << "Mano de " << alias << " (" << cantidadPiezas() << "):\n";
        for (size_t i = 0; i < piezasMano.size(); ++i)
            cout << " " << i << ": " << piezasMano[i].comoTexto() << "\n";
    }

    // Vacía completamente la mano (para reiniciar rondas)
    void limpiarMano() { piezasMano.clear(); }

    // Método virtual puro: cada tipo de participante define su turno
    virtual bool realizarJugada(deque<PiezaDomino>& tablero,
                                vector<PiezaDomino>& pozo,
                                int extremoIzq,
                                int extremoDer) = 0;
};

// =========================
// ParticipanteHumano : derivado de Participante
// Implementa interacción por consola para elegir jugadas.
// =========================
class ParticipanteHumano : public Participante {
public:
    // Constructor: usa el nombre recibido
    ParticipanteHumano(const string& nombre_) : Participante(nombre_) {}

    // Lógica completa del turno para un jugador humano
    bool realizarJugada(deque<PiezaDomino>& tablero,
                        vector<PiezaDomino>& pozo,
                        int extremoIzq,
                        int extremoDer) override
    {
        // --- Mostrar estado actual del juego ---
        cout << "\nTurno de " << alias << ":\n";
        cout << "Tablero (izq ... der): ";
        if (tablero.empty()) cout << "(vacía)";
        else {
            for (const auto& p : tablero) cout << p.comoTexto();
        }
        cout << "\nValores extremos: ";
        if (tablero.empty()) cout << "N/A (tablero vacío)\n";
        else cout << extremoIzq << " ... " << extremoDer << "\n";
        mostrarMano();

        // --- Lambda: verifica si una pieza es jugable ---
        auto esJugable = [&](const PiezaDomino& p) {
            if (tablero.empty()) return true;
            return p.obtenerIzquierdo() == extremoIzq || p.obtenerDerecho() == extremoIzq ||
                   p.obtenerIzquierdo() == extremoDer || p.obtenerDerecho() == extremoDer;
        };

        // --- Robar del pozo mientras no haya jugada válida ---
        while (true) {
            bool hayJugable = false;
            for (const auto& p : piezasMano) if (esJugable(p)) { hayJugable = true; break; }
            if (hayJugable) break; // Ya puede jugar desde su mano

            if (!pozo.empty()) {
                cout << alias << " no tiene jugadas válidas, roba del pozo.\n";
                PiezaDomino robada = pozo.back();
                pozo.pop_back();
                tomarPieza(robada);
                cout << "Robó " << robada.comoTexto() << "\n";
                continue;
            } else {
                cout << alias << " no puede jugar y el pozo está vacío. Hace PASS.\n";
                return false;
            }
        }

        // --- Interacción con el usuario ---
        while (true) {
            cout << "\nElige acción:\n";
            cout << " p: jugar una pieza\n";
            cout << " v: ver pieza en detalle\n";
            cout << " s: ver tablero y mano (otra vez)\n";
            cout << " x: pasar voluntariamente\n";
            cout << "Opción: ";
            char opcion;
            cin >> opcion;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

            if (opcion == 'p') {
                // --- Seleccionar pieza por índice ---
                cout << "Ingrese el índice de la pieza a jugar (ej: 0): ";
                int idx;
                cin >> idx;
                if (cin.fail() || idx < 0 || idx >= (int)piezasMano.size()) {
                    cout << "Índice inválido.\n";
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                PiezaDomino elegida = piezasMano[idx];

                // --- Caso especial: tablero vacío ---
                if (tablero.empty()) {
                    cout << alias << " coloca " << elegida.comoTexto() << " en tablero.\n";
                    sacarPieza(idx);
                    tablero.push_back(elegida);
                    return true;
                }

                // --- Elegir lado del tablero (Izquierda o Derecha) ---
                cout << "Colocar en (I)zquierda o (D)erecha? (I/D): ";
                char lado;
                cin >> lado;
                if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
                lado = toupper(lado);

                bool colocada = false;
                if (lado == 'I') {
                    // Intentar colocar en la izquierda
                    if (elegida.obtenerDerecho() == extremoIzq) {
                        sacarPieza(idx);
                        tablero.push_front(elegida);
                        colocada = true;
                    } else if (elegida.obtenerIzquierdo() == extremoIzq) {
                        sacarPieza(idx);
                        elegida.voltear();
                        tablero.push_front(elegida);
                        colocada = true;
                    }
                } else if (lado == 'D') {
                    // Intentar colocar en la derecha
                    if (elegida.obtenerIzquierdo() == extremoDer) {
                        sacarPieza(idx);
                        tablero.push_back(elegida);
                        colocada = true;
                    } else if (elegida.obtenerDerecho() == extremoDer) {
                        sacarPieza(idx);
                        elegida.voltear();
                        tablero.push_back(elegida);
                        colocada = true;
                    }
                } else {
                    cout << "Opción de lado inválida.\n";
                    continue;
                }

                if (colocada) {
                    cout << alias << " colocó " << (tablero.front().comoTexto())
                         << " ... " << (tablero.back().comoTexto()) << "\n";
                    return true;
                } else {
                    cout << "La pieza seleccionada no puede colocarse en ese lado.\n";
                    continue;
                }

            } else if (opcion == 'v') {
                // Ver detalle de una pieza específica
                cout << "Introduce índice para ver: ";
                int idx; cin >> idx;
                if (cin.fail() || idx < 0 || idx >= (int)piezasMano.size()) {
                    cout << "Índice inválido.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
                cout << "Pieza " << piezasMano[idx].comoTexto() << "\n";

            } else if (opcion == 's') {
                // Mostrar tablero y mano nuevamente
                cout << "Tablero: ";
                for (const auto& p : tablero) cout << p.comoTexto();
                cout << "\n";
                mostrarMano();

            } else if (opcion == 'x') {
                // Pasar voluntariamente
                cout << alias << " decide pasar voluntariamente.\n";
                return false;

            } else {
                cout << "Opción desconocida.\n";
            }
        } // fin while interacción
    } // fin realizarJugada
}; // fin ParticipanteHumano

// =========================
// Clase PartidaDomino
// Gestiona el flujo completo del juego: piezas, reparto, turnos, puntuación y rondas.
// =========================
class PartidaDomino {
private:
    vector<PiezaDomino> fabricaPiezas;    // Conjunto completo de 28 piezas
    vector<PiezaDomino> pozo;             // Piezas restantes para robar
    deque<PiezaDomino> tablero;           // Piezas colocadas en la mesa
    vector<Participante*> jugadores;      // Lista de participantes (polimorfismo)
    map<string,int> puntuacionTotal;      // Puntos acumulados por nombre
    int totalJugadores;                   // Número actual de jugadores
    mt19937 generadorAleatorio;           // Motor de aleatorización

public:
    // Constructor: inicializa el generador aleatorio con la hora actual
    PartidaDomino() {
        generadorAleatorio.seed((unsigned)time(nullptr));
    }

    // Destructor: libera memoria de los jugadores
    ~PartidaDomino() {
        for (auto p : jugadores) delete p;
        jugadores.clear();
    }

    // Genera las 28 piezas estándar (0-0 hasta 6-6)
    void generarConjuntoCompleto() {
        fabricaPiezas.clear();
        for (int izq = 0; izq <= 6; ++izq)
            for (int der = izq; der <= 6; ++der)
                fabricaPiezas.emplace_back(izq, der);
    }

    // Baraja las piezas y prepara el pozo
    void mezclarYPreparar() {
        generarConjuntoCompleto();
        shuffle(fabricaPiezas.begin(), fabricaPiezas.end(), generadorAleatorio);
        pozo = fabricaPiezas; // Copia completa al pozo
    }

    // Solicita número y nombres de jugadores (2-4)
    void configurarParticipantes() {
        for (auto p : jugadores) delete p;
        jugadores.clear();
        puntuacionTotal.clear();

        cout << "Ingrese número de jugadores (2-4): ";
        while (true) {
            cin >> totalJugadores;
            if (cin.fail() || totalJugadores < 2 || totalJugadores > 4) {
                cout << "Número inválido. Ingrese entre 2 y 4: ";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else break;
        }

        for (int i = 0; i < totalJugadores; ++i) {
            cout << "Nombre del jugador " << (i+1) << ": ";
            string nombre;
            cin >> ws; getline(cin, nombre);
            if (nombre.empty()) nombre = "Jugador" + to_string(i+1);
            Participante* p = new ParticipanteHumano(nombre);
            jugadores.push_back(p);
            puntuacionTotal[nombre] = 0;
        }
    }

    // Reparte 7 piezas a cada jugador; el resto queda en el pozo
    void distribuirPiezas() {
        for (auto p : jugadores) p->limpiarMano();
        tablero.clear();

        for (int r = 0; r < 7; ++r) {
            for (auto p : jugadores) {
                if (pozo.empty()) break;
                PiezaDomino pieza = pozo.back(); pozo.pop_back();
                p->tomarPieza(pieza);
            }
        }
    }

    // Ejecuta una ronda completa hasta que alguien gane o se bloquee
    string ejecutarRonda() {
        mezclarYPreparar();
        distribuirPiezas();

        int turnoActual = 0;
        int pasesConsecutivos = 0;
        tablero.clear();

        while (true) {
            Participante* jugadorActual = jugadores[turnoActual];

            // Obtener valores de los extremos del tablero
            int valIzq = -1, valDer = -1;
            if (!tablero.empty()) {
                valIzq = tablero.front().obtenerIzquierdo();
                valDer = tablero.back().obtenerDerecho();
            }

            bool jugo = jugadorActual->realizarJugada(tablero, pozo, valIzq, valDer);
            if (jugo) {
                pasesConsecutivos = 0;
                if (jugadorActual->cantidadPiezas() == 0) {
                    cout << "\n*** " << jugadorActual->obtenerNombre()
                         << " se quedó sin piezas y gana la ronda! ***\n";
                    return jugadorActual->obtenerNombre();
                }
            } else {
                pasesConsecutivos++;
                cout << jugadorActual->obtenerNombre() << " pasó.\n";
            }

            // Detectar bloqueo: todos los jugadores pasaron consecutivamente
            if (pasesConsecutivos >= totalJugadores) {
                cout << "\n--- Estado de bloqueo: ninguno puede jugar ---\n";
                return ""; // Cadena vacía indica bloqueo
            }

            turnoActual = (turnoActual + 1) % totalJugadores;
        }
    }

    // Calcula puntos de la ronda y actualiza acumulados
    string procesarPuntuacion(const string& nombreGanador) {
        map<string,int> sumasPips;
        for (auto p : jugadores) {
            int suma = 0;
            for (int i = 0; i < p->cantidadPiezas(); ++i)
                suma += p->verPieza(i).totalPuntos();
            sumasPips[p->obtenerNombre()] = suma;
        }

        if (!nombreGanador.empty()) {
            // Ganador directo: suma de pips de los demás
            int puntos = 0;
            for (auto& kv : sumasPips) if (kv.first != nombreGanador) puntos += kv.second;
            puntuacionTotal[nombreGanador] += puntos;
            cout << nombreGanador << " obtiene " << puntos << " puntos de esta ronda.\n";
            return nombreGanador;
        } else {
            // Bloqueo: gana quien tenga menor suma de pips
            string ganadorBloqueo = "";
            int menorSuma = INT_MAX;
            for (auto& kv : sumasPips) {
                if (kv.second < menorSuma) { menorSuma = kv.second; ganadorBloqueo = kv.first; }
            }
            int puntos = 0;
            for (auto& kv : sumasPips) if (kv.first != ganadorBloqueo) puntos += kv.second;
            puntuacionTotal[ganadorBloqueo] += puntos;
            cout << "Bloqueo: gana " << ganadorBloqueo << " con menor total (" << menorSuma << ").\n";
            cout << ganadorBloqueo << " obtiene " << puntos << " puntos de esta ronda.\n";
            return ganadorBloqueo;
        }
    }

    // Muestra los puntajes acumulados actuales
    void mostrarPuntajes() const {
        cout << "\n=== PUNTAJES ACUMULADOS ===\n";
        for (const auto& kv : puntuacionTotal) {
            cout << " " << kv.first << ": " << kv.second << "\n";
        }
        cout << "==========================\n";
    }

    // Flujo principal del juego
    void iniciar() {
        cout << "===== BIENVENIDO A DOMINÓ (CONSOLE) =====\n";
        configurarParticipantes();
        bool salir = false;

        while (!salir) {
            cout << "\n--> Nueva ronda: barajando y repartiendo...\n";
            string ganadorRonda = ejecutarRonda();
            string ganadorFinal = procesarPuntuacion(ganadorRonda);
            cout << "\nResumen de la ronda: ganador -> ";
            if (!ganadorFinal.empty()) cout << ganadorFinal << "\n";
            else cout << "Ninguno (bloqueo)\n";
            mostrarPuntajes();

            // Opciones tras la ronda
            cout << "\nOpciones:\n"
                 << " 1 - Jugar otra ronda (mantener puntajes)\n"
                 << " 2 - Reiniciar puntajes y empezar desde cero\n"
                 << " 3 - Reiniciar jugadores y empezar con nueva configuración\n"
                 << " 4 - Salir\n"
                 << "Elija una opción: ";
            int opcion;
            cin >> opcion;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

            if (opcion == 1) {
                continue;
            } else if (opcion == 2) {
                for (auto& kv : puntuacionTotal) kv.second = 0;
                cout << "Puntajes reiniciados.\n";
            } else if (opcion == 3) {
                configurarParticipantes();
            } else if (opcion == 4) {
                salir = true;
            } else {
                cout << "Opción desconocida, continuando.\n";
            }
        }
        cout << "Gracias por jugar. ¡Hasta la próxima!\n";
    }
}; // fin clase PartidaDomino

// =========================
// MAIN
// =========================
int main() {
    PartidaDomino partida;
    partida.iniciar();
    return 0;
}