/* 11C. Результатом должна быть консольная программа с текстовым меню,
выполняющая все перечисленные ниже пункты (1-8) для контейнера список (std::list)
и преобразования 11.

11. Разделить все числа на половину максимального из чисел.*/


#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>    // std::generate
#include <iterator>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <cstdio>
#include <cstring>
#include <list>
using namespace std;

//генерация случайного числа
struct random_gen
{
	int m; //диапозон чисел [-m;m]

	random_gen(int mn) : m(mn) {}

	int operator() ()
	{
		return rand() % (2 * m + 1) - m;
	}
};

/*функция заполнения текстового файла N целыми случайными числами 
в диапазоне от - M до M. Имя файла, N и M поступают на вход функции, 
файл возвращается функцией как результат;
1 вариант. Генерация чисел в цикле. */
FILE* create_file_cycle(char name[], int n, int m)
{
	FILE *file;
	file = fopen(name, "w+");
	fstream outfile(file);

	if (outfile)
	{
		random_gen random(m);
		for (int i = 0; i < n; ++i)
			outfile << random() << " ";
		cout << "Файл создан." << endl;
	}
	else
	{
		cout << "Файл не удалось открыть." << endl;
	}

	return file;
}

//2 вариант. Генерация чисел с использованием алгоритма std::generate
FILE* create_file_generate(char name[], int n, int m)
{
	FILE *file;
	file = fopen(name, "w+");
	fstream outfile(file);

	list<int> my_list(n);

	if (outfile)
	{
		//генерирование случайных чисел
		generate(my_list.begin(), my_list.end(), random_gen(m));
		//запись чисел в файл
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(outfile, " "));
		cout << "Файл создан." << endl;
	}
	else
	{
		cout << "Файл не удалось открыть." << endl;
	}

	return file;
}

/*функция, получающая файл на вход, и возвращающая последовательный контейнер,
заполненный числами из файла*/
list<int> load_from_file(FILE *file)
{
	int a;

	ifstream infile(file);
	infile.seekg(0);

	list<int> my_list;
	if (infile)
	{
		//пока можно считать число
		while (!infile.eof())
		{
			infile >> a;
			my_list.push_back(a);
		}
	}
	else
	{
		cout << "Файл не удалось открыть." << endl;
	}

	infile.close();
	return my_list;
}

//поиск максимального элемента
int search_max(list<int> my_list)
{
	int max = *my_list.begin();
	for (list<int>::const_iterator iter = ++my_list.begin(); iter != my_list.end(); ++iter)
	{
		if (*iter > max)
			max = *iter;
	}
	return max;
}

/*функция modify, выполняющая преобразование полученного контейнера.
На вход функции должен поступать контейнер, в качестве результата функция 
должна возвращать модифицированный контейнер;
(разделить все числа на половину максимального из чисел)!!!!!!!!*/
list<int> modify(list<int> my_list)
{
	double half_max = 1. * search_max(my_list) / 2; //половина максимального элемента

	list<int> my_list_mod;
	//изменение контейнера
	for (list<int>::const_iterator iter = my_list.begin(); iter != my_list.end(); ++iter)
	{
		my_list_mod.push_back(*iter / half_max);
	}
	return my_list_mod;
}

//перегруженный поиск максимального элемента
int search_max(list<int>::iterator begin, list<int>::iterator end)
{
	int max = *begin;
	for (auto iter = ++begin; iter != end; ++iter)
	{
		if (*iter > max)
			max = *iter;
	}
	return max;
}

/*перегруженная функция modify, принимающая на вход итераторы начала 
и конца обрабатываемой части контейнера*/
list<int> modify(list<int>::iterator begin, list<int>::iterator end)
{
	double half_max = 1. * search_max(begin, end) / 2;  //половина максимального элемента

	list<int> my_list_mod;
	//изменение контейнера
	for (auto iter = begin; iter != end; ++iter)
	{
		*iter /= half_max;
		my_list_mod.push_back(*iter);
	}
	return my_list_mod;
}

//деление на половину максимального
struct div_into_half_max_transform
{
	//половина максимального
	double half_max;

	div_into_half_max_transform(int max)
	{
		half_max = 1. * max / 2;
	}

	int operator() (int& elem) const
	{
		return elem / half_max;
	}
};

/*функция, реализующая требуемое преобразование с использованием 
алгоритма std::transform вместо функции modify*/
list<int> _transform(list<int> my_list)
{
	int max = search_max(my_list);
	list<int> my_list_mod(my_list.size());
	transform(my_list.begin(), my_list.end(), my_list_mod.begin(), div_into_half_max_transform(max));
	return my_list_mod; 
}

//деление на половину максимального
struct div_into_half_max_for
{
	//половина максимального
	double half_max;

	div_into_half_max_for(int max)
	{
		half_max = 1. * max / 2;
	}

	void operator() (int& elem) const
	{
		cout << (int) (elem / half_max) << " ";
	}
};

/*функция, реализующая требуемое преобразование с использованием 
алгоритма std::for_each вместо функции modify*/
list<int> _for_each(list<int> my_list)
{
	int max = search_max(my_list);
	//for_each не изменяет
	for_each(my_list.begin(), my_list.end(), div_into_half_max_for(max));
	return my_list; 
}

//функция, вычисляющая сумму чисел, содержащихся в контейнере
int sum(list<int> my_list)
{
	int summa = 0;
	for (list<int>::const_iterator iter = my_list.begin(); iter != my_list.end(); ++iter)
	{
		summa += *iter;
	}
	return summa;
}

