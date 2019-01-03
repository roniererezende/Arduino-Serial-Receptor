//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>

// Variáveis de definição das GPIOs
int outZero = 2;
int outOne  = 3;
int outTwo  = 11;
int inZero  = 12;

// Varíavel utilizada na transmissão serial
int countSerial = 0;

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // Código de configuração do Arduino 
  
  pinMode(outZero,OUTPUT);
  pinMode(outOne,OUTPUT);
  pinMode(outTwo,OUTPUT);

  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);

  // Define o baudrate da comunicação serial no Arduino
  Serial.begin(9600);
  
}

// Função que realiza a leitura dos dados recebidos pela porta serial
String readStringSerial(){
  String content = "";
  char caracter;
  
  // Enquanto receber algo pela serial
  while(Serial.available()) {
    // Lê byte da serial
    caracter = Serial.read();
    // Ignora caractere de quebra de linha
    if (caracter != '\n'){
      // Concatena valores
      content.concat(caracter);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }    
  return content;
}

void loop(){
  // Se houver dados disponíveis na porta serial executa os comandos 
  if (Serial.available() > 0){

     String dpy;
     String ser;
     String output;
     String receivedBuffer = readStringSerial();
     String ptl = receivedBuffer.substring(0,3);
     int key;
     int lengthLcd; 

     // Interpreta o cabeçalho do pacote de dados e define se eles são da "Entrada Digital", "Saída Digital", "LCD" ou "Serial" 
     if(ptl == "INP"){
        key = 0;
     }
     else{
       if(ptl == "OUT"){
          key = 1;
       }
       else{
         if(ptl == "LCD"){
            key = 2;
         }
         else{
           if(ptl == "SER"){
              key = 3;
           }
           else{
              key = -1;
           }
         }
       }
     }

     // De acordo com cabeçalho recebido se manipula a carga útil do pacote de dados
     switch(key){
        case 0:
           if(digitalRead(inZero) == 1){
               Serial.print("INPUT HIGH");
           }  
           else{
               Serial.print("INPUT LOW");
           }
           break;
        case 1:
           if(receivedBuffer.substring(3,5) == "00"){
              digitalWrite(outZero,LOW);
              Serial.print("OUTPUT 1 LOW ");
           }
           if(receivedBuffer.substring(3,5) == "01"){
              digitalWrite(outZero,HIGH);
              Serial.print("OUTPUT 1 HIGH");
           }
           if(receivedBuffer.substring(3,5) == "10"){
              digitalWrite(outOne,LOW);
              Serial.print("OUTPUT 2 LOW ");
           }
           if(receivedBuffer.substring(3,5) == "11"){
              digitalWrite(outOne,HIGH);
              Serial.print("OUTPUT 2 HIGH");
           }
           if(receivedBuffer.substring(3,5) == "20"){
              digitalWrite(outTwo,LOW);
              Serial.print("OUTPUT 3 LOW ");
           }
           if(receivedBuffer.substring(3,5) == "21"){
              digitalWrite(outTwo,HIGH);
              Serial.print("OUTPUT 3 HIGH");
           }
           break;
          
        case 2:
          lengthLcd = receivedBuffer.substring(3,6).toInt();
          dpy = receivedBuffer.substring(6,lengthLcd + 6);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(dpy);
          delay(500);

          Serial.print(lengthLcd);
          
          if(lengthLcd > 16){
              for (int positionCounter = 0; positionCounter < lengthLcd; positionCounter++) {
                // scroll one position right:
                lcd.scrollDisplayLeft();
                // wait a bit:
                delay(500);
              }
          }
          
          lcd.clear();
          lcd.print(dpy);
          
          break;
          
        case 3:
          ser = receivedBuffer.substring(3);
          Serial.print(ser);
          break;
          
        default:
          break;
     }   
  }    
}
