using namespace std;

typedef struct full_name
{
	string fname;
	string lname;
} full_name;

class Pessoa{
private:
	full_name name;
	float credit;
	bool is_enrolled;

public:
	Pessoa();
	~Pessoa();

	full_name getName(void);
	void setName(void);

	bool getEnrolled(void);
	void setEnrolled(void);

	void addCash(void);
	void decCash(void);

};
