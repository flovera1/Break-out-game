#ifndef error_services
	#define error_services
	
	/*	Cuando ocurre un error, el apuntador es cambiado por uno que
	 *	indique cuál fue el error
	 */
	static char* error_carga = "Error no detectado";

	/*	Revisa si "errcode" indica un código de error en la lectura
	 *	del archivo o si simplemente no se leyó la cantidad de elementos
	 *	que se esperaban. En caso afirmativo, fija "error_carga" en "msg"
	 *	y retorna 1. En caso contrario, retorna 0
	 */
	int checkMisread(int errcode, int expected, char* msg);

	//atajo para uniformizar la acción a tomar cuando ocurre un error
	#define CHECKMISREAD(errcode, expected, msg)\
		if(checkMisread(errcode, expected, msg))\
			return NULL

	/*	crea un mensaje de error reemplazando %d en "desired"
	 *	con "number". Ya que esta función puede ser llamada cuando
	 *	no hay memoria suficiente, se requiere un mensaje alternativo
	 *	en caso de que ella no logre hacer "malloc".
	 */
	char* createMsg(const char* desired, int number, char* alternative);

	/*	crea un mensaje de error reemplazando ambos %d en "desired"
	 *	con "number1" y "number2" respectivamente.
	 *	Ya que esta función puede ser llamada cuando
	 *	no hay memoria suficiente, se requiere un mensaje alternativo
	 *	en caso de que ella no logre hacer "malloc".
	 */
	char* createMsgOfTwo(const char* desired, int number1, int number2, char* alternative);

	/*	Intenta concatenar dos char ("first" y "second").
	 *	Ya que esto require un malloc, y este método es llamado desde
	 *	CHECKMALLOC, requiere un mensade alternativo para dar en caso
	 *	de que el malloc falle.
	 */
	char* fail_safe_concat(char* first, char* second, char* alternative);

	/*	Revisa si a ptr le fue asignado memoria correctamente, y en caso
	 *	contrario fija "error_carga" y retorna NULL. "msg" debe ser una
	 *	descripción de para qué se estaba pidiendo esta memoria, ya que
	 *	se utiliza para fijar este mensaje de error.
	 */
	#define	CHECKMALLOC(ptr, msg)\
		if(ptr == NULL){\
			error_carga = fail_safe_concat("Memoria insuficiente para ", msg, "Memoria insuficiente");\
			return NULL;\
		}

#endif