#include <Def_INC.h>
#include <assert.h>

void AUTO_COLOR_TEST(void)
{
BYTE TEST = 0;

while(1)
{
		if((++TEST) > 6)
			TEST = 1;
	switch(TEST)
		{
		case 0:
			printf("%d\n case 0:",TEST);
			break;
		case 1:
			printf("%d\n case 1:",TEST);
			break;
		case 2:
			printf("%d\n case 2:",TEST);
			break;
		case 3:
			printf("%d\n case 3:",TEST);
			break;
		case 4:
			printf("%d\n case 4:",TEST);
			break;
		case 5:
			printf("%d\n case 5:",TEST);
			break;
		case 6:
			printf("%d\n case 6:",TEST);
			break;

		default:
			break;

		}
}
}

void try_test(void)
{
BYTE i;

#if 1

while(0)
{
ACC = 0;
_nop_();
ACC = 1;
_nop_();
}



for(i=1;0;i++)
{
ACC = 0;
_nop_();
ACC = 1;
_nop_();
}
#pragma asm
		mov ACC,#1d
run_loop:
		djnz ACC,run_loop
		jnz run_loop
		mov ACC,#0AAH
#pragma endasm
		printf("\nA = %x\n",ACC);
#elif 0
		scanf("\n%d %f %c",&x,&y,&z);
		printf("\n%d %f %c\n",x,y,z);

		printf("\n%x\n",ALERT_BTN_OK);
		printf("%x\n",ALERT_BTN_CANCEL);
		printf("%x\n",MAX_ALERT_BTN_NUM);
#else

#endif
}

void check_parms (char *string)
{
assert (string != NULL); /* check for NULL ptr */
printf ("String %s is OK\n", string);
}

void Check_drafting(void)
{
BYTE xdata index=0;
BYTE xdata arrary[10]={1,2,3,4,5,6,7,8,9,0};


while(index != 'q')
{
	index=getchar();
	index-=65;
	
	printf("\r%d",arrary[index]);

/*
   switch(index)
   {
	case 'a':
		y--;
//	    gotoxy(x,y);
	printf("\r 1");
		break;
	case 's':
		x--;
//	    gotoxy(x,y);
	printf("\r 2");
		break;
	case 'd':
		x++;
//	    gotoxy(x,y);
	printf("\r 3");
		break;
	case 'f':
		y++;
//	    gotoxy(x,y);
	printf("\r 4");
		break;
	default:
		break;
   }
*/
}
printf("EXIT",index);
for(;;);

}

void Test_Char_Key(void)
{
BYTE get_ch_123[2] = {0,0},i;

do{
get_ch_123[1] = 0;
}while(/*get_ch_123[1]  == '\n'*/0);

while(1)
{

	for(i=0;i<2;i++)
		{
		get_ch_123[i] = getchar();

	switch(get_ch_123[0])
		{
		case 41:
			get_ch_123[0] = 'A';
			break;
		case 42:
			get_ch_123[0] = 'B';
			break;
		case 43:
			get_ch_123[0] = 'C';
			break;
		case 44:
			get_ch_123[0] = 'D';
			break;
		case 45:
			get_ch_123[0] = 'E';
			break;
		case 46:
			get_ch_123[0] = 'F';
			break;
		case 30:
			get_ch_123[0] = 0;
			break;
		case 31:
			get_ch_123[0] = 1;
			break;
		case 32:
			get_ch_123[0] = 2;
			break;
		case 33:
			get_ch_123[0] = 3;
			break;
		case 34:
			get_ch_123[0] = 4;
			break;
		case 35:
			get_ch_123[0] = 5;
			break;
		case 36:
			get_ch_123[0] = 6;
			break;
		case 37:
			get_ch_123[0] = 7;
			break;
		case 38:
			get_ch_123[0] = 8;
			break;
		case 39:
			get_ch_123[0] = 9;
			break;
			
		default:
			get_ch_123[0] = 0xff;
			break;
		}
	printf("INPUT_KEY = %d\n",get_ch_123[i]);
		}
	if(get_ch_123[1] == '\n')
	P1 = get_ch_123[0];
	
	for(i=0;i<2;i++)
		{
		get_ch_123[i] = 0;
		}

}
}


