/* Dominik Loncar
LED Cube - flash a 3x3x3 cube of LEDs using multiplexing
ATMEGA 2560
Version 3.2
*/

//a line is the vertical wire which is connected to the positive pins of 3 leds
byte ledLine[] = {13,12,11,10,9,8,7,6,5}; // from left to right, top to bottom
//0 1 2
//3 4 5
//6 7 8

//a layer is the horizontal wire which is connected to the ground pins of 9 leds
byte ledLayer[] = {52,44,32}; //from bottom to top
//2
//1
//0

int shortT = 200; //short time, used in most routines

//only for routine 5
byte line = 200; //used for random lines
byte layer = 200; //used for random layer
//both of these are used to prevent the same LED from lighting up twice in a row
byte linePrev; //used to store previous value of line
byte layerPrev; //used to store previous value of layer


//for PWM routine, p stands for power
byte p100 = 255; //max brightness
byte p75 = (255*0.75); //75% brightness
byte p50 = 255/2; //half brightness
byte p25 = (255*0.25); //25% brightness

byte knightLedLine[] = {0,0,0,0,0,ledLine[0],ledLine[1],ledLine[2],
ledLine[5],ledLine[4],ledLine[3],ledLine[6],ledLine[7],ledLine[8],0,0,0,0,0};
//this array is only used in the PWM knight rider routine (routine7)
//the order of the pins is changed and five 0s are added on the beginning and end
//the 0s act as coushins to allow to routine to light up less than 4 leds at a time
//the other array is used so that only one array has to be changed for everything to work 

int longT = 5000; //amount of time between routines

void setup(){ //sets all the pins as outputs, sizeof just gives you the size of the array
  for(byte i = 0; i < sizeof(ledLine); i++){
    pinMode(ledLine[i], OUTPUT);
  }//end for
  for(byte i = 0; i < sizeof(ledLayer); i++){
    pinMode(ledLayer[i], OUTPUT);
  }//end for
  randomSeed(analogRead(0)); //uses analog pin 0 as a random seed
}//end setup

void loop(){ //to run a routine, you simply uncomment it and the delay below it
  reset();
  //routine1(); //horizontal left right
  //delay(longT);
  //routine2(); //vertical up down
  //delay(longT);
  //routine3(); //starburst
  //delay(longT);
  //routine4(); //rotate wall
  //delay(longT);
  //routine5(); //random twinkle
  //delay(longT);
  //routine6(); //light up all, one by one
  //delay(longT);
  //routine7(); //PWM knightrider
  //delay(longT);
}//end loop

//this routine is used everywhere, it sets all the lines to LOW and all the layers to HIGH
//each routine is dependent on this routine, they need all the pins to be "off"
//reset is called after every other routine incase I missed something
void reset(){ //turns off all leds
  for(byte i = 0; i < sizeof(ledLine); i++){ //sets all positive pins to low
    digitalWrite(ledLine[i], LOW);
  }//end for
  for(byte i = 0; i < sizeof(ledLayer); i++){ //sets all ground pins to high
    digitalWrite(ledLayer[i], HIGH);
  }//end for
}//end reset

void routine1(){ //flash all leds on the bottom layer, then layers up and down 3 times
  for(byte k = 0; k < sizeof(ledLine); k++){ //turn on all led lines
     digitalWrite(ledLine[k], HIGH);
  }//end for
  
  //this just turns on and off each layer in the correc order 3 times
  for(byte i = 0; i < 3; i++){ //repeat 3 times
    digitalWrite(ledLayer[0], LOW); //turn on bottom layer
    delay(shortT);
    digitalWrite(ledLayer[0], HIGH); //turn off bottom layer
    digitalWrite(ledLayer[1], LOW); //turn on mid layer
    delay(shortT);
    digitalWrite(ledLayer[1], HIGH); //turn off mid layer
    digitalWrite(ledLayer[2], LOW); //turn on top layer
    delay(shortT);
    digitalWrite(ledLayer[2], HIGH); //turn off top layer
    digitalWrite(ledLayer[1], LOW); //turn on mid layer
    delay(shortT);
    digitalWrite(ledLayer[1], HIGH); //turn off mid layer
    if (i == 2){ //this is to prevent the bottom layer from lighting up double the time
      digitalWrite(ledLayer[0], LOW); //turn on bottom layer
      delay(shortT);
    }
  }//end for
  reset(); //set everything back to default
}//end routine1

