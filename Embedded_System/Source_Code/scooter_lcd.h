/* 
 * File:   scooter_lcd.h
 * Author: Matt
 *
 * Created on 22 July 2013, 3:46 PM
 */

#ifndef SCOOTER_LCD_H
#define	SCOOTER_LCD_H

#define LCD_LINE_1 0
#define LCD_LINE_2 1

#define DATA 1
#define INSTRUCTION 0

#define LCD_OFF 0
#define LCD_ON 1

#define READ 1
#define WRITE 0



void lcd_init(void);
void lcd_command_8bit(char rs,char rw, char data);
void lcd_command_4bit(char rs,char rw, char data);
void lcd_wait_busy_flag(void);
void lcd_print_int(unsigned int Num , unsigned int max_digits);
void lcd_print_str(rom char *Message);
void lcd_return_home(void);
void lcd_go_to_line(unsigned short lineNum);
void lcd_deinit(void);
void lcd_reinit(void);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCOOTER_LCD_H */

