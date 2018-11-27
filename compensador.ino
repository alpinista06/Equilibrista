double Kp = 3;
double Kd = 5;
double Ki = 1;

double resposta_p = 0;
double resposta_d = 0;
double resposta_i = 0;
double Resposta_PID = 0;
double erro = 0;
double amostra_atual = 0;
double amostra_anterior = 0;
double erro_atual = 0;
double erro_anterior = 0;

double compensador(double controle)
{
erro = controle;
resposta_p = erro * Kp;
resposta_i = resposta_i + erro * Ki;
resposta_i = resposta_i < 0 ? 0 : resposta_i;
resposta_i = resposta_i > 255 ? 255 : resposta_i;

resposta_d = Kd * (erro - erro_anterior);
Resposta_PID = resposta_p + resposta_i + resposta_d;
erro_anterior = erro;
controle = abs(Resposta_PID);

return controle;
}
