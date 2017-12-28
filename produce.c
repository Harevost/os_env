int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_produce(int sht_win_pro);
void api_end(void);

char buf[328*256];

void HariMain(void)
{
	int prowin;
	//struct proAndCon *pac = (struct proAndCon *) * ((int *) (ADDR));
	prowin = api_openprowin(buf, 328, 256, -1 ,"Produce");
    api_produce(prowin);
	api_end();
}
