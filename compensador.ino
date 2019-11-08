double Kp = 30;
//double Kd = 5;
double Ki = 5;

double resposta_p = 0;
//double resposta_d = 0;
double resposta_i = 0;
double Resposta_PID = 0;
double Resposta_PI = 0;
double erro = 0;
double amostra_atual = 0;
double amostra_anterior = 0;
double erro_atual = 0;
double erro_anterior = 0;
float set_point = -3;

double compensador(double controle)
{
Resposta_PID = 0;
Resposta_PI = 0;

erro = set_point - controle;
resposta_p = erro * Kp;
resposta_i = resposta_i + erro * Ki;
resposta_i = resposta_i < 0 ? 0 : resposta_i;
resposta_i = resposta_i > 255 ? 255 : resposta_i;

//resposta_d = Kd * (erro - erro_anterior);
//Resposta_PID = resposta_p + resposta_i + resposta_d;
Resposta_PI = resposta_p + resposta_i
erro_anterior = erro;
controle = abs(Resposta_PID);

#ifdef SINAL_CONTROLE
Serial.print("\t");
Serial.print(resposta_p);
Serial.print("\t");
Serial.print(resposta_i);
Serial.print("\t");
//Serial.print(resposta_d);
//Serial.print("\t");
Serial.println(Resposta_PI);
#endif

return controle;
}
