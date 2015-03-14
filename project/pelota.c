//importar declaraciones de funciones y especificación de la esfera
#include "pelota.h"

//importar funciones compartidas con básico.c
//#include "basico.h"

//importar funciones matemáticas
#include <math.h>//sin, cos, tan, abs
#include <errno.h>//deteccion de errores matematicos

#define epsilon 0.01 //definición de error aceptable

/*	verifica si la pelota dada ha entrado en colisión con el plano dado
 *	devuelve 0 si no lo ha hecho, 1 si lo ha hecho en el centro del plano, 
 *	2 si lo ha hecho chocando contra la esquina superior, y -2 si lo ha hecho 
 *	chocando contra la esquina inferior 
 */
int colision_plano(struct sphere* p, struct plano* x){
	double plane_line;	//coordenada de p que debe coincidir con x->coord
	double plane_perpend;	//coordenada de p que debe estar entre x->inf y x->sup
	
	if(x->vertical){
		plane_line = p->x + p->vel_x;	//anticipar colisiones a ocurrir entre esta llamada y la próxima
		plane_perpend = p->y;
	}else{
		plane_line = p->y + p->vel_y/2;
		plane_perpend = p->x;
	}
	
	if(abs( (plane_line - x->coord)*1 ) - epsilon <= p->radio){
		//está en línea con el plano
		if(plane_perpend+epsilon > x->inf && plane_perpend-epsilon < x->sup){
			//está en contacto con el plano
			if(plane_perpend-epsilon < x->inf){
				//está en contacto con la esquina inferior del plano
				return -2;
			}else if(plane_perpend+epsilon > x->sup){
				//está en contacto con la esquina superior
				return 2;
			}else{
				return 1;
			}
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

/*	Actualiza la posición de la pelota dada, suponieno que no ha
 *	actuado sobre ella una fuerza exterior
 */
void mover_pelota(struct sphere* p){
	p->x += p->vel_x;
	p->y += p->vel_y;
}

/*	Cambia la velocidad de la pelota, multiplicándola por el escalar dado
 */
void acelerar_pelota(struct sphere* p, double aceleracion){
	p->vel_x *= aceleracion;
	p->vel_y *= aceleracion;
	fprintf(stderr,"*%.2f", p->vel_y);
}

unsigned long int next = 1;

/*
genera numeros aleatorios
utilizando el algirtmo de Kernighan y Ritchie
*/
int generar_random(void)
{
	next = next*1103515245 + 12345;
	return (unsigned int)(next/65536)%32768;
}
	




/*	Cambia la dirección de la pelota, suponiendo que acaba de
 *	colisionar contra una superficie a "r" radianes anti-horario
 *	del eje x.
 *	"r" debe estar en el intervalo [0, PI)
 *	Devuelve 0 si ocurrió un error de calculo (el cual se guarda en errno), 1 en cualquier otro caso
 */
int rebotar_pelota(struct sphere* p, double r){
	double rapidez, anguloAct, anguloChoque, anguloNew;
	if(r+epsilon < 0 || r-epsilon > PI){
		errno = EDOM;
		return 0;
	}

	//short-circuiting por eficiencia
	if(r-epsilon <= 0 || r+epsilon > PI){
		//la superficie es horizontal
		p->vel_y *= -1;
		return 1;
	}
	if(r-epsilon <= PI/2 && r+epsilon >= PI/2){
		//la superficie es vertical
		p->vel_x *= -1;
		return 1;
	}
	
	//Si llegamos hasta aqui, la superficie tiene un ángulo no-notable
	
	//calcular magnitud del vector velocidad
	rapidez = sqrt( pow(p->vel_x,2)+pow(p->vel_y,2) );
	
	//calcular ángulo de la trayectoria actual con respecto al eje x
	if(p->vel_x - epsilon <= 0 && p->vel_x + epsilon >= 0)	//evitar división entre cero
		anguloAct = PI/2;
	else
		anguloAct = atan( p->vel_y / p->vel_x );	//Dom(atan) entre -infinito y +infinito; no hay error EDOM. Resultado entre -PI/2 y PI/2; no puede haber error ERANG
	


	//calcular ángulo de la trayectoria nueva con respecto al eje y
	if(r-epsilon > PI/2){
		anguloNew = -(anguloAct - 2*r) + PI/2;
	}else{
		anguloChoque = PI/4 + r - anguloAct;
		anguloNew = PI/2 - (anguloChoque + anguloAct);
	}
	//AnguloNew entre -PI/2 y 4*PI, lo cual está en el dominio de sin y cos

	p->vel_x = rapidez * sin(anguloNew);	//sin entre -1 y 1; no puede haber error ERANG
	p->vel_y = rapidez * cos(anguloNew);	//cos entre -1 y 1; no puede haber error ERANG
	printf("%3.1f grados ==> %3.1f grados, vel_x = %.3f, vel_y = %.3f", anguloAct*180/PI, anguloNew*180/PI, p->vel_x, p->vel_y);
	return 1;
}

/*	Inicializa la velocidad de la pelota "p", habiendo sido lanzada
 *	con el ángulo "a" dado (radianes con respecto al eje y)
 *	y la rapidez "s" dada
 *	"a" debe estar en el intervalo (-PI/2, PI/2)
 *	Devuelve 0 si no lo está y fija errno en EDOM
 *	Devuelve 1 en cualquier otro caso
 */
int lanzar_pelota(struct sphere* p, double a, double s){
	if(a+epsilon < -PI/2 || a-epsilon > PI/2){
		errno = EDOM;
		return 0;
	}
	//los vectores vel_x y vel_y forman un triángulo de hipotenusa = s
	p->vel_x = s * sin(a);
	p->vel_y = s * cos(a);
	return 1;
}


/*
int main(int argc, char **argv)
{
	glutInit(&argc,argv); //Solo necesario en Linux
	glutInitWindowSize (500, 500); 
        glutInitWindowPosition (10, 50);
	glutCreateWindow ("ventana");	
	
    	pelota.vel_x = 0.05f;
    	pelota.vel_y = 0.05f;
    	pelota.aceleracion=1;
	pelota.x =-1.0f;
    	pelota.y=2.0f;
    	pelota.radio=0.5f;
    	pelota.slices=500;
    	pelota.stacks=500;

	paleta.large = 2.0;
	paleta.high = 2.0;
	paleta.length = 5.0;
	paleta.x = 3;
	paleta.y = 3;	        
	
        glutReshapeFunc(reproject);//lo que hace es agarrar y definir que pasa cuando se cambia la ventana de vista
       	glutDisplayFunc(drawScene);
       	
	glutTimerFunc(ms, Advance, 1);
	glutKeyboardFunc(handleKeypress);
	
	glutMainLoop();
    
return 0;
}*/
