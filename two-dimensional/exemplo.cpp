#include <GL/glut.h> // Inclui a biblioteca GLUT

// Função que contém o código de desenho
void desenha(void) {
    // Limpa a janela de visualização com a cor de fundo definida
    glClear(GL_COLOR_BUFFER_BIT);

    // O código da sua imagem começa aqui
    // Define a espessura da linha
    glLineWidth(2.0f); // [cite: 370]

    // Desenha os eixos X e Y
    glBegin(GL_LINES); // [cite: 371]
        glColor3f(1.0f, 1.0f, 0.0f); // Define a cor amarela [cite: 372]
        // Linha vertical do eixo Y
        glVertex2i(0, 40);  // [cite: 373]
        glVertex2i(0, -40); // [cite: 376]
        // Linha horizontal do eixo X
        glVertex2i(-40, 0); // [cite: 377]
        glVertex2i(40, 0);  // [cite: 378]
    glEnd(); // [cite: 379]

    // Desenha o quadrado vermelho
    glBegin(GL_LINE_LOOP); // GL_LINE_LOOP desenha uma sequência de linhas e fecha o contorno [cite: 380]
        glColor3f(1.0f, 0.0f, 0.0f); // Define a cor vermelha [cite: 381, 382]
        glVertex2i(-20, 20); // Vértice superior esquerdo [cite: 383, 384]
        glVertex2i(-20, -20); // Vértice inferior esquerdo [cite: 385, 386]
        glVertex2i(20, -20);  // Vértice inferior direito [cite: 387]
        glVertex2i(20, 20);   // Vértice superior direito [cite: 388]
    glEnd(); // [cite: 389]

    // Garante que os comandos de desenho sejam executados
    glFlush();
}

// Função de inicialização de parâmetros
void inicializa(void) {
    // Define a cor de fundo da janela (preto)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Define o sistema de coordenadas que será usado na janela (SRU)
    // Os eixos X e Y irão de -40 a +40
    gluOrtho2D(-40.0f, 40.0f, -40.0f, 40.0f); // [cite: 369]
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa a GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Define o modo de display (buffer único, cores RGB)
    
    // Define a posição e o tamanho da janela na tela (SRT)
    glutInitWindowPosition(100, 100); // [cite: 368]
    glutInitWindowSize(400, 400);     // [cite: 368]

    // Cria a janela com o título especificado
    glutCreateWindow("Exemplo 02 - Eixos e Quadrado");

    // Registra a função de desenho para ser chamada quando a janela precisar ser redesenhada
    glutDisplayFunc(desenha);

    // Registra a função de inicialização
    inicializa();

    // Inicia o loop de processamento de eventos da GLUT
    glutMainLoop();

    return 0;
}