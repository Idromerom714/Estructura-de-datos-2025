#include "juego.h"
#include "jugador.h"
#include "gestor_turnos.h"
#include <fstream>

void mostrarMenu() {
    std::cout << "\n===== BATALLA NAVAL =====\n";
    std::cout << "1. Iniciar partida\n";
    std::cout << "2. Cargar partida\n";
    std::cout << "3. Guardar partida\n";
    std::cout << "4. Salir\n";
    std::cout << "Seleccione una opción: ";
}

void guardarPartida(const Jugador& j1, const Jugador& j2, int turno) {
    std::ofstream file("partida.dat", std::ios::binary);
    file << j1.nombre << '\n' << j2.nombre << '\n' << turno << '\n';
    for(int k=0; k<2; k++) {
        const Tablero& t = (k==0) ? j1.tablero : j2.tablero;
        for(int i=0; i<20; i++) {
            for(int j=0; j<20; j++) {
                file << t.Core[i][j].getEstado() << ' ';
            }
        }
        file << '\n';
    }
    file.close();
}

bool cargarPartida(Jugador& j1, Jugador& j2, int& turno) {
    std::ifstream file("partida.dat", std::ios::binary);
    if(!file) return false;
    std::getline(file, j1.nombre);
    std::getline(file, j2.nombre);
    file >> turno;
    file.ignore();
    for(int k=0; k<2; k++) {
        Tablero& t = (k==0) ? j1.tablero : j2.tablero;
        for(int i=0; i<20; i++) {
            for(int j=0; j<20; j++) {
                int est;
                file >> est;
                if(est == 0) t.Core[i][j].MarcarImpacto();
                else if(est == 1) t.Core[i][j].MarcarSinImpacto();
                else if(est == 2) t.Core[i][j].PonerBarco();
                else t.Core[i][j].Mar();
            }
        }
    }
    file.close();
    return true;
}

int main() {
    srand(time(NULL));
    int opcion;
    Jugador j1("", true), j2("", true);
    GestorTurnos gestor;
    bool partidaActiva = false;
    do {
        mostrarMenu();
        std::cin >> opcion;
        if(opcion == 1) {
            std::string nombre1, nombre2;
            std::cout << "Ingrese nombre del Jugador 1: ";
            std::cin >> nombre1;
            std::cout << "Ingrese nombre del Jugador 2: ";
            std::cin >> nombre2;
            j1 = Jugador(nombre1, true);
            j2 = Jugador(nombre2, true);
            for(int i=0; i<4; i++){
                Barco b1;
                j1.tablero.PonerBarco(b1);
                Barco b2;
                j2.tablero.PonerBarco(b2);
            }
            gestor = GestorTurnos();
            partidaActiva = true;
            gestor.jugar(j1, j2);
        } else if(opcion == 2) {
            int turno = 0;
            if(cargarPartida(j1, j2, turno)) {
                gestor = GestorTurnos();
                gestor.turno = turno;
                partidaActiva = true;
                gestor.jugar(j1, j2);
            } else {
                std::cout << "No se encontró partida guardada.\n";
            }
        } else if(opcion == 3) {
            if(partidaActiva) {
                guardarPartida(j1, j2, gestor.turno);
                std::cout << "Partida guardada exitosamente.\n";
            } else {
                std::cout << "No hay partida activa para guardar.\n";
            }
        } else if(opcion == 4) {
            std::cout << "¡Hasta luego!\n";
        } else {
            std::cout << "Opción inválida. Intente de nuevo.\n";
        }
    } while(opcion != 4);
    return 0;
}
