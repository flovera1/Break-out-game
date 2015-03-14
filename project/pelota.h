/*
* PELOTA.H
* Autores:
* 		Fernando Lovera
*		Fernando Torre
*Objetivo del codigo:
*		Representar todos los movimientos de la pelota. Y cosas relativas a la pelota
*
*/

#ifndef __PELOTA
#define __PELOTA
	#include <stdio.h>
	#include <GL/glut.h>
	#include "valores_globales.h"
	#include "plano.h"
	
	/*	Firma: int colision_plano(struct pelota* p, struct plano* x)
	 *	Parámetros de entrada:
	 *		p	la esfera para la cual se debe revisar si colisionó contra el plano
	 *		x	plano contra el cual puede haber colisionado p
	 *	Parámetros de salida: int
	 *		0	si no hay colisión
	 *		1	si la colisión ocurrió en el centro del plano
	 *		2	si la colssión ocurrió en la esquina superior del plano
	 *		-2	si la colisión ocurrió en la esquina inferior del plano
	 *	Funcionalidad:
	 *		verifica si la pelota dada ha entrado en colisión con el plano dado
	 */
	int colision_plano(struct sphere* p, struct plano* x);
	
	/*
	*Firma: mover_pelota(struct sphere* p)
	*Parametros de entrada: esfera p
	*Parametros de salida: Void
	*Funcionalidad:
	*	Actualiza las coordenadas almacenada de la pelota dada,
	*	suponiendo que no ha actuado sobre ella una fuerza exterior
	*/
	void mover_pelota(struct sphere* p);
	
	/*
	*Firma: acelerar_pelota(struct sphere* p, double aceleracion)
	*Parametros de entrada: esfera p, double aceleracion
	*					se le cambia la aceleracion a la esfera
	*Parametros de salida: Void
	*Funcionalidad:
	* 	setea la aceleracion de una esfera, multiplicando su velocidad
	* 	por el escalar "aceleracion" dado
	*/
	void acelerar_pelota(struct sphere* p, double aceleracion);

	/*
	*Firma: rebotar_pelota(struct sphere* p, double g)
	*Parametros de entrada: esfera p, double g
	*					
	*Parametros de salida: int
	*		0 si ocurrió un error de calculo
	*		1 en culaquier otro caso
	*Funcionalidad:
	*Cambia la dirección de la pelota, suponiendo que acaba de
	*	colisionar contra una superficie a "r" radianes anti-horario
	*	del eje x.
	*	"r" debe estar en el intervalo [0, PI)
	*	Devuelve 0 si ocurrió un error de calculo (el cual se guarda en errno), 1 en cualquier otro caso
	*/
	int rebotar_pelota(struct sphere* p, double g);
				
	/* Firma: int lanzar_pelota(struct sphere* p, double a, double s)
	 *Parámetros de entrada:
	 *		p	esfera a la que se le debe inicializar el vector velocidad
	 *		a	ángulo en radianes con respecto al eje y con el cual se debe lanzar p. "a" debe estar en el intervalo (-PI/2, PI/2)
	 *		s	rapidez deseada para el vector velocidad de p
	 *	Parámetros de salida: int
	 *		0	si "a" no está en el intervalo (-PI/2, PI/2)
	 *		1	en cualquier otro caso
	 *	Funcionalidad:
	 *		Inicializa la velocidad de la pelota "p"
	 *		con el ángulo "a" dado. Si "a" no está en (-PI/2, PI/2),
	 *		devuelve 0 y fija errno en EDOM.
	*/
	int lanzar_pelota(struct sphere* p, double a, double s);

	/*
	genera numeros aleatorios
	utilizando el algirtmo de Kernighan y Ritchie
	*/
	int  generar_random(void);
#endif
