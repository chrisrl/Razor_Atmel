/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

/* Messages */
static u8 clear[] = "ALL CLEAR.";
static u8 detect[] = "GOTCHA ! ! !";
static u8 numofdetect[] = "# OF DETECTIONS:";
static u8 standby[] = "PLEASE STAND BY.";
static u8 mode[] = "CAMERA MODE:";
static u8 burstm[] = "BURST";
static u8 single[] = "SINGLE";
static u8 timer3[] = "TIMER3";
static u8 timer10[]  = "TIMER10";
static u8 booth[] = "BOOTH";

static u8 *modep = burstm;

/* detection counter */
static u8 num[] = "0123456789";
static int trans = 0;
static u8 *numdetect1 = num;
static u8 *numdetect2 = num;
static int ten = 0;

static u8 *timerp = num;
/*mode constant*/
static int modec = 0;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  /* initialize LEDs */
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
  LedOff(LCD_BLUE);
  LedOff(LCD_RED);
  LedBlink(GREEN, LED_1HZ);
  LedPWM(WHITE,LED_PWM_5);
  
  /* initial LCD messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, clear);
  LCDMessage(LINE2_START_ADDR, numofdetect);
  LCDMessage(LINE2_START_ADDR + 17, "0");
  
  LedOff(BLADE_AN0);
  
  


  
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static int timer = 0;
  static int dots = 10; //counts the dots


  if( IsButtonPressed(BLADE_AN1) || IsButtonPressed(BUTTON0) ) {
    /* The PIR is detecting something , go to transition state*/
    
    if (modec == 2 || modec == 3) {
      trans = 3;
    }
    
    else if (modec == 4) {
      trans = 4;
    }
    else {
      trans = 1;
    }
    
    timer = 0;
    dots = 10;
    UserApp1_StateMachine = UserApp1SM_Transition;
    
  }
  
  else if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
    
    timer = 0;
    trans = 2;
    dots = 10;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  else {
   /* The PIR is not detecting anything, idle state */
    timer++;
    
    /* reset the counter */
    if(WasButtonPressed(BUTTON1)) {
      ButtonAcknowledge(BUTTON1);
      
      ten = 0;
      numdetect1 = num;
      numdetect2 = num;
      
      LCDClearChars(LINE2_START_ADDR + 18, 2);
      LCDMessage(LINE2_START_ADDR + 17, "0");
    }
    
    if (dots == 13) {
      LCDClearChars(LINE1_START_ADDR + 9, 4);
      dots = 9;
    }
    
    if(timer >= 500) {
      LCDMessage(LINE1_START_ADDR + dots, ".");
      dots++;
      timer = 0;
    }
    
   
 
  }
  
      
} /* end UserApp1SM_Idle() */

