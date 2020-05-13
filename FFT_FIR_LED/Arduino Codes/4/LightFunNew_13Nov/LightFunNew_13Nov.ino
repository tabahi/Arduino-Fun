#define REDpin 5
#define GREENpin 9
#define BLUEpin 3

#define REDgain 1.3
#define GREENgain 1.5
#define BLUEgain 0.8

#define REDsensitivity 1
#define GREENsensitivity 20
#define BLUEsensitivity 2



#define FILTERTAPS 30
#define MAXVAL 1023
#define MAXVALREL 256
#define MINVAL 0
#define AVGNUM 30
#define WAKEUPSAMP 2
#define BUFFER 40
#define FADEGAP 1
#define TIMECHANGEMIL 5000
#define FREQ 8000
#define SENDSERIAL 0

template<int filterTaps>
class FIR {
  public:
    //construct without coefs
    FIR() {
      k = 0; //initialize so that we start to read at index 0
      for (int i = 0; i < filterTaps; i++) {
        values[i] = 0; // to have a nice start up, fill the array with 0's
      }
      //TODO calculate default gain?
      //TODO calculate default coefs?
    }
    //construct with coefs
    FIR(float newGain, float *newCoefs) {
      k = 0; //initialize so that we start to read at index 0
      setGain(newGain);
      for (int i = 0; i < filterTaps; i++) {
        values[i] = 0; // to have a nice start up, fill the array with 0's
      }
      setCoefficients(newCoefs);
    }

    void setGain(float newGain) {
      gain = newGain;
    }

    void setCoefficients(float *newCoefs) {
      for (int i = 0; i < filterTaps; i++) {
        coef[i] = newCoefs[i];
      }
    }
    //set coefficient at specified index
    void setCoefficient(int idx, float newCoef) {
      coef[idx] = newCoef;
    }

    float process(float in) {
      float out = 0;                        // out is the return variable. It is set to 0 every time we call the filter!

      values[k] = in;                        // store the input of the routine (contents of the 'in' variable) in the array at the current pointer position

      for (int i = 0; i < filterTaps; i++) {                          // we step through the array
        out += coef[i] * values[(i + k) % filterTaps];      // ... and add and multiply each value to accumulate the output
        //  (i + k) % filterTaps creates a cyclic way of getting through the array
      }

      out /= gain;                        // We need to scale the output (unless the coefficients provide unity gain in the passband)

      k = (k + 1) % filterTaps;          // k is increased and wraps around the filterTaps, so next time we will overwrite the oldest saved sample in the array

      return out;                              // we send the output value back to whoever called the routine
    }

  private:
    float values[filterTaps];

    float coef[filterTaps];

    //declare gain coefficient to scale the output back to normal
    float gain; // set to 1 and input unity to see what this needs to be

    int k; // k stores the index of the current array read to create a circular memory through the array
};


class RGBControl {
  public:
    RGBControl() {
      ledPin = 0;
      micVal = 0;
      DCVal = -1;
      brightness = 0;
      LastBrightness = 0;
      micMax = 0;
      micMin = 255; //1023;
      counter = 0;
      SumDCVal = 0;
      UpFade = 0;
      DownFade = 0;
    }
    RGBControl operator=( RGBControl C2)
    {
      ledPin = C2.ledPin;
      brightness = C2.brightness;
      LastBrightness = C2.LastBrightness;
      micVal = C2.micVal;
      DCVal = C2.DCVal;
      micMax = C2.micMax;
      micMin = C2.micMin;
      counter = C2.counter;
      SumDCVal = C2.SumDCVal;
      UpFade = C2.UpFade;
      DownFade = C2.DownFade;
    }
    void SetZero() {
      brightness = 0;
    }
    void SetLedPin(int Num) {
      ledPin = Num;
    }
    void SetDCVal(int Num) {
      DCVal = Num;
    }
    void SetFade(int Up, int Down) {
      UpFade = Up;
      DownFade = Down;
    }
    void SetMinMax(int Min, int Max) {
      micMin = Min;
      micMax = Max;
    }
    int micVal2Brightness()
    {
      micVal = abs(micVal - DCVal);
      //micVal = min(micVal+DCALLVal,MAXVAL);
      micMin = min(micVal, micMin);
      micMax = max(micVal, micMax);
      brightness = int((double(micVal - micMin) / double(micMax - micMin)) * MAXVALREL) - 1;
      brightness = min(max(brightness - 20, 0), MAXVALREL - 1);
      return brightness;
    }
    void WriteBright(int bbrightness)
    {
      int counter = 0;
      while ( abs(LastBrightness - bbrightness) > FADEGAP / 2)
      {
        counter++;
        if (LastBrightness > bbrightness)
        {
          LastBrightness = LastBrightness - FADEGAP;
          if (counter >= DownFade) break;
        }
        else
        {
          LastBrightness = LastBrightness + FADEGAP;
          if (counter >= UpFade) break;
        }
        analogWrite(ledPin, LastBrightness);
      }
    }
    int CalcDC() {
      if (DCVal == -1)
      {
        if (counter >= WAKEUPSAMP) SumDCVal = SumDCVal + micVal;
        counter++;
        if (counter == AVGNUM + WAKEUPSAMP )
        {
          DCVal = SumDCVal / AVGNUM;
          return 1;
        }
        return 0;
      }
      else
      {
        return 1;
      }
    }
    int ledPin;
    int brightness;
    int LastBrightness;
    int micVal;
    int micMax;
    int micMin;
  private:
    int DCVal;
    int counter;
    int SumDCVal;
    int UpFade;
    int DownFade;
};

