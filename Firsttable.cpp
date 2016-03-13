#include <iostream>
#include <stdio.h>
#include <deque>
using namespace std;     
template <class T>
class Cell;
template <class P>        
class St;
template <class T>
T rec(Cell<T> *ptr, St<Cell<T>*> &st);
template <class T>
class SuperCalc;
template <class P>
class St {
	public:
		deque<pair<P, char>> st;   
		St(): st() {
			
		}
		St(const St &elem) {
			this->st=elem.st;
		}
		void take(P x) {
			st.push_back(make_pair(x,0));
		}
		void up(const P x) {
			st.push_front(make_pair(x,0));
		}
		St(char op, const St<P> &first, const St<P> &second) {
			auto temp=first.st.back();
			for (auto x : second.st)
				st.push_back(x);
			for (auto x : first.st) 
				st.push_back(x);
			st.push_back(make_pair(temp.first,op));
		}
		void pop(char op, St<P> &elem) {
			auto temp=st.back();
			for (auto i=elem.st.rend(); i!=elem.st.rbegin(); i--) 
				st.push_front(*i);
			st.push_back(make_pair(temp.first,op));
		}
		pair<P,char> pop() {
			auto t=st.back();
			st.pop_back();
			return t;
		}
		bool empty() {
			return st.empty();
			
		}
		void Out() {
			cout << "st: ";
			for (auto x : st) 
				if (x.second !=0) 
					cout << x.second << " ";
				else 
					cout << x.first->value << " ";
			cout << endl;
		}
};
     
template <class T>
class Cell {
	public:
		T value;
		bool sost;
		St<Cell<T>*> arith;                   
		void Out() {
			arith.Out();
			
		}
		Cell(): arith() {
			sost=true;
			arith.take(this);
		}
		Cell(const T &v): arith() {
			value=v;
			sost=true;
			arith.take(this);
		}
		Cell(St<Cell<T>*> f) {
			arith.st=f.st;
			sost=false;
		}
		void check() {
			sost=false;
			
		}
		operator T() const {
			return calculation(*this, arith);
		}
		Cell& operator=(const T &v) {
			value=v;
			arith.st.clear();
			arith.take(this);
			return *this;
		}
		Cell& operator=(const Cell &elem) {
			if (this != &elem) {
				if (elem.sost) {
					arith.st.clear();
					arith.take(const_cast<Cell<T>*>(&elem));
				} 
				else arith.st=elem.arith.st;
			}
			return *this;
		}
		void CellHelp(const Cell &elem, char op) {
			int k=0;
			if(k==0)
			{
				if (elem.sost) {
					arith.st.push_front(make_pair(const_cast<Cell<T>*>(&elem), 0));
					arith.st.push_back(make_pair(this, op));
				} 
				else if (k==0){
					arith.pop(op, const_cast<St<Cell<T>*>&>(elem.arith));
			}
			else
				if (elem.sost) {
					arith.st.push_front(make_pair(const_cast<Cell<T>*>(&elem), 0));
					arith.st.push_back(make_pair(this, op));
				} 
				else if (k==0){
					arith.pop(op, const_cast<St<Cell<T>*>&>(elem.arith));
				}
			}
		}
			
		void Number(const T &v, char op) {
			Cell<T> *elem=new Cell<T>(v);
			SuperCalc<T>::Destr(elem);
			arith.st.push_front(make_pair(const_cast<Cell<T>*>(elem), 0));
			arith.st.push_back(make_pair(this, op));
		}
		St<Cell<T>*> Operation1(const Cell &elem, char op) const {
			St<Cell<T>*> a1, b1;
			if (this->sost) 
				a1.take(const_cast<Cell<T>*>(this));
			else 
				a1.st=this->arith.st;
			if (elem.sost) 
				b1.take(const_cast<Cell<T>*>(&elem));
			else 
				b1.st=elem.arith.st;
			return St<Cell<T>*>(op , a1 , b1);
		}
		St<Cell<T>*> Operation2(const T &v, char op) const {
			int debag=0;
			St<Cell<T>*> a1, b1;
			if (this->sost)
				a1.take(const_cast<Cell<T>*>(this));
			else
				a1.st=this->arith.st;
			if(debag==0)
			{
				Cell<T> *elem=new Cell<T>(v);
				SuperCalc<T>::Destr(elem);
				b1.take(elem);
			}
			else
			{
				Cell<T> *elem=new Cell<T>(v);
				SuperCalc<T>::Destr(elem);
				b1.take(elem);
			}
			return St<Cell<T>*>(op , a1 , b1);
		}
		Cell operator-() {
			if (this->sost) {
				St<Cell<T>*> temp;
				temp.take(const_cast<Cell<T>*>(this));
				temp.st.push_back(make_pair(const_cast<Cell<T>*>(this), '#'));
				return Cell<T>(temp);
			} else {
				this->arith.st.push_back(make_pair(const_cast<Cell<T>*>(this), '#'));
				return *this;
			}
		}
		Cell& operator+=(const Cell &elem) {
			CellHelp(elem, '+');
			return *this;
		}
		Cell& operator+=(const T &v) {
			Number(v, '+');
			return *this;
		}
		Cell& operator-=(const Cell &elem) {
			CellHelp(elem, '-');
			return *this;
		}
		Cell& operator-=(const T &v) {
			Number(v, '-');
			return *this;
		}
		Cell& operator*=(const Cell &elem) {
			CellHelp(elem, '*');
			return *this;
		}
		Cell& operator*=(const T &v) {
			Number(v, '*');
			return *this;
		}
		Cell& operator/=(const Cell &elem) {
			CellHelp(elem, '/');
			return *this;
		}
		Cell& operator/=(const T &v) {
			Number(v, '/');
			return *this;
		}
		const Cell operator+(const Cell<T> &elem) const {
			return Cell<T>(this->Operation1(elem, '+'));
		}
		const Cell operator+(const T &v) const {
			return Cell<T>(this->Operation2(v, '+'));
		}
		const Cell operator-(const Cell<T> &elem) const {
			return Cell<T>(this->Operation1(elem, '-'));
		}
		const Cell operator-(const T &v) const {
			return Cell<T>(this->Operation2(v, '-'));
		}
		const Cell operator*(const Cell<T> &elem) const {
			return Cell<T>(this->Operation1(elem, '*'));
		}
		const Cell operator*(const T &v) const {
			return Cell<T>(this->Operation2(v, '*'));
		}
		const Cell operator/(const Cell<T> &elem) const {
			return Cell<T>(this->Operation1(elem, '/'));
		}
		const Cell operator/(const T &v) const {
			return Cell<T>(this->Operation2(v, '/'));
		}
};
     