static void UserApp1SM_Transition(void)
{
  if (trans == 1) {
    /* If transitioning to detection, do th following */
    
    /* set LEDs */
    LedOff(LCD_GREEN);
    LedOff(LCD_BLUE);
    LedOn(LCD_RED);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(CYAN);
    LedOff(ORANGE);
    LedBlink(RED, LED_2HZ);
    
    /* set LCD */
    LCDCommand(LCD_CLEAR_CMD);
    
    
    /* turn on buzzers */
   
    PWMAudioOn(BUZZER2);
    PWMAudioOn(BUZZER1);
  
    /* updates counter. checks if counting with a tens place or not */
    if(ten == 1) {
      numdetect2++;
    }
    if (ten == 0 || *numdetect2 == '\0') {
      numdetect1++;
    }
    
    
    /*send constant signal to camera if in burst or single mode*/
    LedOn(BLADE_AN0);
    
    
    
    UserApp1_StateMachine = UserApp1SM_Detected;
  }    
  
  if (trans == 0) {
    /* if transitioning to an idle state, do the following */
    
    /* set LEDs */
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOn(LCD_GREEN);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(YELLOW);
    LedOff(CYAN);
    LedOff(ORANGE);
    LedBlink(GREEN, LED_1HZ);
    
    /* checks if counter is past the num array, sets ten mode and resets counters */
    if(*numdetect1 == '\0') {
      numdetect1 -= 9;
      ten = 1;
    }
    if(*numdetect2 == '\0') {
      numdetect2 -= 10;
    }
    /* sets LCDs */
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, clear);
    LCDMessage(LINE2_START_ADDR, numofdetect);
    LCDMessage(LINE2_START_ADDR + 17, numdetect1);
    
    /* if not in ten mode, clear last two chars, otherwise clear only last char */
    if(ten == 0) {
      LCDClearChars(LINE2_START_ADDR + 18, 2);
    }
    else {
      LCDMessage(LINE2_START_ADDR + 18, numdetect2);
      LCDClearChars(LINE2_START_ADDR + 19, 1);
    }
    
    
    /* turn off buzzers */
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    
    /* no signal to camera */
    LedOff(BLADE_AN0);
    
    UserApp1_StateMachine = UserApp1SM_Idle;
    
  }
  
  if(trans == 2) {
    /* if transitioning to the nothing state, do the following */
    
    /* set LEDs */
    LedOff(LCD_BLUE);
    LedOn(LCD_GREEN);
    LedOn(LCD_RED);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(GREEN);
    LedOn(YELLOW);
    
    /* sets LCDs */
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, standby);
    LCDMessage(LINE2_START_ADDR, mode);
    LCDMessage(LINE2_START_ADDR + 13, modep);
    
    /* turn off buzzers */
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    
    /* no signal to camera */
    LedOff(BLADE_AN0);

    
    UserApp1_StateMachine = UserApp1SM_Standby;
  }
  
  if(trans == 3) {
    /* if transitioning to the timer state, do the following */
    
    /*set LEDs*/
    LedOff(LCD_RED);
    LedOff(LCD_GREEN);
    LedOn(LCD_BLUE);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(ORANGE);
    LedBlink(CYAN, LED_1HZ);
    
    /* set LCD */
    LCDCommand(LCD_CLEAR_CMD);
    
    /* buzzers on */
    PWMAudioOn(BUZZER2);
    PWMAudioOn(BUZZER1);
    
    /* updates counter. checks if counting with a tens place or not */
    if(ten == 1) {
      numdetect2++;
    }
    if (ten == 0 || *numdetect2 == '\0') {
      numdetect1++;
    }
     
    UserApp1_StateMachine = UserApp1SM_Timer;

  }
  
  if(trans == 4) {
    /* if transitioning to the timer state, do the following */
    
     /*set LEDs*/
    LedOff(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(CYAN);
    LedBlink(ORANGE, LED_1HZ);
    
      /* set LCD */
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "~~~~~FOUR SHOTS~~~~~");
    
    
    
    /* updates counter. checks if counting with a tens place or not */
    if(ten == 1) {
      numdetect2++;
    }
    if (ten == 0 || *numdetect2 == '\0') {
      numdetect1++;
    }
    
    UserApp1_StateMachine = UserApp1SM_Booth;
    
  }
}