long TimeTMP1 = 0;
long TimeTMP2 = 0;
long timeTMPDT = 0;

long timeChange = 0;

long Time1 = 0;
long Time2 = 0;
long timeDT = 0;
int mic_NoFilt[BUFFER] = {0};
int micPin = 0;
volatile int AutoPin = 12;
volatile bool AutoMode = 0;
RGBControl RCtrl, GCtrl, BCtrl;
FIR<FILTERTAPS> firR, firG, firB;


void setup()  {
  RCtrl.SetFade(200, 50);
  GCtrl.SetFade(200, 50);
  BCtrl.SetFade(100, 50);


  //RCtrl.SetMinMax(0, REDsensitivity);
  GCtrl.SetMinMax(0, GREENsensitivity);
  BCtrl.SetMinMax(0, BLUEsensitivity);

  RCtrl.SetLedPin(REDpin);
  GCtrl.SetLedPin(GREENpin);
  BCtrl.SetLedPin(BLUEpin);

  pinMode(RCtrl.ledPin, OUTPUT);
  pinMode(GCtrl.ledPin, OUTPUT);
  pinMode(BCtrl.ledPin, OUTPUT);


  pinMode(A0, INPUT);


  float Rcoef[FILTERTAPS] = {
    0.0295011,
    0.0302565,
    0.0309711,
    0.0316420,
    0.0322666,
    0.0328426,
    0.0333677,
    0.0338398,
    0.0342571,
    0.0346178,
    0.0349207,
    0.0351645,
    0.0353481,
    0.0354710,
    0.0355325,
    0.0355325,
    0.0354710,
    0.0353481,
    0.0351645,
    0.0349207,
    0.0346178,
    0.0342571,
    0.0338398,
    0.0333677,
    0.0328426,
    0.0322666,
    0.0316420,
    0.0309711,
    0.0302565,
    0.0295011

  };

  float Gcoef[FILTERTAPS] = {
    0.0185890,
    0.0226042,
    0.0265141,
    0.0302829,
    0.0338761,
    0.0372605,
    0.0404046,
    0.0432790,
    0.0458566,
    0.0481132,
    0.0500275,
    0.0515813,
    0.0527597,
    0.0535516,
    0.0539495,
    0.0539495,
    0.0535516,
    0.0527597,
    0.0515813,
    0.0500275,
    0.0481132,
    0.0458566,
    0.0432790,
    0.0404046,
    0.0372605,
    0.0338761,
    0.0302829,
    0.0265141,
    0.0226042,
    0.0185890
  };


  float Bcoef[FILTERTAPS]  = {
    -0.0053070,
    -0.0076642,
    -0.0103589,
    -0.0134558,
    -0.0170447,
    -0.0212541,
    -0.0262743,
    -0.0323993,
    -0.0401064,
    -0.0502242,
    -0.0643223,
    -0.0857717,
    -0.1233472,
    -0.2092418,
    -0.6332501,
    0.6332501,
    0.2092418,
    0.1233472,
    0.0857717,
    0.0643223,
    0.0502242,
    0.0401064,
    0.0323993,
    0.0262743,
    0.0212541,
    0.0170447,
    0.0134558,
    0.0103589,
    0.0076642,
    0.0053070
  };




  firR.setCoefficients(Rcoef);
  firR.setGain(REDgain);
  firG.setCoefficients(Gcoef);
  firG.setGain(GREENgain);
  firB.setCoefficients(Bcoef);
  firB.setGain(BLUEgain);
  //     BCtrl.SetDCVal(0);


  //set up continuous sampling of analog pin 0 (you don't need to understand this part, just know how to use it in the loop())

  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;

  ADMUX |= (1 << REFS0); //set reference voltage
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only

  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements

  //if you want to add other things to setup(), do it here

}

void loop()
{


  TimeTMP2 = micros();
  timeTMPDT = TimeTMP2 - TimeTMP1;
  TimeTMP1 = micros();

  int Buff = 0;

  while (Buff < BUFFER / 2)
  {
    Time2 = micros();
    if (  Time2 - Time1 >= 56)
    {
      Time2 = micros();
      mic_NoFilt[Buff] = ADCH;//analogRead(micPin);
      timeDT = Time2 - Time1;
      Time1 = micros();
      Buff++;
    }
  }

  for ( Buff = 0; Buff < BUFFER / 2; Buff++)
  {
    RCtrl.micVal = int(firR.process(float(mic_NoFilt[Buff])));
    GCtrl.micVal = int(firG.process(float(mic_NoFilt[Buff])));
    BCtrl.micVal = int(firB.process(float(mic_NoFilt[Buff])));
  }


  if ( RCtrl.CalcDC() && GCtrl.CalcDC() && BCtrl.CalcDC())
  {
    int rb = RCtrl.micVal2Brightness();
    int gb = GCtrl.micVal2Brightness();
    int bb = BCtrl.micVal2Brightness();

    if ((rb > gb) && (rb > bb))
    {
      rb = 255;
      gb = 0;
      bb = 0;
    }
    else if ((gb > rb) && (gb > bb))
    {
      rb = 0;
      gb = 255;
      bb = 0;
    }
    else if ((bb > rb) && (bb > gb))
    {
      rb = 0;
      gb = 0;
      bb = 255;
    }
    RCtrl.WriteBright(rb);
    BCtrl.WriteBright(gb);
    GCtrl.WriteBright(bb);
  }

}





