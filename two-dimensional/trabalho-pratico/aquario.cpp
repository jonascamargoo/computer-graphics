/*
 * TRABALHO PRÁTICO I - COMPUTAÇÃO GRÁFICA
 *
 * TEMA: Jogo de Sinuca 2D vs. Computador (IA com Animação)
 *
 * DESCRIÇÃO:
 * Simula um jogo de sinuca 2D 1 vs. 1 contra um oponente de IA.
 * - O jogador e a IA jogam em turnos.
 * - A física de colisão (bola-bola, bola-borda) e atrito é simulada.
 * - A IA calcula a melhor tacada e executa uma ANIMAÇÃO de mira e tacada,
 * similar à do jogador.
 *
 * CONTROLES (Jogador):
 * - Mover o Mouse: Mira o taco.
 * - Clicar e Arrastar (Botão Esquerdo): Puxa o taco para definir a força.
 * - Soltar (Botão Esquerdo): Dá a tacada (apenas no seu turno).
 * - Teclado 'r' ou 'R': Reinicia o jogo.
 *
 * CONCEITOS APLICADOS (Baseado nas Práticas):
 * - Primitivas Gráficas (GL_POLYGON para bolas e mesa)
 * - Animação Contínua (glutTimerFunc, GLUT_DOUBLE)
 * - Interação (glutMouseFunc, glutPassiveMotionFunc, glutMotionFunc)
 * - Mapeamento SRT -> SRU (Conversão de coordenadas do mouse)
 * - Transformações Geométricas 2D (glTranslatef, glRotatef)
 * - Lógica de Simulação (Física de colisão, IA, Sistema de Turnos, Máquina de Estados)
 * - Exibição de Texto (glutBitmapCharacter)
 */

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>    // Para sprintf
#include <ctime>     // Para srand(time(NULL))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- 1. Definições, Estruturas e Constantes ---

// Estrutura para matemática de vetores 2D
struct Vetor2D {
    float x, y;
    Vetor2D(float x_ = 0.0f, float y_ = 0.0f) : x(x_), y(y_) {}

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
const float ATRITO = 0.995f; 
const float FATOR_FORCA_TACADA_JOGADOR = 0.15f;
const float FATOR_FORCA_TACADA_IA = 0.12f; 
const float RESTITUICAO_BORDA = 0.8f;

// Dimensões do Mundo (SRU)
const float MESA_LARGURA = 100.0f;
const float MESA_ALTURA = 50.0f;
const float MUNDO_BORDA = 10.0f; 

// Estados de Jogo e IA
enum Turno { JOGADOR, COMPUTADOR };
enum EstadoIA { IA_PENSANDO, IA_MIRANDO, IA_PUXANDO, IA_ATIRANDO };

// --- 2. Variáveis Globais ---

// Janela
int gLarguraTela = 1200;
int gAlturaTela = 650; 
float gProporcaoMundo = 1.0f;

// Atores
std::vector<Bola> gBolas;
std::vector<Cacapa> gCacapas;

// Estado de Jogo
Turno gTurnoAtual = JOGADOR;
bool gEstaMirando = false;
bool gBolasEmMovimento = false;
Vetor2D gPosMouseSRU;  
Vetor2D gPosCliqueTaco;
float gForcaTacada = 0.0f;
int gPontuacaoJogador = 0;
int gPontuacaoIA = 0;
char gMensagemStatus[100];

// Estado da Animação da IA
EstadoIA gEstadoIATurno = IA_PENSANDO;
int gIATimer = 0; 
Vetor2D gIADirTacada;    
float   gIAAnguloAlvo = 0.0f; 
float   gIAForcaAlvo = 0.0f;  
float   gIATacoAngulo = 0.0f; 
float   gIATacoForca = 0.0f;  

// --- 3. Funções de Desenho ---

/**
 * Desenha um texto na tela usando glutBitmapCharacter.
 */
void desenhaTexto(const char* texto, float x, float y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y); 
    const char* c = texto;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        c++;
    }
}

/**
 * Desenha um círculo preenchido (baseado no exemplo2.cpp).
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
    float L = MESA_LARGURA;
    float A = MESA_ALTURA;
    float B = MUNDO_BORDA;

    // Borda Marrom
    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(-L - B, -A - B);
        glVertex2f( L + B, -A - B);
        glVertex2f( L + B,  A + B);
        glVertex2f(-L - B,  A + B);
    glEnd();

    // Feltro Verde
    glColor3f(0.0f, 0.4f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(-L, -A);
        glVertex2f( L, -A);
        glVertex2f( L,  A);
        glVertex2f(-L,  A);
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
 * Desenha o taco de sinuca (controlado pelo Jogador OU pela IA).
 */
