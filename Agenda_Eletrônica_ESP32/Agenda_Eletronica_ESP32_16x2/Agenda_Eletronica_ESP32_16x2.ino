//----------------------------------------------------------------------------------------------
// Projeto: Agenda Eletrônica com ESP32 e DisplayLCD16x2
// Autor: João Vitor Barros Bertin
//----------------------------------------------------------------------------------------------

// Importação das bibliotecas
// Biblioteca do teclado matricial (No caso, o 4x4)
#include <Keypad.h>
// Biblioteca utilizada para gerenciar a comunicação entre dispositivos através do protocolo I2C
#include <Wire.h>
// Biblioteca do display 16x2 I2C
#include <LiquidCrystal_I2C.h>

// Define o número de linhas
const uint8_t ROWS = 4;
// Define o número de colunas
const uint8_t COLS = 4;
// Define a distribuição das teclas
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
// Pinos correspondentes as linhas
uint8_t colPins[COLS] = {16, 4, 2, 15};
// Pinos correspondentes as colunas
uint8_t rowPins[ROWS] = {19, 18, 5, 17};
// Cria um objeto com os parametros criados previamente
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define os parâmetros do display 16x2 I2C
LiquidCrystal_I2C lcd(0x27,16,2);

// Definindo variáveis dos diferentes conjuntos de botões
/* É necessário usar variáveis de primeiro botão para não dar conflito nos códigos das letras, definidos mais abaixo. 
EX: "A" seria 0 e 1, enquanto "J" seria 1 e 0 e os dois seriam impressos, causando conflito */
int primbtn1=0, primbtn2=0, primbtn0=0,
btn1=0, btn2=0, btn3=0, btnA=0,
btn4=0, btn5=0, btn6=0, btnB=0,
btn7=0, btn8=0, btn9=0, btnC=0,
btn0=0, btnD=0;

// Variável seletora de funções
int selec_funcao=0;
// Sistema para descer linha no display
int linha_atual=0, contador_coluna=0;

void setup() {
// Iniciando display LCD - 16 colunas, 2 linhas
  lcd.init();
  lcd.backlight();
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
}

