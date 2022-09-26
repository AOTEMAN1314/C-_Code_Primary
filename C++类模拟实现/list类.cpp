#include "iostream"
using namespace std;

//结点类
template<typename T>
struct ListNode {
	ListNode<T>* prev;//指向前一个结点
	ListNode<T>* next;//指向后一个结点
	T value;//该结点的值域，里面可能保存内置类型的元素
			//也可能保存自定义类型的对象，如日期类对象

	ListNode(const T& _value = T())
		:prev(nullptr)
		,next(nullptr)
		,value(_value)
	{}
};

//=======================================================
// 迭代器模块

//正向迭代器类
template<typename T, typename Ref, typename Ptr>
struct ListIterator {
	typedef ListNode<T> Node;//为结点类取别名
	typedef ListIterator<T, Ref, Ptr> Self;//为正向迭代器类取别名

	Node* pNode;//指向结点的指针

	//正向迭代器构造函数
	ListIterator(Node* _pNode = nullptr)
		:pNode(_pNode)
	{}
	//重载*
	//返回迭代器指向的结点的值域
	Ref operator*() {
		return pNode->value;
	}
	//重载->
	//返回迭代器指向的结点值域的地址
	Ptr operator->() {
		return &(operator*());
	}
	//重载前置++
	//返回迭代器对象自身的引用
	//因为对象自身并不是该函数中的局部对象
	Self& operator++() {
		pNode = pNode->next;
		return *this;
	}
	//重载后置++
	//此时需要返回temp对象，而不是引用
	//因为temp对象是局部的对象
	//函数结束后就被释放
	Self operator++(int a) {
		Self temp(*this);
		pNode = pNode->next;
		return temp;
	}
	//重载前置--
	Self& operator--() {
		pNode = pNode->prev;
		return *this;
	}
	//重载后置--
	Self operator--(int a) {
		Self temp(*this);
		pNode = pNode->prev;
		return temp;
	}
	//重载!=
	bool operator!=(const Self& s)const {
		return pNode != s.pNode;
	}
	//重载==
	bool operator==(const Self& s)const {
		return pNode == s.pNode;
	}
};

//反向迭代器类
template<typename T, typename Ref, typename Ptr>
struct RListIterator {
	typedef ListNode<T> Node;
	typedef RListIterator<T, Ref, Ptr> Self;
	Node* pNode;
	//反向迭代器构造函数
	RListIterator(Node* _pNode = nullptr)
		:pNode(_pNode)
	{}
	//重载*
	Ref operator*() {
		return pNode->value;
	}
	//重载->
	Ptr operator->() {
		return &(operator*());
	}
	//重载前置++
	//反向迭代器的自增其实是反方向的自增
	//所以迭代器其实是向该节点的prev指针域滚动
	Self& operator++() {
		pNode = pNode->prev;
		return *this;
	}
	//重载后置++
	Self operator++(int a) {
		Self temp(*this);
		pNode = pNode->prev;
		return temp;
	}
	//重载前置--
	//反向迭代器的自减
	//其实就是正向迭代器的自增
	Self& operator--() {
		pNode = pNode->next;
		return *this;
	}
	//重载后置--
	Self operator--(int a) {
		Self temp(*this);
		pNode = pNode->next;
		return temp;
	}
	//重载!=
	bool operator!=(const Self& s)const {
		return pNode != s.pNode;
	}
	//重载==
	bool operator==(const Self& s)const {
		return pNode == s.pNode;
	}
};

//带头结点的双向链表
template<typename T>
class List {
	typedef ListNode<T> Node;
	typedef ListIterator<T, T&, T*> Iterator;//正向迭代器
	typedef RListIterator<T, T&, T*> RIterator;//反向迭代器
private:
	Node* Head;//指向头结点的指针

