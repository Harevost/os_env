int api_openconwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_consume(int sht_win_con, int con_time);

int api_openprowin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_produce(int sht_win_pro, int pro_time);

void api_produce0(int sht_win_pro, int pro_time);
void api_consume0(int sht_win_con, int con_time);

int api_openstawin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_pacsta(int sht_win_sta);

char *api_malloc(int size);
void api_initmalloc(void);

void api_end(void);


void user_consume0(int contime, char* contitle)
{
	char *con_buf;
	int conwin;
	api_initmalloc();
	con_buf = api_malloc(328*256);
	conwin = api_openconwin(con_buf, 328, 256, -1 , contitle);
    api_consume0(conwin, contime);
}

void user_produce0(int protime, char* protitle)
{
	char *pro_buf;
	int prowin;
	api_initmalloc();
	pro_buf = api_malloc(328*256);
	prowin = api_openprowin(pro_buf, 328, 256, -1 , protitle);
    api_produce0(prowin, protime);
}

void user_consume(int contime, char* contitle)
{
	char *con_buf;
	int conwin;
	api_initmalloc();
	con_buf = api_malloc(328*256);
	conwin = api_openconwin(con_buf, 328, 256, -1 , contitle);
    api_consume(conwin, contime);
}

void user_produce(int protime, char* protitle)
{
	char *pro_buf;
	int prowin;
	api_initmalloc();
	pro_buf = api_malloc(328*256);
	prowin = api_openprowin(pro_buf, 328, 256, -1 , protitle);
    api_produce(prowin, protime);
}

void user_pacsta(char* pactitle)
{
	char *pac_buf;
	int pacwin;
	api_initmalloc();
	pac_buf = api_malloc(600*200);
	pacwin = api_openstawin(pac_buf, 600, 200, -1 , pactitle);
    api_pacsta(pacwin);
}

void user_end(void)
{
	api_end();
}
