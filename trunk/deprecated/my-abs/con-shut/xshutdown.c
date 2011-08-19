/*
 * Программа спрашивает, что делать : перезагрузка, остановка или отмена.
 * (Для подвешивания на Ctrl+Alt+Del)
 * (c) 2000, Пупыкин С.Н.
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <linux/vt.h>

#include <curses.h>

int con_open(int nFlags)
{
	int nRes;

	nRes = open("/dev/console",nFlags);
	if(nRes<0)
	{
		nRes = open("/dev/tty",nFlags);
	}
	return nRes;
}

struct console_state_t
{
	ushort v_active;
	ushort v_signal;
	ushort v_state;
};

#define PERM 0666
#define MAGIC_NUMBER	0x12345678

typedef struct tag_tSharedMem
{
	int nMagic;
	int nConsole;
	char pad[256];
} tSharedMem;

char ShowMessage(void)
{
	WINDOW *prWindow;
	int nRes;

	prWindow = newwin(10,40,5,20);
	mvwprintw(prWindow,2,14,"Ctrl+Alt+Del");
	mvwprintw(prWindow,4,5,"Reboot(R) / Halt(H) / Cancel(C)");
	box(prWindow,0,0);
	refresh();
	wrefresh(prWindow);
	doupdate();
	while((nRes=wgetch(prWindow))==ERR)
	{
	}
	delwin(prWindow);
	return (char)(nRes & 0xFF);
}

int main(int nArgc,char *apszArgv[])
{
	int nConsole;
	int fd_tty;
	int fd_my_tty_in;
	int fd_my_tty_out;
	char szTTY[32];
	struct console_state_t console_state;
	char cAnswer;
	key_t key;
	int nShmID;
	tSharedMem * prSharedMem;

	//
	// Завершение родительского процесса
	//
	if(fork())
	{
		goto out;
	}

	//
	// Проверить наличие уже запущенного процесса
	//
	if((key = ftok(apszArgv[0],'A'))<0)
	{
		goto out;
	}
	if( (nShmID=shmget(key,sizeof(tSharedMem),0))>=0 )
	{
		//
		// Запущенная копия найдена
		//
		if( (prSharedMem=(tSharedMem*)shmat(nShmID,0,0))<0 )
		{
			goto out2;
		}
		if(prSharedMem->nMagic != MAGIC_NUMBER)
		{
			goto out_mem2;
		}
		fd_tty = con_open(O_RDONLY);
		if(fd_tty<0)
		{
			goto out_mem2;
		}
		ioctl(fd_tty,VT_ACTIVATE,prSharedMem->nConsole);
		close(fd_tty);
out_mem2:
		shmdt(prSharedMem);
out2:
		return 0;
	}
	//
	// Процесс еще не запущен
	//
	if((nShmID=shmget(key,sizeof(tSharedMem),IPC_CREAT | PERM))<0)
	{
		goto out;
	}
	if( (prSharedMem=(tSharedMem*)shmat(nShmID,0,0))<0 )
	{
		goto out_mem_id;
	}
	prSharedMem->nMagic = MAGIC_NUMBER;

	setsid();

	//
	// Получение информации о консолях
	//
	fd_tty = con_open(O_RDONLY);
	if(fd_tty<0)
	{
		goto out_mem;
	}

	if(ioctl(fd_tty,VT_OPENQRY,&nConsole)<0)
	{
		goto out_console;
	}
	prSharedMem->nConsole = nConsole;

	if(ioctl(fd_tty,VT_GETSTATE,&console_state)<0)
	{
		goto out_console;
	}

	if(ioctl(fd_tty,VT_ACTIVATE,nConsole)<0)
	{
		goto out_console;
	}

	//
	// Открытие свободной консоли
	//
	sprintf(szTTY,"/dev/tty%d",nConsole);
	fd_my_tty_in = open(szTTY,O_RDONLY);
	fd_my_tty_out = open(szTTY,O_WRONLY);

	close(0);
	close(1);
	close(2);

	dup2(fd_my_tty_in,0);
	dup2(fd_my_tty_out,1);
	dup2(fd_my_tty_out,2);

	//
	// Работа ...
	//
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);

	cAnswer = ShowMessage();

	clear();
	refresh();
	doupdate();

	endwin();

	switch(cAnswer)
	{
		case 'R':
		case 'r':
		case 'К':
		case 'к':
			printf("Rebooting NOW\n\n");
			system("/sbin/shutdown -t3 -r now");
			goto out_console;
		case 'H':
		case 'h':
		case 'Р':
		case 'р':
			printf("Shuting down NOW\n\n");
			system("/sbin/shutdown -t3 -h now");
			goto out_console;
		case 'C':
		case 'c':
		case 'С':
		case 'с':
			printf("Shutdown Canceled\n\n");
			break;
		default:
			printf("Shutdown Canceled via invalid input\n\n");
			break;
	}
	//
	// ////////////////////////////////////////////////////////////////////////
	//

	//
	// Закрытие консоли
	//
	close(fd_my_tty_out);
	close(fd_my_tty_in);
	close(0);
	close(1);
	close(2);

	//
	// Возвратить на место старую консоль
	//
	ioctl(fd_tty,VT_ACTIVATE,(int)console_state.v_active);
	//
	// Удалить консоль
	//
	ioctl(fd_tty,VT_DISALLOCATE,nConsole);

out_console:
	close(fd_tty);
out_mem:
	shmdt(prSharedMem);
out_mem_id:
	shmctl(nShmID, IPC_RMID, 0);
out:
	return 0;
}
