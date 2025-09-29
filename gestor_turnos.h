#ifndef GESTOR_TURNOS_H
#define GESTOR_TURNOS_H

#include "jugador.h"
#include <iostream>

class GestorTurnos {
public:
    int turno; // 0: j1, 1: j2
    GestorTurnos();
    bool disparar(Jugador& atacante, Jugador& defensor);
    void jugar(Jugador& j1, Jugador& j2);
};

#endif // GESTOR_TURNOS_H
