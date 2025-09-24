# Estructura-de-datos-2025

## 🎯 Diagrama de Clases UML
```mermaid
classDiagram
    class MenuPrincipal {
        -GameManager gameManager
        +MostrarTitulo()
        +MostrarMenu()
        +EjecutarOpcion(int)
        +Run()
    }
    
    class GameManager {
        -Player jugador1
        -Player jugador2
        -bool turnoJugador1
        +IniciarNuevaPartida(string, string)
        +CargarPartida(string)
        +GuardarPartida(string)
        +ProcesarDisparo(int, int)
    }
    
    class Player {
        -string nombre
        -Tablero tableroPropio
        -Tablero tableroEnemigo
        +ColocarBarcos()
        +RealizarDisparo()
        +RecibirDisparo(int, int)
    }
    
    class Tablero {
        -Casilla Core[20][20]
        -bool tipo
        +Mostrar()
        +PonerBarco(Barco)
        +VerificarImpacto(int, int)
    }
    
    class Casilla {
        -int estado
        +MarcarImpacto()
        +PonerBarco()
        +MarcarSinImpacto()
        +Mostrar(bool)
    }
    
    class FileManager {
        +GuardarPartida(GameManager)
        +CargarPartida(string)
        +ValidarArchivo(string)
    }
    
    MenuPrincipal --> GameManager
    GameManager --> Player
    GameManager --> FileManager
    Player --> Tablero
    Tablero --> Casilla
