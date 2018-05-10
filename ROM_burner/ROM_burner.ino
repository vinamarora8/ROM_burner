#define SR_CLK A0
#define SR_CLR A1
#define SR_DATA A2
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

void setup() {
  /* Shift Register Initialization */
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_CLR, OUTPUT);

  // initialize CLK and DATA
  digitalWrite(SR_CLK, LOW);
  digitalWrite(SR_DATA, LOW);

  // send some random address
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, 255);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, 255);

  // clear the shift register
  digitalWrite(SR_CLR, LOW);
  delay(10);
  digitalWrite(SR_CLR, HIGH);
  
  /* ROM interaction pins */
  pinMode(ROM_WE, OUTPUT);
  pinMode(ROM_OE, OUTPUT);
  pinMode(ROM_CE, OUTPUT);
  pinMode(ROM_D0, OUTPUT);
  pinMode(ROM_D1, OUTPUT);
  pinMode(ROM_D2, OUTPUT);
  pinMode(ROM_D3, OUTPUT);
  pinMode(ROM_D4, OUTPUT);
  pinMode(ROM_D5, OUTPUT);
  pinMode(ROM_D6, OUTPUT);
  pinMode(ROM_D7, OUTPUT);
  
  // Initialize pins
  digitalWrite(ROM_WE, HIGH);
  digitalWrite(ROM_OE, HIGH);
  digitalWrite(ROM_CE, HIGH);

  /* Done output */
  pinMode(DONE, OUTPUT);
  digitalWrite(DONE, LOW);

  Serial.begin(9600);

  /*
  for (int i=(0x7ff3-0x39); i<0x7ff0; i++){
    writeData(i, 0x00);
  }
  writeData(0x7ff0, 0xEA);
  writeData(0x7ff1, 0x00);
  writeData(0x7ff2, 0x00);
  writeData(0x7ff3, 0x00);
  writeData(0x7ff4, 0xF8);
  writeData(0x7ff5, 0x00);
  writeData(0x7ff6, 0x00);
  writeData(0x7ff7, 0x00);
  writeData(0x7ff8, 0x00);
  writeData(0x7ff9, 0x00);  
  writeData(0x7ffA, 0x00);

  exit(0);
  /*
  
  writeData(0x0000, 0xB0);
  writeData(0x0001, 0x0A);
  writeData(0x0002, 0xE6);
  writeData(0x0003, 0xFF);
  writeData(0x0004, 0xB0);
  writeData(0x0005, 0x0B);
  writeData(0x0006, 0xE6);
  writeData(0x0007, 0xFE);
  writeData(0x0008, 0xF4);
  for (unsigned int i=0x9; i<0x40; i++){
    writeData(i, 0x00);
  }
  Serial.println("done");`
  Serial.flush();
  exit(0);
  */

}

unsigned int addr = 0;

void loop() {
  Serial.println(addr);
  Serial.flush();
  
  while (!Serial.available());
  
  digitalWrite(DONE, LOW);
  String data;
  data = Serial.readString();

  unsigned int lim = addr + CHUNK;
  for (unsigned int i=addr; i<lim; i++) {
    writeData(i, (byte) data[i%CHUNK]);
  }

  addr += CHUNK;
  
  digitalWrite(DONE, HIGH);   
}


void writeData(unsigned int addr, byte data) {
  // Set the data pins 
  setDataPins(data);
  
  // Get 2 bytes from addr (MSB and LSB)
  byte addr_1 = (addr & 0xFF);
  
  byte addr_2 = ((addr >> 8) & 0xFF);


  // Shift the 2 bytes out to the SRs
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, addr_2);
  shiftOut(SR_DATA, SR_CLK, MSBFIRST, addr_1);

  // Write data
  delayMicroseconds(1);
  digitalWrite(ROM_CE, LOW);
  digitalWrite(ROM_WE, LOW);
  delayMicroseconds(1);
  digitalWrite(ROM_WE, HIGH);
  digitalWrite(ROM_CE, HIGH);

  // Clear shift registers
  digitalWrite(SR_CLR, LOW);
  digitalWrite(SR_CLR, HIGH);
}

void setDataPins(byte data) {
  digitalWrite(ROM_D0, data & 1);
  digitalWrite(ROM_D1, (data >> 1) & 1);
  digitalWrite(ROM_D2, (data >> 2) & 1);
  digitalWrite(ROM_D3, (data >> 3) & 1);
  digitalWrite(ROM_D4, (data >> 4) & 1);
  digitalWrite(ROM_D5, (data >> 5) & 1);
  digitalWrite(ROM_D6, (data >> 6) & 1);
  digitalWrite(ROM_D7, (data >> 7) & 1);
}
