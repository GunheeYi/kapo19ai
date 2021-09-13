#include "Client.h"
#include <string.h>
#include <fstream>

void Command_Template()
{
	//�ð� ����ȭ ����� C++ Client���� �����Ǿ� ���� �ʽ��ϴ�. 
	//Client�� �����մϴ�. ������ ���ÿ� ���� �� ������ �ڵ����� �����մϴ�.
	//���ڿ��� ������ ������ ������ ID�� �Է����ָ� �˴ϴ�.
	Client_Cpp::Client client((std::string)"ID", (std::string)"IP", (int)"port");

	//���� ���� Data�� �о�ɴϴ�. Server���� ���� ������ �� ���� �ֽ� �����͸� ���� �� �ֽ��ϴ�.
	Server2Client::TurnData data;
	client.GetPlayerData(&data);

	//Server���� ������ ���� Character�� ���� CommandData�� �����մϴ�.
	//client.SendUnitCommand((int)"Command_number", Client2Server::CharacterCommand());

	//���� ĳ���Ϳ� ���� CommandData�� �����մϴ�.
	client.SendCommand((int)"Command_number", Client2Server::Command());

	//���� ���� ���� Feedback�� Dequeue�մϴ�.
	Client_Cpp::Feedback f = client.DequeueFeedback();
	//f.CommandID f�� ��ɿ� ���� Feedback�� ���, feedback�� �����ϴ� ����� ��ȣ�� ��Ÿ���ϴ�. ����� �ƴ� ��� -1
	//f.Type; Client_CSharp.Utility.Constants.S2CFeedbackType ������ �����Դϴ�. Feedback�� ������ ��� �ֽ��ϴ�.
}


int main()
{
	std::string ID = "", IP = "";
	std::ifstream login("./Login.txt");
	if (login)
	{
		std::string t;
		login >> IP;
	}

	std::cout << "ID�� �Է����ּ���" << std::endl;
	std::cin >> ID;

	/* ���� ip, id �Է�
	
	std::string ID = "", IP = "";
	std::ifstream login("./Login.txt");
	if (login)
	{
		std::string t;
		login >> t;
		ID = t.substr(t.find(":") + 1);
		login >> t;
		IP = t.substr(t.find(":") + 1);
	}
	else
	{
		std::cout << "ID�� �Է����ּ���" << std::endl;
		std::cin >> ID;
	}

	*/




	Client_Cpp::Client client(ID, IP);

	//������� �������� AI�� �����ø� �˴ϴ�. (���� �ڵ�� ������ ���� �� ���� ������ �ൿ�� ���� �����ϴ� �ſ� �ܼ��� �ڵ��Դϴ�)

	PoKaWarAIGame::TestCommandGenerator gen(PoKaWarAIGame::TestCommandGenerator::TestCommandSchema::GUNNY);
	long command_count = 1;
	int lastturn = -1;

	std::cout << "Start";
	while (true)
	{
		//TODO: ������ �÷����� �� �ְ� �ۼ�(is_end logic)
		if (client.recentpick.ByteSize() > 0)
		{
			client.SendPick(PickData::CharacterType::ROLLER);
			client.recentpick.Clear();
		}

		//Maingame Logic�Դϴ�. �� ĳ������ �þ� ������ ���� ��, ������ ����� Server���� �����մϴ�.
		Server2Client::TurnData dataqueue;
		client.GetPlayerData(&dataqueue);
		if (dataqueue.turn() > lastturn && dataqueue.ByteSize() > 0)
		{
			lastturn = dataqueue.turn();
			for (int i = 0; i < dataqueue.framedata(0).characterdata_size(); i++)
			{
				Server2Client::CharacterData d = dataqueue.framedata(0).characterdata(i);

				//std::cout << "Turn: " << lastturn << ", Sight:" << d.ShortDebugString() << std::endl; (�� ĳ���ͺ� ���� ���)

				gen.SetCharacterData(d, lastturn);

				Client2Server::CharacterCommand cd;
				gen.GetCommandData(d.tag(), &cd);
				cd.set_turn(lastturn + 1);
				client.SendUnitCommand(command_count, &cd);
				command_count++;


				/*
				Client_Cpp::Feedback tfb = client.DequeueFeedback();

				//�ǵ�� �����͸� �޾ƿɴϴ�.
				if (tfb.CommandID != -100) // -1 for 'No Feedback'
				{
					//std::cout << "CommandType: " << (Client_Cpp::Constants::S2CFeedbackType)tfb.Type << std::endl; //(Feedback ���� ���)
				}
				*/

			}
		}
	}

	//������� Ai�� ���� �˴ϴ�.
	return 0;
}