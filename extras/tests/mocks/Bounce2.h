class Bounce
{
public:
  void attach(int pin) {
    this->pin = pin;
  }
  void attach(int pin, int /*mode*/) {
    this->pin = pin;
  }
  bool update() { return false; }
  bool read() { return true; }
  bool rose() { return false; }
  bool fell() { return false; }
  
protected:
  unsigned long previous_millis = 0;
  int pin = 0;
};
