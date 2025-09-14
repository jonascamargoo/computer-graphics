#include <GL/glut.h>

void draw() {
    // Limpa a janela com a cor de fundo (definida como branco em initialize)
    glClear(GL_COLOR_BUFFER_BIT);

    // --- Corpo da casa (Azul) ---
    // Usamos GL_QUADS para um quadrado preenchido
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f); // Azul
        glVertex2i(-20, 0);   // Vértice superior esquerdo
        glVertex2i(-20, -20); // Vértice inferior esquerdo
        glVertex2i(20, -20);  // Vértice inferior direito
        glVertex2i(20, 0);   // Vértice superior direito
    glEnd();

    // --- Telhado (Vermelho) ---
    // GL_TRIANGLES para o telhado
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho
        glVertex2i(0, 15);   // Vértice superior
        glVertex2i(-20, 0);  // Vértice inferior esquerdo
        glVertex2i(20, 0);   // Vértice inferior direito
    glEnd();

    // --- Porta (Cinza) ---
    // Usamos GL_QUADS para a porta preenchida
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f); // Cinza
        glVertex2i(-5, -5);  // Vértice superior esquerdo
        glVertex2i(-5, -20); // Vértice inferior esquerdo (toca o chão da casa)
        glVertex2i(5, -20);  // Vértice inferior direito (toca o chão da casa)
        glVertex2i(5, -5);   // Vértice superior direito
    glEnd();

    // --- Janelas (Cinza) ---
    // Desenhamos os 4 pequenos quadrados da janela
    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f, 0.5f); // Cinza
        // Quadrado superior esquerdo da janela
        glVertex2f(10.0, -5.0);
        glVertex2f(10.0, -8.0);
        glVertex2f(13.0, -8.0);
        glVertex2f(13.0, -5.0);
        // Quadrado superior direito da janela
        glVertex2f(15.0, -5.0);
        glVertex2f(15.0, -8.0);
        glVertex2f(18.0, -8.0);
        glVertex2f(18.0, -5.0);
        // Quadrado inferior esquerdo da janela
        glVertex2f(10.0, -10.0);
        glVertex2f(10.0, -13.0);
        glVertex2f(13.0, -13.0);
        glVertex2f(13.0, -10.0);
        // Quadrado inferior direito da janela
        glVertex2f(15.0, -10.0);
        glVertex2f(15.0, -13.0);
        glVertex2f(18.0, -13.0);
        glVertex2f(18.0, -10.0);
    glEnd();

    // Garante que todos os comandos sejam processados
    glFlush();
}

void initialize(void) {
    // Define a cor de fundo da janela como branco, para combinar com a imagem
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Define o sistema de coordenadas
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500); // Aumentei um pouco a janela para melhor visualização

    glutCreateWindow("Minha Casa com OpenGL");

    glutDisplayFunc(draw);
    initialize();
    glutMainLoop();

    return 0;
}