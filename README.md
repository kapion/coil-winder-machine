# Coil winder machine

##### It helps make wires coil with defined amount of turnover!
---

## Features

  - Start/stop motor
  - Control of turnovers
  - Revers
  - Autostop
  - Set counter to zero


You can also:
  - Setup stop counter
  - Continue run after autostop

## How to manage
```sh
   NORMAL MODE {
    //direction depends on reverseKey
    single click - start/stop motor
    when stop and hold - main counter to 0
    when stop and double click - switch to SETUP MODE (display out Edit)
   } 

   SETUP MODE {
    //direction depends on reverseKey
    single click - increase/decrease stopCount value per 100
    hold - increase/decrease stopCount value per 500
    double click - store stopCount value
   }

   ALL MODES {
    switch reverse is working
   }
```

### Materials and components
    
- Arduino (eg Nano) 
- Arduino Relay
- Arduino TM1637 (Digital display)
- Motor
- Power supply for arduino board
- Independent motor power supply
- Reverse switch
- Reed switch or contact button
- Hall sensor 
- Resistor 10 kOm
- Main button
- 2 small magnets

### Plugins

You need install plugins for Arduino IDE:

| Plugin | Source |
| ------ | ------ |
| GyverTM1637 | [AlexGyver/GyverLibs/GyverTM1637][TM1637] |
| GyverButton | [AlexGyver/GyverLibs/GyverButton][button]  |

### Development

Want to contribute? Great!

See build example here...

## License


MIT ;-)


**Free Software, Hell Yeah!**

   [button]: <https://github.com/AlexGyver/GyverLibs/tree/master/GyverButton>
   [TM1637]: <https://github.com/AlexGyver/GyverLibs/tree/master/GyverTM1637>
 