void routine2(){ //flash vertical layer left to right 3 times
  for(byte i = 0; i < sizeof(ledLayer); i++){ //set all 3 layers to low
    digitalWrite(ledLayer[i], LOW);
  }//end for
  
  //the following segment is a bunch of nested for loops, some loops dependent on others
  //to figure out how to do this, I had to turn on and off each pin manually and then find a pattern
  for(byte i = 0; i < 3; i++){ //repeat 3 times
    for(byte k = 0; k < 7; k=k+3){ //go through vertical levels, first one starts at 0, second at 3 and last at 6
      for(byte j = k; j < k+3; j++){//turn on leds at each vertical level, first one is pins 0,1,2 second is pins 3,4,5 and last is pins 6,7,8
        digitalWrite(ledLine[j], HIGH);
      }//end for
      delay(shortT);
      for(byte j = k; j < k+3; j++){ //do the same thing but this time turn them off
        digitalWrite(ledLine[j], LOW); //turn off the leds
      }//end for
    }//end k for loop
    
    
    //this whole segemnt does the whole thing but the other way around
    for(int k = 6; k >= 0; k=k-3){ //go through vertical levels, the k is an int instead of byte because it tries to do 0-3 and thus runs into a problem because byte cant store negatives
      for(byte j = k; j < k+3; j++){ //go through leds
        digitalWrite(ledLine[j], HIGH); //turn them on
      }//end for
      delay(200);
      for(byte j = k; j < k+3; j++){
        digitalWrite(ledLine[j], LOW); //turn off the leds
      }//end for
    }//end k for loop
    
  }//end repeat loop
  reset();
}//end routine2

void routine3(){ //starburst
  //turn on center led
  digitalWrite(ledLine[4], HIGH);
  digitalWrite(ledLayer[1], LOW);
  delay(shortT);
  
  reset(); //turn off center led and set middle leyer back to HIGH
  
  //turn on the 6 adjecent leds, up, down, left, right, forward and backward
  for(int i = 0; i <= shortT/2; i++){ //shortT is divided by 2 because there are 2 delays in this loop which wait for 1ms each and the processing time is neglegible
    //turn on the top middle led and bottom middle led
    digitalWrite(ledLayer[0], LOW);
    digitalWrite(ledLayer[2], LOW);
    digitalWrite(ledLine[4], HIGH);
    delay(1);
    
    reset(); //turn off all leds
    
    //turn on 4 leds in the middle layer
    digitalWrite(ledLayer[1], LOW);
    for(byte k = 1; k < 8; k=k+2){
      digitalWrite(ledLine[k], HIGH);
    }//end for
    delay(1);
    
    reset(); //turn off all leds
  }//end for which repeats
  
   //now turn on the rest of the leds
   for(int i = 0; i <= shortT/2; i++){ //shortT is divided by 2 because there are 2 delays in this loop which wait for 1ms each and the processing time is neglegible
     //turn on all leds on top and bottom layer except for center led
     digitalWrite(ledLayer[2], LOW); //set top layer to ground
     digitalWrite(ledLayer[0], LOW); //set bottom layer to ground
     for(byte k = 0; k < 9; k++){ //go through all leds
       digitalWrite(ledLine[k], HIGH); //and turn them on
       digitalWrite(ledLine[4], LOW); //but make sure the middle ones are off
     }//end for
     delay(1);
     
     reset(); //turn off all leds
     
     //turn on only corner leds on middle layer
     digitalWrite(ledLayer[1], LOW); //set middle layer to ground
     for(byte k = 0; k < 9; k=k+2){ //k+2 conveniently skips the leds I used in the previous for loop
       digitalWrite(ledLine[k], HIGH);
       digitalWrite(ledLine[4], LOW); //but make sure the middle one is off
     }//end for
     delay(1);
     
     reset(); //turn off all leds
   }//end for which repeats
}//end routine3

void routine4(){ //vertical wall rotation times 3
  for(byte i = 0; i < sizeof(ledLayer); i++){ //sets all ground pins to low
    digitalWrite(ledLayer[i], LOW);
  }//end for
  digitalWrite(ledLine[4], HIGH); //turn on all 3 middle leds, they are always on
  
  //the following 5 lines exist so that the loop does not start at a diagonal
  //it makes no difference in the real world other than looking nicer
  digitalWrite(ledLine[3], HIGH);
  digitalWrite(ledLine[5], HIGH);
  delay(250);
  digitalWrite(ledLine[3], LOW);
  digitalWrite(ledLine[5], LOW);
  for(byte i = 0; i < 6; i++){//repeat 6 times, one rotation is 2 loops
    //the following code uses 2 variables to turn on the correct leds then turn them on and off
    for(byte k = 0, l = 8; k < 4, l > 4; k++, l--){
      digitalWrite(ledLine[k], HIGH);
      digitalWrite(ledLine[l], HIGH);
      delay(250);
      digitalWrite(ledLine[k], LOW);
      digitalWrite(ledLine[l], LOW);
    }//end for
  }//end for loop which loops thrice
  reset(); //turn off all leds
}//end routine4

