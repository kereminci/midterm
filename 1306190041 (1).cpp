//kerem inci
// 1306190041
// Bilgisayar Muhendisligi 1.Sinif
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
using namespace std;

class entity {
public:

	string gate; //AABCD
	char sign; // X O D R infected,normal,dead,recovered
	int row;
	int column;
	int time; // turn 
	entity();
};
vector<entity>entitys;
ifstream inputFile;
ofstream outputFile;
ofstream turnFile;
string lines;
//input çekme functionlarý
void getSizeAndTurn(int& size, int& turn_count);
void getEntityInput(vector<entity>& entitys);
string takeNumber(string);
string getEntityGate(string);
int getRow(string line);
int getColumn(string line);
bool isInfected(string line);
// turn functionlarý
void printTurn(vector<entity>& entitys, int size, int turn);
void printOutput(vector<entity>& entitys);
void turns(vector<entity>& entitys, int turn_count, int size);
int turnRow(string line);
int turnColumn(string line);
// infect functionlarý
void checkInfection(vector<entity>& entitys);
bool checkRange(entity& l, entity& r);
bool canInfect(entity& l);
bool canDead(entity& l);
bool canRecover(entity& l);
bool checkInfect(entity& l, entity& r);
// yeni infected,normal,recovered entitylere bakan functions
void timeIncrease(vector<entity>& entitys);
void newInfected(vector<entity>& entitys);
void newDead(vector<entity>& entitys);
void newRecover(vector<entity>& entitys);

vector<vector<char>>habitat;
int size, turn_count;
int main()
{
	int size, turn_count = 0;
	inputFile.open("INPUT.TXT");
	outputFile.open("OUTPUT.TXT");
	turnFile.open("TURNS.TXT");
	getSizeAndTurn(size, turn_count);
	getEntityInput(entitys);
	turns(entitys, turn_count, size);
	printOutput(entitys);

}

entity::entity() {
	gate = "";
	sign = 32; // '' yerine 32
	row = -1;
	column = -1;
	time = -1;
}

void newInfected(vector<entity>& entitys) {
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].sign == 'O' && entitys[i].time > 3) { // aaa
			entitys[i].sign = 'X';
			entitys[i].time = 0;
		}
	}

}

void newDead(vector<entity>& entitys) {
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].sign == 'X' && entitys[i].time >= 14 && canDead(entitys[i])) {
			entitys[i].sign = 'D';
			entitys[i].time = 0;
		}
	}
}
void newRecover(vector<entity>& entitys) {
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].sign == 'X' && entitys[i].time > 30) {
			entitys[i].sign = 'R';
		}
	}
}

void timeIncrease(vector<entity>& entitys) {
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].time >= 0) entitys[i].time++;
	}
}

bool canDead(entity& l) {
	int count = 0;
	for (size_t i = 0; i < l.gate.size(); i++) {
		if (l.gate[i] == 'A' || l.gate[i] == 'B' || l.gate[i] == 'C' || l.gate[i] == 'D') {
			count++;
		}
	}
	return count >= 3;

}

bool canRecover(entity& l) {
	int count = 0;
	for (size_t i = 0; i < l.gate.size(); i++) {
		if (l.gate[i] == 'A' || l.gate[i] == 'B' || l.gate[i] == 'C' || l.gate[i] == 'D') {
			count++;
		}
	}
	return count < 3;
}


bool canInfect(entity& l) {
	for (size_t i = 0; i < l.gate.size(); i++) {
		if (l.gate[i] == 'A' || l.gate[i] == 'B')
			return true;
	}
	return false;
}
bool checkRange(entity& l, entity& r) {
	int diffX = abs(l.row - r.row);
	int diffY = abs(l.column - r.column);
	if (diffX > 3 || diffY > 3) return false;
	if ((diffX + diffY) <= 6) return true;
	return false;
}

bool checkInfect(entity& l, entity& r) {
	if (l.sign == 'X' && r.sign == 'O') return true;
	if (r.sign == 'X' && l.sign == 'O') return true;
	if ((l.sign == 'D' && l.time < 5) && r.sign == 'O') return true;
	if ((r.sign == 'D' && r.time < 5) && l.sign == 'O') return true;

	return false;

}

void checkInfection(vector<entity>& entitys) {
	for (size_t i = 0; i < entitys.size(); i++) {
		for (size_t j = 0; j < entitys.size(); j++) {
			if (i != j && checkRange(entitys[i], entitys[j]) && checkInfect(entitys[i], entitys[j])) {
				if (entitys[i].time == -1 && canInfect(entitys[i])) entitys[i].time = 0;
				if (entitys[j].time == -1 && canInfect(entitys[j])) entitys[j].time = 0;
			}
		}
	}
}


void printTurn(vector<entity>& entitys, int size, int turn) {
	int n = size;
	vector<vector<char> > habitat(size, vector<char>(size, 32)); // ""

	for (size_t i = 0; i < entitys.size(); i++) {
		if (!(entitys[i].sign == 'D' && entitys[i].time >= 5)) {
			habitat[entitys[i].row - 1][entitys[i].column - 1] = entitys[i].sign;
		}
	}

	turnFile << "Turn " << turn + 1 << ":" << endl;
	for (int i = 0; i < n + 2; i++) turnFile << "-";
	turnFile << endl;
	for (int i = 0; i < n; i++) {
		turnFile << "-";
		for (int j = 0; j < n; j++) {
			turnFile << habitat[i][j];
			if (j == n - 1) turnFile << "-";
		}
		turnFile << endl;

	}
	for (int i = 0; i < n + 2; i++) turnFile << "-";
	turnFile << endl;
}

