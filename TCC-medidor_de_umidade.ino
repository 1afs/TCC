// Teste de configuração do sensor de umidade
// A0 plugano no A0 do chip
// GND do arduino plugado no GND do chip
// 5v do arduino plugado no vcc do chip


#define chuva A0
#define solo A1
#define valvula 8
#define buzzer 7

int led[5]= {2,3,4,5,6};
int valor_analogico;
float umidade;
int cd = 50;
int intervalo = 3000; 


void setup() {
  // Declarando todas as informações dos pinos, se são entrada ou saídas:
  pinMode(chuva, INPUT); // Declara o Sensor de chuva como entrada de dados
  pinMode(solo,INPUT);  // Declara o sensor de umidade do solo como entrada de dados
  pinMode (valvula, OUTPUT); // Declara a válvula como saída
  pinMode (buzzer, OUTPUT);
  
  Serial.begin(9600); // Ativa a comunicação serial para leitura dos dados
  int i;
  for (i=0; i<5; i++){
    pinMode(led[i], OUTPUT);    // Declaração dos leds para niveis, como saída.
  }
}


void alerta(){
  int i,x;
  float seno;
  
  for(x=0;x<180;x++){
  //converte graus para radiando e depois obtém o valor do seno
  seno=(sin(x*3.1416/180));
  //gera uma frequência a partir do valor do seno
  //frequencia = 2000+(int(seno*1000));
  //tone(9,frequencia);
  if(i<4000){
  tone(buzzer,i++);
  }
  if (i>=4000){
      while(i!=2000){
       tone(buzzer,i--); 
       //delay(2);
      }
     }
  //delay(2);
    }
}

float leitura(){
   valor_analogico = analogRead(solo);
   umidade = map(valor_analogico, 0, 1023, 0,100);
   umidade = 100 - umidade;
   return umidade;
}

void exibeDados(){
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("% - ");
  Serial.println(valor_analogico);
  Serial.print("\n");
  
}

void irriga(){
  int contador=0, umidadeAnt;
   
   umidadeAnt = leitura();
   digitalWrite(valvula,LOW);
   delay(intervalo);
   //umidade = leitura();
   digitalWrite(valvula,HIGH);
   while (umidade<50){
    umidade = leitura();
    exibeDados();
    contador++;
    Serial.println(contador);
    if(contador>360 && umidade < umidadeAnt + 5){
      alerta();
    }
   }
   digitalWrite(valvula,LOW);
   digitalWrite(buzzer, LOW);
   noTone(buzzer);
}

void loop() {
   //Le o valor do pino A0 do sensor
 
  //Mostra o valor da porta analogica no serial monitor

  // Info: Nº máximo =  1023 = 100% SECO e 0% uMIDO
  //       Nº mínimo =  0 =  0% SECO e 100% MOLHADO
  
  umidade = leitura();
  exibeDados();
  
 // Serial.print(umidade);

  if(umidade<20){
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
    digitalWrite(led[4], LOW);
    irriga();
  }
   if(umidade>=20 && umidade <40){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
    digitalWrite(led[4], LOW);
    irriga();
  }
    if(umidade>=40 && umidade<60){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], HIGH);
    digitalWrite(led[3], LOW);
    digitalWrite(led[4], LOW);
  }
    if(umidade>=60 && umidade<80){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], HIGH);
    digitalWrite(led[4], LOW);
  }
  if(umidade>=80){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
    digitalWrite(led[3], LOW);
    digitalWrite(led[4], HIGH);
  }
  delay(cd);
}