void desenhaTaco() {
    float angulo, forca;
    Vetor2D posBolaBranca = gBolas[0].pos;
    Vetor2D dirOpostaTaco;

    if (gTurnoAtual == JOGADOR && gEstaMirando && !gBolasEmMovimento) {
        // Lógica do JOGADOR (baseada no mouse)
        Vetor2D dirTaco = posBolaBranca - gPosMouseSRU; // Direção do mouse para a bola
        angulo = atan2(dirTaco.y, dirTaco.x) * 180.0f / M_PI;
        
        float distPuxada = (gPosMouseSRU - gPosCliqueTaco).magnitude();
        forca = fmin(distPuxada, 50.0f); 
        gForcaTacada = forca; 
        
        dirOpostaTaco = dirTaco.normalizado() * -1.0f; // Taco aponta na direção oposta

    } else if (gTurnoAtual == COMPUTADOR && gEstadoIATurno >= IA_MIRANDO && !gBolasEmMovimento) {
        // Lógica da IA (baseada nos estados de animação)
        angulo = gIATacoAngulo; // Usa o ângulo animado
        forca = gIATacoForca;   // Usa a força/puxada animada
        dirOpostaTaco = gIADirTacada * -1.0f; // Usa a direção calculada
    } else {
        return; // Não desenha o taco
    }

    // Posição do taco (puxado para trás pela força)
    Vetor2D posTaco = posBolaBranca + dirOpostaTaco * (RAIO_BOLA + 2.0f + forca);

    glPushMatrix();
    glTranslatef(posTaco.x, posTaco.y, 0.0f);
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);

    // Desenha o taco (um retângulo longo e fino)
    float L = 150.0f; // Comprimento
    float w = 1.0f;   // Largura
    glColor3f(0.8f, 0.7f, 0.5f); // Cor de madeira
    glBegin(GL_QUADS);
        glVertex2f(0, -w);
        glVertex2f(L, -w * 0.5f); 
        glVertex2f(L,  w * 0.5f);
        glVertex2f(0,  w);
    glEnd();
    
    glPopMatrix();
}

// --- 4. Funções de Lógica e Animação (Física e IA) ---

/**
 * Converte coordenadas da Tela (SRT) para o Universo (SRU).
 */
void converteMouseParaUniverso(int x_tela, int y_tela, Vetor2D* posUniverso) {
    float mundoX = (MESA_LARGURA + MUNDO_BORDA) * gProporcaoMundo;
    float mundoY = MESA_ALTURA + MUNDO_BORDA;
    if (gProporcaoMundo < 1.0) { 
        mundoY = (MESA_LARGURA + MUNDO_BORDA) / gProporcaoMundo;
    }
    
    posUniverso->x = -mundoX + (x_tela / (float)gLarguraTela) * (mundoX * 2.0f);
    posUniverso->y =  mundoY - (y_tela / (float)gAlturaTela) * (mundoY * 2.0f);
}

/**
 * Arruma as bolas na posição inicial (triângulo).
 */
