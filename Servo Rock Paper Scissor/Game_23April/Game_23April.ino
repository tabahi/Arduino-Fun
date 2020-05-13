#include <Servo.h>
#include <EEPROM.h>

//---------------------------------Settings
#define number_of_options 3
//if you change number_of_options to different number than these arrays should have number of elements equal to number_of_options:
const String option_name[] = {"Rock", "Paper", "Scissor"};  //add or remove elements of your choice if you change number_of_options
const int servo_pin[] = {9, 10, 11};          //rock, paper, scissor
const int servo_position_A[] = {0, 0, 0};     //start position is 0 degrees, rock, paper, scissor
const int servo_position_B[] = {90, 90, 90};  //90 degrees, servo will oscillate between 0 to 90 degrees:
const int select_button_pin[] = {2, 3, 4};  //rock, paper, scissor

/*
// * Example with 5 options:
#define number_of_options 5
//if you change number_of_options to different number than these arrays should have number of elements equal to number_of_options:
const String option_name[] = {"Rock", "Paper", "Scissor", "Cup", "Glass"};  //add or remove elements of your choice if you change number_of_options
const int servo_pin[] = {9, 10, 11, 12, 13};          //rock, paper, scissor, Cup, Glass
const int servo_position_A[] = {0, 0, 0, 0, 0};     //start position is 0 degrees, rock, paper, scissor
const int servo_position_B[] = {90, 90, 90, 90, 90};  //90 degrees, servo will oscillate between 0 to 90 degrees:
const int select_button_pin[] = {2, 3, 4, 5, 6};  //rock, paper, scissor, Cup, Glass
*/

const int yes_no_tie_pin[3] = {2, 3, 4}; //it has fixed number of options. no change.   //yes, no, tie button pin. Currently Same as rock, paper, scissor. Can be different pins

//number of oscillations for servo:
#define oscillations 2  //rock/paper/scissor will move back and forth 3 times
#define back_and_forth_speed 10   //greater this number, lesser the speed

//---------------------------------Settings END

#define win 0
#define loss 1
#define tie 2

Servo servo[number_of_options];

boolean last_state_of_pins[number_of_options];
boolean last_state_yes_no_tie[] = {HIGH, HIGH, HIGH};
int epsilon[number_of_options][number_of_options];
const float alpha = 0.20; //how much to depend on user's opinion of Win or Loss - Higher it is, higher it will depend on user's opinion of game result
const float kappa = 0.50; //Sets the difference of points between win, loss and tie. Lower it is, higher is the difficulty for robot.
const float reward_rate = 40.0; //no need to change

int last_move[] = {0, 0};


void setup()
{
  for (int g = 0; g < number_of_options; g++)
  {
    pinMode(select_button_pin[g], INPUT_PULLUP);
    last_state_of_pins[g] = HIGH;
  }
  for (int g = 0; g < 3; g++)
  {
    pinMode(yes_no_tie_pin[g], INPUT_PULLUP);
  }

  for (int g = 0; g < number_of_options; g++)
  {
    servo[g].attach(servo_pin[g]);
  }

  for (int g = 0; g < number_of_options; g++)
  {
    servo[g].write(servo_position_A[g]);
  }

  Serial.begin(9600);


  //EEPROM_clear(); //uncomment to clear memory

  for (int g = 0; g < number_of_options; g++)
  {
    for (int h = 0; h < number_of_options; h++)
    {
      epsilon[g][h] = 1000;
    }
  }

  for (int g = 0; g < number_of_options; g++)
  {
    for (int b = 0; b < number_of_options; b++)
    {
      int readEEPROMvalue = EEPROM_read_int((g * 6) + (b * 2));
      if (readEEPROMvalue != -1)
      {
        epsilon[g][b] = readEEPROMvalue;
      }
      // Serial.println(epsilon[g][b]);
    }
  }


  Serial.println(F("Waiting for user to play first"));

}

char flag = 0;

void loop()
{


  if (flag == 0)    //user starts the game
  {
    unsigned char user_move = read_move_input();
    if ((user_move >= 0) && (user_move < number_of_options))
    {
      Serial.println("User: " + option_name[user_move]);
      move_servo(best_move_to_play(user_move));
      flag = 1;
    }
  }

  else if (flag == 1)
  {
    unsigned char user_opinion = read_opinion_input();
    if (user_opinion == win)
    {
      if (epsilon[last_move[0]][last_move[1]] < 2000)
      {
        epsilon[last_move[0]][last_move[1]] = epsilon[last_move[0]][last_move[1]] + (int)reward_rate;
        EEPROM_write_int((last_move[0] * 6) + (last_move[1] * 2), epsilon[last_move[0]][last_move[1]]);
      }
      make_descion(last_move[0], last_move[1], user_opinion);
      flag = 2;
    }
    else if (user_opinion == loss)
    {
      if (epsilon[last_move[0]][last_move[1]] > ((int)(reward_rate / kappa)))
      {
        epsilon[last_move[0]][last_move[1]] = epsilon[last_move[0]][last_move[1]] - (int)(reward_rate / kappa);
        EEPROM_write_int((last_move[0] * 6) + (last_move[1] * 2), epsilon[last_move[0]][last_move[1]]);
      }
      make_descion(last_move[0], last_move[1], user_opinion);
      flag = 2;
    }
    else if (user_opinion == tie)
    {
      if (epsilon[last_move[0]][last_move[1]] < 2000)
      {
        epsilon[last_move[0]][last_move[1]] = epsilon[last_move[0]][last_move[1]] - (int)(reward_rate * kappa);
        EEPROM_write_int((last_move[0] * 6) + (last_move[1] * 2), epsilon[last_move[0]][last_move[1]]);
      }
      make_descion(last_move[0], last_move[1], user_opinion);
      flag = 2;
    }
  }
  else if (flag == 2)
  {
    for (int g = 0; g < 3; g++)
    {
      last_state_yes_no_tie[g] = HIGH;
    }
    for (int g = 0; g < number_of_options; g++)
    {
      last_state_of_pins[g] = HIGH;
    }
    flag = 0;
  }
}

