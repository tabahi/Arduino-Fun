// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

//              GND---(BATTERY 1)---(BATTERY 2)---(BATTERY 3)---(BATTERY 4)---(+)
//Voltages:                       V1            V2            V3            V4
//Relay pin:              10,11          7,8           5,6           3,4

#define ALL_RELAYS_NORMALY_OPEN true   //true = If relay is Open (OFF) when no voltage is supplied. false = When relay is closed (ON) when no voltage is supplied


#define battery_charge_max_duration 7500 //ms //interval of charging
#define battery_charge_pulses_delay 2500 //ms //interval between two charging pulses


//Pins connections:
#define battery_voltage_1_pin  A0
#define battery_voltage_2_pin  A1
#define battery_voltage_3_pin  A2
#define battery_voltage_4_pin  A3

#define relay_battery_1_positive_pin 10
#define relay_battery_1_negative_pin 11
#define relay_battery_2_positive_pin 7
#define relay_battery_2_negative_pin 8
#define relay_battery_3_positive_pin 5
#define relay_battery_3_negative_pin 6
#define relay_battery_4_positive_pin 3
#define relay_battery_4_negative_pin 4
#define relay_buck_converter_positive_pin 12
#define relay_buck_converter_negative_pin 13

#define V1_adjustment 0.00 //This is compensation for errors in sensed value:   (Voltage + Adjustment) = Compensated_voltage
#define V2_adjustment 0.30
#define V3_adjustment 0.30
#define V4_adjustment 0.30


const float charge_volt_upper_limit = 3.30; //all batteries will be maintained within this upper limit
const float temperature_heat_limit = 60.0;  //if temperature goes above this, batteries will not be charged
const float max_diff_btwn_batteries = 0.01;  //will keep the difference maximun to this value



const float Voltage_adjustment[] = {V1_adjustment, V2_adjustment, V3_adjustment, V4_adjustment};


const unsigned char battery_charge_relay_pins[4][2] = { {relay_battery_1_positive_pin, relay_battery_1_negative_pin},
  {relay_battery_2_positive_pin, relay_battery_2_negative_pin},
  {relay_battery_3_positive_pin, relay_battery_3_negative_pin},
  {relay_battery_4_positive_pin, relay_battery_4_negative_pin}
};
const unsigned char buck_converter_relay_pins[] = {relay_buck_converter_positive_pin, relay_buck_converter_negative_pin};





//Temperature sensor settings:
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// arrays to hold device addresses
DeviceAddress insideThermometer;//, Thermometer2, Thermometer3, Thermometer4;

float tmpx_last = 0.00;
float vx[] = {0.00, 0.00, 0.00, 0.00};
int min_volt_battery_no = -1;
unsigned long timer1, timer2, onTimer, offTimer = 0;
boolean charging_state = LOW;

const boolean ON = ALL_RELAYS_NORMALY_OPEN;
const boolean OFF = !ALL_RELAYS_NORMALY_OPEN;

void setup(void)
{
  pinMode(buck_converter_relay_pins[0], OUTPUT);
  pinMode(buck_converter_relay_pins[1], OUTPUT);
  for (int pk = 0; pk < 4; pk++)
  {
    pinMode(battery_charge_relay_pins[pk][0], OUTPUT);
    pinMode(battery_charge_relay_pins[pk][1], OUTPUT);
  }
  for (int pk = 0; pk < 4; pk++)
  {
    digitalWrite(battery_charge_relay_pins[pk][0], OFF);
    digitalWrite(battery_charge_relay_pins[pk][1], OFF);
  }
  digitalWrite(buck_converter_relay_pins[0], OFF);
  digitalWrite(buck_converter_relay_pins[1], OFF);
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Assign address manually. The addresses below will beed to be changed
  // to valid device addresses on your bus. Device address can be retrieved
  // by using either oneWire.search(deviceAddress) or individually via
  // sensors.getAddress(deviceAddress, index)
  // Note that you will need to use your specific address here
  //insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };

  // Method 1:
  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then
  // use those addresses and manually assign them (see above) once you know
  // the devices on your bus (and assuming they don't change).
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices,
  // or you have already retrieved all of them. It might be a good idea to
  // check the CRC to make sure you didn't get garbage. The order is
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  //oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!oneWire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Checking temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // print the device information
  printData(insideThermometer);

  charging_state = LOW;
}