void arrumarBolas() {
    gBolas.clear();
    
    // 1. Bola Branca
    gBolas.push_back({Vetor2D(-MESA_LARGURA / 2.0f, 0.0f), Vetor2D(0, 0), RAIO_BOLA, 1.0f, 1, 1, 1, true, 0});

    // 2. Bolas Coloridas
    Vetor2D posInicialTriangulo(MESA_LARGURA / 3.0f, 0.0f);
    int bolaId = 1;
    for (int i = 0; i < 5; i++) { 
        for (int j = 0; j <= i; j++) {
            if (bolaId >= NUM_BOLAS) break;
            float x = posInicialTriangulo.x + i * (RAIO_BOLA * 2.0f * 0.866f);
            float y = posInicialTriangulo.y + (j * RAIO_BOLA * 2.0f) - (i * RAIO_BOLA);
            gBolas.push_back({Vetor2D(x, y), Vetor2D(0, 0), RAIO_BOLA, 1.0f, 
                             (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f,
                             true, bolaId++});
        }
    }
    gBolas[5].r = 0.1f; gBolas[5].g = 0.1f; gBolas[5].b = 0.1f; // Bola 8 preta
    
    gPontuacaoJogador = 0;
    gPontuacaoIA = 0;
    gBolasEmMovimento = false;
    gTurnoAtual = JOGADOR;
    sprintf(gMensagemStatus, "Seu turno. Mire (mouse), puxe (clique) e solte.");
}

/**
 * Cria as caçapas (bolsos).
 */
void inicializaCacapas() {
    gCacapas.clear();
    float L = MESA_LARGURA + 1.0f; 
    float A = MESA_ALTURA + 1.0f;
    gCacapas.push_back({{ -L, -A }, RAIO_CACAPA});
    gCacapas.push_back({{  L, -A }, RAIO_CACAPA});
    gCacapas.push_back({{ -L,  A }, RAIO_CACAPA});
    gCacapas.push_back({{  L,  A }, RAIO_CACAPA});
    gCacapas.push_back({{ 0.0f, -A - (RAIO_CACAPA/3) }, RAIO_CACAPA * 0.9f});
    gCacapas.push_back({{ 0.0f,  A + (RAIO_CACAPA/3) }, RAIO_CACAPA * 0.9f});
}

/**
 * Lógica de "pensamento" da IA. Calcula a jogada e inicia a animação.
 */
void iaPensaJogada() {
    // 1. Encontrar a bola-alvo (bola de menor ID na mesa)
    int idAlvo = -1;
    for (int i = 1; i < gBolas.size(); i++) { // Começa em 1 (pula a branca)
        if (gBolas[i].naMesa) {
            idAlvo = i;
            break;
        }
    }
    if (idAlvo == -1) return; // Fim de jogo

    // 2. Encontrar a caçapa-alvo (mais próxima da bola-alvo)
    Cacapa cacapaAlvo = gCacapas[0];
    float menorDist = 9999.0f;
    for (const auto& cacapa : gCacapas) {
        float d = (gBolas[idAlvo].pos - cacapa.pos).magnitude();
        if (d < menorDist) {
            menorDist = d;
            cacapaAlvo = cacapa;
        }
    }

    // 3. Calcular a "Posição Fantasma" (onde a branca deve bater)
    Vetor2D dirAlvoCacapa = (cacapaAlvo.pos - gBolas[idAlvo].pos).normalizado();
    Vetor2D posFantasma = gBolas[idAlvo].pos - dirAlvoCacapa * (RAIO_BOLA * 2.0f);

    // 4. Salvar os resultados para a animação
    Vetor2D& posBolaBranca = gBolas[0].pos;
    gIADirTacada = (posFantasma - posBolaBranca).normalizado();
    gIAForcaAlvo = 45.0f; // IA usa uma força fixa
    
    // Calcula o ângulo visual do taco (oposto à direção da tacada)
    gIAAnguloAlvo = atan2(-gIADirTacada.y, -gIADirTacada.x) * 180.0f / M_PI;

    // 5. Iniciar a Animação
    gIATacoAngulo = gIAAnguloAlvo + 30.0f; // Começa 30 graus "errado" para animar
    gIATacoForca = 0.0f;
    gEstadoIATurno = IA_MIRANDO; // Próximo estado
    gIATimer = 60; // 1 segundo para mirar
    sprintf(gMensagemStatus, "IA esta mirando...");
}

/**
 * O coração do jogo: a função de simulação de física e turnos.
 */
void anima(int value) {
    if (gBolasEmMovimento) {
        bool algumaBolaSeMove = false;
        
        for (int i = 0; i < gBolas.size(); i++) {
            if (!gBolas[i].naMesa) continue;

            // 1. Aplicar Atrito
            gBolas[i].vel = gBolas[i].vel * ATRITO;

            if (gBolas[i].vel.magnitude() < 0.01f) {
                gBolas[i].vel = Vetor2D(0, 0);
            } else {
                algumaBolaSeMove = true;
            }

            // 2. Atualizar Posição (Translação)
            gBolas[i].pos = gBolas[i].pos + gBolas[i].vel;

            // 3. Colisão com Bordas
            if (gBolas[i].pos.x + RAIO_BOLA > MESA_LARGURA) gBolas[i].vel.x *= -RESTITUICAO_BORDA;
            if (gBolas[i].pos.x - RAIO_BOLA < -MESA_LARGURA) gBolas[i].vel.x *= -RESTITUICAO_BORDA;
            if (gBolas[i].pos.y + RAIO_BOLA > MESA_ALTURA) gBolas[i].vel.y *= -RESTITUICAO_BORDA;
            if (gBolas[i].pos.y - RAIO_BOLA < -MESA_ALTURA) gBolas[i].vel.y *= -RESTITUICAO_BORDA;

            // 4. Colisão com Caçapas
            for (const auto& cacapa : gCacapas) {
                if ((gBolas[i].pos - cacapa.pos).magnitude() < cacapa.raio) {
                    gBolas[i].naMesa = false;
                    gBolas[i].vel = Vetor2D(0, 0);
                    if (i != 0) { // Se não for a branca
                        if (gTurnoAtual == JOGADOR) gPontuacaoJogador++;
                        else gPontuacaoIA++;
                    } else {
                        gBolas[i].pos = Vetor2D(-MESA_LARGURA / 2.0f, 0.0f);
                        gBolas[i].naMesa = true;
                    }
                }
            }

            // 5. Colisão Bola-Bola (Física)
            for (int j = i + 1; j < gBolas.size(); j++) {
                if (!gBolas[j].naMesa) continue;
                Vetor2D diff = gBolas[i].pos - gBolas[j].pos;
                float dist = diff.magnitude();
                if (dist < RAIO_BOLA + RAIO_BOLA) {
                    float overlap = (RAIO_BOLA + RAIO_BOLA) - dist;
                    Vetor2D normal = diff.normalizado();
                    gBolas[i].pos = gBolas[i].pos + normal * (overlap / 2.0f);
                    gBolas[j].pos = gBolas[j].pos - normal * (overlap / 2.0f);
                    Vetor2D vRel = gBolas[i].vel - gBolas[j].vel;
                    float velAlongNormal = vRel.dot(normal);
                    if (velAlongNormal > 0) continue; 
                    float impulso = -(1.0f + 1.0f) * velAlongNormal;
                    impulso /= (1.0f / gBolas[i].massa + 1.0f / gBolas[j].massa);
                    Vetor2D impulsoVec = normal * impulso;
                    gBolas[i].vel = gBolas[i].vel + impulsoVec * (1.0f / gBolas[i].massa);
                    gBolas[j].vel = gBolas[j].vel - impulsoVec * (1.0f / gBolas[j].massa);
                }
            }
        }
        
        gBolasEmMovimento = algumaBolaSeMove;

        // Se o movimento ACABOU de parar, troca o turno
        if (!gBolasEmMovimento) {
            if (gTurnoAtual == JOGADOR) {
                gTurnoAtual = COMPUTADOR;
                gEstadoIATurno = IA_PENSANDO;
                gIATimer = 120; // 2 segundos para "pensar"
            } else {
                gTurnoAtual = JOGADOR;
            }
        }
    } 
    else {
        // Bolas PARADAS
        if (gTurnoAtual == COMPUTADOR) {
            
            // --- MÁQUINA DE ESTADOS DA ANIMAÇÃO DA IA ---
            switch (gEstadoIATurno) {
                case IA_PENSANDO:
                    sprintf(gMensagemStatus, "Computador esta pensando... (%d)", gIATimer / 60 + 1);
                    gIATimer--;
                    if (gIATimer <= 0) {
                        iaPensaJogada(); // Pensa e muda o estado para IA_MIRANDO
                    }
                    break;
                    
                case IA_MIRANDO:
                    sprintf(gMensagemStatus, "Computador esta mirando...");
                    gIATimer--;
                    // Anima o ângulo (interpolação linear simples)
                    gIATacoAngulo = gIATacoAngulo * 0.95f + gIAAnguloAlvo * 0.05f;
                    
                    if (gIATimer <= 0 || std::abs(gIATacoAngulo - gIAAnguloAlvo) < 0.5f) {
                        gIATacoAngulo = gIAAnguloAlvo; // Trava no alvo
                        gEstadoIATurno = IA_PUXANDO;
                        gIATimer = 90; // 1.5s para puxar
                    }
                    break;
                    
                case IA_PUXANDO:
                    sprintf(gMensagemStatus, "Computador esta puxando...");
                    gIATimer--;
                    // Anima a força (puxada)
                    gIATacoForca = gIATacoForca * 0.95f + gIAForcaAlvo * 0.05f;
                    
                    if (gIATimer <= 0 || std::abs(gIATacoForca - gIAForcaAlvo) < 0.5f) {
                        gIATacoForca = gIAForcaAlvo; // Trava na força
                        gEstadoIATurno = IA_ATIRANDO;
                        gIATimer = 10; // 10 frames para a tacada
                    }
                    break;
                    
                case IA_ATIRANDO:
                    sprintf(gMensagemStatus, "Computador ATIRA!");
                    gIATimer--;
                    // Anima a tacada (movimento rápido para frente)
                    gIATacoForca = gIATacoForca * 0.6f; // Move rápido para 0
                    
                    if (gIATimer <= 0) {
                        // APLICA A TACADA
                        gBolas[0].vel = gIADirTacada * gIAForcaAlvo * FATOR_FORCA_TACADA_IA;
                        gBolasEmMovimento = true;
                        gIATacoForca = 0.0f; // Reseta
                    }
                    break;
            }
        } else {
             // Turno do Jogador, bolas paradas
             sprintf(gMensagemStatus, "Seu turno. Mire (mouse) e atire.");
        }
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, anima, 0); 
}

// --- 5. Callbacks do GLUT ---

/**
 * Função principal de desenho.
 */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT); 
    glLoadIdentity(); 

    desenhaMesa();

    for (const auto& bola : gBolas) {
        if (!bola.naMesa) continue;
        glPushMatrix();
        glTranslatef(bola.pos.x, bola.pos.y, 0.0f);
        glColor3f(bola.r, bola.g, bola.b);
        desenhaCirculo(bola.raio, 30, GL_POLYGON);
        glPopMatrix();
    }
    
    // Esta função agora desenha o taco para AMBOS, jogador e IA
    desenhaTaco();
    
    // Desenha UI (Pontuação e Status)
    float uiY = MESA_ALTURA + (MUNDO_BORDA / 2.0f);
    char textoBuffer[100];
    sprintf(textoBuffer, "Jogador: %d", gPontuacaoJogador);
    desenhaTexto(textoBuffer, -MESA_LARGURA, uiY, 1, 1, 1);
    
    sprintf(textoBuffer, "Computador: %d", gPontuacaoIA);
    desenhaTexto(textoBuffer, MESA_LARGURA - 30.0f, uiY, 1, 1, 1);
    
    // Exibe a mensagem de status atual (controlada pela função anima)
    desenhaTexto(gMensagemStatus, -70, -MESA_ALTURA - (MUNDO_BORDA / 2.0f), 1, 1, 1);
    
    glutSwapBuffers();
}

