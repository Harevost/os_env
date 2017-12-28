int api_openstawin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
int api_openconwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_pacsta(int sht_win_sta);
void api_consume(int sht_win_con);
void api_produce(int sht_win_pro);
void api_end(void);

char buf0[328*256], buf1[328*256], buf2[600*200];

void HariMain(void)
{
	int stawin, prowin, conwin;
	//struct proAndCon *pac = (struct proAndCon *) * ((int *) (ADDR));
    stawin = api_openstawin(buf0, 600, 200, -1, "PaCSta");
    prowin = api_openprowin(buf1, 328, 256, -1, "Produce");
	conwin = api_openconwin(buf2, 328, 256, -1, "Consume");
   
    api_produce(prowin);
    api_consume(conwin); 
    api_pacsta(stawin);
	
}