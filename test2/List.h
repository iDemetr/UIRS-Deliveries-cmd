//Листинг #1            Линейный двусвязный список
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct obj;														//прототип структуры объектов

struct resources;

template <class T>
void addList(T*&, T*&, T*);

template <class T>
void delList(int, T*&, T*&);

struct STemp_Index
{
	int objOut;
	int objRes;

	string objIn;
};

//структура поставок
struct deliveryStruct
{
public:

	obj* adresDeliv;				//adresDeliv - указатель на адрес поставки (для запоминания "из" какого объекта поставляется
	deliveryStruct* next, * pred;	//указатели на следующий и предыдущий элементы
	resources* adresRes;			//указатель на поставляемый ресурс

	deliveryStruct()
	{
		next = pred = NULL;
		adresDeliv = NULL;
		adresRes = NULL;
	}

	~deliveryStruct() {}
};

//структура ресурсов
struct resources
{
public:

	string nameRes;						//поле для хранения названия ресурса
	resources* next, * pred;			//указатели на следующий и предыдущий элементы
	deliveryStruct* headD, * tailD;		//указатели на голову и хвост соответственно 
	int countDeliv = 0;					//кол-во поставок

	resources()
	{
		next = pred = NULL;
		headD = tailD = NULL;
	}
	~resources()
	{
		delList(countDeliv, headD, tailD);				//Удаляем список поставок ресурсов.
	}

	//функция добавления поставок в лист и запоминания объекта "в" который идет поставка
	void addDelivFrom(obj*& inObj)
	{
		deliveryStruct* newDelivFrom = new deliveryStruct;

		newDelivFrom->adresDeliv = inObj;

		addList(headD, tailD, newDelivFrom);
	}

	bool isDelivery()
	{
		if (headD) { return true; }
		else { return false; }
	}
};

//структура объектов
struct obj
{
public:
	string name;															//name - поле, содержащее название объекта

	obj* next, * pred;														//указатели на следующий и предыдущий элементы списка
	resources* headRes, * tailRes;											//указатели на голову и хвост списка ресурсов

	deliveryStruct* headD, * tailD;											//указатели на голову и хвост списка поставок

	int nomerObj = 0;														//хранит номер объекта "в" который идет поставка
	int countRes = 0;														//количество ресурсов для конкретного объекта


	obj()
	{
		next = pred = NULL;
		headRes = tailRes = NULL;
		headD = tailD = NULL;
	}

	~obj()
	{
		resources* oRes = headRes;

		while (oRes != NULL)
		{
			headRes = oRes->pred;
			delete oRes;
			oRes = headRes;
		}
	}

	//функция добавления ресурса в список ресурсов
	void addRes(string newNameRes)
	{
		resources* newRes = new resources;
		newRes->nameRes = newNameRes;

		addList(headRes, tailRes, newRes);
	}

	//функция вывода списка ресурсов
	void show()
	{
		resources* temp = headRes;
		int i = 0;

		//Если ресурса есть, выводит список ресурсов.
		if (temp)
		{
			cout << " Ресурсы: ";

			while (temp != NULL)
			{
				i++;

				cout << i << "." << " " << temp->nameRes << " ";
				temp = temp->pred;
			}
		}
		else
		{
			cout << " Ресурсов нет.";
		}
	}

	//функция добавления объекта из которого идет поставка
	void addDeliv(obj*& fromObj, resources*& tempRes)
	{
		deliveryStruct* newDelivIn = new deliveryStruct;

		newDelivIn->adresDeliv = fromObj;
		newDelivIn->adresRes = tempRes;

		addList(headD, tailD, newDelivIn);
	}

	int get_nomberDeliv(obj*& fromObj, resources*& tempRes)										//fromObj - адрес промышленности, номер поставки которой нужно найти
																								//tempRes - адрес поставки.
	{
		deliveryStruct* temp = headD;

		int i = 0;

		while (temp)
		{
			i++;

			if (temp->adresDeliv == fromObj && temp->adresRes == tempRes) return i;
			
			temp = temp->pred;				
		}

		return 0;
	}