/**
 * Callback de clique do mouse (Pressionar e Soltar).
 */
void gerenciaMouse(int button, int state, int x_tela, int y_tela) {
    if (gTurnoAtual != JOGADOR || gBolasEmMovimento) return;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            gEstaMirando = true;
            converteMouseParaUniverso(x_tela, y_tela, &gPosCliqueTaco);
            gPosMouseSRU = gPosCliqueTaco;
        } 
        else if (state == GLUT_UP && gEstaMirando) {
            gEstaMirando = false;
            if (gForcaTacada > 1.0f) { 
                Vetor2D dirTacada = (gBolas[0].pos - gPosMouseSRU).normalizado();
                gBolas[0].vel = dirTacada * gForcaTacada * FATOR_FORCA_TACADA_JOGADOR;
                gBolasEmMovimento = true;
                sprintf(gMensagemStatus, "Jogador jogando...");
            }
            gForcaTacada = 0.0f;
        }
    }
}

/**
 * Callback de mouse se movendo sem clicar (Mira).
 */
void mousePassivo(int x_tela, int y_tela) {
    if (gTurnoAtual != JOGADOR || gBolasEmMovimento || gEstaMirando) return;
    converteMouseParaUniverso(x_tela, y_tela, &gPosMouseSRU);
    gPosCliqueTaco = gPosMouseSRU; 
}

