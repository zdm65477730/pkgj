#include <stdio.h>
#include <math.h>
#include <stdlib.h>
int HexToDec(char *p){
	
	int result=0;
	for (int i=0;i<4;i++){
		//printf("%c",p[i]);
		if ('0'<=p[i]&&p[i]<='9') p[i]=p[i]-'0';
		if ('a'<=p[i]&&p[i]<='z') p[i]=p[i]-'a'+10;
		result+=(int)pow(16,(3-i))*p[i];
	}
	return result;
}
void ListAppend(int *list,int length,int data){
	list=realloc(list, sizeof(int)*(length+1));
	list[length]=data;
}
void sort(int *p,int length){
	int t;
	for (int i=0;i<length;i++){
		for (int j=i;j<length;j++){
			if (p[i]>p[j]){
				t=p[i];
				p[i]=p[j];
				p[j]=t;
			}
		}
	}
}
 int main(void){
	char str[]="5b89,6557,677f,7248,5305,4fdd,88ab,672c,5fc5,9589,7de8,8868,5e76,4e0d,90e8,83dc,53c3,63d2,67e5,9577,5e38,5617,7a31,6c60,6301,51fa,521d,9664,8655,5275,5b58,932f,9054,6253,5927,4ee3,55ae,4f46,7576,5230,7684,4f4e,96fb,8abf,5b9a,52d5,8b80,5ea6,65b7,5c0d,591a,767c,6cd5,975e,5206,5426,5fa9,8a72,9ad8,66f4,56fa,95dc,570b,904e,542b,51fd,884c,865f,5f8c,5316,58de,7de9,63db,6062,6216,7372,53ca,5373,8a08,969b,52a0,593e,517c,9593,6aa2,4ef6,5efa,5c07,63a5,89e3,9032,53e5,64da,958b,53ef,7a7a,5eab,6cc1,985e,7406,7acb,9023,94fe,93c8,91cf,5217,78bc,6eff,6c92,7f8e,5bc6,9762,540d,660e,547d,6a21,5185,80fd,6b50,6392,6ce1,914d,5339,504f,671f,9f4a,8d77,5553,6c23,524d,6e05,60c5,8acb,6c42,5340,53d6,5168,7f3a,78ba,65e5,5bb9,5982,5165,8edf,7be9,522a,8a2d,8072,5931,6642,5be6,4f7f,59cb,793a,5f0f,662f,8a66,53d7,6578,5237,9806,8aac,641c,640d,7e2e,6240,7d22,614b,9ad4,6dfb,689d,901a,540c,7d71,982d,5916,5b8c,671b,70ba,672a,6587,7121,8aa4,6790,606f,7cfb,6232,4e0b,5148,986f,73fe,76f8,8a73,97ff,5411,9805,6d88,5c0f,6548,5beb,65b0,4fe1,578b,9808,9700,5e8f,9078,5c0b,58d3,4e9e,8981,9470,79fb,5df2,4ee5,7570,610f,5f15,61c9,7528,904a,6e38,6709,65bc,8207,8a9e,57df,9810,904b,8f09,518d,5728,66ab,5247,64c7,627e,8a3a,6574,6b63,652f,77e5,57f7,81f3,81f4,7f6e,88fd,4e2d,91cd,6d32,88dd,72c0,8ffd,684c,81ea,8db3,6700,53cb,60c5,63d0,793a,5df2,7ecf,53d1,5e03,5efa,8bae,4f7f,7528,5176,4ee3,66ff,517c,5bb9,5305,8fd9,662f,4f60,9996,6b21,6253,5f00,4e2d,6587,7248,6b22,8fce,4f7f,7528,672c,8f6f,4ef6,6e90,7801,57fa,4e8e,7684,672c,8f6f,4ef6,9075,5faa,6388,6743,7981,6b62,7528,4e8e,5546,4e1a,7528,9014,672c,8f6f,4ef6,4e0b,8f7d,4f7f,7528,5b8c,5168,514d,8d39,8bf7,52ff,4ece,5176,4ed6,6e20,9053,8d2d,4e70,672c,8f6f,4ef6,7531,7834,89e3,5427,7ffb,8bd1,7834,89e3,5427,4fee,6539,7f16,8bd1,66f4,591a,4fe1,606f,8bf7,8bbf,95ee,76f8,5173,4e0d,518d,63d0,793a,597d";
	char str5[5];
	int a=0,b,length=1,i;
	int result,count=0;
	static int p[1000];
	for (int i=0;;i++){
		if ((a=(i+1)%5)==0){
			str5[4]=0;
			b=HexToDec(str5);
			//ListAppend(p, length++, b);
			p[length++]=b;
		}
		if (str[i]==0) break;
		
		str5[a-1]=str[i];
		
	}
	sort(p,length);
	p[0]=19968;
	for (int i=1;i<length;i++){
		if ((p[i]-p[i-1]))
			printf("%d,",p[i]-p[i-1]);
	}
}