template<class T>
St<Cell<T>*> Operat(const T &v, char op, const Cell<T> &t) {
	int k=0;
	St<Cell<T>*> a1, b1;
	if (t.sost)
		b1.take(const_cast<Cell<T>*>(&t));
	else 
		if(k==0)
			b1.st=t.arith.st;
		else
			b1.st=t.arith.st;
	Cell<T> *elem=new Cell<T>(v);
	SuperCalc<T>::Destr(elem);
	if(k==0)
	{
		a1.take(elem);
		return St<Cell<T>*>(op , a1 , b1);
	}
	else
	{
		a1.take(elem);
		return St<Cell<T>*>(op , a1 , b1);
	}
}
template<class T>
Cell<T> operator+(const T &v, const Cell<T> &elem) {
	return Cell<T>(Operat(v, '+', elem));
}
template<class T>
Cell<T> operator-(const T &v, const Cell<T> &elem) {
	return Cell<T>(Operat(v, '-', elem));
}
template<class T>
Cell<T> operator*(const T &v, const Cell<T> &elem) {
	return Cell<T>(Operat(v, '*', elem));
	
}
template<class T>
Cell<T> operator/(const T &v, const Cell<T> &elem) {
	return Cell<T>(Operat(v, '/', elem));
}
     
     
template <class T>
T calculation(const Cell<T> &ptr, St<Cell<T>*> st) {
	return rec(ptr,st);
}
template <class T>
T rec(const Cell<T> &ptr, St<Cell<T>*> &st) {
	auto pr=st.pop();
	if (pr.second != 0) {
		if (pr.second=='#') 
			return -rec(ptr,st);
		T a=rec(ptr,st);
		T b=rec(ptr,st);
		if (pr.second != 0){
			if(pr.second=='+')
				return a+b;
			if(pr.second=='-')
				return a-b;
			if(pr.second=='*')
				return a*b;
			if(pr.second=='/')
				return a/b;
		}
		else{
			if(pr.second=='+')
				return a+b;
			if(pr.second=='-')
				return a-b;
			if(pr.second=='*')
				return a*b;
			if(pr.second=='/')
				return a/b;
		}
	} 
	else 
	{
		if (pr.second==0){
			if (pr.first->sost) {
				if (pr.first==&ptr) {
					return pr.first->value;
				}
				return calculation(*(pr.first), pr.first->arith);
			} else
				return pr.first->value;
		}
		else{
			if (pr.first->sost) {
				if (pr.first==&ptr) {
					return pr.first->value;
				}
				return calculation(*(pr.first), pr.first->arith);
			} else
				return pr.first->value;
		}		
	}     
}
     
template <class T>
class SuperCalc {
	private:
		Cell<T> **data;
		int n, m;
	public:
		static deque<Cell<T>*>& getNumbers() {
			static deque<Cell<T>*> Numbers;
			return Numbers;
		}
		static void Destr(Cell<T> *in) {
			deque<Cell<T>*> &temp=getNumbers();
			temp.push_back(in);
		}
		SuperCalc(int n, int m) {
			this->n=n;
			int k=0;
			this->m=m;
			data=new Cell<T>*[n];
			if(k==0)
				for (int i=0; i<n; i++) 
					data[i]=new Cell<T>[m];
		}
		~SuperCalc() {
			for (int i=0; i<n; i++) 
				delete[] data[i];
			delete[] data;
			deque<Cell<T>*> &temp=getNumbers();
			for (auto ptr : temp) 
				delete ptr;
		}
		Cell<T>& operator() (int i, int j) {
			return data[i][j];
		}
};
