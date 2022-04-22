//Motor A
int enA=8;
int in1=9;
int in2=7;
//Motor B
int enB=6;
int in3=5;
int in4=4;

void stilstand()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void vooruit()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 200);
  analogWrite(enB, 200);
}
void achteruit()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 200);
  analogWrite(enB, 200);
}
void eigenasdraaienlinks()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 175);
  analogWrite(enB, 175);
}
void eigenasdraaienrechts()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 175);
  analogWrite(enB, 175);
}
//void linksnegtiggraden()
//{
//  eigenasdraaienlinks(); 
//  delay(300);
//}
//void rechtsnegtiggraden()
//{
//  eigenasdraaienrechts(); 
//  delay(220);
//}
//void honderdtachtiggraden()
//{
//  eigenasdraaienlinks(); 
//  delay(400);
//}