static void UserApp1SM_Detected(void)
{
  static int counter = 0;
  static int blue = 0;

  if( IsButtonPressed(BLADE_AN1) || IsButtonPressed(BUTTON0) ) {
    /* adjusts frequency within a range */
    static int  freq = 912;
    static int increment = -1;

    PWMAudioSetFrequency(BUZZER1, freq);
    PWMAudioSetFrequency(BUZZER2, freq);
    
    if (freq >= 912)
       increment = -1;
    if (freq <= 635)
       increment = 1;
    
    freq = freq + increment;
    
    if(counter == 250 && blue == 0) {
      LedBlink(BLUE, LED_2HZ);
      blue = 1;
    }
    
    if(counter == 0 || counter == 500 || counter == 1000) {
      LCDMessage(LINE1_START_ADDR, detect);
      LCDClearChars(LINE2_START_ADDR, 20);
    }
    
    if(counter == 250 || counter == 750) {
      LCDMessage(LINE2_START_ADDR, detect);
      LCDClearChars(LINE1_START_ADDR, 20);
    }
    
    if(counter == 1000) {
      counter = -1;
    }
    
    /* switch off signal if in single mode */
    if(counter == 50 && modec == 1) {
      LedOff(BLADE_AN0);
      LedOff(CYAN);
    }
    
    
    counter++;

  }
  
  else if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
    
    counter = 0;
    trans = 2;
    blue = 0;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  else {
    counter = 0;
    trans = 0;
    blue = 0;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
   
}

static void UserApp1SM_Standby(void)
{
  static int count = 0;
  
  count += count;
  
  if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
    
    trans = 0;
    count = 0;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  /* switch shooting mode */
  if (WasButtonPressed(BUTTON3)) {
    ButtonAcknowledge(BUTTON3);
    
    if (modec < 4) {
      modec++;
    }
    else {
      modec = 0;
    }
    
    
    if (modec == 0) {
      LedOff(PURPLE);
      LedOff(ORANGE);
      LedOff(CYAN);
      LedPWM(WHITE,LED_PWM_5);
      modep = burstm;
      LCDClearChars(LINE2_START_ADDR + 13, 7); 
      LCDMessage(LINE2_START_ADDR + 13, modep);
    }
    
    if (modec == 1) {
      LedOff(WHITE);
      LedOff(ORANGE);
      LedOff(CYAN);
      LedPWM(PURPLE,LED_PWM_5);
      modep = single;
      LCDClearChars(LINE2_START_ADDR + 13, 7); 
      LCDMessage(LINE2_START_ADDR + 13, modep);
    }
    
    if (modec == 2 || modec == 3) {
      LedOff(PURPLE);
      LedOff(WHITE);
      LedOff(ORANGE);
      LedPWM(CYAN,LED_PWM_5);
      
      if (modec == 2) {
        modep = timer3;
      }
      else {
        modep = timer10;
      }
      
      LCDClearChars(LINE2_START_ADDR + 13, 7); 
      LCDMessage(LINE2_START_ADDR + 13, modep);
    }
    
    if (modec == 4) {
      LedOff(WHITE);
      LedOff(PURPLE);
      LedOff(CYAN);
      LedPWM(ORANGE,LED_PWM_5);
      modep = booth;
      LCDClearChars(LINE2_START_ADDR + 13, 7); 
      LCDMessage(LINE2_START_ADDR + 13, modep);
    }
  }
  
  if ( IsButtonPressed(BUTTON0) ) {
    ButtonAcknowledge(BUTTON0);
    
    if (modec == 0 || modec == 1) {
      
      trans = 1;
      UserApp1_StateMachine = UserApp1SM_Transition;
      
    }
    
    if (modec == 2 || modec == 3) {
      
      trans = 3;
      UserApp1_StateMachine = UserApp1SM_Transition;
      
    }
    
    if (modec == 4) {
      
      trans = 4;
      UserApp1_StateMachine = UserApp1SM_Transition;
    }
  }
}

static void UserApp1SM_Timer(void)
{
  static int clock = 0;
  static int sound = 262;
  static int sound_increment = 30;
  static int timesup = 0;
  
  if( IsButtonPressed(BLADE_AN1) || IsButtonPressed(BUTTON0) ) {
    
    
    if( (clock % 1000) == 0 ) {
     PWMAudioSetFrequency(BUZZER1, sound);
     PWMAudioSetFrequency(BUZZER2, sound);
     sound += sound_increment;
     
     if (timesup == 0) {
       LCDMessage(LINE1_START_ADDR + 10, timerp);
       LCDClearChars(LINE1_START_ADDR + 11, 9);
       timerp++;
     }
     
    }
    
    if (clock == 3000 && modec == 2) {
      LedOn(BLADE_AN0);
      sound_increment = 0;
    }
    
    if (clock == 10000 && modec == 3) {
      LedOn(BLADE_AN0);
      sound_increment = 0;
    }
    
    if (clock == 3100 && modec == 2) {
      LedOff(BLADE_AN0);
      LedOn(CYAN);
      timesup = 1;
      LCDMessage(LINE2_START_ADDR + 7, "CHEESE!");
    }
    
    if (clock == 10100 && modec == 3) {
      LedOff(BLADE_AN0);
      LedOn(CYAN);
      timesup = 1;
      LCDMessage(LINE2_START_ADDR + 7, "CHEESE!");
    }
    
    clock++;
  }
  
   else if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
   
    trans = 2;
    clock = 0;
    sound_increment = 30;
    sound = 262;
    timerp = num;
    timesup = 0;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  else {
    trans = 0;
    clock = 0;
    sound_increment = 30;
    sound = 262;
    timerp = num;
    timesup = 0;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
    

}

static void UserApp1SM_Booth(void)
{
  static int tick = 0;
  static int song[] = {392, 466, 440, 294};
  static int *songp = song;
  
  if( IsButtonPressed(BLADE_AN1) || IsButtonPressed(BUTTON0) ) {
    
    
    if (tick == 1000 || tick == 4000 || tick == 7000 || tick == 10000) {
      
      LedOn(BLADE_AN0);
      
      timerp++;
      LCDMessage(LINE2_START_ADDR + 10, timerp);
      LCDClearChars(LINE2_START_ADDR + 11, 9);
      
      /* buzzers on */
      PWMAudioOn(BUZZER2);
      PWMAudioOn(BUZZER1);
      
      PWMAudioSetFrequency(BUZZER1, *songp);
      PWMAudioSetFrequency(BUZZER2, *songp);
     
      songp++;
      
    }
    
    if (tick == 1100 || tick == 4100 || tick == 7100 || tick == 10100) {
    
      LedOff(BLADE_AN0);
    }
    
    tick++;
    
    
  }
  else if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
   
    trans = 2;
    tick = 0;
    timerp = num;
    songp = song;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  else {
    trans = 0;
    tick = 0;
    timerp= num;
    songp = song;
    UserApp1_StateMachine = UserApp1SM_Transition;
  }
  
  
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
