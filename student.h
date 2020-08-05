using namespace std;

class Student: public Person{
private:
	Barcode code;
	Wallet wallet;
	Dept dept;
	bool is_isent;

public:
	Student();
	bool getIsent(void);
	void setIsent(bool);
};