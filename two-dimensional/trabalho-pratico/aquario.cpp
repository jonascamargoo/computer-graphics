/*
 * TRABALHO PRÁTICO I - COMPUTAÇÃO GRÁFICA
 *
 * TEMA: Jogo de Sinuca 2D (Física)
 *
 * DESCRIÇÃO:
 * Este projeto simula um jogo de sinuca 2D.
 * - 16 bolas (1 branca, 15 coloridas) são desenhadas na mesa.
 * - O usuário controla a bola branca usando o mouse.
 * - A física de colisão (bola-bola e bola-borda) é simulada.
 * - As bolas perdem velocidade gradualmente devido ao atrito.
 * - Bolas que caem nas caçapas são removidas.
 *
 * CONTROLES:
 * - Mover o Mouse: Mira o taco (aponta o mouse para a bola branca).
 * - Clicar e Arrastar (Botão Esquerdo): Puxa o taco para trás para definir a força.
 * - Soltar (Botão Esquerdo): Dá a tacada.
 * - Teclado 'r' ou 'R': Reinicia o jogo (arruma as bolas).
 *
 * CONCEITOS APLICADOS (Baseado nas Práticas):
 * - Primitivas Gráficas (GL_POLYGON para bolas e mesa)
 * - Animação Contínua (glutTimerFunc, GLUT_DOUBLE)
 * - Interação (glutMouseFunc, glutPassiveMotionFunc, glutMotionFunc)
 * - Mapeamento SRT -> SRU (Conversão de coordenadas do mouse)
 * - Transformações Geométricas 2D:
 * - glTranslatef (Posicionamento e movimento das bolas)
 * - glRotatef (Rotação do taco)
 * - Lógica de Simulação (Física de colisão, atrito)
 * - Exibição de Texto (glutBitmapCharacter)
 */

 
#include <GL/glut.h>
#include <cmath>
#include <vector>    // Usado para armazenar as bolas e caçapas
#include <string>
#include <cstdio>    // Para sprintf

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- 1. Definições, Estruturas e Constantes ---

// Ajuda a facilitar a matemática de física
struct Vetor2D {
    float x, y;

    Vetor2D(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

    // Operações de vetor
    Vetor2D operator+(const Vetor2D& v) const { return Vetor2D(x + v.x, y + v.y); }
    Vetor2D operator-(const Vetor2D& v) const { return Vetor2D(x - v.x, y - v.y); }
    Vetor2D operator*(float s) const { return Vetor2D(x * s, y * s); }
    
    float magnitude() const { return sqrt(x * x + y * y); }
    Vetor2D normalizado() const {
        float mag = magnitude();
        if (mag > 0) return Vetor2D(x / mag, y / mag);
        return Vetor2D(0, 0);
    }
    float dot(const Vetor2D& v) const { return x * v.x + y * v.y; }
};

struct Bola {
    Vetor2D pos; // Posição
    Vetor2D vel; // Velocidade
    float raio;
    float massa;
    float r, g, b; // Cor
    bool naMesa;   // Se não foi encaçapada
    int id;
};

struct Cacapa {
    Vetor2D pos;
    float raio;
};

// Constantes de Jogo
const int NUM_BOLAS = 16;
const float RAIO_BOLA = 2.5f;
const float RAIO_CACAPA = 4.5f;
const float ATRITO = 0.995f; // Fator de desaceleração
const float FATOR_FORCA_TACADA = 0.15f;
const float RESTITUICAO_BORDA = 0.8f; // Quão "elástica" é a borda

// Dimensões do Mundo (SRU)
const float MESA_LARGURA = 100.0f;
const float MESA_ALTURA = 50.0f;

// --- 2. Variáveis Globais ---

// Janela
int gLarguraTela = 1200;
int gAlturaTela = 650; // Proporção 2:1 da mesa + bordas

// Atores
std::vector<Bola> gBolas;
std::vector<Cacapa> gCacapas;

// Estado de Jogo
bool gEstaMirando = false;
bool gBolasEmMovimento = false;
Vetor2D gPosMouseSRU;  // Posição do mouse no universo
Vetor2D gPosCliqueTaco; // Onde o usuário começou a arrastar
float gForcaTacada = 0.0f;
int gPontuacao = 0;
char gMensagemStatus[100];

// --- 3. Funções de Desenho ---

/**
 * Desenha um texto na tela usando glutBitmapCharacter.
 * A posição é em coordenadas do universo (SRU).
 */
void desenhaTexto(const char* texto, float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y); // Posição no SRU
    const char* c = texto;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        c++;
    }
}

