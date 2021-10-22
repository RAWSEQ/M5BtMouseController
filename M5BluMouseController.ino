/****************************************************************
 * M5 Script Mouse
****************************************************************/
#include "M5Atom.h"
#include <BleMouse.h>
#include "Arduino_JSON.h"

BleMouse bleMouse("M5BluMouseController", "RAWSEQ", 100);

void setup()
{
    M5.begin(true, false, true);
    M5.dis.drawpix(0, 0x00ff00);
    Serial.println("StartMouse.");
    bleMouse.begin();
    M5.dis.drawpix(0, 0x6800B7);
    Serial.println("Ready.");
}

void loop()
{
    if (bleMouse.isConnected())
    {
        if (Serial.available())
        {
            String text = Serial.readStringUntil(0x0a);
            if (text.length() > 0 && text.startsWith("["))
            {
                JSONVar cmds_obj = JSON.parse(text);
                
                for (uint8_t i = 0; i < cmds_obj.length(); i++) {
                    JSONVar cmd_obj = cmds_obj[i];

                    uint8_t times = 1;
                    if (cmd_obj.hasOwnProperty("t"))
                    {
                        times = String((const char*)cmd_obj["t"]).toInt();
                    }
                    uint8_t delay_ms = 50;
                    if (cmd_obj.hasOwnProperty("d"))
                    {
                        delay_ms = String((const char*)cmd_obj["d"]).toInt();
                    }
                    for (uint8_t j = 1; j <= times; j++)
                    {
                        String cmd = String((const char*)cmd_obj["cmd"]);
                        if (cmd == "c") // click
                        {
                            bleMouse.click();
                        }
                        else if (cmd == "p") // press
                        {
                            bleMouse.press();
                        }
                        else if (cmd == "r") // release
                        {
                            bleMouse.release();
                        }
                        else if (cmd == "rc") // RightClick
                        {
                            bleMouse.click(MOUSE_RIGHT);
                        }
                        else if (cmd == "mc") // MiddleClick
                        {
                            bleMouse.click(MOUSE_MIDDLE);
                        }
                        else if (cmd == "m") // move
                        {
                            bleMouse.move(String((const char*)cmd_obj["x"]).toInt(), String((const char*)cmd_obj["y"]).toInt(), 0);
                        }
                        else if (cmd == "w") // wheel
                        {
                            bleMouse.move(0, 0, String((const char*)cmd_obj["w"]).toInt());
                        }
                        delay(delay_ms);
                    }
                }
                Serial.println("processed.");
            }
        }
    }
    delay(50);
    M5.update();
}

