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
	char str[]="5b89,5427,767e,8d25,677f,7248,5305,4fdd,88ab,672c,95ed,7f16,4fbf,8868,5e76,4e0d,90e8,83dc,53c2,63d2,67e5,5c1d,5e38,79f0,7a0b,6c60,6301,51fa,521d,9664,5904,521b,6b21,4ece,5b58,9519,6253,5927,4ee3,5f85,5355,4f46,5f53,5bfc,5230,9053,7684,7b2c,7535,5b9a,52a8,8bfb,5ea6,77ed,65ad,961f,5bf9,591a,4e8c,53d1,6cd5,7ffb,65b9,8bbf,975e,8d39,5206,5426,590d,8be5,6539,611f,9ad8,4e2a,66f4,8d2d,56fa,5173,5b98,56fd,679c,8fc7,542b,51fd,884c,53f7,4f55,540e,5316,574f,7f13,6362,6062,6216,83b7,57fa,53ca,5373,8ba1,9645,52a0,5939,95f4,517c,68c0,4ef6,5efa,5c06,63a5,7ed3,89e3,8fdb,7981,656c,53e5,636e,5f00,53ef,7a7a,53e3,5e93,5757,51b5,7c7b,7406,7acb,8fde,94fe,91cf,5217,7edc,7801,4e70,6ee1,6ca1,7f8e,4eec,5bc6,514d,9762,540d,660e,547d,6a21,5185,80fd,60a8,6b27,6392,914d,5339,504f,7834,671f,9f50,5176,542f,8d77,524d,6e05,60c5,8bf7,6c42,533a,6e20,53d6,6743,5168,7f3a,786e,7136,4efb,65e5,5bb9,5982,5165,8f6f,7b5b,5220,5546,8bbe,751f,58f0,5931,65f6,5b9e,4f7f,59cb,793a,5f0f,8bd5,662f,9996,53d7,6388,6570,5237,987a,8bf4,9001,641c,635f,7f29,6240,7d22,4ed6,5b83,53f0,592a,6001,9898,4f53,6dfb,6761,8c03,8d34,901a,540c,7edf,5934,9014,5916,5b8c,73a9,7f51,671b,4e3a,672a,6587,95ee,65e0,52ff,8bef,6790,606f,620f,7cfb,4e0b,5148,663e,73b0,76f8,8be6,54cd,5411,9879,6d88,5c0f,6548,5199,8c22,65b0,4fe1,5f62,578b,9700,5e8f,9009,5bfb,5faa,538b,4e9a,9a8c,8981,94a5,4e1a,79fb,5df2,4ee5,5f02,8bd1,610f,5f15,5e94,7528,7531,6e38,6709,4e8e,4e0e,8bed,9884,57df,6e90,8fd0,8f7d,518d,5728,6682,62e9,957f,627e,8005,8fd9,8bca,6574,6b63,652f,77e5,6267,6b62,81f3,5236,81f4,7f6e,4e2d,91cd,6d32,4e3b,88c5,72b6,8ffd,684c,81ea,8db3,6700,9075,4f5c,5173,4e8e,4e2d,6587,7248,6e90,7801,57fa,4e8e,5f00,53d1,8005,7684,7531,7834,89e3,767e,5ea6,8d34,5427,7ffb,8bd1,7f16,8bd1,5236,4f5c,9075,5faa,6388,6743,7981,6b62,7528,4e8e,4efb,4f55,5f62,5f0f,7684,5546,4e1a,7528,9014,751f,6548,4e2d,7684,914d,7f6e,4fe1,606f,6e38,620f,8ffd,52a0,4e0b,8f7d,5185,5bb9,6e38,620f,517c,5bb9,5305,81ea,52a8,66f4,65b0,81ea,52a8,66f4,65b0,542f,7981,7528";
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
