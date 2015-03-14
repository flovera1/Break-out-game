#ifndef __cargarArchivo
	#define __cargarArchivo

	/*	Lee el archivo y carga sus parámetros en las variables globalles.
	 *	Retorna la lista de los niveles del archivo, ó NULL si ocurrió
	 *	un error durante la lectura del mismo. El mensaje de error
	 *	es almacenado en "error_carga".
	 */
	struct lista_niveles* cargar_archivo(FILE* f);

	void levelPreview(struct lista_niveles* niveles, int lvl);
#endif
