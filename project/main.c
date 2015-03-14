#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <GLUT/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>	//sqrt

#include "valores_globales.h" 
#include "pelota.h"
#include "basico.h"
#include "cargarArchivo.h"
#include "error_services.h"
#include "ladrillo.h"
#include "ladrilloGL.h"
#include "enfriamiento.h"

#define CINCO 5*PI/180	//5° en radianes

//constantes usadas para posicionar la cámara
const int ancho_campo = (ANCHO_MAXIMO+1)*ANCHO_LADRILLO,
          alto_campo  = ALTURA_MAXIMA*ALTURA_LADRILLO,
          
          camera_x =  ((ANCHO_MAXIMO+1)*ANCHO_LADRILLO)/2,
          camera_y = -ALTURA_MAXIMA*ALTURA_LADRILLO-5,
          camera_z =  20,
          
          center_x =  (ANCHO_MAXIMO+1)*ANCHO_LADRILLO/2,
          center_y = -ALTURA_MAXIMA*ALTURA_LADRILLO/2,
          center_z =  0;

int nextCollisionCheck;
static float centro = 1.5;	//ancho del centro de la paleta
static char msg[32] = {0};	//mensaje impreso en pantalla, actualizado con cada llamada a "drawscene"

struct plano planoA;	//superficie de choque de la paleta
//fronteras
struct plano izq,	//plano ZY Izquierdo
             tras,	//plano YX Fondo
             der,	//plano ZY Derecho
             sup,	//plano ZX Superior
             final;	//plano ZX Inferior ( Plano imaginario ¡NO DEBE SER DIBUJADO! )
#define anchoPlanos	10;	//ancho de los planos en Z

static int cable = 0,	//redirige el flujo del programa dependiendo de si el nivel ha empezado o no
           ceilingCollisions=0;	//indica cuántas colisiones han ocurrido con el plano superior. Resetado al chocar con la paleta.

#define SPEED_LIMIT 0.75	//Velocidad máxima que puede alcanzar la pelota

void Advance() {
	mover_pelota(&pelota);
/*	if(cable != 0){
		pelota.vel_y += pelota.aceleracion;
	}*/
	glutPostRedisplay();
}

void crearPlanosBordes(){
	izq.vertical = 1;
	izq.inf = -alto_campo;
	izq.sup = 0.0;
	izq.coord = 0.0;
	
	der.vertical = 1;
	der.inf = -alto_campo;
	der.sup = 0.0;
	der.coord = ancho_campo;
	
	sup.vertical = 0.0;
	sup.inf = 0.0;
	sup.sup = ancho_campo;
	sup.coord = 0.0;

	final.vertical = 0;
	final.inf = 0.0;
	final.sup = ancho_campo;
	final.coord = -alto_campo;
	
	tras.vertical = 2;
	tras.inf = 0.0;
	tras.sup = ancho_campo;
	tras.coord = 0.0;
}

#define ALTO_PARED 10
void dibujarPlano(struct plano p){
	switch(p.vertical){
		case 0:	//plano paralelo a XZ
			glBegin(GL_QUADS);
				glVertex3f(p.inf, p.coord, 0.0);
				glVertex3f(p.sup, p.coord, 0.0);
				glVertex3f(p.sup, p.coord, ALTO_PARED);
				glVertex3f(p.inf, p.coord, ALTO_PARED);
			glEnd();
			break;
		case 1:	//plano paralelo a YZ
			glBegin(GL_QUADS);
				glVertex3f(p.coord, p.inf, 0.0);
				glVertex3f(p.coord, p.sup, 0.0);
				glVertex3f(p.coord, p.sup, ALTO_PARED);
				glVertex3f(p.coord, p.inf, ALTO_PARED);
			glEnd();
			break;
		default:	//plano paralelo a XY
			break;
	}
}

