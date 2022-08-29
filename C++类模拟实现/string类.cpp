#define _CRT_SECURE_NO_WARNINGS //解决C语言函数报错
#include "iostream"
#include<assert.h>
using namespace std;

class String {
private:
	char* Str;//字符串指针
	size_t Size;//有效元素个数
	size_t Capacity;//容量
	static size_t npos;
public:
//构造模块
/////////////////////////////////////////////////////////////////////////////////////////////
	//构造函数1
	String(const char* s = "") {
		//将指向空的指针指向空字符串
		if (s == nullptr)
			s = "";
		//注意要比申请比字符串长度多1的空间来存储 '\0'
		Str = new char[strlen(s) + 1];
		//将s指向的空间中的内容拷贝到Str指向的空间中，这个操作会把'\0'一并拷贝
		strcpy(Str, s);
		//更新有效元素个数和容量
		Size = strlen(s);
		Capacity = strlen(s)+1;
	}
	//构造函数2
	String(size_t n, char c) {
		//申请比有效元素多一个的空间
		Str = new char[n + 1];
		//把n个c存入Str指向的空间中
		memset(Str, c, n);
		//不要忘记设置'\0'
		Str[n] = '\0';
		//更新有效元素和容量
		//容量要保持比有效元素个数多
		//用来存储'\0'
		Size = n;
		Capacity = n+1;
	}
	//析构函数
	~String() {
		//如果指向空值，释放程序会崩溃的
		if (Str)
			delete[] Str;
		Str = nullptr;
		Size = 0;
		Capacity = 0;
	}

	//传统版拷贝构造
	/*
	String(const String& s) {
		//需要释放原本的空间
		if (Str) {
			delete[] Str;
		}
		Str = new char[strlen(s.Str) + 1];
		strcpy(Str, s.Str);
		Size = strlen(Str);
		Capacity = Size+1;
	}
	*/
	//简洁版拷贝构造
	String(const String& s) {
		String temp(s.Str);
		//交换temp.Str和this->Str指向的空间
		std::swap(Str, temp.Str);
		Size = strlen(Str);
		Capacity = Size+1;
	}

	//传统版赋值运算符重载
	/*
	String& operator=(const String& s) {
		//判断是否是自己给自己赋值
		if (this != &s) {
			String temp(Str);
			//记得释放原本指向的空间
			delete[] Str;
			Str = new char[strlen(s.Str) + 1];
			strcpy(Str, s.Str);
			Size = strlen(s.Str);
			Capacity = Size + 1;
		}
		return *this;
	}
	*/

	//简洁版赋值运算符重载
	String& operator=(const String& s) {
		String temp(s.Str);
		//交换空间
		std::swap(Str, temp.Str);
		Size = strlen(s.Str);
		Capacity = Size +1;
		return *this;
	}

//容量模块
/////////////////////////////////////////////////////////////////////////////////////////////
	//有效元素
	size_t SIZE() {
		return Size;
	}

	//容量
	size_t CAPAcity() {
		return Capacity;
	}

	//扩容函数
	void Reverse(size_t newCapacity) {
		//新容量大于旧容量
		if (newCapacity >= Capacity) {
			char* temp = new char[newCapacity *2];
			strcpy(temp, Str);
			delete[] Str;
			Str = temp;
			Capacity = 2 * newCapacity;
		}
	}

	//设置有效元素
	void Resize(size_t newSize, char c) {
		size_t oldSize = Size;
		size_t oldCapa = Capacity;
		//新的有效元素比旧的多
		if (newSize > oldSize) {
			//新元素比容量多 扩容
			if (newSize > oldCapa) {
				Reverse(newSize);
			}
			//往Str尾部插入合适数量的c
			memset(Str + Size, c, newSize - oldSize);
		}
		Size = newSize;
		Str[Size] = '\0';
	}

	//判空函数
	bool Empty() {
		//容量等于0返回true
		return Size == 0;
	}

	//清空函数
	void clear() {
		//只改变有效元素个数，不改变容量
		Size = 0;
		Str[0] = '\0';
	}
//迭代器模块
/////////////////////////////////////////////////////////////////////////////////////////////
	typedef char* Iterator;
	//获取正向首指针
	Iterator Begin() {
		return Str;
	}

	//获取正向尾指针
	Iterator End() {
		return Str + Size;
	}
	typedef char* Iterator;
	//获取反向首指针
	Iterator Rbegin() {
		return End();
	}

	//获取反向尾指针
	Iterator Rend() {
		return Begin();
	}
//访问模块
/////////////////////////////////////////////////////////////////////////////////////////////
	//普通[]重载
	char& operator[](size_t pos) {
		assert(pos < Size);
		return Str[pos];
	}
	//const[]重载
	const char& operator[](size_t pos)const {
		assert(pos < Size);
		return Str[pos];
	}
	// <<重载
	friend ostream& operator<<(ostream& _cout, const String& s) {
		_cout << s.Str;
		return _cout;
	}
//访问模块
/////////////////////////////////////////////////////////////////////////////////////////////
	//尾插
	void Push_back(char c) {
		//满足条件说明空间已经存满了，因为那个1是用来存储'\0'的
		if (Size + 1== Capacity) {
			Reverse(Capacity);
		}
		Str[Size] = c;
		Str[Size+1] = '\0';
		Size++;
	}
	//+=重载
	void operator+=(const char* s) {
		size_t oldSize = Size;
		//两个空间加起来元素个数
		size_t size = Size + strlen(s);
		//元素个数接近容量
		if (size+1 >= Capacity) {
			Reverse(size);
		}
		strcat(Str + oldSize, s);
		Size = size;
	}
	//+=重载
	String& operator+=(const String& s) {
		*this += s.Str;
		return *this;
	}
//特殊操作模块
/////////////////////////////////////////////////////////////////////////////////////////////
	//转换C语言字符串
	const char* c_str()const
	{
		return Str;
	}

	//复制
	String Substr(size_t pos = 0, size_t n = npos) {
		if (n == npos)
			n = Size;
		if (pos + n >= Size) {
			n =Size - pos;
		}
		char* temp = new char[n + 1];
		strncpy(temp, Str + pos, n);
		temp[Size] = '\0';

		String Stemp(temp);
		delete[] temp;
		return Stemp;
	}
	//正向查找
	size_t Find(char c, size_t pos = 0) {
		for (int i = pos; i < Size; i++) {
			if (Str[i] == c)
				return i;
		}
		return npos;
	}
	//反向查找
	size_t Rfind(char c, size_t pos = npos) {
		pos = pos < Size ? pos : Size;
		for (int i = pos; i >= 0; i--) {
			if (Str[i] == c) {
				return i;
			}
		}
		return npos;
	}
};

size_t String::npos = -1;

int main() {
	const char* sp = "abc12345566743212343455655";
	String s1(sp);
	String s2 = s1;
	auto it = s2.Begin();
	while (it != s2.End()) {
		cout << *it << " ";
		it++;
	}
}