/**
 * Desenha um círculo preenchido (baseado no exemplo2.cpp).
 * Usado para as bolas e caçapas.
 */
void desenhaCirculo(float raio, int lados, GLenum tipo) {
    glBegin(tipo);
    for (int i = 0; i < lados; i++) {
        float angulo = i * 2.0f * M_PI / lados;
        glVertex2f(cos(angulo) * raio, sin(angulo) * raio);
    }
    glEnd();
}

/**
 * Desenha a mesa de sinuca (bordas, feltro, caçapas).
 */
void desenhaMesa() {
    // Borda Marrom
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(-MESA_LARGURA - 10, -MESA_ALTURA - 10);
        glVertex2f( MESA_LARGURA + 10, -MESA_ALTURA - 10);
        glVertex2f( MESA_LARGURA + 10,  MESA_ALTURA + 10);
        glVertex2f(-MESA_LARGURA - 10,  MESA_ALTURA + 10);
    glEnd();

    // Feltro Verde
    glColor3f(0.0f, 0.4f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-MESA_LARGURA, -MESA_ALTURA);
        glVertex2f( MESA_LARGURA, -MESA_ALTURA);
        glVertex2f( MESA_LARGURA,  MESA_ALTURA);
        glVertex2f(-MESA_LARGURA,  MESA_ALTURA);
    glEnd();

    // Caçapas (Pretas)
    glColor3f(0.0f, 0.0f, 0.0f);
    for (const auto& cacapa : gCacapas) {
        glPushMatrix();
        glTranslatef(cacapa.pos.x, cacapa.pos.y, 0.0f);
        desenhaCirculo(cacapa.raio, 20, GL_POLYGON);
        glPopMatrix();
    }
}

/**
 * Desenha o taco de sinuca se o jogador estiver mirando.
 */
void desenhaTaco() {
    if (!gEstaMirando || gBolasEmMovimento) return;

    // Posição da bola branca
    Vetor2D posBolaBranca = gBolas[0].pos;

    // Vetor do mouse até a bola branca (direção oposta da tacada)
    Vetor2D dirTaco = posBolaBranca - gPosMouseSRU;
    
    // Calcula o ângulo de rotação para o taco
    float angulo = atan2(dirTaco.y, dirTaco.x) * 180.0f / M_PI;

    // Calcula a posição do taco (puxado para trás pela força)
    float distPuxada = (gPosMouseSRU - gPosCliqueTaco).magnitude();
    gForcaTacada = fmin(distPuxada, 50.0f); // Limita a força máxima

    // Posição inicial do taco (na borda da bola branca)
    Vetor2D posTaco = posBolaBranca + dirTaco.normalizado() * (RAIO_BOLA + 2.0f + gForcaTacada);

    glPushMatrix();
    
    // 1. Translação: Move para a posição da ponta do taco
    glTranslatef(posTaco.x, posTaco.y, 0.0f);
    
    // 2. Rotação: Gira o taco para apontar para a bola branca
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);

    // 3. Desenha o taco (um retângulo longo e fino)
    float L = 150.0f; // Comprimento
    float w = 1.0f;   // Largura
    glColor3f(0.8f, 0.7f, 0.5f); // Cor de madeira
    glBegin(GL_QUADS);
        glVertex2f(0, -w);
        glVertex2f(L, -w * 0.5f); // Afunila
        glVertex2f(L,  w * 0.5f);
        glVertex2f(0,  w);
    glEnd();
    
    glPopMatrix();
}

// --- 4. Funções de Lógica e Animação (Física) ---

/**
 * Converte coordenadas da Tela (SRT) para o Universo (SRU).
 */
void converteMouseParaUniverso(int x_tela, int y_tela, Vetor2D* posUniverso) {
    float mundoLarguraTotal = (MESA_LARGURA + 10) * 2;
    float mundoAlturaTotal = (MESA_ALTURA + 10) * 2;
    
    posUniverso->x = -mundoLarguraTotal / 2.0f + (x_tela / (float)gLarguraTela) * mundoLarguraTotal;
    // O eixo Y do mouse (SRT) é invertido em relação ao SRU.
    posUniverso->y = mundoAlturaTotal / 2.0f - (y_tela / (float)gAlturaTela) * mundoAlturaTotal;
}