/*
*Esta funcion dibuja los tres planos que se deben ver en la parte inicial de cada nivel.
*Son planos 
*/
void dibujarPlanosBordes()
{
	dibujarPlano(sup);
	dibujarPlano(izq);
	dibujarPlano(der);

		//plano 1. plano zy
		/*struct plano plano1 = malloc(sizeof(struct plano));
		plano1.vertical = 1;	
		plano1.inf = 0.0;
		plano1.sup = 25.0;
		plano1.coord = 0.0;
		*
		//dibujar el plano1
		
		glBegin(GL_QUADS);
			glVertex3f(-5.0, 12.5, 0.0);
			glVertex3f(5.0, 12.5, 10.0);
			glVertex3f(0.0, 25.0, 10.0);
			glVertex3f(0.0, 25.0, 0.0);
		glEnd();
		//plano 2
		/*struct plano plano2 = malloc(sizeof(struct plano));
		plano1.vertical = 1;	
		plano1.inf = 0.0;
		plano1.sup = 25.0;
		plano1.coord = 0.0;
		*
		//dibujar plano2
		glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(10.0, 25.0, 0.0);
			glVertex3f(0.0, 25.0, 0.0);
			glVertex3f(10.0, 0.0, 0.0);
		glEnd();
		//plano 3		
		/*struct plano plano3 = malloc(sizeof(struct plano));
		plano1.vertical = 1;	
		plano1.inf = 0.0;
		plano1.sup = 25.0;
		plano1.coord = 0.0;
		*
		glBegin(GL_QUADS);
			glVertex3f(10.0, 0.0, 0.0);
			glVertex3f(10.0, 0.0, 25.0);
			glVertex3f(10.0, 25.0, 0.0);
			glVertex3f(10.0, 25.0, 25.0);
		glEnd();
		*/
}


/*funcion que retorna si un plano colisiono con un ladrillo!! no con una esfera.
*devuelve 1 si si colisiona
*0 si no
*/
int colision_plano_final(struct parallelepiped* p){
	if(p->y <= final.coord) return 1;
	else return 0;
}

/*tengo que dibujar un plano "imaginario" que represente la linea final
    y ver si la pelota colisiona con ese plano.
    La funcion devuelve 1 si colisiona con el plano
    0 si no
*/
int ladrilloEnLineaFinal(struct parallelepiped** p){
	int retorno=0;
	int i;

	if(niveles == NULL)
		return 0;	//se cumple por vacuidad

	//iteracion sobre los ladrillos del nivel!
	for(i = 0; i < niveles->niveles[nivel]->numero_bloques; i++){
			if(colision_plano_final(p[i]) != 0)
			{
				vidas = vidas -1;// tengo que reducir el numero de vidas cuando llego al final
				retorno = 1;
				break;
			}
	}
		return retorno;
}

/*
condiciones finales son:
1) numero de vidas es 0
	1.1) pierdo una vida cuando la pelota choca con la linea final
2) el ladrillo alcanza la linea final(detectar cuando alcanza la linea final)
3) El juego se considera vencido cuando el numero total de niveles ha sido alcanzado
	3.1) Un nivel se considera finalizado cuando todos los bloques factibles
	       son eliminados
La funcion devuelve 0 cuando las condiciones finales NO son verdaderas(no se llega al fin) o 1 cuando las condiciones finales se alcanzan(se llega al fin)
*/
int condiciones_finales(struct parallelepiped** p )
{	
	if(vidas == 0 || ladrilloEnLineaFinal(p) || (niveles != 0 && niveles->n == nivel) )
	   		{
	   		return 1;
	   		}
	else
		return 0;
}

//coloca la pelota sobre la paleta
void resetBall(){
	cable = 0;
	pelota.x = paleta.x + paleta.length/2;
	pelota.y = paleta.y + paleta.high + pelota.radio*2;
	pelota.vel_x = 0.0;
	pelota.vel_y = 0.0;
}

//resetea varios valores
void startLevel(){
	resetBall();
	if(niveles != 0){
		printf("loading bricks\n");
		ladrillos = crearLadrillos();
		if(ladrillos == 0)
			fprintf(stderr, "No se pudieron colocar los ladrillos en el tablero: %s\n", error_carga);
		else
			printf("bricks loaded successfully\n");
	}
	startTimer();
	printf("timer has been started\n");
}