unsigned char best_move_to_play(unsigned char user_input)
{
  int bestmove = random(0, (number_of_options - 1));
  for (int p = 0; p < number_of_options; p++)
  {
    if (epsilon[user_input][p] > epsilon[user_input][bestmove])
    {
      bestmove = p;
    }
  }

  last_move[0] = user_input;
  last_move[1] = bestmove;

  return bestmove;
}



unsigned char read_move_input()
{
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.println(c);
    Serial.flush();
    if (c == '1')
    {
      return 1;
    }
    else if (c == '2')
    {
      return 2;
    }
    else if (c == '3')
    {
      return 3;
    }
    else if (c == '4')
    {
      return 4;
    }
    else if (c == '5')
    {
      return 5;
    }
    else if (c == '6')
    {
      return 6;
    }
    else if (c == '7')
    {
      return 7;
    }
    else if (c == '0')
    {
      return 0;
    }
  }

  for (int g = 0; g < number_of_options; g++)
  {
    boolean cstate = digitalRead(select_button_pin[g]);
    if (last_state_of_pins[g] != cstate)
    {
      delay(10);
      if (digitalRead(select_button_pin[g]) != cstate)
      {
        last_state_of_pins[g] = cstate;
        if (cstate == LOW)
        {
          return g;
        }
      }
    }
  }

  return 0xFF;
}


unsigned char read_opinion_input()
{
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.println(c);
    Serial.flush();
    if (c == '1')
    {
      return 1;
    }
    else if (c == '2')
    {
      return 2;
    }
    else if (c == '3')
    {
      return 3;
    }
    else if (c == '0')
    {
      return 0;
    }
  }

  for (int g = 0; g < 3; g++)
  {
    boolean cstate = digitalRead(yes_no_tie_pin[g]);
    if (last_state_yes_no_tie[g] != cstate)
    {
      delay(10);
      if (digitalRead(yes_no_tie_pin[g]) != cstate)
      {
        last_state_yes_no_tie[g] = cstate;
        if (cstate == LOW)
        {
          return g;
        }
      }
    }
  }

  return 0xFF;
}

void move_servo(unsigned char choice)
{

  Serial.println("Robot: " + option_name[choice]);
  int pos = 0;
  for (int k = 0; k < oscillations; k++)
  {
    for (pos = servo_position_A[choice]; pos <= servo_position_B[choice]; pos += 1)
    {
      servo[choice].write(pos);
      delay(back_and_forth_speed);
    }
    for (pos = servo_position_B[choice]; pos >= servo_position_A[choice]; pos -= 1)
    {
      servo[choice].write(pos);
      delay(back_and_forth_speed);
    }
  }
  Serial.println("Done. Enter result:");
}

unsigned char make_descion(unsigned char user_move, unsigned char robot_move, unsigned char user_opinion)
{
  unsigned char descion = user_opinion;
  int min_diff = 0;
  for (int j = 0 ; j < number_of_options; j++)
  {
    if ( ((epsilon[user_move][robot_move]) - (epsilon[user_move][j])) > min_diff)
    {
      min_diff =  ((epsilon[user_move][robot_move]) - (epsilon[user_move][j]));
    }
  }

  if ((epsilon[user_move][robot_move] == 0) || (min_diff == 0))
  {
    descion = user_opinion;
  }
  else
  {
    float f930 = ((float)min_diff / epsilon[user_move][robot_move]);
    //Serial.println(f930);
    if (f930 < alpha)
    {
      descion =  user_opinion;
    }
    else
    {
      descion =  win;
    }
  }


  if (descion == win)
  {
    Serial.println("I Won!");
    Serial.println(F("\r\nNew game. Waiting for your move."));
  }
  else if (descion == loss)
  {
    Serial.println("I Lost!");
    Serial.println(F("\r\nNew game. Waiting for your move."));
  }
  else if (descion == tie)
  {
    Serial.println("It's a tie.");
    Serial.println(F("\r\nNew game. Waiting for your move."));
  }
  return descion;
}


void EEPROM_write_int(int address, int intg)
{
  byte bytes_1 = intg & 0xFF;
  byte bytes_2 = intg >> 8;
  EEPROM.write(address, bytes_1);
  EEPROM.write(address + 1, bytes_2);
}

int EEPROM_read_int(int address)
{

  byte bytes_1 = EEPROM.read(address);
  byte bytes_2 = EEPROM.read(address + 1);
  if ((bytes_1 == 255) || (bytes_2 == 255))
  {
    return -1;
  }
  int ret = (bytes_2 << 8) | bytes_1;
  return ret;
}

void EEPROM_clear()
{
  for (int i = 0 ; i < 20 ; i++)
  {
    EEPROM.write(i, 0xFF);
  }
}

