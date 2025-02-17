#include"game.h"

game::game() {

}

void game::intro()
{
	cout << setw(25) << "====================================" << endl;
	cout << setw(25) << "|          Hogwarts School         |" << endl;
	cout << setw(25) << "|    Of Witchcraft and Wizardry    |" << endl;
	cout << setw(25) << "|                                  |" << endl;
	cout << setw(25) << "| We're pleased to imform you that |" << endl;
	cout << setw(25) << "|   you have a place at Hogwarts.  |" << endl;
	cout << setw(25) << "|    You'll be traveling to the    |" << endl;
	cout << setw(25) << "| Wizarding World of Harry Potter. |" << endl;
	cout << setw(25) << "|         Enjoy your trip!         |" << endl;
	cout << setw(25) << "====================================" << endl;
	system("pause");
	system("cls");
}

void game::task()
{
	cout << setw(25) << "==================================================" << endl;
	cout << setw(25) << "|             Task for the semester:             |" << endl;
	cout << setw(25) << "|              Defeat Lord Voldemort             |" << endl;
	cout << setw(25) << "|                                                |" << endl;
	cout << setw(25) << "|      You might encounter enemies at school.    |" << endl;
	cout << setw(25) << "|   Defeat them wins you money and experience,   |" << endl;
	cout << setw(25) << "|   which might help you fight with Voldemort.   |" << endl;
	cout << setw(25) << "|  Whenever you encounter an enemy, you can ask  |" << endl;
	cout << setw(25) << "|  other wizard for help. If you lose all your   |" << endl;
	cout << setw(25) << "|   hp during the battle, you lose the game.     |" << endl;
	cout << setw(25) << "|  The Ministery of Magic will send you back to  |" << endl;
	cout << setw(25) << "|     your house and delete all your memory.     |" << endl;
	cout << setw(25) << "|                   Good luck:)                  |" << endl;
	cout << setw(25) << "==================================================" << endl;
	system("pause");
	system("cls");
}

void game::run()
{
	cout << "Would you like to continue the game from your previous save?" << endl;
	cout << "Press 'y' for yes / Press others for no" << endl;
	cout << "Please choose:";
	char a;
	cin >> a;

	if (a == 'y' || a == 'Y') {
		ifstream in("Player.txt", ios::in); //資料夾名稱common/加檔名
		ifstream in2("save_map.txt", ios::in);
		ifstream in3("Player_in_map.txt", ios::in);
		if (!in.is_open() || !in2.is_open() || !in3.is_open())
		{
			cerr << "Sorry, we have problems on opening the saving files :(" << endl;
			exit(1);
		}
		else {
			system("cls");
			string player;
			in >> player;
			NovicePlayer* me = me->unserialize(player);
			string save, temp, save_map_name, curr_x, curr_y;
			in3 >> save;
			stringstream ss(save);
			getline(ss, temp, '$');
			save_map_name = temp;
			temp.clear();
			getline(ss, temp, '$');
			curr_x = temp;
			temp.clear();
			getline(ss, temp, '$');
			curr_y = temp;
			temp.clear();

			//確認遊戲進度
			if (save_map_name == "The_Great_Hall") {
				Field f("save_map.txt", stoi(curr_x), stoi(curr_y), 7, 7, me, "The_Great_Hall");
				f.display();
				char c;
				while (c = _getch()) {
					f.move(c);
					if (f.leave) {
						break;
					}
				}

				Field f2("The_Quidditch_Pitch.txt", 3, 3, 7, 7, me);
				f2.display();
				while (c = _getch()) {
					f2.move(c);
					if (f2.leave) {
						break;
					}
				}

				Field f3("Forbidden_Forest.txt", 3, 3, 7, 7, me);
				f3.display();
				while (c = _getch()) {
					f3.move(c);
					if (f3.leave) {
						break;
					}
				}
			}
			else if (save_map_name == "The_Quidditch_Pitch") {
				Field f("save_map.txt", stoi(curr_x), stoi(curr_y), 7, 7, me, "The_Quidditch_Pitch");
				f.display();
				char c;
				while (c = _getch()) {
					f.move(c);
					if (f.leave) {
						break;
					}
				}

				Field f3("Forbidden_Forest.txt", 3, 3, 7, 7, me);
				f3.display();
				while (c = _getch()) {
					f3.move(c);
					if (f3.leave) {
						break;
					}
				}
			}
			else if (save_map_name == "Forbidden_Forest") {
				Field f("save_map.txt", stoi(curr_x), stoi(curr_y), 7, 7, me, "Forbidden_Forest");
				f.display();
				char c;
				while (c = _getch()) {
					f.move(c);
					if (f.leave) {
						break;
					}
				}
			}
		}

	}
	else {
		system("cls");
		//簡介遊戲背景和取名
		intro();
		string s;
		cout << "What's your name:";
		cin >> s;
		system("pause");
		system("cls");

		//歡迎和說明遊戲規則
		NovicePlayer me(1, s);
		cout << "Welcome to Hogwarts School, " << s << "!" << endl;
		task();

		Field f1("The_Great_Hall.txt", 3, 3, 7, 7, &me);
		f1.display();

		char c;
		while (c = _getch()) {
			f1.move(c);
			if (f1.leave) {
				break;
			}
		}

		Field f2("The_Quidditch_Pitch.txt", 3, 3, 7, 7, &me);
		f2.display();
		while (c = _getch()) {
			f2.move(c);
			if (f2.leave) {
				break;
			}
		}

		Field f3("Forbidden_Forest.txt", 3, 3, 7, 7, &me);
		f3.display();
		while (c = _getch()) {
			f3.move(c);
			if (f3.leave) {
				break;
			}
		}
	}
}