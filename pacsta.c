int api_openstawin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_pacsta(int sht_win_sta);
void api_end(void);

char buf[200*150];

void HariMain(void)
{
	int stawin;
	//struct proAndCon *pac = (struct proAndCon *) * ((int *) (ADDR));
	stawin = api_openstawin(buf, 600, 200, -1 ,"PaCsta");
    api_pacsta(stawin);
	api_end();
}
