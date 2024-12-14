//----------------------------------------------------------------------------------------------
// Projeto: Agenda Eletrônica com ESP32 e DisplayLCD16x2
// Autor: João Vitor Barros Bertin
//----------------------------------------------------------------------------------------------

//------------------------------ IMPORTANTE!!! -------------------------------------------------
/* Caso vá fazer com o display 20x4 sem o módulo I2C, basta trocar o display da imagem (16x2) 
pelo diplay 20x4, pois a ligação dos dois é a mesma. Entretanto, o simulador que eu usei 
(Tinkerpad) não possui o display 20x4 no seu acervo de componentes, por isso a ausência de seu 
circuito */

// Incluindo biblioteca do display
#include <LiquidCrystal.h>

// Definindo variáveis dos pinos do display
LiquidCrystal lcd{12, 13, 5, 4, 3, 2};
// Definindo variáveis das linhas e colunas do Teclado Matricial
int linha1=4, linha2=5, linha3=6, linha4=7;
int coluna1=8, coluna2=9, coluna3=10, coluna4=11;
// Variável seletora de funções
int selec_funcao=0;
// Definindo variáveis dos diferentes conjuntos de botões
/* É necessário usar variáveis de primeiro botão para não 
   dar conflito nos códigos das letras, mais abaixo. 
   EX: A seria 0 e 1, enquanto J seria 1 e 0 e os dois 
   seriam impressos, causando conflito */
int primbtn1=0, primbtn2=0, primbtn0=0,
btn1=0, btn2=0, btn3=0, btnA=0,  
btn4=0, btn5=0, btn6=0, btnB=0, 
btn7=0, btn8=0, btn9=0, btnC=0,
btnast, btn0=0, btnhash, btnD=0;

// Sistema para descer linha no display
int linha_atual=0, contador_coluna=0;

void setup()
{
// Iniciando display LCD - 20 colunas, 4 linhas
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando.");
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(500);
  lcd.clear();
  delay(100);
// Inicializando monitor serial
  Serial.begin(9600);
  Serial.print("Iniciando.");
  delay(200);
  Serial.print(".");
  delay(200);
  Serial.println(".");
  delay(100);
// Definindo todos os fios das linhas como saídas
  pinMode(linha1, OUTPUT);
  pinMode(linha2, OUTPUT);
  pinMode(linha3, OUTPUT);
  pinMode(linha4, OUTPUT);
// Definindo todos os fios das colunas como entradas
  pinMode(coluna1, INPUT);
  pinMode(coluna2, INPUT);
  pinMode(coluna3, INPUT);
  pinMode(coluna4, INPUT);
}

void loop()
{
// Definição da variável do contador
  int contador=0;
// Redefinindo as variáveis dos códigos das letras como 0 a cada loop
  primbtn1=0, primbtn2=0, primbtn0=0,
  btn1=0, btn2=0, btn3=0, btnA=0,
  btn4=0, btn5=0, btn6=0, btnB=0,
  btn7=0, btn8=0, btn9=0, btnC=0,
  btn0=0, btnD=0;
  if (selec_funcao==0) {
// Print da tela de seleção da Agenda
    lcd.setCursor(0, 0);
    lcd.print("Gravar Agenda......C");
    lcd.setCursor(0, 1);
    lcd.print("Voltar.............B");
    Serial.println("Gravar Agenda......C");
    Serial.println("Voltar.............B");
  }
  
// Condição que joga para linha de baixo após prencher a linha 
// de cima (No Display 20X4, 20 colunas ou caracteres)
  if (contador_coluna>19) {
    linha_atual++;
    contador_coluna=0;
  }
  
// Condição que reínicia as linhas após passar o número máximo
// (No Display 20X4, 4 linhas)
  if (linha_atual>3) {
    linha_atual=0;
  }
  
// Repetição que dá 2 voltas para pegar os 2 digitos que formam o código das letras
  while (contador<2) {
    for (int linhax=4; linhax<8; linhax++) {
// Define por padrão, todas as linhas como 0V
      digitalWrite(linha1, LOW);
      digitalWrite(linha2, LOW);
      digitalWrite(linha3, LOW);
      digitalWrite(linha4, LOW);
// Coloca a linhax como 5V, visto que a cada repetição, o valor dela irá variar
      digitalWrite(linhax, HIGH);
// Leituras dos Botões, formando um curto circuito fechando as colunas
      if (digitalRead(coluna1)==HIGH) {
// Apenas executa a gravação de caracteres caso o usuário já tenha apertado C
        if (selec_funcao==1) {
          displaywrite(linhax, coluna1, contador);
          delay(100);
          contador++;
        }
      }
      if (digitalRead(coluna2)==HIGH) {
        if (selec_funcao==1) {
          displaywrite(linhax, coluna2, contador);
          delay(100);
          contador++;
        }
      }     
      if (digitalRead(coluna3)==HIGH) {
        if (selec_funcao==1) {
          displaywrite(linhax, coluna3, contador);
          delay(100);
          contador++;
        }
      } 
      if (digitalRead(coluna4)==HIGH) {
// Muda para o modo de gravação caso o usuário aperte o botão C
        if (selec_funcao==0) {
          if (linhax==6) {
            selec_funcao=1;
            contador=2;
            delay(200);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Gravando.");
            delay(200);
  			lcd.print(".");
  			delay(200);
  			lcd.print(".");
  			delay(500);
  			lcd.clear();
            Serial.println("Gravando...");
          }
        }
        if (selec_funcao==1) {
          displaywrite(linhax, coluna4, contador);
          delay(100);
          contador++;
        }
      }
    }
  }
}

