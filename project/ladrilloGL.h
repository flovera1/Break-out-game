#ifndef ladrilloGL
	#define ladrilloGL
	
	static double offset = 0.0;	//distancia desde la fila superior a partir de la cual se deben dibujar los ladrillos
	
	/*	dibuja "numero" los ladrillos en la variable global "ladrillos" en pantalla
	 */
	void dibujarLadrillos(struct parallelepiped** ladrillos, int numero);
#endif