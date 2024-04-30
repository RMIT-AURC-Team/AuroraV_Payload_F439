# 100Hz Timer (TIM6)
To have the timer interrupt at a frequency of 100Hz, we can use the 8MHz HSI timer with a prescaler and count value.
If we want the clock to run at a speed of 10000Hz, the following formula can be used:

10000 = 8000000/(prescaler+1)
prescaler+1 = 800
prescaler = 799

At this speed, it takes 1/10000 seconds for each tick of the timer. 
We need the timer to reset at a rate of 1/100 seconds to achieve the desired rate of 100Hz

(1/100) / (1/10000) = 100 counts. 

Hence the period value is 100.

# 1Hz Timer (TIM6)
To have the timer interrupt at a frequency of 1Hz, we can use the 16MHz HSI timer with a prescaler and count value.
If we want the clock to run at a speed of 1000Hz, the following formula can be used:

1000 = 16000000/(prescaler+1)
prescaler+1 = 16000
prescaler = 15999

At this speed, it takes 1/100 seconds for each tick of the timer. 
We need the timer to reset at a rate of 1 second to achieve the desired rate of 1Hz

(1) / (1/1000) = 1000 counts. 

Hence the period value is 1000.
