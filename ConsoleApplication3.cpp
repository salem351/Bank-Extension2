#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void ShowMainMenueOption();
void ShowTransactionsMainMenueOption();
void ShowManageUsersMenueOption();
void Login();

const string ClientFileName = "Client.txt";
const string UsersFileName = "Users.txt";

enum enMainMenue {
	eShowClient = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eTransactions = 6,
	eManageUsers = 7,
	eExit = 8
};
enum enTransactions {
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalance = 3,
	eMainMenue = 4
};
enum enMangeUsers {
	eListUser = 1,
	eAddNewUser = 2,
	eDeleteUser = 3,
	eUpdateUser = 4,
	eFindUser = 5,
	eMainMenueFromManage = 6
};
enum enMainMenuePermission {
	eAll = -1,
	qListClients = 1,
	qAddNewClient = 2,
	qDeleteClient = 4,
	qUpdateClient = 8,
	qFindClient = 16,
	qTransactions = 32,
	qManageUsers = 64,
};

struct stClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string phone;
	double AccountBalance;
	bool DeletClient = false;
};
struct stUsers {
	string UserName;
	string Password;
	int Permission;
	bool MarkForDelete = false;
};

stUsers CurrenUser;

vector <string> SpiltLineToVector(string line, string Delim) {
	vector <string> vString;
	string sWord = "";
	int pos = 0;

	while ((pos = line.find(Delim)) != std::string::npos) {

		sWord = line.substr(0, pos);
		if (sWord != " ") {
			vString.push_back(sWord);
		}
		line.erase(0, pos + Delim.length());
	}
	if (line != " ") {
		vString.push_back(line);
	}

	return vString;
}

stClient ConvertClientLineToRecord(string line) {
	stClient Client;

	vector <string> vString = SpiltLineToVector(line, "/##/");

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}
stUsers ConvertUserLineToRecord(string line) {
	stUsers User;

	vector <string> vString = SpiltLineToVector(line, "/##/");

	User.UserName = vString[0];
	User.Password = vString[1];
	User.Permission = stoi(vString[2]);

	return User;
}
string ConvertCLientReocredToLine(stClient Client) {
	string LineClientDetails = "";

	LineClientDetails = Client.AccountNumber + "/##/";
	LineClientDetails += Client.PinCode + "/##/";
	LineClientDetails += Client.Name + "/##/";
	LineClientDetails += Client.phone + "/##/";
	LineClientDetails += to_string(Client.AccountBalance);

	return LineClientDetails;
}
string ConvertUserReocredToLine(stUsers User) {
	string LineUserDetails = "";

	LineUserDetails = User.UserName + "/##/";
	LineUserDetails += User.Password + "/##/";
	LineUserDetails += to_string(User.Permission);

	return LineUserDetails;
}

vector <stClient> LoadClientFromFileToVector(string FileName) {
	vector <stClient> vClient;
	fstream Myfile;

	Myfile.open(FileName, ios::in);

	if (Myfile.is_open()) {
		stClient Client;
		string Line;

		while (getline(Myfile, Line)) {
			Client = ConvertClientLineToRecord(Line);
			vClient.push_back(Client);
		}

		Myfile.close();
	}
	return vClient;
}
vector <stUsers> LoadUsersFromFileToVector(string FileName) {
	vector <stUsers> vUsers;
	fstream Myfile;

	Myfile.open(FileName, ios::in);

	if (Myfile.is_open()) {
		stUsers User;
		string Line;

		while (getline(Myfile, Line)) {
			User = ConvertUserLineToRecord(Line);
			vUsers.push_back(User);
		}

		Myfile.close();
	}
	return vUsers;
}

short ReadTheMenueOption() {
	short num;

	cout << "Choose what do you want to do? [1 - 8]? ";
	cin >> num;

	return num;
}
short ReadTheTransactionsMenueOption() {
	short num;

	cout << "Choose what do you want to do? [1 - 4]? ";
	cin >> num;

	return num;
}
short ReadTheManageUsersMenueOption() {
	short num;

	cout << "Choose what do you want to do? [1 - 6]? ";
	cin >> num;

	return num;
}


