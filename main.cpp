#include "juego.h"
#include "jugador.h"
#include "gestor_turnos.h"
#include <fstream>
#include <iostream>

void mostrarMenu() {
    std::cout << "\n===== ASTUCIA NAVAL =====\n";
    std::cout << "1. Iniciar nueva partida\n";
    std::cout << "2. Cargar partida guardada\n";
    std::cout << "3. Guardar partida actual\n";
    std::cout << "4. Continuar partida en curso\n";
    std::cout << "5. Salir\n";
    std::cout << "Seleccione una opción: ";
}

// Función para guardar la partida completa
bool guardarPartida(const Jugador& j1, const Jugador& j2, const GestorTurnos& gestor, const std::string& nombreArchivo = "partida_guardada.dat") {
    std::cout << "\n[DEBUG] Intentando guardar en: " << nombreArchivo << "\n";
    std::cout << "[DEBUG] Jugador 1: " << j1.nombre << "\n";
    std::cout << "[DEBUG] Jugador 2: " << j2.nombre << "\n";
    std::cout << "[DEBUG] Turno actual: " << gestor.turno << "\n";
    
    std::ofstream file(nombreArchivo);
    
    if (!file.is_open()) {
        std::cerr << "❌ ERROR: No se pudo crear el archivo de guardado.\n";
        std::cerr << "Verifica permisos de escritura en el directorio actual.\n";
        return false;
    }
    
    std::cout << "[DEBUG] Archivo abierto correctamente.\n";
    
    // Guardar información básica
    file << "ASTUCIA_NAVAL_SAVE_V1\n"; // Identificador de versión
    file << j1.nombre << '\n';
    file << j2.nombre << '\n';
    file << gestor.turno << '\n';
    
    // Guardar tablero del jugador 1
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            file << j1.tablero.Core[i][j].getEstado() << ' ';
        }
        file << '\n';
    }
    
    // Guardar tablero del jugador 2
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            file << j2.tablero.Core[i][j].getEstado() << ' ';
        }
        file << '\n';
    }
    
    file.close();
    
    // Verificar que el archivo se creó
    std::ifstream verificar(nombreArchivo);
    if (verificar.is_open()) {
        std::cout << "[DEBUG] Archivo verificado - existe en disco.\n";
        verificar.close();
    } else {
        std::cout << "[DEBUG] ADVERTENCIA: No se puede verificar el archivo.\n";
    }
    
    std::cout << "\n✓ Partida guardada exitosamente en: " << nombreArchivo << "\n";
    return true;
}

