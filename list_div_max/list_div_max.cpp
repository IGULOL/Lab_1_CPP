/* 11C. ����������� ������ ���� ���������� ��������� � ��������� ����,
����������� ��� ������������� ���� ������ (1-8) ��� ���������� ������ (std::list)
� �������������� 11.

11. ��������� ��� ����� �� �������� ������������� �� �����.*/


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

//��������� ���������� �����
struct random_gen
{
	int m; //�������� ����� [-m;m]

	random_gen(int mn) : m(mn) {}

	int operator() ()
	{
		return rand() % (2 * m + 1) - m;
	}
};

/*������� ���������� ���������� ����� N ������ ���������� ������� 
� ��������� �� - M �� M. ��� �����, N � M ��������� �� ���� �������, 
���� ������������ �������� ��� ���������;
1 �������. ��������� ����� � �����. */
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
		cout << "���� ������." << endl;
	}
	else
	{
		cout << "���� �� ������� �������." << endl;
	}

	return file;
}

//2 �������. ��������� ����� � �������������� ��������� std::generate
FILE* create_file_generate(char name[], int n, int m)
{
	FILE *file;
	file = fopen(name, "w+");
	fstream outfile(file);

	list<int> my_list(n);

	if (outfile)
	{
		//������������� ��������� �����
		generate(my_list.begin(), my_list.end(), random_gen(m));
		//������ ����� � ����
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(outfile, " "));
		cout << "���� ������." << endl;
	}
	else
	{
		cout << "���� �� ������� �������." << endl;
	}

	return file;
}

/*�������, ���������� ���� �� ����, � ������������ ���������������� ���������,
����������� ������� �� �����*/
list<int> load_from_file(FILE *file)
{
	int a;

	ifstream infile(file);
	infile.seekg(0);

	list<int> my_list;
	if (infile)
	{
		//���� ����� ������� �����
		while (!infile.eof())
		{
			infile >> a;
			my_list.push_back(a);
		}
	}
	else
	{
		cout << "���� �� ������� �������." << endl;
	}

	infile.close();
	return my_list;
}

//����� ������������� ��������
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

/*������� modify, ����������� �������������� ����������� ����������.
�� ���� ������� ������ ��������� ���������, � �������� ���������� ������� 
������ ���������� ���������������� ���������;
(��������� ��� ����� �� �������� ������������� �� �����)!!!!!!!!*/
list<int> modify(list<int> my_list)
{
	double half_max = 1. * search_max(my_list) / 2; //�������� ������������� ��������

	list<int> my_list_mod;
	//��������� ����������
	for (list<int>::const_iterator iter = my_list.begin(); iter != my_list.end(); ++iter)
	{
		my_list_mod.push_back(*iter / half_max);
	}
	return my_list_mod;
}

//������������� ����� ������������� ��������
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

/*������������� ������� modify, ����������� �� ���� ��������� ������ 
� ����� �������������� ����� ����������*/
list<int> modify(list<int>::iterator begin, list<int>::iterator end)
{
	double half_max = 1. * search_max(begin, end) / 2;  //�������� ������������� ��������

	list<int> my_list_mod;
	//��������� ����������
	for (auto iter = begin; iter != end; ++iter)
	{
		*iter /= half_max;
		my_list_mod.push_back(*iter);
	}
	return my_list_mod;
}

