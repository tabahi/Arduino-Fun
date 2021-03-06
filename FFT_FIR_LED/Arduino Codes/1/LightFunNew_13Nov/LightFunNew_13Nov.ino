#define REDpin 5
#define GREENpin 9
#define BLUEpin 3

#define REDgain 0.8
#define GREENgain 0.24
#define BLUEgain 1.0864




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
    void micVal2Brightness() {
      micVal = abs(micVal - DCVal);
      //micVal = min(micVal+DCALLVal,MAXVAL);
      micMin = min(micVal, micMin);
      micMax = max(micVal, micMax);
      brightness = int((double(micVal - micMin) / double(micMax - micMin)) * MAXVALREL) - 1;
      brightness = min(max(brightness - 20, 0), MAXVALREL - 1);
    }
    void WriteBright()
    {
      int counter = 0;
      while ( abs(LastBrightness - brightness) > FADEGAP / 2)
      {
        counter++;
        if (LastBrightness > brightness) {
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
  RCtrl.SetFade(20, 50);
  GCtrl.SetFade(20, 10);
  BCtrl.SetFade(50, 70);


  //RCtrl.SetMinMax(0, 2);
  GCtrl.SetMinMax(0, 5);
  BCtrl.SetMinMax(0, 10);

  RCtrl.SetLedPin(REDpin);
  GCtrl.SetLedPin(GREENpin);
  BCtrl.SetLedPin(BLUEpin);

  pinMode(RCtrl.ledPin, OUTPUT);
  pinMode(GCtrl.ledPin, OUTPUT);
  pinMode(BCtrl.ledPin, OUTPUT);


  pinMode(A0, INPUT);


  float Rcoef[FILTERTAPS] = {
    0.0275412,
    0.0286572,
    0.0297220,
    0.0307296,
    0.0316745,
    0.0325516,
    0.0333558,
    0.0340827,
    0.0347281,
    0.0352883,
    0.0357601,
    0.0361409,
    0.0364285,
    0.0366211,
    0.0367177,
    0.0367177,
    0.0366211,
    0.0364285,
    0.0361409,
    0.0357601,
    0.0352883,
    0.0347281,
    0.0340827,
    0.0333558,
    0.0325516,
    0.0316745,
    0.0307296,
    0.0297220,
    0.0286572,
    0.0275412
  };

  float Gcoef[FILTERTAPS] = {
    -0.0320455,
    -0.0221913,
    -0.0117018,
    -0.0007897,
    0.0103194,
    0.0213924,
    0.0321936,
    0.0424910,
    0.0520612,
    0.0606954,
    0.0682038,
    0.0744209,
    0.0792088,
    0.0824612,
    0.0841057,
    0.0841057,
    0.0824612,
    0.0792088,
    0.0744209,
    0.0682038,
    0.0606954,
    0.0520612,
    0.0424910,
    0.0321936,
    0.0213924,
    0.0103194,
    -0.0007897,
    -0.0117018,
    -0.0221913,
    -0.0320455
  };


  float Bcoef[FILTERTAPS]  = {
    0.0201474,
    0.0201097,
    0.0191832,
    0.0172335,
    0.0141161,
    0.0096640,
    0.0036662,
    -0.0041744,
    -0.0143231,
    -0.0275842,
    -0.0455030,
    -0.0714521,
    -0.1143247,
    -0.2069880,
    -0.6461537,
    0.6461537,
    0.2069880,
    0.1143247,
    0.0714521,
    0.0455030,
    0.0275842,
    0.0143231,
    0.0041744,
    -0.0036662,
    -0.0096640,
    -0.0141161,
    -0.0172335,
    -0.0191832,
    -0.0201097,
    -0.0201474
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
    RCtrl.micVal2Brightness();
    GCtrl.micVal2Brightness();
    BCtrl.micVal2Brightness();

    RCtrl.WriteBright();
    BCtrl.WriteBright();
    GCtrl.WriteBright();
  }

}





