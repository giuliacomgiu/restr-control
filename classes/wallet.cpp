#include "wallet.h"

Wallet::Wallet(){
	credit = 0;
};

Wallet::~Wallet(){
};


void Wallet::setCash(float c){
	credit = c;
	return;
}

float Wallet::getCash(void){
	return credit;
}