using namespace std;

typedef struct full_name
{
	string fname;
	string lname;
} full_name;

class Person{
private:
	full_name name;
	bool is_enrolled;

public:
	Wallet wallet;

	Person();
	~Person();

	full_name getName(void);
	void setFName(string);
	void setLName(string);

	bool getEnrolled(void);
	void setEnrolled(bool);
};