/**
 * Arruma as bolas na posição inicial (triângulo).
 */
void arrumarBolas() {
    gBolas.clear();
    
    // 1. Bola Branca
    Bola branca;
    branca.pos = Vetor2D(-MESA_LARGURA / 2.0f, 0.0f);
    branca.vel = Vetor2D(0, 0);
    branca.raio = RAIO_BOLA;
    branca.massa = 1.0f;
    branca.r = 1.0f; branca.g = 1.0f; branca.b = 1.0f;
    branca.naMesa = true;
    branca.id = 0;
    gBolas.push_back(branca);

    // 2. Bolas Coloridas (Arrumadas em triângulo)
    Vetor2D posInicialTriangulo(MESA_LARGURA / 3.0f, 0.0f);
    int bolaId = 1;
    
    for (int i = 0; i < 5; i++) { // 5 fileiras
        for (int j = 0; j <= i; j++) {
            if (bolaId >= NUM_BOLAS) break;
            
            Bola bola;
            // Posição em "coordenadas" de triângulo, depois escalonada
            float x = posInicialTriangulo.x + i * (RAIO_BOLA * 2.0f * 0.866f); // 0.866 = cos(30)
            float y = posInicialTriangulo.y + (j * RAIO_BOLA * 2.0f) - (i * RAIO_BOLA);
            
            bola.pos = Vetor2D(x, y);
            bola.vel = Vetor2D(0, 0);
            bola.raio = RAIO_BOLA;
            bola.massa = 1.0f;
            // Cores aleatórias (exceto branca)
            bola.r = (rand() % 100) / 100.0f * 0.8f + 0.1f;
            bola.g = (rand() % 100) / 100.0f * 0.8f + 0.1f;
            bola.b = (rand() % 100) / 100.0f * 0.8f + 0.1f;
            bola.naMesa = true;
            bola.id = bolaId++;
            gBolas.push_back(bola);
        }
    }
    
    // Coloca a bola 8 (preta) no meio
    gBolas[5].r = 0.1f; gBolas[5].g = 0.1f; gBolas[5].b = 0.1f;
    
    gPontuacao = 0;
    gBolasEmMovimento = false;
    sprintf(gMensagemStatus, "Mova o mouse para mirar. Arraste para atirar.");
}

/**
 * Cria as caçapas (bolsos).
 */
void inicializaCacapas() {
    gCacapas.clear();
    float L = MESA_LARGURA;
    float A = MESA_ALTURA;
    gCacapas.push_back({{ -L, -A }, RAIO_CACAPA});
    gCacapas.push_back({{  L, -A }, RAIO_CACAPA});
    gCacapas.push_back({{ -L,  A }, RAIO_CACAPA});
    gCacapas.push_back({{  L,  A }, RAIO_CACAPA});
    gCacapas.push_back({{ 0.0f, -A - (RAIO_CACAPA/3) }, RAIO_CACAPA * 0.9f});
    gCacapas.push_back({{ 0.0f,  A + (RAIO_CACAPA/3) }, RAIO_CACAPA * 0.9f});
}

/**
 * O coração do jogo: a função de simulação de física.
 */
