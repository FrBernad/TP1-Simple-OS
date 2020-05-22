#include <keyboard.h>
#include <keyboardInfo.h>
#include <commands.h>
#include <keys.h>
#include <stringLibrary.h>
#include <videoDriver.h>

#define BUFFER_SIZE 50

#define ABS(x) x > 0 ? x : x*-1

static uint8_t action(uint8_t scanCode);
static void checkCommand();
static void cleanBuffer();

static const char pressCodes[KEYS][2] =
    {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'},
    {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('},
    {'0', ')'}, {'-', '_'}, {'=', '+'}, {0, 0}, {0, 0}, {'q', 'Q'}, 
    {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, 
    {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, 
    {']', '}'}, {0, 0}, {0, 0}, {'a', 'A'}, {'s', 'S'}, 
    {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'},
    {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'},
    {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, 
    {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, 
    {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

static uint8_t scanCode, currentAction, specialChars = 0, capsLock = 0, bufferIndx = 0, l_ctrl = 0;
static char buffer[BUFFER_SIZE]={0};
static t_command commands[] = {{&help, "help"}, {&inforeg, "inforeg"}, {&printmem, "printmem"}, {&time, "time"}, {&cpuid, "cpuid"}, {&temp, "temp"}, {0 , ""}};

void
keyboard_handler()
{
    if (hasKey())
    {
        scanCode = getKey();
        currentAction = action(scanCode);
        if (currentAction == PRESSED)
        {
            switch (scanCode)
            {
            case L_SHFT:
            case R_SHFT:
                specialChars = 1;
                break;

            case ENTER:
                newLine();
              //  checkCommand();
                cleanBuffer();
                break;

            case CAPS_LCK:
                scrollDownScreen();
                capsLock = capsLock == 1 ? 0 : 1;
                break;

            case SPACE:
                buffer[bufferIndx++] = 0;
                putChar(' ');
                break;

            case L_CONTROL:
                l_ctrl = 1;
                break;

            case B_SPACE:
                if(bufferIndx!=0){
                    buffer[bufferIndx--]=0;
                }
                deletechar();
                break;

            default:
                if (pressCodes[scanCode][0])
                {
                    if (l_ctrl){
                        if (pressCodes[scanCode][0] == '0' + SCREEN_1)
                        {
                            changeScreen(SCREEN_1);
                        }
                        else if (pressCodes[scanCode][0] == '0' + SCREEN_2)
                        {
                            changeScreen(SCREEN_2);
                        }
                        else if (pressCodes[scanCode][0] == 'l'){
                            clearScreen();
                        }
                        l_ctrl = 0;
                    }
                    else{
                        char letter;
                        if (!IS_LETTER(pressCodes[scanCode][0]))
                        {
                            letter = pressCodes[scanCode][specialChars];
                            buffer[bufferIndx++] = letter;
                            putChar(letter);
                        }
                        else
                        {
                            {
                                letter = pressCodes[scanCode][ABS(capsLock - (specialChars))];
                                buffer[bufferIndx++] = letter;
                                putChar(letter);
                            }
                        }
                    }
                }
            }
        }
        else if (currentAction == RELEASED)
        {
            switch (scanCode)
            {
            case L_SHFT | 0x80:   //for realease code 
            case R_SHFT | 0x80:
                specialChars = 0;
                break;

            case L_CONTROL | 0x80:
                l_ctrl = 0;
                break;
            }
        }
    }
}

static void checkCommand(){
    uint32_t command,found=0;

    if(buffer[0]==0){
        return;
    }

    for (command = 0; commands[command].command != 0 && !found; command++)
    {
        if(stringcmp(commands[command].name,buffer)){
            found=1;
        }
    }
    if(found){
        switch(command-1){
            case HELP:
                commands[HELP].command();
                break;
            case INFOREG:
                commands[INFOREG].command();
                break;
            case PRINTMEM:
                commands[PRINTMEM].command();
                break;
            case TIME:
                commands[TIME].command();
                break;
            case CPUID:
                commands[CPUID].command();
                break;
            case TEMP:
                commands[TEMP].command();
                break;
        }
    }

    else{
        printStringLn("Invalid command");
    }
    
}

static uint8_t action(uint8_t scanCode)
{
    if (scanCode >= 0x01 && scanCode <= 0x3A)
        return PRESSED;
    else if (scanCode >= 0x81 && scanCode <= 0xBA)
        return RELEASED;

    return ERRROR;
}

static void cleanBuffer(){
    for (uint32_t i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i]=0;
    }
    bufferIndx=0;
}