void loop() {
// Definição da variável do contador
  int contador=0;
// Redefinindo as variáveis dos códigos das letras como 0 a cada loop
  primbtn1=0, primbtn2=0, primbtn0=0,
  btn1=0, btn2=0, btn3=0, btnA=0,
  btn4=0, btn5=0, btn6=0, btnB=0,
  btn7=0, btn8=0, btn9=0, btnC=0,
  btn0=0, btnD=0;
  if (selec_funcao == 0) {
// Print da tela de seleção da Agenda
    lcd.setCursor(0, 0);
    lcd.print("Gravar Agenda..C");
    lcd.setCursor(0, 1);
    lcd.print("Voltar.........B");
    Serial.println("Gravar Agenda..C");
    Serial.println("Voltar.........B");
  }
  
// Condição que joga para linha de baixo após prencher a linha de cima (No Display 20X4, 20 colunas ou caracteres)
  if (contador_coluna > 15) {
    linha_atual++;
    Serial.println("");
    contador_coluna=0;
  }
  
// Condição que reínicia as linhas após passar o número máximo (No Display 20X4, 4 linhas)
  if (linha_atual > 1) {
    linha_atual=0;
  }

// Repetição que dá 2 voltas para pegar os 2 digitos que formam o código das letras
  while (contador < 2) {

// Variável que obtém a tecla pressionada e a armazena
    char key = keypad.getKey();

// Condição que confirma se o usuário está apertando a tecla
    if (key) {
// Apenas executa a gravação de caracteres caso o usuário já tenha apertado C
      if (key == 'C') {
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
// Após o botão C ser pressionado, entra nesta condição que executa a função de escrever no display
      if (selec_funcao == 1) {
        displaywrite(key, contador);
        delay(100);
        contador++;
      }
    }
  }
}

 void displaywrite(int tecla, int repeticao)
{ 
// Alteração do cursor do display de acordo com a quantidade digitada
  lcd.setCursor(contador_coluna, linha_atual);
// Gravações da primeira leitura do teclado Matricial
// Primeiro 1
  if ((tecla == '1') && repeticao == 0) {
    primbtn1=1;
  }
// Primeiro 2
  if ((tecla == '2') && repeticao == 0) {
    primbtn2=2;
  }
// Primeiro 0
  if ((tecla == '0') && repeticao == 0) {
    primbtn0=10;
  }
// 1
  if ((tecla == '1') && repeticao == 1) {
    btn1=1;
  }
// 2
  if ((tecla == '2') && repeticao == 1) {
    btn2=2;
  }
// 3
  if (tecla == '3') {
    btn3=3;
  }
// A
  if (tecla == 'A') {
    btnA=0;
  }
// 4
  if (tecla == '4') {
    btn4=4;
  }
// 5
  if (tecla == '5') {
    btn5=5;
  }
// 6
  if (tecla == '6') {
    btn6=6;
  }
// B
  if (tecla == 'B') {
    selec_funcao=0;
    contador_coluna=0;
    linha_atual=0;
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
  if (tecla == '7') {
    btn7=7;
  }
// 8
  if (tecla == '8') {
    btn8=8;
  }
// 9
  if (tecla == '9') {
    btn9=9;
  }
// C
  if (tecla == 'C') {
    btnC=0;
  }
// *
  if (tecla == '*') {
    contador_coluna=0;
    linha_atual++;
    Serial.println("");
  }
// 0
  if ((tecla == '0') && repeticao == 1) {
    btn0=10;
  }
// #
  if (tecla == '#') {
    lcd.clear();
    Serial.println("");
    contador_coluna=0;
    linha_atual=0;
  }
// D
  if (tecla == 'D') {
// Condição que impede que a coluna chegue a um valor negativo 
    if (contador_coluna>0) {
      contador_coluna--;
    }
    lcd.setCursor(contador_coluna, linha_atual);
    lcd.print(" ");
  }
    
// Impressão dos caracteres
// 0 & 1 = A
  if (primbtn0==10 && btn1==1) {
    lcd.print("A");
    Serial.print("A");
    contador_coluna++;
  }
// 0 & 2 = B
  if (primbtn0==10 && btn2==2) {
    lcd.print("B");
    Serial.print("B");
    contador_coluna++;
  }
// 0 & 3 = C
  if (primbtn0==10 && btn3==3) {
    lcd.print("C");
    Serial.print("C");
    contador_coluna++;
  }
// 0 & 4 = D
  if (primbtn0==10 && btn4==4) {
    lcd.print("D");
    Serial.print("D");
    contador_coluna++;
  }
// 0 & 5 = E
  if (primbtn0==10 && btn5==5) {
    lcd.print("E");
    Serial.print("E");
    contador_coluna++;
  }
// 0 & 6 = F
  if (primbtn0==10 && btn6==6) {
    lcd.print("F");
    Serial.print("F");
    contador_coluna++;
  }
// 0 & 7 = G
  if (primbtn0==10 && btn7==7) {
    lcd.print("G");
    Serial.print("G");
    contador_coluna++;
  }
// 0 & 8 = H
  if (primbtn0==10 && btn8==8) {
    lcd.print("H");
    Serial.print("H");
    contador_coluna++;
  }
// 0 & 9 = I
  if (primbtn0==10 && btn9==9) {
    lcd.print("I");
    Serial.print("I");
    contador_coluna++;
  }
// 1 & 0 = J
  if (primbtn1==1 && btn0==10) {
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
  if (primbtn1==1 && btn4==4) {
    lcd.print("N");
    Serial.print("N");
    contador_coluna++;
  }
// 1 & 5 = O
  if (primbtn1==1 && btn5==5) {
    lcd.print("O");
    Serial.print("O");
    contador_coluna++;
  }
// 1 & 6 = P
  if (primbtn1==1 && btn6==6) {
    lcd.print("P");
    Serial.print("P");
    contador_coluna++;
  }
// 1 & 7 = Q
  if (primbtn1==1 && btn7==7) {
    lcd.print("Q");
    Serial.print("Q");
    contador_coluna++;
  }
// 1 & 8 = R
  if (primbtn1==1 && btn8==8) {
    lcd.print("R");
    Serial.print("R");
    contador_coluna++;
  }
// 1 & 9 = S
  if (primbtn1==1 && btn9==9) {
    lcd.print("S");
    Serial.print("S");
    contador_coluna++;
  }
// 2 & 0 = T
  if (primbtn2==2 && btn0==10) {
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
  if (primbtn2==2 && btn4==4) {
    lcd.print("X");
    Serial.print("X");
    contador_coluna++;
  }   
// 2 & 5 = Y
  if (primbtn2==2 && btn5==5) {
    lcd.print("Y");
    Serial.print("Y");
    contador_coluna++;
  }
// 2 & 6 = Z
  if (primbtn2==2 && btn6==6) {
    lcd.print("Z");
    Serial.print("Z");
    contador_coluna++;
  }
// 2 & 7 = Space
  if (primbtn2==2 && btn7==7) {
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