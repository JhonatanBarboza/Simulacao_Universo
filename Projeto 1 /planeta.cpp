#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define NUM_PLANETAS 10
#define G 6.67430  // Constante gravitacional

// Estrutura para armazenar as propriedades de cada planeta
typedef struct {
    float x, y;           // Posição do planeta
    float vx, vy;         // Velocidade do planeta
    float massa;          // Massa do planeta
    float raio;           // Raio do planeta
    int ativo;            // Planeta ativo ou não
} Planeta;

Planeta planetas[NUM_PLANETAS];
float massa_estrela = 12000.0f;  // Massa da estrela
float raio_estrela = pow(massa_estrela, 1.0f / 3.0f);
int bordas = 500;

// Função para calcular a força gravitacional
void calcular_gravidade(Planeta* p) {
    float dx = 0.0f - p->x;  // Distância no eixo X entre planeta e estrela
    float dy = 0.0f - p->y;  // Distância no eixo Y entre planeta e estrela
    float distancia = sqrt(dx * dx + dy * dy);

    // Força gravitacional
    float forca = G * massa_estrela * p->massa / (distancia * distancia);

    // Acelerações no eixo X e Y
    float ax = forca * dx / distancia / p->massa;
    float ay = forca * dy / distancia / p->massa;

    // Atualizar as velocidades do planeta
    p->vx += ax;
    p->vy += ay;
}

// Função para atualizar a posição dos planetas
void atualizar_planetas() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (!planetas[i].ativo) continue;

        calcular_gravidade(&planetas[i]);

        // Atualizar a posição com base na velocidade
        planetas[i].x += planetas[i].vx;
        planetas[i].y += planetas[i].vy;
    }
}

// Função para inicializar os planetas com posições, velocidades e massas
void inicializar_planetas() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        planetas[i].raio = (rand() % 11) + 5;  // Raio inicial do planeta
        planetas[i].massa = rand() % 100 + 1;  // Massa do planeta (não pode ser zero)
        planetas[i].x = rand() % 700 + 200;  // Posição no eixo X
        planetas[i].y = rand() % 700 + 200;  // Posição no eixo Y
        planetas[i].vx = -7;  // Velocidade inicial em X
        planetas[i].vy = 7;//(G * massa_estrela / (100.0f + (i * 50.0f)));  // Velocidade inicial em Y
        planetas[i].ativo = 1;  // Planeta começa ativo
    }
}

// Função para desenhar um círculo (estrela/planetas)
void desenhar_circulo(float cx, float cy, float raio) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * M_PI * i / 100;
        float x = raio * cosf(theta);
        float y = raio * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Desenho na tela
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar a estrela no centro
    glColor3f(1.0f, 1.0f, 0.0f);  // Amarelo escuro
    desenhar_circulo(0.0f, 0.0f, raio_estrela*2);

    // Desenhar os planetas
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (planetas[i].ativo) {
            glColor3f(1.0f, 1.0f, 1.0f);  // Cor dos planetas
            desenhar_circulo(planetas[i].x, planetas[i].y, planetas[i].raio*1.2);  // Desenho do planeta
        }
    }

    glutSwapBuffers();
}

// Função Timer para atualização
void timer(int valor) {
    atualizar_planetas();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);  // 60 FPS
}

// Configurações do OpenGL
void inicializarOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Fundo preto
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-bordas * 3, bordas * 3, -bordas * 3, bordas * 3);  // Ambiente 2D
}

// Função principal
int main(int argc, char** argv) {
    srand(time(0));  // Inicializar o gerador de números aleatórios

    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 900);
    glutCreateWindow("Simulacao de orbita com Gravidade");

    // Inicializar planetas e OpenGL
    inicializar_planetas();
    inicializarOpenGL();

    // Registrar callbacks
    glutDisplayFunc(exibir);
    glutTimerFunc(0, timer, 0);

    // Iniciar loop principal
    glutMainLoop();
    return 0;
}