//������� �� �������� �������������
struct div_into_half_max_transform
{
	//�������� �������������
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

/*�������, ����������� ��������� �������������� � �������������� 
��������� std::transform ������ ������� modify*/
list<int> _transform(list<int> my_list)
{
	int max = search_max(my_list);
	list<int> my_list_mod(my_list.size());
	transform(my_list.begin(), my_list.end(), my_list_mod.begin(), div_into_half_max_transform(max));
	return my_list_mod; 
}

//������� �� �������� �������������
struct div_into_half_max_for
{
	//�������� �������������
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

/*�������, ����������� ��������� �������������� � �������������� 
��������� std::for_each ������ ������� modify*/
list<int> _for_each(list<int> my_list)
{
	int max = search_max(my_list);
	//for_each �� ��������
	for_each(my_list.begin(), my_list.end(), div_into_half_max_for(max));
	return my_list; 
}

//�������, ����������� ����� �����, ������������ � ����������
int sum(list<int> my_list)
{
	int summa = 0;
	for (list<int>::const_iterator iter = my_list.begin(); iter != my_list.end(); ++iter)
	{
		summa += *iter;
	}
	return summa;
}

/*�������, ����������� ������� �������������� �����,
������������ � ����������*/
double average(list<int> my_list)
{
	if (my_list.size() > 0)
		return 1. * sum(my_list) / my_list.size();
	else
		return 0;
}

//������� ������ ���������� � ����
void output_file(string name, list<int> my_list)
{
	ofstream outfile(name);
	if (outfile)
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(outfile, " "));
	else
		cout << "���� �� ������� �������." << endl;
}

//������� ������ ���������� �� �����
void output_screen(list<int> my_list)
{
	if (my_list.size() > 0)
		copy(my_list.begin(), my_list.end(), ostream_iterator<int>(cout, " "));
	else
		cout << "������ ����." << endl;
}

char menu(char max)
{
	char correct[20] = "0123";

	bool ok;
	char ch;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "                               ����" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cout << "1 - ���������� ���������� ����� ���������� ������� (����� ����)" << endl;
	cout << "2 - ���������� ���������� ����� ���������� ������� (����� generate)" << endl;
	cout << "3 - �������� ���������� �� �����" << endl;
	if (max != '3')
	{
		strcpy(correct, "0123456789");
		cout << "4 - �������������� ���������� ����� modify" << endl;
		cout << "5 - �������������� ���������� ����� ������������� modify" << endl;
		cout << "6 - �������������� ���������� ����� _transform" << endl;
		cout << "7 - �������������� ���������� ����� _for_each" << endl;
		cout << "8 - ����� � ������� �������������� �����, ������������ � ����������" << endl;
		cout << "9 - ����� ���������� �� ����� (� ������������ ���������� � ����)" << endl;
	}
	cout << "0 - �����" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	do
	{
		cout << ">> ";
		cin >> ch;
		//�������� �� ������ ������ � ������ �������
		ok = (ch != '\n') && (strchr(correct, ch) != NULL);
		if (ok)
			return ch;
		else
			cout << "������������ ����.";
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
			cout << "������� ��� ����� (*.txt): ";
			cin >> name;
			if (correct_filename(name))
			{
				cout << "������� ���������� ��������� N: ";
				cin >> N;
				cout << "������� ����� M: ";
				cin >> M;
				file = create_file_cycle(name, N, M);
				if (file != NULL)
				{
					cout << "���� ������ ��� ������ " << name << ".\n";
					max_item = '3';
				}
				else
				{
					cout << "���� �� ������� �������." << name << ".\n";
				}
			}
			else
			{
				cout << "������������ ��� �����." << endl;
			}
		}
			break;
		case '2':
		{
			cout << "������� ��� ����� (*.txt): ";
			cin >> name;
			if (correct_filename(name))
			{
				cout << "������� ���������� ���������: ";
				cin >> N;
				cout << "������� ����� M: ";
				cin >> M;
				file = create_file_generate(name, N, M);
				if (file != NULL)
				{
					cout << "���� ������ ��� ������ " << name << ".\n";
					max_item = '3';
				}
				else
				{
					cout << "���� �� ������� �������." << name << ".\n";
				}
			}
			else
			{
				cout << "������������ ��� �����." << endl;
			}
		}
			break;
		case '3':
		{
			if (file == NULL)
			{
				cout << "��� ������ �������� ����." << endl;
			}
			else
			{
				my_list = load_from_file(file);
				cout << "�������� �����������." << endl;
				max_item = '9';
			}
		}
			break;
		case '4':
			my_list = modify(my_list);
			cout << "�������������� �����������." << endl;
			break;
		case '5':
			my_list = modify(my_list.begin(), my_list.end());
			cout << "�������������� �����������." << endl;
			break;
		case '6':
			my_list = _transform(my_list);
			cout << "�������������� �����������." << endl;
			break;
		case '7':
			my_list = _for_each(my_list);
			cout << endl << "�������������� �����������." << endl;
			break;
		case '8':
			cout << "�����: " << sum(my_list) << endl;
			cout << "������� ��������������: " << average(my_list) << endl;
			break;
		case '9':
			cout << "������� ������: " << endl;
			output_screen(my_list);
			cout << endl << "C�������� ����? (y/n/����� ������� - ������)" << endl;
			cin >> ch;
			if (ch == 'y')
			{
				cout << "������� ��� �����: ";
				cin >> name;
				if (correct_filename(name))
				{
					output_file(name, my_list);
					cout << "��������� �������� � ���� " << name << ".\n";
				}
				else
				{
					cout << "������������ ��� �����." << endl;
				}
			}
			break;
		case '0':
			exit(0);
			break;
		default:
			cout << "����������� ����." << endl;
			break;
		}
		cout << endl;
	} while (item != 0);

    return 0;
}