	//Функция проверки, есть-ли ресурсы у промышленности.
	bool isRes()
	{
		if (headRes) { return true; }
		else { return false; }
	}
};

class List														//Создаем тип данных Список
{
private:

	string fileName = "Delivery.txt";

	obj* Head, * Tail;											//Указатели на адреса начала списка и его конца.

	vector <STemp_Index> VTemp_Index;

	int countObj = 0;											//число объектов

public:

	bool isdontSaveData = false;								//Есть ли не несохранённые данные

	List()
	{
		Head = Tail = NULL;
	}

	~List()                           //Деструктор
	{
		while (Head)                       //Пока по адресу на начало списка что-то есть
		{
			Tail = Head->next;             //Резервная копия адреса следующего звена списка
			delete Head;                   //Очистка памяти от первого звена
			Head = Tail;                   //Смена адреса начала на адрес следующего элемента
		}
	}


	//Добавление промышленностей
	void Add_Obj(string newName)
	{
		countObj++;												//подсчитываем номер добавляемого объекта

		obj* newObj = new obj;

		newObj->name = newName;									//Записываем имя объекта

		addList(Head, Tail, newObj);

		isdontSaveData = true;									//Устанавливаем флаг, что с момента последного сохранения были изменены данные.
	}

	//Добавление рессурса в объект
	void Add_ObjRes(string newNameRes, int countInd)
	{
		obj* temp = Head;

		for (int i = 1; i < countInd; i++)
		{
			temp = temp->pred;									//переводим курсор на следующий элемент
		}

		temp->addRes(newNameRes);								//вызываем функцию добавления ресурсов
		temp->countRes++;

		isdontSaveData = true;									//Устанавливаем флаг, что с момента последного сохранения были изменены данные.
	}

	//Добавление поставок
	void Add_delivery(int nomerObj, int inObj, int nomerRes)	//nomerObj	- номер пром. откуда поставляется ресурс
	{															//inObj		- номер пром. куда поставляется ресурс
																//nomerRes	- номер поставляемого ресурса

		obj* temp = Head;										//указатель на nomerObj
		obj* tempInObj = Head;									//указатель на inObj
		resources* tempRes;										//указатель на nomerRes

		if (nomerObj <= countObj && inObj <= countObj)
		{
			for (int i = 1; i < nomerObj; i++)					//передвигаем курсор до номера промышленности из которой поставляется ресурс
			{
				temp = temp->pred;
			}

			if (nomerRes <= temp->countRes)
			{
				cout << temp->countRes;

				tempRes = temp->headRes;
				for (int i = 1; i < nomerRes; i++)				//передвигаем курсор до номера поставляемого ресурса
				{
					tempRes = tempRes->pred;
				}

				for (int i = 1; i < inObj; i++)					//передвигаем курсор до номера промышленности в которую поставляется ресурс
				{
					tempInObj = tempInObj->pred;
				}

				tempRes->addDelivFrom(tempInObj);
				tempInObj->addDeliv(temp, tempRes);

				isdontSaveData = true;							//Устанавливаем флаг, что с момента последного сохранения были изменены данные.

				temp->nomerObj = inObj;
				tempRes->countDeliv++;

				system("cls");
			}
		}
	}


	//Вывод списка объектов
	void Show()
	{
		obj* temp = Head;										//Временный указатель на адрес первого элемента		

		if (temp == NULL) cout << "\n Cписок промышленностей пуст!";
		else while (temp != NULL)								//Пока не встретим пустое значение
		{
			for (int i = 1; i <= countObj; i++)
			{
				cout << "\n" << i << ". " << temp->name << "\n";//Выводим каждое считанное значение на экран

				temp = temp->pred;								//Смена адреса на адрес следующего элемента
			}
		}
		cout << "\n";
	}

	//Выводит конкретный объект и принадлежащие ему ресурсы
	void Show(int nomerObj)
	{
		obj* temp = Head;

		if (nomerObj <= countObj)
		{
			for (int i = 1; i < nomerObj; i++)
			{
				temp->name;
				temp = temp->pred;
			}
			cout << temp->name << ":";
			temp->show();
		}
	}

