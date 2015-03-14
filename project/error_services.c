#include <stdlib.h>	//malloc
#include <string.h>	//strlen, strerror, strcat
#include <stdio.h>	//sprintf
#include <errno.h>	//errno
#include "error_services.h"

/*	Revisa si "errcode" indica un código de error en la lectura
 *	del archivo o si simplemente no se leyó la cantidad de elementos
 *	que se esperaban. En caso afirmativo, fija "error_carga" en "msg"
 *	y retorna 1. En caso contrario, retorna 0
 */
int checkMisread(int errcode, int expected, char* msg){
	if(errcode == EOF){
		error_carga = (char*)strerror(errno);
		return 1;
	}else if(errcode != expected){
		error_carga = msg;
		return 1;
	}else
		return 0;
}


/*	crea un mensaje de error reemplazando %d en "desired"
 *	con "number". Ya que esta función puede ser llamada cuando
 *	no hay memoria suficiente, se requiere un mensaje alternativo
 *	en caso de que ella no logre hacer "malloc".
 */
char* createMsg(const char* desired, int number, char* alternative){
	char* output = malloc(strlen(desired)+number/10);
	if(output != NULL){
		sprintf(output, desired, number);
		return output;
	}else{
		return alternative;
	}
}

/*	crea un mensaje de error reemplazando ambos %d en "desired"
 *	con "number1" y "number2" respectivamente.
 *	Ya que esta función puede ser llamada cuando
 *	no hay memoria suficiente, se requiere un mensaje alternativo
 *	en caso de que ella no logre hacer "malloc".
 */
char* createMsgOfTwo(const char* desired, int number1, int number2, char* alternative){
	char* output = malloc(strlen(desired)+number1/10+number2/10);
	if(output != NULL){
		sprintf(output, desired, number1, number2);
		return output;
	}else{
		return alternative;
	}
}

/*	Intenta concatenar dos char ("first" y "second").
 *	Ya que esto require un malloc, y este método es llamado desde
 *	CHECKMALLOC, requiere un mensade alternativo para dar en caso
 *	de que el malloc falle.
 */
char* fail_safe_concat(char* first, char* second, char* alternative){
	char* out = malloc(strlen(first)+strlen(second)+1);
	if(out == NULL){
		return alternative;
	}else{
		strcpy(out, first);
		strcat(out, second);
		return out;
	}
}

