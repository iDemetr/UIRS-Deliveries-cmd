#include <string>
#include "List.h"

//�����-��������� (�� ���� ���������� ������ ������������� �� cpp � "List")
class Industry2
{
public:

	List* IndL = new List;		//���������� ��� ��������� � ������ List

	Industry2() {}

	~Industry2() {}

	void addInd(string newInd)	//������� �� ���� ����� �������� �������������� � �������� � List
	{
		IndL->Add_Obj(newInd);	//�������� �������, ����������� �������� ������ ���������������	
	}

	void addRes(string newRes, int countInd) { IndL->Add_ObjRes(newRes, countInd); }									//���������� ��������

	void addDelivery(int nomerObj, int inObj, int nomerRes) { IndL->Add_delivery(nomerObj, inObj, nomerRes); }			//���������� ��������


	void show() { IndL->Show(); }								//������� �������

	void show(int nomerInd) { IndL->Show(nomerInd); }			//������� ��������� ������ � ������������� ��� �������

	void ShowAll() { IndL->ShowAll(); }


	void dellInd(int nomerInd) { IndL->dell_Obj(nomerInd); }	//�������� ��������������

	void dellRes(int nomerInd, int nomerRes) { IndL->dell_Res(nomerInd, nomerRes); }										//�������� ��������

	void dellDeliv(int nomerObj, int nomerRes, int nomerDeliv) { IndL->dell_Deliv(nomerObj, nomerRes, nomerDeliv); }	//�������� ��������


	void OpenFile() { IndL->OpenFile(); }

	void SaveFile() { IndL->SaveFile(); }


	int get_NomberObj(string str) { return IndL->get_NomberObj(str); }

	int get_NomerRes(int nomberObj, string str) { return IndL->get_NomerRes(nomberObj, str); }

	//��������, ���� �� ������������ ������
	bool isdontSaveData()
	{
		return IndL->isdontSaveData;
	} 

	//��������, ����-�� ���� �� 1 ������� � ������
	bool isObj() { return IndL->isObj(); }

	//��������, ����-�� ���� �� 1 ������ � �������
	bool isRes(int nomberObj) { return IndL->isRes(nomberObj); }

	//��������, ����-�� ���� �� 1 �������� �������
	bool isDelivery(int nomberObj, int nomberRes) { return IndL->isDelivery(nomberObj, nomberRes); }

	//int checkInd(int nomerObj)
	//{
	//	return IndL->checkInd(nomerObj);
	//}	
	
	//int checkRes(int nomerObj,int nomerRes)
	//{		
	//	return IndL->checkRes(nomerObj, nomerRes);
	//}	
	
	//int checkDeliv(int nomerObj, int nomerRes, int nomerDeliv)
	//{
	//	return IndL->checkDeliv(nomerObj, nomerRes, nomerDeliv);
	//}
};


	/*void showIndRes()	//������� ��� ������� � �������
	{
		//IndL->showAllObjRes();
		IndL->ShowAll();
	}*/

	/*void ShowDeliv() //������� ��������
	{
		//IndL->ShowAllDeliv();
		IndL->ShowAll();
	}*/