/*	Verifica si se han cumplido las condiciones necesarias para cambiar el nivel y, en caso afirmativo, lo cambia.
 */
void checkLevelChange(){
	int flag = 1,
	    i;
	if(niveles == NULL)
		return;

	for(i=niveles->niveles[nivel]->numero_bloques-1; i >=0 ; i--)
		flag = flag && (ladrillos[i]->color == GRIS || ladrillos[i]->resistencia <= 0);
	if(flag){
		nivel++;
		if(nivel < niveles->n){
			levelPreview(niveles, nivel);
			startLevel();
		}else
			printf("HAS GANADO!\n Puntuacion final: %d puntos", puntuacion);
			exit(0);
	}
}

void drawScene(){
	double dist, angulo;
	int collision_type, i;
	
	glMatrixMode(GL_MODELVIEW);	//ADDED
	glLoadIdentity();	//ADDED
	gluLookAt(camera_x, camera_y, camera_z, center_x, center_y, center_z, 0.0, 0.0, 1.0);	//ADDED
	if(niveles != NULL)
		revisarEnfriamiento(ladrillos, niveles->niveles[nivel]->numero_bloques);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
	//glLoadIdentity();
	//glTranslatef(0.0,4.0,0.0);	
	

	glPushMatrix();
		glScalef(1.0, 1.0, 1.0);
		glColor3f(.6, .6, .7);
		dibujarPlanosBordes();
	glPopMatrix();

	if(ladrillos != NULL){
		dibujarLadrillos(ladrillos, niveles->niveles[nivel]->numero_bloques);
	}
	
	//actualizar superficie de choque de la paleta
	planoA.inf = paleta.x;
	planoA.sup = paleta.x + paleta.length;

	glPushMatrix();
		
		if(cable != 0 && nextCollisionCheck <= 0){
			//verificar colisión con la paleta
			collision_type = colision_plano(&pelota, &planoA);

			if(collision_type != 0){
				nextCollisionCheck = abs(1/pelota.vel_y);
				printf("\ncolision %d: ", collision_type);
				switch(collision_type){
					case 1:
						dist = ( pelota.x - (paleta.x + (paleta.length/2)) );	//distancia de la pelota desde el centro de la paleta
						printf("dist = %.2f ",dist);

						if(dist < -centro/2){
							//caso mitad izquierda de la paleta
							angulo = PI-CINCO; //PI+(dist/paleta.length)*(PI/4);
							//angulo = PI-PI/9;
						}else if(dist > centro/2){
							//caso mitad derecha de la paleta
							angulo = CINCO;//(dist/paleta.length)*(PI/4);
							//angulo = PI/9;
						}else{
							//caso centro de la pelota
							angulo = 0.0;
						}
						break;
					case 2:
						angulo = PI/4;
						break;
					case -2:
						angulo = 3*PI/4;
						break;
				}
				if( rebotar_pelota(&pelota, angulo) == 0)
					perror("error en rebotar_pelota");
				//else everything worked okay
				
				//evitar que la pelota atraviese la paleta
				if(pelota.vel_y<0)
					pelota.vel_y *= -1;
				
				//printf("(next check in %d)\n",nextCollisionCheck);
				ceilingCollisions = 0;
			}else{
				//verificar si colisionó contra algún ladrillo
				if( ladrillos != NULL && detectarColisionPelotaLadrillos(&pelota, ladrillos, niveles->niveles[nivel]->numero_bloques) ){
					nextCollisionCheck = abs(1/pelota.vel_y);
					printf("Colision con ladrillo\n");//"(next check in %d)\n",nextCollisionCheck);
					checkLevelChange();
				}else{
					//verificar si colisionó contra alguna frontera
					collision_type = colision_plano(&pelota, &izq);
					if( collision_type != 0){
						printf("LEFT WALL collision type %d\n", collision_type);
						switch(collision_type){
							case 1:
								angulo = PI/2;
								break;
							default:
								angulo = PI/4;
								break;
						}
					}else{
						collision_type = colision_plano(&pelota, &der);
						if( collision_type != 0){
							printf("RIGHT WALL collision type %d\n", collision_type);
							switch(collision_type){
								case 1:
									angulo = PI/2;
									break;
								default:
									angulo = 3*PI/4;
									break;
							}
						}else{
							collision_type = colision_plano(&pelota, &sup);
							if( collision_type != 0){
								printf("CEILING collision type %d, %dth in a row\n", collision_type, ceilingCollisions+1);
								switch(collision_type){
									case 1:
										angulo = 0;
										break;
									case 2:
										angulo = 3*PI/4;
										break;
									case -2:
										angulo = PI/4;
										break;
								}
								ceilingCollisions++;
								if(ceilingCollisions >= impactos_tope_max){
									acelerar_pelota(&pelota, incremento_tope);
									ceilingCollisions = 0;
									printf("\nSpeed up!\n");
								}
							}else{
								collision_type = colision_plano(&pelota, &final);
								if( collision_type != 0){
									vidas--;
									printf("\nVidas restantes: %d\n", vidas);
									angulo = -1;	//flag value
									resetBall();
								}
							}
						}
					}
					
					if(collision_type != 0 && angulo != -1){	//debe haber ocurrido una colisión con una frontera distinta a la inferior
						//colisionó contra alguna de las paredes
						if( rebotar_pelota(&pelota, angulo) == 0)
							perror("error en rebotar_pelota");
						//else everything worked okay

						//evitar hiperaceleración de la pelota
						if( pelota.vel_x > SPEED_LIMIT)
							pelota.vel_x = SPEED_LIMIT;
						if( pelota.vel_y > SPEED_LIMIT)
							pelota.vel_y = SPEED_LIMIT;

						nextCollisionCheck = abs(1/pelota.vel_y);
					}else{
						//no ocurrió colisión de ningún tipo
						nextCollisionCheck--;
					}
				}
			}
		}else{
			//no ocurrió colisión de ningún tipo
			nextCollisionCheck--;
		}

		//verificar si la pelota saltó la talanquera y volverla a poner por el buen camino en ese caso
		if(pelota.x < 0){
			pelota.x = pelota.radio;
			if(pelota.vel_x < 0)
				pelota.vel_x *= -1;
		}
		if(pelota.x > ancho_campo){
			pelota.x = ancho_campo-pelota.radio;
			if(pelota.vel_x > 0)
				pelota.vel_x *= -1;
		}
		if(pelota.y > 0){
			pelota.y = pelota.radio;
			if(pelota.vel_y > 0)
				pelota.vel_y *= -1;
		}
		if(pelota.y < -alto_campo){
			pelota.y = -ancho_campo+pelota.radio;
			if(pelota.vel_y < 0)
				pelota.vel_y *= -1;
		}

		//borrar posición anterior
		for(i=strlen(&msg); i>0; i--)
			fprintf(stderr, "\b");
		//imprimir posición actual
		sprintf(&msg, "%d puntos (%05.2f,%05.2f)", puntuacion, pelota.x, pelota.y);

		fprintf(stderr, &msg);
		
		glScalef(1.0,1.0,1.0);		
		drawSphere(&pelota);
	glPopMatrix();
	
	if(ladrillos != NULL && condiciones_finales(ladrillos)){
		printf("\nFIN DEL JUEGO\n");
		exit(0);
	}

	glPushMatrix();
		glScalef(1.0, 1.0, 1.0);
		glColor3f(1.0, 0.5, 0.5);
		drawParallelepiped(&paleta);
	glPopMatrix();
	glLoadIdentity();
	
	//gluLookAt(camera_x, camera_y, camera_z, center_x, center_y, center_z, 0.0, 0.0, 1.0);	//DELETED

	glutSwapBuffers();
}

