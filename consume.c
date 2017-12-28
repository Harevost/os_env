int api_openconwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_consume(int sht_win_con);
void api_end(void);

char buf[328*256];

void HariMain(void)
{
	int conwin;
	//struct proAndCon *pac = (struct proAndCon *) * ((int *) (ADDR));
	conwin = api_openconwin(buf, 328, 256, -1 ,"Consume");
    api_consume(conwin);
	api_end();
}
