#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


GLfloat posicaoX = 0, posicaoY = 0;

GLfloat eixo = 40;

GLfloat limiteX = 15.0;
GLfloat limiteY = 16.0;

GLfloat incrementoX = 1;
GLfloat incrementoY = 1;

void desenharCasa(){
    //Desenha a parede
    glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f(-15.0f,-15.0f);
		glVertex2f(-15.0f,  5.0f);
		glVertex2f( 15.0f,  5.0f);
		glVertex2f( 15.0f,-15.0f);
	glEnd();

    // Desenha a porta
	glColor3f(0.85f, 0.85f, 0.85f);
	glBegin(GL_QUADS);
		glVertex2f(-4.0f,-14.5f);
		glVertex2f(-4.0f,  0.0f);
		glVertex2f( 4.0f,  0.0f);
		glVertex2f( 4.0f,-14.5f);
	glEnd();

    // Desenha a janela
	glColor3f(0.85f, 0.85f, 0.85f);
	glBegin(GL_QUADS);
		glVertex2f( 7.0f,-5.0f);
		glVertex2f( 7.0f,-1.0f);
		glVertex2f(13.0f,-1.0f);
		glVertex2f(13.0f,-5.0f);
	glEnd();

	// Desenha as "linhas" da janela
    glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2f( 7.0f,-3.0f);
		glVertex2f(13.0f,-3.0f);
		glVertex2f(10.0f,-1.0f);
		glVertex2f(10.0f,-5.0f);
	glEnd();


	// Desenha o telhado
    glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
		glVertex2f(-15.0f, 5.0f);
		glVertex2f(  0.0f,16.0f);
		glVertex2f( 15.0f, 5.0f);
	glEnd();
}

// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(posicaoX, posicaoY, 0.0f);
        desenharCasa();
    glPopMatrix();

	glutSwapBuffers();
}

// Função de callback para tratamento de eventos do teclado.
static void key(unsigned char key, int x, int y){
    // Encerra o programa quando a tecla ESC (código 27) é pressionada.
    if (key == 27)
        exit(0);
}

void animate(int value){
	// Muda a direção quando chega na borda esquerda ou direita
	if( ((posicaoX + limiteX) > eixo) || ((posicaoX - limiteX) < -eixo) ){
        incrementoX = -incrementoX;
	}

	// Muda a direção quando chega na borda superior ou inferior
	if( ((posicaoY + limiteY) > eixo) || ((posicaoY - limiteY) < -eixo) ){
        incrementoY = -incrementoY;
	}

    posicaoX = posicaoX + incrementoX;
	posicaoY = posicaoY + incrementoY;

	glutPostRedisplay();

	glutTimerFunc(50, animate, 1);
}

// Configurações iniciais de visualização (sistema de coordenadas).
void setup(void){
    glViewport(0, 0, 400, 400);
    gluOrtho2D(-eixo, eixo, -eixo, eixo);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Programa principal
int main(int argc, char *argv[]){
    // Inicializa a biblioteca GLUT
    glutInit(&argc, argv);

    // Define o modo de exibição (buffer simples + modelo de cores RGB)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Define o tamanho inicial da janela em pixels
    glutInitWindowSize(400, 400);

    // Define a posição inicial da janela na tela
    glutInitWindowPosition(100, 100);

    // Cria a janela e define o título
    glutCreateWindow("Prática 05 - Exemplo 02");

    // Registra as funções de callback do GLUT
    glutDisplayFunc(display);   // redesenho
    glutKeyboardFunc(key);      // eventos do teclado
    glutReshapeFunc(resize);    // redimensionamento da janela
    glutTimerFunc(50, animate, 1);// Registra a função callback que será chamada a cada intervalo de tempo

    // Executa as configurações iniciais de visualização
    setup();

    // Inicia o loop principal de eventos da GLUT
    glutMainLoop();

    return 0;
}