#define VEL_PALETA 0.9
void handleKeypress(int key, int x, int y) {  
	
	switch (key) {
		case GLUT_KEY_LEFT: 
			if(paleta.x + paleta.length/2 > 0){
				paleta.x = paleta.x -(abs(pelota.vel_x)+VEL_PALETA);
				if(cable == 0)
					pelota.x -= (abs(pelota.vel_x)+VEL_PALETA);
			}else{
				printf("Ya estas en el borde del campo\n");
			}
			break;
		case GLUT_KEY_RIGHT:
			if(paleta.x + paleta.length/2 < ancho_campo){
				paleta.x = paleta.x + (abs(pelota.vel_x)+VEL_PALETA);
				if(cable == 0)
					pelota.x += (abs(pelota.vel_x)+VEL_PALETA);
			}else{
				printf("Ya estas en el borde del campo\n");
			}
			break;
		default:
			printf("no puedes presionar esa tecla \n");
	}
}

void  startPlaying(unsigned char key, int x, int y)
{

	double angulo1 = 0.0;
	if(cable == 0){
		angulo1 =(double)(-5 +  generar_random()%10)*PI / 180;
	}

	switch(key){
		case ' ':
			printf("presiona la letra a");
			if (cable == 0){
				lanzar_pelota(&pelota, angulo1, .1);
				cable++;
				nextCollisionCheck = 1;
			}
			break;
		default:
			printf("hola, presiona la barra espaciadora para empezar");
	}
}

