#ifndef _ladrillo
#define _ladrillo
	static struct parallelepiped** ladrillos = 0;	//lista de apuntadores a los ladrillos

	/*	Carga en el tablero los ladrillos del nivel actual
	 *	Retorna 0 si ocurrió un error al hacer malloc; 1 en cualquier otro caso
	 *	Guarda un mensaje de error en error_carga
	 *	
	 *	Para evitar fugas de memoria, debe llamarse a freeLadrillos antes
	 */
	struct parallelepiped** crearLadrillos();

	/*	Libera el espacio actualmente ocupado por la lista de ladrillos
	 */
	void freeLadrillos();

	/*	Imprime en pantalla la lista de ladrillos
	 */
	void previewLadrillos();

	/*	Detecta si hubo una colisión entre la esfera dada y alguno de los ladrillos, y si la hay, reduce la resitencia de dicho ladrillo y cambia la dirección de la esfera de manera acorde.
	 *	Parametros:
	 *		s	la esfera que puede estar colisionando con los ladrillos
	 *		ladrillos	lista de apuntadores a ladrillos que pueden estar colisionando con la pelota
	 *		numero	cantidad de ladrillos en la lista
	 *	retorna 1 si ocurrió colisión, 0 en cualquier otro caso
	 */
	int detectarColisionPelotaLadrillos(struct sphere* s, struct parallelepiped** ladrillos, int numero);
#endif
