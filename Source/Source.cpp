#include"Header.h"
//Hàm nhận thông tin trả về từ server
void receive(SOCKET s, char *buff)
{
	int len;
	len = recv(s, buff, 1024, 0);
	buff[len] = '\0';
}
//Hàm chuyển mà trả về từ dạng chuổi sang dạng số nguyên từ server
int getNumber(char *buff)
{
	char num[1024];
	for (int i = 0; i < strlen(buff); i++)
	{
		// ma ascii cua so tu 48->57
		if (buff[i] < 48 || buff[i] > 57)
		{
			num[i] = '\0';
			break;
		}
		num[i] = buff[i];
	}
	return atoi(num);
}
//Hàm đăng nhập vào server
int Login(SOCKET s)
{
	char user[1024];
	char pass[1024];
	char temp[1024];
	while (1)
	{
		//Nhập tên đăng nhập
		cout << "Enter Username: ";
		gets(temp);
		strcpy(user, "USER ");
		strcat(temp, "\n\0");
		strcat(user, temp);
		//Gửi yêu cầu nhập tên cho server
		send(s, user, strlen(user), 0);
		receive(s, temp);
		// Nếu mã nhận về là = 331 thì thành công 
		int num = getNumber(temp);
		if (num != 331)
		{
			cout << "Something went wrong !" << endl;
			return -1;
		}
		//Nhập mật khẩu
		cout << "Enter Password: ";
		gets(temp);
		strcpy(pass, "PASS ");
		strcat(temp, "\n\0");
		strcat(pass, temp);
		//Gửi yêu cầu nhập mật khẩu đến sever
		send(s, pass, strlen(pass), 0);
		//Nhận thông báo từ sever
		receive(s, temp);
		//cout << temp<<endl;
		//Mã nhận có dạng chuổi -> chuyển sang dạng số nguyên
		num = getNumber(temp);
		// Nếu mã nhận về là = 230 thì  đăng nhập thành công 
		if (num == 230)
		{
			cout << "Sucess!" << endl;
			cout << temp;
			return 1;
		}
		//Không thì thất bại
		cout << "Incorrect username or password,please try again!" << endl;

	}
}
//Hàm gửi port để tạo thêm một kênh dùng để put và get dữ liệu từ server
SOCKET sendPort(SOCKET s, int port)
{
	char buff1[1024];
	//char buff1[1024];
	SOCKET s1 = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock.sin_port = htons(port);
	//Kết socket s1 với server
	bind(s1, (sockaddr*)&sock, sizeof(sock));
	//Chờ nhận kết nối từ host đối tác (server chờ các client)
	listen(s1, 3);
	//kích thước tối đa của hàng đợi thiết lập kết nối. Giá trị tối đa được chỉ định bằng hằng số
	//Nếu  port bằng 0 thì tiến hành lấy random_port
	if (port == 0) {
		sockaddr_in srv;
		int len = sizeof(srv);
		getsockname(s1, (sockaddr *)&srv, &len);
		port = ntohs(srv.sin_port);
	}
	//buff có dạng h1,h2,h3,h4,p1,p2
	//gửi lệnh PORT cho server 
	//Trong đó  server's IP address : h1.h2.h3.h4 và  TCP port_number là: p1*256+p2
	sprintf(buff1, "PORT %d,%d,%d,%d,%d,%d\r\n\0", sock.sin_addr.s_net, sock.sin_addr.s_host,
		sock.sin_addr.s_lh, sock.sin_addr.s_impno, port / 256, port % 256);
	send(s, buff1, strlen(buff1), 0);
	receive(s, buff1);
	cout << buff1;
	return s1;
}
//Liệt kê được danh sách các thư mục trên Server (ls)
void sendLs(SOCKET s, bool isActive)
{
	SOCKET s1;
	char buff[1024];
	strcpy(buff, "NLST \n\0");
	if (isActive)//Active
	{
		SOCKET s2 = sendPort(s, 0);
		SOCKADDR_IN server;
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
		int len = sizeof(server);
		s1 = accept(s2, (sockaddr*)&server, &len);
		closesocket(s2);
	}
	else
	{
		s1 = sendPasv(s);
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
	}
	receive(s1, buff);
	printf("%s", buff);
	closesocket(s1);
	receive(s, buff);
	cout << buff;
}
//Liệt kê được danh sách các tập tin trên Server(dir)
void sendDir(SOCKET s, bool isActive)
{
	
	SOCKET s1;
	char buff[4068];
	strcpy(buff, "LIST \n\0");
	if (isActive)
	{
		SOCKET s2 = sendPort(s, 0);
		SOCKADDR_IN server;
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
		int len = sizeof(server);
		s1 = accept(s2, (sockaddr*)&server, &len);
		closesocket(s2);
	}
	else
	{
		s1 = sendPasv(s);
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
	}
	receive(s1, buff);
	printf("%s", buff);
	closesocket(s1);
	receive(s, buff);
	cout << buff;
}
//Upload một file đến Server (put)
void sendPut(SOCKET s, char*name, bool isActive)
{
	SOCKET s1;
	FILE *f = fopen(name, "rb");
	if (!f){
		cout << "Not exist file!" << endl;
		return;
	}
	char buff[1024];
	strcpy(buff, "STOR ");
	strcat(buff, name);
	strcat(buff, " \n\0");
	if (isActive)
	{
		SOCKET s2 = sendPort(s, 0);
		SOCKADDR_IN server;
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
		int len = sizeof(server);
		s1 = accept(s2, (sockaddr*)&server, &len);
		closesocket(s2);
	}
	else
	{
		s1 = sendPasv(s);
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
	}
	// tien hanh send file
	//send 1 luc 1024 byte
	//Doc size file
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	
	rewind(f);
	for (int i = 0; i < size / 1024; i++)
	{
		fread(buff, 1, 1024, f);
		send(s1, buff, 1024, 0);
	}
	
	int	du = size % 1024;
	fread(buff, 1, du, f);
	send(s1, buff, du, 0);
	
	fclose(f);
	closesocket(s1);
	receive(s, buff);
	cout << buff << endl;
}
//Download một file từ Server (get)
void sendGet(SOCKET s, char *name, bool isActive)
{
	SOCKET s1;
	char buff[1024];
	strcpy(buff, "RETR ");
	strcat(buff, name);
	strcat(buff, " \n\0");
	FILE *f = fopen(name, "wb");
	if (!f){
		cout << "Not exist file!" << endl;
		return;
	}
	if (isActive)
	{
		SOCKET s2 = sendPort(s, 0);
		SOCKADDR_IN server;
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		// 550 = file not found
		cout << buff;
		if (getNumber(buff) == 550)
		{
			return;
		}
		int len = sizeof(server);
		s1 = accept(s2, (sockaddr*)&server, &len);
		closesocket(s2);
	}
	else
	{
		s1 = sendPasv(s);
		send(s, buff, strlen(buff), 0);
		receive(s, buff);
		cout << buff;
		if (getNumber(buff) == 550)
		{
			return;
		}
	}
	// tien hanh get file
	//get 1 luc 1024 byte
	int size;
	while ((size = recv(s1, buff, 1024, 0)) > 0) {
		fwrite(buff, 1, size, f);
	}

	fclose(f);
	closesocket(s1);
	receive(s, buff);
	cout << buff << endl;
}
//Upload nhiều file đến Server(mput)
void sendMput(SOCKET s, vector<char*> name, bool isActive)
{
	int count = name.size();
	for (int i = 0; i < count; i++)
		sendPut(s, name[i], isActive);
}
//Dowload nhiều file từ Server (mget)
void sendMget(SOCKET s, vector<char*> name, bool isActive)
{
	int count = name.size();
	for (int i = 0; i < count; i++)
		sendGet(s, name[i], isActive);
}
//Thay đổi đường dẫn trên Server(cd)
void sendCd(SOCKET s, char *name)
{
	char buff[1024];
	strcpy(buff, "CWD ");
	strcat(buff, name);
	strcat(buff, "\n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
//Thay đổi đường dẫn dưới client(lcd)
void sendLcd(SOCKET s, char *name)
{
	int t = _chdir(name);// thay doi 
	if (t != 0)
	{
		cout << "Path not exists!";
		return;
	}
	cout << "Succesfull!" << endl;
	// << "Current Path"
	cout << "Now The Current Working Director is: ";
	system("cd");
}
//Xóa một file trên Server (delete)
void sendDelete(SOCKET s, char *name)
{
	char buff[1024];
	strcpy(buff, "DELE ");
	strcat(buff, name);
	strcat(buff, "\n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
//Xoá nhiều file trên server (mdelete)
void sendMdelete(SOCKET s, vector<char*> name)
{
	for (int i = 0; i < name.size(); i++)
		sendDelete(s, name[i]);
}
//Tạo thư mục trên Server (mkdir)
void sendMkdir(SOCKET s, char *name)
{
	char buff[1024];
	strcpy(buff, "XMKD ");
	strcat(buff, name);
	strcat(buff, "\n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
//Chuyển sang trang thái passive (passive)
SOCKET sendPasv(SOCKET s)
{
	SOCKET s1;
	char buff[1024];
	strcpy(buff, "PASV \n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
	//Buff gửi về có dạng : (h1,h2,h3,h4,p1,p2)
	//Bốc tách để lấy port
	// port = p1*256 + p2
	//Tiến hành bốc tách
	int port;
	int arr[2] = { 0 };
	int count = 1;
	int sl = 1;
	for (int i = strlen(buff) - 4; i >= 0; i--)//bo )\r\n o cuoi chuoi
	{
		if (buff[i] != ',')
		{
			int so = buff[i] - 48;
			arr[count] += sl*so;
			sl *= 10;
		}
		else
		{
			count--;
			sl = 1;
			if (count < 0) break;
		}
	}
	port = arr[0] * 256 + arr[1];
	cout << port << endl;
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	s1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(s1, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR){
		cout << "\n Loi connect toi server";
		exitSocket(s1);
		return NULL;
	}
	cout << "Success!!! Ban dang o che do PASSIVE!" << endl;
	return s1;
}
//Xóa thư mục rỗng trên Server(rmdir)
void sendRmdir(SOCKET s, char *name)
{
	char buff[1024];
	strcpy(buff, "XRMD ");
	strcat(buff, name);
	strcat(buff, "\n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
//Hiển thị đường dẫn hiện tại trên server(pwd)
void sendPwd(SOCKET s)
{
	char buff[1024];
	strcpy(buff, "XPWD \n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
//Thoát khỏi Server (quit)
void sendQuit(SOCKET s)
{
	char buff[1024];
	strcpy(buff, "QUIT \n\0");
	send(s, buff, strlen(buff), 0);
	receive(s, buff);
	cout << buff;
}
void exitSocket(SOCKET s)
{
	closesocket(s);
	WSACleanup();
	_getch();
	return;
}