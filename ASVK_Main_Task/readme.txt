Казантаев Алексей 213

Основное задание выполняет программа main.cpp.
 
Библиотеки, использованные в основном задании :
ctime - стандартная библиотека С++ для максимально случайных результатов
pugixml - сторонняя библиотека для работы с xml файлами

Компиляция основного задания :
cd build
cmake ..
make

Запуск основного задания (происходит в директории build) :
./ASVK_Main_Task file.xml (Имя xml файла в build или путь к нему в системе)
Пример:
./ASVK_Main_Task test128.xml

Формат входного файла :
Task - Корневой элемент
	Proc_Num - Первый подэлемент : число процессоров 
		Атрибут - num
	Top_Proc_Workload - Второй подэлемент : массив из подэлементов (на каждый процессор) верхних границ нагрузки в процентах
		Proc - Proc_Num Подэлементов второго подэлемента
			Атрибут - percent
	Prog_Num - Третий подэлемент : число программ 
		Атрибут - num
	Top_Prog_Workload - Четвертый под-элемент : массив из подэлементов (на каждую программу) нагрузок на процессоры в процентах
		Prog - Prog_Num Подэлементов четвертого подэлемента		
			Атрибут - percent
	Prog_Pairs - Пятый подэлемент : массив из элементов (на каждую программу), считается, что все элементы идут по порядку (от 1 до последней программы)
		Paired_With - Prog_Num Подэлементов пятого подэлемента : массив из элементов, каждый из которых отвечает за одну пару
			Pair - Подэлемент Paied_With
				Атрибут - pair        // номер программы, с которой образована пара у n-ного элемента Paired_With
				Атрибут - Kb        // интенсивность обмена данными по сети пары (n, pair)

К директории build прилагаю готовые 3 теста с наборами данных для 32, 64 и 128 программ (4, 8 и 16 процессоров соответственно)
_______________________________

Также я написал программу, которая создает входной файл формата xml, информация о ней в файле readme.txt директории Input_Maker.
