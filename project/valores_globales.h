#ifndef valores_globales
	#define valores_globales	//evitar que se fijen estos defines varias veces
	
	//constante matemática necesaria
	#define PI 3.1415926535897932
	
	//Dimensiones de los ladrillos
	#define	ALTURA_LADRILLO	2
	#define	ANCHO_LADRILLO	5
	#define	PROFUNDIDAD_LADRILLO	2
	
	//Dimensiones del área de juego
	#define	ALTURA_MAXIMA	25
	#define	ANCHO_MAXIMO	10
	
	//Dimensiones de la pelota
	#define	RADIO	0.5
	
	//dimensiones de la parte ancha de la barra direccional
	#define	ALTURA_BARRA	2
	#define	ANCHO_BARRA	7
	#define	PROFUNDIDAD_BARRA	2
	
	//dimensiones de la zona central de la paleta
	#define	ALTURA_CENTRO	1
	#define	ANCHO_CENTRO	2
	
	//dimensiones de las filas que deben dejarse libres
	#define	FILAS_TOPE		5      
	#define	FILAS_BARRA	10
	
	//codificación de colores en las matrices de nivel
	#define	AMARILLO	'A'
	#define	NARANJA	'N'
	#define	ROJO	'R'
	#define	GRIS	'V'
	#define	VERDE	'G'
	#define	LIBRE	0
	
	//Resistencia de los ladrillos
	#define	RESIST_AMARILLO	1
	#define	RESIST_NARANJA	3
	#define	RESIST_ROJO	5
	//el gris se omite por ser un caso especial
	#define	RESIST_VERDE	1
	
	//valor de impactar los ladrillos
	#define	VALOR_AMARILLO	10
	#define	VALOR_NARANJA	30
	#define	VALOR_ROJO	50
	#define	VALOR_GRIS	0
	#define	VALOR_VERDE	25
	
	//Variables globales
		static int FILAS_LADRILLOS = ALTURA_MAXIMA - (FILAS_TOPE + FILAS_BARRA);	//
		int vidas;	//Vidas que le restan al jugador
		int  enfriamiento;	//límite de tiempo (en milisegundos) para cada nivel
		int   salto_total;	//millisegundos que deben transcurrir para que ocurra un nuevo moviniento
		int    tiempo;	//milisegundos que le restan al jugador actualmente
		int	    nivel;	//nivel actual
		int	    impactos_tope_max;	//número de impactos luego de los cuales se incrementa la velocidad.
		int    impactos_tope;
		float incremento_tope;	/* número por el cual debe ser multiplicado
			              	 * la velocidad cuando impactos_tope alcanza
			              	 * impactos_tope_max
			              	 */
		int puntuacion;
	
	//estructura para el almacenamiento de los niveles
	struct lista_niveles{
		int n;	//número total de niveles
		struct estructura_nivel** niveles;	//la lista en si: un arreglo de apuntadores
	}* niveles;
	
	//estructura para el almacenamiento de 1 nivel
	struct estructura_nivel{
		int numero_bloques;	//número de bloques que debe haber
		                   	//en la estructura siguiente.
		                   	//Almacenado para chequeo.
		char** disposicion;	//matriz n*m indicando cómo están colocados
		                   	//los ladrillos sobre el tablero
		                   	//done n = FILAS_LADRILLOS y m = ANCHO_MAXIMO
	};

	/*
	* Estructura que representa las dimensiones de un paralelepipedo
	*mis intenciones son tener una estructura que tenga
	*un centro y unas dimensiones.
	*porque los algoritmos que funcionan para calcular las colisiones requieren
	*que las figuras en cuestion tengan un "centro"
	*/
	struct parallelepiped { 
		        float large;//dim z
		        float high;//dim y
		        float length;//dim x
		        float x; //left face
		        float y; //bottom
				char color;
				int resistencia;
		        } paleta;
		        
	struct sphere {
			float radio;
			int slices;
			int stacks;
			
			double vel_x;	//velocidad de la pelota a lo largo del eje x
			double vel_y;	//velocidad de la pelota a lo largo del eje y
			double aceleracion; //aceleracion de la esfera
			
			float x, y; //center 
			
	} pelota;

#endif
