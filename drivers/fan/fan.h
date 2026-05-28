#ifndef FAN_H
#define FAN_H

void Fan_Init(void);
void Fan_Update(float current_temp, float threshold);

#endif