void printOutput(vector<entity>& entitys) {
	int normal = 0, dead = 0, recovered = 0, infected = 0;
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].sign == 'O') normal++;
		if (entitys[i].sign == 'D') dead++;
		if (entitys[i].sign == 'R') recovered++;
		if (entitys[i].sign == 'X') infected++;
	}
	outputFile << "Normal    :" << normal << endl;
	outputFile << "Infected  :" << infected << endl;
	outputFile << "Dead      :" << dead << endl;
	outputFile << "Recovered :" << recovered << endl;
	for (size_t i = 0; i < entitys.size(); i++) {
		if (entitys[i].sign == 'X') outputFile << "entity " << i + 1 << " " << entitys[i].row << "x" << entitys[i].column << "  infected" << endl;
		if (entitys[i].sign == 'O') outputFile << "entity " << i + 1 << " " << entitys[i].row << "x" << entitys[i].column << "  normal" << endl;
		if (entitys[i].sign == 'D') outputFile << "entity " << i + 1 << " " << entitys[i].row << "x" << entitys[i].column << "  dead" << endl;
		if (entitys[i].sign == 'R') outputFile << "entity " << i + 1 << " " << entitys[i].row << "x" << entitys[i].column << "  recovered" << endl;
	}
}



void turns(vector<entity>& entitys, int turn_count, int size) {
	string line;
	stringstream ss(lines);
	for (int j = 0; j < 2; j++) ss >> line;
	for (size_t i = 0; i < entitys.size(); i++) {
		ss >> line;
		entitys[i].row = turnRow(line);
		entitys[i].column = turnColumn(line);
	}

	// objelerin durumlarý kontrol edilir
	
	checkInfection(entitys);
	newInfected(entitys);
	newDead(entitys);
	newRecover(entitys);
	printTurn(entitys, size, 0);


	for (int i = 1; i < turn_count; i++) {
		for (int j = 0; j < 2; j++) inputFile >> line;
		for (size_t i = 0; i < entitys.size(); i++) {
			inputFile >> line;
			if (entitys[i].sign != 'D') {
				entitys[i].row = turnRow(line);
				entitys[i].column = turnColumn(line);
			}
		}

		// objelerin durumlarý kontrol edilir
		timeIncrease(entitys);
		checkInfection(entitys);
		newInfected(entitys);
		newDead(entitys);
		newRecover(entitys);
		printTurn(entitys, size, i);

	}


}
int turnRow(string line) {
	string s;
	for (int i = 0; line[i] != 'x'; i++) {
		s.push_back(line[i]);
	}
	return stoi(s);
}
int turnColumn(string line) {
	size_t i = 0;
	string s;
	for (i; line[i] != 'x'; i++);
	i++;
	for (i; i < line.size(); i++) {
		s.push_back(line[i]);
	}
	return stoi(s);
}

bool isInfected(string s) {
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == 'i' && s[i + 1] == 'n' && s[i + 2] == 'f' && s[i + 3] == 'e' && s[i + 4] == 'c' && s[i + 5] == 't' && s[i + 6] == 'e' && s[i + 7] == 'd')
			return true;
	}
	return false;
}

bool isEntity(string s) {
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == 'e' && s[i + 1] == 'n' && s[i + 2] == 't' && s[i + 3] == 'i' && s[i + 4] == 't' && s[i + 5] == 'y')
			return true;
	}
	return false;
}

void getEntityInput(vector<entity>& entitys) {

	entity e;
	string temp;
	getline(inputFile, lines);
	while (isEntity(lines)) {

		e.gate = getEntityGate(lines);
		e.row = getRow(lines);
		e.column = getColumn(lines);
		e.sign = (isInfected(lines) ? 'X' : 'O');
		e.time = -1;
		entitys.push_back(e);
		getline(inputFile, lines);
	}

}

int getRow(string s) {
	int i = 0;
	string temp;
	for (i; !(s[i] >= 'A' && s[i] <= 'Z'); i++);
	for (i; s[i] != 'x'; i++) {
		if (s[i] >= '0' && s[i] <= '9') temp.push_back(s[i]);
	}

	return stoi(temp);
}

int getColumn(string s) {
	size_t i = 0;
	string temp;
	for (i; s[i] != 'x'; i++);
	for (i; i < s.size(); i++) if (s[i] >= '0' && s[i] <= '9') temp.push_back(s[i]);
	return stoi(temp);
}

string getEntityGate(string s) {
	string  gate;
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') gate.push_back(s[i]);
	}
	return gate;
}
void getSizeAndTurn(int& size, int& turn_count) {
	int i = 0;
	string line;
	while (i < 2) {
		getline(inputFile, line);
		if (i == 0)   size = stoi(takeNumber(line));
		if (i == 1)  turn_count = stoi(takeNumber(line));
		i++;
	}
}
string takeNumber(string s) {
	string num;
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			num.push_back(s[i]);
		}
	}
	return num;
}



