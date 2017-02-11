# Break-out-game
El programa adjunto es compatible con cualquier sistema operativo.
Para ejecutar el programa, simplemente debe abrir una consola (Linux) o símbolo de sistema (Windows) y escribir el comando “make”. Este comando efectuará la compilación de todos los archivos, finalmente produciendo un ejecutable llamado “Breakout.exe”
Para llamar al programa, es necesario un archivo de configuración (el comportamiento del programa es no-determinístico sin él). Las llamadas son de la forma


$ ./Breakout.exe <archivo>


donde “$” es el “prompt” de la cónsola y <archivo> es el nombre del archivo de configuración. Así, si el nombre del archivo es “juego.txt”, la llamada sería “./Breakout.exe juego.txt”
Al inicio del juego, se carga el nivel 1 del archivo. El jugador puede mover la paleta con las flechas izquierda y derecha. La pelota se moverá junto con la paleta hasta que el jugador presione la barra espaciadora. La pelota es lanzada verticalmente con un ángulo de [±5]° escogido al azar.
Al impactar un ladrillo, este cambia de color, volviéndose progresivamente más amarillo hasta que se pueda destruir. La puntuación resultante de impactar un ladrillo viene dada por su color original. La puntuación del jugador, y la posición de la pelota, se muestran por la cónsola o símbolo de sistema mientras se juega.
