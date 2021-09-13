#include "Client.h"
#include <string.h>
#include <fstream>

void Command_Template()
{
	//시간 동기화 기능은 C++ Client에는 구현되어 있지 않습니다. 
	//Client를 생성합니다. 생성과 동시에 연결 및 인증을 자동으로 수행합니다.
	//인자에는 사전에 서버에 전달한 ID를 입력해주면 됩니다.
	Client_Cpp::Client client((std::string)"ID", (std::string)"IP", (int)"port");

	//이전 턴의 Data를 읽어옵니다. Server에서 보낸 데이터 중 가장 최신 데이터만 읽을 수 있습니다.
	Server2Client::TurnData data;
	client.GetPlayerData(&data);

	//Server에게 전송할 단일 Character에 대한 CommandData를 전송합니다.
	//client.SendUnitCommand((int)"Command_number", Client2Server::CharacterCommand());

	//여러 캐릭터에 대한 CommandData를 전송합니다.
	client.SendCommand((int)"Command_number", Client2Server::Command());

	//가장 먼저 들어온 Feedback을 Dequeue합니다.
	Client_Cpp::Feedback f = client.DequeueFeedback();
	//f.CommandID f가 명령에 대한 Feedback일 경우, feedback에 대응하는 명령의 번호를 나타냅니다. 명령이 아닐 경우 -1
	//f.Type; Client_CSharp.Utility.Constants.S2CFeedbackType 열거형 변수입니다. Feedback의 종류를 담고 있습니다.
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

	std::cout << "ID를 입력해주세요" << std::endl;
	std::cin >> ID;

	/* 원본 ip, id 입력
	
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
		std::cout << "ID를 입력해주세요" << std::endl;
		std::cin >> ID;
	}

	*/




	Client_Cpp::Client client(ID, IP);

	//여기부터 여러분의 AI를 넣으시면 됩니다. (예시 코드는 정보가 들어올 때 마다 랜덤한 행동을 만들어서 전달하는 매우 단순한 코드입니다)

	PoKaWarAIGame::TestCommandGenerator gen(PoKaWarAIGame::TestCommandGenerator::TestCommandSchema::GUNNY);
	long command_count = 1;
	int lastturn = -1;

	std::cout << "Start";
	while (true)
	{
		//TODO: 여러번 플레이할 수 있게 작성(is_end logic)
		if (client.recentpick.ByteSize() > 0)
		{
			client.SendPick(PickData::CharacterType::ROLLER);
			client.recentpick.Clear();
		}

		//Maingame Logic입니다. 각 캐릭터의 시야 정보를 받은 뒤, 랜덤한 명령을 Server에게 전송합니다.
		Server2Client::TurnData dataqueue;
		client.GetPlayerData(&dataqueue);
		if (dataqueue.turn() > lastturn && dataqueue.ByteSize() > 0)
		{
			lastturn = dataqueue.turn();
			for (int i = 0; i < dataqueue.framedata(0).characterdata_size(); i++)
			{
				Server2Client::CharacterData d = dataqueue.framedata(0).characterdata(i);

				//std::cout << "Turn: " << lastturn << ", Sight:" << d.ShortDebugString() << std::endl; (각 캐릭터별 정보 출력)

				gen.SetCharacterData(d, lastturn);

				Client2Server::CharacterCommand cd;
				gen.GetCommandData(d.tag(), &cd);
				cd.set_turn(lastturn + 1);
				client.SendUnitCommand(command_count, &cd);
				command_count++;


				/*
				Client_Cpp::Feedback tfb = client.DequeueFeedback();

				//피드백 데이터를 받아옵니다.
				if (tfb.CommandID != -100) // -1 for 'No Feedback'
				{
					//std::cout << "CommandType: " << (Client_Cpp::Constants::S2CFeedbackType)tfb.Type << std::endl; //(Feedback 정보 출력)
				}
				*/

			}
		}
	}

	//여기까지 Ai가 들어가면 됩니다.
	return 0;
}