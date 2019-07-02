#include <stdio.h>

#define YEAR  2016
static char const mounth_r[12] = {31,29,31,30,31,30,31,31,30,31,30,31};  // run nian 366 days
static char const mounth_p[12] = {31,28,31,30,31,30,31,31,30,31,30,31};  // pin nian 365 days
static int m_time_stamp = 0; // sencond from 2016/1/1 00:00:00 
static char m_date[14] = {0x32,0x30,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30}; // 2016/1/1 00:00:00 
static char rc_date[14] = {0x32,0x30,0x31,0x36,0x30,0x31,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30}; // 2016/1/1 00:00:00 
static char m_second = 0;
static char m_minute = 0;
static char m_hour = 0;
static char m_day = 1;
static char m_mounth = 1;
static int  m_year = YEAR;
void update_m_date(void)
{
	m_date[0] = m_year/1000;
	m_date[1] = m_year%1000/100;
	m_date[2] = m_year%100/10;
	m_date[3] = m_year%10;
	m_date[4] = m_mounth/10;
	m_date[5] = m_mounth%10;
	m_date[6] = m_day/10;
	m_date[7] = m_day%10;
	m_date[8] = m_hour/10;
	m_date[9] = m_hour%10;
	m_date[10] = m_minute/10;
	m_date[11] = m_minute%10;
	m_date[12] = m_second/10;
	m_date[13] = m_second%10;
	for(int i=0; i<14; i++)
		m_date[i] = m_date[i] + 0x30;
}
void updata_m_time_stamp(void)
{
	int year,day_a = 0;
	char mounth, day, hour, minute, second;
	for(int i=0; i<14; i++)
		rc_date[i] = rc_date[i] - 0x30;
	year = rc_date[0]*1000 + rc_date[1]*100 + rc_date[2]*10 + rc_date[3] - 2016;
	mounth = rc_date[4]*10 + rc_date[5] - 1;
	day = rc_date[6]*10 + rc_date[7] - 1;
	if(year%4 != 0)
	{
		day_a = 366 + year/4*1461 + (year%4-1)*365;
		for(int i=0; i<mounth; i++)
			day_a = day_a + mounth_p[i];
		day_a = day_a + day;
	}
	else
	{
		day_a = year/4*1461;	
		for(int i=0; i<mounth; i++)
			day_a = day_a + mounth_r[i];
		day_a = day_a + day;
	}
	hour = rc_date[8]*10 + rc_date[9];
	minute = rc_date[10]*10 + rc_date[11];
	second = rc_date[12]*10 + rc_date[13];
	m_time_stamp = ((day_a*24+hour)*60+minute)*60+second;
}
void updata_mydate(void)
{
	int  day_a  = m_time_stamp/(3600*24);
	char hour   = m_time_stamp%(3600*24)/3600;
	char minute = m_time_stamp%(3600*24)%3600/60;
	char second = m_time_stamp%(3600*24)%3600%60;
	int  year_n = day_a/1461;
	int  year_d = day_a%1461;
	int  year;
	char mounth,day;
    if( year_d <= 366 ) //run nian
	{
		if( year_d == 366)
		{
			year = year_n*4+1;
			mounth = 1;
			day = 1;	
		}
		else
		{
			year = year_n*4;
			for(int i=0; i<12; i++)
			{
				if(year_d>=mounth_r[i])
				{
					year_d = year_d - mounth_r[i];
				}
				else
				{
					mounth = i+1;	
					day = year_d+1;
					break;
					
				}
			}
		}
	}
	else               //pin nian
	{
		year = year_n*4+1+(year_d - 366)/365;
		year_d = (year_d - 366)%365;
		for(int i=0; i<12; i++)
		{
			if(year_d>=mounth_p[i])
			{
				year_d = year_d - mounth_p[i];
			}
			else
			{
				mounth = i+1;	
				day = year_d+1;
				break;
			}
		}	
	}
	m_year = YEAR + year;
	m_mounth = mounth;
	m_day = day;
	m_hour = hour;
	m_minute = minute;
	m_second = second;
	update_m_date();
}
int main(void)
{
	int rc_buff[255],i=0;
	printf("please input the time now like 201907021624.\r\n");
	do{	
		scanf("%c",&rc_buff[i]);
		i++;
	}while(rc_buff[i-1] != '\r' && rc_buff[i-1] != '\n' && i<=255);
	//memcpy(rc_date,rc_buff,14);
	for(int i=0; i<14; i++)
	{
		rc_date[i] = rc_buff[i];
	}
	updata_m_time_stamp();
	updata_mydate();
	printf("time is: %s\r\n",m_date);

	return 0;
}
