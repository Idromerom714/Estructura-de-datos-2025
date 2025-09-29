#include "juego.h"

std::string Colors[4] = {BG_RED, BG_WHITE, BG_GRAY, BG_BLUE};

Casilla::Casilla() { estado = 3; }
void Casilla::MarcarImpacto() { estado = 0; }
void Casilla::PonerBarco() { estado = 2; }
void Casilla::MarcarSinImpacto() { estado = 1; }
void Casilla::Mar() { estado = 3; }
void Casilla::Mostrar(bool tipo) {
    if(tipo) {
        std::cout << Colors[estado] << ". " << RESET;
    } else {
        if(estado == 2) {
            std::cout << Colors[3] << ". " << RESET;
        } else {
            std::cout << Colors[estado] << ". " << RESET;
        }
    }
}

Barco::Barco() {
    tamano = 2 + rand() % 4;
    bool horizontal = rand() % 2;
    int x, y;
    if(horizontal) {
        x = rand() % (20 - tamano);
        y = rand() % 20;
        for(int i=0; i<tamano; i++) {
            coordenadas[0][i] = x + i;
            coordenadas[1][i] = y;
        }
    } else {
        x = rand() % 20;
        y = rand() % (20 - tamano);
        for(int i=0; i<tamano; i++) {
            coordenadas[0][i] = x;
            coordenadas[1][i] = y + i;
        }
    }
}

Tablero::Tablero(bool tipo_) {
    tipo = tipo_;
    for(int i=0; i<20; i++) {
        for(int j=0; j<20; j++) {
            Core[i][j].Mar();
        }
    }
}
void Tablero::Mostrar() {
    for(int i=0; i<20; i++) {
        for(int j=0; j<20; j++) {
            Core[j][i].Mostrar(tipo);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}
void Tablero::PonerBarco(Barco b) {
    for(int i=0; i<b.tamano; i++) {
        Core[b.coordenadas[0][i]][b.coordenadas[1][i]].PonerBarco();
    }
}
int Tablero::barcosRestantes() const {
    int count = 0;
    for(int i=0; i<20; i++) {
        for(int j=0; j<20; j++) {
            if(Core[i][j].getEstado() == 2) count++;
        }
    }
    return count;
}