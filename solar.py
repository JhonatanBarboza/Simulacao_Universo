import pygame
import math
import random

# Constantes
NUM_PLANETAS = 10
G = 6.67430  # Constante gravitacional
MASSA_ESTRELA = 12000.0  # Massa da estrela
RAIO_ESTRELA = MASSA_ESTRELA ** (1.0 / 3.0)
BORDAS = 500
LARGURA_TELA = 900
ALTURA_TELA = 900

# Classe Planeta
class Planeta:
    def __init__(self):
        self.raio = random.randint(5, 15)
        self.massa = random.randint(1, 100)
        self.x = random.uniform(-350, 350)
        self.y = random.uniform(-350, 350)
        self.vx = random.uniform(-2, 2)
        self.vy = random.uniform(-2, 2)
        self.ativo = True
    
    def calcular_gravidade(self):
        dx = -self.x
        dy = -self.y
        distancia = math.sqrt(dx**2 + dy**2)
        if distancia == 0:
            return  # Evitar divisão por zero
        forca = G * MASSA_ESTRELA * self.massa / (distancia ** 2)
        ax = forca * dx / distancia / self.massa
        ay = forca * dy / distancia / self.massa
        self.vx += ax
        self.vy += ay

    def atualizar_posicao(self):
        if self.ativo:
            self.calcular_gravidade()
            self.x += self.vx
            self.y += self.vy

# Função para desenhar o círculo (planetas e estrela)
def desenhar_circulo(screen, cor, cx, cy, raio):
    pygame.draw.circle(screen, cor, (int(cx + LARGURA_TELA / 2), int(cy + ALTURA_TELA / 2)), int(raio))

# Função principal de simulação
def simulacao():
    pygame.init()
    screen = pygame.display.set_mode((LARGURA_TELA, ALTURA_TELA))
    clock = pygame.time.Clock()

    # Inicializar planetas
    planetas = [Planeta() for _ in range(NUM_PLANETAS)]

    rodando = True
    while rodando:
        for evento in pygame.event.get():
            if evento.type == pygame.QUIT:
                rodando = False
        
        screen.fill((0, 0, 0))  # Fundo preto
        
        # Desenhar estrela
        desenhar_circulo(screen, (255, 255, 0), 0, 0, RAIO_ESTRELA * 2)

        # Atualizar e desenhar planetas
        for planeta in planetas:
            planeta.atualizar_posicao()
            desenhar_circulo(screen, (255, 255, 255), planeta.x, planeta.y, planeta.raio)

        pygame.display.flip()
        clock.tick(60)  # 60 FPS

    pygame.quit()

if __name__ == "__main__":
    simulacao()