#define ANCHO 10
void reproject (int w, int h) {
	
	float aspectratio;
	if (h==0)
		h=1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectratio = (float) w / (float) h;
	gluPerspective(15.0f, aspectratio, 1.0, camera_z*5);	//ADDED
	if (w<=h) {
		glOrtho(-ANCHO, ANCHO, -ANCHO, ANCHO, 1.0, -1.0);
	}else{
		glOrtho(-ANCHO, ANCHO, -ANCHO, ANCHO, 1.0, -1.0);
	}
	//glMatrixMode(GL_MODELVIEW);	//DELETED
}

int main(int argc, char **argv)
{
	FILE* archivo;
	const int viewport_scale = 10;
	_angle = 30.0f;
	_cameraAngle = 10.0f;
	
	crearPlanosBordes();
	
	nivel = 0;

	//definir paleta
	paleta.large = PROFUNDIDAD_LADRILLO;
	paleta.high = ALTURA_LADRILLO;
	paleta.length = ANCHO_LADRILLO*1.75;
	paleta.x = ancho_campo/2 - paleta.length;
	paleta.y = -alto_campo;// /2 - paleta.high;
	
	//definir superficie de choque de la paleta
	planoA.vertical = 0;
	planoA.inf = paleta.x;
	planoA.sup = paleta.x + paleta.length;
	planoA.coord = paleta.y+paleta.high;
	
	//definir pelota
	pelota.radio=0.5;
	pelota.slices=20;
	pelota.stacks=20;
	pelota.aceleracion=-9.8/10000/2.54; //gravedad en centésimas de pulgada sobre segundos al cuadrado
	
	if( argc < 2 ){
		fprintf(stderr, "Warning: ningun archivo fue proporcionado");
	}else{
		archivo = fopen(argv[1], "r");
		if( archivo == NULL ){
			fprintf(stderr, "Warning, No se pudo abrir el archivo \"%s\": ", argv[1]);
			perror("");
		}else{
			niveles = cargar_archivo(archivo);
			if( niveles == NULL ){
				fprintf(stderr, "No se pudo cargar el archivo \"%s\": %s\n", argv[1], error_carga);
				return 1;
			}else{
				printf("\"%s\" fue cargado exitosamente\n", argv[1]);
			}
		}
	}
	
	puntuacion = 0;
	startLevel();

	//establecer parámtetros de GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	
	glutInitWindowSize (ancho_campo*viewport_scale, alto_campo*viewport_scale); 
	glutInitWindowPosition (10, 50);
	glutCreateWindow ("Breakout!3D");
	
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reproject);
	glutIdleFunc(Advance);
	
	glutKeyboardFunc(startPlaying);
	glutSpecialFunc(handleKeypress);
	
	//pasar control a GLUT
	glutMainLoop();
	
	return 0;
}