	//Вывод всех объектов/ресурсов/поставок
	void ShowAll()
	{
		obj* temp = Head;				//указатель на объект
		resources* tempRes;				//указатель на ресурс
		deliveryStruct* tempDeliv;		//указатель на поставки 

		int i = 0, j = 0, k = 0;		//счетчики

		if (temp == NULL)
		{
			cout << "\n Список объектов пуст!";
		}
		else
		{
			cout << "\n Промышленности: \n";

			while (temp != NULL)
			{
				i++;

				tempRes = temp->headRes;

				cout << "------------------------------------------------";
				cout << "\n " << i << ". " << temp->name << "\n";			//выводит промышленность

				j = 0;
				if (tempRes != NULL)
				{
					cout << "\t Ресурсы:\n";
					while (tempRes != NULL)
					{
						j++;

						cout << "\t " << j << ". " << tempRes->nameRes << "\n";

						k = 0;
						if (tempRes->headD != NULL)
						{
							k++;
							cout << "\t    " << "Осуществляются поставки в\n";
						}

						tempDeliv = tempRes->headD;

						int i = 0;

						while (tempDeliv != NULL)
						{
							i++;

							cout << "\t\t|" << i << ". " << tempDeliv->adresDeliv->name << "\n";


							tempDeliv = tempDeliv->pred;
						}

						tempRes = tempRes->pred;
					}

				}
				else cout << "\tРесурсов нет\n";

				//Проверка, есть ли поставки от других промышленностей
				if (temp->headD != NULL)
				{
					cout << "\n Поставки из других промышленностей: \n";

					deliveryStruct* tempDelivOut = temp->headD;

					while (tempDelivOut != NULL)
					{
						cout << "\tПоставка ресурса " << tempDelivOut->adresRes->nameRes << " из " << tempDelivOut->adresDeliv->name << "\n";

						tempDelivOut = tempDelivOut->pred;
					}
				}

				cout << "------------------------------------------------\n";

				temp = temp->pred;
			}
		}
	}


	//Считывание данных из файла в лист
	void OpenFile()
	{
		int i(0);

		ifstream f(fileName, ios_base::in);

		STemp_Index tempIndex;
		string temp;

		if (f.is_open())
		{
			for (i = 0; i < 2; i++) { f >> temp; }								//Считывание первых 2-х строчек файла.

			while (!f.eof())
			{
				f >> temp;														//Переводим маркер с нумерации промышленности.

				getline(f, temp);												//Считываеним имя промышленности целиком 

				Add_Obj(temp);											//Добавляем имя промышленности в список

				f >> temp;														//Определяем, есть ли ресурсы у промышленности

				if (temp == "Ресурсы:")											//Если ресурсы есть, считываем список ресурсов
				{
					f >> temp; //Считываем и пропускаем номерацию

					i = 0;

					while (true)												//Цикл считывания ресурсов, пока не дойдём до конца списка ресурсов
					{
						getline(f, temp);										//Считываеним имя ресурса целиком 

						Add_ObjRes(temp, countObj);						//Добавляем имя ресурса в список 

						i++;													//Считаем текущий номер в списке ресурсов

						f >> temp;												//Фиксируем, есть ли поставка / следующий ресурс

						if (temp == "Осуществляются")							//Если обнаружен список поставок
						{
							getline(f, temp);
							f >> temp;											//Считываем нумерацию

							while (check_Numbering(temp) && temp.c_str()[0] == '|')	//Пока это нумерованный список поставок
							{
								getline(f, temp);								//Считываем имя поставки

								tempIndex.objOut = countObj;
								tempIndex.objRes = i;
								tempIndex.objIn = temp;

								VTemp_Index.push_back(tempIndex);				//Сохраняем индексы объектов, чтоб потом добавить поставки.

								f >> temp;										//Смотрим, что дальше
							}
						}

						if (check_Numbering(temp))								//Если обнаружен следующий ресурс, вернуться к началу 
						{
							continue;											//считывания полного имени ресурса
						}

						if (check_EndBlok(temp))								//Если достигнут конец блока записей для данной промышленности
						{
							break;
						}												//Завершить цикл считывания ресурсов.						

						if (temp == "Поставки")
						{
							getline(f, temp);

							while (!check_EndBlok(temp))						//Пока не достигнут конец блока
							{
								getline(f, temp);
							}

							break;
						}
					}

					f >> temp;													//Считываем начало следующего блока

					continue;
				}
				else
				{
					getline(f, temp);
					f >> temp;
				}

				if (temp == "Поставки")
				{
					getline(f, temp);

					while (!check_EndBlok(temp))						//Пока не достигнут конец блока
					{
						getline(f, temp);
					}

					continue;
				}
				else { f >> temp; }
			}

			//Добавление поставок в лист.
			while (!VTemp_Index.empty())
			{
				Add_delivery(VTemp_Index.back().objOut, get_NomberObj(VTemp_Index.back().objIn), VTemp_Index.back().objRes);
				VTemp_Index.pop_back();
			}

			cout << "\n Данные успешно восстановленны из файла Delivery.txt, расположенного в репозитории программы.\n\n";

			f.close();
		}
	}

