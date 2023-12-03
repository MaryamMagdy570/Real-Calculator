# Real-Calculator
## Description
A calculator application using ATmega32, 4x4 keypad, and 4X20 character LCD.
it takes user input from a 4x4 keypad and outputs the result on a 4x20 LCD.
## Features
- Supports the 4 mathematical operations "+,-,*,/".
- Respects the precedence of operations when applying multiple operations in the same expression.
- Dealing with positive or negative numbers.
- Dealing with both integer and floating numbers up to 5 decimals after point.
- handling the repeated symbol like "++,--,+-"
  where "++" is treated as "+" , and both "--,+-" are treated as "-".
- give a "Math Error" message if any invalid repeated symbols else like "//,**,.*,./, ... etc"
- give a "Math Error" message at division by zero.
## Video
https://github.com/MaryamMagdy570/Real-Calculator/assets/82731141/1d1dcbbd-7ba8-441b-94ac-746681c415fb
## keypad matrix
![4x4-Keypad-Module-Pinout](https://github.com/MaryamMagdy570/Real-Calculator/assets/82731141/bb498984-255d-441a-a313-da0e0ff385db)

