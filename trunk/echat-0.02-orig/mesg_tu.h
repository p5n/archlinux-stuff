/*  mesg_tu.h
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifndef _MESG_H
#define _MESG_H 1

#define CHNL_OUT        "[%s] Kanal Disi %s\n"
#define CHNL_JOIN       "[%s] %s Kanala Girdi %s.\n"
#define CHNL_LEAVE      "[%s] %s Kanaldan Ayrildi %s.\n"
#define CHNL_JOINP      "[%s] %s �zele Geldi.\n"
#define CHNL_LEAVEP     "[%s] %s �zelden Ayrildi.\n"

#define CMND_HQUIT      "[%s] %s(%s,%s): Ayrildi %s.\n"
#define CMND_HHELP      "[%s] %s(%s,%s) [<komut>]: Komut Kullanimi.\n"
#define CMND_HVERSION   "[%s] %s(%s): Versiyonu G�ster.\n"
#define CMND_HSTATUS    "[%s] %s(%s): show current status.\n"
#define CMND_HKEYS      "[%s] %s(%s): Islevi Olan Tuslar.\n"
#define CMND_HVERBOSE   "[%s] %s(%s): Ayrinti Dugmesi.\n"
#define CMND_HCLEAR     "[%s] %s(%s): Komut Ve Ekran Gecmisini Sil.\n"
#define CMND_HSHELL     "[%s] %s [<komut>]: Kabuk Komutu.\n"
#define CMND_HLOAD      "[%s] %s [<Dosya Adi>]: Konfigurasyon Dosyasi Yukle.\n"
#define CMND_HSAVE      "[%s] %s [<Dosya Adi>]: Konfigurasyon Dosyasini Sakla.\n"
#define CMND_HANSWER    "[%s] %s(%s) [<Yeni oto-cevap>]: show/change current autoanswer.\n"
#define CMND_HGENDER    "[%s] %s(%s): Cinsiyet.\n"
#define CMND_HMY        "[%s] %s: Ses Ayarini Degistir.\n"
#define CMND_HFAVORITE  "[%s] %s(%s) [<nick1> <nick2> ...]: show/favorite/unfavorite nick(s).\n"
#define CMND_HIGNORE    "[%s] %s(%s) [<nick1> <nick2> ...]: show/ignore/unignore nick(s).\n"
#define CMND_HBAN       "[%s] %s [<nick1> <nick2> ...]: show/ban/unban address(es).\n"
#define CMND_HJOIN      "[%s] %s(%s) <kanal>: Kanala Gir.\n"
#define CMND_HLEAVE     "[%s] %s(%s) [<kanal>]: Kanaldan Ayril.\n"
#define CMND_HNICK      "[%s] %s(%s) <Yeni Nick>: Nick Degistir.\n"
#define CMND_HMESG      "[%s] %s(%s,%s) <Nick> <Mesg>: Nick'e Mesaj Gonder.\n"
#define CMND_HMASS      "[%s] %s <Mesg>: Ortak Mesaj Gonder.\n"
#define CMND_HEXEC      "[%s] %s <Nick> <Komut>: Komut Calistir.\n"
#define CMND_HME        "[%s] %s [<Aksiyon>]: me Aksiyonu.\n"
#define CMND_HTOPIC     "[%s] %s(%s) [<Yeni Konu>]: show/change current topic.\n"
#define CMND_HADD       "[%s] %s [<Konuya Ekle>]: Simdiki Konuya Ekle.\n"
#define CMND_HMODE      "[%s] %s(%s) [<0-4>]: Kullanici Durumunu Degistir.\n"
#define CMND_HNORMAL    "[%s] %s(%s): Durumu Normal Olarak Degistir .\n"
#define CMND_HDND       "[%s] %s(%s) [<Yeni oto-cevap>]: Durumu DND olarak Degistir.\n"
#define CMND_HAWAY      "[%s] %s(%s) [<Yeni oto-cevap>]: Durumu AWAY olarak Degistir.\n"
#define CMND_HOFF       "[%s] %s(%s) [<yeni oto-cevap>]: Durumu OffLine olarak Degistir.\n"
#define CMND_HINFO      "[%s] %s(%s,%s) [<nick1> <nick2> ...]: Nick(ler) Hakkinda Bilgi Al.\n"
#define CMND_HBEEP      "[%s] %s(%s) [<nick1> <nick2> ...]: Nick(lere) Beep G�nder.\n"
#define CMND_HTALK      "[%s] %s(%s,%s) <nick>: Nick ile �zel Sohbet.\n"
#define CMND_HHERE      "[%s] %s: Kim Var?.\n"
#define CMND_HACTIVE    "[%s] %s: Su Andaki Aktiviteyi Degistir.\n"
#define CMND_HCOMMANDS  "[%s] Kullanilabilir Komutlar:\n"
#define CMND_HKEYWORK   "[%s] Calisan Tuslar:\n"
#define CMND_HKEY1      "F1-8 - Kanal Degistir\n"
#define CMND_HKEY2      "^O veya TAB - Sonraki Kanal\n"
#define CMND_HKEY3      "^D - Sohbetten �ik\n"
#define CMND_HKEY4      "^A - Aktiviteyi Degistir\n"
#define CMND_HKEY5      "^K - Kullanicilari Geri Getir\n"
#define CMND_HKEY6      "^L veya INS - Listeyi Guncelle\n"
#define CMND_HKEY7      "^R - Ekrani Tazele\n"
#define CMND_HKEY8      "^T - Sayfa Gecmisinin En �stune\n"
#define CMND_HKEY9      "^B - Sayfa Gecmisinin En Altina\n"
#define CMND_HKEY0      "^P veya PGUP - Sayfayi Yukari Kaydir\n"
#define CMND_HKEYA      "^N veya PGDN - Sayfayi Asagi Kaydir\n"
#ifdef  CHARSET
# define CMND_HKEYB     "^E - Satiri K�rs�rden Sonra Yeniden Kodla\n"
#endif  /* CHARSET */
#define CMND_HKEYC      "'`' , [1-9] - Giris Satirina Nick Koy #[1-9]\n"
#define CMND_HKEYD      "'/' , [1-9] - Nicke Mesaj At #[1-9]\n"
#define CMND_VERSION    "[%s] %s v%s.\n"
#define CMND_STATUS     "[%s] currernt status:\n"
#define CMND_STATUSA    "addr: %s, port: %u, proto: %s\n"
#define CMND_STATUSL    "clients connected: %d\n"
#define CMND_VERBOSEON  "[%s] Bildirme Acik.\n"
#define CMND_VERBOSEOFF "[%s] Bildirme Kapali.\n"
#define CMND_CLEAR      "[%s] Kanal Temizlendi.\n"
#ifdef  SHELL
# define CMND_SHELL     "[%s] Kabuk Komutu:\n"
# define CMND_SHELLE    "[%s] shell command failed.\n"
#else   /* SHELL */
# define CMND_SHELLD    "[%s] /! Komut iptal Edildi.\n"
#endif  /* SHELL */
#define CMND_LOAD       "[%s] Yuklenen Konfigurasyon Dosyasi:\n"
#define CMND_LOADF      "[%s] Konfigurasyon Yukleme Durduruldu.\n"
#define CMND_SAVE       "[%s] Konfigurasyon Kaydedildi:\n"
#define CMND_SAVEF      "[%s] Konfigurasyon Kaydetme Durduruldu.\n"
#define CMND_ANSDND     "[%s] DND oto-cevap:\n"
#define CMND_ANSAWAY    "[%s] AWAY oto-cevap:\n"
#define CMND_ANSOFF     "[%s] OffLine oto-cevap:\n"
#define CMND_GENDERM    "[%s] Erkek.\n"
#define CMND_GENDERF    "[%s] Bayan.\n"
#define CMND_BEEPALL    "[%s] T�m Beep'leri �al.\n"
#define CMND_BEEPCUR    "[%s] Sadece Simdiki Beep'i Cal.\n"
#define CMND_FAVORITEN  "[%s] Favori Nickler:\n"
#define CMND_FAVORITEI  "[%s] %s Favorilere Eklendi.\n"
#define CMND_FAVORITEU  "[%s] %s Favorilerden Cikarildi.\n"
#define CMND_IGNOREN    "[%s] Engellenen Nickler:\n"
#define CMND_IGNOREI    "[%s] %s Engellendi.\n"
#define CMND_IGNOREU    "[%s] %s Engellenenlerden Silindi.\n"
#define CMND_BANA       "[%s] Ban Edilen Adresler:\n"
#define CMND_BANB       "[%s] Adres %s Banlendi.\n"
#define CMND_BANU       "[%s] Adres %s 'nin Bani Kaldirildi.\n"
#define CMND_INICK      "[%s] Gecersiz Nick %s\n"
#define CMND_ICHANNEL   "[%s] Gecersiz Kanal %s\n"
#define CMND_LEAVE      "[%s] Kanaldan Ayrilamiyor %s\n"
#define CMND_NICK       "[%s] %s Nicki Zaten Var.\n"
#define CMND_MESG       "[%s] message to %s:\n"
#define CMND_MASS       "[%s] mass message:\n"
#define CMND_QTOPIC     "[%s] SuAnki Konu:\n"
#define CMND_TOPIC      "[%s] SuAnki Konu %s:\n"
#define CMND_UNKNOWN    "[%s] Bilinmeyen Komut: %s\n"

