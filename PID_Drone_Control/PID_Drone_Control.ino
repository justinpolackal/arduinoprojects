///////////////////////////
int Kp = 0;
int Ki = 0;
int Kd = 0;
//////////////////////////
int cGyro,prevGyro,cRec,prevRec;
int prevI;
void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

}

int pid_control()
{
  int p = (cGyro-cRec)*Kp;
  int i = prevI+((cGyro-cRec)*Ki);
  int d = (cGyro-cRec-prevGyro-prevRec)*Kd;

  int u = p+i+d;
  prevI = i;
  prevGyro = cGyro;
  prevRec = cRec;
  return u;
}

