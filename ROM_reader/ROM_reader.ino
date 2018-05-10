#define SR_CLK A0
#define SR_DATA A2
#define SR_CLR A1
#define ROM_D0 2
#define ROM_D1 3
#define ROM_D2 4
#define ROM_D7 5
#define ROM_D6 6
#define ROM_D5 7
#define ROM_D4 8
#define ROM_D3 9
#define ROM_WE 10
#define ROM_OE 11
#define ROM_CE 12

#define DONE 13

#define CHUNK 64
#define MAX 0x400

byte readData(int);

void setup() {
  /* Shift Register Initialization */
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_CLR, OUTPUT);

  // clear the shift register
  digitalWrite(SR_CLR, LOW);
  delay(10);
  digitalWrite(SR_CLR, HIGH);

  // initialize CLK and DATA
  digitalWrite(SR_CLK, LOW);
  digitalWrite(SR_DATA, LOW);

  // send some random address
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, 255);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, 255);

  /* ROM interaction pins */
  pinMode(ROM_WE, OUTPUT);
  pinMode(ROM_OE, OUTPUT);
  pinMode(ROM_CE, OUTPUT);
  pinMode(ROM_D0, INPUT);
  pinMode(ROM_D1, INPUT);
  pinMode(ROM_D2, INPUT);
  pinMode(ROM_D3, INPUT);
  pinMode(ROM_D4, INPUT);
  pinMode(ROM_D5, INPUT);
  pinMode(ROM_D6, INPUT);
  pinMode(ROM_D7, INPUT);
  
  // Initialize pins
  digitalWrite(ROM_WE, HIGH);
  digitalWrite(ROM_OE, HIGH);
  digitalWrite(ROM_CE, HIGH);
  digitalWrite(ROM_D0, LOW);
  digitalWrite(ROM_D1, LOW);
  digitalWrite(ROM_D2, LOW);
  digitalWrite(ROM_D3, LOW);
  digitalWrite(ROM_D4, LOW);
  digitalWrite(ROM_D5, LOW);
  digitalWrite(ROM_D6, LOW);
  digitalWrite(ROM_D7, LOW);

  /* Done output */
  pinMode(DONE, OUTPUT);
  digitalWrite(DONE, LOW);

  Serial.begin(9600);

  Serial.println("0");

  /*
  Serial.println(readData((unsigned int) 0x7FF0));
  Serial.println(readData((unsigned int) 0x7FF1));
  Serial.flush();
  exit(0);
   */
}

unsigned int addr = 0x0000;

void loop() {
  while(!Serial.available());

  byte n[CHUNK];
  
  for (int i = 0; i<CHUNK; i++) {
    delayMicroseconds(100);
    n[i] = readData(addr + i);
  }
  
  addr += CHUNK;

  for (int i=0; i<CHUNK; i++)
    Serial.println(n[i]);

  Serial.flush();
  
  if (addr >= MAX) {
    digitalWrite(DONE, HIGH);
    exit(0);
  }
}


byte readData(unsigned int addr) {
  // clear the shift register
  // and get 2 bytes from addr (MSB and LSB)
  digitalWrite(SR_CLR, LOW);
  byte addr_1 = (addr & 0xFF);
  byte addr_2 = ((addr >> 8) & 0xFF);
  digitalWrite(SR_CLR, HIGH);

  // Shift the 2 bytes out to the SRs
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, addr_2);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, addr_1);

  // read data
  digitalWrite(ROM_CE, LOW);
  digitalWrite(ROM_OE, LOW);
  delayMicroseconds(1);
  byte ans;
  ans = digitalRead(ROM_D0);
  ans |= digitalRead(ROM_D1) << 1;
  ans |= digitalRead(ROM_D2) << 2;
  ans |= digitalRead(ROM_D3) << 3;
  ans |= digitalRead(ROM_D4) << 4;
  ans |= digitalRead(ROM_D5) << 5;
  ans |= digitalRead(ROM_D6) << 6; 
  ans |= digitalRead(ROM_D7) << 7;
  digitalWrite(ROM_OE, HIGH);
  digitalWrite(ROM_CE, HIGH);
  
  return ans;
}

