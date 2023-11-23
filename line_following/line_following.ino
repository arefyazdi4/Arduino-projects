int input = A0;
int val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(input);
  Serial.println(val);
  // if (val < 45){
  //   // it's black thresh hold
  // }
  delay(40);
}
