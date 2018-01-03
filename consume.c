int api_openconwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_consume(int sht_win_con, int con_time);
void api_end(void);
char *api_malloc(int size);
void api_initmalloc(void);

void HariMain(void)
{
	char *conbuf;
	int conwin;
	api_initmalloc();
	conbuf = api_malloc(328*256);
	int contime = 100;
	conwin = api_openconwin(conbuf, 328, 256, -1 ,"Consume");
    api_consume(conwin, contime);
	api_end();
}