	//为list对象创建头结点的函数
	void CreateHead() {
		Head = new Node();
		Head->next = Head;
		Head->prev = Head;
	}
public:
//============================================
//构造模块
	//无参构造
	List() {
		CreateHead();
	}
	//n个相同元素构造
	List(int n, const T& _value = T()) {
		CreateHead();
		for (int i = 0; i < n; i++) {
			Push_back(_value);
		}
	}
	//区间构造
	//这里需要用到函数模板
	//因为list可以存储各种类型的元素
	template <typename iterator>
	List(iterator first, iterator last) {
		CreateHead();
		while (first != last) {
			Push_back(*first);
			first++;
		}
	}
	//拷贝构造
	List(List<T>& L) {
		CreateHead();
		auto it = L.Begin();
		while (it != L.Head) {
			Push_back(*it);
			it++;
		}
	}
	//赋值运算符重载
	//此处的参数是值传递
	//所以只需要this指针和参数对象的指针交换指向的空间即可
	//当函数结束后，参数对象也会被销毁
	List<T>& operator=(const List<T> L) {
		std::swap(this->Head, L.Head);
		return *this;
	}
	//析构函数
	~List() {
		Clear();
		delete Head;
		Head = nullptr;
	}

//===============================================
//容量模块
	//容量函数
	size_t Size()const {
		size_t count = 0;
		auto it = Begin();
		while (it != End()) {
			count++;
			it++;
		}
		return count;
	}
	//判空函数
	//因为头结点申请时
	//next指针域指向头结点
	//此时list是空的
	bool Empty()const {
		return Head->next == Head;
	}
	//设置有效元素个数
	void Resize(size_t newSize, const T& _value = T()) {
		size_t oldSize = Size();
		//新的元素个数小于旧的元素个数
		//即：缩减list的有效元素个数
		//将多余的元素尾删
		if (newSize <= oldSize) {
			for (int i = 0; i < oldSize - newSize; i++)
				Pop_back();
		}
		//新的元素个数大于旧元素个数
		//将多余空间尾插value元素
		else {
			for (size_t i = oldSize; i < newSize; i++) {
				Push_back(_value);
			}
		}
	}
//===============================================
//迭代器模块
	//正向迭代器
	Iterator Begin() const {
		return Iterator(Head->next);
	}
	Iterator End() const {
		return Iterator(Head);
	}

	//反向迭代器
	RIterator Rbegin() const {
		return RIterator(Head->prev);
	}
	RIterator Rend() const {
		return RIterator(Head);
	}
	
//===============================================
//增删改查模块

	//任意位置的插入
	//将value元素插在it迭代器对象的位置
	//此处的 Iterator 是迭代器类
	//it 是迭代器对象
	Iterator Insert(Iterator it, const T& value) {
		//用pPos指针保存迭代器对象中的结点地址
		Node* pPos = it.pNode;
		//用value创建一个结点对象
		//用pNewNode指向该结点对象
		Node* pNewNode = new Node(value);

		pNewNode->next = pPos;
		pNewNode->prev = pPos->prev;

		pPos->prev->next = pNewNode;
		pPos->prev = pNewNode;

		return pNewNode;
	}
	//任意位置的删除
	//返回被删除结点下一个结点的迭代器
	Iterator Erase(Iterator it) {
		if (it == End())
			return End();

		Node* pPos = it.pNode;
		Node* pTemp = pPos->next;

		pPos->prev->next = pTemp;
		pTemp->prev = pPos->prev;

		delete pPos;
		return pTemp;
	}
	//尾插
	//也就是在End迭代器的位置插入value
	//所以可以直接复用Insert函数
	void Push_back(const T& value) {
		Insert(End(), value);
	}
	//头插
	//即在Begin迭代器的位置插入value
	void Push_front(const T& value) {
		Insert(Begin(), value);
	}
	//尾删
	void Pop_back() {
		Erase(--End());
	}
	//头删
	void Pop_front() {
		Erase(Begin());
	}
	//清空
	void Clear() {
		auto it = Begin();
		while (it != End()) {
			it = Erase(it);
		}
	}
	//获取首元素
	T& Front() const {
		return *Begin();
	}
	//获取尾部元素
	T& Back()const {
		return *(--End());
	}
};

int main() {}
