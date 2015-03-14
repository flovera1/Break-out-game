#ifndef _enfriamiento
	#define _enfriamiento
	
	//inicializa el contador interno
	void startTimer();

	/*	verifica si se ha cumplido el tiempo de enfriamiento
	 *	y, en caso afirmativo, mueve todos los ladrillos
	 *	hacia abajo una distancia ALTURA_LADRILLO.
	 *	
	 *	Puede imprimir un mensaje de error si no logra obtener la hora del sistema.
	 */
	void revisarEnfriamiento(struct parallelepiped** ladrillos, int numero);
#endif