#define ECHAT_SIGNAL    "[%s] Sinyal %d Alindi, Simdi Cikiliyor.\n"
#define ECHAT_BYE       "[%s] Hoscakalin, %s.\n"
#define ECHAT_WELCOME   "[%s] %s v%s, %s Hosgeldiniz.\n"

#define PARS_NICK       "[%s] %s Nickini %s Olarak Degistirdi.\n"
#define PARS_MESG       "[%s] %s 'dan Mesaj:\n"
#define PARS_MASS       "[%s] %s 'dan Ortak Mesaj:\n"
#define PARS_MESGACK    "[%s] %s  Mesaji Aldi\n"
#define PARS_MESGANS    "[%s] %s 'dan oto-cevap:\n"
#define PARS_EXEC       "[%s] %s 'dan Calistir:\n"
#define PARS_EXECACK    "[%s] %s 'Calistir Sonuclari:\n"
#define PARS_QTOPIC     "[%s] Yeni Konu:\n"
#define PARS_TOPIC      "[%s] Yeni Konu %s:\n"
#define PARS_QTOPICC    "[%s] Simdiki Konu:\n"
#define PARS_TOPICC     "[%s] Simdiki Konu %s:\n"
#define PARS_MODE       "[%s] %s Girdi "
#define PARS_MODENORMAL "Normal Modda.\n"
#define PARS_MODEDND    "DND Modda:\n"
#define PARS_MODEAWAY   "Away Modda:\n"
#define PARS_MODEOFF    "Offline Modda:\n"
#define PARS_INFO       "[%s] %s info'ya Bakti.\n"
#define PARS_INFORM     "[%s] %s 'nin infosu:\n"
#define PARS_QINFOUH    "Kullanici: %s, Makine: %s [%s], Mem: %s\n"
#define PARS_INFOUH     "Kullanici: %s, Makine: %s [%s]\n"
#define PARS_INFOCH     "Kanallar: %s\n"
#define PARS_BEEP       "[%s] %s Beep G�nderdi.\n"
#define PARS_BEEPACK    "[%s] %s Beep Aldi.\n"
#define PARS_HERE       "[%s] Buradakiler %s (%s).\n"
#define PARS_HEREACT    "actif"
#define PARS_HERENAC    "aktif degil"

