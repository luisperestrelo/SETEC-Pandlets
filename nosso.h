#ifndef __NOSSO_H__
#define __NOSSO_H__


unsigned long getTimeStamp();
unsigned long getflagBAT();
unsigned long getflagACC();
unsigned long getGPS();

void setTimeStamp(unsigned long x);
void setflagBAT(unsigned long x);
void setflagACC(unsigned long x);
void setGPS(unsigned long x);

void increTimeStamp();



#endif /* __NOSSO_H__ */
