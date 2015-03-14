#include "timer.h"
#include "valores_globales.h"

SMTKU levelStart,
      lastStepDown;

//inicializa el contador interno
void startTimer(){
	CURR_TIME(levelStart);
	lastStepDown = levelStart;
}

/*	mueve todos los ladrillos de la lista hacia abajo
 *	una distancia ALTURA_LADRILLO.
 *	"numero" es la cantidad de ladrillos
 */
void stepDown(struct parallelepiped** ladrillos, int numero){
	int i=numero-1;
	for(; i>=0; i--)
		ladrillos[i]->y -= ALTURA_LADRILLO;
}

/*	verifica si se ha cumplido el tiempo de enfriamiento
 *	y, en caso afirmativo, mueve todos los ladrillos
 *	hacia abajo una distancia ALTURA_LADRILLO.
 *	
 *	Puede imprimir un mensaje de error si no logra obtener la hora del sistema.
 */
void revisarEnfriamiento(struct parallelepiped** ladrillos, int numero){
	if(ladrillos == NULL)
		return;
	if(secondsElapsed(levelStart)*1000 >= enfriamiento)
		if(SMTKU_EQUAL(levelStart, lastStepDown)	//aún no se había hecho el primer movimiento
		   	|| secondsElapsed(lastStepDown)*1000 >= salto_total
		  ){	
			CURR_TIME(lastStepDown);
			stepDown(ladrillos, numero);
		}
}
