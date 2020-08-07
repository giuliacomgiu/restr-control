using namespace std;

class Wallet{
private:
	float credit;

public:
	Wallet();
	~Wallet();
	void setCash(float);
	float getCash(void);
};