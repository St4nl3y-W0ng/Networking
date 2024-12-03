#include <nw_net.h>

enum class CustomMsgTypes : uint32_t 
{
	FireBullet,
	MovePlayer
};

class CustomClient : public nw::net::client_interface<CustomMsgTypes>
{
public:
	bool FireBullet(float x, float y)
	{
		nw::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::FireBullet;
		msg << x << y;
		Send(msg);
	}
};

int main() 
{

	CustomClient c;
	c.Connect("https://www.google.com/", 60000);
	c.FireBullet(2.0f, 5.0f);

	nw::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::FireBullet;

	int a = 1;
	bool b = true;
	float c = 3.14159f;

	struct 
	{
		float x;
		float y;
	} d[5];

	msg << a << b << c << d;

	a = 99;
	b = false;
	c = 99.0f;

	msg >> d >> c >> b >> a;

	return 0;
}