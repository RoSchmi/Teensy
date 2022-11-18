#include <Arduino.h>
#include <Audio.h>

typedef struct GainSetting
      {
        float In_0 = 0.0;
        float In_1 = 0.0;
        float In_2 = 0.0;
        float In_3 = 0.0;
      } GainSetting;

class MixerSettings
{
     private:
     GainSetting GainSetting_none {0.0, 0.0, 0.0, 0.0};
     GainSetting GainSetting_1 {0.3, 0.05, 0.05, 0.0};
     GainSetting GainSetting_2 {0.2, 0.7, 0.4, 0.3};
     GainSetting GainSetting_3 {0.0, 0.0, 0.5, 0.0};
     GainSetting GainSetting_4 {0.0, 0.0, 0.5, 0.0};
      
    public:
      GainSetting getGainsetting(int setting)
      {
        switch (setting)
        {
           case 0: return GainSetting_none; break;
           case 1: return GainSetting_1; break;
           case 2: return GainSetting_2; break;
           case 3: return GainSetting_3; break;
           case 4: return GainSetting_4; break;
           default: return GainSetting_none; break;
        }
      }

      void setMixerGainSettings(AudioMixer4 & mixer, GainSetting gainSetting)
      {
        mixer.gain(0, gainSetting.In_0);
        mixer.gain(1, gainSetting.In_1);
        mixer.gain(2, gainSetting.In_2);
        mixer.gain(3, gainSetting.In_3);   
      }    
};