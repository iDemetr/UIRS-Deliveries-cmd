//������� #1            �������� ���������� ������
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct obj;														//�������� ��������� ��������

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

//��������� ��������
struct deliveryStruct
{
public:

	obj* adresDeliv;				//adresDeliv - ��������� �� ����� �������� (��� ����������� "��" ������ ������� ������������
	deliveryStruct* next, * pred;	//��������� �� ��������� � ���������� ��������
	resources* adresRes;			//��������� �� ������������ ������

	deliveryStruct()
	{
		next = pred = NULL;
		adresDeliv = NULL;
		adresRes = NULL;
	}

	~deliveryStruct() {}
};

//��������� ��������
struct resources
{
public:

	string nameRes;						//���� ��� �������� �������� �������
	resources* next, * pred;			//��������� �� ��������� � ���������� ��������
	deliveryStruct* headD, * tailD;		//��������� �� ������ � ����� �������������� 
	int countDeliv = 0;					//���-�� ��������

	resources()
	{
		next = pred = NULL;
		headD = tailD = NULL;
	}
	~resources()
	{
		delList(countDeliv, headD, tailD);				//������� ������ �������� ��������.
	}

	//������� ���������� �������� � ���� � ����������� ������� "�" ������� ���� ��������
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

//��������� ��������
struct obj
{
public:
	string name;															//name - ����, ���������� �������� �������

	obj* next, * pred;														//��������� �� ��������� � ���������� �������� ������
	resources* headRes, * tailRes;											//��������� �� ������ � ����� ������ ��������

	deliveryStruct* headD, * tailD;											//��������� �� ������ � ����� ������ ��������

	int nomerObj = 0;														//������ ����� ������� "�" ������� ���� ��������
	int countRes = 0;														//���������� �������� ��� ����������� �������


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

	//������� ���������� ������� � ������ ��������
	void addRes(string newNameRes)
	{
		resources* newRes = new resources;
		newRes->nameRes = newNameRes;

		addList(headRes, tailRes, newRes);
	}

	//������� ������ ������ ��������
	void show()
	{
		resources* temp = headRes;
		int i = 0;

		//���� ������� ����, ������� ������ ��������.
		if (temp)
		{
			cout << " �������: ";

			while (temp != NULL)
			{
				i++;

				cout << i << "." << " " << temp->nameRes << " ";
				temp = temp->pred;
			}
		}
		else
		{
			cout << " �������� ���.";
		}
	}

	//������� ���������� ������� �� �������� ���� ��������
	void addDeliv(obj*& fromObj, resources*& tempRes)
	{
		deliveryStruct* newDelivIn = new deliveryStruct;

		newDelivIn->adresDeliv = fromObj;
		newDelivIn->adresRes = tempRes;

		addList(headD, tailD, newDelivIn);
	}

	int get_nomberDeliv(obj*& fromObj, resources*& tempRes)										//fromObj - ����� ��������������, ����� �������� ������� ����� �����
																								//tempRes - ����� ��������.
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

	//������� ��������, ����-�� ������� � ��������������.
	bool isRes()
	{
		if (headRes) { return true; }
		else { return false; }
	}
};

class List														//������� ��� ������ ������
{
private:

	string fileName = "Delivery.txt";

	obj* Head, * Tail;											//��������� �� ������ ������ ������ � ��� �����.

	vector <STemp_Index> VTemp_Index;

	int countObj = 0;											//����� ��������

public:

	bool isdontSaveData = false;								//���� �� �� ������������ ������

	List()
	{
		Head = Tail = NULL;
	}

	~List()                           //����������
	{
		while (Head)                       //���� �� ������ �� ������ ������ ���-�� ����
		{
			Tail = Head->next;             //��������� ����� ������ ���������� ����� ������
			delete Head;                   //������� ������ �� ������� �����
			Head = Tail;                   //����� ������ ������ �� ����� ���������� ��������
		}
	}


	//���������� ���������������
	void Add_Obj(string newName)
	{
		countObj++;												//������������ ����� ������������ �������

		obj* newObj = new obj;

		newObj->name = newName;									//���������� ��� �������

		addList(Head, Tail, newObj);

		isdontSaveData = true;									//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.
	}

