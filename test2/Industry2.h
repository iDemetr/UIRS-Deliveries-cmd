#include <string>
#include "List.h"

//класс-посредник (по сути происходит только переадресация из cpp в "List")
class Industry2
{
public:

	List* IndL = new List;		//переменная для обращения к классу List

	Industry2() {}

	~Industry2() {}

	void addInd(string newInd)	//примает на вход новое название промышленности и передает в List
	{
		IndL->Add_Obj(newInd);	//вызывает функцию, реализующую линейный список промышленностей	
	}

	void addRes(string newRes, int countInd) { IndL->Add_ObjRes(newRes, countInd); }									//добавление ресурсов

	void addDelivery(int nomerObj, int inObj, int nomerRes) { IndL->Add_delivery(nomerObj, inObj, nomerRes); }			//добавление поставок


	void show() { IndL->Show(); }								//выводит ресурсы

	void show(int nomerInd) { IndL->Show(nomerInd); }			//Выводит выбранный объект и принадлежащие ему ресурсы

	void ShowAll() { IndL->ShowAll(); }


	void dellInd(int nomerInd) { IndL->dell_Obj(nomerInd); }	//удаление промышленности

	void dellRes(int nomerInd, int nomerRes) { IndL->dell_Res(nomerInd, nomerRes); }										//удаление ресурсов

	void dellDeliv(int nomerObj, int nomerRes, int nomerDeliv) { IndL->dell_Deliv(nomerObj, nomerRes, nomerDeliv); }	//удаление поставок


	void OpenFile() { IndL->OpenFile(); }

	void SaveFile() { IndL->SaveFile(); }


	int get_NomberObj(string str) { return IndL->get_NomberObj(str); }

	int get_NomerRes(int nomberObj, string str) { return IndL->get_NomerRes(nomberObj, str); }

	//Проверка, есть ли несохранённые данные
	bool isdontSaveData()
	{
		return IndL->isdontSaveData;
	} 

	//Проверка, есть-ли хотя бы 1 объекты в списке
	bool isObj() { return IndL->isObj(); }

	//Проверка, есть-ли хотя бы 1 ресурс в объекте
	bool isRes(int nomberObj) { return IndL->isRes(nomberObj); }

	//Проверка, есть-ли хотя бы 1 поставка ресурса
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


	/*void showIndRes()	//выводит все объекты и ресурсы
	{
		//IndL->showAllObjRes();
		IndL->ShowAll();
	}*/

	/*void ShowDeliv() //выводит поставки
	{
		//IndL->ShowAllDeliv();
		IndL->ShowAll();
	}*/
