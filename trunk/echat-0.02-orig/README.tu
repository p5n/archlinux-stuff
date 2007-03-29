#  eChat v0.02

    eChat Turkce BeniOku yardim ve tanitim dosyasi..

    <copyright (c) 2000 SeaD>

    bu yazilimi cogaltma bilgileri icin GPL dosyasina gozatiniz..


##  eChat nedir?

    eChat tcp/ip networkunuzde ; server'siz (sunucusuz) birsekilde mesajlasabilmenize olanak saglayan 
    bir yazilimdir.

    Windows9x/NT/2000 networklerinde IRC ortaminda mesajlasmaniza olanak saglayan 
    Vypress Chat ve quickChat yazilimlari ile uyumludur..


##  kurulum :

    programi download edeceginiz adres: ftp.vypress.com/clones/vyc

    Makefile dosyasini sisteminiz icin duzenleyin
    Not: ncurses kutuphanesine ihtiyaciniz olacak..
	  (ftp://ftp.clark.net/pub/dickey/ncurses)
    ve bu komutlari uygulayin;
    make
    make install

    cp /usr/local/doc/.echatrc.sample ~/.echatrc

    edit ~/.echatrc

    hepsi bu!
    not: .echatrc dosyasini duzenlemek zorunda degilsiniz.. fakat programi her acisinizda ayni 
    ayarlari (nick'iniz,mod'unuz,ignore listeniz...)bastan yapmak istemiyorsaniz duzenlemeniz daha akilci olacaktir

##  kullanim :

    klavye:

	F1-F8 - kanal degistirme
	^O or TAB - sonraki kanal
	^D - programdan cikis
	^A - aktivite degistirme
	^L veya INS - kullanicilari guncelleme
	^R - ekrani yenileme
	^T - ekranin en basina..
	^B - ekranin en altina..
	^P or PGUP - bir sayfa yukari
	^N or PGDN - bir sayfa asagi
        '`' , [1-9] - nick yaz #[1-9] sohbet satirina:
	    `1 - birinci nick tarafindan karsilanacak,
	    `2 - ikinci nick tarafindan karsilanacak, v.s.
	'/' , [1-9] - nicke mesaj #[1-9]:
	    /1 - ilk nicke mesaj,
	    /2 - ikinci nicke mesaj, v.s.

    kullanilabilir komutlar:

	quit - eChat'den cikis
	help - komutlar hakkinda yardim verir
	version - show version
	status - show current status
	keys - kisayol tuslarini gosterir
	verbose - verbosity acar/kapatir
	clear - komut ve text hafizasini bosaltir
	! - shell command
	load - konfigurasyon dosyasini yeniden yukler
	save - konfigurasyon dosyasini `~/.echatrc' olarak kaydeder
	answer - show/change current autoanswer
	gender - cinsiyet belirtir
	my - beep mod acik/kapali
	favorite - show/favorite/unfavorite nick(s)
	ignore - show/ignore/unignore nick(s)
	ban - show/ban/unban address(es)
	join - join kanal
	leave - leave kanal
	nick - nick degistirir
	message - nick sahibine ozel mesaj atar
	mass - coklu mesaj atar
	exec - bir komut calistirir
	me - me aksiyon
	topic - show/change current topic
	add - oncekini silmeden sonuna ekleme yapar
	mode - kullanici mod'unuzu degistirir
	dnd - kullanici modunuzu "dnd" yapar
	away - kullanici modunuzu "away" yapar
	off - kullanici modunuzu "offline" yapar
	info - nickler hakkinda bilgi verir
	beep - nick(ler)e beep gonderir
	talk - nick sahibiyle ozel chat
	here - kimler online
	active - halihazirdaki aktiviteyi degistirir

    kullanici modlari menusu:
	+ - normal modu (normal)
	- - dnd modu (rahatsiz etmeyin)
	= - away modu (bilgisayar basinda degil)
	* - offline modu (kapali)

##  calistigi platformlar

    eChat ;
    Linux kernel 2.2.15, glibc-2.1.2, gcc-2.95.2, ncurses-4.2
    uzerinde gelistirilmistir..

    Debian GNU/Linux 2.1
    RedHat Linux 5.2
    Corel Linux 1.0
    Mandrake Linux 8.0 
    FreeBSD kernel 4.0-RELEASE-1, gcc-2.95.2, ncurces-3.1

    sistemlerde derlenip calistirilmistir..

    baska platformlarda calistirabilenlerden bilgi almaya haziriz..


##  hazirlayanlar

    VyPRESS gelistirme, LLC - for nice program
    Sergey Barkov <sergey@vypress.com> - Teknik Destek icin
    Eugene E. Zaitsev <eugene@zaitsev.org> - FreeBSD 'ye port edilis
    sibkot <sibkot@mail.ru> - linux icin vypress chat yazari,tum FreezNet (test asistanligi icin)
    ve kisisel olarak Andy ve Helen
   


##  bug'lar 

    rastladiginiz bug'lari SeaD <seadz@mail.ru> adresine rapor ediniz..

<Mos>

SeaD <seadz@mail.ru>


