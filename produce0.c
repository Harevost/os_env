int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_produce0(int sht_win_pro, int protime);
void api_end(void);
char *api_malloc(int size);
void api_initmalloc(void);

void HariMain(void)
{
	char *probuf;
	int prowin;
	api_initmalloc();
	probuf = api_malloc(328*256);
	int protime = 220;
	prowin = api_openprowin(probuf, 328, 256, -1 ,"Produce0");
    api_produce0(prowin, protime);
	api_end();
}
