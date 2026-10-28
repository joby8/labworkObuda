/*
 * test.c
 *
 *  Created on: Nov 17, 2023
 *      Author: BD
 */

#include "test.h"

	/*
	 * Menu 1: LED Switch off [OFF]
	 * Menu 2: Connecting, 100ms LED blinking [Connecting]
	 * Menu 3: Life signal, 500ms LED blinking [Normal]
	 * Menu 4: Low battery, LED PWM 5% duty cycle [5%]
	 * Menu 5: Medium battery, LED PWM 50% duty cycle [50%]
	 * Menu 6: Charged, LED PWM 98% duty cycle [98%]
	 * Menu 7: DANGER, reduce and increase LED brightness continuously [DANGER!!!]
	 * Menu 8: LED Switch on [ON]
	 * Menu 9: Random number generation between 10-30 [RNG]
	 */

RNG_HandleTypeDef hrng;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim12;

UART_HandleTypeDef huart3;

const uint8_t menu[9][25]= {	"Menu 1: [OFF]\n\r",
								"Menu 2: [Normal]\n\r",
								"Menu 3: [10%]\n\r",
								"Menu 4: [40%]\n\r",
								"Menu 5: [90%]\n\r",
								"Menu 6: [DANGER!!!]\n\r",
								"Menu 7: [ON]\n\r",
								"Menu 8: [RNG]\n\r"};

const uint8_t enter[]="\n\r";

void f_status_test(uint8_t connecting, uint8_t normal, uint8_t danger, uint8_t on)
{
	/*TEST VARIABLES*/
	uint8_t on_s[2]={0};
	uint8_t connecting_s[2]={0};
	uint8_t normal_s[2]={0};
	uint8_t danger_s[2]={0};
	const uint8_t on_t[]="on ";
	const uint8_t connecting_t[]="connecting: ";
	const uint8_t normal_t[]="normal: ";
	const uint8_t danger_t[]="danger: ";
	itoa(on, on_s, 10);
	itoa(connecting, connecting_s, 10);
	itoa(normal, normal_s, 10);
	itoa(danger, danger_s, 10);
	HAL_UART_Transmit(&huart3, on_t, sizeof(on_t), 100);
	HAL_UART_Transmit(&huart3, on_s, 2, 100);
	HAL_UART_Transmit(&huart3, enter, sizeof(enter), 100);
	HAL_UART_Transmit(&huart3, connecting_t, sizeof(connecting_t), 100);
	HAL_UART_Transmit(&huart3, connecting_s, 2, 100);
	HAL_UART_Transmit(&huart3, enter, sizeof(enter), 100);
	HAL_UART_Transmit(&huart3, normal_t, sizeof(normal_t), 100);
	HAL_UART_Transmit(&huart3, normal_s, 2, 100);
	HAL_UART_Transmit(&huart3, enter, sizeof(enter), 100);
	HAL_UART_Transmit(&huart3, danger_t, sizeof(danger_t), 100);
	HAL_UART_Transmit(&huart3, danger_s, 2, 100);
	HAL_UART_Transmit(&huart3, enter, sizeof(enter), 100);
}

void f_status_off(uint8_t on)
{
	const uint8_t off[]="!Status: OFF!\n\r";
	if(!on) HAL_UART_Transmit(&huart3, off, sizeof(off), 100);
}

void f_menu_transmit(void)
{
	HAL_UART_Transmit(&huart3, menu, sizeof(menu), 1000);
}

void f_off_1(uint8_t * connecting, uint8_t * normal, uint8_t * danger, uint8_t * on)
{
	*connecting=0;
	*normal=0;
	*danger=0;
	*on=0;

	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 0);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 0);//NEW

	HAL_UART_Transmit(&huart3, menu[0], sizeof(menu[0]), 100);

	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim4);
}

void f_on_8(uint8_t * connecting, uint8_t * normal, uint8_t * on)
{
	*connecting=0;
	*normal=0;
	*on=1;
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);

	HAL_UART_Transmit(&huart3, menu[6], sizeof(menu[6]), 100);
	//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	//HAL_TIM_Base_Stop_IT(&htim2);
}

