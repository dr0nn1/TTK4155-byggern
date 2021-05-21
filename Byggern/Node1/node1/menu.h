/*
 * menu.h
 *
 * Created: 24.09.2020 12:23:03
 *  Author: jarlenes
 */ 


#ifndef MENU_H_
#define MENU_H_
void initMenu();
void menu_print(uint8_t arrow);
uint8_t menu_navigate();
void Scoreboard();
void Newgame();
void createName();
void menu_main();
void updateScoreboard(int newScore);
void selectPlayer(void);
void resetScore();
#endif /* MENU_H_ */