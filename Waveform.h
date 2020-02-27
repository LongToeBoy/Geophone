#ifndef Waveform_h
#define Waveform_h

#include <Arduino.h>

class Waveform {
public:
	Waveform(int);
    Waveform();

	void add(float);
    float avg(void);
    void clear(void);
    float ppv(float);
    void Cal(float);
    void resize(int);
    int16_t peaky=0;
    int16_t peakmestime=millis();
    double hz(uint16_t);
    int16_t peak(uint16_t);
    float dwpeak=1000;
    float clipping(void);
    int16_t cor(int);
    float freq=0;
        int16_t *arrayy;
    void smooth(uint16_t,int16_t);
    float zerohz(long);
    float threshz(long,int16_t,int16_t,uint16_t);
    //int16_t
private:
    float aveg=0;
    float Waveform::mapfloat(float, float , float , float , float );
    bool sign=false;
    uint16_t cal_rates[100];
    float avrcal(void);
    int16_t peakcounter=0;
    unsigned long delta[2]={0,0};
    bool ispeak(uint16_t,uint16_t,uint16_t);
    int coun=0;
    int16_t uppeaktime=0,downpeaktime=0;
    int sizeey=0;
    int buf=100;
    double zerohz(void);
    double hzfunc(int16_t*, uint16_t,int16_t);
    
};


#endif