	//Запись данных из листа в файл
	void SaveFile()
	{
		obj* temp = Head;				//указатель на объект
		resources* tempRes;				//указатель на ресурс
		deliveryStruct* tempDeliv;		//указатель на поставки 

		ofstream f(fileName, ios_base::out | ios_base::trunc);//открываем для записи, предварительно очистив его

		int i = 0, j = 0, k = 0;		//счетчики

		f << "\n Промышленности: \n";

		while (temp != NULL)
		{
			isdontSaveData = false;									//Ставим флаг, что все данные сохранены.

			i++;

			tempRes = temp->headRes;

			f << "------------------------------------------------";
			f << "\n " << i << ". " << temp->name << "\n";			//выводит промышленность

			j = 0;
			if (tempRes != NULL)
			{
				f << "\t Ресурсы:\n";
				while (tempRes != NULL)
				{
					j++;

					f << "\t " << j << ". " << tempRes->nameRes << "\n";

					k = 0;
					if (tempRes->headD != NULL)
					{
						k++;
						f << "\t    " << "Осуществляются поставки в\n";
					}

					tempDeliv = tempRes->headD;

					int i = 0;

					while (tempDeliv != NULL)
					{
						i++;

						f << "\t\t|" << i << ". " << tempDeliv->adresDeliv->name << "\n";


						tempDeliv = tempDeliv->pred;
					}

					tempRes = tempRes->pred;
				}

			}
			else f << "\tРесурсов нет\n";

			//Проверка, есть ли поставки от других промышленностей
			if (temp->headD != NULL)
			{
				f << "\n Поставки из других промышленностей: \n";

				deliveryStruct* tempDelivOut = temp->headD;

				while (tempDelivOut != NULL)
				{
					f << "\tПоставка ресурса " << tempDelivOut->adresRes->nameRes << " из " << tempDelivOut->adresDeliv->name << "\n";

					tempDelivOut = tempDelivOut->pred;
				}
			}

			f << "------------------------------------------------\n";

			temp = temp->pred;
		}

		f.close();
	}


	//Удаление объекта
	void dell_Obj(int nomerObj)
	{
		delList(nomerObj, Head, Tail);

		isdontSaveData = true;							//Устанавливаем флаг, что с момента последного сохранения были изменены данные.

		countObj--;
		system("cls");
	}

