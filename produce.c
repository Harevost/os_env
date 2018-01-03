int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_produce(int sht_win_pro, int pro_time);
void api_end(void);
char *api_malloc(int size);
void api_initmalloc(void);

void HariMain(void)
{
	char *probuf;
	int prowin;
	api_initmalloc();
	probuf = api_malloc(328*256);
	int protime = 100;
	prowin = api_openprowin(probuf, 328, 256, -1 ,"Produce");
    api_produce(prowin, protime);
	api_end();
}
