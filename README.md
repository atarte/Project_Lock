# Project_Lock

![cover](https://github.com/atarte/Project_Lock)

### Participant : [C-atarina](https://github.com/C-atarina), [atarte](https://github.com/atarte)

## Security

For opening the gate, you need to enter the correct password on the keypad. But if you want to reset the current password will the code at runtime. You can do it by using a RFID card on the sensor and setting a new password.

You can also change the password in the code at line 20 and change the length of the password at line 13.

And you can also change the RFID card to reset the password at line 19. 

## Code

You can find the code for your project in the file  [main.ino](https://github.com/atarte/main/main.ino).

This code is design to run on an **arduino mega** because of the number of pin required (*13 in total*). But you probably can use an other card. For that you will only need to change the old pin in the `#define` list at the beginning or the bytes array at line 33 and 34 for the keypad.

## Compiling

For compiling the code, you can use the **Arduino IDE** or your own compiler (*like with a makefile*) if you want.