	//���������� �������� � ������
	void Add_ObjRes(string newNameRes, int countInd)
	{
		obj* temp = Head;

		for (int i = 1; i < countInd; i++)
		{
			temp = temp->pred;									//��������� ������ �� ��������� �������
		}

		temp->addRes(newNameRes);								//�������� ������� ���������� ��������
		temp->countRes++;

		isdontSaveData = true;									//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.
	}

	//���������� ��������
	void Add_delivery(int nomerObj, int inObj, int nomerRes)	//nomerObj	- ����� ����. ������ ������������ ������
	{															//inObj		- ����� ����. ���� ������������ ������
																//nomerRes	- ����� ������������� �������

		obj* temp = Head;										//��������� �� nomerObj
		obj* tempInObj = Head;									//��������� �� inObj
		resources* tempRes;										//��������� �� nomerRes

		if (nomerObj <= countObj && inObj <= countObj)
		{
			for (int i = 1; i < nomerObj; i++)					//����������� ������ �� ������ �������������� �� ������� ������������ ������
			{
				temp = temp->pred;
			}

			if (nomerRes <= temp->countRes)
			{
				cout << temp->countRes;

				tempRes = temp->headRes;
				for (int i = 1; i < nomerRes; i++)				//����������� ������ �� ������ ������������� �������
				{
					tempRes = tempRes->pred;
				}

				for (int i = 1; i < inObj; i++)					//����������� ������ �� ������ �������������� � ������� ������������ ������
				{
					tempInObj = tempInObj->pred;
				}

				tempRes->addDelivFrom(tempInObj);
				tempInObj->addDeliv(temp, tempRes);

				isdontSaveData = true;							//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.

				temp->nomerObj = inObj;
				tempRes->countDeliv++;

				system("cls");
			}
		}
	}


	//����� ������ ��������
	void Show()
	{
		obj* temp = Head;										//��������� ��������� �� ����� ������� ��������		

		if (temp == NULL) cout << "\n C����� ��������������� ����!";
		else while (temp != NULL)								//���� �� �������� ������ ��������
		{
			for (int i = 1; i <= countObj; i++)
			{
				cout << "\n" << i << ". " << temp->name << "\n";//������� ������ ��������� �������� �� �����

				temp = temp->pred;								//����� ������ �� ����� ���������� ��������
			}
		}
		cout << "\n";
	}

	//������� ���������� ������ � ������������� ��� �������
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

