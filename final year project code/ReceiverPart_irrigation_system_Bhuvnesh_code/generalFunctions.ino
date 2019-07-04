
/***************************************************
* Wait Button to be pressed a defined time in seconds
****************************************************/
void waitButtonPress (int waitTime)
{
  long startTiming = millis();
  while (debounce(SENSORS_READ_BUTTON)) 
  { 
    if ((millis()-startTiming) > (waitTime*1000)) break;
  }
}

/***************************************************
* Starting Timers
****************************************************/
void startTimers(void)
{
  timer.setInterval(READ_BUTTONS_TM*1000, readLocalCmd);
  //timer.setInterval(DISPLAY_DATA_TM*1000, displayData);
}

/***************************************************
* Debouncing a key
****************************************************/
boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  const int debounceDelay = 30;
  
  previousState = digitalRead(pin);
  for(int counter=0; counter< debounceDelay; counter++)
  {
    delay(1);
    state = digitalRead(pin);
    if(state != previousState)
    {
      counter = 0;
      previousState = state;
    } 
  }
  return state;
}
