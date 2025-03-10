#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define NUM_ESTRELAS 2
#define NUM_PLANETAS 100000
#define G 6.67430  // Constante gravitacional

// Estrutura para armazenar as propriedades de cada planeta
typedef struct Estrela_ {
    float x, y;           // Posição do planeta
    float vx, vy;         // Velocidade do planeta
    float massa;          // Massa do planeta
    float raio;           // Raio do planeta
    int ativo;            // Planeta ativo ou não
} Estrela;

// Estrutura para armazenar as propriedades de cada planeta
typedef struct Planeta_{
    float x, y;           // Posição do planeta
    float vx, vy;         // Velocidade do planeta
    float massa;          // Massa do planeta
    float raio;           // Raio do planeta
    int ativo;            // Planeta ativo ou não
} Planeta;

Estrela estrelas [NUM_ESTRELAS];
Planeta planetas [NUM_PLANETAS];
int bordas = 5000;

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

// Função para verificar colisões entre estrelas
void verificar_colisoes_estrelas() {
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        if (!estrelas[i].ativo) continue;

        for (int j = i + 1; j < NUM_ESTRELAS; j++) {
            if (!estrelas[j].ativo) continue;

            // Calcular a distância entre as duas estrelas
            float dx = estrelas[j].x - estrelas[i].x;
            float dy = estrelas[j].y - estrelas[i].y;
            float distancia = sqrt(dx * dx + dy * dy);

            // Verificar se há colisão (distância menor que a soma dos raios)
            if (distancia <= (estrelas[i].raio*5 + estrelas[j].raio*5)) {
                // Colisão detectada, combinar as duas estrelas
                if (estrelas[i].massa >= estrelas[j].massa) {
                    // Estrela i absorve a estrela j
                    float massa_total = estrelas[i].massa + estrelas[j].massa;

                    // Conservação do momento para calcular a nova velocidade
                    estrelas[i].vx = (estrelas[i].massa * estrelas[i].vx + estrelas[j].massa * estrelas[j].vx) / massa_total;
                    estrelas[i].vy = (estrelas[i].massa * estrelas[i].vy + estrelas[j].massa * estrelas[j].vy) / massa_total;

                    estrelas[i].massa = massa_total;

                    // Atualizar o raio com base na nova massa (aproximadamente)
                    estrelas[i].raio = pow(massa_total, 1.0f / 3.0f);

                    estrelas[j].ativo = 0;  // Estrela j deixa de existir
                } else {
                    // Estrela j absorve a estrela i
                    float massa_total = estrelas[i].massa + estrelas[j].massa;

                    estrelas[j].vx = (estrelas[i].massa * estrelas[i].vx + estrelas[j].massa * estrelas[j].vx) / massa_total;
                    estrelas[j].vy = (estrelas[i].massa * estrelas[i].vy + estrelas[j].massa * estrelas[j].vy) / massa_total;

                    estrelas[j].massa = massa_total;
                    estrelas[j].raio = pow(massa_total, 1.0f / 3.0f);  // Atualizar o raio

                    estrelas[i].ativo = 0;  // Estrela i deixa de existir
                }
            }
        }
    }
}

float distancia(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Função para calcular a força gravitacional entre duas estrelas
void calcular_gravidade_estrela(Estrela* a, Estrela* b) {
    float dx = b->x - a->x;  // Distância no eixo X entre estrela a e estrela b
    float dy = b->y - a->y;  // Distância no eixo Y entre estrela a e estrela b
    float distancia = sqrt(dx * dx + dy * dy);

    if (distancia > 0.01f) {  // Evitar divisão por zero
        // Força gravitacional
        float forca = G * a->massa * b->massa / (distancia * distancia);

        // Aceleração no eixo X e Y para a (direção oposta à estrela b)
        float ax_a = forca * dx / distancia / a->massa;
        float ay_a = forca * dy / distancia / a->massa;

        // Aceleração no eixo X e Y para b (direção oposta à estrela a)
        float ax_b = -forca * dx / distancia / b->massa;
        float ay_b = -forca * dy / distancia / b->massa;

        // Atualizar as velocidades das estrelas
        a->vx += ax_a;
        a->vy += ay_a;

        b->vx += ax_b;
        b->vy += ay_b;
    }
}

/*
// Função para calcular a força gravitacional de cada planeta em relação a todas as estrelas
void calcular_gravidade(Planeta* p) {
    float ax = 0, ay = 0;  // Acelerações resultantes no planeta

    // Somar a atração gravitacional de todas as estrelas
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        if (!estrelas[i].ativo) continue;

        float dx = estrelas[i].x - p->x;  // Distância no eixo X entre planeta e estrela
        float dy = estrelas[i].y - p->y;  // Distância no eixo Y entre planeta e estrela
        float distancia = sqrt(dx * dx + dy * dy);

        if (distancia > 0.01f) {  // Evitar divisão por zero
            // Força gravitacional
            float forca = G * estrelas[i].massa * p->massa / (distancia * distancia);

            // Aceleração causada pela estrela i
            ax += forca * dx / distancia / p->massa;
            ay += forca * dy / distancia / p->massa;
        }
    }

    // Atualizar a velocidade do planeta com as acelerações resultantes
    p->vx += ax;
    p->vy += ay;
}*/

// Função para atualizar a posição das estrelas
void atualizar_estrelas() {
    for (int i = 0; i < NUM_ESTRELAS; i++) {
        if (!estrelas[i].ativo) continue;

        // Calcular a interação gravitacional com as outras estrelas
        for (int j = i + 1; j < NUM_ESTRELAS; j++) {
            if (estrelas[j].ativo) {
                calcular_gravidade_estrela(&estrelas[i], &estrelas[j]);
            }
        }

        // Atualizar a posição das estrelas com base na velocidade
        estrelas[i].x += estrelas[i].vx;
        estrelas[i].y += estrelas[i].vy;
    }
    verificar_colisoes_estrelas();
}


void atualizar_planetas() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (planetas[i].x > bordas*4 && planetas[i].y > bordas*4){
            planetas[i].ativo = 0;
        }
        if (!planetas[i].ativo) continue;

        float ax = 0, ay = 0;  // Acelerações resultantes

        // Força gravitacional das duas estrelas
        for (int j = 0; j < NUM_ESTRELAS; j++) {
            if (!estrelas[j].ativo) continue;

            float dx = estrelas[j].x - planetas[i].x;
            float dy = estrelas[j].y - planetas[i].y;
            float distancia = sqrt(dx * dx + dy * dy);

            if (distancia > 0.01f) {  // Evitar divisão por zero
                float forca = G * (estrelas[j].massa * planetas[i].massa) / (distancia * distancia);

                // Aceleração causada pela estrela j
                ax += forca * dx / distancia;
                ay += forca * dy / distancia;
            }
        }

        // Atualizar velocidade com a aceleração calculada
        planetas[i].vx += ax;
        planetas[i].vy += ay;

        // Atualizar posição dos planetas
        planetas[i].x += planetas[i].vx;
        planetas[i].y += planetas[i].vy;
    }
}