	//����� ���� ��������/��������/��������
	void ShowAll()
	{
		obj* temp = Head;				//��������� �� ������
		resources* tempRes;				//��������� �� ������
		deliveryStruct* tempDeliv;		//��������� �� �������� 

		int i = 0, j = 0, k = 0;		//��������

		if (temp == NULL)
		{
			cout << "\n ������ �������� ����!";
		}
		else
		{
			cout << "\n ��������������: \n";

			while (temp != NULL)
			{
				i++;

				tempRes = temp->headRes;

				cout << "------------------------------------------------";
				cout << "\n " << i << ". " << temp->name << "\n";			//������� ��������������

				j = 0;
				if (tempRes != NULL)
				{
					cout << "\t �������:\n";
					while (tempRes != NULL)
					{
						j++;

						cout << "\t " << j << ". " << tempRes->nameRes << "\n";

						k = 0;
						if (tempRes->headD != NULL)
						{
							k++;
							cout << "\t    " << "�������������� �������� �\n";
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
				else cout << "\t�������� ���\n";

				//��������, ���� �� �������� �� ������ ���������������
				if (temp->headD != NULL)
				{
					cout << "\n �������� �� ������ ���������������: \n";

					deliveryStruct* tempDelivOut = temp->headD;

					while (tempDelivOut != NULL)
					{
						cout << "\t�������� ������� " << tempDelivOut->adresRes->nameRes << " �� " << tempDelivOut->adresDeliv->name << "\n";

						tempDelivOut = tempDelivOut->pred;
					}
				}

				cout << "------------------------------------------------\n";

				temp = temp->pred;
			}
		}
	}


	//���������� ������ �� ����� � ����
	void OpenFile()
	{
		int i(0);

		ifstream f(fileName, ios_base::in);

		STemp_Index tempIndex;
		string temp;

		if (f.is_open())
		{
			for (i = 0; i < 2; i++) { f >> temp; }								//���������� ������ 2-� ������� �����.

			while (!f.eof())
			{
				f >> temp;														//��������� ������ � ��������� ��������������.

				getline(f, temp);												//����������� ��� �������������� ������� 

				Add_Obj(temp);											//��������� ��� �������������� � ������

				f >> temp;														//����������, ���� �� ������� � ��������������

				if (temp == "�������:")											//���� ������� ����, ��������� ������ ��������
				{
					f >> temp; //��������� � ���������� ���������

					i = 0;

					while (true)												//���� ���������� ��������, ���� �� ����� �� ����� ������ ��������
					{
						getline(f, temp);										//����������� ��� ������� ������� 

						Add_ObjRes(temp, countObj);						//��������� ��� ������� � ������ 

						i++;													//������� ������� ����� � ������ ��������

						f >> temp;												//���������, ���� �� �������� / ��������� ������

						if (temp == "��������������")							//���� ��������� ������ ��������
						{
							getline(f, temp);
							f >> temp;											//��������� ���������

							while (check_Numbering(temp) && temp.c_str()[0] == '|')	//���� ��� ������������ ������ ��������
							{
								getline(f, temp);								//��������� ��� ��������

								tempIndex.objOut = countObj;
								tempIndex.objRes = i;
								tempIndex.objIn = temp;

								VTemp_Index.push_back(tempIndex);				//��������� ������� ��������, ���� ����� �������� ��������.

								f >> temp;										//�������, ��� ������
							}
						}

						if (check_Numbering(temp))								//���� ��������� ��������� ������, ��������� � ������ 
						{
							continue;											//���������� ������� ����� �������
						}

						if (check_EndBlok(temp))								//���� ��������� ����� ����� ������� ��� ������ ��������������
						{
							break;
						}												//��������� ���� ���������� ��������.						

						if (temp == "��������")
						{
							getline(f, temp);

							while (!check_EndBlok(temp))						//���� �� ��������� ����� �����
							{
								getline(f, temp);
							}

							break;
						}
					}

					f >> temp;													//��������� ������ ���������� �����

					continue;
				}
				else
				{
					getline(f, temp);
					f >> temp;
				}

				if (temp == "��������")
				{
					getline(f, temp);

					while (!check_EndBlok(temp))						//���� �� ��������� ����� �����
					{
						getline(f, temp);
					}

					continue;
				}
				else { f >> temp; }
			}

			//���������� �������� � ����.
			while (!VTemp_Index.empty())
			{
				Add_delivery(VTemp_Index.back().objOut, get_NomberObj(VTemp_Index.back().objIn), VTemp_Index.back().objRes);
				VTemp_Index.pop_back();
			}

			cout << "\n ������ ������� �������������� �� ����� Delivery.txt, �������������� � ����������� ���������.\n\n";

			f.close();
		}
	}

	//������ ������ �� ����� � ����
	void SaveFile()
	{
		obj* temp = Head;				//��������� �� ������
		resources* tempRes;				//��������� �� ������
		deliveryStruct* tempDeliv;		//��������� �� �������� 

		ofstream f(fileName, ios_base::out | ios_base::trunc);//��������� ��� ������, �������������� ������� ���

		int i = 0, j = 0, k = 0;		//��������

		f << "\n ��������������: \n";

		while (temp != NULL)
		{
			isdontSaveData = false;									//������ ����, ��� ��� ������ ���������.

			i++;

			tempRes = temp->headRes;

			f << "------------------------------------------------";
			f << "\n " << i << ". " << temp->name << "\n";			//������� ��������������

			j = 0;
			if (tempRes != NULL)
			{
				f << "\t �������:\n";
				while (tempRes != NULL)
				{
					j++;

					f << "\t " << j << ". " << tempRes->nameRes << "\n";

					k = 0;
					if (tempRes->headD != NULL)
					{
						k++;
						f << "\t    " << "�������������� �������� �\n";
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
			else f << "\t�������� ���\n";

			//��������, ���� �� �������� �� ������ ���������������
			if (temp->headD != NULL)
			{
				f << "\n �������� �� ������ ���������������: \n";

				deliveryStruct* tempDelivOut = temp->headD;

				while (tempDelivOut != NULL)
				{
					f << "\t�������� ������� " << tempDelivOut->adresRes->nameRes << " �� " << tempDelivOut->adresDeliv->name << "\n";

					tempDelivOut = tempDelivOut->pred;
				}
			}

			f << "------------------------------------------------\n";

			temp = temp->pred;
		}

		f.close();
	}


	//�������� �������
	void dell_Obj(int nomerObj)
	{
		delList(nomerObj, Head, Tail);

		isdontSaveData = true;							//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.

		countObj--;
		system("cls");
	}

	//�������� ������� �� ������� (�������� ������� �������� ���������� �������� �������)
	void dell_Res(int nomerObj, int nomerRes)
	{
		obj* temp = Head;
		obj* tempIn;									//�������� ����� ��������������, ����������� ��������.
		resources* tempRes;
		deliveryStruct* tempDeliv;

		int i;

		for (i = 1; i < nomerObj; i++)					//����������� ������ �� ������� �������
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;

		for (i = 1; i < nomerRes; i++)					//��������� ������ ���������� �������
		{
			tempRes = tempRes->pred;
		}

		tempDeliv = tempRes->headD;						//��������� ������ ������ �������� ���������� �������

		//���� �������� �������� ����� ������� �� ���������������, � ������� ��� (��������) ����.
		while (tempDeliv)								//���� ������ �������� ����
		{
			tempIn = tempDeliv->adresDeliv;

			delList(tempIn->get_nomberDeliv(temp, tempRes), tempIn->headD, tempIn->tailD);

			tempDeliv = tempDeliv->pred;
		}

		delList(nomerRes, temp->headRes, temp->tailRes);

		isdontSaveData = true;							//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.

		temp->countRes--;
		system("cls");
	}

	//������� �������� ��������
	void dell_Deliv(int nomerObj, int nomerRes, int nomerDeliv)
	{
		obj* tempFrom = Head;						//������, �� �������� ���� ��������
		obj* tempIn;								//������, � ������� ���� �������� 
		resources* tempResFrom;						//tempResFrom - ������������ ������
		deliveryStruct* tempDelivFrom;				//tempDelivFrom - ����� ���� �������� �� ������� 
		deliveryStruct* tempDelivIn;				//tempDelivIn - ����� ���� �������� � ������

		for (int i = 1; i < nomerObj; i++)					//��������� ������ �� ������� �������
		{
			tempFrom = tempFrom->pred;
		}

		tempResFrom = tempFrom->headRes;
		for (int i = 1; i < nomerRes; i++)					//��������� ������ �� �������, �� �������� ������� ������� ��������
		{

			tempResFrom = tempResFrom->pred;
		}

		tempDelivFrom = tempResFrom->headD;

		for (int i = 1; i < nomerDeliv; i++)				//��������� ������ �� ��������, ������� ����� �������
		{
			tempDelivFrom = tempDelivFrom->pred;
		}

		tempIn = tempDelivFrom->adresDeliv;					//����� �������, � ������� ��� ��������

		tempDelivIn = tempIn->headD;						//�������� ��������� �� ������ ������ �������� ���������� � ������

		int nomber_delete_deliv_in(0);

		while (tempResFrom != tempDelivIn->adresRes && tempFrom != tempDelivIn->adresDeliv)
		{
			nomber_delete_deliv_in++;
			tempDelivIn = tempDelivIn->pred;				//������� �������, ������� ����� �������
		}

		delList(nomber_delete_deliv_in, tempIn->headD, tempIn->tailD);

		delList(nomerDeliv, tempResFrom->headD, tempResFrom->tailD);

		isdontSaveData = true;								//������������� ����, ��� � ������� ���������� ���������� ���� �������� ������.

		tempResFrom->countDeliv--;
		system("cls");
		cout << "\n �������� ������� �������!";
		ShowAll();
	}



	//������ ����� �������������� �� �����
	int get_NomberObj(string str)
	{
		obj* temp = Head;
		int i = 0;														//������� ��������

		while (temp)
		{
			i++;

			if (temp->name == str) return i;							//������������ ����� �������
			else temp = temp->pred;
		}

		return 0;														//������������ 0 ���� �������������� � ����� ������ �� �������.
	}

	//������ ����� ������� �� �����
	int get_NomerRes(int nomberObj, string str)
	{
		obj* tempObj = Head;
		resources* tempRes;

		int i = 0;														//������� ��������

		//��������� ������ ��������������, ���. ����������� ������
		for (i = 1; i < nomberObj; i++)
		{
			tempObj = tempObj->pred;
		}

		tempRes = tempObj->headRes;
		i = 0;

		while (tempRes)													//���� ���� ������
		{
			i++;

			if (tempRes->nameRes == str) return i;						//������������ ����� �������
			else
				tempRes = tempRes->pred;
		}

		return 0;														//������������ 0 ���� ������ � ����� ������ �� ������.
	}



	//��������, ���� �� ������� � ������
	bool isObj()
	{
		if (Head) { return true; }
		else { return false; }
	}

	//��������, ����-�� ���� �� 1 ������ � �������
	bool isRes(int nomberObj)
	{
		obj* temp = Head;
		for (int i = 1; i < nomberObj; i++)					//��������� ������ �� ������� �������
		{
			temp = temp->pred;
		}

		return temp->isRes();
	}

	//��������, ����-�� ���� �� 1 �������� �������
	bool isDelivery(int nomberObj, int nomberRes)
	{
		obj* temp = Head;
		resources* tempRes;

		for (int i = 1; i < nomberObj; i++)					//��������� ������ �� ������� �������
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;
		for (int i = 1; i < nomberRes; i++)					//��������� ������ �� �������
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
			cout << "\n �������������� ��� ����� ������� �� ����������!";
			return 0;
		}
		else	return 1;
	}

	int checkRes(int nomerObj, int nomerRes)
	{
		obj* temp = Head;
		for (int i = 1; i < nomerObj; i++)					//��������� ������ �� ������� �������
		{
			temp = temp->pred;
		}
		if (nomerRes <= 0 || nomerRes > temp->countRes)
		{
			cout << "\n� �������������� " << temp->name << " ������� ��� ����� ������� �� ����������!";
			return 0;
		}
		else	return 1;
	}

	int checkDeliv(int nomerObj, int nomerRes, int nomerDeliv)
	{
		obj* temp = Head;
		resources* tempRes;
		for (int i = 1; i < nomerObj; i++)					//��������� ������ �� ������� �������
		{
			temp = temp->pred;
		}

		tempRes = temp->headRes;
		for (int i = 1; i < nomerRes; i++)					//��������� ������ �� �������
		{
			tempRes = tempRes->pred;
		}

		if (nomerDeliv <= 0 || nomerDeliv > tempRes->countDeliv)
		{
			cout << "\n �������� ������� " << tempRes->nameRes << "  ��� ����� ������� �� ����������!";
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

	for (i = 1; i < nomber; i++)	//����������� ������ �� ������� �������
	{
		temp = temp->pred;
	}

	if (temp == head && temp->pred == NULL)			//���� ������� ������ � ������������
	{
		head = tail = NULL;
		delete(temp);
	}
	else if (temp == head && temp->pred != NULL)	//������ � �� ������������
	{
		head = head->pred;
		head->next = NULL;
		delete(temp);
	}
	else if (temp == tail && temp->next != NULL)  // ��������� � �� ������������ 
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



////����� ������ �������� � ��������� ������� ��������
//void showAllObjRes()										//������� ��� ������� � �������
//{
//	obj* temp = Head;										//��������� ��������� �� ����� ������� ��������
//
//	if (temp == NULL)	cout << "\n C����� ����!";
//	else while (temp != NULL)								//���� �� �������� ������ ��������
//	{
//		for (int i = 1; i <= countObj; i++)
//		{
//			cout << "\n" << i << ". " << temp->name;        //������� ������ ��������� �������� �� �����
//
//			temp->show();
//
//			temp = temp->pred;								//����� ������ �� ����� ���������� ��������
//		}
//	}
//}