#define SOCK_ALLOC      "[%s] socket Atama BAsarisiz.\n"
#define SOCK_ROUTE      "[%s] route Durduruldu to %s\n"
#define SOCK_RECV       "[%s] receive failed from %s\n"
#define SOCK_ADDRESS    "[%s] %s i�in Gecersiz Adres: %s\n"
#define SOCK_USEND      "[%s] unicast Gonderimi Basarisiz %s\n"
#define SOCK_BSEND      "[%s] broadcast Gonderimi Basarisiz %s\n"

#define SOCK_TCONN      "[%s] client connected.\n"
#define SOCK_TWELCOME   "Welcome to eChat.\n"
#define SOCK_TOUT       "Out of sockets.\n"

#define CMND_SCLEAR     "Kanal Temizlendi."
#define CMND_SLOAD      "Konfigurasyon Yuklendi."
#define CMND_SLOADF     "Konfigurasyon Yukleme Durduruldu ."
#define CMND_SSAVE      "Konfigurasyon Kaydedildi."
#define CMND_SSAVEF     "Konfigurasyon Kaydetme Durduruldu."
#define CMND_SGENDERM   "Erkek."
#define CMND_SGENDERF   "Bayan."
#define CMND_SBEEPALL   "Tum Beepleri Cal."
#define CMND_SBEEPCUR   "Sadece Simdiki Beepleri Cal."
#define CMND_SUNKNOWN   "Bilinmeyen Komut."

#define TERM_SQUIT      "Ayril."

#define MAKE_SCHAT      "Sohbet String'i G�nderildi."
#define MAKE_SNICK      "Nick Degistirildi."
#define MAKE_SMESG      "Mesaj G�nderildi."
#define MAKE_SMASS      "Ortak Mesaj G�nderildi."
#define MAKE_SME        "Me String'i G�nderildi."
#define MAKE_STOPIC     "Yeni Konu G�nderildi."
#define MAKE_SMODE      "Mod Degistirildi."
#define MAKE_SINFO      "Bilgi G�nderildi."
#define MAKE_SBEEP      "Beep G�nderildi."
#define MAKE_SPRIVJOIN  "�zel Sohbet Basladi."
#define MAKE_SPRIVLEAVE "�zel Sohbet Kapandi."
#define MAKE_SPRIVCHAT  "�zel Sohbet String'i G�nderildi."
#define MAKE_SPRIVME    "�zel ME String'i G�nderildi."
#define MAKE_SHERE      "'Buradakiler' G�nderildi."
#define MAKE_SACTIVE    "AKtivite Degistirildi."

#define PARS_SHERE      "Kim Burada? Alindi."
#define PARS_SBEEPREQ   "Mesaj \"beepreq\" Alindi."

#define SOCK_SROUTE     "Route Durduruldu."
#define SOCK_SRECEIVE   "Alinma Durduruldu."
#define SOCK_SUSEND     "Unicast G�nderme Durduruldu."
#define SOCK_SBSEND     "Broadcast G�nderme Durduruldu."

#endif  /* _MESG_H */