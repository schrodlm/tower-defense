Tower Defense
=============

##Zadání

Kromě základního zadání přímo z ProgTestu jsem se rozhodl navíc implementovat Menu, které se hráči zobrazí na začátku hry a kde si může přečíst informace o hře, měnit herní nastavení a načítat uložené hry.

Další rozšíření zadání jsou:  
	1) Efekty věží a nepřátel (smrt, upgrade, pohyb) - efekty budou vytvořeny jednoduchým ASCII artem  
	2) Rekordní hala - hala rekordů hráče a datum, kdy byly tyto rekordy zahrány  
	3) Čas jak dlouho je hráč ve hře  
	4) Při dostatku času bych se chtěl pokusit o možnost pro hráče vytvářet si vlastní mapy  

##Části programu/návrh programu
ncurses
=======
	- program bude implementován přes terminálovou knihovnu pro C++ ncurses
	- knihovna emuluje chování knihovny curses z Unixu System V Release 4.0. 
	- knihovna je kompatibilní s operačním systémem linux
Menu
====
 	
 	- 	na začátku programu uživatele čeká menu, to bude mít více hlaviček (např. "Game", "Settings",atd.) -> ty budou mít své vlastní možnosti, přičemž se v menu můžeme pohybovat šipkami
 
Mapa a její generace
====================	
	-	Mapa bude ohraničena "#" po celém okně, tuto oblast nesmí žádná entita opustit, ani se mimo ní negeneruje obsah
	-	v jednom bodu toto ohraničení nebude, tudy nepřátelé vchází a odchází
	-	Mapa se bude ukládat do vektoru vektorů charakterů, indexy těchto vektorů budou představovat x a y složky pro možnost pohybu na mapě

Entity ze souborů
===============================
	-	Entity (jejich zobrazení ve hře a jejich atributy) budou generovány z config souborů
	-	Budě několik druhů entit (věží i nepřátel), kteří se od sebe budou navzájem lišit (např. v rychlosti pohybu, v životech, v poškození atd.)  

Pohyb nepřátel na mapě
======================
	- většina nepřátel se bude na mapě pohybovat pomocí algoritmu BFS, kde budou hledat nejrychlejší možnou cestu k cíli. Někteří nepřátelé mohou ingorovat různé překážky a tedy se k cíli dostat ještě ryhcleji
	- hloupí nepřátelé, kteří nevidí na konec mapy (seeing_distance je moc malá) budou používat hloupejší algoritmus, kdy se bude snažit jen dostat na nějaký bod kde už cíl uvidí - náhodná odchylka
	
Levely
======
	- při úspěšném dokončení levelu se hráč dostane do levelu dalšího a tento postup se zapíše do nějakého log filu a uloží se, aby si mohl hráč ukládat postup. Zároveň se data z těchto log filů budou používat k implementaci rekordní haly - která bude ukládat rekorní skóre na mapě.
	- hra bude mít konečný počet levelů
	- s postupně vyššími levely by se měla i lineárně zvedat obtížnost hry a zároveň generovat nové nepřátele a překážky

Config soubory
==============
	- z config souborů se budou načítat enity a mapy - budou uloženy ve složce data
	- měly by být strukturovány určitým způsobem, aby načítání programu proběhlo spárvně
	


## Ovládání

Hráč se pohybuje v menu pomocí šipek na klávesnici, ale je zde i možnost volby menu přes počáteční písmena volby (např. do nastavení "Settings" se dostaneme přes stisk klávesy "s") 
Pro volbu specifické možnosti se stiskne "enter". 

Při hře hráč bude moci pohybovat s věžmi na mapě také pomocí šipek a enteru. Věže si může hráč také nakupovat. Tyto veže budou umístěny pod mapou a hráč se na ně dostate stiknustím tlačítka "T" (Towers). Bude moci mezi věžmi vybírat opět pomocí šipek a zakoupit věž opět pomocí entru.

## Implementace

Celá hra je implementována v terminále za použití knihovny pro C++ ncurses. 
 
 1) Menu
 		
 	- menu je realizováno přes 3 třídy -  Option, Menu, MenuBar --> MenuBar je největší třída, která obsahuje Menu - což je právě jedna možnost např. "About" - toto Menu bude obsahovat vektor Options jako je například "Author", "Levels", atd. tedy MenuBar obsahuje vektor Menu a každé menu obsahuje vektor Options, dalo by se to znázornit takto:

<pre>
+--------------------------------------------------------------------------------------+  
|                                     MenuBar                                          |       
+-------------------------------+------------------------------+-----------------------+  
|            Menu1              |         Menu2                |       Menu3           |  
+-------------------------------+------------------------------+-----------------------+  
|           Option1             |      Option1                 |      Option1          |  
|           Option2             |      Option2                 |      Option2          |  
|           Option3             |      Option3                 |      Option3          |  
+-------------------------------+------------------------------+-----------------------+	
</pre>

2) Map
	
	- Mapu generuji ze souboru ve složce data/maps/*.txt 
	- body na mapě (charaktery) mají všechny svůj PointType, který bude určovat jaké Entity mohou na tento bod na mapě vkročit
	- body si ukladám std::vector<std::vector<char>> _map; => kde index vnejšího vektoru představuje souřadnice y a index vnitřního vektoru představuje souřadnice x - mohu tak popsat jakýkoliv bod na mapě
	- přes tyto vektory také mapu zobrazuji
	
3)Entity
	- jejich atributy jsou načítany ze souboru 
	- atributy pro věže i nepřátele jsou čteny ze souboru data/entities/entites.txt
	- tyto atributy ze souboru vyberu do mapy std::map<std::string, std::string> tower_attributes, kde tower_attributes.first představuje jméno atributy a tower_attributes.second jeho hodnotu zatím zobrazenou ve stringu
	- ze souboru jsou tyto hodnoty vybírány přes využití knihovny regulárních expresí <regex> 
	




##Polymorfizmus
	
	Polymorfizmus najde v tomto programu využití na více místech. Rozhodně ho využiji na práci s Entitami například při jejich zobrazování na mapě, kde se bude hodit brát všechny věže a nepřátele právě jako Entity. Nejspíše polymorfismus využiji i při pohybu něpřátel po mapě, ale ještě si nejsem jistý, jak budu pohyb implementovat. Rozhodně bych se ještě chtěl pokusit při volbě možnosti v menu, které  mám momentálně implementované přes funkci menu_handler() a návratové hodnoty, ale pokusím se implementovat jejich funkčnost právě přes nějakou polymorfní funkci action(), která by se chovala jinak pro každou možnost.

 
