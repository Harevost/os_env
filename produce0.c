int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_produce0(int sht_win_pro0);
void api_end(void);

char buf[200*150];

void HariMain(void)
{
	int prowin;
	prowin = api_openprowin(buf, 328, 256, -1 ,"Produce0");
    api_produce0(prowin);
	api_end();
}
