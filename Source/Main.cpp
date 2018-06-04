#include"Header.h"
int main()
{
	bool isActive = true;
	char buff[1024];
	//Khỏi tạo winsock
	WORD wversion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wversion, &wsaData) == SOCKET_ERROR){
		cout << "\n Loi khoi tao winsock";
		_getch();
		return 0;
	}
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//Dinh ip
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(21);
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	////Connect tới server 
	if (connect(clientSocket, (SOCKADDR*)&addrServer, sizeof(addrServer)) == SOCKET_ERROR){
		cout << "\n loi connect toi server";
		exitSocket(clientSocket);
	}
	cout << "Khoi Tao Thanh Cong!" << endl;
	// Nhan message tu server
	receive(clientSocket, buff);
	cout << buff << endl;
	cout << "\t\t\t-----------CHUONG TRINH FTP-CLIENT-----------" << endl;
	cout << "Trang Dang Nhap" << endl;
	Login(clientSocket);
	cout << "Press Any Key To Continue" << endl;
	getch();
	bool flag = true;
	while (flag)
	{
		system("CLS");
		cout << "\t\t\t-----------CHUONG TRINH FTP-CLIENT-----------" << endl;
		cout << "\t\t\tCac Chuc Nang Cua Chuong Trinh" << endl;
		cout << "1. Liet ke danh sach cac thu muc, tap tin tren Server (ls)\n"
			"2. Liet ke danh sach cac thu muc, tap tin tren Server (dir)\n"
			"3. Upload mot file den Server (put)\n"
			"4. Download mot file tu Server (get)\n"
			"5. Upload nhieu file den Server (mput)\n"
			"6. Dowload nhieu file tu Server (mget)\n"
			"7. Thay doi duong dan tren Server (cd)\n"
			"8. Thay doi duong dan duoi client (lcd)\n"
			"9. Xoa mot file tren Server (delete)\n"
			"10. Xoa nhieu file tren server (mdelete)\n"
			"11. Tao thu muc tren Server (mkdir)\n"
			"12. Xoa thu muc rong tren Server (rmdir)\n"
			"13. Hien thi duong dan hien tai tren server (pwd)\n"
			"14. Chuyen sang trang thai passive (passive)\n"
			"15. Thoat khoi Server (quit)\n"
			"16. Thoat khoi Server (exit)" << endl;
		cout << "Ban muon chon chuc nang nao: ";
		int key;
		cin >> key;
		vector<char *> name;
		int sl;
		switch (key)
		{
		case 1:
			sendLs(clientSocket, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 2:
			sendDir(clientSocket, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 3:
			cout << "Nhap ten file: ";
			fflush(stdin);
			gets(buff);
			sendPut(clientSocket, buff, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 4:
			cout << "Nhap ten file: ";
			fflush(stdin);
			gets(buff);
			sendGet(clientSocket, buff, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 5:
			cout << "Nhap so luong file: ";
			cin >> sl;
			for (int i = 0; i < sl; i++)
			{
				char *txt = new char;
				cout << "Nhap ten file: ";
				fflush(stdin);
				gets(txt);
				name.push_back(txt);
			}
			for (int i = 0; i < sl; i++)
				cout << name[i] << endl;
			sendMput(clientSocket, name, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			name.empty();
			break;
		case 6:
			cout << "Nhap so luong file: ";
			cin >> sl;
			for (int i = 0; i < sl; i++)
			{
				char *txt = new char;
				cout << "Nhap ten file: ";
				fflush(stdin);
				gets(txt);
				name.push_back(txt);
			}
			sendMget(clientSocket, name, isActive);
			cout << "Press Any Key To Continue" << endl;
			getch();
			name.empty();
			break;
		case 7:
			cout << "Nhap duong dan moi cho server: ";
			fflush(stdin);
			gets(buff);
			sendCd(clientSocket, buff);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 8:
			cout << "Nhap duong dan moi cho client: ";
			fflush(stdin);
			gets(buff);
			sendLcd(clientSocket, buff);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 9:
			cout << "Nhap ten file: ";
			fflush(stdin);
			gets(buff);
			sendDelete(clientSocket, buff);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 10:
			cout << "Nhap so luong file: ";
			cin >> sl;
			for (int i = 0; i < sl; i++)
			{
				char *txt = new char;
				cout << "Nhap ten file: ";
				fflush(stdin);
				gets(txt);
				name.push_back(txt);
			}
			sendMdelete(clientSocket, name);
			name.empty();
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 11:
			cout << "Nhap ten thu muc: ";
			fflush(stdin);
			gets(buff);
			sendMkdir(clientSocket, buff);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 12:
			cout << "Nhap ten thu muc: ";
			fflush(stdin);
			gets(buff);
			sendRmdir(clientSocket, buff);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 13:
			sendPwd(clientSocket);
			cout << "Press Any Key To Continue" << endl;
			getch();
			break;
		case 14:
			sendPasv(clientSocket);
			cout << "Press Any Key To Continue" << endl;
			isActive = false;
			getch();
			break;
		case 15:
			sendQuit(clientSocket);
			cout << "Press Any Key To Continue" << endl;
			getch();
			flag = false;
			break;
		case 16:
			exitSocket(clientSocket);
			cout << "Press Any Key To Continue" << endl;
			getch();
			flag = false;
			break;
		default:
			break;
		}
	}
	getch();
	return 0;

}