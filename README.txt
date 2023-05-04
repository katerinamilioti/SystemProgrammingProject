Aikaterini Milioti 1115201900112

Arxika, to swma tou server uparxei sto fakelo server_folder kai to swma tou client uparxei sto fakelo client_folder.Gia na trexete ton server pathste ./myserver -p -s -q -b,
opws anaferetai sthn ekfwnhsh.Antistoixa gia na trexete ton client pathste ./myclient -i -p -d, opou sto p na dinetai to ip se morfh px 127.0.0.1.Epishs,ypothetw oti o client zhtaei
fakelo o opoios yparxei sto server kai oti o server den exei adeious katalogous.Epipleon, na anaferw oti to programma to elegxa me tous fakelous server1 kai server2 pou exw mesa
sto fakelo server_folder kai thn parallhlia thn elegxa me ena sleep sthn arxh tou communication thread me duo clients. Epishs, na anaferw oti xrhsimopoihsa kwdika twn diafaneiwn
sto kommati me ta sockets kai ekana kapoies mikro allages.Telos,gia na diagrapsetai ta ektelesima myserver kai myclient pathste sudo make clean.

Epexhghsh programmatos
	server.c sto fakelo server_folder
		int *pclient = malloc(sizeof(int));
        *pclient = newsock;
		pthread_create(&t, NULL, communication_thread, pclient)
		To kommati auto to exw etsi me pointer kai den pernaw kateufeian th dieufunsh tou newsock giati otan to ekana auto mou dhmiourghfhke ena provlhma me thn parallhlia twn clients
	
	comm.c sto fakelo server_folder
		Arxika,diabazw to aithma pou exei kanei o client me ena ena byte th fora kai to vazw se ena dynamiko pinaka mexri na dei to xarakthra '/0' pou shmainei oti den exei kati allo
		na diavasei.Epeita,me th anadromikh synarthsh find_files vriskw ola ta paths twn arxeiwn pou uparxoun sto fakelo.Otan entry->d_type == 4 shmainei oti vrhke fakelo opote enwnnw 
		anadromika osous fakelous tha vrei.Epipleon,exw kai auta strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 gia na paravlephw twn torino katalogo kai 
		ton goniko katalogo tou torinou katalogou.Apo thn allh pleura otan entry->d_type == 8 shmainei oti brhke arxeio opote to enwnw anadromika kai etsi exw to path tou arxeiou,
		to opoio to vazw se mia oura fifo_copy_files.H oura ayth mou xreiasthke kai na metrhsw posa paths arxeiwn yparxoun wste na to kanw gnwsto sto client gia na mporesei meta
		na diavasei akrivws osa einai ta paths kai na mhn mplokaristei to write me to read.Afou vrethoun ola ta paths exw th synarthsh write_path sthn opoia ginetai pthread_mutex_lock(&lock);
		ayto to kanw gia na mhn ginei tautoxrono push apo allo thread.Meta jekinaei kai kanei push ena ena path apo thn oura pou anafera prin sthn oura pou einai koinh gia ola ta
		threads fifo_files kai kanei push kai to socket gia na kserei o server se poio socket na grafei kathe fora.Epeita,afou egine to push dinei shma sto wroker thread pthread_cond_signal(&w);     
		oti h oura exei sigoura ena path mesa opote mporeis na afaireseis.Meta elegxei ean ta stoixeia pou exoun mpei sthn oura einai isa me to megisto megethos ths ouras.Ama einai isa
		shmainei oti to communication thread prepei na perimenei mexri na ginei ena pop apo thn oura apo kapoio worker thread. Auto ginetai mesw ths entolhs pthread_cond_wait(&c, &lock);  
		Telos,afou ginoun ola ayta ginetai unlock tou mutex  pthread_mutex_unlock(&lock); kai etsi mporei na paei allo thread na grapsei sthn oura.
	
	worker.c sto fakelo server_folder
		Arxika to worker thread prepei na perimenei mexri na gemisei h oura estw kai me ena stoixeio. Auto ginetai me thn entolh while(fifo_files_size == 0)
																																{
																																	pthread_cond_wait(&w, &lock);         
																																}
		Otan to fifo_files_size einai 0 shmainei oti h oura einai adeia tha ekshghsw parakatw.Otan telika gemisei h oyra ginetai pop kai stelnetai shma sto communication thread
		oti twra mporei na paei na kanei push sthn oura  pthread_cond_signal(&c);. Epishw ginetai kai pop tou socket opote etsi o server kserei se poio socket thaa grafei kathe fora.
		Opote twra to worker thread tha diavasei ti exei mesa to arxeio ana block.Arxika ypologizei to megethow tou arxeiou.Epishsw,exei kai mia metavlhth  int curr_size = 0;
		h opoia tha metraei posa bytes apo blocks exoun diavastei th fora.Ena paradeigma einai estw oti thelw na diavasv 13 bytes me block_size = 5.Tote curr_size  = 5 diavazei
		ayta ta 5 5 < 13 opote synexizei curr_size = 10 diavazei ta alla 5 byte 10 <13 opote synexizei curr_size= 15 diavazei ta alla 3 bytes 15 >=13 opote stamataei.Kathe block 
		kathe fora mpainei se mia oura.Ayto me vohtthse gia na kanw gnwsto sto client posa blocks uparxoun.
		
	fifos.c sto fakelo server_folder
		Arxika, na anaferw oti fifo_files h koinh oura gia ola ta threads,fifo_sockets h oura pou periexei ta sockets , fifo_copy_files h oura poy periexei ta paths twn arxeiwn
		pou tha vrei o server kai fifo_blocks h oura pou periexei to periexomeno ton blocks pou diabase.H idea ths push einai exw ena size to opoio einai arxika 0 opote otan einai 0
		shmainei oti tha mpei to prwto stoixeio opote ftiaxnw ena pinaka me mia thesh kai vazw mesa sto string mou.An den einai ena allazw to megethos tou pinaka analoga me to size
		kai vazw mesa to string.Exw kai enan allo pinaka o opoios tha mou xreiastei gia to pop.Sto pop twra afairw to prvto stoixeio tou pinaka opote exw to First in first out
		Epeita antigraphw oti stoixeia exei o pinakas se enan allo pinaka.Meiwnw to megethow tou pinaka kai twr afou efyge to prwto stoixeio ola ta stoixeia prepei na erthoun mia
		thesh mprosta se ayto me vohfaei o new pinakaw pou exw.
		
	client.c sto fakelo client_folder
		H synarthsh makeDir einai h synarthsh pou antigrafei to fakelo.Arxika spaei to path se tokens px an exei to path server/folder1/file1 tha finei server,folder1,file1 kai
		metraei posa tokens exei.Epeita ftiaxnei ena fakelo me to prwto token pou einai kai to to onoma tou fakelou pou exei kanei request o client.Meta enwnei kathe fora ena ena
		token me to / kai to epomeno tou kai ftiaxnei ton antistoixo fakelo h arxeio.An to token den einai to teleutaio shmainei oti einai fakelos opote ftiaxnei to fakelo,an omws
		einai to teleutaio shmainei oti einai arxeio , opote ftiaxnei to arxeio,to opoio elegxei prwta an uparxei.An uparxei to svhnei me thn sunarhths remove.Epeita afou einai 
		sto arxeio diabazei kathe fora ta blocks apo ton server kai ta graphei sto arxeio.
	
	
	
