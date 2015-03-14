#include <GL/glut.h>
#include <assert.h>
#include "valores_globales.h"
#include "ladrillo.h"
#include "basico.h"
#include "ladrilloGL.h"

/*	dibuja el paralelepípedo dado en pantalla
 */
void dibujarLadrillo(struct parallelepiped* L){
	float R, G, B;
	if(L->resistencia == 0)	//no se dibuja
		return;
	switch((int)L->color){
		case (int)AMARILLO:
			R = 1.0;
			G = 1.0;
			B = 0.0;
			break;
		case (int)NARANJA:
			R = 1.0;
			G = 0.5 + (RESIST_NARANJA - L->resistencia)*0.25;
			B = 0.0;
			break;
		case (int)ROJO:
			R = 1.0;
			G = 0.0 + (RESIST_ROJO - L->resistencia)*0.25;
			B = 0.0;
			break;
		case (int)GRIS:
			R = 0.5;
			G = 0.5;
			B = 0.5;
			break;
		case (int)VERDE:
			R = 0.0;
			G = 1.0;
			B = 0.0;
			break;
		default: //Rosado
			R = 1.0;
			G = 0.5;
			B = 0.5;
			break;
	}
	glPushMatrix();
		glScalef(1.0, 1.0, 1.0);
		glColor3f(R, G, B);
		drawParallelepiped(L);
	glPopMatrix();
}

/*	dibuja todos los ladrillos en la variable global "ladrillos" en pantalla
 */
void dibujarLadrillos(struct parallelepiped** ladrillos, int numero){
	int i;
	assert(ladrillos != NULL);
	for( i = (numero - 1); i>=0; i-- )
		dibujarLadrillo(ladrillos[i]);
}