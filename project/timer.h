#ifndef TIMINGSERVICE	//evitar que se sobreescriban los "define"s
	#define TIMINGSERVICE
	
	#include <stdio.h>	//fprintf, perror
	
	/*	División de dobles entre enteros
	 *	Reemplaza quot con el cociente y doub con el residuo
	 */
	#define	MOD(doub, integ, quot)\
		quot = doub/integ;\
		doub = doub - integ*quot
	
	/*	Formato para imprimir el tiempo en minutos y segundos redondeado al
	 *	segundo más cercano por debajo.
	 *	Utilícese en funciones donde la siguiente impresión debe esperar un
	 *	segundo o más.
	 */
	#define	SHORT_TIME_FMT	"%3d:%02.0f "
	
	/*	Define la cantidad de "backspaces" necesarios para borrar una impresión
	 *	en SHORT_TIME_FMT
	 */
	#define	SHORT_TIMErase	"\b\b\b""\b""\b\b""\b"
	
	/*	Formato para imprimir el tiempo en minutos y segundos con la máxima
	 *	precisión disponible.
	 *	La definición de SEC_PREC depende del sistema operativo
	 */
	#define	EXACT_TIME_FMT	"%3d:" SEC_PREC
	
	/*	Definición del tipo de datos SMTKU
	 *	(abreviatura de "Smallest Time-keeping Unit")
	 *	SEC_PREC (abreviado de Seconds' precision),
	 *	y EXACT_TIMErase (abreviado de "Exact time erase", la cantidad de
	 *	"backspaces" necesarios para borrar una impresión de EXACT_TIME_FMT)
	 */
	#if defined(_WIN32) || defined(WINDOWS)
		#include <Windows.h>	//DWORD
		#include <Winbase.h>	//GetTickCount()
		#define	SMTKU	DWORD	/*usando GetTickCount, lo más preciso que se
		       	     	     	 *puede medir el tiempo es en milisegundos
		       	     	     	 *(milésimas de segundo). Estos se almacenan
		       	     	     	 *en un "DWORD"
								 */
		#define	SEC_PREC	"%06.3f"
		#define	TIMErase	SHORT_TIMErase "\b""\b\b\b"
	#elif defined(__gnu_linux__) || defined(linux) || defined(UNIX)
		#include <sys/time.h>	//gettimeofday(struct timeval, NULL)
		#define	SMTKU	struct timeval	/*usando gettimeofday, lo más preciso
		       	     	              	 *que se puede medir el tiempo es en
		       	     	              	 *microsegundos (millonésimas de
		       	     	              	 *segundo). Estos se almacenan en un
		       	     	              	 *"struct timeval"
										 */
		#define	SEC_PREC      	"%09.6f"
		#define	EXACT_TIMErase	SHORT_TIMErase "\b""\b\b\b\b\b\b"
	#else
		#include <time.h>	//time(), time_t
		#define	SMTKU	time_t	//usando time, lo mas preciso que se puede
		       	     	      	//medir el tiempo es en segundos. Estos se
		       	     	      	//almacenan en un "time_t"
		       	     	      	//Nota: clock() es más preciso, pero no
		       	     	      	//funciona correctamente en todos los sistemas
		#define	SEC_PREC      	"%02.0f"
		#define	EXACT_TIMErase	SHORT_TIMErase
	#endif
	
	/*	Imprime MSG, seguido del tiempo transcurrido desde "since" en una
	 *	nueva línea, almacenando en "minutes" los minutos
	 *	transcurridos y en "seconds" los segundos transcurridos.
	 *	
	 *	Imprime por stderr para evitar que el sistema operativo
	 *	se quede esperando el fin de la línea que contiene el tiempo transcurido
	 */
	#define	printWithTime(MSG, since, minutes, seconds)\
		seconds = secondsElapsed(since);\
		MOD(seconds, 60, minutes);\
		fprintf(stderr, "\n" MSG "\n" SHORT_TIME_FMT, minutes, seconds)
	
	/*	Actualiza el tiempo impreso en pantalla, suponiendo que lo último
	 *	que se escribió fue el tiempo transcurrido desde "since"
	 *	en formato SHORT_TIME_FMT, almacenando en "minutes" los minutos
	 *	transcurridos y en "secondss" los segundos transcurridos.
	 */
	#define	retypeTime(since, minutes, seconds)\
		seconds = secondsElapsed(since);\
		MOD(seconds, 60, minutes);\
		fprintf(stderr, SHORT_TIMErase SHORT_TIME_FMT, minutes, seconds)
	
	/*	Mensaje estadard de error */
	#define	TIME_ERR	perror("No se pudo obtener la hora")
	
	/*	Almacena en x la representación de la fecha-hora actual del sistema
	 *	operativo bajo el cual se está compilando, suponiendo que "x" es de 
	 *	tipo SMTKU. Si no logra obtener la hora, imprime un error en pantalla.
	 *	Debido a que la representación es correlativa en Windows (medido desde
	 *	que se encendió la computadora), pero absoluta en Linux (medido desde
	*	el 1° de Enero de 1970), para mantener la uniformidad entre sistemas
	 *	operativos, esta llamada sólo debe usarse para propósitos de medición
	 *	de tiempo.
	 *	
	 *	Para obtener la fecha-hora real de forma confiable en todos los sistemas
	 *	operativos, por favor use asctime( localtime( time(NULL) ) ), todos
	 *	ubicados en time.h del estándar ANSI de C.
	 */
	#if defined(_WIN32) || defined(WINDOWS)
		#define	CURR_TIME(x)	x = GetTickCount()
	#elif defined(__gnu_linux__) || defined(linux)|| defined(UNIX)
		#define	CURR_TIME(x)\
			if(gettimeofday(&x, NULL))\
				TIME_ERR
	#else
		#define	CURR_TIME(x)\
			x = time(NULL);\
			if(x == -1)\
				TIME_ERR
	#endif
	
	/*	SMTKU_EQUAL(t1,t2) Compara dos SMTKU usando la forma apropiada para ese sistema operativo.
	 *	En sistemas no basados en linux, esto es equivalente a hacer t1 == t2.
	 */
	#if defined(__gnu_linux__) || defined(linux) || defined(UNIX)
		#define SMTKU_EQUAL(t1, t2) t1.tv_sec == t2.tv_sec && t1.tv_usec == t2.tv_usec
	#else
		#define SMTKU_EQUAL(t1, t2) t1 == t2
	#endif
	
	/*	Devuelve la cantidad de segundos que han pasado desde la fecha-hora dada,
	 *	o la fecha-hora actual si startTime es NULL.
	 *	Si el sistema no puede obtener la fecha-hora actual, devuelve 0.0
	 *	e imprime un error en pantalla
	 */
	double secondsElapsed(SMTKU startTime);
#endif