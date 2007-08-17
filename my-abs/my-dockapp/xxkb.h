#define XT_RESOURCE_SEARCH

#define MAX_GROUP	4
#define APPDEFFILE	"XXkb"
#define USERDEFFILE	".xxkbrc"

#define	When_create	(1<<0)
#define	When_change	(1<<1)
#define	Focus_out	(1<<2)
#define	Two_state	(1<<3)
#define	Button_enable	(1<<4)
#define	Main_enable	(1<<5)
#define	WMaker		(1<<6)
#define	Button_delete	(1<<7)
#define	When_start	(1<<8)
#define	Bell_enable	(1<<9)
#define	Ignore_reverse	(1<<10)
#define	Main_delete	(1<<11)

#define	But1_reverse	(1<<12)
#define	But3_reverse	(1<<13)

#define	Forget_window	(1<<14)

typedef struct {
	int	mask;
	int	x,y;
	unsigned int width, height;
	int	gravity;
} Geometry;

typedef enum { T_string, T_bool, T_int } ResType;
typedef enum { WMClassClass, WMClassName, WMName, Prop} MatchType;
typedef int  ListAction;

#define GrpMask    (0x3)
#define AltGrp     (1<<2)
#define InitAltGrp (1<<3)
#define Ignore     (1<<4)

typedef struct _SearchList {
        struct _SearchList *next;
	ListAction	action;
	MatchType	type;
	int		num;
	char		**idx;
	char		*list;
} SearchList;

typedef struct {
	unsigned int controls;
	int          Base_group, Alt_group;
	int          Bell_percent;
        Geometry     main_geom, but_geom;
        SearchList*  lists[4];
        char*        user_config;
        char*        tray_type;
	XImage*      pictures[2*MAX_GROUP];
} XXkbConfig;

void GetConfig(Display *, XXkbConfig *);
void getGC(Window, GC *);
void update_window(Window w, GC gc, int group);
void update_button(Window w, GC gc, int group);
void Reset(void);
void Terminate(void);

Bool ExpectInput(Window win);
void ErrHandler(Display* dpy, XErrorEvent* err);

void   GetAppWindow(Window w, Window* app);
Window GetGrandParent(Window w);
Window MakeButton(Window parent);

char* GetAppIdent(Window w, MatchType type);
void AddAppToConfig(Window win, unsigned int state);
void SaveAppInConfig(XXkbConfig *conf, char* name, MatchType type);
