/*        Your Name & E-mail: Angelica Simityan   asimi003@ucr.edu

 *         Discussion Section: 021

 *         Assignment: Lab # 6 Exercise # 3

 *         Exercise Description: [optional - include for your own benefit]

 *

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link:

https://youtube.com/shorts/eNSKDXHOoP8?feature=share

 */

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.

const int light_pattern_size = 4;
int light_pattern[light_pattern_size] = {0, 0, 0, 0};

const int pattern_size = 4;
int pattern[light_pattern_size] = {0, 0, 0, 0};


const char sound_pattern_size = 9;
const double sound_pattern[sound_pattern_size] = {329.63,311.13,329.63,311.13,329.63,493.88,293.66,261.63,440.00}; // C4, D4

const int buzzer = 3;

int LightLastRun = 0;
int LightPeriod = 500;
int LightStatus = 0;
int b_buf = 0;

int LightLastRun2 = 0;
int LightPeriod2 = 100;
int LightStatus2 = 0;

int code_true = 1;
int code_true_num = 0;

int btn_state = 5;

int record[4] = {5,5,5,5};

const int button = 2;

int countMusic =0;
int countLight =0;

int LightMap[4] = {2,4,1,8};

const int xAxis_median = 100; // Adjust this if needed

// Reset the Output Buffer. 
void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
    }

}


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, SM1_S0, SM1_S1, SM1_S2};
int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM1_INIT:
         //State Transition
            state = SM1_S0;
             for(int i = 0; i<4;i++){
              pattern[i] = random(4);
              }
        break;
      case SM1_S0:
          if (countLight == 4){
            state = SM1_S1;
            countLight = 0;
          }
          else{
            state = SM1_S0;
          }
         //State Transition
        break;
      case SM1_S1:
            if (digitalRead(button) == 0 || code_true == 0){
                state = SM1_S0;
                code_true = 1;
                code_true_num = 0;
                 for(int i = 0; i<4;i++){
                  pattern[i] = random(4);
                }
            }
            else if(code_true == 1 && code_true_num == 4)
            {
              state = SM1_S2;
              code_true_num = 0;
            }
            else{
                state = SM1_S1;
            }
             //State Transition

        break;
        case SM1_S2:
        if (digitalRead(button) == 0){
                state = SM1_S0;
                 for(int i = 0; i<4;i++){
                 pattern[i] = random(4);
                  }
                  code_true_num = 0;
        }
            else{
                state = SM1_S2;
            }
       
             //State Transition
        break;
    }
    switch(state){ // State Action
      case SM1_INIT:
      
         //State Action
        break;
      case SM1_S0:
            //State Action
            
            
           
           
           
            if ((millis() - LightLastRun) >= LightPeriod)
            {
              Serial.println(countLight);
              if (LightStatus == 0)
              {
                b_buf = LightMap[pattern[countLight]];
                writeBuffer(b_buf);
                
                LightStatus = 1;
              }
              else{
                resetBuffer();
                countLight++;
                LightStatus = 0;
              }
              LightLastRun = millis();
            }
         //State Action
        break;
      case SM1_S1:
      
            //State Action
           // if the joystick or button are in center position
            if(btn_state == 5){

              if(analogRead(A0) > 850)
              {
                 
                 btn_state =2;
              }
              else if (analogRead(A0) < 250)
              {
                
                btn_state = 3;
              }
              else if (analogRead(A1) < 200)
              {
                
                btn_state = 0;
              }
              else if (analogRead(A1) > 600)
              {
                
                btn_state = 1;
              }
              
            }


            
            //If the joystick or button are still pressed
            else
            {
              
              //checks if the up down is on center position
              if(btn_state == 0 || btn_state == 1){
                if(analogRead(A1) < 850 && analogRead(A1) >250){
                  if(pattern[code_true_num]  == btn_state)
                   {
                    code_true = 1;
                    code_true_num++;
                  }
                  else
                  {
                    code_true = 0;
                  }
                  btn_state = 5;
                  resetBuffer();
                }
                //if not then play sound
                else{
                       b_buf = LightMap[btn_state];
                     writeBuffer(b_buf);
                      
                    }
              }

 
              
              //checks if the left or right are on center position
             if(btn_state == 2 || btn_state == 3){
                if(analogRead(A0) < 850 && analogRead(A0) >250){
                   if(pattern[code_true_num]  == btn_state)
                   {
                    code_true = 1;
                    code_true_num++;
                  }
                  else
                  {
                    code_true = 0;
                  }
                  btn_state = 5;
                  resetBuffer();
                }
              
              //if not play sound
              else{
                     
                     b_buf = LightMap[btn_state];
                     writeBuffer(b_buf);
                  }

             }
             //checks if the button is released
           
         }
         
           
        
         //State Action
        break;

          case SM1_S2:
            //State Action
           if ((millis() - LightLastRun2) >= LightPeriod2)
            {
              Serial.println(countLight);
              if (LightStatus2 == 0)
              {
                
                writeBuffer(15);
                
                LightStatus2 = 1;
              }
              else{
                resetBuffer();
                
                LightStatus2 = 0;
              }
              LightLastRun2 = millis();
            }
         //State Action
        break;
    }

    return state;
}

enum SM2_States { SM2_INIT, SM2_S0};
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM2_INIT:
         //State Transition
            state = SM1_S0;
        break;
      case SM2_S0:
            
        //State Transition
        break;
  
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        break;
      case SM2_S0:
            //State Action
  
            
            tone(buzzer, sound_pattern[countMusic]);
        
            countMusic++;
            if (countMusic > 9){
              countMusic = 0;
            }
          
         //State Action
        break;
     
    }

    return state;
}






void setup() {
    //some set up (Default Arduino Function)

    // LEDs
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    Serial.begin(9600);

    pinMode(button, INPUT_PULLUP);

     randomSeed(analogRead(A5));   
     
  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;

  //delay_gcd = 500; // GCD

}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
  //delay(delay_gcd); // GCD.
}
