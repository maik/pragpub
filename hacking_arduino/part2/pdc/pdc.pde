const unsigned int BAUD_RATE      = 57600;
const unsigned int SENSOR_PIN     = A0;
const unsigned int SPEAKER_PIN    = 13;
const unsigned int TONE_FREQUENCY = 1000;
const unsigned int TONE_DURATION  = 200;
const unsigned int MAX_SAMPLES    = 16;
const float        GAP            = 3.0;
const float        SUPPLY_VOLTAGE = 4.7;
const float        VOLTS_PER_CM   = 27.0;
const float        MIN_DISTANCE   = 8.0;
const float        MAX_DISTANCE   = 80.0;

unsigned int samples[MAX_SAMPLES];
unsigned int sample_pos = 0;

void init_samples(void) {
  for (unsigned int i = 0; i < MAX_SAMPLES; i++)
    samples[i] = analogRead(SENSOR_PIN);
}

unsigned int get_average_voltage(void) {
  samples[sample_pos] = analogRead(SENSOR_PIN);
  sample_pos = (sample_pos + 1) % MAX_SAMPLES;
  unsigned long sum = 0;
  for (unsigned int i = 0; i < MAX_SAMPLES; i++)
    sum += samples[i];
  return round(sum / MAX_SAMPLES);
}

float get_distance(void) {
  const float voltage =
    get_average_voltage() * SUPPLY_VOLTAGE / 1024.0;
  return VOLTS_PER_CM / voltage - GAP;
}

void setup(void) {
  init_samples();
  Serial.begin(BAUD_RATE);
}

void loop(void) {
  const float distance = get_distance();
  if (distance <= MIN_DISTANCE) {
    Serial.println("Too close!");
    tone(SPEAKER_PIN, TONE_FREQUENCY, TONE_DURATION);
  } else if (distance >= MAX_DISTANCE) {
    Serial.println("Ok.");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(50);
}

