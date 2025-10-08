#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

//Controle da translação
GLfloat posicaoX=0, posicaoY=0;

//Controle da escala
GLfloat escalaCirculo=4;
GLfloat escalaQuadrado=4;

//Controle da rotação
GLfloat angulo=0;

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

void desenharEixo(){
    glBegin(GL_LINES);
       glVertex2i(0, 40);
       glVertex2i(0, -40);
       glVertex2i(-40, 0);
       glVertex2i(40, 0);
     glEnd();
}

// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 0.0f);
    desenharEixo();

    //circulo
    glPushMatrix();
        glRotatef(angulo, 0, 0 , 1);
        glScalef(escalaCirculo, escalaCirculo, 1);
        glColor3f(1.0f, 0.2f, 1.0f);
        desenharCirculo(10); //rosa
    glPopMatrix();

    //quadradros
    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, 0.0f);
        glTranslatef(20, 20, 0.0f);
        glScalef(escalaQuadrado, escalaQuadrado, 1);
        glColor3f(1.0f, 0.0f, 0.0f);
        desenharQuadrado(); //vermelho
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, 0.0f);
        glTranslatef(20, -20, 0.0f);
        glScalef(escalaQuadrado, escalaQuadrado, 1);
        glColor3f(0.0f, 0.0f, 1.0f);
        desenharQuadrado(); //azul
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, 0.0f);
        glTranslatef(-20, -20, 0.0f);
        glScalef(escalaQuadrado, escalaQuadrado, 1);
        glColor3f(0.0f, 1.0f, 1.0f);
        desenharQuadrado(); //ciano
    glPopMatrix();

    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, 0.0f);
        glTranslatef(-20, 20, 0.0f);
        glScalef(escalaQuadrado, escalaQuadrado, 1);
        glColor3f(0.0f, 1.0f, 0.0f);
        desenharQuadrado(); //verde
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
            angulo--;
            break;
    	case GLUT_KEY_RIGHT:
    	    posicaoX++;
    	    angulo++;
            break;
        case GLUT_KEY_UP:
            posicaoY++;
            angulo++;
            break;
        case GLUT_KEY_DOWN:
            posicaoY--;
            angulo--;
            break;

        case GLUT_KEY_HOME:
            escalaQuadrado += 0.3;
            break;
        case GLUT_KEY_END:
            escalaQuadrado -= 0.3;
            break;

        case GLUT_KEY_PAGE_UP:
            escalaCirculo += 0.3;
            break;
        case GLUT_KEY_PAGE_DOWN:
            escalaCirculo -= 0.3;
            break;
    }

    glutPostRedisplay();
}

// Configurações iniciais de visualização (sistema de coordenadas).
void setup(void){

    glViewport(0, 0, 400, 400);
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
    glutCreateWindow("Prática 03 - Exercício 02");

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