/**
 * Callback de mouse se movendo clicado (Força).
 */
void mouseArrastando(int x_tela, int y_tela) {
    if (gTurnoAtual != JOGADOR || !gEstaMirando) return;
    converteMouseParaUniverso(x_tela, y_tela, &gPosMouseSRU);
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
    glViewport(0, 0, w, h); 
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float mundoX = MESA_LARGURA + MUNDO_BORDA;
    float mundoY = MESA_ALTURA + MUNDO_BORDA;
    gProporcaoMundo = (float)w / (float)h / (mundoX / mundoY);

    if (gProporcaoMundo > 1.0) { 
        mundoX *= gProporcaoMundo;
    } else { 
        mundoY /= gProporcaoMundo;
    }
    
    gluOrtho2D(-mundoX, mundoX, -mundoY, mundoY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Função de inicialização do OpenGL.
 */
void inicializa(void) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
    srand(time(NULL));
    
    inicializaCacapas();
    arrumarBolas();
}

// --- 6. Função Principal ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(gLarguraTela, gAlturaTela);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Sinuca 2D vs. IA (com Animacao) - Trabalho de CG");

    inicializa();
    
    glutDisplayFunc(display);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(gerenciaTeclado);
    glutMouseFunc(gerenciaMouse);
    glutPassiveMotionFunc(mousePassivo);
    glutMotionFunc(mouseArrastando);
    
    glutTimerFunc(1000 / 60, anima, 0);

    glutMainLoop();
    return 0;
}