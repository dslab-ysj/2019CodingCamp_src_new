#include <apps/shell/tash.h>
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "game.h"
#include "A053BasicKit.h"
#include "mario.h"

/*************************************************
 * functions for games
 ************************************************/

//**********나만의 악기 I에서 사용**********//
//pwm 제어 함수
void pwm_func(int gpio_pin, int period, int duty_cycle, char* name)
{
	int PWM_PIN = PWM0;
	int fd = pwm_open(PWM_PIN);
	printf("%s\n", name);
	pwm_write(fd,period,duty_cycle);
	while(gpio_read(gpio_pin)){}   //switch가 눌려있는 동안 while문 안에 머물기
	pwm_close(fd);
	return;
}
//**********나만의 악기 I에서 사용**********//


//**********2진수 게임**********//
int binary_main(int argc, FAR char *argv[])
{
   int sw_left = PIN_D2;
   int sw_right = PIN_D4;
   int OOO = PIN_D7;
   int XXX = PIN_D8;

   while(1)
   {
      gpio_write(OOO, gpio_read(sw_left));
      gpio_write(XXX, gpio_read(sw_right));

      // Exit condition
      if((gpio_read(sw_right)==1) && (gpio_read(sw_left)==1))
      {
		gpio_write(OOO, LOW);
		gpio_write(XXX, LOW);
		break;
      }

   }
   return 0;
}
//**********2진수 게임**********//


//**********감성자극 오르골**********//
int mp3_main(int argc, FAR char *argv[])
{
	int play_sw = PIN_D2;
	int stop_sw = PIN_D4;

	int play_val = 0;

	int buzzer_pin = PWM0;

	int i;

	int size = sizeof(mario_song) / sizeof(int);

	printf("==============================================\n");
	printf("                                              \n");
	printf("                 MP3 PLAYER                   \n");
	printf("               (SuperMario BGM)               \n");
	printf("==============================================\n");
	printf("                                              \n");
	printf("  Left switch : play | Right switch : stop    \n");



	while(1)
	{
		play_val = gpio_read(play_sw);
		if(play_val == HIGH)
		{
			printf("                                              \n");
			printf("                    PLAY!                     \n");
			printf("                                              \n");

			for(i=0; i<size;i++)
			{
				muse(mario_song[i],mario_beat[i],buzzer_pin);
				if (gpio_read(stop_sw)==1) break;
			}
			break;
		}

	}




	printf("==============================================\n");
	printf("                                              \n");
	printf("                End of player.                \n");
	printf("                                              \n");
	printf("==============================================\n");

	return 0;
}
//**********감성자극 오르골**********//

//**********나만의 악기 I**********//

int piano_main(int argc, FAR char *argv[])
{
   int duty_cycle = 100;

   printf("============= Play song =============\n");

   int sw_DO = PIN_D2;
   int sw_RE = PIN_D4;
   int sw_MI = PIN_D7;
   int sw_SO = PIN_D8;

   int sw_DO_val = 0;
   int sw_RE_val = 0;
   int sw_MI_val = 0;
   int sw_SO_val = 0;

   //switch 처음 눌림 감지
   while(1)
   {
	  sw_DO_val = gpio_read(sw_DO);
	  sw_RE_val = gpio_read(sw_RE);
	  sw_MI_val = gpio_read(sw_MI);
	  sw_SO_val = gpio_read(sw_SO);

	  if((sw_DO_val==1) && (sw_MI_val==1)) break; // Exit condition
	  else if(sw_DO_val == 1) pwm_func(sw_DO, O6_DO, duty_cycle, "DO");
      else if(sw_RE_val == 1) pwm_func(sw_RE, O6_RE, duty_cycle, "RE");
      else if(sw_MI_val == 1) pwm_func(sw_MI, O6_MI, duty_cycle, "MI");
      else if(sw_SO_val == 1) pwm_func(sw_SO, O6_FA, duty_cycle, "SO");
      else {}
   }

   return 0;
}

//**********나만의 악기 I**********//

//**********투명자**********//

