#include <iostream>
#include <future>
#include <iomanip>
#include <thread>

using namespace std;
using namespace chrono_literals;

bool exitFlag = false;

int f(int x)
{
	return 2 * x;
}

int g(int x)
{
	return x * 5;
}

int main()
{
	cout << "Enter x: ";
	int x{};
	cin >> x;
	packaged_task<int(int)> fTask(f);
	packaged_task<int(int)> gTask(g);

	future_status stF, stG;
	future<int> fFuture = fTask.get_future();
	future<int> gFuture = gTask.get_future();
	thread fThread(move(fTask), x);    //future отримано перед move. Отже, навіть якщо цей крок залишив ftask у невизначеному стані, майбутнє, яке отримає результат виклику  уже отримано.
	thread gThread(move(gTask), x);
	bool flag = true;
	bool flagF = true;
	bool flagFgot = false;
	do
	{
		if (flagFgot == false)stF = fFuture.wait_for(5s);
		stG = gFuture.wait_for(5s);
		if (flagFgot == false && stF == future_status::ready) {
			flagFgot = true;
			if (!fFuture.get()) {
				cout << "f(x) && g(x)" << '\n';
				cout << "Result: " << false << '\n';
				flagF = false;
				break;
			}
		}
		if (stF == future_status::timeout || stG == future_status::timeout) //the shared state did not become ready before specified timeout duration has passed
		{
			while (flag) {
				cout << "Timeout! Do you want to continue waiting?\n";
				cout << "Enter 'yes' to continue waiting\nEnter 'no' to terminate\nEnter '0' to continue and not asking again \n";
				string command;
				cin >> command;
				if (command == "yes")
				{
					cout << "Continue waiting for 5s\n";
					break;
				}
				else if (command == "no")
				{
					cout << "Terminating\n";
					exitFlag = true;
					fThread.join();  //The function returns when the thread execution has completed
					gThread.join();
					return 0;
				}
				else if (command == "0")
				{
					cout << "Waiting\n";
					flag = false;
					break;
				}
				else
				{
					cout << "Wrong command, try again\n";
					continue;
				}
			}
		}

	} while (stF != future_status::ready || stG != future_status::ready);

	if (stG == future_status::ready && flagF == true) {
		cout << "f(x) && g(x)" << '\n';
		cout << "Result: " << (true && gFuture.get()) << '\n';
	}
	fThread.join();
	gThread.join();
	return 0;
}