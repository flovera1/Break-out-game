/*	Define un plano paralelo al plano Y-Z ó al plano Z-X.
 */
 #ifndef __PLANO
 #define __PLANO
struct plano{
	int vertical;	//Si es cero, el plano es paralelo al eje Z-X.
	             	//Si es cualquier otro valor, el plano es paralelo al eje Y-Z
	double inf;	//frontera inferior del plano en el eje distinto de Z
	           	//al cual es paralelo (eje x para el plano Z-X;
	           	//eje y para el plano Y-Z)
	double sup;	//frontera superior del plano en el eje distinto de Z
	           	//al cual es paralelo 
	double coord;	//coordenada del plano en el eje al cual es
	             	//perpendicular el plano (eje y para el plano Z-X,
	             	//eje x para el plano Y-Z)
}plane;
#endif
