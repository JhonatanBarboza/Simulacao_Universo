#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define NUM_PLANETAS 10000
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
float massa_estrela = 100000.0f;  // Massa da estrela
float raio_estrela = 30.0f;
int bordas = 2000;

// Função para verificar colisões entre planetas
void verificar_colisoes() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (!planetas[i].ativo) continue;

        for (int j = i + 1; j < NUM_PLANETAS; j++) {
            if (!planetas[j].ativo) continue;

            // Calcular a distância entre os dois planetas
            float dx = planetas[j].x - planetas[i].x;
            float dy = planetas[j].y - planetas[i].y;
            float distancia = sqrt(dx * dx + dy * dy);

            // Verificar se há colisão (distância menor que a soma dos raios)
            if (distancia <= (planetas[i].raio + planetas[j].raio)) {
                // Colisão detectada, combinar os dois planetas
                if (planetas[i].massa >= planetas[j].massa) {
                    // Planeta i absorve o planeta j
                    float massa_total = planetas[i].massa + planetas[j].massa;

                    // Conservação do momento para calcular a nova velocidade
                    planetas[i].vx = (planetas[i].massa * planetas[i].vx + planetas[j].massa * planetas[j].vx) / massa_total;
                    planetas[i].vy = (planetas[i].massa * planetas[i].vy + planetas[j].massa * planetas[j].vy) / massa_total;

                    planetas[i].massa = massa_total;

                    // Atualizar o raio com base na nova massa (aproximadamente)
                    planetas[i].raio = pow(massa_total, 1.0f / 3.0f);

                    planetas[j].ativo = 0;  // Planeta j deixa de existir
                } else {
                    // Planeta j absorve o planeta i
                    float massa_total = planetas[i].massa + planetas[j].massa;

                    planetas[j].vx = (planetas[i].massa * planetas[i].vx + planetas[j].massa * planetas[j].vx) / massa_total;
                    planetas[j].vy = (planetas[i].massa * planetas[i].vy + planetas[j].massa * planetas[j].vy) / massa_total;

                    planetas[j].massa = massa_total;
                    planetas[j].raio = pow(massa_total, 1.0f / 3.0f);  // Atualizar o raio

                    planetas[i].ativo = 0;  // Planeta i deixa de existir
                }
            }
        }
    }
}

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
    //verificar_colisoes();
}

// Função para gerar um valor aleatório fora do intervalo
float gerar_valor_com_limite() {
    float valor;
    int borda = bordas-500;
    int intervalo = 500;//(bordas /10) * 2;  // Limite de exclusão ao redor do centro
    do {
        valor = (rand() % (int)(2 * borda + 1));
    } while (valor >= -intervalo && valor <= intervalo);  // Excluir o intervalo [-200, 200]

    return valor;
}

// Função para inicializar os planetas com posições, velocidades e massas
void inicializar_planetas() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        planetas[i].raio = 15;  // Raio inicial do planeta
        planetas[i].massa = 100;  // Massa do planeta (não pode ser zero)
        planetas[i].x = gerar_valor_com_limite();  // Posição no eixo X
        planetas[i].y = gerar_valor_com_limite();  // Posição no eixo Y
        planetas[i].vx = -8 ;//(rand() % 10) - 5;  // Velocidade inicial em X
        planetas[i].vy = 9 ;//(rand() % 10) - 5;//(G * massa_estrela / (100.0f + (i * 50.0f)));  // Velocidade inicial em Y
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

// Função para desenhar um quadrado
void desenhar_quadrado(float cx, float cy, float tamanho) {
    float metade_tamanho = tamanho / 2.0f;

    glBegin(GL_POLYGON);
        glVertex2f(cx - metade_tamanho, cy - metade_tamanho); // canto inferior esquerdo
        glVertex2f(cx + metade_tamanho, cy - metade_tamanho); // canto inferior direito
        glVertex2f(cx + metade_tamanho, cy + metade_tamanho); // canto superior direito
        glVertex2f(cx - metade_tamanho, cy + metade_tamanho); // canto superior esquerdo
    glEnd();
}

// Desenho na tela
void exibir() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar a estrela no centro
    glColor3f(1.1f, 1.1f, 0.1f);  // Amarelo escuro
    desenhar_circulo(0.0f, 0.0f, raio_estrela);

    // Desenhar os planetas
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (planetas[i].ativo) {
            glColor3f(1.0f, 1.0f, 1.0f);  // Cor dos planetas
            desenhar_quadrado(planetas[i].x, planetas[i].y, planetas[i].raio);  // Desenho do planeta
        }
    }

    glutSwapBuffers();
}

// Função Timer para atualização
void timer(int valor) {
    atualizar_planetas();
    glutPostRedisplay();
    glutTimerFunc(1000 / 120, timer, 0);  // 60 FPS
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
