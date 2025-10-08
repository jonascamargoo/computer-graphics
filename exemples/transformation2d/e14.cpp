#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//Controle da translação
GLfloat posicaoX=0;

//Controle da rotação
GLfloat angulo1=0, angulo2=-90;


void desenharBraco()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
		glVertex2f(1.0, 4.6);
		glVertex2f(1.0, -0.8);
		glVertex2f(-1.0, -0.8);
		glVertex2f(-1.0, 4.6);
	glEnd();
	glPointSize(2);
	glBegin(GL_POINTS);
		glVertex2i(0,0);
	glEnd();
}

void desenharBase()
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
		glVertex2f(1.8, 1);
		glVertex2f(1.8, -1.5);
		glVertex2f(1.0, -1.5);
		glVertex2f(1.0, -1);
		glVertex2f(-1.0, -1);
		glVertex2f(-1.0, -1.5);
		glVertex2f(-1.8, -1.5);
		glVertex2f(-1.8, 1);
	glEnd();
}


void desenharChao(){
	glBegin(GL_QUADS);
        glVertex2f(-1.0f,-1.0f);
        glVertex2f(-1.0f, 1.0f);
        glVertex2f( 1.0f, 1.0f);
        glVertex2f( 1.0f,-1.0f);
    glEnd();
}

// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    //chão
    glPushMatrix();
        glTranslatef(0, -20, 0);
        glScalef(40, 20 , 1);
        glColor3f(0.7f, 0.7f, 0.7f);
        desenharChao();
    glPopMatrix();

     glPushMatrix();

        glTranslatef(posicaoX, 0, 0);

        //base
        glPushMatrix();
            glTranslatef(0, 7, 0);
            glScalef(5.0f, 5.0f, 1.0f);
            glColor3f(1.0f, 0.0f, 0.0f);
            desenharBase();
        glPopMatrix();


        glPushMatrix();
         //braço 1
            glPushMatrix();
                glTranslatef(0, 10, 0);
                glScalef(4.0f, 2.5f, 1.0f);
                glRotatef(angulo1, 0.0f, 0.0f, 1.0f);
                glColor3f(0.0f, 1.0f, 0.0f);
                desenharBraco();

                //braço 2
                glPushMatrix();
                    glTranslatef(0, 3, 0);
                    glScalef(0.7f, 1.0f, 1.0f);
                    glRotatef(angulo2, 0.0f, 0.0f, 1.0f);
                    glColor3f(0.0f, 0.0f, 1.0f);
                    desenharBraco();
                glPopMatrix();
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
        //base
        case GLUT_KEY_LEFT:
            posicaoX--;
            break;
    	case GLUT_KEY_RIGHT:
    	    posicaoX++;
            break;

        // Rotaciona braço1
        case GLUT_KEY_HOME:
            angulo1 = angulo1 - 5;
            break;
        case GLUT_KEY_END:
            angulo1 = angulo1 + 5;
            break;

        // Rotaciona braço2
        case GLUT_KEY_PAGE_UP:
            angulo2 = angulo2 - 5;
            break;
        case GLUT_KEY_PAGE_DOWN:
            angulo2 = angulo2 + 5;
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
    glutCreateWindow("Prática 04 - Exercício 02");

    // Registra as funções de callback do GLUT
    glutDisplayFunc(display);   // redesenho
    glutKeyboardFunc(key);      // eventos do teclado
    glutReshapeFunc(resize);    // redimensionamento da janela
    glutSpecialFunc(specialkey); // eventos do teclado para teclas especiais

    // Executa as configurações iniciais de visualização
    setup();

    // Inicia o loop principal de eventos da GLUT
    glutMainLoop();

    return 0;
}