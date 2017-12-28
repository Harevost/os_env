int api_openconwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_consume0(int sht_win_con0);
void api_end(void);

char buf[200*150];

void HariMain(void)
{
	int conwin;
	conwin = api_openconwin(buf, 328, 256, -1 ,"Consume0");
    api_consume0(conwin);
	api_end();
}