/*функция, вычисляющая среднее арифметическое чисел,
содержащихся в контейнере*/
double average(list<int> my_list)
{
	if (my_list.size() > 0)
		return 1. * sum(my_list) / my_list.size();
	else
		return 0;
}

//функция вывода результата в файл
void output_file(string name, list<int> my_list)
{
	ofstream outfile(name);
	if (outfile)
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(outfile, " "));
	else
		cout << "Файл на удалось открыть." << endl;
}

//функция вывода результата на экран
void output_screen(list<int> my_list)
{
	if (my_list.size() > 0)
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(cout, " "));
	else
		cout << "Список пуст." << endl;
}

char menu(char max)
{
	char correct[20] = "0123";

	bool ok;
	char ch;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "                               МЕНЮ" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "1 - Заполнение текстового файла случайными числами (через цикл)" << endl;
	cout << "2 - Заполнение текстового файла случайными числами (через generate)" << endl;
	cout << "3 - Загрузка контейнера из файла" << endl;
	if (max != '3')
	{
		strcpy(correct, "0123456789");
		cout << "4 - Преобразование контейнера через modify" << endl;
		cout << "5 - Преобразование контейнера через перегруженный modify" << endl;
		cout << "6 - Преобразование контейнера через _transform" << endl;
		cout << "7 - Преобразование контейнера через _for_each" << endl;
		cout << "8 - Сумма и среднее арифметическое чисел, содержащихся в контейнере" << endl;
		cout << "9 - Вывод результата на экран (с возможностью сохранения в файл)" << endl;
	}
	cout << "0 - Выход" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	do
	{
		cout << ">> ";
		cin >> ch;
		//проверка на пустую строку и лишние символы
		ok = (ch != '\n') && (strchr(correct, ch) != NULL);
		if (ok)
			return ch;
		else
			cout << "Неккоректный ввод.";
	} while (!ok);
}

char notcorrect[] = "/|\?!&^%$#*:;><-+()";
bool correct_filename(char name[])
{
	bool bad = strstr(name, ".txt") != &name[strlen(name)-4];
	for (int i = 0; (i < (strlen(name)-4)) && !bad ; ++i)
	{
		bad = (strchr(notcorrect, name[i]) != NULL);
	}
	return !bad;
}

int main()
{
	setlocale(LC_ALL, "");
	srand(unsigned(time(0)));

	FILE* file;
	list<int> my_list;
	char item;
	char max_item = '3';
	char ch;
	char name[] = "";
	int N, M;

	do
	{
		item = menu(max_item);
		switch (item)
		{
		case '1':
		{
			cout << "Введите имя файла (*.txt): ";
			cin >> name;
			if (correct_filename(name))
			{
				cout << "Введите количество элементов N: ";
				cin >> N;
				cout << "Введите число M: ";
				cin >> M;
				file = create_file_cycle(name, N, M);
				if (file != NULL)
				{
					cout << "Файл создан под именем " << name << ".\n";
					max_item = '3';
				}
				else
				{
					cout << "Файл не удалось создать." << name << ".\n";
				}
			}
			else
			{
				cout << "Неккоректное имя файла." << endl;
			}
		}
			break;
		case '2':
		{
			cout << "Введите имя файла (*.txt): ";
			cin >> name;
			if (correct_filename(name))
			{
				cout << "Введите количество элементов: ";
				cin >> N;
				cout << "Введите число M: ";
				cin >> M;
				file = create_file_generate(name, N, M);
				if (file != NULL)
				{
					cout << "Файл создан под именем " << name << ".\n";
					max_item = '3';
				}
				else
				{
					cout << "Файл не удалось создать." << name << ".\n";
				}
			}
			else
			{
				cout << "Неккоректное имя файла." << endl;
			}
		}
			break;
		case '3':
		{
			if (file == NULL)
			{
				cout << "Для начала создайте файл." << endl;
			}
			else
			{
				my_list = load_from_file(file);
				cout << "Загрузка произведена." << endl;
				max_item = '9';
			}
		}
			break;
		case '4':
			my_list = modify(my_list);
			cout << "Преобразование произведено." << endl;
			break;
		case '5':
			my_list = modify(my_list.begin(), my_list.end());
			cout << "Преобразование произведено." << endl;
			break;
		case '6':
			my_list = _transform(my_list);
			cout << "Преобразование произведено." << endl;
			break;
		case '7':
			my_list = _for_each(my_list);
			cout << endl << "Преобразование произведено." << endl;
			break;
		case '8':
			cout << "Сумма: " << sum(my_list) << endl;
			cout << "Среднее арифметическое: " << average(my_list) << endl;
			break;
		case '9':
			cout << "Текущий список: " << endl;
			output_screen(my_list);
			cout << endl << "Cохранить файл? (y/n/любая клавиша - отмена)" << endl;
			cin >> ch;
			if (ch == 'y')
			{
				cout << "Введите имя файла: ";
				cin >> name;
				if (correct_filename(name))
				{
					output_file(name, my_list);
					cout << "Результат сохранен в файл " << name << ".\n";
				}
				else
				{
					cout << "Неккоректное имя файла." << endl;
				}
			}
			break;
		case '0':
			exit(0);
			break;
		default:
			cout << "Некорретный ввод." << endl;
			break;
		}
		cout << endl;
	} while (item != 0);

    return 0;
}


