/*
* BASICO.H
* Autores:
* 		Fernando Lovera
*		Fernando Torre
*Objetivo del codigo:
*		Representar graficamente todas las cosas representables(dibujables)
*		del juego
*
*/
#ifndef __BASICO
#define __BASICO
				
	#include <stdio.h>
	#include <math.h>
	#include <GL/glut.h>
	#include "valores_globales.h"
	float _angle;
	float _cameraAngle;

	/*
	*Firma:
	*	drawParallelepiped(struct parallelepiped* p)
	*Parametros de entrada: struct parallelepiped* p
	*					   donde p representa un paralelepipedo
	*Parametros de salida: void
	*funcionalidad:
	*Dibujar un paralelepipedo
	*/
	void drawParallelepiped(struct parallelepiped* p);
	/*
	*Firma:
	*	drawParallelepiped2(struct parallelepiped* p)
	*Parametros de entrada: struct parallelepiped* p
	*					   donde p representa un paralelepipedo
	*Parametros de salida: void
	*funcionalidad:
	*Dibujar un paralelepipedo de manera inocente
	*/
	void drawParallelepiped2(struct parallelepiped* p);
	/*
	*Firma:
	*	drawSphere(struct sphere* s)
	*Parametros de entrada: struct sphere* s
	*					   donde s representa una esfera
	*Parametros de salida: void
	*funcionalidad:
	*Dibujar una esfera, con un 'radio', 'stacks' y 'slices'
	*/
	void drawSphere(struct sphere* s);
#endif
