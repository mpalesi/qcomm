#include <iostream>

using namespace std;

void qft(const int nqubits)
{
  for (int qb1=0; qb1<nqubits; qb1++)
    {
      cout << "(" << qb1 << ")" << endl;
      for (int qb2=qb1+1; qb2<nqubits; qb2++)
	cout << "(" << qb1 << " " << qb2 << ")" << endl;
    }
}

int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      cerr << "Usage " << argv[0] << " <circuit> <qubits>" << endl
	   << "circuits: qft, grover, cuccaro, qv" << endl;
      
      return 1;
    }

  string cname = argv[1];
  int    nqubits = stoi(argv[2]);

  if (cname == "qft")
    qft(nqubits);
  
  return 0;
}
