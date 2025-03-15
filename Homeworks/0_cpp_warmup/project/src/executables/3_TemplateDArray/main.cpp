#include "TemplateDArray.h"

int main(int argc, char** argv)
{
	TemplateDArray<double> a;
	a.InsertAt(0, 2.1);
	a.Print();

	a.PushBack(3.0);
	a.PushBack(3.1);
	a.PushBack(3.2);
	a.Print();

	a.DeleteAt(0);
	a.Print();
	a.InsertAt(0, 4.1);
	a.Print();

	TemplateDArray<double> acopy = a; // �˴��õ��˿������캯��
	acopy.Print();

	TemplateDArray<double> acopy2(a); // ������ͬ���������䣬���ǳ�ʼ��
	acopy2.Print();

	TemplateDArray<double> acopy3, acopy4;
	acopy4 = acopy3 = a; // �˴��õ��˸�ֵ��������"="������
	acopy3.Print();

	TemplateDArray<int> b;
	b.PushBack(21);
	b.Print();

	b.DeleteAt(0);
	b.Print();

	b.PushBack(22);
	b.SetSize(5);
	b.Print();

	TemplateDArray<char> c;
	c.PushBack('a');
	c.PushBack('b');
	c.PushBack('c');
	c.InsertAt(0, 'd');
	c.Print();
}