	//Удаление ресурса из объекта (удаление ресурса работает аналогично удалению объекта)
	void dell_Res(int nomerObj, int nomerRes)
	{
		obj* temp = Head;
		obj* tempIn;									//буферный адрес промышленности, принимающий поставку.
		resources* tempRes;
		deliveryStruct* tempDeliv;

		int i;

		for (i = 1; i < nomerObj; i++)					//передвигаем курсор до нужного объекта
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;

		for (i = 1; i < nomerRes; i++)					//Получение адреса удаляемого ресурса
		{
			tempRes = tempRes->pred;
		}

		tempDeliv = tempRes->headD;						//Получение адреса списка поставок удаляемого ресурса

		//Цикл удаления поставок этого ресурса из промышленностей, в которые они (поставки) идут.
		while (tempDeliv)								//Пока список поставок есть
		{
			tempIn = tempDeliv->adresDeliv;

			delList(tempIn->get_nomberDeliv(temp, tempRes), tempIn->headD, tempIn->tailD);

			tempDeliv = tempDeliv->pred;
		}

		delList(nomerRes, temp->headRes, temp->tailRes);

		isdontSaveData = true;							//Устанавливаем флаг, что с момента последного сохранения были изменены данные.

		temp->countRes--;
		system("cls");
	}

	//функция удаления поставок
	void dell_Deliv(int nomerObj, int nomerRes, int nomerDeliv)
	{
		obj* tempFrom = Head;						//объект, из которого идет поставка
		obj* tempIn;								//объект, в который идет поставка 
		resources* tempResFrom;						//tempResFrom - поставляемый ресурс
		deliveryStruct* tempDelivFrom;				//tempDelivFrom - адрес узла поставок из объекта 
		deliveryStruct* tempDelivIn;				//tempDelivIn - адрес узла поставок в объект

		for (int i = 1; i < nomerObj; i++)					//переводим курсор до нужного объекта
		{
			tempFrom = tempFrom->pred;
		}

		tempResFrom = tempFrom->headRes;
		for (int i = 1; i < nomerRes; i++)					//переводим курсор до ресурса, из которого следует удалить поставку
		{

			tempResFrom = tempResFrom->pred;
		}

		tempDelivFrom = tempResFrom->headD;

		for (int i = 1; i < nomerDeliv; i++)				//переводим курсор до поставки, которую нужно удалить
		{
			tempDelivFrom = tempDelivFrom->pred;
		}

		tempIn = tempDelivFrom->adresDeliv;					//Адрес объекта, в который идёт поставка

		tempDelivIn = tempIn->headD;						//Получаем указатель на голову списка поставок приходящих в объект

		int nomber_delete_deliv_in(0);

		while (tempResFrom != tempDelivIn->adresRes && tempFrom != tempDelivIn->adresDeliv)
		{
			nomber_delete_deliv_in++;
			tempDelivIn = tempDelivIn->pred;				//находим элемент, который нужно удалить
		}

		delList(nomber_delete_deliv_in, tempIn->headD, tempIn->tailD);

		delList(nomerDeliv, tempResFrom->headD, tempResFrom->tailD);

		isdontSaveData = true;								//Устанавливаем флаг, что с момента последного сохранения были изменены данные.

		tempResFrom->countDeliv--;
		system("cls");
		cout << "\n Поставка успешно удалена!";
		ShowAll();
	}



	//Выдает номер промышленности по имени
	int get_NomberObj(string str)
	{
		obj* temp = Head;
		int i = 0;														//Счётчик объектов

		while (temp)
		{
			i++;

			if (temp->name == str) return i;							//Возвращается номер объекта
			else temp = temp->pred;
		}

		return 0;														//Возвращается 0 если промышленность с таким именем не найдена.
	}

	//Выдает номер ресурса по имени
	int get_NomerRes(int nomberObj, string str)
	{
		obj* tempObj = Head;
		resources* tempRes;

		int i = 0;														//Счётчик объектов

		//Получение адреса промышленности, кот. принадлежит ресурс
		for (i = 1; i < nomberObj; i++)
		{
			tempObj = tempObj->pred;
		}

		tempRes = tempObj->headRes;
		i = 0;

		while (tempRes)													//Пока есть ресурс
		{
			i++;

			if (tempRes->nameRes == str) return i;						//Возвращается номер ресурса
			else
				tempRes = tempRes->pred;
		}

		return 0;														//Возвращается 0 если ресурс с таким именем не найден.
	}



	//Проверка, есть ли объекты в списке
	bool isObj()
	{
		if (Head) { return true; }
		else { return false; }
	}

