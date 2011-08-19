#!/bin/sh

EMAIL=email@domain.org

if [ `basename $1` == "echat-msg.log" ]; then
	if [ `echo $2 | grep "message from" | wc -l` -ne 0 ]; then
		echo -n 1 >~/echat-sms.dat
		exit 0
	fi
	if [ `cat ~/echat-sms.dat` -eq 1 ]; then
		DATE=`date "+%Y.%m.%d.%H.%M.%S"`
		tail -n -4 $1 | perl -ne " \
			s/\[.*\] message from//g; s/\[.*\] autoanswer from .*//g; \
	    	s/\[.*\] message to/2/g; \
		    s/á/A/g; s/â/B/g; s/÷/W/g; s/ç/G/g; s/ä/D/g; s/å/E/g; s/³/YO/g; s/ö/ZH/g; \
		    s/ú/Z/g; s/é/I/g; s/ê/J/g; s/ë/K/g; s/ì/L/g; s/í/M/g; s/î/N/g; s/ï/O/g; \
		    s/ð/P/g; s/ò/R/g; s/ó/S/g; s/ô/T/g; s/õ/U/g; s/æ/F/g; s/è/KH/g; s/ã/C/g; \
		    s/þ/CH/g; s/û/SH/g; s/ý/SCH/g; s/ø//g; s/ù/Y/g; s/ÿ//g; s/ü/E/g; s/à/YU/g; \
		    s/ñ/YA/g; s/Á/a/g; s/Â/b/g; s/×/w/g; s/Ç/g/g; s/Ä/d/g; s/Å/e/g; s/£/yo/g; \
		    s/Ö/zh/g; s/Ú/z/g; s/É/i/g; s/Ê/j/g; s/Ë/k/g; s/Ì/l/g; s/Í/m/g; s/Î/n/g; \
		    s/Ï/o/g; s/Ð/p/g; s/Ò/r/g; s/Ó/s/g; s/Ô/t/g; s/Õ/u/g; s/Æ/f/g; s/È/kh/g; \
		    s/Ã/c/g; s/Þ/ch/g; s/Û/sh/g; s/Ý/sch/g; s/Ø//g; s/Ù/y/g; s/ß//g; s/Ü/e/g; \
		    s/À/yu/g; s/Ñ/ya/g; print; \
			" | mail -s "echat-$DATE" $EMAIL >>~/echat-sms.log 2>>~/echat-sms.log
		echo -n 0 >~/echat-sms.dat
	fi
fi