// Función para cargar una partida guardada
bool cargarPartida(Jugador& j1, Jugador& j2, GestorTurnos& gestor, const std::string& nombreArchivo = "partida_guardada.dat") {
    std::ifstream file(nombreArchivo);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se encontró el archivo de partida guardada.\n";
        return false;
    }
    
    // Verificar identificador de versión
    std::string version;
    std::getline(file, version);
    if (version != "ASTUCIA_NAVAL_SAVE_V1") {
        std::cerr << "Error: Formato de archivo inválido o versión no compatible.\n";
        file.close();
        return false;
    }
    
    // Cargar nombres de jugadores
    std::string nombre1, nombre2;
    std::getline(file, nombre1);
    std::getline(file, nombre2);
    
    if (nombre1.empty() || nombre2.empty()) {
        std::cerr << "Error: Nombres de jugadores inválidos en el archivo.\n";
        file.close();
        return false;
    }
    
    // Cargar turno actual
    int turnoGuardado;
    file >> turnoGuardado;
    file.ignore(); // Ignorar el salto de línea
    
    if (turnoGuardado < 0 || turnoGuardado > 1) {
        std::cerr << "Error: Turno inválido en el archivo.\n";
        file.close();
        return false;
    }
    
    // Crear nuevos jugadores con los nombres cargados
    j1 = Jugador(nombre1, true);
    j2 = Jugador(nombre2, true);
    
    // Cargar tablero del jugador 1
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int estado;
            file >> estado;
            
            if (file.fail() || estado < 0 || estado > 3) {
                std::cerr << "Error: Datos de tablero corruptos en el archivo.\n";
                file.close();
                return false;
            }
            
            // Restaurar el estado de cada casilla
            switch (estado) {
                case 0: // Impacto
                    j1.tablero.Core[i][j].PonerBarco(); // Primero poner barco
                    j1.tablero.Core[i][j].MarcarImpacto(); // Luego marcar impacto
                    break;
                case 1: // Disparo al agua
                    j1.tablero.Core[i][j].MarcarSinImpacto();
                    break;
                case 2: // Barco intacto
                    j1.tablero.Core[i][j].PonerBarco();
                    break;
                case 3: // Mar sin disparar
                    j1.tablero.Core[i][j].Mar();
                    break;
            }
        }
    }
    
    // Cargar tablero del jugador 2
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int estado;
            file >> estado;
            
            if (file.fail() || estado < 0 || estado > 3) {
                std::cerr << "Error: Datos de tablero corruptos en el archivo.\n";
                file.close();
                return false;
            }
            
            // Restaurar el estado de cada casilla
            switch (estado) {
                case 0: // Impacto
                    j2.tablero.Core[i][j].PonerBarco();
                    j2.tablero.Core[i][j].MarcarImpacto();
                    break;
                case 1: // Disparo al agua
                    j2.tablero.Core[i][j].MarcarSinImpacto();
                    break;
                case 2: // Barco intacto
                    j2.tablero.Core[i][j].PonerBarco();
                    break;
                case 3: // Mar sin disparar
                    j2.tablero.Core[i][j].Mar();
                    break;
            }
        }
    }
    
    // Restaurar el gestor de turnos
    gestor.turno = turnoGuardado;
    
    file.close();
    std::cout << "\n✓ Partida cargada exitosamente desde: " << nombreArchivo << "\n";
    std::cout << "Continuando partida: " << nombre1 << " vs " << nombre2 << "\n";
    std::cout << "Turno actual: " << (turnoGuardado == 0 ? nombre1 : nombre2) << "\n";
    
    return true;
}