void ReturnToMainMenue() {
	cout << "\nPress any key to go back to main menue....";
	system("pause>0");
	ShowMainMenueOption();
}
void ReturnToTrnasactionsMainMenue() {
	cout << "\nPress any key to go back to Transactions main menue....";
	system("pause>0");
	ShowTransactionsMainMenueOption();
}
void ReturnToManageUsersMenue() {
	cout << "\nPress any key to go back to Manage Users Menue....";
	system("pause>0");
	ShowManageUsersMenueOption();
}

void printClientCard(stClient Client) {

	cout << "|" << setw(15) << left << Client.AccountNumber;
	cout << "|" << setw(10) << left << Client.PinCode;
	cout << "|" << setw(20) << left << Client.Name;
	cout << "|" << setw(12) << left << Client.phone;
	cout << "|" << setw(17) << left << Client.AccountBalance;
}
void printClientCardForTrem(stClient Client) {

	cout << "|" << setw(15) << left << Client.AccountNumber;
	cout << "|" << setw(20) << left << Client.Name;
	cout << "|" << setw(17) << left << Client.AccountBalance;
}
void printUserCard(stUsers User) {

	cout << "|" << setw(15) << left << User.UserName;
	cout << "|" << setw(10) << left << User.Password;
	cout << "|" << setw(6) << left << User.Permission;
}

bool CheckAccessPermission(enMainMenuePermission Permission) {

	if (CurrenUser.Permission == Permission) 
		return true;
	
	if ((Permission & CurrenUser.Permission) == Permission)
		return true;
	else
		return false;
}
void ShowAccessDeniedMessage() {
	cout << "____________________________________";
	cout << "\nAccess Denied,\n";
	cout << "You don't have permission to do this\n";
	cout << "please conact with Admin\n";
	cout << "____________________________________\n";
}

void ShowClientList() {

	if (!CheckAccessPermission(enMainMenuePermission::qListClients)) {

		ShowAccessDeniedMessage();
		return;
	}

	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	cout << "\n\t\t Client list (" << vClient.size() << ") Client(s).";
	cout << "\n________________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "Account Number";
	cout << "|" << setw(10) << left << "PinCode";
	cout << "|" << setw(20) << left << "Client Name";
	cout << "|" << setw(12) << left << "Phone";
	cout << "|" << setw(17) << left << "Account Balance";
	cout << "\n________________________________________________________________________________\n";

	for (stClient& c : vClient) {
		printClientCard(c);
		cout << endl;
	}
	cout << "\n________________________________________________________________________________\n";

}

string ReadAccountNumber() {
	string AccountNumber;

	cout << "\nEnter AccountNumber? ";
	cin >> AccountNumber;

	return AccountNumber;
}
string ReadUserName() {
	string UserName;

	cout << "\nEnter UserName? ";
	cin >> UserName;

	return UserName;
}

bool IsCLientExit(string AccountNumber, stClient& Client) {
	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	for (stClient& C : vClient) {

		if (C.AccountNumber == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}
bool IsUserExit(string UserName, stUsers& User) {
	vector <stUsers> vUsers = LoadUsersFromFileToVector(UsersFileName);

	for (stUsers& U : vUsers) {

		if (U.UserName == UserName) {
			User = U;
			return true;
		}
	}
	return false;
}

int ReadPermissionToSet() {

	int Permission = 0;
	char Answe;

	cout << "\nDo you want to give full access? ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		return -1;
	}

	cout << "\nDo you want to give access to:\n";

	cout << "\nShowClient List? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qListClients;
	}

	cout << "\nAdd New Client? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qAddNewClient;
	}

	cout << "\nDelete Client? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qDeleteClient;
	}

	cout << "\nUpdate Client? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qUpdateClient;
	}

	cout << "\nFind Client? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qFindClient;
	}

	cout << "\nTransactions ? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qTransactions;
	}

	cout << "\nManage Users? y/n ";
	cin >> Answe;
	if (toupper(Answe) == 'Y') {
		Permission += enMainMenuePermission::qManageUsers;
	}

	return Permission;
}

