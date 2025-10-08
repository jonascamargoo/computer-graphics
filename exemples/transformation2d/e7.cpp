#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Função de callback chamada sempre que a janela é redimensionada.
static void resize(int width, int height){
    glViewport(0, 0, width, height);
}

void desenharEixos(void){
     glBegin(GL_LINES);
       glVertex2i(0, 20); //eixo Y
       glVertex2i(0, -20);
       glVertex2i(-20, 0);//eixo X
       glVertex2i(20, 0);
     glEnd();
}

void desenharTriangulo(){
     glBegin(GL_LINE_LOOP);
       glVertex2f(8.0, 11.0);
       glVertex2f(4.0, 3.0);
       glVertex2f(12.0, 6.0);
     glEnd();
}

// Função de callback responsável por desenhar os elementos na tela.
static void display(){

    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1.5f);

    glColor3f(1.0f, 1.0f, 0.0f); //amarelo
    desenharEixos();

    glColor3f(1.0, 0.0, 0.0); //vermelho
    desenharTriangulo();

    //Refleção com relação ao eixo X
    glPushMatrix();
        glScalef(1.0, -1.0, 0.0);
        glColor3f(0.0, 1.0, 0.0); //verde
        desenharTriangulo();
    glPopMatrix();

    //Refleção com relação ao eixo Y
    glPushMatrix();
        glScalef(-1.0, 1.0, 0.0);
        glColor3f(0.0, 0.0, 1.0); //azul
        desenharTriangulo();
    glPopMatrix();

    //Refleção com relação a origem
    glPushMatrix();
        glScalef(-1.0, -1.0, 0.0);
        glColor3f(1.0, 0.0, 1.0); //rosa
        desenharTriangulo();
    glPopMatrix();

	glFlush();
}

// Função de callback para tratamento de eventos do teclado.
static void key(unsigned char key, int x, int y){
    // Encerra o programa quando a tecla ESC (código 27) é pressionada.
    if (key == 27)
        exit(0);
}


// Configurações iniciais de visualização (sistema de coordenadas).
void setup(void){

    gluOrtho2D(-20.0f, 20.0f, -20.0f, 20.0f);

    glViewport(0, 0, 400, 400);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 0.0f);

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
    glutCreateWindow("Transformações Geométricas - Exemplo 07");

    // Registra as funções de callback do GLUT
    glutDisplayFunc(display);   // redesenho
    glutKeyboardFunc(key);      // eventos do teclado
    glutReshapeFunc(resize);    // redimensionamento da janela

    // Executa as configurações iniciais de visualização
    setup();

    // Inicia o loop principal de eventos da GLUT
    glutMainLoop();

    return 0;
}