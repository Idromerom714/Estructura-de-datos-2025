#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "juego.h"

class Jugador {
public:
    std::string nombre;
    Tablero tablero;
    Jugador(const std::string& nombre_, bool tipoTablero);
};

#endif // JUGADOR_H
