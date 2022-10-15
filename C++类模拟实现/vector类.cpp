#include <assert.h>
#include "iostream"
using namespace std;

//Vector类存储的数据是任意类型，所以需要设置模板参数
template<typename T>
class Vector {
private:
	T* _start;//指向T类型元素的首元素的指针
	T* _finish;//指向最后一个有效元素下一个位置
	T* _end;//指向有效空间的下一个位置。
public:
	//构造模块
	//////////////////////////////////////////////////////////////////////////////////
		//无参构造
	Vector()
		:_start(nullptr)
		, _finish(nullptr)
		, _end(nullptr)
	{}
	//半缺省构造
	Vector(int n, const T& val = T())
		:_start(new T[n])//在初始化时直接从堆上申请空间
	{
		for (int i = 0; i < n; i++) {
			_start[i] = val;
		}
		//_finish指向最后一个有效元素的下一个位置
		_finish = _start + n;
		//_end指向顺序表最后一个位置的下一个位置
		_end = _finish;
	}
	//区间构造
	template<typename Iterator>
	Vector(Iterator first, Iterator last) {
		Iterator it = first;
		size_t num = 0;//统计元素个数
		while (it != last) {
			num++;
			it++;
		}

		_start = new T[num];
		_finish = _start;
		while (first != last) {
			*_finish = *first;
			_finish++;
			first++;
		}
		_end = _finish;
	}
	//拷贝构造函数
	Vector(const Vector<T>& v)
		:_start(new T[v.Size()]) //这里用到的Size函数后面会实现的
	{
		size_t num = v.Size();
		for (size_t i = 0; i < num; i++) {
			_start[i] = v._start[i];
		}
		//让对应的指针指向应该指向的位置
		_finish = _start + num;
		_end = _finish;
	}
	//赋值运算符重载
	Vector& operator=(const Vector v) {
		std::swap(_start, v._start);
		std::swap(_finish, v._finish);
		std::swap(_end, v._end);
		return *this;
	}
	//析构函数
	~Vector() {
		if (_start) {
			delete[] _start;
		}
		_start = _finish = _end = nullptr;
	}
	//迭代器模块
	//////////////////////////////////////////////////////////////////////////////////
	typedef T* iterators;
	//正向迭代器 
	iterators Begin() {
		return _start;
	}
	iterators End() {
		return _finish;
	}

	//反向迭代器
	iterators Rbegin() {
		return End();
	}
	iterators Rend() {
		return Begin();
	}
	//容量模块
	//////////////////////////////////////////////////////////////////////////////////
		//有效元素个数
	size_t Size() const {
		return _finish - _start;
	}
	//容量大小
	size_t Capacity()const {
		return _end - _start;
	}
	//判空函数
	bool Empty()const {
		return _start == _finish;
	}
	//扩容函数
	void Reserve(size_t newCapacity) {
		size_t oldCapacity = Capacity();
		size_t oldSize = Size();

		T* temp = new T[newCapacity];
		//当要设置的新空间大于旧空间
		if (newCapacity > oldCapacity) {
			//判断首指针是否是空
			if (_start) {
				//进行深拷贝
				//思考：memcpy可以使用吗？
				for (size_t i = 0; i < oldSize; i++) {
					temp[i] = _start[i];
				}
				//释放旧空间
				delete[] _start;
			}
			_start = temp;
			_finish = _start + oldSize;
			_end = _start + newCapacity;
		}
	}
	//设置有效元素函数
	void Resize(size_t newSize, const T& val = T()) {
		size_t oldCapacity = Capacity();
		size_t oldSize = Size();
		//要设置的有效元素个数大于现在的有效元素
		if (newSize > oldSize) {
			//要设置的有效元素个数大于现有空间，扩容
			if (newSize > oldCapacity) {
				Reserve(newSize);
			}
			//思考：memeset可以用吗？
			for (size_t i = oldSize; i < newSize; i++) {
				_start[i] = val;
			}
		}
		_finish = _start + newSize;
	}
	//元素访问模块
	//////////////////////////////////////////////////////////////////////////////////
		//获取首元素
	T& Front() {
		return _start[0];
	}
	const T& Front()const {
		return _start[0];
	}
	//获取尾部元素
	T& Back() {
		return *(_finish - 1);
	}
	const T& Back()const {
		return *(_finish - 1);
	}
	//重载[]
	T& operator[](size_t index) {
		//需要判断是否越界
		assert(-1 < index < Size());
		return _start[index];
	}
	const T& operator[](size_t index)const {
		assert(-1 < index < Size());
		return _start[index];
	}
	//修改模块
	//////////////////////////////////////////////////////////////////////////////////
		//尾插
	void Push_back(const T& val) {
		//判断顺序表是否已经存满了
		if (_finish == _end) {
			Reserve(Capacity() * 2);
		}
		*_finish = val;
		_finish++;
	}
	//尾删
	void Pop_back() {
		if (Empty()) {
			return;
		}
		_finish--;
	}
	//任意位置的插入
	iterators Insert(iterators index, const T& val) {
		//判断迭代器是否越界
		if (index < _start || index > _finish)
			return End();
		//判断空间是否存满
		if (_start == _finish)
			Reserve(Capacity() * 2);

		auto it = _finish;
		while (it != index) {
			*it = *(it - 1);
			it--;
		}
		*index = val;
		_finish++;
		return index;
	}
	//任意位置删除
	iterators Erase(iterators index) {
		//判断迭代器是否越界
		if (index < _start || index > _finish)
			return End();
		auto it = index;
		//将index后的元素往前搬移一位
		while (it != _finish) {
			*it = *(it + 1);
			it++;
		}
		_finish--;
		return index;
	}
	//清空有效元素
	void Clear() {
		_finish = _start;
	}
};

int main() {
	Vector<int> v(10, 5);
	v.Push_back(9);
	v.Erase(v.End());
}


