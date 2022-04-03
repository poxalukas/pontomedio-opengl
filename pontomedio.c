// Traçado de Retas Otimizado
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

void init();
void display();
void drawLine(int x1, int y1, int x2, int y2);

int main(int argc, char** argv) {
    glutInit(&argc, argv);                              
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (512, 512);                      
    glutInitWindowPosition (100, 100);                  
    glutCreateWindow ("Tracado de Linhas Otimizado");  
    init();
    glutDisplayFunc(display);
    glutMainLoop();   
}

void init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho (0, 512, 0, 512, -1 ,1);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha uma linha preta do ponto (5, 10) até (400, 300)
    glColor3f (0.0, 0.0, 0.0);
    drawLine(5, 10, 400, 300);

    // Desenha uma linha vermelha do ponto (0, 0) até (300, 200)
    glColor3f (1.0, 0.0, 0.0);
    drawLine(0, 0, 300, 200);

    // Desenha uma linha azul do ponto (50, 50) até (400, 450)
    glColor3f (0.0, 0.0, 1.0);
    drawLine(50, 50, 400, 450);

    // Desenha uma linha verde do ponto (10, 10) até (20, 250)
    glColor3f (0.2, 0.7, 0.2);
    drawLine(10, 10, 20, 250);

    glFlush();
}

// Troca os pontos
void swapPoints(int* x1, int* y1, int* x2, int* y2) {
    int px2 = *x2;
    int py2 = *y2;

    *x2 = *x1;
    *y2 = *y1;

    *x1 = px2;
    *y1 = py2;
}

// Desenha linhas de 0º a 90º
void drawLine(int x1, int y1, int x2, int y2) {
    // Se o ponto 1 estiver à esquerda
    // do ponto 2, troca os pontos
    if (x1 >= x2)
      swapPoints(&x1, &y1, &x2, &y2);

    // Cálculos iniciais para a determinação da reta
    int dy = y2 - y1; // Variação de Y
    int dx = x2 - x1; // Variação de X
    float dy_dx = (float) dy / (float) dx;  // Derivada de Y em função de X
    
    // As variações são multiplicadas por 2 porque
    // a variável de decisão é, também, multiplicada por 2 para
    // se livrar das frações
    int dE = 2 * dy; // Variação da variável de decisão quando E é escolhido
    int dNE = 2 * (dy - dx); // Variação da variável de decisão quando NE é escolhido

    // Espelhamento (eixo x = y)
    // Se a inclinação da reta for maior que 45º, calcula pela linha espelhada
    // E pinta a linha original pelo "reespelhamento"
    int espelhada = dy_dx > 1;
    if (espelhada) {
      int px1 = x1;
      x1 = y1;
      y1 = px1;

      int px2 = x2;
      x2 = y2;
      y2 = px2;

      // Recalcula os coeficientes da linha, agora espelhada
      dy = y2 - y1;
      dx = x2 - x1;
      dy_dx = dy/dx;

      dE = 2 * dy;
      dNE = 2 * (dy - dx);
    }

    printf("DERIVADA: %f\n", dy_dx);
    printf("LINHA CALCULADA: (%d, %d) -> (%d, %d)\n\n", x1, y1, x2, y2);

    glBegin(GL_POINTS);

      int c_x = x1, c_y = y1; // Ponto atual
      int d = 2*(dy-dx/2); // Variável de decisão .:. Multiplicada por 2 para eliminar a fração
      while(c_x <= x2) {
        // Pintamos o ponto atual
        if (espelhada) // Se a linha tiver sido espelhada, reespelhamos
          glVertex2i(c_y, c_x);
        else 
          glVertex2i(c_x, c_y);

        // Calculamos o próximo
        // Primeiro, decidimos se pegaremos o ponto E ou NE
        if (d <= 0) { // Escolhemos o ponto E
            c_x += 1; // Para o ponto E, simplesmente incrementamos c_x em 1
            d += dE; // Incrementa a variável de decisão
        } else { // Escolhemos o ponto NE
            c_x += 1; c_y += 1; // Para o ponto NE, incrementam-se c_x e c_y em 1
            d += dNE; // Incrementa a variável de decião
        }
      }

    glEnd();
}