// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficient to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)

// Buffer parameters for median filter
#define N 3               // Number of recent samples to keep for median calculation

// global variables
unsigned long last_sampling_time;    // unit: msec
float dist_prev = _DIST_MAX;         // Distance last-measured
float dist_ema;                      // EMA distance
float samples[N];                    // Circular buffer to store recent N samples
int sample_idx = 0;                  // Current index in the buffer

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // initialize serial port
  Serial.begin(57600);

  // Initialize buffer with max distance
  for (int i = 0; i < N; i++) {
    samples[i] = _DIST_MAX;
  }
}

void loop() {
  float dist_raw;
  
  // wait until next sampling time.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);
  
  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
      dist_raw = dist_prev;
      digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (dist_raw < _DIST_MIN) {
      dist_raw = dist_prev;
      digitalWrite(PIN_LED, 1);       // LED OFF
  } else {    // In desired Range
      digitalWrite(PIN_LED, 0);       // LED ON     
      dist_prev = dist_raw;
  }

  // Update buffer with new sample
  samples[sample_idx] = dist_raw;
  sample_idx = (sample_idx + 1) % N;  // Circular buffer

  // Get the median value from the buffer
  float dist_median = medianFilter(samples, N);

  // Update EMA filter
  dist_ema = _EMA_ALPHA * dist_raw + (1 - _EMA_ALPHA) * dist_ema;

  // Output the distance to the serial port
  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",median:");  Serial.print(dist_median);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  // Update last sampling time
  last_sampling_time += INTERVAL;
}

// Function to calculate the median value from the buffer
float medianFilter(float buffer[], int size) {
  float temp[size];
  
  // Copy buffer to temp array
  for (int i = 0; i < size; i++) {
    temp[i] = buffer[i];
  }

  // Sort the temp array
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (temp[i] > temp[j]) {
        float tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
      }
    }
  }

  // Return median value
  if (size % 2 == 0)
    return (temp[size/2 - 1] + temp[size/2]) / 2.0;
  else
    return temp[size/2];
}

// Get a distance reading from USS. Return value is in millimeters.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
