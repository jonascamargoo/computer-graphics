#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <cstring>

GLfloat eixo = 15;
GLfloat panX = 0;
GLfloat panY = 0;
char texto[30];

void desenharArvore(){
    //tronco
    glColor3f(0.55f, 0.27f, 0.07f); // marrom
    glBegin(GL_QUADS);
        glVertex2f(-0.5f, -5.0f);
        glVertex2f( 0.5f, -5.0f);
        glVertex2f( 0.5f,  0.0f);
        glVertex2f(-0.5f,  0.0f);
    glEnd();

    //Copa da árvore com 3 triângulos sobrepostos
    glColor3f(0.0f, 0.8f, 0.0f); // verde
    // Base maior
    glBegin(GL_TRIANGLES);
        glVertex2f(-2.5f,  0.0f);
        glVertex2f( 0.0f,  2.5f);
        glVertex2f( 2.5f,  0.0f);
    glEnd();

     // Triângulo do meio
    glBegin(GL_TRIANGLES);
        glVertex2f(-2.0f,  1.25f);
        glVertex2f( 0.0f,  3.75f);
        glVertex2f( 2.0f,  1.25f);
    glEnd();

    // Triângulo superior
    glBegin(GL_TRIANGLES);
        glVertex2f(-1.5f,  2.5f);
        glVertex2f( 0.0f,  5.0f);
        glVertex2f( 1.5f,  2.5f);
    glEnd();
}

void desenharEixos(){
	//desenhar eixos
	glColor3f(1.0f, 1.0f, 0.0f);
	glLineWidth(2);
	glBegin(GL_LINES);
		glVertex2f(-eixo, 0); //eixo X
		glVertex2f( eixo, 0);
		glVertex2f(0,  eixo); //eixo Y
		glVertex2f(0, -eixo);
	glEnd();
}

// Exibir um texto na janela GLUT
void exibirTexto(char *string){
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-eixo, -(eixo - 1));
    while(*string){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
    }
}

// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    desenharEixos();

    desenharArvore();

    exibirTexto(texto);

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
            panX = panX + 1;
            break;

    	case GLUT_KEY_RIGHT:
            panX = panX - 1;
            break;

        case GLUT_KEY_UP:
            panY = panY - 1;
            break;

        case GLUT_KEY_DOWN:
            panY = panY + 1;
            break;
    }

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-eixo+panX, eixo+panX, -eixo+panY, eixo+panY);

    glutPostRedisplay();
}

// função para tratar os eventos de cliques do mouse
static void mouseButton(int button, int state, int x, int y){
    // zoom in
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
        eixo = eixo - 3;
    }

    // zoom out
    if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)){
        eixo = eixo + 3;
    }

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-eixo, eixo, -eixo, eixo);

    glutPostRedisplay();
}

// Função callback chamada para movimento do mouse
void mouseMotion(int x, int y){
     sprintf(texto, "(%d,%d)", x, y);
     glutPostRedisplay();
}

// Configurações iniciais de visualização (sistema de coordenadas).
void setup(void){

    glViewport(0, 0, 400, 400);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-eixo, eixo, -eixo, eixo);

    strcpy(texto, "(0,0)");

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
    glutCreateWindow("Prática 05 - Exemplo 01");

    // Registra as funções de callback do GLUT
    glutDisplayFunc(display);   // redesenho
    glutKeyboardFunc(key);      // eventos do teclado
    glutReshapeFunc(resize);    // redimensionamento da janela
    glutSpecialFunc(specialkey);// eventos do teclado das teclas especiais
    glutMouseFunc(mouseButton); // eventos do mouse
    glutPassiveMotionFunc(mouseMotion); //movimentação do mouse sobre a janela

    // Executa as configurações iniciais de visualização
    setup();

    // Inicia o loop principal de eventos da GLUT
    glutMainLoop();

    return 0;
}