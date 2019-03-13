#include<iostream>
#include<fstream>
#include<iomanip>
#include<math.h>
#define n 251
using namespace std;
struct Gailv{//用链表存储概率分布，a表示面积值，p表示a出现的概率
	double a;
	double p;
	Gailv *next;
};
class Prdistribution{
public:
	Gailv *head;//头结点不存储数据
	Prdistribution(){//构造函数
		head = new Gailv; 
		head->next = NULL;//尾结点为空，此其标志也
	}
	Prdistribution& operator=(const Prdistribution& c) {
		destruct();//摧毁
		//重建
		Gailv *temp, *tempc;
		for (temp = head->next, tempc = c.head->next; tempc != NULL; temp = temp->next,tempc=tempc->next) {
			temp = new Gailv;
			temp->a = tempc->a;
			temp->p = tempc->p;
			
		}
		temp = NULL;//尾置空
		return *this;
	}
	/*-----------------插入结点--------------------*/
	void insert(double a0, double p0){
		Gailv *temp;
		for (temp = head; temp->next != NULL && temp->next->a < a0; temp = temp->next);
		
		if (temp->next == NULL){//皆比插入结点小(严格小)，故置之尾
			Gailv *newGailv = new Gailv;
			newGailv->a = a0;
			newGailv->p = p0;

			newGailv->next = temp->next;//插入
			temp->next = newGailv;
		}
		else if (temp->next->a == a0){//若有相等，则加概率也
			temp->next->p += p0;
		}
		else {//最后一种情况则处于两结点之间也，故插于二者之间
			Gailv *newGailv = new Gailv;
			newGailv->a = a0;
			newGailv->p = p0;

			newGailv->next = temp->next;
			temp->next = newGailv;
		}

	}
	/*-----------------论文中的并运算--------------------*/
	void unions(Prdistribution *c){
		Gailv * temp = head, *ctemp = c->head;
		Gailv * nextCTemp, *nextTemp;
		Gailv * wei;
		while (ctemp->next != NULL){
			for (;temp->next != NULL && temp->next->a < ctemp->next->a; temp = temp->next);
			if (temp->next == NULL) {//头接尾
				temp->next = ctemp->next;
				return;
			}
			else {
				if (temp->next->a == ctemp->next->a) {
					temp->next->p += ctemp->next->p;
					temp = temp->next;
					//删除节点
					Gailv *delNode = ctemp->next;
					ctemp->next = ctemp->next->next;
					delete delNode;
				}
				else  {
					for ( wei = ctemp->next; wei->next != NULL && wei->next->a < temp->next->a; wei = wei->next);
					nextTemp = temp->next;
					nextCTemp = wei->next;
					wei->next = temp->next;
					temp->next = ctemp->next;
					ctemp->next = nextCTemp;
					temp = wei;
					//Gailv * newNode = new Gailv;
					//newNode->a = ctemp->next->a;
					//newNode->p = ctemp->next->p;
					//newNode->next = temp->next;
					//temp->next = newNode;
					//temp = temp->next;
					////删除节点
					//Gailv *delNode = ctemp->next;
					//ctemp->next = ctemp->next->next;
					//delete delNode;
				}
			}
		}
		delete c->head;

	}
	/*-----------------论文中的o运算--------------------*/
	Prdistribution* dot(double a0, double p0){
		Gailv *temp, *ctemp;
		Prdistribution *c = new Prdistribution; 
		ctemp = c->head;
		for (temp = head->next; temp != NULL; temp = temp->next){
			Gailv* newNode = new Gailv;
			newNode->a = temp->a + a0;
			newNode->p = temp->p * p0;
			//将newNode插入尾部
			newNode->next = ctemp->next;
			ctemp->next = newNode;
			
			ctemp = ctemp->next;
		}
		return c;
	}
	/*-----------------数学期望--------------------*/
	double Expectation(){
		double E = 0;
		Gailv *temp;
		for (temp = head->next; temp != NULL; temp = temp->next){
			E += temp->a * temp->p;
		}
		return E;
	}
	/*---------------众数，概率最大的数值------------------------*/
	double Mode(){
		double a0 = head->next->a, p0 = head->next->p;
		Gailv *temp;
		for (temp = head->next->next; temp != NULL; temp = temp->next){
			if (temp->p > p0){
				p0 = temp->p;
				a0 = temp->a;
			}
		}
		return a0;
	}
	/*---------------中位数------------------------*/
	double Median(){
		double sum = 0;
		Gailv* temp;
		for (temp = head; temp->next != NULL; temp = temp->next){
			sum += temp->next->p;
			if (sum >= 0.5)break;
		}
		cout << "至此概率和为：" << sum << endl;
		return temp->next->a;
	}
	/*---------------方差------------------------*/
	double Variance(){
		double sum = 0, e = Expectation();
		Gailv *temp;
		for (temp = head->next; temp != NULL; temp = temp->next){
			sum += temp->p * (temp->a - e) * (temp->a - e);
		}
		return sum;
	}
	/*---------------上a分位点------------------------*/
	double Upper(double a){
		double  sum = 0;
		Gailv* temp;
		for (temp = head->next; temp != NULL; temp = temp->next){
			sum += temp->p;
			if (sum >= 1 - a)break;
		}
		return temp->a;
	}
	/* -----------------概率分布输出 --------------- */
	void print(){
		Gailv *temp;
		int c = 1;
		for (temp = head->next; temp != NULL; temp = temp->next){
			cout << "(" << temp->a << ", " << temp->p << ")" << "  ";
			if (c++ % 5 == 0)cout << endl;
		}
		cout << endl;
	}
	/* -----------------检验 --------------- */
	void check(double sump){
		Gailv* temp;
		double sum = 0;
		for (temp = head->next; temp != NULL; temp=temp->next)sum += temp->p;
		cout << sum << endl;
		if (sum != sump)cout << "Error Result!\n";
		else cout << "Right.\n";
	}
	/* -----------------摧毁 --------------- */
	void destruct(){
		Gailv *del;
		while (head->next != NULL){
			del = head->next;
			head->next = head->next->next;
			delete del;
		}
	}
};

