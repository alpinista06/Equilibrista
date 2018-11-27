#include<Wire.h>
#include <Kalman.h>


Kalman kalmanX; // Criando instancias para a classe Kalman
Kalman kalmanY;


//Endereco I2C do MPU6050
const int MPU=0x68;
//Variaveis para armazenar valores dos sensores
double AcX,AcY,AcZ,GyX,GyY,GyZ;

double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;

void setup()
{
  Serial.begin(9600);

  //Inicializa o MPU-6050
  Wire.write(0);
  Wire.endTransmission(true);

  //Calculo dos angulos apenas com o acelerometro e trigonometria basica
  double pitch = atan2(-AcX, AcZ) *57.29478;
  double roll = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) *57.29478;

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);

  timer = micros();

}

void loop()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);

  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) Caso queira dados da temperatura
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Calculando os angulo pitch e roll apenas pelo acelerometro (deg)
  //pitch e o movimento de rotacao em torno do eixo Y, sendo Z para cima e X para a direita
  // e o robo de frente para o eixo X, de lado para o observador
  double pitch = atan2(-AcX, AcZ) *57.29478;
  double roll = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) *57.29478;

  double gyroXrate = GyX / 131.0; // Convert to deg/s
  double gyroYrate = GyY / 131.0; // Convert to deg/s


  //Calculo da estimativa do angulo usando o acelerometro e o giroscopio fundidos
  //atraves do filtro de Kalman
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);

  Serial.print(pitch);
  Serial.print("\t");
  Serial.println(kalAngleY);

}
