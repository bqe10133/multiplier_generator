//------------------------------------------------------------------------------
// 1st     release   Dec.24,2023 : all codes from scratch
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
// version 1.0 Dec.24,2023
//-------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <math.h>
using namespace std;
void daddasort(int bufinlen,int *bufin);
void daddafa(int first_val,int second_val,int third_val);
void daddatreecalc(int *mainbuf,int afternum);

int main(int argc, char * const argv[]) {
	
  int  mainbuf[1024];//equation main
	char c0;
	int  d0;
	int  i;
	int  afternum;
	
	ifstream fv0(argv[1]);//equation 
	
	for(i=0;i<1024;i++){
		mainbuf[i]=0;
	}
	for(i=0;i<1024;i++){
		fv0.get(c0);	
		d0=(int)c0;
		mainbuf[i]=d0;
		if(d0==90){break;}
	}
	
	afternum  = atoi(argv[2]);
	daddatreecalc(mainbuf,afternum);
	
	fv0.close();
	return 0;
}

//---------------------------------
//function : daddatreecalculation
// 3bit x n -> 2bit x n
//---------------------------------
void daddatreecalc(int *mainbuf,int afternum){
	
	int i,j,m,n;
	int bufin[256];
	int bufinlen;
	int flag;
	int first_val,second_val,third_val;
	int daddavalue;
	int half_carry,half_sum;
	int carrybuf[256];
	int sumbuf[256];
	int fanum,hanum;
	
	//-----------------------------
	// store the value in bufin[].
	//-----------------------------
	m=0;
	daddavalue=0;
	for(i=0;i<32768;i++){
		if(mainbuf[i]==90){ // char "Z" = 90
			break;
		}
		else if(mainbuf[i]==32 & i==0){ // 20230510 ignore the 1st char " " = 32
		}
		else if(mainbuf[i]==32){ // char " " = 32
			bufin[m]=daddavalue;
			m++;
			daddavalue=0;
		}
		else{
			daddavalue=daddavalue*10;
			daddavalue=daddavalue+(mainbuf[i]-48);//char "0" = 48
		}
	}
	bufinlen=m;
	
	//-----------------------------
	// fanum hanum calculation
	//-----------------------------
	fanum = (bufinlen -afternum)/2;
	hanum = (bufinlen -afternum)%2;

	//-----------------------------
	// calculation 
	//-----------------------------
	printf("bufinlen0 :%d\n",bufinlen);
	for(i=0; i<bufinlen; i++) {
		printf("%d ",bufin[i]);
	}
	printf("\n");
	
	daddasort(bufinlen, bufin);
	
	printf("bufinlen1 :%d\n",bufinlen);
	for(i=0; i<bufinlen; i++) {
		printf("%d ",bufin[i]);
	}
	printf("\n");
	
	flag=0;
	
	for(i=0;i<bufinlen;i++){
		if(i>=(bufinlen-fanum*3)){
			bufin[i]=bufin[i]*1000;
		}
		else{
			bufin[i]=bufin[i];
		}
	}
	
	printf("bufinlen2 :%d\n",bufinlen);
	for(i=0; i<bufinlen; i++) {
		printf("%d ",bufin[i]);
	}
	printf("\n");
	
//------------------------------------
// bufinlen is smaller then afternum?
//------------------------------------
//smaller
if(bufinlen<=afternum){
	  printf("smaller\n");
	  m=0;
		n=0;
	  for(i=0;i<bufinlen;i++){
		  sumbuf[m]=bufin[i]; m++;
		}
	  printf("\n");

  printf("smaller m n: %d %d\n",m,n);
	printf("FINAL sumbuf ");
	for(i=0;i<m;i++){
		printf("%d ",sumbuf[i]);
	}
	printf("\n");

	printf("FINAL carrybuf ");
	if(n==0){
	  printf("0");
	}
	else{
	  for(i=0;i<n;i++){
	 	  printf("%d ",carrybuf[i]);
	  }
	}
	printf("\n");
}

//bigger 
else{
	printf("bigger\n");

  m=0;
	n=0;
  //-----------------
  //thru
  //-----------------
	if((bufinlen-fanum*3-hanum*2)>0){
	  for(i=0;i<(bufinlen-fanum*3-hanum*2);i++){
	  	printf("resultthru s%d\n",bufin[i]);
			sumbuf[m]=bufin[i]; m++;
		}
	  printf("\n");
	}
	else{
		printf("no thru\n");
	  printf("resultthru s0\n");
	}

  //-----------------
  //half adder
  //-----------------
	if((bufinlen-fanum*3-hanum*2)>=0 & hanum==1){//20230511
	  printf("half adder %d,%d\n",bufin[bufinlen-fanum*3-hanum*2],bufin[bufinlen-fanum*3-hanum*2+1]);
	  half_carry=bufin[bufinlen-fanum*3-hanum*2]+1;
	  half_sum  =bufin[bufinlen-fanum*3-hanum*2]+3;
	  printf("resultha c%d\nresultha s%d\n",half_carry,half_sum);
		sumbuf[m]=half_sum;   m++;
		carrybuf[n]=half_carry; n++;
	}
	else{
		printf("no half adder\n");
	  printf("resultha c0\nresultha s0\n");
	}

  //-----------------
  //full adder
  //-----------------
	for(i=0;i<fanum;i++){
		daddasort(bufinlen, bufin);
		flag=0;
		
		for(j=0;j<bufinlen;j++){
			if(j==0){
				bufin[j]=bufin[j]/1000;
				first_val=bufin[j];
			}
			else if(j>0 & j<(fanum-i)){
				if((bufin[j]/1000<=(bufin[0]-3)) & flag==0){
					flag=1;
					bufin[j]=bufin[j]/1000;
					bufin[j+1]=bufin[j+1]/1000;
					second_val=bufin[j];
					third_val=bufin[j+1];
				}
			}
			else if(j==(fanum-i)){
				if(flag==0){
					if(bufin[j]==bufin[j-1]){
						flag=1;
						bufin[j]=bufin[j]/1000;
						bufin[j-1]=bufin[j-1]/1000;
						second_val=bufin[j];
						third_val=bufin[j-1];
					}
					else{
						flag=1;
						bufin[j]=bufin[j]/1000;
						bufin[j+1]=bufin[j+1]/1000;
						second_val=bufin[j];
						third_val=bufin[j+1];
					}
				}
			}
		}
		printf("first_val:%d second_val:%d,third_val:%d\n",first_val,second_val,third_val);

	  int first_carry,first_sum;
	  int second_carry,second_sum;
	
	  first_carry=first_val+2;
	  first_sum=first_val+3;
	  second_carry=second_val+3;
	  second_sum=second_val+6;

	  if(first_carry>=second_carry){
		  printf("resultfa c%d\n",first_carry);
		  carrybuf[n]=first_carry; n++;
	  }
	  else{
		  printf("resultfa c%d\n",second_carry);
		  carrybuf[n]=second_carry; n++;
	  }
	
	  if(first_sum>=second_sum){
		  printf("resultfa s%d\n",first_sum);
		  sumbuf[m]=first_sum;   m++;
	  }
	  else{
		  printf("resultfa s%d\n",second_sum);
		  sumbuf[m]=second_sum;   m++;
	  }
	}	

  printf("sum %d carry %d\n",m,n);

	printf("FINAL sumbuf ");
	for(i=0;i<m;i++){
		printf("%d ",sumbuf[i]);
	}
	printf("\n");

	printf("FINAL carrybuf ");
	if(n==0){
	  printf("0");
	}
	else{
	  for(i=0;i<n;i++){
	 	  printf("%d ",carrybuf[i]);
	  }
	}
	printf("\n");
}
}


void daddasort(int bufinlen,int *bufin){

	int i,j,k;
	int min,tmp;
	int bufintmp[256];
	
	for(i=0;i<bufinlen-1;i++){
		min=bufin[i];
		k=i;
		for(j=i+1;j<bufinlen;j++){
			if(bufin[j]<min){
				min=bufin[j];
				k=j;
			}
		}
		tmp=bufin[i];
		bufin[i]=bufin[k];
		bufin[k]=tmp;
	}
	
	//sort reverse
	for(i=0;i<bufinlen;i++){
		bufintmp[bufinlen-1-i]=bufin[i];
	}
	for(i=0;i<bufinlen;i++){
		bufin[i]=bufintmp[i];
	}
}


