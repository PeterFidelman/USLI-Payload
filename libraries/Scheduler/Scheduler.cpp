// GNU GPL v2

#include<Scheduler.h>
#include<Arduino.h>

//unsigned long __ulCounter = 0;

Scheduler::Scheduler()
{
  __ulCounter = 0;
  clearQueue();
  clearSchedule();
}

void Scheduler::setup()
{
  //Timer2 Settings: Timer Prescaler /64,
  TCCR2A |= (1<<CS22);
  TCCR2A &= ~((1<<CS21) | (1<<CS20));

  // Use normal mode
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));

  // Use internal clock - external clock not used in Arduino
  ASSR |= (0<<AS2);

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= (1<<TOIE2) | (0<<OCIE2A);

  // Reset timer
  RESET_TIMER2;
  sei();
}

void Scheduler::processMessages()
{
  // Check the schedule
  for( int i=0; i<SCHEDULE_MAX; i++)
  {
    if(_taskSchedule[i].task && 
       _taskSchedule[i].milliseconds <= __ulCounter)
    {
      queue(_taskSchedule[i].task);
      _taskSchedule[i].task = 0;
    }
  }
  
  // Clear the queue
  for( int i=0; i<QUEUE_MAX; i++)
  {
    if(_taskQueue[i] )
    {
      _taskQueue[i]->run(this);
      _taskQueue[i] = 0;
    }
  }
}

void Scheduler::queue(ITask* task)
{  
  for( int i=0; i<QUEUE_MAX; i++)
  {
    if(!_taskQueue[i] )
    {
      _taskQueue[i] = task;
      return;
    }
  }
}

void Scheduler::clearQueue()
{
  for( int i=0; i<QUEUE_MAX; i++)
    _taskQueue[i] = 0;
}

void Scheduler::schedule(ITask* task, int milliseconds)
{
  for( int i=0; i<SCHEDULE_MAX; i++)
  {
    if(!_taskSchedule[i].task)
    {
      _taskSchedule[i].task = task;
      _taskSchedule[i].milliseconds = __ulCounter + milliseconds;
      return;
    }
  }
}

void Scheduler::clearSchedule()
{
  for( int i=0; i<SCHEDULE_MAX; i++)
    _taskSchedule[i].task = 0;
}
