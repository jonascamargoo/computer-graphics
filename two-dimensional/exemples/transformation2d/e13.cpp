#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

//Controle da translação
GLfloat posicaoX=0, posicaoY=0;

//Controle da rotação
GLfloat angulo=0;

//Controle Cisalhamento
GLfloat SHyx = 0.0;

void desenharCirculo(GLsizei numeroVertice){
     float angulo, incremento;
     incremento = (2 * M_PI) / numeroVertice;
     glBegin(GL_POLYGON);
         for(angulo=0; angulo<2*M_PI; angulo+=incremento){
            glVertex2f(cos(angulo), sin(angulo));
         }
     glEnd();
}

void desenharQuadrado(){
	glBegin(GL_QUADS);
        glVertex2f(-1.0f,-1.0f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f( 1.0f, 1.0f);
        glVertex2f( 1.0f,-1.0f);
    glEnd();
}

void desenharRoda(GLboolean calota){
    desenharCirculo(30);

    if(calota){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
           glVertex2f(0, 0.8);
           glVertex2f(0, -0.8);
           glVertex2f(-0.8, 0);
           glVertex2f(0.8, 0);
        glEnd();
    }
}

void desenharCarroceria() {
    // parte inferior
    glBegin(GL_POLYGON);
        glVertex2f(-2.0f, -1.0f);
        glVertex2f(2.0f, -1.0f);
        glVertex2f(2.0f, 0.0f);
        glVertex2f(-2.0f, 0.0f);
    glEnd();

    // parte superior
    glBegin(GL_POLYGON);
        glVertex2f(-1.5f, 0.0f);
        glVertex2f(1.5f, 0.0f);
        glVertex2f(1.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
    glEnd();
}

GLfloat shearMatrix[16] = {
   1.0f, 0.0f, 0.0f, 0.0f,
   SHyx, 1.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 0.0f, 1.0f
};

// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);

  //gramado
    glPushMatrix();
        glTranslatef(0, -20, 0);
        glScalef(40, 20 , 1);
        glColor3f(0.0f, 1.0f, .0f);
        desenharQuadrado();
    glPopMatrix();

    //linha
    glLineWidth(1.5f);
    glColor3f(0.0f, 0.0f, .0f);
    glBegin(GL_LINES);
       glVertex2i(-40, 0);
       glVertex2i(40, 0);
    glEnd();

	//sol
	glPushMatrix();
		glTranslatef(0, 33, 0);
		glScalef(5, 5 , 1);
		glColor3f(1.0f, 1.0f, 0.0f);
		desenharCirculo(30);
	glPopMatrix();

	//carro
    glPushMatrix();
       glTranslatef(posicaoX, posicaoY, 0);

       glPushMatrix();
           //Carroceria do Carro
           glTranslatef(0, 5, 0);
           glScalef(3, 3 , 1);
           glColor3f(0.0f, 0.0f, 0.0f);
           desenharCarroceria();
       glPopMatrix();

       //Rodas do Carro
       glPushMatrix();

           glPushMatrix();

               glTranslatef(-3, 1.5, 0);
               glScalef(2, 2 , 1);
               glRotated(angulo, 0, 0, 1);
               glColor3f(1.0f, 0.0f, 0.0f);
               desenharRoda(true);
           glPopMatrix();

           glPushMatrix();
               glTranslatef(3, 1.5, 0);
               glScalef(2, 2 , 1);
               glRotated(angulo, 0, 0, 1);
               glColor3f(1.0f, 0.0f, 0.0f);
               desenharRoda(true);
           glPopMatrix();

       glPopMatrix();

       //sombra do carro
       glPushMatrix();

            glMultMatrixf(shearMatrix);

            //Carroceria do Carro
            glPushMatrix();
               glTranslatef(0, -5, 0);
               glScalef(-3, -3 , 1);
               glColor3f(0.8f, 0.8f, 0.8f);
               desenharCarroceria();
           glPopMatrix();


           //Rodas do Carro
           glPushMatrix();
               glTranslatef(-3, -2.5, 0);
               glScalef(2, 2 , 1);
               glColor3f(0.8f, 0.8f, 0.8f);
               desenharRoda(false);
           glPopMatrix();

           glPushMatrix();
               glTranslatef(3, -2.5, 0);
               glScalef(2, 2 , 1);
               glColor3f(0.8f, 0.8f, 0.8f);
               desenharRoda(false);
           glPopMatrix();

        glPopMatrix();

    glPopMatrix();

	glFlush();
}

// Função de callback para tratamento de eventos do teclado.
static void key(unsigned char key, int x, int y){
    // Encerra o programa quando a tecla ESC (código 27) é pressionada.
    if (key == 27)
        exit(0);
}

// função para tratar os eventos do teclado para teclas Especiais
static void specialkey(int key, int x, int y){

    switch (key)
    {
        case GLUT_KEY_LEFT:
            posicaoX--;
            angulo += 5;
            SHyx += 0.05;
            shearMatrix[4] = SHyx;
            break;
    	case GLUT_KEY_RIGHT:
    	    posicaoX++;
    	    angulo -= 5;
    	    SHyx -= 0.05;
    	    shearMatrix[4] = SHyx;
            break;
    }

    glutPostRedisplay();
}

// Configurações iniciais de visualização (sistema de coordenadas).
void setup(void){

    glViewport(0, 0, 400, 400);
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Programa principal
int main(int argc, char *argv[]){
    // Inicializa a biblioteca GLUT
    glutInit(&argc, argv);

    // Define o modo de exibição (buffer simples + modelo de cores RGB)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Define o tamanho inicial da janela em pixels
    glutInitWindowSize(400, 400);

    // Define a posição inicial da janela na tela
    glutInitWindowPosition(100, 100);

    // Cria a janela e define o título
    glutCreateWindow("Prática 04 - Exercício 01");

    // Registra as funções de callback do GLUT
    glutDisplayFunc(display);   // redesenho
    glutKeyboardFunc(key);      // eventos do teclado
    glutReshapeFunc(resize);    // redimensionamento da janela
    glutSpecialFunc(specialkey);// eventos do teclado das teclas especiais

    // Executa as configurações iniciais de visualização
    setup();

    // Inicia o loop principal de eventos da GLUT
    glutMainLoop();

    return 0;
}