#include <assert.h>
#include <stdlib.h>	//malloc
#include <stdio.h> //printf en previewLadrillo
#include <limits.h> //INT_MAX
#include "valores_globales.h"
#include "error_services.h"
#include "ladrillo.h"
#include "pelota.h"


/*	Pone un ladrillo dado en la posición dada. Supone que la variable global "ladrillos"
 *	ya ha sido inicializada.
 *	Retorna el ladrillo creado, o NULL si ocurrió un problema con el malloc
 *	Guarda un mensaje de error en error_carga
 */
struct parallelepiped* crearLadrillo(char color, int row, int col){
	struct parallelepiped* ladrillo = (struct parallelepiped*)malloc(sizeof(struct parallelepiped));
	CHECKMALLOC(ladrillo, "cargar el ladrillo en el tablero");

	printf("creating %c at %d, %d\n", color, row, col);

	ladrillo->large = PROFUNDIDAD_LADRILLO;
	ladrillo->high = ALTURA_LADRILLO;
	ladrillo->length = ANCHO_LADRILLO;
	ladrillo->x = (float)(row * ANCHO_LADRILLO);
	ladrillo->y = (float)(-(col+1) * ALTURA_LADRILLO);
	ladrillo->color = color;

	//resistencia inicial
	switch(color){
		case AMARILLO:
			ladrillo->resistencia = RESIST_AMARILLO;
			break;
		case NARANJA:
			ladrillo->resistencia = RESIST_NARANJA;
			break;
		case ROJO:
			ladrillo->resistencia = RESIST_ROJO;
			break;
		case VERDE:
			ladrillo->resistencia = RESIST_VERDE;
			break;
		default:
			ladrillo->resistencia = INT_MAX;
			break;
	}
	return ladrillo;
}

/*	Libera el espacio actualmente ocupado por la lista de ladrillos
 */
void freeLadrillos(){
	int i;
	if(ladrillos == NULL)
		return;
	if(nivel > 0){
		i = niveles->niveles[nivel-1]->numero_bloques - 1;
		for( ; i>=0; i-- )
			free(ladrillos[i]);
	}
	free(ladrillos);
}

void previewLadrillo(struct parallelepiped* L);

/*	Carga en el tablero los ladrillos del nivel actual
 *	requiere que esté inicializado la variable global "niveles"
 *	Retorna 0 si ocurrió un error al hacer malloc; 1 en cualquier otro caso
 *	Guarda un mensaje de error en error_carga
 */
struct parallelepiped** crearLadrillos(){
	int row, col, brickNum=0;
	struct parallelepiped** output = (struct parallelepiped**)malloc(sizeof(struct parallelepiped*)*niveles->niveles[nivel]->numero_bloques);
	printf("malloc'ed: 0x%X\n", output);
	CHECKMALLOC(output, "cargar los ladrillos en el tablero");

	assert(niveles != NULL);

	for(row=0; row<FILAS_LADRILLOS; row++){
		for(col=0; col<ANCHO_MAXIMO; col++){
			if(niveles->niveles[nivel]->disposicion[row][col] != 0){
				output[brickNum] = crearLadrillo(niveles->niveles[nivel]->disposicion[row][col], row, col);
				if(output[brickNum] == NULL){
					error_carga = fail_safe_concat( createMsgOfTwo("Error al cargar el ladrillo en la fila %d, columna %d: ", row, col, ""), error_carga, error_carga);
					return NULL;
				}else
					printf("created ");
					previewLadrillo(output[brickNum]);
					printf("\n");
				brickNum++;
				assert(brickNum>= 0 && brickNum <= niveles->niveles[nivel]->numero_bloques);
			}
		}
	}
	assert(brickNum == niveles->niveles[nivel]->numero_bloques);
	return output;
}

void previewLadrillo(struct parallelepiped* L){
	printf(" [(%.0f, %.0f) %c HP:%d]", L->x, L->y, L->color, L->resistencia);
}

