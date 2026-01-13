extern void	Hprint( const int x, const int y, char *destbuf, const char *szStr );
extern void	Hprint2( const int x, const int y, char *destbuf, char *s,... );
extern void	HprintBold(const int x, const int y, const int fc, const int bc, char *s, ... );

extern void Hcolor( WORD color );
extern void Hcolor( int r, int g, int b );
extern void Getcolor( int *r, int *g, int *b );

extern void SetHangulAlign( int type );
extern void MakeCursorUpDown(char *text, int &newCurPos, bool up);
