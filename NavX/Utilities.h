//Combine the two bytes to make one number we can use
unsigned int twoBytesToUnsignedInt(byte high, byte low) {
  unsigned int intHigh = high;
  unsigned int intLow = low;
  return (intHigh << 8) | intLow;
}

//Combine the two bytes to make one number we can use
signed int twoBytesToSignedInt(byte high, byte low) {
  signed int intHigh = high;
  signed int intLow = low;
  return (intHigh << 8) | intLow;
}