void previewLadrillos(){
	int i;
	assert(niveles != NULL);
	if(ladrillos == NULL)
		return;

	for( i = (niveles->niveles[nivel]->numero_bloques - 1); i>=0; i-- )
		previewLadrillo(ladrillos[i]);
	printf("\n");
}
//definir 4 planos y ver si colisionan con la pelota y pasarlo a la funcion colision_plano
//devuelve 1 si hay colision y 0 si no
/*
0 no choca con ningun plano
1 si choca con el plano 1 o el 2
2                       3 o el 4
3               la orilla entre el plano 1 y 3 o la orilla entre el plano 2 y 4
4               la orilla entre el plano 2 y 3 o la orilla entre el plano 1 y 4
*/
int detectarColisionPelotaLadrillo(struct parallelepiped* p, struct sphere* s)
{

	int a, b, c, d;
	struct plano* p1 = malloc(sizeof(struct plano));
	struct plano* p2 = malloc(sizeof(struct plano));
	struct plano* p3 = malloc(sizeof(struct plano));
	struct plano* p4 = malloc(sizeof(struct plano));
	if(p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL)
		fprintf(stderr, "memoria insuficiente para crear los planos del ladrillo\n");
	/*
	p1 cara de abajo de p
	p2 cara de arriba de p
	p3 cara l.derecha de p
	p4 cara l.izq de p
	*/
	p1->vertical = 0;
	p1->inf = p->x;
	p1->sup = p->x + p->length;
	p1->coord = p->y;

	
	p2->vertical = 0;
	p2->inf = p->x;
	p2->sup = p->x + p->length;
	p2->coord = p->y + p->high;

	
	p3->vertical = 1;
	p3->inf = p->y;
	p3->sup = p->y + p->high;
	p3->coord = p->x + p->length;

	p4->vertical = 1;
	p4->inf = p->y;
	p4->sup = p->y + p->high;
	p4->coord = p->x;
	
	a = colision_plano(s, p1);
	b = colision_plano(s, p2);
	c = colision_plano(s, p3);
	d = colision_plano(s, p4);

	if(a ==1 || b ==1) return 1;
	if(c ==1 || d ==1) return 2;
	if(b == -2 || d ==2 || a==2 ||c == -2) return 3;
	if(b==2 || c == 2 || a == -2 || d==-2) return 4;
	else return 0;



	free(p1);
	free(p2);
	free(p3);
	free(p4);

}

/*	Detecta si hubo una colisión entre la esfera dada y alguno de los ladrillos, y si la hay, reduce la resitencia de dicho ladrillo y cambia la dirección de la esfera de manera acorde.
 *	Parametros:
 *		s	la esfera que puede estar colisionando con los ladrillos
 *		ladrillos	lista de apuntadores a ladrillos que pueden estar colisionando con la pelota
 *		numero	cantidad de ladrillos en la lista
 *	retorna 1 si ocurrió colisión, 0 en cualquier otro caso
 */
int detectarColisionPelotaLadrillos(struct sphere* s, struct parallelepiped** ladrillos, int numero){
	int result, i;
	double angulo;
	for( i = (numero - 1); i>=0; i-- ){
		if(ladrillos[i]->resistencia > 0){
			result = detectarColisionPelotaLadrillo(ladrillos[i], s);
			if(result != 0){
				switch( result ){
					case 1:
						angulo = 0.0;
						break;
					case 2:
						angulo = PI/2;
						break;
					case 3:
						angulo = 3*PI/4;
						break;
					case 4:
						angulo = PI/4;
						break;
				}
				if( rebotar_pelota(s, angulo) == 0)
					perror("error en rebotar_pelota");
				//else everything worked okay

				switch(ladrillos[i]->color){
					case AMARILLO:
						ladrillos[i]->resistencia--;
						puntuacion += VALOR_AMARILLO;
						break;
					case NARANJA:
						ladrillos[i]->resistencia--;
						puntuacion += VALOR_NARANJA;
						break;
					case ROJO:
						ladrillos[i]->resistencia--;
						puntuacion += VALOR_ROJO;
						break;
					case GRIS:
						//no se descuenta resistencia
						puntuacion += VALOR_GRIS;
						break;
					case VERDE:
						ladrillos[i]->resistencia--;
						puntuacion += VALOR_VERDE;
						acelerar_pelota(s, 1.1);
						printf("Speed bounus!\n");
						break;
					default:break;
				}
				return 1;
			}
		}
	}
	return 0;
}