
Tower Defense
=============

Projekt jako semestrální práce předmětu PA2 - vytvoření enginu, který bude fungovat jako real-time hra
v prostředí termínálu za pomocí knihovny ncurses a programovacího jazyk C++


Zadání z Progtestu
------------------

Naprogramujte jendoduchou grafickou tower defense hru

Váš engine:
-----------

    - ze souboru nahraje definici věží (cena, velikost, útok, dosah, ...)
    - ze souboru nahraje možné mapy a typy nepřátel (životy, rychlost, odolnost na určitý typ věže ,...)
    - implementuje jednoduchou interakci věž vs. útočníci (útok, proběhnutí, ...), počitadlo skóre, detekci vítězství (po nezabití x útočníků)
    - implementuje alespoň 2 strategie průchodu hracím polem
    - umožňuje ukládat a načítat rozehrané hry

	!Engine může fungovat jako real-time hra, či tahová hra.!

	Cesta kudy se budu útočníci ubírat bude vždy nejkratší možná vzhledem ke zdím a věžím. "Nejkratší" se může lišit pro létající (ignorují zdi) a pozemní jednotky. Pro chytré jednotky, 				
	které se vyhýbají věžím. Pro jednotky s krátkým dohledem (k pohybu využívají jen informaci o okolí 5ti polí a cíl).

Možné využití polymorfismu
---------------------


    Parametry věží: znak, barva, dosah, zranění, ...
    Efekty útoku věží: zranění, zpomalení, ...
    Políčka mapy: volno, věž, útočník ...
    Strategie hledání cesty: předdefinovaná cesta, nejkratší (BFS, případně náhodná odchylka), A* prohledávání, vyhýbání se věžím
    Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ...


Moje rozšíření zadání
----------------
Implementace
	-	celý program implementuji přes knihovny ncurses v prostředí terminálu
	-	funkce z této knihovny nebudu popisovat, protože jsou již zdokumentované
	


Menu
 	
 	- 	na začátku programu uživatele čeká menu, to bude mít více hlaviček např. "Game", "Settings",atd. -> ty budou mít své vlastní možnosti, přičemž 
		se v menu můžeme pohybovat šipkami
	
	-	pro zvolení možnosti danou možnost vybereme a stiskneme "Enter", některé možnosti nemusí opouštět menu, jako například změna obtížnosti, za to některé jako "New Game"
		musí Menu opustit a pracovat mimo
		
	- 	Menu by mělo být realizováno pomocí tříd MenuBar, Menu, Options)
	
Další rozšíření zadání jsou:  
	1) Efekty věží a nepřátel (smrt, upgrade, pohyb) - efekty budou vytvořeny jednoduchým ASCII artem  
	2) Rekordní hala - hala rekordů hráče a datum, kdy byly tyto rekordy zahrány  
	3) Čas jak dlouho je hráč ve hře  
	4) Při dostatku času bych se chtěl pokusit o možnost pro hráče vytvářet si vlastní mapy  



