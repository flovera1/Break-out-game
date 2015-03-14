#include <stdlib.h>	//malloc, calloc, EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>	//fopen, fscanf, fprintf
#include "valores_globales.h"
#include "cargarArchivo.h"
#include "error_services.h"

/*	método para cargar los ladrillos de un nivel */
int cargar_disposicion(FILE* f, int lvl, struct lista_niveles* niveles);

/*	Lee el archivo y carga sus parámetros en las variables globalles.
 *	Retorna la lista de los niveles del archivo, ó NULL si ocurrió
 *	un error durante la lectura del mismo. El mensaje de error
 *	es almacenado en "error_carga".
 */
struct lista_niveles* cargar_archivo(FILE* f){
	struct lista_niveles* output;
	int lvl, i;	//iteradores
	
	CHECKMISREAD( fscanf(f, "%d", &vidas), 1, \
		"error al leer el numero de vidas (se esperaba un entero)" );
	CHECKMISREAD( fscanf(f, "%d", &enfriamiento), 1, \
		"error al leer el tiempo de enfriamiento (se esperaba un entero)" );
	CHECKMISREAD( fscanf(f, "%d", &salto_total), 1, \
		"error al leer el tiempo de salto (se esperaba un entero)" );
	
	//inicializar estructura de salida
	output = malloc(sizeof(struct lista_niveles));
	CHECKMALLOC(output, "almacenar el numero de niveles");
	CHECKMISREAD( fscanf(f, "%d", &(output->n)), 1, \
		"error al leer el tiempo de salto (se esperaba un entero)" );
	
	CHECKMISREAD( fscanf(f, "%d %f", &impactos_tope, &incremento_tope), 2, \
		"error al leer el numero de impactos (se esperaba un entero y un decimal)" );
	incremento_tope /= 100; //convertir el porcentage en una proporción
	incremento_tope += 1;	//para ser un porcentage de aumento, debe ser >1
	
	//inicializar lista niveles
	output->niveles = malloc(sizeof(struct estructura_nivel)*output->n);
	CHECKMALLOC(output->niveles, "almacenar la lista de niveles");
	for(lvl=0; lvl<output->n; lvl++){
		output->niveles[lvl] = malloc(sizeof(struct estructura_nivel));
		CHECKMALLOC((output->niveles[lvl]), \
			createMsg("almacenar el nivel %d", i, "almacenar los niveles"));
		output->niveles[lvl]->disposicion = malloc(sizeof(int*)*FILAS_LADRILLOS);
		CHECKMALLOC((output->niveles[lvl])->disposicion, \
			createMsg("almacenar las filas del area de juego para el nivel %d", i, "almacenar las filas de los niveles"));
		for(i=0; i<FILAS_LADRILLOS; i++){
			output->niveles[lvl]->disposicion[i] = calloc(ANCHO_MAXIMO, sizeof(char));	//los valores en sí de la matriz se inicializan en cero
			CHECKMALLOC(output->niveles[lvl]->disposicion[i], \
				createMsg("almacenar las columnas del area de juego para el nivel %d", i, "almacenar las columnas de los niveles"));
		}
	}
	
	//cargar niveles
	for(lvl=0; lvl<output->n; lvl++){
		CHECKMISREAD( fscanf(f, "%d", &(output->niveles[lvl]->numero_bloques)), 1, \
			createMsg("error al leer el numero de ladrillos en el nivel %d (se esperaba un entero)", lvl+1, "error al leer el numero de ladrillos (se esperaba un entero)") );
		if(cargar_disposicion(f, lvl, output) == (int)NULL){
			//El mensaje de error ya está fijado
			return NULL;
		}
	}
	return output;
}

/*	Revisa que 0<=toCheck<top. Si toCheck excede está en estas fronteras,
 *	fija "error_carga" en "msg" y retorna 0.
 *	Nuevamente, esto uniformiza el manejo de errores.
 */
#define	CHECKBOUNDS(toCheck, top, msg)\
	if(toCheck<0){\
		error_carga = "No se permiten valores negativos";\
		return 0;\
	}\
	if(toCheck>=top){\
		error_carga = msg;\
		return 0;\
	}

/*	método para cargar los ladrillos de un nivel.
 *	Retorna NULL encaso de error (almacenando el error en error_carga)
 *	ó 1 en cualquier otro caso.
 */
int cargar_disposicion(FILE* f, int lvl, struct lista_niveles* niveles){
	int 	row, col, i;
	char	hue;
	for(i=0; i<niveles->niveles[lvl]->numero_bloques; i++){
		CHECKMISREAD( fscanf(f, "%d %d %c", &row, &col, &hue), 3, createMsgOfTwo("error al leer el %do ladrillo del nivel %d", i+1, lvl+1, "error leyendo ladrillo"));
		CHECKBOUNDS(row, FILAS_LADRILLOS, \
			createMsgOfTwo("Se ha detectado un ladrillo en la fila %d (no puede haber ladrillos debajo de la fila %d)", row, FILAS_LADRILLOS, "Se ha detectado un ladrillo entre la ultima hilera y la barra direccional"));
		CHECKBOUNDS(col, ANCHO_MAXIMO, \
			createMsgOfTwo("Se ha detectado un ladrillo en la columna %d (el area de juego es de %d columnas)", col, ANCHO_MAXIMO, "Se ha detectado un ladrillo fuera del area de juego"));
		niveles->niveles[lvl]->disposicion[row][col] = hue;
	}
	return 1;
}

//Atajo para mostrar en pantalla el valor de una variable entera
#define	REVEAL(var)	printf(#var " = %d\n", var)
//Atajo para mostrar en pantalla el valor de una variable con decimales
#define	REVEAL_F(var)	printf(#var " = %f\n", var)

void levelPreview(struct lista_niveles* niveles, int lvl);

/*Programa de prueba*
int main(int argc, char* argv[]){
	FILE* archivo;
	struct lista_niveles* niveles;
	int i;
	
	if(argc<2){
		fprintf(stderr, "Para probar este archivo, DEBE proporcionar un nombre de archivo como argumento\n");
		return EXIT_SUCCESS;
	}
	
	archivo = fopen(argv[1], "r");
	if( archivo == NULL ){
		fprintf(stderr, "No se pudo abrir el archivo \"%s\": ", argv[1]);
		perror("");
		return EXIT_FAILURE;
	}
	niveles = cargar_archivo(archivo);
	if( niveles == NULL ){
		fprintf(stderr, "No se pudo cargar el archivo \"%s\": %s\n", argv[1], error_carga);
		return EXIT_FAILURE;
	}else{
		printf("\"%s\" fue cargado exitosamente\n", argv[1]);
	}
	REVEAL(vidas);
	REVEAL(enfriamiento);
	REVEAL(salto_total);
	REVEAL(niveles->n);
	REVEAL(impactos_tope);
	REVEAL_F(incremento_tope);
	printf("\n");
	
	for(i=0; i<niveles->n; i++)
		levelPreview(niveles, i);

}*/

void levelPreview(struct lista_niveles* niveles, int lvl){
	int row, col;
	printf("\nN I V E L %d\n", lvl+1);
	for(row=0; row<FILAS_LADRILLOS; row++){
		for(col=0; col<ANCHO_MAXIMO; col++){
			if(niveles->niveles[lvl]->disposicion[row][col])
				printf(" %c", niveles->niveles[lvl]->disposicion[row][col]);
			else
				printf("  ");
		}
		printf("\n");
	}
}

