#include <GL/glut.h>
#include <cmath> // Necessário para sin() e cos()

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variáveis globais para armazenar as dimensões da janela
int g_largura_janela;
int g_altura_janela;

// Função genérica para desenhar um polígono regular (ou círculo)
void desenhaPoligono(int num_lados, float raio, GLenum tipo_primitiva) {
    glBegin(tipo_primitiva);
    for (int i = 0; i < num_lados; i++) {
        float angulo = i * 2.0f * M_PI / num_lados;
        float x = raio * cos(angulo);
        float y = raio * sin(angulo);
        glVertex2f(x, y);
    }
    glEnd();
}

// Função principal de desenho
void desenha(void) {
    // Limpa a janela inteira com a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    // Define a cor para todos os desenhos (vermelho)
    glColor3f(1.0f, 0.0f, 0.0f);

    // --- 1. Viewport Superior Esquerda (Círculo) ---
    glViewport(0, g_altura_janela / 2, g_largura_janela / 2, g_altura_janela / 2);
    // Um círculo é um polígono com muitos lados
    desenhaPoligono(100, 40.0f, GL_LINE_LOOP);

    // --- 2. Viewport Superior Direita (Decágono) ---
    glViewport(g_largura_janela / 2, g_altura_janela / 2, g_largura_janela / 2, g_altura_janela / 2);
    desenhaPoligono(10, 40.0f, GL_LINE_LOOP);

    // --- 3. Viewport Inferior Esquerda (Pontos) ---
    glViewport(0, 0, g_largura_janela / 2, g_altura_janela / 2);
    // Usa os mesmos vértices do octógono, mas desenha apenas os pontos
    desenhaPoligono(8, 40.0f, GL_POINTS);

    // --- 4. Viewport Inferior Direita (Octógono) ---
    glViewport(g_largura_janela / 2, 0, g_largura_janela / 2, g_altura_janela / 2);
    desenhaPoligono(8, 40.0f, GL_LINE_LOOP);


    // Garante que os comandos de desenho sejam executados
    glFlush();
}

// Função chamada quando a janela é redimensionada
void redimensiona(int largura, int altura) {
    // Armazena as novas dimensões nas variáveis globais
    g_largura_janela = largura;
    g_altura_janela = altura;

    // É importante definir um viewport inicial que ocupe toda a janela
    // para que o comando glClear funcione corretamente.
    glViewport(0, 0, largura, altura);

    // Define o sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // O sistema de coordenadas vai de -50 a 50 em ambos os eixos,
    // o que centraliza a origem (0,0) em cada viewport.
    gluOrtho2D(-50.0, 50.0, -50.0, 50.0);

    // Volta para a matriz de modelo/visualização
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função de inicialização
void inicializa(void) {
    // Define a cor de fundo (preto)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Define o tamanho dos pontos
    glPointSize(5.0f);
    // Define a espessura das linhas
    glLineWidth(2.0f);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowSize(500, 500); // Tamanho inicial da janela
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Exercicio 01");

    // Registra as funções de callback
    glutDisplayFunc(desenha);
    // Registra a função de redimensionamento! Essencial para este exemplo.
    glutReshapeFunc(redimensiona);
    
    inicializa();
    glutMainLoop();

    return 0;
}