#include "gestor_turnos.h"
#include <string>
#include <iostream>

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
    } else if (objetivo.getEstado() == 3) {
        objetivo.MarcarSinImpacto();
        std::cout << "Disparo al agua.\n";
        return false;
    } else {
        std::cout << "Ya disparaste en esta posición. Pierdes el turno.\n";
        return false;
    }
}

void GestorTurnos::jugar(Jugador& j1, Jugador& j2) {
    bool sigueTurno = true;
    std::string comando;
    
    std::cout << "\n===========================================\n";
    std::cout << "    ¡LA BATALLA HA COMENZADO!\n";
    std::cout << "===========================================\n";
    std::cout << "\nINSTRUCCIONES:\n";
    std::cout << "  - Para disparar: ingresa coordenadas (x y)\n";
    std::cout << "  - Para GUARDAR y salir: escribe 'g'\n";
    std::cout << "  - Para rendirse: escribe 'r'\n\n";
    
    while (true) {
        // Verificar si algún jugador perdió todos sus barcos
        if (j1.tablero.barcosRestantes() == 0) {
            std::cout << "\n===========================================\n";
            std::cout << "  " << j2.nombre << " HA GANADO!\n";
            std::cout << "===========================================\n";
            std::cout << "Todos los barcos de " << j1.nombre << " fueron hundidos!\n\n";
            break;
        }
        if (j2.tablero.barcosRestantes() == 0) {
            std::cout << "\n===========================================\n";
            std::cout << "  " << j1.nombre << " HA GANADO!\n";
            std::cout << "===========================================\n";
            std::cout << "Todos los barcos de " << j2.nombre << " fueron hundidos!\n\n";
            break;
        }
        
        // Turno del jugador actual
        if (turno == 0) {
            std::cout << "\n-------------------------------------------\n";
            std::cout << "Turno de: " << j1.nombre << "\n";
            std::cout << "Barcos enemigos restantes: " << j2.tablero.barcosRestantes() << "\n";
            std::cout << "-------------------------------------------\n\n";
            
            std::cout << "Tablero enemigo:\n";
            j2.tablero.Mostrar();
            
            std::cout << j1.nombre << ", tu turno (x y / g=guardar / r=rendirse): ";
            std::cin >> comando;
            
            // Verificar comandos especiales
            if (comando == "g" || comando == "G" || comando == "guardar" || comando == "GUARDAR") {
                std::cout << "\n[INFO] Saliendo al menú para guardar...\n";
                std::cout << "[INFO] Selecciona opción 3 en el menú para guardar la partida.\n";
                return; // Salir del juego y volver al menú
            }
            else if (comando == "r" || comando == "R" || comando == "rendirse" || comando == "RENDIRSE") {
                std::cout << "\n" << j1.nombre << " se ha rendido.\n";
                std::cout << j2.nombre << " GANA POR RENDICIÓN!\n\n";
                return;
            }
            else {
                // Intentar parsear coordenadas
                int x, y;
                try {
                    x = std::stoi(comando);
                    if (!(std::cin >> y)) {
                        std::cout << "Entrada inválida. Pierdes el turno.\n";
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                        turno = 1;
                        continue;
                    }
                    
                    if (x < 0 || x >= 20 || y < 0 || y >= 20) {
                        std::cout << "Coordenadas fuera de rango (0-19). Pierdes el turno.\n";
                        sigueTurno = false;
                    } else {
                        Casilla& objetivo = j2.tablero.Core[x][y];
                        if (objetivo.getEstado() == 2) {
                            objetivo.MarcarImpacto();
                            std::cout << "\n¡IMPACTO EN BARCO ENEMIGO!\n";
                            sigueTurno = true;
                        } else if (objetivo.getEstado() == 3) {
                            objetivo.MarcarSinImpacto();
                            std::cout << "\nDisparo al agua...\n";
                            sigueTurno = false;
                        } else {
                            std::cout << "\nYa disparaste aquí. Pierdes el turno.\n";
                            sigueTurno = false;
                        }
                    }
                } catch (...) {
                    std::cout << "Comando no reconocido. Pierdes el turno.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    sigueTurno = false;
                }
            }
            
            if (!sigueTurno) turno = 1;
            
        } else {
            // Turno del jugador 2 (igual lógica)
            std::cout << "\n-------------------------------------------\n";
            std::cout << "Turno de: " << j2.nombre << "\n";
            std::cout << "Barcos enemigos restantes: " << j1.tablero.barcosRestantes() << "\n";
            std::cout << "-------------------------------------------\n\n";
            
            std::cout << "Tablero enemigo:\n";
            j1.tablero.Mostrar();
            
            std::cout << j2.nombre << ", tu turno (x y / g=guardar / r=rendirse): ";
            std::cin >> comando;
            
            if (comando == "g" || comando == "G" || comando == "guardar" || comando == "GUARDAR") {
                std::cout << "\n[INFO] Saliendo al menú para guardar...\n";
                std::cout << "[INFO] Selecciona opción 3 en el menú para guardar la partida.\n";
                return;
            }
            else if (comando == "r" || comando == "R" || comando == "rendirse" || comando == "RENDIRSE") {
                std::cout << "\n" << j2.nombre << " se ha rendido.\n";
                std::cout << j1.nombre << " GANA POR RENDICIÓN!\n\n";
                return;
            }
            else {
                int x, y;
                try {
                    x = std::stoi(comando);
                    if (!(std::cin >> y)) {
                        std::cout << "Entrada inválida. Pierdes el turno.\n";
                        std::cin.clear();
                        std::cin.ignore(10000, '\n');
                        turno = 0;
                        continue;
                    }
                    
                    if (x < 0 || x >= 20 || y < 0 || y >= 20) {
                        std::cout << "Coordenadas fuera de rango (0-19). Pierdes el turno.\n";
                        sigueTurno = false;
                    } else {
                        Casilla& objetivo = j1.tablero.Core[x][y];
                        if (objetivo.getEstado() == 2) {
                            objetivo.MarcarImpacto();
                            std::cout << "\n¡IMPACTO EN BARCO ENEMIGO!\n";
                            sigueTurno = true;
                        } else if (objetivo.getEstado() == 3) {
                            objetivo.MarcarSinImpacto();
                            std::cout << "\nDisparo al agua...\n";
                            sigueTurno = false;
                        } else {
                            std::cout << "\nYa disparaste aquí. Pierdes el turno.\n";
                            sigueTurno = false;
                        }
                    }
                } catch (...) {
                    std::cout << "Comando no reconocido. Pierdes el turno.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    sigueTurno = false;
                }
            }
            
            if (!sigueTurno) turno = 0;
        }
    }
}