void anima(int value) {
    if (gBolasEmMovimento) {
        gBolasEmMovimento = false; // Assumimos que pararam, até provar o contrário
        
        for (int i = 0; i < gBolas.size(); i++) {
            if (!gBolas[i].naMesa) continue;

            // 1. Aplicar Atrito
            gBolas[i].vel = gBolas[i].vel * ATRITO;

            // Se a bola parar, zera a velocidade
            if (gBolas[i].vel.magnitude() < 0.01f) {
                gBolas[i].vel = Vetor2D(0, 0);
            } else {
                gBolasEmMovimento = true; // Pelo menos uma bola ainda se move
            }

            // 2. Atualizar Posição (Translação)
            gBolas[i].pos = gBolas[i].pos + gBolas[i].vel;

            // 3. Colisão com Bordas (Almofadas)
            if (gBolas[i].pos.x + RAIO_BOLA > MESA_LARGURA) {
                gBolas[i].pos.x = MESA_LARGURA - RAIO_BOLA;
                gBolas[i].vel.x *= -RESTITUICAO_BORDA;
            } else if (gBolas[i].pos.x - RAIO_BOLA < -MESA_LARGURA) {
                gBolas[i].pos.x = -MESA_LARGURA + RAIO_BOLA;
                gBolas[i].vel.x *= -RESTITUICAO_BORDA;
            }
            if (gBolas[i].pos.y + RAIO_BOLA > MESA_ALTURA) {
                gBolas[i].pos.y = MESA_ALTURA - RAIO_BOLA;
                gBolas[i].vel.y *= -RESTITUICAO_BORDA;
            } else if (gBolas[i].pos.y - RAIO_BOLA < -MESA_ALTURA) {
                gBolas[i].pos.y = -MESA_ALTURA + RAIO_BOLA;
                gBolas[i].vel.y *= -RESTITUICAO_BORDA;
            }

            // 4. Colisão com Caçapas
            for (const auto& cacapa : gCacapas) {
                if ((gBolas[i].pos - cacapa.pos).magnitude() < cacapa.raio) {
                    gBolas[i].naMesa = false;
                    gBolas[i].vel = Vetor2D(0, 0);
                    if (i != 0) { // Se não for a branca
                        gPontuacao++;
                    } else {
                        // Bola branca caiu, reseta ela
                        gBolas[i].pos = Vetor2D(-MESA_LARGURA / 2.0f, 0.0f);
                        gBolas[i].naMesa = true;
                    }
                }
            }

            // 5. Colisão Bola-Bola (Física de Impulso)
            for (int j = i + 1; j < gBolas.size(); j++) {
                if (!gBolas[j].naMesa) continue;

                Vetor2D diff = gBolas[i].pos - gBolas[j].pos;
                float dist = diff.magnitude();

                if (dist < RAIO_BOLA + RAIO_BOLA) {
                    // Colidiram!
                    
                    // a. Corrige superposição (para não grudar)
                    float overlap = (RAIO_BOLA + RAIO_BOLA) - dist;
                    Vetor2D normal = diff.normalizado();
                    gBolas[i].pos = gBolas[i].pos + normal * (overlap / 2.0f);
                    gBolas[j].pos = gBolas[j].pos - normal * (overlap / 2.0f);
                    
                    // b. Calcula o impulso (física de colisão elástica 2D)
                    Vetor2D vRel = gBolas[i].vel - gBolas[j].vel;
                    float velAlongNormal = vRel.dot(normal);

                    if (velAlongNormal > 0) continue; // Já estão se afastando

                    float impulso = -(1.0f + 1.0f) * velAlongNormal;
                    impulso /= (1.0f / gBolas[i].massa + 1.0f / gBolas[j].massa);
                    
                    Vetor2D impulsoVec = normal * impulso;

                    // c. Aplica o impulso às velocidades
                    gBolas[i].vel = gBolas[i].vel + impulsoVec * (1.0f / gBolas[i].massa);
                    gBolas[j].vel = gBolas[j].vel - impulsoVec * (1.0f / gBolas[j].massa);
                }
            }
        }
        
        // Atualiza mensagem se o jogo parou
        if (!gBolasEmMovimento) {
             sprintf(gMensagemStatus, "Mova o mouse para mirar. Arraste para atirar.");
        }
    }

    // Força o redesenho da tela
    glutPostRedisplay();
    
    // Reagenda a próxima chamada de animação
    glutTimerFunc(1000 / 60, anima, 0); // ~60 FPS
}

// --- 5. Callbacks do GLUT ---

/**
 * Função principal de desenho.
 */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); // Limpa o buffer
    
    glLoadIdentity(); // Reseta transformações

    // Desenha a mesa
    desenhaMesa();

    // Desenha todas as bolas
    for (const auto& bola : gBolas) {
        if (!bola.naMesa) continue;
        
        glPushMatrix();
        // 1. Translação: Move para a posição da bola
        glTranslatef(bola.pos.x, bola.pos.y, 0.0f);
        
        // 2. Desenha a bola na origem (0,0) do sistema transladado
        glColor3f(bola.r, bola.g, bola.b);
        desenhaCirculo(bola.raio, 30, GL_POLYGON);
        
        // (Opcional: desenha sombra)
        glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
        glTranslatef(-0.5f, -0.5f, -0.1f); // Desloca sombra
        desenhaCirculo(bola.raio, 30, GL_POLYGON);

        glPopMatrix();
    }
    
    // Desenha o taco
    desenhaTaco();
    
    // Desenha UI (Pontuação e Status)
    sprintf(gMensagemStatus, "Bolas Encacapadas: %d", gPontuacao);
    desenhaTexto(gMensagemStatus, -MESA_LARGURA - 5, MESA_ALTURA + 5, 1, 1, 1);
    
    if (!gBolasEmMovimento) {
         desenhaTexto("Mire (mouse), puxe (clique) e solte para atirar.", -70, -MESA_ALTURA - 5, 1, 1, 1);
    }
    
    // Troca os buffers (Double Buffering)
    glutSwapBuffers();
}

