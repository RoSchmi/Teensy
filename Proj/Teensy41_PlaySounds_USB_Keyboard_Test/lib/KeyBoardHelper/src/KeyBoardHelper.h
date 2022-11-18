#include <Arduino.h>
#include "USBHost_t36.h"

class KeyBoardHelper
{
    public:
    KeyBoardHelper();
    void ShowHIDExtrasPress(uint32_t top, uint16_t key);
    void PrintKey(int key);
};