#include "Waveform.h"
#include "Arduino.h"
#include <math.h>
#define count(x) (sizeof(x) / sizeof(x[0]))

Waveform::Waveform(int sizee){
  sizeey=sizee;
    int16_t *z=new int16_t[sizee];
    arrayy=z;
    memset(arrayy,0,sizeey);
    memset(cal_rates,0,100);
    //index=0;
}

Waveform::Waveform(){
    int16_t *z=new int16_t[10];
    arrayy=z;
    memset(arrayy,0,sizeey);
    memset(cal_rates,0,100);

    //index=0;
}

float Waveform::ppv(float hertz){
  if(cal_rates[int(hertz)]>0){
  return(mapfloat(peaky,0,cal_rates[int(hertz)],0,1));}
  else if(avrcal()>0){
    return(peaky/avrcal());
  }
  else{
    return(peaky);
  }
}
float Waveform::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void Waveform::Cal(float hertz){
  if(cal_rates[int(hertz)]!=0){cal_rates[int(hertz)]=(cal_rates[int(hertz)]+abs(peaky))/2;}
  else{
    cal_rates[int(hertz)]=abs(peaky);
  }
}
float Waveform::clipping(void){
  int counter=0;
  for(int i=0;i<sizeey-1;i++){
    if (arrayy[i]==arrayy[i+1]){
      counter++;
    }
  }
  return(counter*100/sizeey);
}
float Waveform::avrcal(void){
  uint16_t count=0;
  uint16_t avry=0;
  for(int i=0;i<100;i++){
    if(cal_rates[i]>0){
      avry+=cal_rates[i];
      count++;
    }
  }
  if(count>0){
    return(avry/count);
  }
  else{
    return(0);
  }
}

void Waveform::add(float reading){
for(int i=0;i<sizeey-1;i++){
    arrayy[i]=arrayy[i+1];
}
arrayy[sizeey-1]=reading;
peakcounter++;
if(peakcounter>500){peaky=reading-aveg;peakcounter=0;}
else if(abs(reading-aveg)>abs(peaky))peaky=reading-aveg;

}

int16_t Waveform::peak(uint16_t region){
  uint16_t peakindx=0;
  uint16_t abspeak=0;
  for(int i=sizeey-region;i<sizeey;i++){
    if(abs(arrayy[i])>abspeak)peakindx=i;
  }
  return(arrayy[peakindx]);
}
int16_t Waveform::cor(int del){
  if (del<=sizeey&&del>0){
    return(arrayy[sizeey-1]*arrayy[sizeey-del]/1000);
  }
  else{
  return(sizeey);
}
}
void Waveform::smooth(uint16_t factor,int16_t dir){
  switch(dir){
    case 1:
  for(int i=0;i<sizeey-factor-1;i++){
      int16_t sash=0;
      for(int o=0;0<factor;o++){
        sash+=arrayy[i+o];
        }
      arrayy[i]=sash/factor;
  }
  break;
  case -1:
  for(int i=factor;i<sizeey;i++){
      int16_t sash=0;
      for(int o=0;0<factor;o++){
        sash+=arrayy[i-o];
        }
      arrayy[i]=sash/factor;
  }
  break;
}
}
double Waveform::hz(uint16_t a){
  delta[0]=delta[1];
  delta[1]=a;
  int h=1;
  int iter=40;
  int x=sizeey-1;
  double average=0;
  for(int i=0;i<iter;i++){
    double calcr=hzfunc(arrayy,x,delta[1]-delta[0]);
    double calcm=hzfunc(arrayy,x-1,delta[1]-delta[0]);
    double calcl=hzfunc(arrayy,x-2,delta[1]-delta[0]);
    if(!(isnan(calcm)||isinf(calcm))){
      if((calcm+calcl+calcr)/3<1){
        average+=calcr;
      }
      if(calcm>calcl&&calcm>calcr){
  average+=calcm;}
    }
  else{
    iter--;
  }
  x--;
  }
  if(average==average&&iter>0&&average>0){
  return(average/iter);}
  else{
    return(zerohz());
  }
}
double Waveform::zerohz(void){
  unsigned long diff=delta[1]-delta[0];
  int average=0;
  int indx=0;
  int firstindx=0;
  bool isfirst=false;
  int i=1;
  for(i=1;i<sizeey-1;i++){
    if(ispeak(arrayy[i-1],arrayy[i],arrayy[i+1])){
      if(!isfirst)firstindx=i;isfirst!=isfirst;
      indx=i;
      average++;
      if(abs(arrayy[i])<avg())break;
    }
  }
  if(average>1&&((indx-firstindx)*diff)>0){
    return(average*1000000/(2*(indx-firstindx)*diff));
  }
  else{
    return(100);
  }
}
float Waveform::zerohz(long cycletime){
  int average=0;
  int indx=0;
  int firstindx=0;
  bool isfirst=false;
  int i=1;
  for(i=1;i<sizeey-1;i++){
    if(ispeak(arrayy[i-1],arrayy[i],arrayy[i+1])){
      if(!isfirst)firstindx=i;isfirst!=isfirst;
      indx=i;
      average++;
      if(abs(arrayy[i])<avg())break;
    }
  }
  if(average>1&&((indx-firstindx)*cycletime)>0){
    return(average*1000000/(2*(indx-firstindx)*cycletime));
  }
  else{
    return(100);
  }
}
float Waveform::threshz(long cycletime, int16_t upthres, int16_t downthres,uint16_t sample){
  int sign=0;
  int exccount=0;
  //upthres-=aveg;
  //downthres-=aveg;
  for(int i=sizeey-sample;i<sizeey;i++){
    if(sign==0){
    if(arrayy[i]>upthres){
    sign=1;
    exccount++;
    }
    if(arrayy[i]<downthres){
    sign=-1;
    exccount++;
    }
    }
    if(sign<0){
      if(arrayy[i]<downthres){
        exccount++;
        sign*=(-1);
      }
    }
    if(sign>0){
      if(arrayy[i]>upthres){
        exccount++;
        sign*=(-1);
      }
    }
  }
  return(exccount*1000000/(2*sample*cycletime));
}
double Waveform::hzfunc(int16_t *b,uint16_t index, int16_t delt){
  int h=1;
  double calc=1000000*sqrt(((arrayy[index-2*h]-2*arrayy[index-h]+arrayy[index])*1000/(pow((delt),2)))/(-1*arrayy[index]));
  return(calc);
}
bool Waveform::ispeak(uint16_t head, uint16_t mid, uint16_t bot){
  mid=abs(mid);
  head=abs(head);
  bot=abs(bot);
  if(head<mid&&mid>bot){
    return (true);
  }
  else{
    return(false);
  }
}
float Waveform::avg(){
int16_t a=0;
for(int i=0;i<sizeey;i++){
    a+=arrayy[i];
}
aveg=a/sizeey;
return (a/sizeey);
}



void Waveform::clear(){
memset(arrayy,0,sizeey);
}


void Waveform::resize(int sizee){
  sizeey=sizee;
int16_t *z=new int16_t[sizeey];
arrayy=z;
memset(arrayy,0,sizeey);
}
