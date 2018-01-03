int api_openstawin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_pacsta(int sht_win_sta);
void api_end(void);
char *api_malloc(int size);
void api_initmalloc(void);

void HariMain(void)
{
	char *stabuf;
	int stawin;
	api_initmalloc();
	stabuf = api_malloc(600*200);
	stawin = api_openstawin(stabuf, 600, 200, -1 ,"Pacsta");
    api_pacsta(stawin);
	api_end();
} 