void routine5(){ //random twinkle 50 times
  for (int i = 0; i < 51; i++){ //repeat 50 times
    linePrev = line; //make the linePrev equal to the previous line
    layerPrev = layer; //make the layerPrev equal to the previous layer
    while (line == linePrev && layer == layerPrev){ //if the same random values are generated as last time, which means the same LED will be lit up, regenerate until they are not
      line = random(0, 9); //pick a random number from 0 to 8
      layer = random(0, 3); //pick a random number form 0 to 2
    }//end while
    digitalWrite(ledLayer[layer], LOW); //turn on layer using random num
    digitalWrite(ledLine[line], HIGH); //turn on line using random num
    delay(shortT);
    reset(); //turn off all leds
  }//end for which repeats
}//end routine5

void routine6(){ //turn on all leds, one led at a time
  for(byte k = 0; k < 3; k++){ //go through the 3 layers bottom to top
    digitalWrite(ledLayer[k], LOW); //and set one of them to low
    for (byte i = 0; i < 9; i++){ //while one of the layers is at low, go through the leds
      digitalWrite(ledLine[i], HIGH); //turn on one led
      delay(shortT);
      digitalWrite(ledLine[i], LOW); //then turn it off
    }//end for
    digitalWrite(ledLayer[k], HIGH); //finally set the layer to high
  }//end for
  
  for(byte k = 2; k >= 0; k--){ //go through the 3 layers top to bottom
    digitalWrite(ledLayer[k], LOW); //and set one of them to low
    for (byte i = 8; i <= 0; i--){ //while one of the layers is at low, go through the leds
      digitalWrite(ledLine[i], HIGH); //turn on one led
      delay(shortT);
      digitalWrite(ledLine[i], LOW); //then turn it off
    }//end for
    digitalWrite(ledLayer[k], HIGH); //finally set the layer to high
  }//end for
  reset(); //turn off all leds
}//end routine6

void routine7(){ //PWM knight rider
  knightBottom(); knightMiddle(); knightTop(); knightMiddle(); knightBottom();
}//end knight

void knightBottom(){
    digitalWrite(ledLayer[0], LOW);
  for(byte i = 5; i < sizeof(knightLedLine) - 5; i++){ //this loop uses i to go through the leds
  //the extra 0s in the array are used are coushins, without them, it doesn't work properly
    analogWrite(knightLedLine[i], p100); //set led to max brightness
    analogWrite(knightLedLine[i-1], p75); //set the one behind it (if it exists) to three quarters brightness
    analogWrite(knightLedLine[i-2], p50); //set the one behind it (if it exists) to half brightness
    analogWrite(knightLedLine[i-3], p25); //set the one behind it (if it exists) to quarter brightness
    analogWrite(knightLedLine[i-4], 0); //set the one behind it (if it exists) off
    delay(400); 
  }//end i for
  digitalWrite(ledLayer[0], HIGH);
  reset();
}//end knightBottom

void knightMiddle(){
  digitalWrite(ledLayer[1], LOW);
  for(byte i = 13; i > sizeof(knightLedLine) - 15; i--){ //this loop uses i to go through the leds
  //the extra 0s in the array are used are coushins, without them, it doesn't work properly
    analogWrite(knightLedLine[i], p100); //set led to max brightness
    analogWrite(knightLedLine[i+1], p75); //set the one behind it (if it exists) to three quarters brightness
    analogWrite(knightLedLine[i+2], p50); //set the one behind it (if it exists) to half brightness
    analogWrite(knightLedLine[i+3], p25); //set the one behind it (if it exists) to quarter brightness
    analogWrite(knightLedLine[i+4], 0); //set the one behind it (if it exists) off
    delay(400); 
  }//end i for
  digitalWrite(ledLayer[1], HIGH);
  reset();
}//end knightMiddle

void knightTop(){
    digitalWrite(ledLayer[2], LOW);
  for(byte i = 5; i < sizeof(knightLedLine) - 5; i++){ //this loop uses i to go through the leds
  //the extra 0s in the array are used are coushins, without them, it doesn't work properly
    analogWrite(knightLedLine[i], p100); //set led to max brightness
    analogWrite(knightLedLine[i-1], p75); //set the one behind it (if it exists) to three quarters brightness
    analogWrite(knightLedLine[i-2], p50); //set the one behind it (if it exists) to half brightness
    analogWrite(knightLedLine[i-3], p25); //set the one behind it (if it exists) to quarter brightness
    analogWrite(knightLedLine[i-4], 0); //set the one behind it (if it exists) off
    delay(400); 
  }//end i for
  digitalWrite(ledLayer[2], HIGH);
  reset();
}//end knightTop
