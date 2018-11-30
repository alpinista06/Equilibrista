int ENA = 9; // motor esquerdo

int ENB = 10; //motor direito

//declaracao dos motores//

int motor1_a = 4;

int motor1_b = 6;

int motor2_a = 5;

int motor2_b = 7;


void frente(int velocidade) {

  analogWrite(ENA, velocidade );

  analogWrite(ENB, velocidade );



  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , LOW);

  digitalWrite(motor2_a , LOW);

  digitalWrite(motor2_b , HIGH);
#ifdef PRINT_VELOCIDADE
  Serial.print(" frente ");
  Serial.print("\t");
  Serial.println(velocidade);
#endif
}

void re(int velocidade) {

  analogWrite(ENA, velocidade );

  analogWrite(ENB, velocidade );



  digitalWrite(motor1_a , LOW);

  digitalWrite(motor1_b , HIGH);

  digitalWrite(motor2_a , HIGH);

  digitalWrite(motor2_b , LOW);
#ifdef PRINT_VELOCIDADE
  Serial.print(" re ");
  Serial.print("\t");
  Serial.println(velocidade);
  #endif
}

void motores_parados() {
  
  digitalWrite(motor1_a , HIGH);

  digitalWrite(motor1_b , HIGH);

  digitalWrite(motor2_a , HIGH);

  digitalWrite(motor2_b , HIGH);
}