void displaywrite(int linha, int coluna, int repeticao)
{ 
  lcd.setCursor(contador_coluna, linha_atual);
// Gravações da primeira leitura do Teclado Matricial
// Primeiro 1
  if ((linha==4 && coluna==8) && repeticao==0) {
    primbtn1=1;
  }
// Primeiro 2
  if ((linha==4 && coluna==9) && repeticao==0) {
    primbtn2=2;
  }
// Primeiro 0
  if ((linha==7 && coluna==9) && repeticao==0) {
    primbtn0=14;
  }
// 1
  if ((linha==4 && coluna==8) && repeticao==1) {
    btn1=1;
  }
// 2
  if ((linha==4 && coluna==9) && repeticao==1) {
    btn2=2;
  }
// 3
  if ((linha==4 && coluna==10) && repeticao==1) {
    btn3=3;
  }
// A
  if (linha==4 && coluna==11) {
    btnA=4;
  }
// 4
  if (linha==5 && coluna==8) {
    btn4=5;
  }
// 5
  if (linha==5 && coluna==9) {
    btn5=6;
  }
// 6
  if (linha==5 && coluna==10) {
    btn6=7;
  }
// B
  if (linha==5 && coluna==11) {
    selec_funcao=0;
    delay(200);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Encerrando.");
    delay(200);
  	lcd.print(".");
  	delay(200);
  	lcd.print(".");
  	delay(100);
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gravar Agenda......C");
    lcd.setCursor(0, 1);
    lcd.print("Voltar.............B");
    Serial.println("");
    Serial.println("Gravar Agenda......C");
    Serial.println("Voltar.............B");
  }
// 7
  if (linha==6 && coluna==8) {
    btn7=9;
  }
// 8
  if (linha==6 && coluna==9) {
    btn8=10;
  }
// 9
  if (linha==6 && coluna==10) {
    btn9=11;
  }
// C
  if (linha==6 && coluna==11) {
    btnC=12;
  }
// *
  if (linha==7 && coluna==8) {
    contador_coluna=0;
    linha_atual++;
    Serial.println("");
  }
// 0
  if ((linha==7 && coluna==9) && repeticao==1) {
    btn0=14;
  }
// #
  if (linha==7 && coluna==10) {
    lcd.clear();
    Serial.println("");
    contador_coluna=0;
    linha_atual=0;
  }
// D
  if (linha==7 && coluna==11) {
// Condição que impede que a coluna chegue a um valor negativo 
    if (contador_coluna>0) {
      contador_coluna--;
    }
    lcd.setCursor(contador_coluna, linha_atual);
    lcd.print(" ");
  }
    
// Impressão dos caracteres
// 0 & 1 = A
  if (primbtn0==14 && btn1==1) {
    lcd.print("A");
    Serial.print("A");
    contador_coluna++;
  }
// 0 & 2 = B
  if (primbtn0==14 && btn2==2) {
    lcd.print("B");
    Serial.print("B");
    contador_coluna++;
  }
// 0 & 3 = C
  if (primbtn0==14 && btn3==3) {
    lcd.print("C");
    Serial.print("C");
    contador_coluna++;
  }
// 0 & 4 = D
  if (primbtn0==14 && btn4==5) {
    lcd.print("D");
    Serial.print("D");
    contador_coluna++;
  }
// 0 & 5 = E
  if (primbtn0==14 && btn5==6) {
    lcd.print("E");
    Serial.print("E");
    contador_coluna++;
  }
// 0 & 6 = F
  if (primbtn0==14 && btn6==7) {
    lcd.print("F");
    Serial.print("F");
    contador_coluna++;
  }
// 0 & 7 = G
  if (primbtn0==14 && btn7==9) {
    lcd.print("G");
    Serial.print("G");
    contador_coluna++;
  }
// 0 & 8 = H
  if (primbtn0==14 && btn8==10) {
    lcd.print("H");
    Serial.print("H");
    contador_coluna++;
  }
// 0 & 9 = I
  if (primbtn0==14 && btn9==11) {
    lcd.print("I");
    Serial.print("I");
    contador_coluna++;
  }
// 1 & 0 = J
  if (primbtn1==1 && btn0==14) {
    lcd.print("J");
    Serial.print("J");
    contador_coluna++;
  }
// 1 & 1 = K
  if (primbtn1==1 && btn1==1) {
    lcd.print("K");
    Serial.print("K");
    contador_coluna++;
  }
// 1 & 2 = L
  if (primbtn1==1 && btn2==2) {
    lcd.print("L");
    Serial.print("L");
    contador_coluna++;
  }
// 1 & 3 = M
  if (primbtn1==1 && btn3==3) {
    lcd.print("M");
    Serial.print("M");
    contador_coluna++;
  }
// 1 & 4 = N
  if (primbtn1==1 && btn4==5) {
    lcd.print("N");
    Serial.print("N");
    contador_coluna++;
  }
// 1 & 5 = O
  if (primbtn1==1 && btn5==6) {
    lcd.print("O");
    Serial.print("O");
    contador_coluna++;
  }
// 1 & 6 = P
  if (primbtn1==1 && btn6==7) {
    lcd.print("P");
    Serial.print("P");
    contador_coluna++;
  }
// 1 & 7 = Q
  if (primbtn1==1 && btn7==9) {
    lcd.print("Q");
    Serial.print("Q");
    contador_coluna++;
  }
// 1 & 8 = R
  if (primbtn1==1 && btn8==10) {
    lcd.print("R");
    Serial.print("R");
    contador_coluna++;
  }
// 1 & 9 = S
  if (primbtn1==1 && btn9==11) {
    lcd.print("S");
    Serial.print("S");
    contador_coluna++;
  }
// 2 & 0 = T
  if (primbtn2==2 && btn0==14) {
    lcd.print("T");
    Serial.print("T");
    contador_coluna++;
  }
// 2 & 1 = U
  if (primbtn2==2 && btn1==1) {
    lcd.print("U");
    Serial.print("U");
    contador_coluna++;
  }
// 2 & 2 = V
  if (primbtn2==2 && btn2==2) {
    lcd.print("V");
    Serial.print("V");
    contador_coluna++;
  }
// 2 & 3 = W
  if (primbtn2==2 && btn3==3) {
    lcd.print("W");
    Serial.print("W");
    contador_coluna++;
  }
// 2 & 4 = X
  if (primbtn2==2 && btn4==5) {
    lcd.print("X");
    Serial.print("X");
    contador_coluna++;
  }   
// 2 & 5 = Y
  if (primbtn2==2 && btn5==6) {
    lcd.print("Y");
    Serial.print("Y");
    contador_coluna++;
  }
// 2 & 6 = Z
  if (primbtn2==2 && btn6==7) {
    lcd.print("Z");
    Serial.print("Z");
    contador_coluna++;
  }
// 2 & 7 = Space
  if (primbtn2==2 && btn7==9) {
    lcd.print(" ");
    Serial.print(" ");
    contador_coluna++;
  }
// 2 & 8 = *
  if (primbtn2==2 && btn8==8) {
    lcd.print("*");
    Serial.print("*");
    contador_coluna++;
  }
}