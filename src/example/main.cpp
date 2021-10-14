#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <QApplication>
#include <QHostAddress>
#include <QMessageBox>

#include <sys/socket.h> // struct sockaddr
#include <sys/un.h> // struct sockaddr_un
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr_in6

void doTest()
{
	QMessageBox mbox;
	QHostAddress qaddr;
	union {
		struct sockaddr addr;
		char _[128];
	} _;

	auto doMsg = [&](){
		qaddr.setAddress(&_.addr);
		mbox.setText(qaddr.toString());
		mbox.exec();
	};
	auto reset = [&](){
		memset(&_, 0, sizeof(_));
	};

	{
		// unix socket to a concrete path
		reset();
		_.addr.sa_family = AF_UNIX;
		auto &unaddr = reinterpret_cast<struct sockaddr_un&>(_.addr);
		strcpy(unaddr.sun_path, "/path/to/socket");
		doMsg(); // empty string!
	}
	{
		// unix socket to an abstract socket
		reset();
		_.addr.sa_family = AF_UNIX;
		auto &unaddr = reinterpret_cast<struct sockaddr_un&>(_.addr);
		unaddr.sun_path[0] = '\0';
		strcpy(unaddr.sun_path+1, "abstract socket");
		doMsg(); // empty string!
	}
	{
		// ipv4
		reset();
		_.addr.sa_family = AF_INET;
		auto &v4addr = reinterpret_cast<struct sockaddr_in&>(_.addr);
		v4addr.sin_port = htons(12345);
		v4addr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
		doMsg(); // 127.0.0.1
	}
	{
		// ipv6
		reset();
		_.addr.sa_family = AF_INET6;
		auto &v6addr = reinterpret_cast<struct sockaddr_in6&>(_.addr);
		v6addr.sin6_port = htons(12345);
		v6addr.sin6_flowinfo = 0; // blah blah
		v6addr.sin6_addr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}; // ::1
		v6addr.sin6_scope_id = 0; // link interface id
		doMsg(); // ::1
	}
}

int main(
	int argc,
	char ** argv
)
{
	try
	{
		QApplication app{argc, argv};

		// Run unit tests...
		doTest();

		std::cout << "Test done!" << std::endl;

		// UnitTest::cleanup()
		app.processEvents();
		return EXIT_SUCCESS; // app.exec();
	}
	catch (const std::exception &e)
	{
		std::cerr << "uncaught exception: " << e.what();
		return EXIT_FAILURE;
	}
}
