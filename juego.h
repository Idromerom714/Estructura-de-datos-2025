#ifndef JUEGO_H
#define JUEGO_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#define BG_RED      "\x1B[41m"
#define BG_WHITE    "\x1B[47m"
#define BG_GRAY     "\x1B[48;2;176;174;174m"
#define BG_BLUE     "\x1B[44m"
#define RESET       "\x1b[0m"

extern std::string Colors[4];

class Casilla {
private:
    int estado;
public:
    Casilla();
    void MarcarImpacto();
    void PonerBarco();
    void MarcarSinImpacto();
    void Mar();
    void Mostrar(bool tipo);
    int getEstado() const { return estado; }
};

class Barco {
public:
    Barco();
    int coordenadas[2][5];
    int tamano;
};

class Tablero {
private:
    bool tipo;
public:
    Casilla Core[20][20];
    Tablero(bool tipo_);
    void Mostrar();
    void PonerBarco(Barco b);
    int barcosRestantes() const;
};

#endif // JUEGO_H
