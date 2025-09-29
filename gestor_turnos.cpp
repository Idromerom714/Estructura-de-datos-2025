#include "gestor_turnos.h"

GestorTurnos::GestorTurnos() : turno(0) {}

bool GestorTurnos::disparar(Jugador& atacante, Jugador& defensor) {
    int x, y;
    std::cout << atacante.nombre << ", ingresa coordenadas para disparar (x y): ";
    std::cin >> x >> y;
    if (x < 0 || x >= 20 || y < 0 || y >= 20) {
        std::cout << "Coordenadas fuera de rango. Pierdes el turno.\n";
        return false;
    }
    Casilla& objetivo = defensor.tablero.Core[x][y];
    if (objetivo.getEstado() == 2) {
        objetivo.MarcarImpacto();
        std::cout << "¡Impacto en barco enemigo!\n";
        return true;
    } else {
        objetivo.MarcarSinImpacto();
        std::cout << "Disparo al agua.\n";
        return false;
    }
}

void GestorTurnos::jugar(Jugador& j1, Jugador& j2) {
    bool sigueTurno = true;
    while (true) {
        if (j1.tablero.barcosRestantes() == 0) {
            std::cout << j2.nombre << " ha ganado!\n";
            break;
        }
        if (j2.tablero.barcosRestantes() == 0) {
            std::cout << j1.nombre << " ha ganado!\n";
            break;
        }
        if (turno == 0) {
            std::cout << "\nTurno de " << j1.nombre << std::endl;
            sigueTurno = disparar(j1, j2);
            j2.tablero.Mostrar();
            if (!sigueTurno) turno = 1;
        } else {
            std::cout << "\nTurno de " << j2.nombre << std::endl;
            sigueTurno = disparar(j2, j1);
            j1.tablero.Mostrar();
            if (!sigueTurno) turno = 0;
        }
    }
}