	//Проверка, есть-ли хотя бы 1 ресурс в объекте
	bool isRes(int nomberObj)
	{
		obj* temp = Head;
		for (int i = 1; i < nomberObj; i++)					//переводим курсор до нужного объекта
		{
			temp = temp->pred;
		}

		return temp->isRes();
	}

	//Проверка, есть-ли хотя бы 1 поставка ресурса
	bool isDelivery(int nomberObj, int nomberRes)
	{
		obj* temp = Head;
		resources* tempRes;

		for (int i = 1; i < nomberObj; i++)					//переводим курсор до нужного объекта
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;
		for (int i = 1; i < nomberRes; i++)					//переводим курсор до ресурса
		{
			tempRes = tempRes->pred;
		}

		return tempRes->isDelivery();
	}


private:

	bool check_EndBlok(string str)
	{
		if (str.find("----------") == string::npos) return false;
		else return true;
	}

	bool check_Numbering(string str)
	{
		string symbols = "0123456789";

		if (str.find_first_of(symbols) == string::npos) return false;
		else return true;
	}

public:

	int checkInd(int nomerObj)
	{
		if (nomerObj <= 0 || nomerObj > countObj)
		{
			cout << "\n Промышленности под таким номером не существует!";
			return 0;
		}
		else	return 1;
	}

	int checkRes(int nomerObj, int nomerRes)
	{
		obj* temp = Head;
		for (int i = 1; i < nomerObj; i++)					//переводим курсор до нужного объекта
		{
			temp = temp->pred;
		}
		if (nomerRes <= 0 || nomerRes > temp->countRes)
		{
			cout << "\nВ промышленности " << temp->name << " ресурса под таким номером не существует!";
			return 0;
		}
		else	return 1;
	}

	int checkDeliv(int nomerObj, int nomerRes, int nomerDeliv)
	{
		obj* temp = Head;
		resources* tempRes;
		for (int i = 1; i < nomerObj; i++)					//переводим курсор до нужного объекта
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;
		for (int i = 1; i < nomerRes; i++)					//переводим курсор до ресурса
		{
			tempRes = tempRes->pred;
		}

		if (nomerDeliv <= 0 || nomerDeliv > tempRes->countDeliv)
		{
			cout << "\n Поставок ресурса " << tempRes->nameRes << "  под таким номером не существует!";
			return 0;
		}
		else	return 1;
	}
};

template <class T>
void delList(int nomber, T*& head, T*& tail)
{
	int i;

	T* temp = head;

	for (i = 1; i < nomber; i++)	//передвигаем курсор до нужного ресурса
	{
		temp = temp->pred;
	}

	if (temp == head && temp->pred == NULL)			//если элемент первый и единственный
	{
		head = tail = NULL;
		delete(temp);
	}
	else if (temp == head && temp->pred != NULL)	//первый и не единственный
	{
		head = head->pred;
		head->next = NULL;
		delete(temp);
	}
	else if (temp == tail && temp->next != NULL)  // последний и не единственный 
	{
		tail = tail->next;
		tail->pred = NULL;
		delete(temp);
	}
	else
	{
		temp->pred->next = temp->next;
		temp->next->pred = temp->pred;

		delete (temp);
	}
}

template <class T>
void addList(T*& head, T*& tail, T* obj)
{
	if (head == NULL && tail == NULL)
	{
		head = tail = obj;
	}
	else
	{
		obj->next = tail;
		tail->pred = obj;

		tail = obj;
	}
}



////Вывод списка объектов с вложенным списком ресурсов
//void showAllObjRes()										//выводит все объекты и ресурсы
//{
//	obj* temp = Head;										//Временный указатель на адрес первого элемента
//
//	if (temp == NULL)	cout << "\n Cписок пуст!";
//	else while (temp != NULL)								//Пока не встретим пустое значение
//	{
//		for (int i = 1; i <= countObj; i++)
//		{
//			cout << "\n" << i << ". " << temp->name;        //Выводим каждое считанное значение на экран
//
//			temp->show();
//
//			temp = temp->pred;								//Смена адреса на адрес следующего элемента
//		}
//	}
//}
