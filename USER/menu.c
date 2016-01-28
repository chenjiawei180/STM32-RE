 /******************************************************************************
  * @file       USER/menu.c
  * @author  cjw
  * @date     2016.1.28
  * @brief     This file contains the source of the menu.
  ******************************************************************************/
#include "menu.h"
#include "tm1629.h"
#include "usart2.h"

#if defined MENU_GLOBAL

/**
  * @brief  This is Menu index.
  * @param  None
  * @retval None
  */

const struct Menu_def MenuProc[]=
{
    {STANDBY_MENU,  STANDBY_MENU, STANDBY_MENU, STANDBY_MENU,STANDBY_MENU,Menu_Standby},  
    {ONE_MENU_F0,  ONE_MENU_F0, ONE_MENU_FE, ONE_MENU_F1,STANDBY_MENU,Menu_F0},  
    {ONE_MENU_F1,  ONE_MENU_F1, ONE_MENU_F0, ONE_MENU_F2,STANDBY_MENU,Menu_F1},  
    {ONE_MENU_F2,  ONE_MENU_F2, ONE_MENU_F1, ONE_MENU_F3,STANDBY_MENU,Menu_F2},  
    {ONE_MENU_F3,  ONE_MENU_F3, ONE_MENU_F2, ONE_MENU_F4,STANDBY_MENU,Menu_F3},  
    {ONE_MENU_F4,  ONE_MENU_F4, ONE_MENU_F3, ONE_MENU_F5,STANDBY_MENU,Menu_F4}, 
    {ONE_MENU_F5,  ONE_MENU_F5, ONE_MENU_F4, ONE_MENU_F6,STANDBY_MENU,Menu_F5},  
    {ONE_MENU_F6,  ONE_MENU_F6, ONE_MENU_F5, ONE_MENU_F7,STANDBY_MENU,Menu_F6},  
    {ONE_MENU_F7,  ONE_MENU_F7, ONE_MENU_F6, ONE_MENU_F8,STANDBY_MENU,Menu_F7},  
    {ONE_MENU_F8,  ONE_MENU_F8, ONE_MENU_F7, ONE_MENU_F9,STANDBY_MENU,Menu_F8},  
    {ONE_MENU_F9,  ONE_MENU_F9, ONE_MENU_F8, ONE_MENU_FA,STANDBY_MENU,Menu_F9},  
    {ONE_MENU_FA,  ONE_MENU_FA, ONE_MENU_F9, ONE_MENU_FB,STANDBY_MENU,Menu_FA},  
    {ONE_MENU_FB,  ONE_MENU_FB, ONE_MENU_FA,ONE_MENU_FC,STANDBY_MENU,Menu_FB},  
    {ONE_MENU_FC,  ONE_MENU_FC, ONE_MENU_FB, ONE_MENU_FD,STANDBY_MENU,Menu_FC},  
    {ONE_MENU_FD,  ONE_MENU_FD, ONE_MENU_FC, ONE_MENU_FE,STANDBY_MENU,Menu_FD},  
    {ONE_MENU_FE,  ONE_MENU_FE, ONE_MENU_FD, ONE_MENU_F0,STANDBY_MENU,Menu_FE},
    
};

/**
  * @brief  This function is Show TM1629 of Standby menu.
  * @param  None
  * @retval None
  */

void Menu_Standby(void)
{
    static u8 index=0;
    Tm1629_Clear();
    Tm1629_Display_Ram[0][index&0x03]=0x40; /* - */
    Tm1629_Display();
    index++;
}

/**
  * @brief  This function is Show TM1629 of F0 menu.
  * @param  None
  * @retval None
  */

void Menu_F0(void)
{
    Tm1629_Show_Fx(0x00);
}

/**
  * @brief  This function is Show TM1629 of F1 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F1(void)
{
    Tm1629_Show_Fx(0x01);
}

/**
  * @brief  This function is Show TM1629 of F2 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F2(void)
{
    Tm1629_Show_Fx(0x02);
}

/**
  * @brief  This function is Show TM1629 of F3 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F3(void)
{
    Tm1629_Show_Fx(0x03);
}

/**
  * @brief  This function is Show TM1629 of F4 menu.
  * @param  None
  * @retval None
  */

void Menu_F4(void)
{
    Tm1629_Show_Fx(0x04);
}

/**
  * @brief  This function is Show TM1629 of F5 menu.
  * @param  None
  * @retval None
  */

void Menu_F5(void)
{
    Tm1629_Show_Fx(0x05);
}

/**
  * @brief  This function is Show TM1629 of F6 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F6(void)
{
    Tm1629_Show_Fx(0x06);
}

/**
  * @brief  This function is Show TM1629 of F7 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F7(void)
{
    Tm1629_Show_Fx(0x07);
}

/**
  * @brief  This function is Show TM1629 of F8 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F8(void)
{
    Tm1629_Show_Fx(0x08);
}

/**
  * @brief  This function is Show TM1629 of F9 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F9(void)
{
    Tm1629_Show_Fx(0x09);
}

/**
  * @brief  This function is Show TM1629 of FA menu.
  * @param  None
  * @retval None
  */
  
void Menu_FA(void)
{
    Tm1629_Show_Fx(0x0A);
}

/**
  * @brief  This function is Show TM1629 of FB menu.
  * @param  None
  * @retval None
  */
  
void Menu_FB(void)
{
    Tm1629_Show_Fx(0x0B);
}

/**
  * @brief  This function is Show TM1629 of FC menu.
  * @param  None
  * @retval None
  */
  
void Menu_FC(void)
{
    Tm1629_Show_Fx(0x0C);
}

/**
  * @brief  This function is Show TM1629 of FD menu.
  * @param  None
  * @retval None
  */
  
void Menu_FD(void)
{
    Tm1629_Show_Fx(0x0D);
}

/**
  * @brief  This function is Show TM1629 of FE menu.
  * @param  None
  * @retval None
  */
  
void Menu_FE(void)
{
    Tm1629_Show_Fx(0x0E);
}


#endif /* MENU_GLOBAL */

