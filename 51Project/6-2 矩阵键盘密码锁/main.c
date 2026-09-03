#include <REGX52.H>
#include <INTRINS.H>
#include "Delay.h"
//#include "Nixie.h"
#include "LCD1602.h"
#include "MatrixKey.h"

//	LCD_ShowChar(1,1,"A");
//	LCD_ShowString(1,1,"HelloWorld!");
//	LCD_ShowNum(1,9,123,3);
//	LCD_ShowSignedNum(1,13,-66,2);
//	LCD_ShowHexNum(2,1,0xA8,2);
//	LCD_ShowBinNum(2,4,0xAA,8);

void LCD_Clear(unsigned char *str){
	LCD_ShowString(1,1,str);
	Delay(2000);				    //清除及时性反馈
	LCD_ShowString(1,1,"Password:");
}

void main() {
	//如果要放到函数中传递，需要改成全局变量（放到函数外面）
	unsigned char KeyNum;
	unsigned int passwd = 0;
	unsigned char count = 0; //存储输入的密码
	
	// 先初始化
	LCD_Init();
	LCD_ShowString(1,1,"Password:");

    while(1) {
		KeyNum=MatrixKey();
		
		if(KeyNum){
			if(KeyNum<10){
				//如果输入密码小于4
				if(count<4){
					passwd*=10;			//密码左移一位
					passwd+=KeyNum%10;  //获取一位密码
				}count++;
				
				if(count==4){
					if(passwd==1234){
						LCD_Clear("UNLOCK!     ");
					}else{
						LCD_Clear("FAULE__      ");
					}
					count=0;
					passwd=0;						//666，只有 密码 和 计数器 同时重置才行
				}
				
				
				//无密码显示模式
//				LCD_ShowNum(2,1,passwd,4); //最后执行，如果提前会导致无法清除密码，因为passwd=0是后执行的；
			}
		}
    }
}