/**
 * Callback de clique do mouse (Pressionar e Soltar).
 */
void gerenciaMouse(int button, int state, int x_tela, int y_tela) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN && !gBolasEmMovimento) {
            // Pressionou: Começa a mirar e puxar
            gEstaMirando = true;
            converteMouseParaUniverso(x_tela, y_tela, &gPosCliqueTaco);
            gPosMouseSRU = gPosCliqueTaco; // Sincroniza
        } 
        else if (state == GLUT_UP && gEstaMirando) {
            // Soltou: Dá a tacada!
            gEstaMirando = false;
            if (gForcaTacada > 1.0f) { // Só atira se puxou o mínimo
                // Calcula a direção da tacada (do taco para a bola)
                Vetor2D dirTacada = (gBolas[0].pos - gPosMouseSRU).normalizado();
                
                // Aplica a força à velocidade da bola branca
                gBolas[0].vel = dirTacada * gForcaTacada * FATOR_FORCA_TACADA;
                gBolasEmMovimento = true;
                sprintf(gMensagemStatus, "Jogando...");
            }
            gForcaTacada = 0.0f;
        }
    }
}

/**
 * Callback de mouse se movendo sem clicar (Mira).
 */
void mousePassivo(int x_tela, int y_tela) {
    if (!gEstaMirando && !gBolasEmMovimento) {
        converteMouseParaUniverso(x_tela, y_tela, &gPosMouseSRU);
        gPosCliqueTaco = gPosMouseSRU; // O "puxar" começa de onde o mouse está
    }
}

/**
 * Callback de mouse se movendo clicado (Força).
 */
void mouseArrastando(int x_tela, int y_tela) {
    if (gEstaMirando) {
        // Atualiza a posição do mouse (para o taco seguir)
        converteMouseParaUniverso(x_tela, y_tela, &gPosMouseSRU);
    }
}

/**
 * Callback de teclado.
 */
void gerenciaTeclado(unsigned char key, int x, int y) {
    switch(key) {
        case 'r':
        case 'R':
            arrumarBolas();
            break;
    }
}

/**
 * Callback de redimensionamento da janela.
 */
void redimensiona(int w, int h) {
    gLarguraTela = w;
    gAlturaTela = h;
    glViewport(0, 0, w, h); // Atualiza o viewport
    
    // Atualiza a projeção (gluOrtho2D) para manter o SRU
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o SRU com uma "borda" para o taco e UI
    float mundoX = MESA_LARGURA + 10;
    float mundoY = MESA_ALTURA + 10;
    
    // Mantém a proporção da mesa
    if (w / (float)h > mundoX / mundoY) {
        mundoX = mundoY * (w / (float)h);
    } else {
        mundoY = mundoX * (h / (float)w);
    }
    
    gluOrtho2D(-mundoX, mundoX, -mundoY, mundoY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Função de inicialização do OpenGL.
 */
void inicializa(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fundo cinza escuro
    
    // Habilita transparência (para a sombra da bola)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    inicializaCacapas();
    arrumarBolas();
}

// --- 6. Função Principal ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(gLarguraTela, gAlturaTela);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Sinuca 2D - Trabalho de CG");

    inicializa();
    
    // Registra todos os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(gerenciaTeclado);
    glutMouseFunc(gerenciaMouse);
    glutPassiveMotionFunc(mousePassivo);
    glutMotionFunc(mouseArrastando);
    
    // Inicia a animação (chama 'anima' pela primeira vez)
    glutTimerFunc(1000 / 60, anima, 0);

    glutMainLoop(); // Inicia o loop principal do GLUT
    return 0;
}