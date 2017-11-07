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
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
 
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
  static int place = 0;
  static int pass [] = {1,2,3,2,0,0,0,0,0,0};
  static int store [] = {0,0,0,0,0,0,0,0,0,0};
  static int lock = 1;
  static bool bRed = TRUE;
  static bool bGreen = TRUE;
  
  /* locked states */
  if (lock == 1) {
    LedOff(GREEN);
    LedOn(RED);
  }
  if (lock == 2) {
    LedOff(GREEN);
    if (bRed) {
      bRed = FALSE;
      LedBlink(RED, LED_4HZ);
    }
  }
  if (lock == 3) {
    LedOff(RED);
    if (bGreen) {
      bGreen = FALSE;
      LedBlink(GREEN, LED_4HZ);
    }
  }

  /*Light when inputing buttons*/
  if (IsButtonPressed(BUTTON0)) {
    LedOn(WHITE);
  }
  else {
    LedOff(WHITE);
  }
  if (IsButtonPressed(BUTTON1)) {
    LedOn(PURPLE);
  }
  else {
    LedOff(PURPLE);
  }
  if (IsButtonPressed(BUTTON2)) {
    LedOn(BLUE);
  }
  else {
    LedOff(BLUE);
  }

  /*Storage of input password*/
  if (WasButtonPressed (BUTTON0) && lock == 1) {
    ButtonAcknowledge(BUTTON0);
    store[place] = 1;
    place++;
  } 
  else if (WasButtonPressed (BUTTON1) && lock == 1) {
    ButtonAcknowledge(BUTTON1);
    store[place] = 2;
    place++;
  }
  else if (WasButtonPressed (BUTTON2) && lock == 1) {
    ButtonAcknowledge(BUTTON2);
    store[place] = 3;
    place++;
  }
  else if (WasButtonPressed (BUTTON3) && lock == 1 && place != -1) {
    ButtonAcknowledge(BUTTON3);
    if (place > 10) {
      lock = 2;
    }
    else {
      store[place] = 0;
      for (int i = 0; i < 10; i++) {
        if (pass[i] != store[i]) {
          lock = 2;
          place = 0;
          break;
        } 
      }
    }
    if (lock == 1)
      lock = 3;
    for (int j = 0; j < 10; j++) {
      store[j] = 0;
    }
  }
  else if ( (WasButtonPressed(BUTTON0) || WasButtonPressed(BUTTON1) || WasButtonPressed(BUTTON2) || WasButtonPressed(BUTTON3)) && lock > 1) {
    place = -1;
    bRed = TRUE;
    bGreen = TRUE;
    lock = 1;
  }

  
        
      

} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