stClient ReadClientDetails() {
	stClient Client;

	cout << "\nEnter AccountNumber? ";
	getline(cin >> ws, Client.AccountNumber);

	while (IsCLientExit(Client.AccountNumber, Client)) {

		cout << "\n Client With [ " << Client.AccountNumber << " ] already exit, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "\nEnter Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone? ";
	getline(cin, Client.phone);

	cout << "\nEnter AcocuntBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}
stUsers ReadUserDetails() {
	stUsers User;

	cout << "\nEnter UserName? ";
	getline(cin >> ws, User.UserName);

	while (IsUserExit(User.UserName, User)) {

		cout << "\n User With [ " << User.UserName << " ] already exit, Enter another UserName? ";
		getline(cin >> ws, User.UserName);
	}

	cout << "\nEnter Password? ";
	getline(cin, User.Password);

	 User.Permission = ReadPermissionToSet();

	return User;
}
stClient ReadClientDetailsToUpdate(string AccountNumber) {
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "\nEnter Name? ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone? ";
	getline(cin, Client.phone);

	cout << "\nEnter AcocuntBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}
stUsers ReadUserDetailsToUpdate(string UserName) {
	stUsers User;

	User.UserName = UserName;

	cout << "\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionToSet();

	return User;
}

void AddNewClientToFile(stClient Client, string FileName) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	string line;

	if (MyFile.is_open()) {
		line = ConvertCLientReocredToLine(Client);
		MyFile << line << endl;

		MyFile.close();
	}
}
void AddNewClient() {
	stClient Client;
	Client = ReadClientDetails();
	AddNewClientToFile(Client, ClientFileName);
}
void AddClient() {
	char Addmore = 'N';

	do {
		cout << "\nAdding New Client:\n";

		AddNewClient();

		cout << "\nClient Added Successfully, Do you want to add more? Y/N? ";
		cin >> Addmore;

	} while (Addmore == 'Y' || Addmore == 'y');

}
void ShowAddNewClient() {

	if (!CheckAccessPermission(enMainMenuePermission::qAddNewClient)) {

		ShowAccessDeniedMessage();
		return;
	}

	cout << "____________________________________";
	cout << "\n\t Add New Client\n";
	cout << "____________________________________\n";

	AddClient();
}

void AddNewUserToFile(stUsers User, string FileName) {
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	string line;

	if (MyFile.is_open()) {
		line = ConvertUserReocredToLine(User);
		MyFile << line << endl;

		MyFile.close();
	}
}
void AddNewUser(){
	stUsers User;
	User = ReadUserDetails();
	AddNewUserToFile(User, UsersFileName);
}
void AddUser() {
	char Addmore = 'N';

	do {
		cout << "\nAdding New User:\n";

		AddNewUser();

		cout << "\nUser Added Successfully, Do you want to add more? Y/N? ";
		cin >> Addmore;

	} while (Addmore == 'Y' || Addmore == 'y');

}
void ShowAddNewUser() {
	cout << "____________________________________";
	cout << "\n\t Add New User\n";
	cout << "____________________________________\n";

	AddUser();
}

void printClientDetials(stClient Client) {

	cout << "\n The following are the client details:\n";
	cout << "___________________________________________________\n";
	cout << "Acount Number  = " << Client.AccountNumber << endl;
	cout << "PinCode        = " << Client.PinCode << endl;
	cout << "Client Name    = " << Client.Name << endl;
	cout << "Phone Number   = " << Client.phone << endl;
	cout << "Acount Balance = " << Client.AccountBalance << endl;
	cout << "___________________________________________________\n";
}
void printUserDetials(stUsers User) {

	cout << "\n The following are the Users details:\n";
	cout << "___________________________________________________\n";
	cout << "User Name  = " << User.UserName   << endl;
	cout << "Password   = " << User.Password   << endl;
	cout << "Permission = " << User.Permission << endl;
	cout << "___________________________________________________\n";
}

bool MarkTheClientWantToDelete(string AccountNumber, vector <stClient>& vClient) {

	for (stClient& C : vClient) {

		if (C.AccountNumber == AccountNumber) {
			C.DeletClient = true;
			return true;
		}
	}
	return false;
}
bool MarkTheUserWantToDelete(string UserName, vector <stUsers>& vUsers) {

	for (stUsers& U : vUsers) {

		if (U.UserName == UserName) {
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <stClient> SaveClientsToFile(string FileName, vector <stClient> vClient) {

	fstream Myfile;

	Myfile.open(FileName, ios::out);

	string Line;

	if (Myfile.is_open()) {

		for (stClient& C : vClient) {

			if (C.DeletClient == false) {
				Line = ConvertCLientReocredToLine(C);
				Myfile << Line << endl;
			}
		}

		Myfile.close();
	}
	return vClient;
}
vector <stUsers> SaveUsersToFile(string FileName, vector <stUsers> vUsers) {

	fstream Myfile;

	Myfile.open(FileName, ios::out);

	string Line;

	if (Myfile.is_open()) {

		for (stUsers& U : vUsers) {

			if (U.MarkForDelete == false) {
				Line = ConvertUserReocredToLine(U);
				Myfile << Line << endl;
			}
		}

		Myfile.close();
	}
	return vUsers;
}

bool DeleteClientDetials(string AccountNumber) {
	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	char Delete = 'N';
	stClient Client;

	if (IsCLientExit(AccountNumber, Client)) {

		printClientDetials(Client);

		cout << "\n\nAre you sure you want to delete this client? Y/N? ";
		cin >> Delete;

		if (Delete == 'Y' || Delete == 'y') {

			MarkTheClientWantToDelete(AccountNumber, vClient);
			SaveClientsToFile(ClientFileName, vClient);

			cout << "\nClient Deleted Succssfully\n";
			return true;
		}
	}
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] is not found.\n";

	return false;
}
bool DeleteUserDetials(string UserName) {
	vector <stUsers> vUsers = LoadUsersFromFileToVector(UsersFileName);

	char Delete = 'N';
	stUsers User;
	
	if (IsUserExit(UserName, User)) {

		printUserDetials(User);

		cout << "\n\nAre you sure you want to delete this User? Y/N? ";
		cin >> Delete;

		if (Delete == 'Y' || Delete == 'y') {

			MarkTheUserWantToDelete(UserName, vUsers);
			SaveUsersToFile(UsersFileName, vUsers);

			cout << "\nUser Deleted Succssfully\n";
			return true;
		}
	}
	else
		cout << "\nUser With UserName [" << UserName << "] is not found.\n";

	return false;
}
bool UpdateClientDetials(string AccountNumber) {
	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	char Delete = 'N';
	stClient Client;

	if (IsCLientExit(AccountNumber, Client)) {

		printClientDetials(Client);

		cout << "\n\nAre you sure you want to Upate this client? Y/N? ";
		cin >> Delete;

		if (Delete == 'Y' || Delete == 'y') {

			for (stClient& C : vClient) {

				if (C.AccountNumber == AccountNumber)
					C = ReadClientDetailsToUpdate(AccountNumber);
			}

			SaveClientsToFile(ClientFileName, vClient);

			cout << "\nClient Updated Succssfully\n";
			return true;
		}
	}
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] is not found.\n";

	return false;
}
bool UpdateUserDetials(string UserName) {
	vector <stUsers> vUsers = LoadUsersFromFileToVector(UsersFileName);

	char Delete = 'N';
	stUsers User;

	if (IsUserExit(UserName, User)) {

		printUserDetials(User);

		cout << "\n\nAre you sure you want to Update this User? Y/N? ";
		cin >> Delete;

		if (Delete == 'Y' || Delete == 'y') {

			for (stUsers& U : vUsers) {

				if (U.UserName == UserName)
					U = ReadUserDetailsToUpdate(UserName);
			}

			SaveUsersToFile(UsersFileName, vUsers);

			cout << "\nUser Updated Succssfully\n";
			return true;
		}
	}
	else
		cout << "\nUser With User Name [" << UserName << "] is not found.\n";

	return false;
}
void ShowDeleteClient() {

	if (!CheckAccessPermission(enMainMenuePermission::qDeleteClient)) {

		ShowAccessDeniedMessage();
		return;
	}


	cout << "____________________________________";
	cout << "\n\t Delete Client\n";
	cout << "____________________________________\n";

	string AccountNumber = ReadAccountNumber();

	DeleteClientDetials(AccountNumber);
}
void ShowDeleteUser() {
	cout << "____________________________________";
	cout << "\n\t Delete User\n";
	cout << "____________________________________\n";

	string UserName = ReadAccountNumber();

	if (UserName == "Admin") {
		cout << "\n\nYou can't Delete the Admin\n";
		cout << "\nYou have to contact with the emergency\n";
	}
	else {
		DeleteUserDetials(UserName);
	}
}
void ShowUpdateClient() {

	if (!CheckAccessPermission(enMainMenuePermission::qUpdateClient)) {

		ShowAccessDeniedMessage();
		return;
	}


	cout << "____________________________________";
	cout << "\n\t Update Client\n";
	cout << "____________________________________\n";

	string AccountNumber = ReadAccountNumber();

	UpdateClientDetials(AccountNumber);
}
void ShowUpdateUser() {
	cout << "____________________________________";
	cout << "\n\t Update User\n";
	cout << "____________________________________\n";

	string UserName = ReadAccountNumber();

	UpdateUserDetials(UserName);
}

void ShowFindClient() {

	if (!CheckAccessPermission(enMainMenuePermission::qFindClient)) {

		ShowAccessDeniedMessage();
		return;
	}

	cout << "____________________________________";
	cout << "\n\t Find Client\n";
	cout << "____________________________________\n";

	string AccountNumber = ReadAccountNumber();
	stClient Client;

	if (IsCLientExit(AccountNumber, Client)) {

		printClientDetials(Client);
	}
	else
		cout << "\nClient With Account Number [" << AccountNumber << "] is not found.\n";
}
void ShowFindUser() {
	cout << "____________________________________";
	cout << "\n\t Find User\n";
	cout << "____________________________________\n";

	string UserName = ReadAccountNumber();
	stUsers User;

	if (IsUserExit(UserName, User)) {

		printUserDetials(User);
	}
	else
		cout << "\nUser With UserName [" << UserName << "] is not found.\n";
}
void ExitProgram() {
	cout << "____________________________________";
	cout << "\n\t End Program\n";
	cout << "____________________________________\n";
}

void ShowTotalBalance() {
	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);

	cout << "\n\t\t Client list (" << vClient.size() << ") Client(s).";
	cout << "\n________________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "Account Number";
	cout << "|" << setw(20) << left << "Client Name";
	cout << "|" << setw(17) << left << "Account Balance";
	cout << "\n________________________________________________________________________________\n";

	if (vClient.size() == 0) {
		cout << "\t\t\t\tNo Users Available in the system\n";
	}
	else
	for (stClient& c : vClient) {
		printClientCardForTrem(c);
		cout << endl;
	}
	cout << "\n________________________________________________________________________________\n";

}
void ShowListUsers() {
	vector <stUsers> vUsers = LoadUsersFromFileToVector(UsersFileName);

	cout << "\n\t\t User list (" << vUsers.size() << ") User(s).";
	cout << "\n________________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "User Name";
	cout << "|" << setw(10) << left << "Password";
	cout << "|" << setw(6) << left << "Permissions";
	cout << "\n________________________________________________________________________________\n";

	if (vUsers.size() == 0) {
		cout << "\t\t\t\tNo Users Available in the system\n";
	}
	else
	   for (stUsers& U : vUsers) {
		  printUserCard(U);
		  cout << endl;
	   }
	cout << "\n________________________________________________________________________________\n";
}

bool DepositAmountToAccountBalance(int Amount, string AccountNumber) {
	vector <stClient> vClient = LoadClientFromFileToVector(ClientFileName);
	char DepositAmount = 'N';

	cout << "\n\nAre you sure you want to Perform this transactions? Y/N? ";
	cin >> DepositAmount;

	if (DepositAmount == 'Y' || DepositAmount == 'y') {

		for (stClient& C : vClient) {

			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;

				SaveClientsToFile(ClientFileName, vClient);

				cout << "\nDone Succssfully, New Balance is: " << C.AccountBalance << endl;
				return true;
			}

		}
	}
	return false;
}
void ShowDeposit() {
	cout << "____________________________________";
	cout << "\n\t Deposit Client\n";
	cout << "____________________________________\n";

	string AccountNumber = ReadAccountNumber();
	int Amount = 0;
	stClient Client;

	while (!IsCLientExit(AccountNumber, Client)) {

		cout << "\nClient With Account Number [" << AccountNumber << "] is not found.\n";
		AccountNumber = ReadAccountNumber();
	}

	printClientDetials(Client);

	cout << "\n\nPlease, Enter Deposit Amount? ";
	cin >> Amount;

	DepositAmountToAccountBalance(Amount, AccountNumber);
}
void ShowWithdraw() {
	cout << "____________________________________";
	cout << "\n\t Withdraw Client\n";
	cout << "____________________________________\n";

	string AccountNumber = ReadAccountNumber();
	int Amount = 0;
	stClient Client;

	while (!IsCLientExit(AccountNumber, Client)) {

		cout << "\nClient With Account Number [" << AccountNumber << "] is not found.\n";
		AccountNumber = ReadAccountNumber();
	}

	printClientDetials(Client);

	cout << "\n\nPlease, Enter Withdraw Amount? ";
	cin >> Amount;

	while (Client.AccountBalance < Amount) {
		cout << "\n\nAmount Exceed the balance, you can withdraw less than : " << Client.AccountBalance;
		cout << "\nPlease, Enter Withdraw Amount? ";
		cin >> Amount;
	}
	DepositAmountToAccountBalance(Amount * -1, AccountNumber);
}

void PerformManageUsersMenueOption(enMangeUsers MangeUsers) {

	switch (MangeUsers) {

	case enMangeUsers::eListUser:
		system("cls");
		ShowListUsers();
		ReturnToManageUsersMenue();
		break;

	case enMangeUsers::eAddNewUser:
		system("cls");
		ShowAddNewUser();
		ReturnToManageUsersMenue();
		break;

	case enMangeUsers::eDeleteUser:
		system("cls");
		ShowDeleteUser();
		ReturnToManageUsersMenue();
		break;

	case enMangeUsers::eUpdateUser:
		system("cls");
		ShowUpdateUser();
		ReturnToManageUsersMenue();
		break;

	case enMangeUsers::eFindUser:
		system("cls");
		ShowFindUser();
		ReturnToManageUsersMenue();
		break;

	case enMangeUsers::eMainMenueFromManage:
		system("cls");
		ShowMainMenueOption();
		break;
	}
}
void ShowManageUsersMenueOption() {

	if (!CheckAccessPermission(enMainMenuePermission::qManageUsers)) {

		ShowAccessDeniedMessage();
		return;
	}


	system("cls");
	cout << "==================================\n";
	cout << "\tManage Users Menue Screen\n";
	cout << "==================================\n";
	cout << "\t[1] List Users\n";
	cout << "\t[2] Add New User\n";
	cout << "\t[3] Delete User\n";
	cout << "\t[4] Update User Info\n";
	cout << "\t[5] Find User\n";
	cout << "\t[6] MainMenue\n";
	cout << "==================================\n";
	PerformManageUsersMenueOption((enMangeUsers)ReadTheManageUsersMenueOption());
}

void PerformTheTrnasactionsMainMenueOption(enTransactions Transactions) {

	switch (Transactions) {

	case enTransactions::eDeposit:
		system("cls");
		ShowDeposit();
		ReturnToTrnasactionsMainMenue();
		break;

	case enTransactions::eWithdraw:
		system("cls");
		ShowWithdraw();
		ReturnToTrnasactionsMainMenue();
		break;

	case enTransactions::eTotalBalance:
		system("cls");
		ShowTotalBalance();
		ReturnToTrnasactionsMainMenue();
		break;

	case enTransactions::eMainMenue:
		system("cls");
		ShowMainMenueOption();
		break;
	}
}
void ShowTransactionsMainMenueOption() {

	if (!CheckAccessPermission(enMainMenuePermission::qTransactions)) {

		ShowAccessDeniedMessage();
		return;
	}


	system("cls");
	cout << "==================================\n";
	cout << "\tTransactions Main Menue Screen\n";
	cout << "==================================\n";
	cout << "\t[1] Deposit\n";
	cout << "\t[2] Withdarw\n";
	cout << "\t[3] Total Balance\n";
	cout << "\t[4] Main Menue\n";
	cout << "==================================\n";
	PerformTheTrnasactionsMainMenueOption((enTransactions)ReadTheTransactionsMenueOption());
}

void PerformTheMainMenueOption(enMainMenue MainMenue) {

	switch (MainMenue) {

	case enMainMenue::eShowClient:
		system("cls");
		ShowClientList();
		ReturnToMainMenue();
		break;

	case enMainMenue::eAddNewClient:
		system("cls");
		ShowAddNewClient();
		ReturnToMainMenue();
		break;

	case enMainMenue::eDeleteClient:
		system("cls");
		ShowDeleteClient();
		ReturnToMainMenue();
		break;

	case enMainMenue::eUpdateClient:
		system("cls");
		ShowUpdateClient();
		ReturnToMainMenue();
		break;

	case enMainMenue::eFindClient:
		system("cls");
		ShowFindClient();
		ReturnToMainMenue();
		break;

	case enMainMenue::eTransactions:
		system("cls");
		ShowTransactionsMainMenueOption();
		ReturnToMainMenue();
		break;

	case enMainMenue::eManageUsers:
		system("cls");
		ShowManageUsersMenueOption();
		ReturnToMainMenue();
		break;

	case enMainMenue::eExit:
	
		Login();
		break;
	}
}
void ShowMainMenueOption() {
	system("cls");
	cout << "==================================\n";
	cout << "\tMain Menue\n";
	cout << "==================================\n";
	cout << "\t[1] show client list\n";
	cout << "\t[2] Add New Client\n";
	cout << "\t[3] Delete Client\n";
	cout << "\t[4] Update Client Info\n";
	cout << "\t[5] Find Client\n";
	cout << "\t[6] Transactions\n";
	cout << "\t[7] Manage Users\n";
	cout << "\t[8] Log out\n";
	cout << "==================================\n";
	PerformTheMainMenueOption((enMainMenue)ReadTheMenueOption());
}

bool FindUserByUserNameAndPassword(string UserName, string Password, stUsers& User) {

	vector <stUsers> vUsers = LoadUsersFromFileToVector(UsersFileName);

	for (stUsers &U : vUsers) {

		if (U.UserName == UserName && U.Password == Password) {

			User = U;
			return true;
		}
	}
	return false;
}
bool LoadUserInfo(string UserName, string Password) {

	if (FindUserByUserNameAndPassword(UserName, Password, CurrenUser))
		return true;
	else
		return false;
}

void Login() {

	bool LoginFaild = false;

	string UserName, Password;

	do {
		system("cls");

		cout << ".................................\n";
		cout << "\n\tLogin Screen\n";
		cout << ".................................\n";

		if (LoginFaild) {
			cout << "Invaild UserName/Password!\n";
		}

		cout << "Enter UserName? ";
		cin >> UserName;

		cout << "Enter Password? ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(UserName, Password);

	} while (LoginFaild);

	ShowMainMenueOption();
}

int main() {

	Login();

	system("pause>0");
	return 0;

}