void loop(void)
{
  int in1 = analogRead(battery_voltage_1_pin);
  int in2 = analogRead(battery_voltage_2_pin);
  int in3 = analogRead(battery_voltage_3_pin);
  int in4 = analogRead(battery_voltage_4_pin);
  float v1 = map_voltage(in1);
  float v2 = map_voltage(in2);
  float v3 = map_voltage(in3);
  float v4 = map_voltage(in4);

  float vx1 = v1 + Voltage_adjustment[0];
  float vx2 = (v2 * 2.00) - vx1 + Voltage_adjustment[1];
  float vx3 = (v3 * 3.00) - (v2 * 2.00) + Voltage_adjustment[2];
  float vx4 = (v4 * 4.00) - (v3 * 3.00) + Voltage_adjustment[3];


  if (((vx1 != vx[0]) || (vx2 != vx[1]) || (vx3 != vx[2]) || (vx4 != vx[3])) || ((millis() - timer1) > 2000))
  {
    vx[0] = vx1;
    vx[1] = vx2;
    vx[2] = vx3;
    vx[3] = vx4;
    Serial.print("V:\t" + String(vx[0], 2));
    Serial.print("\t" + String(vx[1], 2));
    Serial.print("\t" + String(vx[2], 2));
    Serial.println("\t" + String(vx[3], 2));
    timer1 = millis();
  }

  min_volt_battery_no = -1;
  float min_volts = charge_volt_upper_limit;
  for (int p = 3; p >= 0; p--)                 //detect the battery with lowest voltage level
  {
    if (vx[p] < min_volts)
    {
      min_volts = vx[p];
      min_volt_battery_no = p;
    }
  }
  unsigned long cmillis = millis();

  if (charging_state == LOW)
  {
    if ((min_volt_battery_no > -1) && (min_volt_battery_no < 4) && (tmpx_last < temperature_heat_limit) && ((cmillis - offTimer) > battery_charge_pulses_delay) )
    {
      //when a battery voltage is low, AND temperature in within limit, OFF delay has been passed
      Serial.print("Charging B:");
      Serial.println(min_volt_battery_no + 1);
      digitalWrite(buck_converter_relay_pins[0], OFF);
      digitalWrite(buck_converter_relay_pins[1], OFF);
      for (int pk = 0; pk < 4; pk++)            //switching off all relays
      {
        digitalWrite(battery_charge_relay_pins[pk][0], OFF);
        digitalWrite(battery_charge_relay_pins[pk][1], OFF);
      }
      digitalWrite(battery_charge_relay_pins[min_volt_battery_no][0], ON);
      digitalWrite(battery_charge_relay_pins[min_volt_battery_no][1], ON);
      digitalWrite(buck_converter_relay_pins[0], ON);
      digitalWrite(buck_converter_relay_pins[1], ON);
      onTimer = cmillis;
      charging_state = HIGH;
    }
  }
  else if (charging_state == HIGH)
  {
    if ((cmillis - onTimer) > battery_charge_max_duration)
    {
      digitalWrite(buck_converter_relay_pins[0], OFF);
      digitalWrite(buck_converter_relay_pins[1], OFF);
      for (int pk = 0; pk < 4; pk++)            //switching off all relays
      {
        digitalWrite(battery_charge_relay_pins[pk][0], OFF);
        digitalWrite(battery_charge_relay_pins[pk][1], OFF);
      }
      charging_state = LOW;
      offTimer = cmillis;
    }
  }





  float tempC = sensors.getTempC(insideThermometer);
  if ((tempC != tmpx_last) || ((millis() - timer2) > 20000))
  {
    tmpx_last = tempC;
    Serial.print("T: ");
    if (tmpx_last > temperature_heat_limit)
    {
      Serial.println(F("Overheat"));
    }
    Serial.println(tmpx_last);
    timer2 = millis();
  }

  delay(10);

}


float map_voltage(int value)
{
  float voltage = 5.00F;
  voltage = ((((float)value) / 1023.00) * 5.00);
  return voltage;
}



// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

