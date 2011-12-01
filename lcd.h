#ifndef TIM_H
#define TIM_H

void LCD_Init(void);
void static wait(unsigned short delay);
void LCD_Instruction(unsigned short instru);
void LCD_Send(unsigned short content);
void LCD_Printc(unsigned char character);
void LCD_Printmsg(char *pt);

#endif