
#include <Ultrasonic.h>


//#define chuva A0
#define solo A5
#define valvula 10

#define pino_trigger 3
#define pino_echo 2



int led_agua[3]={4,5,6};
int led_umidade[3] = {7,8,9};
int valor_analogico; //
float umidade;  //Variavel referente a umidade do solo
int cd = 50; //Tempo de intervalo sobre cada leitura
int intervalo = 3000; //Tempo de intervalo sobre a leitura durante a irrigação 
double nivelMin=5.0, nivelMax=19.0;
int umidadeIdeal=40;

Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {
  // Declarando todas as informações dos pinos, se são entrada ou saídas:
  pinMode(solo,INPUT);  // Declara o sensor de umidade do solo como entrada de dados
  pinMode (valvula, OUTPUT); // Declara a válvula como saída
  Serial.begin(9600); // Ativa a comunicação serial para leitura dos dados
  int i;
  for (i=0; i<3; i++){
    pinMode(led_agua[i], OUTPUT);    // Declaração dos leds para niveis, como saída.
    pinMode(led_umidade[i],OUTPUT);
  }
}



double nivelAgua(){
  double cm;
  long dadoSonico = ultrasonic.timing();
  cm = ultrasonic.convert(dadoSonico, Ultrasonic::CM);
//  delay(cd);
  return cm;
 }



void infoAgua(){
  int delay_t = 500;
  double leitura_agua = nivelAgua(); 
  Serial.println("ALERTA!!!");
    if(leitura_agua <= nivelMin){
      digitalWrite(led_agua[0],LOW);
      Serial.println("Nivel de Agua alto demais, esvazie um pouco:");
      Serial.print(nivelMin);
      digitalWrite(led_agua[1],HIGH);
      delay(delay_t);
      digitalWrite(led_agua[1],LOW);
      delay(delay_t); 
      
      }else if(leitura_agua >= nivelMax){
        
        digitalWrite(led_agua[0],LOW);
        Serial.println ("Nivel de Agua vazio:");
        Serial.print(nivelMax);
        digitalWrite(led_agua[2],HIGH);
        delay(delay_t/4);
        digitalWrite(led_agua[2],LOW);
        delay(delay_t/4); 
      }else{
      //if(nivelAgua() > nivelMin && nivelAgua() < nivelMax){
        Serial.println ("Nivel de água normal");
        digitalWrite(led_agua[0],HIGH); 
        
      }
 }

float leitura(){
//Le o valor do pino A5 do sensor
 
  //Mostra o valor da porta analogica no serial monitor

  // Info: Nº máximo =  1023 = 100% SECO e 0% uMIDO
  //       Nº mínimo =  0 =  0% SECO e 100% MOLHADO 

// Teste de configuração do sensor de umidade
// A5 plugando no A0 do chip
// GND do arduino plugado no GND do chip
// 5v do arduino plugado no vcc do chip
  
   valor_analogico = analogRead(solo); // Recebe o valor detectado do leitor de umidade do solo  
   umidade = map(valor_analogico, 0, 1023, 0,100); // Converte o valor para ser calculado em escala de 0 a 100
   umidade = 100 - umidade; // COrreçao de valores
   return umidade;
}

void exibeDados(){
  Serial.println("Umidade: ");
  Serial.print(umidade);
  Serial.print("% - ");
  Serial.println(valor_analogico);
  Serial.print("\n");
//  nivelAgua();
  
  
}

void irriga(){
//  int umidadeAnt;
  double leitura_agua; 
  // umidadeAnt = leitura();
   umidade = leitura();
   
      while (umidade < umidadeIdeal){   
          umidade = leitura();   
          exibeDados();
          leitura_agua = nivelAgua();
          if(leitura_agua <= nivelMax){   // Se a água estiver em um nivel estável, procede com a execução (se estiver menor que 5 cm, 
                                                     //  tem água demais. se estiver maior que 19 cm, está vazio)
            digitalWrite (led_umidade[0],LOW);
            Serial.println ("A valvula foi ligada");
            digitalWrite(valvula,HIGH); 
            Serial.println ("cm do recipiente: ");
            Serial.print (leitura_agua);  
            Serial.print("\n");
            digitalWrite(led_umidade[1],HIGH); 
            delay(intervalo);                          
          }else{
            Serial.println ("A valvula foi desligada: ");
            Serial.print (leitura_agua);
            Serial.print(" - ");
            Serial.print (nivelMax);
            digitalWrite(valvula,LOW);
            Serial.println ("cm do recipiente: ");
            Serial.print (leitura_agua);  
            Serial.print("\n");
            digitalWrite(led_umidade[1],LOW);
            digitalWrite(led_umidade[0],HIGH);  
            infoAgua();
          }
     } 
   digitalWrite(valvula,LOW);
}




void loop() {

  
  umidade = leitura();
  exibeDados();


   if(umidade < umidadeIdeal){
    //Serial.println("Abaixo de 20%");
    digitalWrite(led_umidade[0],HIGH);
    digitalWrite(led_umidade[1],LOW);
    digitalWrite(led_umidade[2],LOW);
    infoAgua();
    irriga();
   }
                   

    if(umidade>=umidadeIdeal){
     digitalWrite(led_umidade[0],LOW);
     digitalWrite(led_umidade[1],LOW);
     digitalWrite(led_umidade[2],HIGH);
     infoAgua();
  }
                                          
  delay(cd);
}