void f_connecting_2(uint8_t * connecting, uint8_t * normal)
{
	*connecting=1;
	*normal=0;
	HAL_UART_Transmit(&huart3, menu[5], sizeof(menu[5]), 100);
	//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim2);
}

void f_normal_3(uint8_t * connecting, uint8_t * normal, uint8_t * danger)
{
	*connecting=0;
	*normal=1;
	*danger=0;
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
	HAL_UART_Transmit(&huart3, menu[1], sizeof(menu[1]), 100);
	//HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	HAL_TIM_Base_Stop_IT(&htim2);

}

void f_pwm_6(uint8_t pwm, uint8_t * on)
{
	HAL_UART_Transmit(&huart3, menu[4], sizeof(menu[4]), 100);
	//HAL_TIM_Base_Stop_IT(&htim2);
	if(on) HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, pwm);
}
void f_pwm_5(uint8_t pwm, uint8_t * on)
{
	HAL_UART_Transmit(&huart3, menu[3], sizeof(menu[3]), 100);
	//HAL_TIM_Base_Stop_IT(&htim2);
	if(on) HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, pwm);
}
void f_pwm_4(uint8_t pwm, uint8_t * on)
{
	HAL_UART_Transmit(&huart3, menu[2], sizeof(menu[2]), 100);
	//HAL_TIM_Base_Stop_IT(&htim2);
	if(on) HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, pwm);
}

void f_danger_7(uint8_t * danger, uint8_t * normal, uint8_t * on)
{
	*danger=1;
	*normal=0;
	HAL_UART_Transmit(&huart3, menu[6], sizeof(menu[6]), 100);
	//HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);
	if(on)
	{
		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, 1);
		HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	}

}

void f_rng_9(uint8_t * connecting, uint8_t * normal, uint8_t * danger, uint8_t *on, uint8_t rng_min, uint8_t rng_max)
{
	uint32_t rng_32=0;
	uint8_t rng_min_max=0;
	uint8_t rng_min_max_str[3]={0};
	*connecting=0;
	*normal=0;
	*danger=0;
	*on=2;
	HAL_RNG_GenerateRandomNumber(&hrng, &rng_32);
	rng_min_max = rng_32%(rng_max-rng_min+1)+rng_min;
	itoa(rng_min_max, rng_min_max_str, 10);
	HAL_UART_Transmit(&huart3, rng_min_max_str, sizeof(rng_min_max_str), 100);
	HAL_UART_Transmit(&huart3, enter, sizeof(enter), 100);
}

uint8_t menu_command(void)
{
	const uint8_t command[9][15]= {	"OFF",
									"Normal",
									"10%",
									"40%",
									"90%",
									"DANGER!!!",
									"ON",
									"RNG"};
	uint8_t cmd=0;
	uint8_t j=0;
	uint8_t i=0;
	uint8_t read_cmd[15]={0};

	while(j<sizeof(read_cmd))
		  {
			  if(HAL_UART_Receive(&huart3, &read_cmd[j], 1, 10)==HAL_OK)
			  {
				  HAL_UART_Transmit(&huart3, &read_cmd[j], 1, 10);
				  // \n - NEW LINE		\r - RETURN		<-- ENTER
				  if((read_cmd[j]=='\n')||(read_cmd[j]=='\r'))
					  {
					  	  read_cmd[j]='\0';
					  	  break;
					  }
				  j++;
			  }
		  }

	  //TEST
	  HAL_UART_Transmit(&huart3, enter, sizeof(enter), 10); //enter
	  HAL_UART_Transmit(&huart3, read_cmd, sizeof(read_cmd), 10);
	  HAL_UART_Transmit(&huart3, enter, sizeof(enter), 10); //enter

	  //STRING COMPARE - strcmp(string1, string2) - 0

	while(i<9)	//clear read_cmd
	{
		if(!strcmp(command[i],read_cmd))
		{
			cmd=(i+1)+'0';	//ASCII code
			break;
		}
		i++;
	}
	return cmd;
}