int main() {
    srand(time(NULL));
    
    int opcion;
    Jugador j1("", true);
    Jugador j2("", true);
    GestorTurnos gestor;
    bool partidaActiva = false;
    
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║     BIENVENIDO A ASTUCIA NAVAL    ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    
    do {
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer
        
        switch (opcion) {
            case 1: { // Iniciar nueva partida
                std::string nombre1, nombre2;
                std::cout << "\n--- Nueva Partida ---\n";
                std::cout << "Ingrese nombre del Jugador 1: ";
                std::getline(std::cin, nombre1);
                std::cout << "Ingrese nombre del Jugador 2: ";
                std::getline(std::cin, nombre2);
                
                // Crear jugadores
                j1 = Jugador(nombre1, true);
                j2 = Jugador(nombre2, true);
                
                // Colocar 4 barcos para cada jugador
                std::cout << "\nColocando barcos aleatoriamente...\n";
                for (int i = 0; i < 4; i++) {
                    Barco b1;
                    j1.tablero.PonerBarco(b1);
                    Barco b2;
                    j2.tablero.PonerBarco(b2);
                }
                
                gestor = GestorTurnos();
                partidaActiva = true;
                
                std::cout << "\n¡Partida iniciada! Que comience la batalla.\n";
                
                // Comenzar el juego
                gestor.jugar(j1, j2);
                
                // Verificar si la partida terminó (alguien ganó) o solo se pausó
                if (j1.tablero.barcosRestantes() == 0 || j2.tablero.barcosRestantes() == 0) {
                    // La partida terminó con un ganador
                    partidaActiva = false;
                } else {
                    // La partida se pausó (se escribió 'g' para guardar)
                    partidaActiva = true;
                    std::cout << "\n[INFO] Partida pausada. La partida sigue activa.\n";
                    std::cout << "[INFO] Ahora puedes guardarla (opción 3) o continuar jugando (opción 1).\n";
                }
                break;
            }
            
            case 2: { // Cargar partida
                if (partidaActiva) {
                    char confirmar;
                    std::cout << "\n¡Advertencia! Hay una partida en curso.\n";
                    std::cout << "¿Desea abandonarla y cargar otra? (s/n): ";
                    std::cin >> confirmar;
                    std::cin.ignore();
                    if (confirmar != 's' && confirmar != 'S') {
                        break;
                    }
                }
                
                std::cout << "\n--- Cargar Partida ---\n";
                std::cout << "Buscando archivo: partida_guardada.dat\n";
                
                if (cargarPartida(j1, j2, gestor)) {
                    partidaActiva = true;
                    
                    // Mostrar estado de los tableros
                    std::cout << "\n--- Estado actual del juego ---\n";
                    std::cout << "Barcos restantes de " << j1.nombre << ": " << j1.tablero.barcosRestantes() << "\n";
                    std::cout << "Barcos restantes de " << j2.nombre << ": " << j2.tablero.barcosRestantes() << "\n";
                    
                    std::cout << "\n¿Desea continuar jugando ahora? (s/n): ";
                    char continuar;
                    std::cin >> continuar;
                    std::cin.ignore();
                    
                    if (continuar == 's' || continuar == 'S') {
                        // Continuar el juego
                        gestor.jugar(j1, j2);
                        // Al terminar la partida, ya no está activa
                        partidaActiva = false;
                    } else {
                        std::cout << "Partida cargada. Puedes continuar desde el menú.\n";
                    }
                } else {
                    std::cout << "\n❌ No se pudo cargar la partida.\n";
                    std::cout << "Posibles causas:\n";
                    std::cout << "  - El archivo 'partida_guardada.dat' no existe\n";
                    std::cout << "  - El archivo está corrupto\n";
                    std::cout << "  - No hay una partida guardada previamente\n";
                }
                break;
            }
            
            case 3: { // Guardar partida
                std::cout << "\n--- Guardar Partida ---\n";
                std::cout << "[DEBUG] partidaActiva = " << (partidaActiva ? "true" : "false") << "\n";
                
                if (!partidaActiva) {
                    std::cout << "\n❌ No hay ninguna partida activa para guardar.\n";
                    std::cout << "Inicia una nueva partida o carga una existente primero.\n";
                } else {
                    std::cout << "[DEBUG] Intentando guardar partida...\n";
                    if (guardarPartida(j1, j2, gestor)) {
                        std::cout << "Puedes continuar jugando o salir del juego.\n";
                        
                        // Preguntar si quiere continuar jugando
                        std::cout << "\n¿Desea continuar jugando? (s/n): ";
                        char continuar;
                        std::cin >> continuar;
                        std::cin.ignore();
                        
                        if (continuar == 's' || continuar == 'S') {
                            gestor.jugar(j1, j2);
                            partidaActiva = false;
                        }
                    } else {
                        std::cout << "❌ Hubo un error al guardar la partida.\n";
                    }
                }
                break;
            }
            
            case 4: { // Continuar partida
                if (!partidaActiva) {
                    std::cout << "\nNo hay ninguna partida en curso.\n";
                    std::cout << "Inicia una nueva partida (opción 1) o carga una guardada (opción 2).\n";
                } else {
                    std::cout << "\n--- Continuando Partida ---\n";
                    std::cout << "Reanudando: " << j1.nombre << " vs " << j2.nombre << "\n";
                    std::cout << "Turno actual: " << (gestor.turno == 0 ? j1.nombre : j2.nombre) << "\n\n";
                    
                    // Continuar el juego
                    gestor.jugar(j1, j2);
                    
                    // Verificar si terminó o se pausó de nuevo
                    if (j1.tablero.barcosRestantes() == 0 || j2.tablero.barcosRestantes() == 0) {
                        partidaActiva = false;
                    } else {
                        std::cout << "\n[INFO] Partida pausada nuevamente.\n";
                    }
                }
                break;
            }
            
            case 5: { // Salir
                if (partidaActiva) {
                    char confirmar;
                    std::cout << "\n¡Advertencia! Hay una partida en curso.\n";
                    std::cout << "¿Desea guardar antes de salir? (s/n): ";
                    std::cin >> confirmar;
                    if (confirmar == 's' || confirmar == 'S') {
                        guardarPartida(j1, j2, gestor);
                    }
                }
                std::cout << "\n¡Gracias por jugar Astucia Naval! ¡Hasta pronto!\n\n";
                break;
            }
            
            default:
                std::cout << "\nOpción inválida. Por favor, seleccione una opción del 1 al 5.\n";
                break;
        }
        
    } while (opcion != 5);
    
    return 0;
}