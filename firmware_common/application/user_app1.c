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

/* detection counter */
static u8 num[] = "0123456789";
static int trans = 0;
static u8 *numdetect1 = num;
static u8 *numdetect2 = num;
static int ten = 0;
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
  
  /* initial LCD messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, clear);
  LCDMessage(LINE2_START_ADDR, numofdetect);
  LCDMessage(LINE2_START_ADDR + 17, "0");
  
  


  
 
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
    trans = 1;
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
    LedOn(LCD_RED);
    LedOff(GREEN);
    LedOff(YELLOW);
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
    
    
    UserApp1_StateMachine = UserApp1SM_Detected;
  }    
  
  if (trans == 0) {
    /* if transitioning to an idle state, do the following */
    
    /* set LEDs */
    LedOff(LCD_RED);
    LedOn(LCD_GREEN);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(YELLOW);
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
    
    UserApp1_StateMachine = UserApp1SM_Idle;
    
  }
  
  if(trans == 2) {
    /* if transitioning to the nothing state, do the following */
    
    /* set LEDs */
    LedOn(LCD_GREEN);
    LedOn(LCD_RED);
    LedOff(RED);
    LedOff(BLUE);
    LedOff(GREEN);
    LedOn(YELLOW);
    
    /* sets LCDs */
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, standby);
    
    /* turn off buzzers */
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    
    UserApp1_StateMachine = UserApp1SM_Standby;
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
  if (WasButtonPressed(BUTTON2)) {
    ButtonAcknowledge(BUTTON2);
    
    trans = 0;
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
