#include <vector>
#include "MillerRabin.cpp"
#include "BlumBlumShub.cpp"

class RSA {
private:
	ZZ p, q;
	ZZ privatekey;
	ZZ phi;

public:
	ZZ publickey;
	ZZ mod;

	RSA(long nbits);
	~RSA();
	vector<ZZ> encrypt(const string text);
	string decrypt(const vector<ZZ> v);
	void getAttributes();

private:
	ZZ publickey_generator(const ZZ &phi);
	ZZ privatekey_generator(const ZZ &e, const ZZ &phi);
};


RSA::RSA(long nbits) {
	cout << "Encrypting..." << endl;	
	BlumBlumShub bbs(nbits);
	do {
		// do p = bbs.generate_number();
		do p = large_prime_generator(nbits);
		while (!MillerRabin().isPrime(p));
		// do q = bbs.generate_number();
		do q = large_prime_generator(nbits);
		while (!MillerRabin().isPrime(q));

		mod = p * q;
		phi = (p-1) * (q-1);

		publickey = publickey_generator(phi);
	} while (mdc(phi, publickey) != 1);

	this->privatekey = privatekey_generator(this->publickey, this->phi);
	ofstream fout("temp", ios::trunc);
}

RSA::~RSA() {
	publickey.kill();
	privatekey.kill();
	mod.kill();
	phi.kill();
}

vector<ZZ> RSA::encrypt(const string text) {
	vector<ZZ> out;
	for (int i = 0; i < text.size(); i++) {
		ZZ C, M;
		M = text[i];
		C = powerMod(M, this->publickey, this->mod);
		out.push_back(C);
	}

	return out;
}

string RSA::decrypt(const vector<ZZ> v) {
	cout << "Decrypting..." << endl;
	string out;
	for (int i = 0; i < v.size(); i++) {
		ZZ C, M;
		C = v[i];
		M = powerMod(C, this->privatekey, this->mod);
		out += ZZtoi(M);
	}

	cout << "Done!" << endl;
	return out;
}

void RSA::getAttributes() {
	cout << "p: " << this->p << endl << endl;
	cout << "q: " << this->q << endl << endl;
	cout << "mod: " << this->mod << endl << endl;
	cout << "phi: " << this->phi << endl << endl;
	cout << "publickey: " << this->publickey << endl << endl;
	cout << "privatekey: " << this->privatekey << endl << endl;
}

// return e = 65537
ZZ RSA::publickey_generator(const ZZ &phi) {
	ZZ e, a, b;
	a = 2;
	b = 16;
	e = Power(a, b) + 1;

	return e;
}

// return d = e^{-1} mod(phi)
ZZ RSA::privatekey_generator(const ZZ &e, const ZZ &phi) {
	ZZ d;
	InvModStatus(d, e, phi);
	
	return d;
}