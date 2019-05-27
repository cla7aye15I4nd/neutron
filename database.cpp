#include<bits/stdc++.h>
using namespace std;
string s;
int main()
{
	//puts("fuck");
	while(getline(cin, s))
	{
		if(s=="") continue;
		if(s=="exit") return 0;
		if(s.find("query_ticket_all")!=s.npos)
		{
			puts("2");
			puts(u8"fuck 北京 08:00 上海 08:10 1 1d1h10m");
			puts("3");
			puts("Master 5 765.50 ");
			puts("Normal 8 765.49 ");
			puts("Trivial 0 765.48");
			puts("shite Beijing 08:00 Shanghai 11:00 0 3h");
			puts("3");
			puts("Master 0 765.50");
			puts("Normal 0 765.49");
			puts("Trivial 0 765.48");
			
		}
		else if(s.find("query_ticket_transfer_available")!=s.npos)
		{
			puts("0");
		}
		else if(s.find("query_ticket_available")!=s.npos)
		{
			puts("1");
			puts("fuck Beijing 08:00 Shanghai  08:10 1 1d1h10m");
			puts("3");
			puts("Master 5 765.50 ");
			puts("Normal 8 765.49 ");
			puts("Trivial 0 765.48");
		}
		else if(s.find("query_ticket_transfer")!=s.npos)
		{
			puts("2");
			puts("fuck Beijing  08:00 Zhengzhou  08:10 1 1d1h10m");
			puts("3");
			puts("Master 5 765.50 ");
			puts("Normal 8 765.49 ");
			puts("Trivial 0 765.48");
			puts("shite Zhengzhou  08:00 Shanghai  11:00 0 3h");
			puts("3");
			puts("Master 0 765.50");
			puts("Normal 0 765.49");
			puts("Trivial 0 765.48");
		}
		else if(s.find("query_order_all")!=s.npos)
		{
			puts("2");
			puts("fuck Beijing  2019-06-07 08:00 Shanghai  08:10 1 1d1h10m Master");
			puts("shite Beijing  2019-09-05 08:00 Shanghai  11:00 0 3h Trivial");
		}
		else if(s.find("query_station")!=s.npos)
		{
			puts("3");
			puts("Beijing");
			puts("Shanghai");
			puts("Zhengzhou");
		}
		else if(s.find("register")!=s.npos)
		{
			puts("1");
		}
		else if(s.find("add_train")!=s.npos)
		{
			puts("1");
		}
		else if(s.find("sale_train")!=s.npos)
		{
			puts("1");
		}
		else if(s.find("query_train")!=s.npos)
		{
			puts("abc123456 G123456 G 3 3 商务座 大师座 大屌坐 0");
			puts("北京 xx:xx 08:00 00:00 0.0 0.0 0.0");
			puts("幻想乡 10:20 10:30 00:00 1.5 15.0 500.0");
			puts("夏威夷 18:01 xx:xx 00:00 2.8 36.0 23333.233");
		}
		else if(s.find("delete_train")!=s.npos)
		{
			puts("1");
		}
		else if(s.find("modify_train")!=s.npos)
		{
			puts("1");
		}
		
		else puts("Invalid");
	}
	return 0;
}