// Função para inicializar os planetas com posições, velocidades e massas
void inicializar_estrelas() {

    estrelas[0].massa = 90000.0f;  // Massa do planeta (não pode ser zero)
    estrelas[0].raio = pow(estrelas[0].massa, 1.0f / 3.0f);  // Raio inicial do planeta
    estrelas[0].x = -bordas+7000;  // Posição no eixo X
    estrelas[0].y = -bordas;  // Posição no eixo Y
    estrelas[0].vx = 3;//(rand() % 10) - 5;  // Velocidade inicial em X
    estrelas[0].vy = 3;//(rand() % 10) - 5;//(G * massa_estrela / (100.0f + (i * 50.0f)));  // Velocidade inicial em Y
    estrelas[0].ativo = 1;  // Planeta começa ativo

    estrelas[1].massa = 90000.0f;  // Massa do planeta (não pode ser zero)
    estrelas[1].raio = pow(estrelas[1].massa, 1.0f / 3.0f);  // Raio inicial do planeta
    estrelas[1].x = bordas-7000;  // Posição no eixo X
    estrelas[1].y = bordas;  // Posição no eixo Y
    estrelas[1].vx = -3;//(rand() % 10) - 5;  // Velocidade inicial em X
    estrelas[1].vy = -3;//(rand() % 10) - 5;//(G * massa_estrela / (100.0f + (i * 50.0f)));  // Velocidade inicial em Y
    estrelas[1].ativo = 1;  // Planeta começa ativo
}

// Função para gerar um valor aleatório fora do intervalo
float gerar_valor_com_limite() {
    float valor;
    int borda = bordas + 1000;
    int intervalo = 5000;//(bordas /10) * 2;  // Limite de exclusão ao redor do centro
    do {
        valor = (rand() % (int)(2*borda + 1));
    } while (valor >= -intervalo && valor <= intervalo);  // Excluir o intervalo [-200, 200]

    return valor;
}

// Função para inicializar os planetas com posições, velocidades e massas
void inicializar_planetas() {
    for (int i = 0; i < NUM_PLANETAS; i++) {
        planetas[i].massa = 100;//rand() % 10000 + 1;  // Massa do planeta (não pode ser zero)
        planetas[i].raio = 10;//pow(estrelas[i].massa, 1.0f / 3.0f); // Raio inicial do planeta
        planetas[i].x = gerar_valor_com_limite();  // Posição no eixo X
        planetas[i].y = gerar_valor_com_limite();  // Posição no eixo Y
        planetas[i].vx = 100;//(rand() % 10) - 5;  // Velocidade inicial em X
        planetas[i].vy = -100;//(rand() % 10) - 5;//(G * massa_estrela / (100.0f + (i * 50.0f)));  // Velocidade inicial em Y
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


    for (int i = 0; i < NUM_ESTRELAS; i++) {
        if (estrelas[i].ativo) {
            glColor3f(1.0f, 1.0f, 1.0f);  // Amarelo escuro
            desenhar_circulo(estrelas[i].x, estrelas[i].y, estrelas[i].raio*2);
        }
    }

    // Desenhar os planetas
    for (int i = 0; i < NUM_PLANETAS; i++) {
        if (planetas[i].ativo) {
            glColor3f(1.0f, 1.0f, 1.0f);  // Cor dos planetas
            desenhar_quadrado(planetas[i].x, planetas[i].y, planetas[i].raio*5);  // Desenho do planeta
        }
    }



    glutSwapBuffers();
}

// Função Timer para atualização
void timer(int valor) {
    atualizar_estrelas();
    atualizar_planetas();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);  // 60 FPS
}

// Configurações do OpenGL
void inicializarOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);  // Fundo preto
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-bordas * 5, bordas * 5, -bordas * 5, bordas * 5);  // Ambiente 2D
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
    inicializar_estrelas();
    inicializarOpenGL();

    // Registrar callbacks
    glutDisplayFunc(exibir);
    glutTimerFunc(0, timer, 0);

    // Iniciar loop principal
    glutMainLoop();
    return 0;
}
