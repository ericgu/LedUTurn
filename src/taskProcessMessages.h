class TaskProcessMessages : public Task
{
public:
    TaskProcessMessages(uint32_t timeInterval, LEDStrip* pStrip, FaderGroup* pFaderGroup) : // pass any custom arguments you need
        Task(timeInterval),
        _pFaderGroup(pFaderGroup),
        _pStrip(pStrip)
    { 
      _value = 0;
      _valueIncrement = 0.05;
    };

    void Init()
    {
      _maximumValue = _pFaderGroup->GetTotalDistance();
    }

private:
    FaderGroup* _pFaderGroup;
    LEDStrip* _pStrip;

    float _value;
    float _valueIncrement;
    float _maximumValue;

    virtual bool OnStart() // optional
    {
        return true;
    }

    virtual void OnStop() // optional
    {

    }

    void SetPixelsTopBottom(int pixelNumber, RgbColorReal color)
    {
      RgbColor rgbColor = color.GetRgbColor();
      _pStrip->SetPixelColor(pixelNumber, rgbColor);
      _pStrip->SetPixelColor(50 - pixelNumber, rgbColor);
    }

    virtual void OnUpdate(uint32_t deltaTime)
    {
      const float normalSpacing = 0.95F;
      //const float headSpacing = 0.708F;
      const float headSpacing = 1.25F;
      const float arcMiddleDistance = 10.7F;
      const float innerArcSpacing = arcMiddleDistance / 5;
      const float outerArcSpacing = arcMiddleDistance / 13;

      //Serial.println("OnUpdate");
      //Serial.flush();


      _value = _value + _valueIncrement;
      if (_value >= _maximumValue)
      {
        _value = 0;
        Serial.print("0");
        Serial.flush();
      }

      RgbColorReal color;

      float current = _value;

      color = _pFaderGroup->GetCurrent(current);
      _pStrip->SetPixelColor(0, color.GetRgbColor());
      current += headSpacing;

      for (int pixelNumber = 1; pixelNumber < 5; pixelNumber++)
      {
        SetPixelsTopBottom(pixelNumber, _pFaderGroup->GetCurrent(current));
        current += headSpacing;
      }

      // back of arrow head...
      color = _pFaderGroup->GetCurrent(current);
      SetPixelsTopBottom(5, _pFaderGroup->GetCurrent(current));
      SetPixelsTopBottom(6, _pFaderGroup->GetCurrent(current));
      current += normalSpacing;

      // single pixel for straight section behind arrow...
      //SetPixelsTopBottom(7, _pFaderGroup->GetCurrent(current));
      //current += normalSpacing;

      // Now it gets complicated...
      // the inner arc is 5 pixels.
      // The outer arc is 13 pixels.
      // The nominal (midpoint) distance is 12, so that sets the spacing. 

      float inner = current;
      for (int pixelNumber = 7; pixelNumber < 13; pixelNumber++)
      {
        _pStrip->SetPixelColor(pixelNumber, _pFaderGroup->GetCurrent(inner).GetRgbColor());
        inner += innerArcSpacing;
      }

      float outer = current;
      for (int pixelNumber = 43; pixelNumber >=30; pixelNumber--)
      {
        _pStrip->SetPixelColor(pixelNumber, _pFaderGroup->GetCurrent(outer).GetRgbColor());
        outer += outerArcSpacing;
      }

      // adjust so the straight part is using that distance...
      inner -= innerArcSpacing;
      inner += normalSpacing;

      // straight parts
      for (int pixelNumber = 13; pixelNumber < 19; pixelNumber++)
      {
        _pStrip->SetPixelColor(pixelNumber, _pFaderGroup->GetCurrent(inner).GetRgbColor());
        _pStrip->SetPixelColor(42 - pixelNumber, _pFaderGroup->GetCurrent(inner).GetRgbColor());
        inner += normalSpacing;
      }

      color = _pFaderGroup->GetCurrent(inner);
      for (int pixelNumber = 19; pixelNumber < 24; pixelNumber++)
      {
        _pStrip->SetPixelColor(pixelNumber, color.GetRgbColor());
      }

      _pStrip->Show();
    }     
};
