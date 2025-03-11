int bright360to255(int val)
{
  return static_cast<int>((val / 360.0) * 255);
}

void ledsShow(int h, int b) {
  for (int i = 0; i <= numled - 1; i++) // default color for aquarium
  {
    ledHues[i] = bright360to255(h); // Enregistrer la teinte
    leds[i].setHSV(ledHues[i], 255, b);
  }
  FastLED.show();
}