int main(){
	int M;

	Prdistribution F1[n];
	F1[0].insert(0, 1);
	for (M = 1; M < n; M++){
		int k;
		for (k = 1; k <= M; k++){
			F1[M].unions(F1[M - k].dot(k*k / 2.0, pow(0.5, k)));
		}
	}

	Prdistribution F2[n];
	F2[0].insert(0, 1);
	for (M = 1; M < n; M++){
		int k;
		for (k = 1; k <= M; k++){
			F2[M].unions(F2[M - k].dot(k*(2*k*k+1)/12.0, pow(0.5,k)));
		}
	}

	Prdistribution G1[n];
	G1[0].insert(0, 1);
	for (M = 1; M < n; M++){
		int k;
		for (k = 1; k <= M; k++)    G1[M].unions(G1[M - k].dot(k*k / 2.0, pow(0.5, k)));
		for (; k < 2 * M; k++)   G1[M].insert((k*k - (k - M)*(2 * (k - M) - 1)) / 2.0, pow(0.5, k));
		G1[M].insert((k * k - (k - M)*(2 * (k - M) - 1)) / 2.0, pow(0.5, k - 1));
	}

	Prdistribution G2[n];
	G2[0].insert(0, 1);
	for (M = 1; M < n; M++){
		int k;
		for (k = 1; k <= M; k++)    G2[M].unions(G2[M - k].dot(k*(2 * k*k + 1) / 12.0, pow(0.5, k)));
		for (; k < 2 * M; k++)   G2[M].insert((k*(2 * k*k + 1) - (k - M)*(2 * (k - M) - 1)*(4 * (k - M) - 1)) / 12.0, pow(0.5, k));
		G2[M].insert((k*(2 * k*k + 1) - (k - M)*(2 * (k - M) - 1)*(4 * (k - M) - 1)) / 12.0, pow(0.5, k - 1));
	}

	

	return 0;
}