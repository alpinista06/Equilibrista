#include<Wire.h>
#include <Kalman.h>

<<<<<<< HEAD

=======
>>>>>>> 8767005fda205a85ad9813439e0c1118341e12bb
#define PRINT_KALMAN_COMPARE
//#define PRINT_VELOCIDADE
//#define SINAL_CONTROLE
#define Y_ANGLE
//#define X_ANGLE

Kalman kalmanX; // Criando instancias para a classe Kalman
Kalman kalmanY;

//Endereco I2C do MPU6050
const int MPU = 0x68;
//Variaveis para armazenar valores dos sensores
double AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

double controle;
double sinal_de_controle;

uint32_t timer;

uint8_t i2cData[14]; // Buffer for I2C data

void setup()
{


  //fazer a inicializacao correta do MPU6050
  //Inicializa o MPU-6050
  // Wire.write(0);
  // Wire.endTransmission(true);
  Serial.begin(9600);


  Wire.begin();
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
#else
  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while (1);
  }

  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));
  AcX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  AcY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  AcZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);


  //Calculo dos angulos apenas com o acelerometro e trigonometria basica
  double pitch = atan2(-AcX, AcZ) * 57.29478;
  double roll = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) * 57.29478;

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
  Wire.requestFrom(MPU, 14, true);

  while (i2cRead(0x3B, i2cData, 14));
  AcX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  AcY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  AcZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  GyX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
  GyY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
  GyZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);;

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Calculando os angulo pitch e roll apenas pelo acelerometro (deg)
  //pitch e o movimento de rotacao em torno do eixo Y, sendo Z para cima e X para a direita
  // e o robo de frente para o eixo X, de lado para o observador
  double pitch = atan2(-AcX, AcZ) * 57.29478;
  double roll = atan(AcY / sqrt(AcX * AcX + AcZ * AcZ)) * 57.29478;

  double gyroXrate = -(GyX / 131.0); // Convert to deg/s
  double gyroYrate = GyY / 131.0; // Convert to deg/s


  //Calculo da estimativa do angulo usando o acelerometro e o giroscopio fundidos
  //atraves do filtro de Kalman
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);

#ifdef PRINT_KALMAN_COMPARE
  Serial.print(pitch);
  Serial.print("\t");
  Serial.println(kalAngleY);
<<<<<<< HEAD

=======
 
>>>>>>> 8767005fda205a85ad9813439e0c1118341e12bb
#endif

  sinal_de_controle = compensador(kalAngleY);


  if (kalAngleY < 0) {
    frente(sinal_de_controle);
  } else if (kalAngleY > 0) {
    re(sinal_de_controle);
  } else {
    motores_parados();
  }

}
