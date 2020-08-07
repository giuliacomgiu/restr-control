using namespace std;

class Staff: public Person{
private:
	Barcode code;
	Wallet wallet;
	Dept dept;

public:
	Staff();
};