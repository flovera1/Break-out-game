#include "basico.h"
void drawParallelepiped(struct parallelepiped* p)
{
	glPushMatrix();
		glTranslatef(p->x, p->y, 0.0);
		 glBegin(GL_POLYGON);
		        //face in zx plane
		        glVertex3f(0,0 ,p->large);//A
		        glVertex3f(p->length, 0, p->large);//B
		        glVertex3f(p->length, 0, 0);//C
		        glVertex3f(0, 0, 0);//H
		glEnd();

		glBegin(GL_POLYGON);
		        //face in yz plane
		        glVertex3f(0, 0 , 0);//A
		        glVertex3f(0, 0, p->large);//F
		        glVertex3f(0, p->high, 0);//H
		        glVertex3f(0, p->high, p->large);//E

		glEnd();

		glBegin(GL_POLYGON);
		        //face in xy plance
		        glVertex3f(p->length, 0, 0);//C
		        glVertex3f(0, 0, 0);//H
		        glVertex3f(0, p->high,0);//E
		        glVertex3f(p->length,p->high, 0);//D
		glEnd();
		glBegin(GL_POLYGON);
		        //|| to XY plane. AGBF
		        glVertex3f(0, p->high, p->large);//F
		        glVertex3f(0,0 ,p->large);//A
		        glVertex3f(p->length, p->high, p->large);//G
		        glVertex3f(p->length, 0, p->large);//B
		        
		glEnd();
		
		glBegin(GL_POLYGON);
		        //|| to yz plane bcdg
		        glVertex3f(p->length, 0, p->large);//B
		        glVertex3f(p->length, 0, 0);//C
		        glVertex3f(p->length, p->high, 0);//D
			glVertex3f(p->length, p->high, p->large);//G	
		glEnd();
		glBegin(GL_POLYGON);

		         //|| to xz
		        glVertex3f(0, p->high,0);//E
		        glVertex3f(0, p->high, p->large);//F
		        glVertex3f(p->length, p->high, 0);//D
		        glVertex3f(p->length, p->high, p->large);//G
		 glEnd();
	glPopMatrix();
         glFlush();
}
void drawSphere(struct sphere* s)
{
	glPushMatrix();
		glTranslatef(s->x,s->y, 0.0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(s->radio, s->slices, s->stacks);
	glPopMatrix();
}

void dibujarPaleta()
{
/*
	struct parallelepiped paleta;
	paleta.large =2.0;
	paleta.high = 2.0;
	paleta.length = 5.0;
	paleta.y = h;
	drawParallelepiped(&paleta);	
*/
}

void drawParallelepiped2(struct parallelepiped* p)
{
glPushMatrix();
	glLoadIdentity(); 
	glutSolidCube(1); //se crea un cubo con arista de tama;o 1 en el punto (0,0,0)
	glScalef(p->large, p->high, p->length); 
glPopMatrix();
}

void window(int w, int h){              

	float aspectratio;
	if (h==0)
		h=1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    aspectratio = (float) w / (float) h;
    gluPerspective(30.0f, aspectratio, 1.0, 300.0);
}

/*
int main(int argc,char** argv) {

	glutInit(&argc,argv);
	glutInitWindowSize (500, 500); 
        glutInitWindowPosition (10, 50);
	glutCreateWindow ("ventana");	

	glutDisplayFunc(drawScene);
	//glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(window);
	//glutIdleFunc(animacion1);
	
	//glutTimerFunc(25, update, 0); //llamada a update 25 milisegundos 
	glutMainLoop();

	return 0;
}
*/