int ruler_main(int argc, FAR char *argv[])
{
	int32_t ir_val;
	int sw_stop = PIN_D2;
	int pin_ir = A0;

	while(1)
	{
		ir_val = read_adc(pin_ir);

		if(ir_val>850) printf("Distance : 0cm (%d)\n",ir_val);
		else if(ir_val>500) printf("Distance : 1cm (%d)\n",ir_val);
		else if(ir_val>300) printf("Distance : 2cm (%d)\n",ir_val);
		else if(ir_val>200) printf("Distance : 3cm (%d)\n",ir_val);
		else if(ir_val>170) printf("Distance : 4cm (%d)\n",ir_val);
		else if(ir_val>150) printf("Distance : 5cm (%d)\n",ir_val);
		else if(ir_val>130) printf("Distance : 6cm (%d)\n",ir_val);
		else printf("out of range \n");

		if(gpio_read(sw_stop)==1) break;  // Exit condition

		up_mdelay(1000);
	}
	return 0;
}
//**********투명자**********//


//**********나만의 악기 II**********//
int inst2_main(int argc, FAR char *argv[])
{
	int low_tone_sw  = 0;
	int high_tone_sw = 0;
	int pin_ir = 0;
	int pin_buzzer = 0;
	int val_low_tone_sw = 0;
	int val_high_tone_sw = 0;
	int val_ir = 0;
	int fd_buzzer = 0;
	int TONE = 0;
	int Duty_Cycle = 100;

	low_tone_sw  = PIN_D2; // GPIO 0 PORT
	high_tone_sw = PIN_D4;

	pin_ir = A0; // ADC 0 PORT
	pin_buzzer = PWM0; // PWM 0 PORT


	while(1)
	{
		val_low_tone_sw = gpio_read(low_tone_sw);
		val_high_tone_sw = gpio_read(high_tone_sw);
		val_ir = read_adc(pin_ir);
		up_mdelay(100);

		if((val_low_tone_sw == HIGH)&&(val_high_tone_sw == LOW))
		{
			fd_buzzer = pwm_open(pin_buzzer);
			if(val_ir>550) 		TONE = O5_DO;
			else if(val_ir>250) TONE = O5_RE;
			else if(val_ir>140) TONE = O5_MI;
			else if(val_ir>105) TONE = O5_FA;
			else 				TONE = O5_SO;
			pwm_write(fd_buzzer,TONE,Duty_Cycle);
			up_mdelay(100);
			pwm_close(fd_buzzer);
		}
		else if ((val_low_tone_sw == LOW)&&(val_high_tone_sw == HIGH))
		{
			fd_buzzer = pwm_open(pin_buzzer);
			if(val_ir>550) 		TONE = O5_LA;
			else if(val_ir>250) TONE = O5_TI;
			else if(val_ir>140) TONE = O6_DO;
			else if(val_ir>105) TONE = O6_RE;
			else 				TONE = O6_MI;
			pwm_write(fd_buzzer,TONE,Duty_Cycle);
			up_mdelay(100);
			pwm_close(fd_buzzer);

		}
		else if ((val_low_tone_sw == HIGH)&&(val_high_tone_sw == HIGH))
		{
			break;
		}
		else{ }
	}
	return 0;
}
//**********나만의 악기 II**********//



static void show_game_hexagon(FAR const char *program)
{
   printf("|-------------------------------------------|\n");
   printf("| ******* ARTIK053 BASIC KIT GAMES ******** |\n");
   printf("|               %s binary                 |\n", program);
   printf("|              %s musicbox                |\n", program);
   printf("|             %s instrument1              |\n", program);
   printf("|               %s ruler                  |\n", program);
   printf("|             %s instrument2              |\n", program);
   printf("|-------------------------------------------|\n");

}


int game_main(int argc, FAR char *argv[])
{

   if ( (argc==2) && (strcmp(argv[1],"binary")==0) ) {binary_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"musicbox")==0) ) {mp3_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"instrument1")==0) ) {piano_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"ruler")==0) ) {ruler_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"instrument2")==0) ) {inst2_main(argc,argv);}
   else show_game_hexagon(argv[0]);

   return 0;
}







